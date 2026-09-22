// client_fancyline() runs the hash table client for fancylines.
// The syntax for the IO test client is as follows:
//   HASHTABLE [INT]: creates a hash table with a hash-function with [INT] bit.
//                    This must be the first instruction right after the type!
//   INSERT [SYMBOL] [LENGTH]: inserts a line consisting of [LENGTH] symbols
//                             [SYMBOL] into the hash table
//   REMOVE [SYMBOL] [LENGTH]: removes a line consisting of [LENGTH] symbols
//                             [SYMBOL] from the hash table
//   PRINT         : prints the content of the hash table
void client_fancyline(void);