#include "doc.h"
#include "sign.h"
#include <assert.h>
#include <stdio.h>

void sign(struct document *doc){
    // could have asserted doc but do not know if students
    // will end up ensuring preconditions are met.
    if (doc == NULL){
      printf("Document is null. Not printing.\n");
      return;
    }
    doc->signature = true;
}
