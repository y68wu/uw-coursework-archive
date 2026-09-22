// This module provides a function for finding the next 10 integers
//   greater than a specified value

extern const int MAX_LEN;

// next_ten_greater(arr, len, value, result) finds the 10 (or fewer)
//    elements in arr that are closest to but greater than value
// note : - elements found are placed into result
//        - elements in result must be ordered from smallest to largest
//        - returns how many elements were placed in result (e.g., 10 or fewer)
//          if there are no numbers greater than value, result is not changed
//        - len specifies the number of elements in arr
//        - yes arr is a constant and can not be modified
// requires: length of result >= 10 [not asserted] 
//           (but at most 10 elements will be added)
//        - len > 0
//        - arr and result are not NULL
// effects: may mutate result
// time: ??
int next_ten_greater(const int arr[], int len, int value, int result[]);
