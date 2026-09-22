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

#include <stdbool.h>

// divides_followers(a, n) determines how many items in a are divisors
//   of the sum of the sub-array starting at that item and ending at
//   the final item of the array.
// requires: a is a valid array with given len [not asserted]
//           len > 1
// time: O(n^2), n is the length of the array
//       Inner loop: sum[j=i...n-1] time: n-i
//       Outer loop: sum[i=0...n-1] time: n
// The total time is sum[n, n-1, n-1...0] = (n*(n+1))/2 = O(n^2)
int divides_followers(const int a[], int len);

// consec_sum(a, n, b, m) determines if an element of b is the sum
//   of any two consecutive elements in a.
// requires: a and b are arrays of length n and m, respectively
//           [not asserted]
//           n, m > 1
// 	     a is sorted in increasing order (b may not be sorted)
// time: O(m * n), n is the length of array a, m is the length of array b
//       Inner loop: sum[i=0...n-2] time: n-1
//       Outer loop: sum[j=0...m] time: m
// The total time is the m sum of n-2 and the time complexity is O(m * n) 
bool consec_sum(const int a[], int n, const int b[], int m);

// even_first(a,len) puts all the even numbers before all of the odd numbers
//   in the array a.  The order of the even/odd items may be changed in some
//   unspecified way.
// requires: a is a valid array with given len [not asserted]
//           len > 1
// effects: modifies a
// time: O(n^2), n is the length of the array
//       Inner loop: sum[j=i...1] time: i
//       Outer loop: sum[i=0...n-1] time: n
// The total time is sum[0, 1, 2, 3... n-1] = (n*(n-1))/2 = O(n^2)
void even_first(int a[], int len);
