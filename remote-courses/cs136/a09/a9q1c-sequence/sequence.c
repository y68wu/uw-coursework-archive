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

#include "sequence.h"
#include "deque.h"
#include "assert.h"
#include "cs136-trace.h"
#include <stdlib.h>

struct sequence {
  // DO NOT MODIFIY THIS STRUCT
  struct deque *data;
};

// seq_create() creates a new empty string sequence.
// effects: allocates heap memory; client must call seq_destroy
// time: O(1)

struct sequence *seq_create(void) {
  struct sequence* new_seq = malloc(sizeof(struct sequence));
  new_seq->data = deque_create();
  return new_seq;
}

// seq_destroy(seq) frees all resources allocated by seq.
// effects: invalidates seq
// time: O(n)

void seq_destroy(struct sequence *seq) {
  assert(seq);

  deque_destroy(seq->data);
  free(seq);
}

// seq_insert(seq, idx, str) inserts the string str into the sequence seq at
//   index idx.
// requires: idx must be between 0 and the length of seq (both inclusive)
// time: O(n * s), where n: length of seq, s: max length of string in seq

void seq_insert(struct sequence *seq, int idx, const char *str) {
  assert(seq);
  assert(idx >= 0);
  assert(idx <= deque_length(seq->data));
  assert(str);

  for (int i = 0; i < idx; i++) {
    char* temp = deque_remove_front(seq->data);
    deque_add_back(seq->data, temp);
    free(temp);
  }

  deque_add_front(seq->data, str);

  for (int j = 0; j < idx; j++) {
    char *temp = deque_remove_back(seq->data);
    deque_add_front(seq->data, temp);
    free(temp);
  }
}

// seq_remove(seq, idx) removes and returns the string from the sequence seq at 
//   index idx. The client must free the returned string.
// requires: idx must be between 0 (inclusive) and the length of seq (exclusive)
// time: O(n * s), where n: length of seq, s: max length of string in seq

char *seq_remove(struct sequence *seq, int idx) {
  assert(seq);
  assert(idx >= 0);
  assert(idx <= deque_length(seq->data));

  for (int i = 0; i < idx; i++) {
    char* temp = deque_remove_front(seq->data);
    deque_add_back(seq->data, temp);
    free(temp); 
  }

  char *removed_str = deque_remove_front(seq->data);

  for (int j = 0; j < idx; j++) {
    char *temp = deque_remove_back(seq->data);
    deque_add_front(seq->data, temp);
    free(temp);
  }

  return removed_str;
}
