#ifndef GAME_H
#define GAME_H

#include "obj.h"

struct GAME_STATE {
  int as_count;
};

struct PLAYER {
  struct OBJ* obj;
  int is_destroyed;
};

struct SHOT {
  struct OBJ *obj;
  int is_active;
  int is_hot;
  int time;
};

struct ASTEROID {
  struct OBJ* obj;
  int destroyed_time;
  int rot_vel;
  int tile;
};

#define ASTEROID_COUNT 8

extern struct SHOT shot;
extern struct PLAYER player;
extern struct GAME_STATE game_state;
extern struct ASTEROID asteroids[ASTEROID_COUNT];


void game_update(void);

void game_init(void);

#endif /* GAME_H */
