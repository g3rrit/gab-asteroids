#include "vec.h"

void v_add(struct VEC* v, FIXED x, FIXED y)
{
  v->x = fxadd(v->x, x);
  v->y = fxadd(v->y, y);
}

void v_mul(struct VEC* v, FIXED a)
{
  v->x = fxmul(v->x, a);
  v->y = fxmul(v->y, a);
}

FIXED v_size_s(struct VEC* v)
{
  return fxadd(fxmul(v->x, v->x), fxmul(v->y, v->y));
}

int v_size_lt(struct VEC* v, FIXED a)
{
  return v_size_s(v) < a * a;
}
