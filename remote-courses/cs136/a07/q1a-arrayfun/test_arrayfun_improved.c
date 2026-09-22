#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "arrayfun_improved.h"
#include "cs136-trace.h"

// is_evens_before_odds(arr, len) determines if arr contains
//   no even numbers after an odd number, in the interval [0, len).
static bool is_evens_before_odds(int arr[], int len) {
  for (int i=0; i<len-1; ++i) {
    if ((arr[i] % 2) && !(arr[i + 1] % 2)) {
      return false;
    }
   }
  return true;
}

int main(void) {
  int len = 9;
  int a[9] = {5, 6, 1, 9, 3, 7, 8, 2, 4};
  int b[9] = {1, 2, 3, 4, 5, 6, 7, 8, 9};

  assert(divides_followers(a,len) == 6);

  assert(consec_sum(a, len, b, len) == 1);

  even_first(a, len);
  assert(is_evens_before_odds(a, len));

  even_first(b, len);
  assert(is_evens_before_odds(b, len));
  trace_msg("ALL ASSERTIONS PASSED!")
}
