#include <stdio.h>
#include "black_box.h"

int main() {
  int x = 10;
  foo(&x);
  
  if (x == 0) {
    printf("Error: x set to 0\n");
    return 3;
  }
  else {
    printf("Successful!\n");
  }
}
