#include "obj.h"

OBJ_ATTR obj_buffer[128];
OBJ_AFFINE *obj_aff_buffer = (OBJ_AFFINE*)obj_buffer;

struct OBJ objs[OBJ_COUNT] = {
  { .attr=&obj_buffer[0], .aff=(OBJ_AFFINE*)&obj_buffer[0 * 4], .tile=TILE_PLAYER, .size=10 },
  { .attr=&obj_buffer[1], .aff=(OBJ_AFFINE*)&obj_buffer[1 * 4], .tile=TILE_SHOT, .size=10 },
  { .attr=&obj_buffer[2], .aff=(OBJ_AFFINE*)&obj_buffer[2 * 4], .tile=TILE_CURSOR, .size=0 },
  { .attr=&obj_buffer[3], .aff=(OBJ_AFFINE*)&obj_buffer[3 * 4], .tile=TILE_AS1, .size=3 },
  { .attr=&obj_buffer[4], .aff=(OBJ_AFFINE*)&obj_buffer[4 * 4], .tile=TILE_AS2, .size=3 },
  { .attr=&obj_buffer[5], .aff=(OBJ_AFFINE*)&obj_buffer[5 * 4], .tile=TILE_AS3, .size=3 },
  { .attr=&obj_buffer[6], .aff=(OBJ_AFFINE*)&obj_buffer[6 * 4], .tile=TILE_AS4, .size=3 },
  { .attr=&obj_buffer[7], .aff=(OBJ_AFFINE*)&obj_buffer[7 * 4], .tile=TILE_AS5, .size=3 },
  { .attr=&obj_buffer[8], .aff=(OBJ_AFFINE*)&obj_buffer[8 * 4], .tile=TILE_AS6, .size=3 },
  { .attr=&obj_buffer[9], .aff=(OBJ_AFFINE*)&obj_buffer[9 * 4], .tile=TILE_AS7, .size=3 },
  { .attr=&obj_buffer[10], .aff=(OBJ_AFFINE*)&obj_buffer[10 * 4], .tile=TILE_AS8, .size=3 }
};

void obj_disable(struct OBJ *obj)
{
  obj->attr->attr0 = ATTR0_SQUARE | ATTR0_8BPP | ATTR0_HIDE;
}

void obj_enable(struct OBJ *obj)
{
  obj->attr->attr0 = ATTR0_SQUARE | ATTR0_8BPP | ATTR0_AFF_DBL;
}

void objs_disable(void)
{
  for (int i = 0; i < OBJ_COUNT; i++) {
    obj_disable(&objs[i]);
  }
}

void obj_init(struct OBJ* obj, int index)
{
  obj_set_attr(
    obj->attr,
    ATTR0_SQUARE | ATTR0_8BPP | ATTR0_HIDE, // leave out ATTR0_AFF_DBL initially
    ATTR1_SIZE_16 | ATTR1_AFF_ID(index),
    ATTR2_PALBANK(0) | ATTR2_ID(obj->tile * 4)
  );
  obj_set_pos(obj->attr, 0, 0);
  obj_aff_identity(obj->aff);
  obj_aff_identity(&obj->aff_base);

  obj->pos.x = 0;
  obj->pos.y = 0;
  obj->vel.x = 0;
  obj->vel.y = 0;
  obj->acc.x = 0;
  obj->acc.y = 0;
  obj->drag = int2fx(1);
}

void obj_tile_update(struct OBJ *obj)
{
  obj->attr->attr2 = ATTR2_PALBANK(0) | ATTR2_ID(obj->tile * 4);
}

void obj_rot_update(struct OBJ* obj)
{
  obj_aff_rotate(&obj->aff_base, obj->rot);
  obj_aff_copy(obj->aff, &obj->aff_base, 1);
  obj_aff_postmul(obj->aff, &obj->aff_base);
}

void obj_pos_wrap(struct OBJ* obj)
{
  // Due to AFF_DBL, we need to check for double the sprites size
#define SCREEN_LEFT_OUT (-(SPRITE_SIZE + (SPRITE_SIZE / 2)))
#define SCREEN_RIGHT_OUT (SCREEN_WIDTH - (SPRITE_SIZE / 2))
#define SCREEN_TOP_OUT (-(SPRITE_SIZE + (SPRITE_SIZE / 2)))
#define SCREEN_BOTTOM_OUT (SCREEN_HEIGHT - (SPRITE_SIZE / 2))

  int discrete_x = fx2int(obj->pos.x);
  int discrete_y = fx2int(obj->pos.y);

  if (discrete_x < SCREEN_LEFT_OUT) obj->pos.x = int2fx(SCREEN_RIGHT_OUT);
  else if (discrete_x > SCREEN_RIGHT_OUT) obj->pos.x = int2fx(SCREEN_LEFT_OUT);

  if (discrete_y < SCREEN_TOP_OUT) obj->pos.y = int2fx(SCREEN_BOTTOM_OUT);
  else if (discrete_y > SCREEN_BOTTOM_OUT) obj->pos.y = int2fx(SCREEN_TOP_OUT);
}

int obj_check_coll(struct OBJ *a, struct OBJ *b)
{
  int dx = fx2int(a->pos.x - b->pos.x);
  int dy = fx2int(a->pos.y - b->pos.y);
  int dc = a->size + b->size;

  return (dx * dx + dy * dy <= dc * dc);
}

void obj_pos_update(struct OBJ* obj, int wrap)
{
  // update vel
  v_add(&obj->vel, obj->acc.x, obj->acc.y);

  // apply drag
  obj->vel.x = fxmul(obj->vel.x, obj->drag);
  obj->vel.y = fxmul(obj->vel.y, obj->drag);

  // update pos
  v_add(&obj->pos, obj->vel.x, obj->vel.y);

  if (wrap) {
    obj_pos_wrap(obj);
  }

  obj_set_pos(obj->attr, fx2int(obj->pos.x), fx2int(obj->pos.y));
}

