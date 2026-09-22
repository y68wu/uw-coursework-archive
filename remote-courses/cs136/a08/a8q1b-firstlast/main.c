/////////////////////////////////////////////////////////////////////////////////////////
// INTEGRITY STATEMENT (v4)
//
// By signing your name and ID below you are stating that you have agreed
// to the online academic integrity statement posted on edX:
// (Course > Assignments > Assignment Information & Policies > Academic Integrity Policy)
/////////////////////////////////////////////////////////////////////////////////////////
// I received help from and/or collaborated with:

// None
//
// Name: Chris Wu
// login ID: [student-id-redacted]
//////////////////////////////////////////////////////////////////////////////////////////

// This program is an introduction to using dynamic memory

// A "word" is a sequence of printable non-whitespace charcters (see modules/string-io.h)

// all words in the input are read in and then two words are printed out,
// each on a separate line:
// * the lexicographically "first" word, and then
// * the lexicographially "last" word

// (comparing public.in and public.expect will help illustrate)

// If no words appear in the input then "INVALID INPUT\n" is printed
// If only one word appears, it is both the first and the last word and so it
// is printed twice

// Words should be compared lexicographically using strcmp()
// (you do not have to worry about about upper case, lower case, etc... just use strcmp)

// All input should be read using read_word() (see modules/string-io.h)
// All output should be printed as printf("%s\n", ...)

// Remember, this is an exercise in using dynamic memory, so endeavour to
// use malloc and free appropriately, and don't forget that the strings
// returned by read_word() must eventually be freed

//////////////////////////////////////////////////////////////////////////////

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cs136-trace.h"
#include "string-io.h"

/*
 * strdup(s) duplicates a string by allocating memory and copying the content.
 * Effects: allocates memory (caller must free)
 */

char* strdup(const char *s) {
  assert(s);

  int len = strlen(s) + 1;
  char* dup = malloc(len * sizeof(char));
  strcpy(dup, s);

  return dup;
}

int main(void) {
  char* word = read_word();  
    
  if (word == NULL) {
    printf("INVALID INPUT\n");
    return 0;
  }

  char* first_word = strdup(word);
  char* last_word = strdup(word);
    
  free(word);  

  while (1) {
    char* input_word = read_word();

    if (input_word == NULL) {
      char* line = read_line();  
      if (line == NULL) { 
        free(line); 
        break;
      }
      free(line);
      continue;
    }
    
    if (strcmp(input_word, first_word) < 0) {
      free(first_word);
      first_word = strdup(input_word);
    }

    if (strcmp(input_word, last_word) > 0) {
      free(last_word);
      last_word = strdup(input_word);
    }
            
    free(input_word);
  }

  printf("%s\n", first_word);
  printf("%s\n", last_word);

  free(first_word);
  free(last_word);

  return 0;
}
