//Written by Chris Sheil 10-1-18 for Systems Programming Rutgers University
void MergeSort(record** headPTR, int id){
  record* head=*headPTR;
  record* a;
  record* b;

  //List is sorted
  if((head==NULL)||(head->next==NULL)){
    return;
  }

  //split the list
  split_list(head, &a, &b);

  //recursively sort the sublists
  MergeSort(&a, id);
  MergeSort(&b, id);

  *headPTR= sort_list(a, b, id);
}

record* sort_list(record* a, record* b, int id){
  record* result= NULL;
  char* duplicateA, *duplicateB;
  //Base Case
  if(a==NULL){
    return b;
  }else if(b==NULL){
    return a;
  }

  //duplicate the entries so orignial data is not changed
  duplicateA=strdup(a->fields[id]);
  duplicateB=strdup(b->fields[id]);
  //remove quotes and trim whitespeace
  duplicateA=prep_data(duplicateA);
  duplicateB=prep_data(duplicateB);

  //sort on string if string sort on int if int
  if(id==2 || id==3 || id==4 || id==5 || id==7 || id==8 || id==12 || id==13 || id==15 || id==18 || id>=22){
    if(atof(duplicateA) <= atof(duplicateB)){
      result= a;
      result->next = sort_list(a->next, b, id);
    }else{
      result= b;
      result->next= sort_list(a, b->next, id);
    }
  }else{
    if(strcmp(duplicateA, duplicateB)<=0){
      result= a;
      result->next = sort_list(a->next, b, id);
    }else{
      result= b;
      result->next= sort_list(a, b->next, id);
    }
  }

  return result;
}

void split_list(record* source, record** frontRef, record** backRef){
  //just splits list in half to recursively sort on each half
  record* fast=NULL;
  record* slow=NULL;
  slow=source;
  fast=source->next;

  while(fast!=NULL){
    fast=fast->next;
    if(fast!=NULL){
      slow=slow->next;
      fast=fast->next;
    }
  }

  *frontRef= source;
  *backRef= slow->next;
  slow->next=NULL;
}
