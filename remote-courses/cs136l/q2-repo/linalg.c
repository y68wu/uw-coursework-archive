#include "vec.h"
#include "linalg.h"

int star(const struct Vec v1, const struct Vec v2) {
   return v1.x * v2.x + v1.y * v2.y;
}

int normSquared(const struct Vec v) { return star(v,v); }
