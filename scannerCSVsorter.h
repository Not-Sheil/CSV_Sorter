/*****
*
*	Define structures and function prototypes for your sorter
*
*
*
******/
//Suggestion: define a struct that mirrors a record (row) of the data set

//Written by Chris Sheil 10-1-18 for Systems Programming Rutgers University
#include <stdio.h>
//This is our "Node" structure with is a record. This represents one entry in the CSV
typedef struct record_{
    char* fields[28];
    struct record_* next;
    struct record_* copy;
}record;

//this is an array used for comparing the input string to the field name that we will be sorting on
char* fieldID[28]={
  "color","director_name","num_critic_for_reviews","duration","director_facebook_likes","actor_3_facebook_likes","actor_2_name","actor_1_facebook_likes","gross",
  "genres","actor_1_name","movie_title","num_voted_users","cast_total_facebook_likes","actor_3_name","facenumber_in_poster","plot_keywords"
  ,"movie_imbd_link","num_user_for_reviews","language","country","content_rating","budget","title_year","actor_2_facebook_likes"
  ,"imbd_score","aspect_ratio","movie_facebook_likes"
};


//Suggestion: prototype a mergesort function

//standard mergesort on a linked list
record* sort_list(record*  a, record* b, int id);
void split_list(record* source, record** frontRef, record** backRef);
void MergeSort(record** headRef, int id);

/*This function will take in the file from STDIN and build an unsorted linked list
  the linked list will be of type record where record is one row of the original CSV.
  returns a pointer to the first record*/
   record* buildList(FILE* CSVfile);


/*Function to print the sorted liked list.
  We manually save the first line from the CSV with all the column names in it.
  We print that line frist then iterate through the list printing each record*/
void print_results(record* head,char* first_row, int id);

/*Simple helper function to get the numeric ID assosiated with the field name
  returns the ID as an int*/
int get_fieldID(char* field_name);

/*Helper Function that takes a string and trims the whitespeaces at the beginning
  and end of the string*/
char *trimwhitespace(char *str);

/*Preps the data to tokenize correctly: This means finding any movie titles
  with commas in them and changing them to an @ symbol*/
char* prep_tokenize(char* line);

/*Coupled with prep_tokenize which makes all ',' in movie_titles a '@'
  This function changes it back to a comma so the data is not mutated at all
  but has tokenized correctly*/
char* switch_back(char* line);

/*This preps the data for sorting: This means making a duplicate of the string
  then removing quotes followed by the whitespaces so it sorts correctly while
  also not changing the original data*/
char* prep_data(char* line);

/* Just a function to correctly print to a file*/
void printList(char**argv, record* head, char* file_path_name, char* first_line, int id);

/*This is my file handler, after any file is found in the directory structure, it is sent here where it is checked for validity before sending to our sorting method then to out printing method.*/
void fileHandler(int argc, char**argv, FILE *csv_file, char *file_name);
