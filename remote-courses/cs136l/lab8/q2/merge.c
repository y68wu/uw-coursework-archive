#include "doc.h"
#include "merge.h"
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>

struct document *merge(struct document *doc1, struct document *doc2){
    // could have asserted doc1 but do not know if students
    // will end up ensuring preconditions are met.
    // It is unfortunate that we will need to return NULL.
    if (doc1 == NULL){
      printf("Document 1 is null. Not merging.\n");
      return NULL;
    }
    // could have asserted doc2 but do not know if students
    // will end up ensuring preconditions are met.
    // It is unfortunate that we will need to return NULL.
    if (doc2 == NULL){
      printf("Document 2 is null. Not merging.\n");
      return NULL;
    }

  struct document *toReturn = create_document();
  
  if (doc1->text == NULL && doc2->text == NULL){ // both null
  // OMG I cannot believe I am going to do this but
  // I have no time to refactor this
  // Do nothing if the texts are both null
  } else if (doc1->text == NULL){ //doc2 is not null
    toReturn->text = malloc(1 + strlen(doc2->text));
    strcpy(toReturn->text, doc2->text);
  } else if (doc2->text == NULL){ //doc1 is not null
    toReturn->text = malloc(1 + strlen(doc1->text));
    strcpy(toReturn->text, doc1->text);
  }
  else { // both not null
    toReturn->text = malloc(1 + strlen(doc1->text) + strlen(doc2->text));
    strcpy(toReturn->text, doc1->text);
    strcat(toReturn->text,doc2->text);
  }
  toReturn->signature = false;  
  printf("Documents merged successfully.\n");
  return toReturn;
}   
