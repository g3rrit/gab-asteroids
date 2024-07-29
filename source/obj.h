#ifndef OBJ_H
#define OBJ_H

#include <tonc.h>
#include "vec.h"
#include "defs.h"

// 32 objects with affine transformation
extern OBJ_ATTR obj_buffer[128];
extern OBJ_AFFINE *obj_aff_buffer;

struct OBJ {
  struct VEC pos;
  struct VEC vel;
  struct VEC acc;
  FIXED drag;
  int size; // i.e. radius

  int rot;

  OBJ_ATTR *attr;
  OBJ_AFFINE *aff;
  OBJ_AFFINE aff_base;
  int tile;
};

#define OBJ_COUNT 11

extern struct OBJ objs[OBJ_COUNT];

void obj_disable(struct OBJ *obj);

void obj_enable(struct OBJ *obj);

void objs_disable(void);

void obj_init(struct OBJ* obj, int index);

void obj_tile_update(struct OBJ *obj);

void obj_rot_update(struct OBJ* obj);

void obj_pos_wrap(struct OBJ* obj);

int obj_check_coll(struct OBJ *a, struct OBJ *b);

void obj_pos_update(struct OBJ* obj, int wrap);

#endif /* OBJ_H */
