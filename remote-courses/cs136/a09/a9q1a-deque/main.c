#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "cs136-trace.h"
#include "deque.h"

// The syntax for the IO test client is as follows:
//   AF [string] : adds [string] to the front of the deque
//   AB [string] : adds [string] to the back of the deque
//   RF : removes, returns, and prints the string from the front of the deque
//   RB : removes, returns, and prints the string from the back of the deque
//   PF : prints the content of the deque from front to back
//   PB : prints the content of the deque from back to front
int main(void) {
  const int cmd_add_front = lookup_symbol("AF");
  const int cmd_add_back = lookup_symbol("AB");
  const int cmd_rem_front = lookup_symbol("RF");
  const int cmd_rem_back = lookup_symbol("RB");
  const int cmd_print_for = lookup_symbol("PF");
  const int cmd_print_back = lookup_symbol("PB");
  
  struct deque *deque = deque_create();
  
  int cmd = INVALID_SYMBOL;
  while ((cmd = read_symbol()) != INVALID_SYMBOL) {
    if (cmd == cmd_add_front) {
      char input[81] = {'\0'};
      assert(scanf("%s", input) == 1);
      deque_add_front(deque, input);
    } else if (cmd == cmd_add_back) {
      char input[81] = {'\0'};
      assert(scanf("%s", input) == 1);
      deque_add_back(deque, input);
    } else if (cmd == cmd_rem_front) {
      char *str = deque_remove_front(deque);
      printf("Front: %s\n", str);
      free(str);
    } else if (cmd == cmd_rem_back) {
      char *str = deque_remove_back(deque);
      printf("Back: %s\n", str);
      free(str);
    } else if (cmd == cmd_print_for) {
      deque_print_forward(deque);
    } else if (cmd == cmd_print_back) {
      deque_print_backward(deque);
    } else {
      printf("ERROR: unknown command\n");
      break;
    }
  }
  deque_destroy(deque);
}
