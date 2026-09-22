// This program demonstrates passing structures to functions
// via pointers and mutating structure fields

#include "cs136.h"
#include "posn.h"

int sqr_dist(struct posn *p1, struct posn *p2) {
  assert(p1);
  assert(p2);
  int xdist = p1->x - p2->x;
  int ydist = p1->y - p2->y;
  return xdist * xdist + ydist * ydist;
}

void scale(struct posn *p, int f) {
  assert(p);
  p->x *= f;
  p->y *= f;
}

int main(void) {
  struct posn a = {1, 2};
  struct posn b = {4, 6};
  
  trace_int(sqr_dist(&a, &b));
  scale(&a, 2);
  trace_int(a.x);
  trace_int(a.y);
}

