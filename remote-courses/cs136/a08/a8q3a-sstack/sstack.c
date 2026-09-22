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

// Implementation of a string stack (sstack) ADT

// This is a "wrapper" ADT that provides a stack ADT interface, but all of the data
// is actually stored inside of an SOS ADT

// IMPORTANT: Each of the functions should only be a few lines long

#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "cs136-trace.h"
#include "sos.h"
#include "sstack.h"

//////////////////////////////////////////////////////////////////////////////
// DO NOT MODIFY THE FOLLOWING DEFINITIONS

struct sstack {
  struct sos *seq;
};

// we have provided sstack_is_empty() for you :)

// see sstack.h
bool sstack_is_empty(const struct sstack *ss) {
  assert(ss);
  return sos_length(ss->seq) == 0;
}

//////////////////////////////////////////////////////////////////////////////

// sstack_create() creates a new empty string stack [sstack]
// effects: allocates memory (client must call sstack_destroy)
// time: O(1)

struct sstack *sstack_create(void) {
  struct sstack* ss = malloc(sizeof(struct sstack));
  ss->seq = sos_create();

  return ss;
}

// sstack_top(ss) returns a (const) pointer the top item in ss
// requires: ss is not empty
// time: O(1)

const char *sstack_top(const struct sstack *ss) {
  assert(ss);
  assert(!sstack_is_empty(ss));

  int pos = sos_length(ss->seq) - 1;
  return sos_item_at(ss->seq, pos);
}

// sstack_pop(ss) returns and pops (removes) the top item in stack ss
// note: caller must free the returned string
// requires: ss is not empty
// effects: modifies ss
// time: O(m)

char *sstack_pop(struct sstack *ss) {
  assert(ss);
  assert(!sstack_is_empty(ss));

  int top_index = sos_length(ss->seq) - 1;
  const char* top_item = sos_item_at(ss->seq, top_index);

  int len = strlen(top_item) + 1;
  char* result = malloc(len * sizeof(char));
  strcpy(result, top_item);

  sos_remove_end(ss->seq);

  return result;
}

// sstack_push(str, ss) pushes str on top of ss
// note: makes a 'copy' of str (caller must free when popped)
// effects: modifies ss
// time: O(m) [amortized]

void sstack_push(const char *str, struct sstack *ss) {
  assert(str);
  assert(ss);

  sos_insert_end(ss->seq, str);
}

// stack_destroy(ss) frees all memory for ss (including any strings)
// effects: ss is no longer valid
// time: O(n)
void sstack_destroy(struct sstack *ss) {
  assert(ss);

  sos_destroy(ss->seq);
  free(ss);
}