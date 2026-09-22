// this is an I/O test client for seq-tools
// see the README.TXT and .in an .expect files
// for instructions & examples

#include <stdio.h>
#include "sequence.h"
#include "seq-tools.h"
#include "seqio.h"

// testing functions: (you can create your own)
// MAP:
int sqr(int x) { 
  return x * x;
}

int neg(int x) {
  return -x;
}

// FILTER:
bool even(int x) {
  return (x % 2 == 0);
}

// FOLDL:
int add(int x, int y) {
  return x + y;
}

int main(void) {
  // you can add your own test functions here:
  add_map_fn("sqr", sqr);
  add_map_fn("neg", neg);
  add_filter_fn("even", even);
  add_foldl_fn("add", add);
  
  sequence_io_test();  
}
