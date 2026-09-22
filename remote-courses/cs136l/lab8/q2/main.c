// we would typically now use const ints in C for these but since we just
// discussed using macros as constant values, we chose to show you
// how they can be used
#define CREATE 1
#define DELETE 2
#define PRINT 3
#define ADD_TEXT 4
#define SIGN 5
#define MERGE 6
#define EDIT 7
#define QUIT 8

#include <stdio.h>
#include "doc.h"
#include "merge.h"
#include "sign.h"
#include "edit.h"
#include <string.h>
#include "cs136.h"



// the test harness will be able to simultaneously manage at most 3 documents
const int MAX = 3;


/*
 * You will edit these functions in Problem 3
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
   #ifdef MERGE_ENABLED
     return merge(doc1, doc2);
   #else
     printf("Feature not enabled\n");
     return NULL;
   #endif
}

void sign_control(struct document *doc){
   #ifdef SIGN_ENABLED
     sign(doc);
   #else
     printf("Feature not enabled\n");
   #endif
}

void edit_control(struct document *doc, int index, char *s){
  #ifdef EDIT_ENABLED
    edit(doc,index,s);
  #else
    printf("Feature not enabled\n");
  #endif
}


// Test harness for document program
// DO NOT EDIT

// Max sequence of chars we can read 
const int SEQ_LENGTH = 1000;

// read_char_seq(seq) will ignore leading whitespace and then read as many characters 
//   as possible (including whitespace). 
//   Reading stops when reading a char fails or a newline is encountered or 
//   SQL_LENGTH characters have been read.
// There are certainly more elegant ways to implement this but those have
//   not yet been covered in CS136
// Effect: the seq array will contain the read sequence of characters
void read_char_seq(char seq[]){
  for(int i=0; i<SEQ_LENGTH; ++i){    // empty the previous sequence
    seq[i] = '\0';
  }  
  int i = 0;
  while(true){
    char c;
    c = (i==0) ? read_char(true) : read_char(false);
    if (c == READ_CHAR_FAIL || c == '\n' ){
      break;
    }
    seq[i]=c;
    ++i;
    if (i == SEQ_LENGTH-1){
      break;
    }
  }
}

// read_location() is a wrapper around read_int() to ensure that a valid int value is read
//   from standard input and falls within the 0 to MAX-1 range
int read_location(){
  int location = read_int();
  if (location == READ_INT_FAIL ) {
    return READ_INT_FAIL;
  }
  else if ( location < 0 || location > MAX-1){
    trace_msg("Read failed: location must be between 0 and MAX-1");
    return READ_INT_FAIL;
  }
  else {
    return location;
  }
}



// The test harness accepts the following commands:
// 1 to create a new document. Format: 1 [location] e.g. 1 0 will create a document and store at location 0
// 2 to delete a document.     Format: 2 [location] e.g. 2 2 will delete a document that was at location 2
// 3 to print document.        Format: 3 [location] e.g. 3 1 will print a document at location 1
// 4 to add text to a document.Format: 4 [location] [seq] e.g. 4 0 abcd will append abcd to the end of the document
//                                                             at location 0
// 5 to sign a document.       Format: 5 [location] e.g. 5 0 will sign the document at location 0
// 6 to merge two documents.   Format: 6 [location1] [location2] [location3] e.g. 6 2 0 1 will merge the documents
//                                                            at location 2 and 0 and store the new document at 1
// 7 to edit a document.       Format: 7 [location] [index] [seq] e.g. 7 2 25 Hello will insert the seq Hello within 
//                                                            the document at location 2 at index 25
// 8 to quit the program

// Notice that some commands expect additional input. For example the create command (1)
// expects a location which is an integer between 0 and MAX-1. The code uses this is the array location 
// we will use to store the newly created document. 

// As another example, command 4 (to add text to a document) expect us to first
// give an integer between 0 and MAX-1 to indicate which document from the array we 
// want to add text to and then it will expect us to type some text which is the text
// it will add to the document

/*
*
* IMPORTANT NOTE ON TESTING:
*
*/
// Recall from our discussion in the Testing Module that when we create Test Harnesses to test programs
// our job is to NOT test the test harness. For example, this test harness will likely cause unspecified
// behaviour if you were to give a location that is not in the range of the documents array or if you were
// to try to delete a document that does not exist or if you were to try to edit a document but gave it an
// incorrect index. We could bullet proof the test harness but that is not the point; the point is to write
// tests which check that: IF WE WERE TO GIVE THE PROGRAM CORRECT INPUT, DOES IT BEHAVE AS EXPECTED.

