#include <stdio.h>
#include "cs136-trace.h"
#include "client_fancyline.h"
#include "client_string.h"

// The syntax for the IO test client is as follows:
//   STRING : starts the string client
//   FANCYLINE : starts the fancyline client
int main(void) {

  const int STRING = lookup_symbol("STRING");
  const int FANCYLINE = lookup_symbol("FANCYLINE");

  int type = read_symbol();
  if (type == STRING) {
    client_string();
  } else if (type == FANCYLINE) {
    client_fancyline();
  } else {
    printf("ERROR: unknown type\n");
  }
}
