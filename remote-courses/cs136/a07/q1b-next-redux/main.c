#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "cs136-trace.h"
#include "array-helpers.h"
#include "next_ten_greater.h"

// In this case we use MAX_LEN from next-ten-greater.c/h
// It is defined there because it may be useful in that code.

int main(void) {
  int cmd = 0;
  // These assume MAX_LEN is 10000
  int array[10000] = {0};
  int result[10000] = {0};
  int r_count = 0;
  int len = 0;
  int num = 0;

  while (1) {
    cmd = read_symbol();
    if (cmd == lookup_symbol("array_read")) {
      len = array_read(array, MAX_LEN);
    } else if (cmd == lookup_symbol("next_ten_greater")) { 
      assert(scanf("%d", &num) == 1);
      r_count = next_ten_greater(array, len, num, result); 
      assert(r_count >= 0);
      assert(r_count <= len);
      printf("%d:", num);
      if (r_count > 0) {
        printf(" ");
        array_print(result, r_count);
      }
      printf("\n");
    } else {
      return EXIT_SUCCESS;
    }
  }
}
