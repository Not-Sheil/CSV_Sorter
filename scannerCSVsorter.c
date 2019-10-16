#include "scannerCSVsorter.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "mergesort.c"
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
//Written by Chris Sheil 10-1-18 for Systems Programming Rutgers University

/*README!!!! All function descriptions are in the
  header file alongside the respective function prototype*/

record* buildList(FILE* CSVfile){
  //declarations
  char rowBuffer[1000];
  char* buffer_copy=NULL;
  char* token[28];
  char* temp=NULL;
  record* new_original=NULL;
  record* head=NULL;
  record* prev_record=NULL;


  memset(rowBuffer, 0, sizeof(rowBuffer));
  //preprocesssing the whole file row by row and making a record struct
  int rowcount=0;
  while(fgets(rowBuffer, 1000, CSVfile)!= NULL){

    buffer_copy= strdup(rowBuffer);

    buffer_copy=prep_tokenize(buffer_copy);

    //tokenizing a single row from input CSV

    int i=0,run_once;
    if(run_once!=1){
      while((temp=strsep(&buffer_copy,","))!=NULL){
        if(i==28){
          fprintf(stderr, "WARNING: Incorrect file structure in one or more CSV's\n");
          run_once=1;
        }
        token[i]=temp;
        i++;
      }
    }

    //manually create the record strucure
    new_original= malloc(sizeof(record));

    new_original->fields[0] = token[0];
    new_original->fields[1] = token[1];
    new_original->fields[2] = token[2];
    new_original->fields[3] = token[3];
    new_original->fields[4] = token[4];
    new_original->fields[5] = token[5];
    new_original->fields[6] = token[6];
    new_original->fields[7] = token[7];
    new_original->fields[8] = token[8];
    new_original->fields[9] = token[9];
    new_original->fields[10] = token[10];
    new_original->fields[11] = token[11];
    new_original->fields[12] = token[12];
    new_original->fields[13] = token[13];
    new_original->fields[14] = token[14];
    new_original->fields[15] = token[15];
    new_original->fields[16] = token[16];
    new_original->fields[17] = token[17];
    new_original->fields[18] = token[18];
    new_original->fields[19] = token[19];
    new_original->fields[20] = token[20];
    new_original->fields[21] = token[21];
    new_original->fields[22] = token[22];
    new_original->fields[23] = token[23];
    new_original->fields[24] = token[24];
    new_original->fields[25] = token[25];
    new_original->fields[26] = token[26];
    new_original->fields[27] = token[27];
    new_original->next=NULL;

    //swtich back the '@' to a ',' so the data is NON mutated
    new_original->fields[11]=switch_back(new_original->fields[11]);

    //stitching the linked list
    if(rowcount==0){
      head=new_original;
      prev_record=new_original;
    }else{
      prev_record->next=new_original;
      prev_record=new_original;
    }
    rowcount++;
    memset(rowBuffer, 0, sizeof(rowBuffer));
  }
  return head;
}

int get_fieldID(char* field_name){
  int i;
  for(i=0;i<28;i++){
    if(strcmp(field_name, fieldID[i])==0){
      return i;
    }
  }
  //if no match return -1. This means there was an error!
  return -1;
}

char* prep_tokenize(char* line){
  int i;
  for(i=0; i<strlen(line); i++){
    if(line[i]=='"'){
      i++;
      while(line[i]!='"'){
        if(line[i]==','){
          line[i]='^';
        }
        i++;
      }
    }
  }
  return line;
}

char *trimwhitespace(char *str)
{
  char *end;

  // Trim leading space
  while(isspace((unsigned char)*str)) str++;

  if(*str == 0)  // All spaces?
    return str;

  // Trim trailing space
  end = str + strlen(str) - 1;
  while(end > str && isspace((unsigned char)*end)) end--;

  // Write new null terminator character
  end[1] = '\0';

  return str;
}

char* switch_back(char* line){
  int i;
  for(i=0;i<strlen(line);i++){
    if(line[i]=='^'){
      line[i]=',';
    }
  }
  return line;
}

