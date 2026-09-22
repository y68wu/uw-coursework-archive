#ifndef VEC_H
#define VEC_H
struct Vec {
  int x;
  int y;
};

struct Vec add(const struct Vec v1, const struct Vec v2);
#endif