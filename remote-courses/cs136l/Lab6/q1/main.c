#include <stdio.h>
#include "doc.h"
#include "merge.h"
#include "sign.h"
#include "edit.h"
#include <string.h>

/*
 * You will edit these functions in Problem 3.
 *
 * The three *_control functions act as wrappers around the
 * merge/sign/edit functions. The intent is to use the C preprocessor to 
 * restrict calls to the functions only when the corresponding feature, i.e.,
 * MERGE, SIGN or EDIT has been purchased by the client.
 *
 * If the feature has not been enabled, the *_control functions should respond
 * with the message: "Feature not enabled\n"
 *
 * If the feature is enabled, the functionality is as specified by the function
 * being called.
 */

struct document *merge_control(struct document *doc1,struct document *doc2){
   return merge(doc1, doc2);
}

void sign_control(struct document *doc){
    sign(doc);
}

void edit_control(struct document *doc, int index, char *s){
    edit(doc,index,s);
}


// sample main function
// feel free to play with the different document functions
int main(void){
    struct document *doc = create_document();
    add_text(doc,"Hello");
    add_text(doc," World");
    sign_control(doc);
    print_document(doc);

    struct document *doc1 = create_document();
    add_text(doc1," This is a test.");
    print_document(doc1);

    struct document *merged = merge_control(doc,doc1);
    print_document(merged);
    
    delete_document(doc); //no longer need this
    delete_document(doc1); //no longer need this   

    edit_control(merged,11,".");
    sign_control(merged);
    print_document(merged);
    delete_document(merged);   
}
