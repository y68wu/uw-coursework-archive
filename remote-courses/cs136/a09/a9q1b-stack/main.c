#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "cs136-trace.h"
#include "stack.h"

// The syntax for the IO test client is as follows:
//   PUSH [string] : pushes [string] to the top of the stack
//   TOP   : returns and prints the string from the top of the stack
//   POP   : removes, returns, and prints the string from the top of the stack
//   EMPTY : prints wheather the stack is empty or not
int main(void) {
  const int cmd_push = lookup_symbol("PUSH");
  const int cmd_top = lookup_symbol("TOP");
  const int cmd_pop = lookup_symbol("POP");
  const int cmd_empty = lookup_symbol("EMPTY");
  
  struct stack *stck = stack_create();
  
  int cmd = INVALID_SYMBOL;
  while ((cmd = read_symbol()) != INVALID_SYMBOL) {
    if (cmd == cmd_push) {
      char input[81] = {'\0'};
      assert(scanf("%s", input) == 1);
      stack_push(stck, input);
    } else if (cmd == cmd_top) {
      char *top = stack_top(stck);
      printf("Top: %s\n", top);
      free(top);
    } else if (cmd == cmd_pop) {
      char *str = stack_pop(stck);
      printf("Pop: %s\n", str);
      free(str);
    } else if (cmd == cmd_empty) {
      printf("Stack is%sempty.\n", stack_is_empty(stck) ? " " : " not ");
    } else {
      printf("ERROR: unknown command\n");
      break;
    }
  }
  stack_destroy(stck);
}
