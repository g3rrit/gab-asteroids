#ifndef VEC_H
#define VEC_H

#include <tonc.h>

struct VEC {
  FIXED x;
  FIXED y;
};

void v_add(struct VEC* v, FIXED x, FIXED y);

void v_mul(struct VEC* v, FIXED a);

FIXED v_size_s(struct VEC* v);

int v_size_lt(struct VEC* v, FIXED a);

#endif /* VEC_H */
