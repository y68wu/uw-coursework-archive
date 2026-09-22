// a sequence of strings
struct sequence;

// seq_create() creates a new empty string sequence.
// effects: allocates heap memory; client must call seq_destroy
// time:    O(1)
struct sequence *seq_create(void);

// seq_destroy(seq) frees all resources allocated by seq.
// effects: invalidates seq
// time:    O(n)
void seq_destroy(struct sequence *seq);

// seq_insert(seq, idx, str) inserts the string str into the sequence seq at
//   index idx.
// requires: idx must be between 0 and the length of seq (both inclusive)
// time:     O(n * s), where n: length of seq, s: max length of string in seq
void seq_insert(struct sequence *seq, int idx, const char *str);

// seq_remove(seq, idx) removes and returns the string from the sequence seq at 
//   index idx. The client must free the returned string.
// requires: idx must be between 0 (inclusive) and the length of seq (exclusive)
// time:     O(n * s), where n: length of seq, s: max length of string in seq
char *seq_remove(struct sequence *seq, int idx);
