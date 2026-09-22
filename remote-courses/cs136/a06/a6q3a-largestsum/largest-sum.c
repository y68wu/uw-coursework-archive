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

// largest-sum.c [IMPLEMENTATION]

#include "largest-sum.h"
#include <assert.h>

// find_diff_max(array, len, max, max_index, min) find the largest value in the array 
// that is smaller than the given maximum value.
// Requires:
//   len > 0, 
//   max, max_index are valid pointer that point to the maximum value and its index in the array
//   min is the smallest number in an array
// Effects:
//   mutates max to be the updated largest number
//   mutates max_index to the index of the new maximum value.
// Time:
//   O(m), m is the length of the array

static void find_diff_max(const int array[], int len, int* max, int* max_index, int min){
    int max_so_far = min;
    int index = -1;
    for (int i = 0; i < len; i++) {
        if (array[i] >= max_so_far && array[i] < *max) {
            max_so_far = array[i];
            index = i;
        }
    }
    *max = max_so_far;
    *max_index = index;
}

// exist_max(array, len, max, max_index) check if the current maximum value 
// exists in the array before a given index.
// Requires:
//   len > 0, 
//   max_index are valid pointer that point to the maximum value in the array
//   max is the maximum number in an array
// Effects:
//   If the current maximum value exists in the array before max_index
//   mutates the value of max_index points to, and returns 1.
//   If the current maximum value does not exist in the array before max_index, returns -1.
// Time:
//   O(m), m is the length of the array

static int exist_max(const int array[], int len, int max, int* max_index) {
    for (int i = len - 1; i >= 0; i--) {
        if (array[i] == max && i < *max_index) {
            *max_index = i;
            return 1;
            break;
        }
    }
    return -1;
}

// Finding the initial maximum and minimum values requires iterating through the array once, 
// which takes O(m) times. The second while loop runs n times. Inside the loop, 
// the exist_max or find_diff_max function is called, which takes O(m).
// There are n O(m) to add together.
// So the total time is O(m) + O(n * m) = O(n * m) , (n <= m)
// The worst case -> n = m, time: O(m ^ 2)
// time: O(m ^ 2), m is the length of the array

// largest_sum(array, len, n) calculates the sum of the n largest 
//   elements in the array
// requires: len > 0, n > 0 && n <= len
//           array must not be mutated
// effects: none
int largest_sum(const int array[], int len, int n) {
    assert(array);
    assert(len > 0);
    assert(n > 0);
    assert(n <= len);
    int sum = 0;
    int max_index = 0;
    int max_val = array[0];
    int min_val = array[0];
    for (int i = 0; i < len; i ++) {
        if (array[i] >= max_val) {
            max_index = i;
            max_val = array[i];
        }
        if (array[i] <= min_val) {
            min_val = array[i];
        }
    }
    sum += max_val;
    n -= 1;
    
    while (n != 0){
        if (exist_max(array, len, max_val, &max_index) == 1) {
            sum += max_val;
        } else {
            find_diff_max(array, len, &max_val, &max_index, min_val);
            sum += max_val;
        }
        n -= 1;
    }  
    return sum;
}