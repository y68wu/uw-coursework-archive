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

// This is the implementation of the hash tree ADT with linked lists.

#include <stdlib.h>
#include "hashtable.h"
#include "cs136-trace.h"
#include "assert.h"
#include "string.h"
#include "stdio.h"

// -----------------------------------------------------------------------
// DO NOT CHANGE THESE
// See hashtable.h for documentation
const int HT_SUCCESS        = 0;
const int HT_ALREADY_STORED = 1;
const int HT_NOT_STORED     = 2;
// -----------------------------------------------------------------------

// linked list to store strings

struct node {
  char* data;
  struct node *next;
};

// a hash table for storing strings

struct hashtable {
  struct node** nodes;
  int (*hash_func)(const char *, int);
  int length;
  int capacity;
};

// calculate_capacity(length) calculates the capacity of the hash table.
// Specifically, the capacity of the hash table is 2^(length).
// requires: length must be positive

static int calculate_capacity(int length) {
  assert(length > 0);

  int capacity = 1;
  for (int i = 0; i < length; i++) {
    capacity *= 2;
  }
  return capacity;
}

// ht_create(hash_func, hash_length) creates a new string hash table with
//   hash_func as hash function and hash_length the length of the result
//   of hash_func (in bits).
// effects: allocates heap memory; client must call ht_destroy
// requires: hash_length must be positive
// time: O(n), where n is the length of the hash table

struct hashtable *ht_create(int (*hash_func)(const char *, int),
                            int hash_length) {
  assert(hash_func);
  assert(hash_length > 0);

  struct hashtable* ht = malloc(sizeof(struct hashtable));
  ht->capacity = calculate_capacity(hash_length);
  ht->hash_func = hash_func;
  ht->length = hash_length;
  struct node** new_nodes = malloc(sizeof(struct node*) * (ht->capacity));

  for (int i = 0; i < (ht->capacity); i++) {
    new_nodes[i] = NULL;
  }

  ht->nodes = new_nodes;
  return ht;
}

// ht_destroy(ht) frees all resources allocated by the hashtable ht.
// effects: invalidates ht
// time: O(n + m), where n is the length of ht and m is the number of
//   items in ht

void ht_destroy(struct hashtable *ht) {
  assert(ht);

  for (int i = 0; i < ht->capacity; i++) {    
    struct node *current = ht->nodes[i];

    while (current != NULL) {
      struct node* next = current->next;
      free(current->data);
      free(current);
      current = next;
    }
  }
  
  free(ht->nodes);
  free(ht);
}

// ht_insert(ht, str) inserts the string str into the hash table ht. The
//   function returns
//   * HT_SUCCESS if str has been inserted into ht or
//   * HT_ALREADY_STORED if str is already stored in ht.
// requires: str must be a non-empty null-terminated string [not asserted] 
// time: O(m + s), where s is the length of str and m is the number of
//       items in ht

int ht_insert(struct hashtable *ht, const char *str) {
  assert(ht);
  assert(str);

  int index = ht->hash_func(str, ht->length);
  struct node* current = ht->nodes[index];

  while (current) {
    const char* data = current->data;
    if (strcmp(str, data) == 0) {
      return HT_ALREADY_STORED;
    } else {
      current = current->next;
    }
  }

  struct node* new_node = malloc(sizeof(struct node));
  new_node->data = malloc(strlen(str) + 1);
  strcpy(new_node->data, str);
  new_node->next = ht->nodes[index];
  ht->nodes[index] = new_node;

  return HT_SUCCESS;
}

// ht_remove(ht, str) removes the string str from the hash table ht. The
//   function returns
//   * HT_SUCCESS if str has been removed from ht, or
//   * HT_NOT_STORED if str was not stored in ht.
// requires: str must be a non-empty null terminated string [not asserted]
// time: O(m + s), where s is the length of str and m is the number of
//       items in ht

int ht_remove(struct hashtable *ht, const char *str) {
  assert(ht);
  assert(str);

  int index = ht->hash_func(str, ht->length);
  struct node *current = ht->nodes[index];
  struct node *prev = NULL;

  while (current != NULL) {
    if (strcmp(current->data, str) == 0) {
      if (prev != NULL) {
        prev->next = current->next;
      } else {
        ht->nodes[index] = current->next;
      }
      free(current->data);
      free(current);
      return HT_SUCCESS;
    }
    prev = current;
    current = current->next;
  }

  return HT_NOT_STORED;
}

// ht_print(ht) prints the content of hash table ht to the console.
// effects: creates output
// time: O(n + m * s), where n: length of ht, s: maximum length of any string,
//       m: number of items in ht

void ht_print(const struct hashtable *ht) {
  assert(ht);
  for (int i = 0; i < ht->capacity; i++) {
    printf("%d: ", i);
    struct node* current = ht->nodes[i];

    if (current != NULL) {
      printf("%s", current->data);
      current = current->next;

      while (current != NULL) {
        printf(" -> %s", current->data);
        current = current->next;
      }
      
    }
    printf("\n");
  }
}