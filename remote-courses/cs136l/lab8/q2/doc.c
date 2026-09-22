#include <stdio.h>
#include "doc.h"
#include <stdlib.h>
#include <assert.h>
#include <string.h>

struct document *create_document(){
    struct document *toRet = malloc(sizeof(struct document));
    toRet->text = NULL;
    toRet->signature = false;
    return toRet;
}

void delete_document(struct document *doc){
    // could have asserted doc but do not know if students
    // will end up ensuring preconditions are met.
    if (doc == NULL){
      printf("Document is null. Not deleting.\n");
      return;
    }
    free(doc->text);
    free(doc);
}

void add_text(struct document *doc, const char *s){
    // could have asserted doc but do not know if students
    // will end up ensuring preconditions are met.
    if (doc == NULL){
      printf("Document is null. Not adding text.\n");
      return;
    }
    char *new_str; 
    if (doc->text == NULL){
        new_str = malloc(1+strlen(s));
        strcpy(new_str,s);
    }
    else {
        new_str = malloc(1+strlen(doc->text)+strlen(s));
        strcpy(new_str,doc->text);
        strcat(new_str,s);
        free(doc->text);
    }
    doc->text = new_str;
}

void print_document(const struct document *doc){
    // could have asserted doc but do not know if students
    // will end up ensuring preconditions are met.
    if (doc == NULL){
      printf("Document is null. Not printing.\n");
      return;
    }
    if (doc->signature){
        printf("Document is signed.\n");
    }
    else {
        printf("Document is unsigned.\n");
    }
    printf("%s\n",doc->text);
}
