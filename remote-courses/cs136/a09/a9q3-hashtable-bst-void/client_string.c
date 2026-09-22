#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "cs136-trace.h"
#include "read.h"
#include "hash.h"
#include "hashtable.h"
#include "client_string.h"

// === STRING HELPER FUNCTIONS CALLED BY CONNECTORS ===========================

// requires: all functions require valid non-empty strings

// str_clone(str) creates a duplicate of str.
// effects: allocate heap memory, client must call str_destroy
// time: O(n) where n is the length of str
static char* str_clone(const char *str) {
  assert(str);
  char *new = malloc((strlen(str) + 1) * sizeof(char));
  strcpy(new, str);
  return new;
}

// str_hash(str, hash_len) calculates the hash value of str with a length
//   of hash_len bits.
// requires: hash_len must be between 2 and 16 (both inclusive)
// time: O(n) where n is the length of str
static int str_hash(const char *str, int hash_len) {
  return hash_str(str, hash_len);
}

// str_compare(str1, str2) compares str1 and str2 and returns the following
//   values:
//   * < 0: str1 comes before str2,
//   * = 0: str1 and str2 are equal, and
//   * > 0: str1 comes after str2.
// time: O(n) where n is the minimum length of str1 and str2
static int str_compare(const char *str1, const char *str2) {
  assert(str1);
  assert(str2);
  return strcmp(str1, str2);
}

// str_destroy(str) releases all resources used by str.
// effects: invalidates str
// time: O(1)
static void str_destroy(char *str) {
  assert(str);
  free(str);
}

// str_print(str) prints str
// effects: creates output
// time: O(n) where n is the minimum length of str1 and str2
static void str_print(const char *str) {
  printf("%s", str);
}

// === CONNECTORS =============================================================

// str_adt_clone(ptr) connects to str_clone.
static void *str_adt_clone(const void *ptr) {
  assert(ptr);
  const char *str = ptr;
  return str_clone(str);
}

// str_adt_hash(ptr) connects to str_hash.
static int str_adt_hash(const void *ptr, int hash_len) {
  assert(ptr);
  const char *str = ptr;
  return str_hash(str, hash_len);
}

// str_adt_compare(ptr) connects to str_compare.
static int str_adt_compare(const void *ptr1, const void *ptr2) {
  assert(ptr1);
  assert(ptr2);
  const char *str1 = ptr1;
  const char *str2 = ptr2;
  return str_compare(str1, str2);
}

// str_adt_destroy(ptr) connects to str_destroy.
static void str_adt_destroy(void *ptr) {
  assert(ptr);
  char *str = ptr;
  str_destroy(str);
}

// str_adt_print(ptr) connects to str_print.
static void str_adt_print(const void *ptr) {
  assert(ptr);
  const char *str = ptr;
  str_print(str);
}

/// === CLIENT FUNCTION =======================================================

// See client_string.h for documentation.
void client_string(void) {

  const int CREATE = lookup_symbol("HASHTABLE");
  const int INSERT = lookup_symbol("INSERT");
  const int REMOVE = lookup_symbol("REMOVE");
  const int PRINT = lookup_symbol("PRINT");

  int cmd = read_symbol();
  assert(cmd == CREATE);
  int hash_length = -1;
  assert(scanf("%d", &hash_length) == 1);
  assert(2 <= hash_length && hash_length <= 16);

  struct hashtable *ht = ht_create(str_adt_clone, 
                                   str_adt_hash, hash_length, 
                                   str_adt_compare, 
                                   str_adt_destroy, 
                                   str_adt_print);

  while ((cmd = read_symbol()) != INVALID_SYMBOL) {
    if (cmd == INSERT) {
      char *input = read_line();
      int status = ht_insert(ht, input);
      if (status == HT_SUCCESS) {
        printf("string \"%s\" inserted into the hash table.\n", input);
      } else {
        printf("WARNING: string \"%s\" is already stored in the hash table.\n",
          input);
      }
      free(input);
    } else if (cmd == REMOVE) {
      char *input = read_line();
      int status = ht_remove(ht, input);
      if (status == HT_SUCCESS) {
        printf("SUCCESS: string \"%s\" removed from the hash table.\n", input);
      } else {
        printf("WARNING: string \"%s\" not removed from the hash table.\n",
               input);
      }
      free(input);
    } else if (cmd == PRINT) {
      ht_print(ht);
    } else {
      printf("ERROR: unknown command\n");
      break;
    }
  }

  ht_destroy(ht);
}
