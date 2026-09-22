// seqio.h [INTERFACE]
// A module for testing your seq-tools module
// see test-seqtools.c and README.TXT for more information

#include <stdbool.h>

// sequence_io_test() performs all of the testing
//   by reading in commands from the input
// effects: reads and writes i/o
void sequence_io_test(void);

// add_map_fn("name", f) adds a function f that can be used with
//   the map function using the given "name"
void add_map_fn(const char *name, int (*f)(int));

// add_filter_fn("name", f) adds a function f that can be used with
//   the filter function using the given "name"
void add_filter_fn(const char *name, bool (*f)(int));

// add_foldl_fn("name", f) adds a function f that can be used with
//   the foldl function using the given "name"
void add_foldl_fn(const char *name, int (*f)(int, int));