int main(void){
  #ifdef TRACEOFF
    trace_off();
  #endif

  // create an array of document pointers with space for MAX documents
  // this means that whenever we want to give an index into this array
  // it must be between 0 and MAX-1
  struct document *documents[MAX];
  for (int i=0; i<MAX; ++i){
    documents[i]=NULL;
  }
  bool done = false; // control the harness loop
  while (!done){
    int n = read_int();
    if (n == READ_INT_FAIL) {
      break;
    } 
    // only gets here if read was successsful
    trace_msg("Command n is ");
    trace_int(n);    
    switch (n){
      case CREATE: { // create a new document (1)
        int location = read_location();
        if (location == READ_INT_FAIL ) {
          done = true;
          trace_msg("Read failed in create command");
          break;
        }
        // assumes that location is within range and there is no document
        // already stored at that location
        // failure to satisfy this will result in unspecified behaviour
        documents[location] = create_document();
        trace_msg("Document created at ");
        trace_int(location);
        break;
      } case DELETE: { // delete a document (2)
        int location = read_location();
        if (location == READ_INT_FAIL ){
          done = true;
          trace_msg("Read failed in delete command");
          break;
        }
        // assumes that location is within range and there is a document
        // at that location to be deleted
        // failure to satisfy this will result in unspecified behaviour
        delete_document(documents[location]);
        documents[location] = NULL;
        trace_msg("Document deleted at ");
        trace_int(location);
        break;
      } case PRINT: { // print document (3)
        int location = read_location();
        if (location == READ_INT_FAIL ){
          done = true;
          trace_msg("Read failed in print command");
          break;
        }
        // assumes that location is within range and there is a document
        // at that location to be printed
        // failure to satisfy this will result in unspecified behaviour
        print_document(documents[location]);
        break;
      } case ADD_TEXT: { // add text to a document (4)
        int location = read_location();
        if (location == READ_INT_FAIL ){
          done = true;
          trace_msg("Read failed in add text command");
          break;
        }
        // assumes that location is within range and there is a document
        // at that location to which we will add text
        // failure to satisfy this will result in unspecified behaviour
        char seq[SEQ_LENGTH];
        read_char_seq(seq);
        add_text(documents[location],seq);
        trace_msg("Added text to document at ");
        trace_int(location);
        break;
      } case SIGN: { // sign a document (5)
        int location = read_location();
        if (location == READ_INT_FAIL ){
          done = true;
          trace_msg("Read failed in sign command");
          break;
        }
        // assumes that location is within range and there is a document
        // at that location to be signed
        // failure to satisfy this will result in unspecified behaviour
        trace_msg("Executing document sign at ");
        trace_int(location);
        sign_control(documents[location]);
        break;
      } case MERGE: { // merge two documents (6)
        int location1 = read_location();
        if (location1 == READ_INT_FAIL){
          done = true;
          trace_msg("Read failed in merge command. Location 1");
          break;
        }
        int location2 = read_location();
        if (location2 == READ_INT_FAIL){
          done = true;
          trace_msg("Read failed in merge command. Location 2");
          break;
        }
        int location3 = read_location();
        if (location3 == READ_INT_FAIL){
          done = true;
          trace_msg("Read failed in merge command. Location 3");
          break;
        }
        // assumes that location1, location2 and location3 are all in range
        // assumes that documents exist at location1 and location2
        // assumes that there is no document at location3
        // failure to satisfy this will result in unspecified behaviour
        documents[location3] = merge_control(documents[location1], documents[location2]);
        // note that the merge function has been implemented to return NULL if the
        // user did not meet the preconditions
        if (documents[location3] != NULL){
          trace_msg("Merged document at location1 with document at location2 and stored result in location3.");
          trace_int(location1);
          trace_int(location2);
          trace_int(location3);
        }
        else {
          trace_msg("Merge failed!");
        }   
        break;
      } case EDIT: {// edit a document (7)
        int location = read_location();
        if (location == READ_INT_FAIL){
          done = true;
          trace_msg("Read failed in edit command while reading location.");
          break;
        }
        // assumes that location is within range and there is a document
        // at that location to be edited
        // failure to satisfy this will result in unspecified behaviour

        int index = read_int();
        if (index == READ_INT_FAIL){
          done = true;
          trace_msg("Read failed in edit command while reading index.");
          break;
        }
        // assume that index is within range of the document's text. 

        char seq[SEQ_LENGTH];
        read_char_seq(seq);
        trace_msg("Executing Edit document at ");
        trace_int(location);
	      edit_control(documents[location],index,seq);
        break;
      } case QUIT: { // quit the program (8)
        done = true;
        break;
      } default: {
        done = true;
        break;
      }
    } // end switch   
  } // end loop

    // out of the loop so make sure all documents we created are deleted
    // we could have left this out but then the user of this test harness
    // would need to make sure that all documents that were created are at somepoint
    // destroyed. By placing this at the end, we make it easier to test other features
    // without having to worry about memory leaks 
    for (int i=0; i<MAX; ++i){
      if (documents[i] != NULL) {
        delete_document(documents[i]);
        documents[i]=NULL;
      }
    }
  return 0;
}
