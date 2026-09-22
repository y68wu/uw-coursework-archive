// HT_SUCCESS indicates successful execution of the function.
extern const int HT_SUCCESS;
// HT_ALREADY_STORED indicates that a string was already stored in the hash
//   table.
extern const int HT_ALREADY_STORED;
// HT_NOT_STORED indicates that a string was not stored in the hash table.
extern const int HT_NOT_STORED;

// a hash table for storing strings
struct hashtable;

// requires: all functions require valid (non-NULL) parameters

// ht_create(hash_func, hash_length) creates a new string hash table with
//   hash_func as hash function and hash_length the length of the result
//   of hash_func (in bits).
// effects: allocates heap memory; client must call ht_destroy
// requires: hash_length must be positive
// time: O(n), where n is the length of the hash table
struct hashtable *ht_create(int (*hash_func)(const char *, int),
                            int hash_length);

// ht_destroy(ht) frees all resources allocated by the hashtable ht.
// effects: invalidates ht
// time: O(n + m), where n is the length of ht and m is the number of
//   items in ht
void ht_destroy(struct hashtable *ht);

// ht_insert(ht, str) inserts the string str into the hash table ht. The
//   function returns
//   * HT_SUCCESS if str has been inserted into ht or
//   * HT_ALREADY_STORED if str is already stored in ht.
// requires: str must be a non-empty null-terminated string [not asserted] 
// time: O(m + s), where s is the length of str and m is the number of
//       items in ht
int ht_insert(struct hashtable *ht, const char *str);

// ht_remove(ht, str) removes the string str from the hash table ht. The
//   function returns
//   * HT_SUCCESS if str has been removed from ht, or
//   * HT_NOT_STORED if str was not stored in ht.
// requires: str must be a non-empty null terminated string [not asserted]
// time: O(m + s), where s is the length of str and m is the number of
//       items in ht
int ht_remove(struct hashtable *ht, const char *str);

// ht_print(ht) prints the content of hash table ht to the console.
// effects: creates output
// time: O(n + m * s), where n: length of ht, s: maximum length of any string,
//       m: number of items in ht
void ht_print(const struct hashtable *ht);
