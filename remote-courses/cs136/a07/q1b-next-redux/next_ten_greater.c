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

#include "next_ten_greater.h"
#include "assert.h"
#include <stdbool.h>

// Assume this is the maximum array length (if you want/need it)
const int MAX_LEN = 10000;

// place(arr, len, val) place the val into the arr to maintain the arr in
// an increasing order. The length of arr is maximum 10. The extra length
// of arr will be cut off.
// requires:
//         arr is an valid, increasing sorted array
//         len >= 0 and len <= 10
// effects:
//         mutates arr
// time: O(1)

static void place(int arr[], int len, int val) {
  assert(arr);
  assert(len >= 0);
  assert(len <= 10);

  if (len == 0) {
    arr[0] = val;
  }

  if (len == 10) {
    for (int i = 0; i < len; i++) {
      if (val < arr[i]) {
        int pos = i;
        for (int j = len - 1; j > i; j--) {
          arr[j] = arr[j - 1];
        }
      arr[pos] = val;
      break;
      }
    }
  } else if (len < 10) {
    bool status = true;
    for (int i = 0; i < len; i++) {
      if (val < arr[i]) {
        status = false;
        int pos = i;
        for (int j = len; j > i; j--) {
          arr[j] = arr[j - 1];
        }
      arr[pos] = val;
      break;
      }
    }
    if (status) {
      arr[len] = val;
    }
  }
}

// time: O(n), n is the length of arr
//   justification: for the loop itself, time: sum[i=0...m=n-1]
//                  inside the loop, time: O(1)
//                  the total time for it is n sum of O(1) = O(n)  

int next_ten_greater(const int arr[], int len, int value, int result[]) {
  assert(arr);
  assert(len > 0);
  assert(result);
  int arr_len = 0;

  for (int i = 0; i < len; i++) {
    if (arr[i] > value) {
      place(result, arr_len, arr[i]);
      if (arr_len < 10) {
        arr_len += 1;
      }
    }
  }

  return arr_len;
}
