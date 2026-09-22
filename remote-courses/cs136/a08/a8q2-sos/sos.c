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

// This is the implementation of the sequence of strings (sos) ADT

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "cs136-trace.h"
#include "string-io.h"
#include "sos.h"

//////////////////////////////////////////////////////////////////////////////
// DO NOT MODIFY THE FOLLOWING DEFINITIONS

struct sos {
  int len;
  int maxlen;
  char **data;
};

// we have provided sos_read() for you :)
// also, there is no need for you to test sos_read()

// see sos.h
struct sos *sos_read(void) {
  struct sos *seq = sos_create();
  while (1) {
    char *line = read_line();
    if (line == NULL) {
      break;
    }
    sos_insert_end(seq, line);
    free(line);
  }
  return seq;
}

//////////////////////////////////////////////////////////////////////////////

// The following are the functions you must implement

// We recommend that you complete them in the order below, as that may help
// facilitate your testing

// For example, we recommend you write the first few functions
// (create,destroy,length,insert_end,print)
// and then create your own simple tests (.in files) that sufficiently test
// those functions

// Once those functions are tested and debugged, write just one function
// at a time and test it before moving on to the next funciton

// Attempting to write all of the functions before you do any debugging or
// testing is an unwise strategy

//////////////////////////////////////////////////////////////////////////////

// sos_create() returns a new empty sequence of strings
// effects: allocates memory (client must call sos_destroy)
// time: O(1)

struct sos *sos_create(void) {
  struct sos* seq = malloc(sizeof(struct sos));
  seq->len = 0;
  seq->maxlen = 1;
  seq->data = malloc(seq->maxlen * sizeof(char *));

  return seq;
}

// sos_destroy(seq) frees all memory for seq
// effects: seq is no longer valid
// time: O(n)

void sos_destroy(struct sos *seq) {
  assert(seq);

  int len = seq->len;
  for (int i = 0; i < len; i++) {
    free(seq->data[i]);
  }

  free(seq->data);
  free(seq);
}

// sos_length(seq) returns the number of items in seq
// time: O(1)

int sos_length(const struct sos *seq) {
  assert(seq);
  int len = seq->len;
  return len;
}

// sos_insert_end(seq, s) inserts a copy of s at the end of the seq
// note: is the same as sos_insert_at(seq, pos, sos_length(seq))
//       except that it is more efficient
// effects: modifies seq
// time: O(m) [amortized]

void sos_insert_end(struct sos *seq, const char *s) {
  assert(seq);
  assert(s);

  if (seq->len >= seq->maxlen) {
    seq->maxlen *= 2;
    seq->data = realloc(seq->data, (seq->maxlen) * sizeof(char *));
  }

  int len = strlen(s) + 1;  
  char *str = malloc(len * sizeof(char));
  strcpy(str, s);
  
  seq->data[seq->len] = str;
  seq->len += 1;
}

// sos_print(seq) prints each string in seq
// notes: prints each string followed by a newline (\n)
//        prints nothing if seq has length 0
// effects: may produce output
// time: O(n * k)

void sos_print(const struct sos *seq) {
  assert(seq);
  int len = seq->len;
  for (int i = 0; i < len; i++) {
    printf("%s\n", seq->data[i]);
  }
}

// sos_item_at(seq, pos) returns a (const) pointer to the string in seq at pos
// requires: 0 <= pos < sos_length(seq)
// time: O(1)

const char *sos_item_at(const struct sos *seq, int pos) {
  assert(seq);
  assert(pos >= 0);
  assert(pos <= (seq->len));

  char* item = seq->data[pos];
  return item;
}

// sos_remove_end(seq) removes the last string in seq
// note: is the same as sos_remove_at(seq, sos_length(seq) - 1)
//       except that it is more efficient
// requires: 0 < sos_length(seq)
// effects: modifies seq
//          the string that was at (sos_length(seq) - 1) is no longer valid
// time: O(1)

void sos_remove_end(struct sos *seq) {
  assert(seq);
  int len = seq->len;
  free(seq->data[len - 1]);
  seq->len -= 1;
}

// sos_insert_at(seq, pos, s) inserts a copy of s at position pos in seq
// note: changes the position of items that were at position >= pos
// requires: 0 <= pos <= sos_length(seq)
// effects: modifies seq
// time: O(n + m)

void sos_insert_at(struct sos *seq, int pos, const char *s) {
  assert(seq);
  assert(pos >= 0);
  assert(pos <= (seq->len));
  assert(s);

  if (seq->len == seq->maxlen) {
    seq->maxlen *= 2;
    seq->data = realloc(seq->data, seq->maxlen * sizeof(char *));
  }

  for (int i = seq->len; i > pos; i--) {
    seq->data[i] = seq->data[i - 1];
  }

  int len = strlen(s) + 1;
  char* str = malloc(len * sizeof(char));
  strcpy(str, s);

  seq->data[pos] = str;
  seq->len += 1;
}


// sos_remove_at(seq, pos) removes the string at position pos in seq
// notes: changes the position of items that were at position > pos
//        frees the string, so any exisiting pointers to it are now dangling
// requires: 0 <= pos < sos_length(seq)
// effects: modifies seq
//          the string that was at pos is no longer valid
// time: O(n)

void sos_remove_at(struct sos *seq, int pos) {
  assert(seq);
  assert(pos >= 0);
  assert(pos < seq->len);

  free(seq->data[pos]);
  for (int i = pos; i < seq->len - 1; ++i) {
    seq->data[i] = seq->data[i + 1];
  }

  seq->len -= 1;
}

// sos_replace_at(seq, pos, s) replaces the string at position pos in seq
//   with a copy of s
// note: frees the old string, so any exisiting pointers to it are now dangling
//       this is equivalent to a remove then insert, but is more efficient
// requires: 0 <= pos < sos_length(seq)
// effects: modifies seq
//          the string that was at pos is no longer valid
// time: O(m)

void sos_replace_at(struct sos *seq, int pos, const char *s) {
  assert(seq);
  assert(pos >= 0);
  assert(pos < seq->len);
  assert(s);

  free(seq->data[pos]);
  int len = strlen(s) + 1;
  char* str = malloc(len * sizeof(char));
  strcpy(str, s);

  seq->data[pos] = str;
}

// sos_swap(seq, pos1, pos2) swaps the position of the two strings at
//   pos1 and pos2
// note: pos1 and pos2 may have the same value
// requires: 0 <= pos1,pos2 < sos_length(seq)
// effects: may modify seq
// time: O(1)

void sos_swap(struct sos *seq, int pos1, int pos2) {
  assert(seq);
  assert(pos1 >= 0);
  assert(pos2 >= 0);
  assert(pos1 < seq->len);
  assert(pos2 < seq->len);

  char *temp = seq->data[pos1];
  seq->data[pos1] = seq->data[pos2];
  seq->data[pos2] = temp;
}

// sos_dup(seq) creates a new sos that is a duplicate of seq
// note: makes a copy of each and every string in seq
// effects: allocates memory (client must call sos_destroy)
// time: O(n * k)

struct sos *sos_dup(const struct sos *seq) {
  assert(seq);

  struct sos *new_seq = sos_create();
  new_seq->len = seq->len;
  new_seq->maxlen = seq->maxlen;
  new_seq->data = realloc(new_seq->data, new_seq->maxlen * sizeof(char *));

  for (int i = 0; i < seq->len; i++) {      
    int len = strlen(seq->data[i]) + 1;
    new_seq->data[i] = malloc(len * sizeof(char));
    strcpy(new_seq->data[i], seq->data[i]);
  }

  return new_seq;
}