char* prep_data(char* line){
  int i;
  for(i=0;i<strlen(line);i++){
    if(line[i]=='"'){
      line[i]=' ';
    }
  }
  line=trimwhitespace(line);
  return line;
}

void fileHandler(int argc, char**argv, FILE *csv_file, char *file_name){
  if(csv_file==NULL){
    fprintf(stderr, "NULL file. EXITING...\n");
    printf("NULL file. EXITING...\n");
    exit(EXIT_FAILURE);
  }

  if(strstr(file_name,".csv")!=NULL){
    if(strstr(file_name,"-sorted-")!=NULL){
      fprintf(stderr, "FLAGGING: Sorted file %s found in CSV handler.\n",file_name);
      exit(1);
    }
  }else{
    fprintf(stderr, "FLAGGING: Non CSV file %s found in CSV handler.\n",file_name);
    exit(1);
  }

  int outputSpecified=0;
  int j,k;
  int output_index=0, sorting_index=0;
  for(j=0;j<argc;j++){
    if(strcmp(argv[j], "-o")==0){
      outputSpecified=1;
      output_index=j+1;
    }
  }

  int coloumnSpecified=0;
  for(k=0;k<argc;k++){
    if(strcmp(argv[k],"-c")==0){
      coloumnSpecified=1;
      sorting_index=k+1;
    }
  }

  char file_path[500];
  if(outputSpecified){
    strcpy(file_path, argv[output_index]);
    mkdir(argv[output_index], 0700);
  }else{
    int directorySpecified=0, directory_index;
    int p;
    for(p=0;p<argc;p++){
      if(strcmp(argv[p], "-d")==0){
        directorySpecified=1;
        directory_index=p+1;
      }
    }
    if(directorySpecified){
      strcpy(file_path,argv[directory_index]);
    }else{
      char currentDirectory[_POSIX_PATH_MAX]={0};
      getcwd(currentDirectory,255);
      strcpy(file_path,currentDirectory);
    }
  }

  //manually save the first line of the file to explicitly print first
  char first_line[1000];
  fgets(first_line,1000, csv_file);
  record* start=buildList(csv_file);
  int id;

  if(coloumnSpecified==0){
    fprintf(stderr,"No sorting flag: -c. EXITING...\n");
    printf("No sorting flag: -c. Exiting...\n");
    exit(EXIT_FAILURE);
  }else{
    id=get_fieldID(argv[sorting_index]);
  }

  //sort the list
  MergeSort(&start, id);

  char* buffer=(char*)malloc(sizeof(char)*40);
  int i;
  for(i=0;i<strlen(file_name);i++){
    if(file_name[i]=='.'){
      break;
    }
    buffer[i]=file_name[i];
  }
  strcat(file_path,"/");
  strcat(file_path,buffer);
  strcat(file_path,"-sorted-");
  strcat(file_path,argv[sorting_index]);
  strcat(file_path,".csv");

  printList(argv, start, file_path, first_line, id);

}

void printList(char**argv, record* head, char* file_path_name, char* first_line, int id){

  FILE* f;
  f=fopen(file_path_name, "w");
  fprintf(f, first_line);
  while(head!=NULL){

    if(head->fields[id][0]!='\0'){
      int i;
      for(i=0;i<28;i++){
        if(i==27){
          fprintf(f,"%s", head->fields[i]);
        }else{
          fprintf(f,"%s,", head->fields[i]);
        }
      }
    }
    head=head->next;
  }
}

