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

// Implementation of a triple_s (SSS) ADT

// This is a "wrapper" ADT that provides a triple_s ADT interface, but all of the data
// is actually stored inside of an SOS ADT

#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "cs136-trace.h"
#include "sos.h"
#include "sss.h"

//////////////////////////////////////////////////////////////////////////////
// DO NOT MODIFY THE FOLLOWING DEFINITION

struct triple_s {
  struct sos *seq;
};

//////////////////////////////////////////////////////////////////////////////

// sss_create() creates a new SSS ADT
// effects: allocates memory (you must call sss_destroy)
// time: O(1)

struct triple_s *sss_create(void) {
  struct triple_s* sss = malloc(sizeof(struct triple_s));
  sss->seq = sos_create();
  return sss;
}


// sss_destroy(sss) destroys sss
// requires: sss is valid (not NULL)
// effects: sss is no longer valid
// time: O(n)

void sss_destroy(struct triple_s *sss) {
  assert(sss);
  sos_destroy(sss->seq);
  free(sss);
}

// sss_count(sss) gets the number of strings in sss
// time: O(1)

int sss_count(const struct triple_s *sss) {
  assert(sss);
  int count = sos_length(sss->seq);

  return count;
}

// sss_search(sss, s) finds the index of s in sss
// note: returns -1 if s does not appear in the sss
// time: O(m*logn)

int sss_search(const struct triple_s *sss, const char *s) {
  assert(sss);
  assert(s);

  int left = 0;
  int right = sss_count(sss) - 1;

  while (left <= right) {
    int mid = left + (right - left) / 2;

    const char* mid_str = sos_item_at(sss->seq, mid);
    int result = strcmp(s, mid_str);
    
    if (result == 0) {
      return mid;
      break;
    } else if (result > 0) {
      left = mid + 1;
    } else {
      right = mid - 1;
    }
  }

  return -1;
}

// sss_get(sss, idx) retrieves a pointer to the string in sss
//   at the given idx (index)
// requires: 0 <= idx < sss_count(sss)
// time: O(1)

const char *sss_get(const struct triple_s *sss, int idx) {
  assert(sss);
  assert(idx >= 0);
  assert(idx < sss_count(sss));

  const char* item = sos_item_at(sss->seq, idx);
  return item;
}

// sss_add(sss, s) adds s to sss in the correct index (lexicographically)
// notes: if s already exists, there is no effect
//        when s is added, the indices of words that lexicographically
//          follow s will be changed (+1) because s is added into the "middle"
// effects: may modify sss
// time: O(n + m*logn)

void sss_add(struct triple_s *sss, const char *s) {
  assert(sss);
  assert(s);

  if (sss_search(sss, s) == -1) {
    int left = 0;
    int right = sss_count(sss) - 1;

    while (left <= right) {
      int mid = left + (right - left) / 2;
      const char* mid_str = sos_item_at(sss->seq, mid);
      int result = strcmp(s, mid_str);
      
      if (result > 0) {
        left = mid + 1;
      } else {
        right = mid - 1;
      }
    }

    sos_insert_at(sss->seq, left, s);
  }
}

// sss_add_follows(sss, s) adds s to the "end" of the sss
// requires: s lexicographically follows all strings already in sss
// effects: modifies sss
// time: O(m) [amortized]

void sss_add_follows(struct triple_s *sss, const char *s) {
  assert(sss);
  assert(s);

  if (sss_count(sss) > 0) {
    const char *last_str = sss_get(sss, sss_count(sss) - 1);
    if (strcmp(s, last_str) > 0) {
      sos_insert_end(sss->seq, s);
    }
  } else {
    sos_insert_end(sss->seq, s);
  }

}

// sss_remove(sss, s) removes s from sss (if it exists)
// notes: if s is removed, the indices of words that lexicographically
//        follow s will be changed (-1)
// effects: may modify sss
// time: O(n + m*logn)

void sss_remove(struct triple_s *sss, const char *s) {
  assert(sss);
  assert(s);

  int index = sss_search(sss, s);

  if (index != -1) {
    sos_remove_at(sss->seq, index);
  }
}
