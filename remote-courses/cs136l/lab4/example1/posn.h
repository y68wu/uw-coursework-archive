struct posn {
  int x;
  int y;
};


// sqr_dist(p1, p2) calculates the square of
// the distance between p1 and p2
// requires: p1, p2 are not null
int sqr_dist(struct posn *p1, struct posn *p2);

// scale(p, f) scales the posn p by f
// requires: p is not null
// effects: modifies p
void scale(struct posn *p, int f);