int main(int argc, char** argv){

  //Invalid input checks
  if(argc<2 || argc>7){
    fprintf(stderr, "Invalid Arguments. EXITING...\n");
    printf("Invalid Arguments. EXITING...\n");
    exit(EXIT_FAILURE);
  }

  int k;
  int coloumnSpecified=0, sorting_index;
  for(k=0;k<argc;k++){
    if(strcmp(argv[k],"-c")==0){
      coloumnSpecified=1;
      if(argc==2){
        fprintf(stderr, "Sorting Coloumn Not specified. EXITING...\n");
        printf("Sorting Coloumn Not specified. Exiting...\n");
        exit(EXIT_FAILURE);
      }else{
        sorting_index=k+1;
        printf("%d\n", get_fieldID(argv[sorting_index]));
        if(get_fieldID(argv[sorting_index])==-1){
          fprintf(stderr,"WARNING: Sorting coloumn not in CSV structure, expecting valid arg after -c __\n");
        }
      }
    }
  }
  if(!coloumnSpecified){
    fprintf(stderr,"No sorting flag -c. EXITING...\n");
    printf("No sorting flag -c. EXITING...\n");
    exit(EXIT_FAILURE);
  }

  int idLength=0; //keeping track of the amount of processes
  int *idList = (int*)(malloc(sizeof(int)));
  int initial_pid = getpid();
  int status;
  char buf[_POSIX_PATH_MAX] = {0};
  char currentDirectory[_POSIX_PATH_MAX]= {0};
  idList[0]= initial_pid;
  char *path= NULL;
  struct dirent *entry;
  DIR *directory;
  int directorySpecified=0, directory_index;
  int p;
  //check command line arguments to see if there is a -d flag
  for(p=0;p<argc;p++){
    if(strcmp(argv[p], "-d")==0){
      directorySpecified=1;
      directory_index=p+1;
    }
  }

  //if no specified directory use the one we are in now.
  if(!directorySpecified){
    getcwd(currentDirectory, 255);
    directory= opendir(currentDirectory);
  }else{
    strcat(currentDirectory, argv[directory_index]);
    directory= opendir(argv[directory_index]);
  }
  //check to make sure it is a directory
  if(directory == NULL){
    fprintf(stderr,"Invalid Pathway. EXITING...\n");
    printf("Invalid Pathway. EXITING...\n");
    exit(EXIT_FAILURE);
  }

  printf("Metadata Summary\n");
  printf("_____________________________________________\n");
  printf("Initial PID: %d\n", initial_pid);
  printf("PID for all child processes: ");
  fflush(stdout);

  //Traverse directory
  while((entry=readdir(directory))!= NULL){

    struct stat s;
    if(fstatat(dirfd(directory), entry->d_name,&s,0)==0){
      //check for dot and dot dot.
      if(strcmp(entry->d_name,".")==0 || strcmp(entry->d_name, "..")==0){
        continue;
      }
      if(S_ISREG(s.st_mode)){//if its a file
        int pid=fork();
        switch(pid){
          FILE *csv_file;
          case 0: //child
            strcat(currentDirectory, "/");
            strcat(currentDirectory, entry->d_name);
            path=realpath(currentDirectory, buf);
            csv_file=fopen(buf, "r");
            fileHandler(argc, argv, csv_file, entry->d_name);
            exit(1);//IMPORTANT

          case -1: // error in forking
            fprintf(stderr,"Fork Unsuccessful. EXITING...\n");
            printf("Fork Unsuccessful. EXITING...\n");
            exit(EXIT_FAILURE);

          default: //parent process
            printf("%d,",pid);
            fflush(stdout);
            waitpid(pid,NULL,0);
            idLength++;
            break;
          }continue;
      }
      if(S_ISDIR(s.st_mode)){//if it is a directory
        int pid = fork();

        switch(pid){
          case 0:
            idLength=0;
            strcat(currentDirectory,"/");
            strcat(currentDirectory,entry->d_name);
            path=realpath(currentDirectory,buf);
            directory=opendir(buf);
            break;

          default:
            printf("%d,",pid);
            fflush(stdout);
            waitpid(pid,&status,0);
            idLength++;
            idLength=idLength+ WEXITSTATUS(status);
            break;
        }
        continue;
      }
    }//end of directory
  }//end of directory structure

  if(getpid()!=initial_pid){
    exit(idLength);
  }
  printf("\nTotal number of processes: %d\n", idLength);
  printf("______________________________________________\n");

  //END OF PROGRAM





  //success!!!
  return 0;
}
