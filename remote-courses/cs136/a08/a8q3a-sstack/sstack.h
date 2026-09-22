// This module provides a strings stack (sstack) ADT

// requires: all functions require valid (non-NULL) parameters

// times: n is the number of strings in the sstack
//        m is the length of the string being pushed or popped

#include <stdbool.h>

struct sstack;

// sstack_create() creates a new empty string stack [sstack]
// effects: allocates memory (client must call sstack_destroy)
// time: O(1)
struct sstack *sstack_create(void);

// sstack_is_empty(ss) determines if ss is empty
// time: O(1)
bool sstack_is_empty(const struct sstack *ss);

// sstack_top(ss) returns a (const) pointer the top item in ss
// requires: ss is not empty
// time: O(1)
const char *sstack_top(const struct sstack *ss);

// sstack_pop(ss) returns and pops (removes) the top item in stack ss
// note: caller must free the returned string
// requires: ss is not empty
// effects: modifies ss
// time: O(m)
char *sstack_pop(struct sstack *ss);

// sstack_push(str, ss) pushes str on top of ss
// note: makes a 'copy' of str (caller must free when popped)
// effects: modifies ss
// time: O(m) [amortized]
void sstack_push(const char *str, struct sstack *ss);

// stack_destroy(ss) frees all memory for ss (including any strings)
// effects: ss is no longer valid
// time: O(n)
void sstack_destroy(struct sstack *ss);
