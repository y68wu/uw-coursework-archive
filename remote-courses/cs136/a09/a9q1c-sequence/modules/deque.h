// a doubly-linked list (or deque) of strings
struct deque;

// deque_create() creates a new empty deque
// effects: allocated heap memory; client must call deque_destroy
// time:    O(1)
struct deque *deque_create(void);

// deque_destroy(deq) releases all resources allocated by deque deq.
// effects: deq becomes invalid.
// time:    O(n)
void deque_destroy(struct deque *deq);

// deque_add_front(deq, str) adds string str to the front of the deque deq.
// effects: modifies deq
// time:    O(s), where s is the length of str
void deque_add_front(struct deque *deq, const char *str);

// deque_add_back(deq, str) adds string str to the back of the deque deq.
// effects: modifies deq
// time:    O(s), where s is the length of str
void deque_add_back(struct deque *deq, const char *str);

// deque_remove_front(deq) removes and returns the string that is stored at
//   the front of the deque deq, or NULL of deq is empty. The client must free
//   the returned string.
// effects: modifies deq
// time:    O(1)
char *deque_remove_front(struct deque *deq);

// deque_remove_back(deq) removes and returns the string that is stored at 
//   the back of the deque deq, or NULL of deq is empty. The client must free
//   the returned string.
// effects: modifies deq
// time:    O(1)
char *deque_remove_back(struct deque *deq);

// deque_length(deq) returns the length of the deque deq.
// time: O(1)
int deque_length(const struct deque *deq);

// deque_print_forward(deq) prints the content of deque deq to the console
//   from front to back.
// effects:  writes to console
// time:     O(n * s), where s: max length of any string
void deque_print_forward(const struct deque *deq);

// deque_print_backward(deq) prints the content of deque deq to the console
//   from back to front.
// effects:  writes to console
// time:     O(n * s), where s: max length of any string
void deque_print_backward(const struct deque *deq);
