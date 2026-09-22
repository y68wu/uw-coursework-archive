#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include "cs136-trace.h"
#include "read.h"
#include "hash.h"
#include "hashtable.h"

int main(void) {

  const int CREATE = lookup_symbol("HASHTABLE");
  const int INSERT = lookup_symbol("INSERT");
  const int REMOVE = lookup_symbol("REMOVE");
  const int PRINT = lookup_symbol("PRINT");

  int cmd = read_symbol();
  assert(cmd == CREATE);
  int hash_length = -1;
  assert(scanf("%d", &hash_length) == 1);
  assert(2 <= hash_length && hash_length <= 16);

  struct hashtable *ht = ht_create(hash_str, hash_length);

  while ((cmd = read_symbol()) != INVALID_SYMBOL) {
    if (cmd == INSERT) {
      char *input = read_line();
      int status = ht_insert(ht, input);
      if (status == HT_SUCCESS) {
        printf("string \"%s\" inserted into the hash table.\n", input);
      } else {
        printf("WARNING: string \"%s\" is already stored in the hash table.\n",
          input);
      }
      free(input);
    } else if (cmd == REMOVE) {
      char *input = read_line();
      int status = ht_remove(ht, input);
      if (status == HT_SUCCESS) {
        printf("SUCCESS: string \"%s\" removed from the hash table.\n", input);
      } else {
        printf("WARNING: string \"%s\" not removed from the hash table.\n",
               input);
      }
      free(input);
    } else if (cmd == PRINT) {
      ht_print(ht);
    } else {
      printf("ERROR: unknown command\n");
      break;
    }
  }

  ht_destroy(ht);
}
