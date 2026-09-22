#include "vec.h"

struct Vec add(const struct Vec v1, const struct Vec v2){
  struct Vec v;
  v.x = v1.x + v2.x;
  v.y = v1.y + v2.y;
  return v;
}
