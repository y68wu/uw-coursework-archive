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

#include "arrayfun_improved.h"
#include <assert.h>
#include <stdbool.h>

// divides_followers(a, n) determines how many items in a are divisors
//   of the sum of the sub-array starting at that item and ending at
//   the final item of the array.
// requires: a is a valid array with given len [not asserted]
//           len > 1
// time: O(n), n is len of a

int divides_followers(const int a[], int len) {
  assert(a);
  assert(len >= 0);

  int sum = 0;
  int count = 0;

  for (int i = len - 1; i >= 0; i--) {
    sum += a[i];
    if ((a[i] != 0) && (sum % a[i] == 0)) {
      ++count;
    }
  }
  
  return count;
}	

// binary_search(arr, val, left, right) determines if the val is the sum
//  of any two consecutive elements in arr.
// requires: arr is a valid, sorted array
// time: O(log n), n is the length of arr

static bool binary_search(const int arr[], int val, int left, int right) {
  assert(arr);

  while (left <= right) {
    int mid = left + (right - left) / 2;
    if (arr[mid] + arr[mid + 1] == val) {
      return true;
      break;
    } else if (arr[mid] + arr[mid + 1] > val) {
      right = mid - 1;
    } else {
      left = mid + 1;
    }
  }

  return false;
}

// consec_sum(a, n, b, m) determines if an element of b is the sum
//   of any two consecutive elements in a.
// requires: a and b are arrays of length n and m, respectively
//           [not asserted]
//           n, m > 1
// 	     a is sorted in increasing order (b may not be sorted)
// time: O(m*logn), n is len of a and m is len of b

bool consec_sum(const int a[], int n, const int b[], int m) {
  assert(a);
  assert(n > 1);
  assert(b);
  assert(m > 1);

  for (int i = 0; i < m; i++) {
    if (binary_search(a, b[i], 0, n - 2)) {
      return true;
      break;
    }
  }
  return false;
}


// even_first(a,len) puts all the even numbers before all of the odd numbers
//   in the array a.  The order of the even/odd items may be changed in some
//   unspecified way.
// requires: a is a valid array with given len [not asserted]
//           len > 1
// effects: modifies a
// time: O(n), n is len of a

void even_first(int a[], int len) {
  assert(a);
  assert(len > 0);

  int left = 0;
  int right = len - 1;

  while (left < right) {

    // move from the left until an odd number is found
    while (left < right && a[left] % 2 == 0) {
      left++;
      }

    // Move right pointer until an even number is found
    while (left < right && a[right] % 2 != 0) {
      right--;
      }
    
    // Swap the odd number at left with the even number at right
    if (left < right) {
      int temp = a[left];
      a[left] = a[right];
      a[right] = temp;
      left += 1;
      right -= 1;
      }
  }
}
