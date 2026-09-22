// this program demonstrates structs

#include <stdio.h>
#include <stdbool.h>

struct posn {       // name of the structure
  int x;            // type and field names
  int y;
};                  // don't forget this ;

// posn_equal(a, b) with return false if the passed structs are not equal
//   and will return true if the structs are exactly the same
bool posn_equal (struct posn a, struct posn b) {
  if (a.x != b.x){
    return false;
  }
  if (a.y != b.y){
    return false;
  }
  return true;
}

int main(void) {
    struct posn p = {1, 2};
    struct posn q = {5, 6};
    
    printf("p is %d,%d\n", p.x, p.y);
    printf("q is %d,%d\n", q.x, q.y);
    
    // Unified message format
    printf("The structures p and q are ");
    if (posn_equal(p, q)) {
        printf("equal!\n");
    } else {
        printf("not equal!\n");
    }
    
    return 0;
}

