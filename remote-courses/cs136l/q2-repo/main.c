#include <stdio.h>
#include "vec.h"
#include "linalg.h"
#include "cs136.h"
int main () {
  trace_version();
  struct Vec v = {1,2};
  v = add(v,v);
  trace_msg("Printing v.x");
  trace_int(v.x);
  trace_msg("Printing v.y");
  trace_int(v.y);
  int n = normSquared(v);
  trace_msg("Printing n");
  trace_int(n);
}
