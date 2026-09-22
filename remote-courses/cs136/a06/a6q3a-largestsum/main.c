// A program to test the largest-sum module

#include <stdio.h>
#include <stdbool.h>
#include <limits.h>
#include <assert.h>
#include "largest-sum.h"

int main(void) {
  int array1[] = {0, 1, 2, 3, 4, 5, 6};
  int array2[] = {-6, -5, -4, -3, -2};
  int array3[] = {-10, -10, -4,-5,-10,-3,2,5,3,6};
  int array4[] = {-10, -10, -4,-4,-4,-5,-10,-3,2,5,3,6};

  assert(largest_sum(array1, sizeof(array1)/sizeof(int), 1) == 6);
  assert(largest_sum(array1, sizeof(array1)/sizeof(int), 2) == 11);
  assert(largest_sum(array1, sizeof(array1)/sizeof(int), 3) == 15);
  assert(largest_sum(array1, sizeof(array1)/sizeof(int), sizeof(array1)/sizeof(int)) == 21);
  assert(largest_sum(array2, sizeof(array2)/sizeof(int), 1) == -2);
  assert(largest_sum(array2, sizeof(array2)/sizeof(int), 2) == -5);
  assert(largest_sum(array3, 10, 5) == 13);
  assert(largest_sum(array3, 10, 10) == -26);
  assert(largest_sum(array4, 12, 12) == -34);
  assert(largest_sum(array4, 12, 10) == -14);
}