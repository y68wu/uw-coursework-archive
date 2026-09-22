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

#include "stack.h"
#include "deque.h"
#include "assert.h"
#include "cs136-trace.h"
#include <stdlib.h>

// a stack of strings

struct stack {
  // DO NOT MODIFIY THIS STRUCT
  struct deque *data;
};

// stack_create() creates a new empty stack.
// effects: allocated heap memory; client must call stack_destroy
// time:    O(1)

struct stack *stack_create(void) {
  struct stack* new_stack = malloc(sizeof(struct stack));
  new_stack->data = deque_create();
  return new_stack;
}

// dnode_destroy(str, destroy_data) releases all resources allocated by dnode dn.
//   if destroy_data is true, the stored data is destroyed as well.
// time: O(1)

void stack_destroy(struct stack *stck) {
  assert(stck);

  deque_destroy(stck->data);
  free(stck);
}

// stack_push(stck, str) pushes the string str onto the top of stack stck.
// effects: modifies stck
// time: O(s), where s is the length of str

void stack_push(struct stack *stck, const char *str) {
  assert(stck);
  assert(str);

  deque_add_front(stck->data, str);
}

// stack_top(stck) returns the string from the top of stack stck. The client
//   must free the returned string.
// requires: stck must not be empty
// time:     O(s), where s is the length of the string at top

char *stack_top(const struct stack *stck) {
  assert(stck);
  assert(deque_length(stck->data) > 0);

  char *top_value = deque_remove_front(stck->data);
  deque_add_front(stck->data, top_value);
  return top_value;
}

// stack_pop(stck) removes and returns the string from the top of the stack
//   stck. The client must free the returned string.
// effects:  modifies stck
// requires: stck must not be empty
// time: O(1)

char *stack_pop(struct stack *stck) {
  assert(stck);
  assert(deque_length(stck->data) > 0);

  return deque_remove_front(stck->data);
}

// stack_is_empty(stck) returns true if the stack stck is empty, and false
//   otherwise.
// time: O(1)
bool stack_is_empty(const struct stack *stck) {
  assert(stck);

  return deque_length(stck->data) == 0;
}
