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

// This is the implementation of the generic hash table ADT with binary search 
//   trees.

#include <stdlib.h>
#include "hashtable.h"
#include "cs136-trace.h"
#include "assert.h"
#include "stdio.h"
#include "stdbool.h"

// -----------------------------------------------------------------------
// DO NOT CHANGE THESE
// See hashtable.h for documentation
const int HT_SUCCESS        = 0;
const int HT_ALREADY_STORED = 1;
const int HT_NOT_STORED     = 2;
// -----------------------------------------------------------------------

// Node in the binary search tree for storing keys
struct bst_node {
  void* key;
  struct bst_node* left;
  struct bst_node* right;
};

// a generic hashtable
struct hashtable {
  struct bst_node** nodes;
  void* (*key_clone) (const void*);
  int (*key_hash) (const void*, int);
  int (*key_compare) (const void*, const void*);
  void (*key_destroy) (void*);
  void (*key_print) (const void*);
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

// bst_node_creat(key, key_clone) creates a new binary search tree node
// effects: allocate memory, user must free

static struct bst_node *bst_node_create(const void* key, void* (*key_clone)(const void*)) {
  assert(key);
  assert(key_clone);

  struct bst_node* node = malloc(sizeof(struct bst_node));
  node->key = key_clone(key);
  node->left = NULL;
  node->right = NULL;
  return node;
}

// bst_destroy(node, key_destroy) destroies a binary search tree

static void bst_destroy(struct bst_node* node, void (*key_destroy)(void*)) {
  assert(key_destroy);

  if (node != NULL) {
    bst_destroy(node->left, key_destroy);
    bst_destroy(node->right, key_destroy);
    key_destroy(node->key);
    free(node);
  }
}

// bst_insert(node, key, key_clone, key_compare) inserts a key into the binary search tree

static int bst_insert(struct bst_node** root, const void* key, 
                      void* (*key_clone)(const void*),
                      int (*key_compare)(const void*, const void*)) {
  assert(root);
  assert(key);
  assert(key_clone);
  assert(key_compare);

  struct bst_node* new_node = bst_node_create(key, key_clone);

  if (*root == NULL) {
    *root = new_node;
    return HT_SUCCESS;
  }

  struct bst_node* current = *root;
  
  while (current != NULL) {
    int cmp = key_compare(key, current->key);

    if (cmp == 0) {
      free(new_node->key);
      free(new_node);
      return HT_ALREADY_STORED;
    } else if (cmp < 0) {
      if (current->left == NULL) {
        current->left = new_node;
        return HT_SUCCESS;
      } else {
        current = current->left;
      }
    } else {
      if (current->right == NULL) {
        current->right = new_node;
        return HT_SUCCESS;
      } else {
        current = current->right;
      }
    }
  }

  return HT_SUCCESS;
}

// bst_remove(node, key, key_compare, key_destroy) removes a key from the binary search tree
static int bst_remove(struct bst_node** node, const void* key, 
                      int (*key_compare)(const void*, const void*),
                      void (*key_destroy)(void*)) {
  if (*node == NULL) {
    return HT_NOT_STORED;
  }

  int cmp = key_compare(key, (*node)->key);
  if (cmp < 0) {
    return bst_remove(&((*node)->left), key, key_compare, key_destroy);
  } else if (cmp > 0) {
    return bst_remove(&((*node)->right), key, key_compare, key_destroy);
  } else {
    struct bst_node *temp;

    if ((*node)->left == NULL) {
      temp = (*node)->right;
      key_destroy((*node)->key);
      free(*node);
      *node = temp;
    } else if ((*node)->right == NULL) {
      temp = (*node)->left;
      key_destroy((*node)->key);
      free(*node);
      *node = temp;
    } else {
      struct bst_node* prev = NULL;
      struct bst_node* curr = (*node)->right;

      while (curr->left != NULL) {
        prev = curr;
        curr = curr->left;
      }
      
      key_destroy((*node)->key);
      (*node)->key = curr->key;
      
      if (prev != NULL) {
        prev->left = curr->right;
      } else {
        (*node)->right = curr->right;
      }
      
      free(curr);
    }
    return HT_SUCCESS;
  }
}


// bst_print prints the content of the binary search tree rooted at node
// effects: creates output

static void bst_print(struct bst_node* node, void (*key_print)(const void*), int level, bool* is_first) {
  assert(key_print);
  assert(is_first);
  
  if (node == NULL) {
    return;
  }

  bst_print(node->left, key_print, level + 1, is_first);

  if (!(*is_first)) {
    printf(",");
  }
  printf("%d-", level);
  key_print(node->key);

  *is_first = false;

  bst_print(node->right, key_print, level + 1, is_first);
}

// ht_create(key_clone, key_hash, hash_length, key_compare, key_destroy, 
// key_print) creates a new empty generic hash table.
// effects: allocates heap memory; client must call ht_destroy
// requires: hash_length must be positive
// time: O(n), where n is the length of the hash table

struct hashtable *ht_create(void* (*key_clone)(const void *),
                            int (*hash_func)(const void *, int), 
                            int hash_length,
                            int (*key_compare)(const void *, const void *),
                            void (*key_destroy)(void *),
                            void (*key_print)(const void *)) {
  assert(key_clone);
  assert(hash_func);
  assert(hash_length > 0);
  assert(key_compare);
  assert(key_destroy);
  assert(key_print);

  struct hashtable* ht = malloc(sizeof(struct hashtable));

  ht->key_clone = key_clone;
  ht->key_hash = hash_func;
  ht->key_compare = key_compare;
  ht->key_destroy = key_destroy;
  ht->key_print = key_print;
  ht->length = hash_length;
  ht->capacity = calculate_capacity(hash_length);
  ht->nodes = malloc(sizeof(struct bst_node*) * ht->capacity);

  for (int i = 0; i < ht->capacity; i++) {
    ht->nodes[i] = NULL;
  }
  
  return ht;
}

// ht_destroy(ht) frees all resources allocated by the hashtable ht.
// effects: invalidates ht
// time: O(n + m), where n is the length of ht and m is the number of
//   items in ht

void ht_destroy(struct hashtable *ht) {
  assert(ht);

  int len = ht->capacity;
  for (int i = 0; i < len; i++) {
    bst_destroy(ht->nodes[i], ht->key_destroy);
  }

  free(ht->nodes);
  free(ht);
}

// ht_insert(ht, key) inserts the key key into the hash table ht. The
//   function returns
//   * HT_SUCCESS if key has been inserted into ht or
//   * HT_ALREADY_STORED if key is already stored in ht.
// time: O(m + cl + co + hf), where m is the number of items in ht, 
//   cl: complexity of key_clone; co: complexity of key_compare; hf: complexity
//   of key_hash

int ht_insert(struct hashtable *ht, const void *key) {
  assert(ht);
  assert(key);

  int index = ht->key_hash(key, ht->length);
  return bst_insert(&(ht->nodes[index]), key, ht->key_clone, ht->key_compare);
}

// ht_remove(ht, key) removes the key key from the hash table ht. The
//   function returns
//   * HT_SUCCESS if key has been removed from ht, or
//   * HT_NOT_STORED if key was not stored in ht.
// time: O(m + cl + co + hf), where m is the number of items in ht, 
//   cl: complexity of key_clone; co: complexity of key_compare; hf: complexity
//   of key_hash

int ht_remove(struct hashtable *ht, const void *key) {
  assert(ht);
  assert(key);

  int index = ht->key_hash(key, ht->length);
  return bst_remove(&(ht->nodes[index]), key, ht->key_compare, ht->key_destroy);
}

// ht_print(ht) prints the content of hash table ht to the console.
// effects: creates output
// time: O(n + m * cp), where n: length of ht, m: number of items in ht,
//   cp: complexity of key_print

void ht_print(const struct hashtable *ht) {
  assert(ht);

  for (int i = 0; i < ht->capacity; i++) {
    printf("%d: [", i);

    if (ht->nodes[i] != NULL) {
      bool is_first = true;
      bst_print(ht->nodes[i], ht->key_print, 0, &is_first);
    }

    printf("]\n");
  }
}