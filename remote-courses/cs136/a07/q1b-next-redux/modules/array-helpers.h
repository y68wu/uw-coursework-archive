// This module provides some functions for reading integers into
//    an array and for printing an array

// array_print(array, len) print the contents of the array
// note: prints array and DOES NOT print a newline at the end
// requires: len > 0
//           array is a valid array/pointer (i.e., not NULL)
// effects: prints ouput
// time: O(n) where n is the length of array
void array_print(const int array[], const int len);

// array_read(array, max_len) read integers from input and stores them
//   in array
// note: reads until a non-integer or the end of file is reached
//       returns the number of integers read
// requires: max_len > 0
//           array is a valid array/pointer (i.e., not NULL)
// effects: reads input
//          modifies array
// time: O(n) where n is the number of integers read
int array_read(int array[], const int max_len);


