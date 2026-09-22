// client_string() runs the hash table client for strings.
// The syntax for the IO test client is as follows:
//   HASHTABLE [INT]: creates a hash table with a hash-function with [INT] bit.
//                    This must be the first instruction right after the type!
//   INSERT [STRING]: inserts [STRING] into the hash table
//   REMOVE [STRING]: removes [STRING] from the hash table
//   PRINT          : prints the content of the hash table
void client_string(void);