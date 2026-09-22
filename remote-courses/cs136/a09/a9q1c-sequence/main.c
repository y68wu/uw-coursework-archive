#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "cs136-trace.h"
#include "sequence.h"

// The syntax for the IO test client is as follows:
//   INS [int] [string] : inserts [string] at index [int] into the sequence
//   REM [int] : removes and prints the string at index [int] from the sequence
int main(void) {
  const int cmd_insert = lookup_symbol("INS");
  const int cmd_remove = lookup_symbol("REM");

  struct sequence *seq = seq_create();

  int cmd = INVALID_SYMBOL;
  while ((cmd = read_symbol()) != INVALID_SYMBOL) {
    if (cmd == cmd_insert) {
      int index = -1;
      char input[81] = {'\0'};
      assert(scanf("%d %s", &index, input) == 2);
      seq_insert(seq, index, input);
    } else if (cmd == cmd_remove) {
      int index = -1;
      assert(scanf("%d", &index) == 1);
      char *str = seq_remove(seq, index);
      printf("Item at %d: %s\n", index, str);
      free(str);
    } else {
      printf("ERROR: unknown command\n");
      break;
    }
  }
  seq_destroy(seq);
}
