#include <stdbool.h>

// a stack of strings
struct stack;

// stack_create() creates a new empty stack.
// effects: allocated heap memory; client must call stack_destroy
// time:    O(1)
struct stack *stack_create(void);

// stack_destroy(stck) releases all resources allocated by stack stck.
// effects: stck becomes invalid.
// time:    O(n)
void stack_destroy(struct stack *stck);

// stack_push(stck, str) pushes the string str onto the top of stack stck.
// effects: modifies stck
// time:    O(s), where s is the length of str
void stack_push(struct stack *stck, const char *str);

// stack_top(stck) returns the string from the top of stack stck. The client
//   must free the returned string.
// requires: stck must not be empty
// time:     O(s), where s is the length of the string at top
char *stack_top(const struct stack *stck);

// stack_pop(stck) removes and returns the string from the top of the stack
//   stck. The client must free the returned string.
// effects:  modifies stck
// requires: stck must not be empty
// time:     O(1)
char *stack_pop(struct stack *stck);

// stack_is_empty(stck) returns true if the stack stck is empty, and false
//   otherwise.
// time: O(1)
bool stack_is_empty(const struct stack *stck);
