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

#include "deque.h"
#include "cs136-trace.h"
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// === DEQUE NODES ==========================================================

// dnode is a node in a double-linked list (or deque) that stores a string
struct dnode {
  char *data;
  struct dnode *next;
  struct dnode *prev;
};

// dnode_create(str) creates a deque node that stores string str.
// time: O(s), where s is the length of str
static struct dnode *dnode_create(const char *str) {
  // Modify this function, if necessary.
  assert(str);
  struct dnode *dn = malloc(sizeof(struct dnode));
  dn->data = malloc(sizeof(char) * (strlen(str) + 1));
  strcpy(dn->data, str);
  dn->prev = NULL;
  dn->next = NULL;
  return dn;
}

// dnode_destroy(str, destroy_data) releases all resources allocated by dnode dn.
//   if destroy_data is true, the stored data is destroyed as well.
// time: O(1)
static void dnode_destroy(struct dnode *dn, bool destroy_data) {
  // Modify this function, if necessary.
  assert(dn);
  if (destroy_data) {
    free(dn->data);
  }
  free(dn);
}

// === DEQUE ================================================================

struct deque {
  struct dnode *front;
  struct dnode *back;
  int length;
};

// deque_create() creates a new empty deque
// effects: allocated heap memory; client must call deque_destroy
// time: O(1)

struct deque *deque_create(void) {
  struct deque *deq = malloc(sizeof(struct deque));
  deq->front = NULL;
  deq->back = NULL;
  deq->length = 0;
  return deq;
}

// deque_destroy(deq) releases all resources allocated by deque deq.
// effects: deq becomes invalid.
// time: O(n)

void deque_destroy(struct deque *deq) {
  assert(deq);
  while (deq->front != NULL) {
    struct dnode *destroy = deq->front;
    deq->front = deq->front->next;
    dnode_destroy(destroy, true);
  }
  free(deq);
}

// deque_add_front(deq, str) adds string str to the front of the deque deq.
// effects: modifies deq
// time: O(s), where s is the length of str

void deque_add_front(struct deque *deq, const char *str) {
  assert(deq);
  assert(str);
  struct dnode *new_node = dnode_create(str);
  if (deq->front == NULL) {
    deq->front = new_node;
    deq->back = new_node; 
  } else {
    new_node->next = deq->front;
    deq->front->prev = new_node; 
    deq->front = new_node;
  }
  deq->length += 1;
}

// deque_add_back(deq, str) adds string str to the back of the deque deq.
// effects: modifies deq
// time:    O(s), where s is the length of str

void deque_add_back(struct deque *deq, const char *str) {
  assert(deq);
  assert(str);

  struct dnode* new_node = dnode_create(str);
  if (deq->back == NULL) {
    deq->front = new_node;
    deq->back = new_node;
  } else {
    new_node->prev = deq->back;
    deq->back->next = new_node; 
    deq->back = new_node;
  }
  deq->length += 1;
}

// deque_remove_front(deq) removes and returns the string that is stored at
//   the front of the deque deq, or NULL of deq is empty. The client must free
//   the returned string.
// effects: modifies deq
// time: O(1)

char *deque_remove_front(struct deque *deq) {
  assert(deq);
  if (deq->front == NULL) {
    return NULL;
  } else {
    struct dnode *destroy = deq->front;
    char *ret_val = destroy->data;
    deq->front = deq->front->next;

    if (deq->front != NULL) {
      deq->front->prev = NULL;
    } else {
      deq->back = NULL;
    }

    dnode_destroy(destroy, false);
    deq->length -= 1;
    return ret_val;
  }
}

// deque_remove_back(deq) removes and returns the string that is stored at 
//   the back of the deque deq, or NULL of deq is empty. The client must free
//   the returned string.
// effects: modifies deq
// time: O(1)

char *deque_remove_back(struct deque *deq) {
  assert(deq);
  if (deq->back == NULL) {
    return NULL;
  } else {
    struct dnode* destroy = deq->back;
    char* ret_val = destroy->data;
    deq->back = deq->back->prev;

    if (deq->back != NULL) {
      deq->back->next = NULL;
    } else {
      deq->front = NULL;
    }

    dnode_destroy(destroy, false);
    deq->length -= 1;
    return ret_val;
  }
}

// deque_length(deq) returns the length of the deque deq.
// time: O(1)

int deque_length(const struct deque *deq) {
  assert(deq);

  int len = deq->length;
  return len;
}

// deque_print_forward(deq) prints the content of deque deq to the console
//   from front to back.
// effects:  writes to console
// time: O(n * s), where s: max length of any string

void deque_print_forward(const struct deque *deq) {
  assert(deq);
  if (deq->front != NULL) {
    struct dnode *current = deq->front;
    printf("[%s", current->data);
    current = current->next;
    while (current != NULL) {
      printf(" -> %s", current->data);
      current = current->next;
    }
    printf("]\n");
  } else {
    printf("[]\n");
  }
}

// deque_print_backward(deq) prints the content of deque deq to the console
//   from back to front.
// effects:  writes to console
// time: O(n * s), where s: max length of any string

void deque_print_backward(const struct deque *deq) {
  assert(deq);
  if (deq->back != NULL) {
    struct dnode *current = deq->back;
    printf("[%s", current->data);
    current = current->prev;
    while (current != NULL) {
      printf(" <- %s", current->data);
      current = current->prev;
    }
    printf("]\n");
  } else {
    printf("[]\n");
  }
}
