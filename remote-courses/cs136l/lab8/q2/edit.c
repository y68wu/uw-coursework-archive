#include "doc.h"
#include "edit.h"
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

// pseudocode because this has different edge cases:
// split doc->text into s1 and s2 where
//  s1 is 0 to index-1
//  s2 is index to strlen
// update doc->text to s1
// add_text(doc,s)
// add_text(doc,s2)
// What happens if doc->text is empty!
void edit(struct document *doc, int index, const char *s){
    // could have asserted doc but do not know if students
    // will end up ensuring preconditions are met.
    if (doc == NULL){
      printf("Document is null. Not editing.\n");
      return;
    }
    if (doc->text == NULL){
        add_text(doc,s);
        return; 
    }

    // text is not null

    // could have asserted the following but do not know if students
    // will end up ensuring preconditions are met.
    if (index > strlen(doc->text)){
      printf("Provided index is greater than current text. Not editing.\n");
      return;
    }

    int len = strlen(doc->text);
    char *s2 = malloc(1+len); //probably could do something cleverer
    int j = 0;
    for(int i=index; i<len; ++i,++j){
        s2[j] = doc->text[i];
    }
    s2[j]='\0';
    doc->text[index]='\0';//the split!
    add_text(doc,s); //insert the new edit
    add_text(doc,s2); //insert the old remaining string
    free(s2);
}
