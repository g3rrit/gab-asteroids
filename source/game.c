#include "game.h"

#include "cmn.h"

struct GAME_STATE game_state = {
  .as_count = 0,
};

struct PLAYER player = {
  .obj=&objs[0],
  .is_destroyed = 0
};

struct SHOT shot = {
  .obj = &objs[1],
  .is_active = 0,
  .is_hot = 0,
  .time = 0
};

struct ASTEROID asteroids[ASTEROID_COUNT] = {
  { .obj=&objs[3], .destroyed_time=1, .tile=TILE_AS1 },
  { .obj=&objs[4], .destroyed_time=1, .tile=TILE_AS2 },
  { .obj=&objs[5], .destroyed_time=1, .tile=TILE_AS3 },
  { .obj=&objs[6], .destroyed_time=1, .tile=TILE_AS4 },
  { .obj=&objs[7], .destroyed_time=1, .tile=TILE_AS5 },
  { .obj=&objs[8], .destroyed_time=1, .tile=TILE_AS6 },
  { .obj=&objs[9], .destroyed_time=1, .tile=TILE_AS7 },
  { .obj=&objs[10], .destroyed_time=1, .tile=TILE_AS8 }
};

void player_update(void);

void shot_update(void);

void asteroid_update(struct ASTEROID* as);

void player_update(void)
{
  if (player.is_destroyed) {
    if (player.obj->tile >= TILE_PLAYER_DESTROY_END) {
      scene = SCENE_MENU;
      return;
    }

    player.obj->tile |= TILE_PLAYER_DESTROY;
    player.obj->tile += TIMER(2) * 1;
    obj_tile_update(player.obj);
    return;
  }

  // rotate
  player.obj->rot += PLAYER_ROT_SCALE * !!key_is_down(KEY_LEFT) - PLAYER_ROT_SCALE * !!key_is_down(KEY_RIGHT);

  int sin = -fxdiv(int2fx(lu_sin(player.obj->rot * 2)), 4096);
  int cos = -fxdiv(int2fx(lu_cos(player.obj->rot * 2)), 4096);
  sin = fxmul(sin, float2fx(PLAYER_MOVE_SCALE));
  cos = fxmul(cos, float2fx(PLAYER_MOVE_SCALE));

  if (key_is_down(KEY_UP)) {
    player.obj->acc.x = fxdiv(sin, int2fx(10));
    player.obj->acc.y = fxdiv(cos, int2fx(10));

    player.obj->tile = player.obj->tile * (player.obj->tile < TILE_PLAYER_MOVE_END);
    player.obj->tile += 1;
  } else {
    player.obj->acc.x = 0;
    player.obj->acc.y = 0;

    player.obj->tile = TILE_PLAYER;
  }

  obj_tile_update(player.obj);

  obj_pos_update(player.obj, 1);
  obj_rot_update(player.obj);
}

void shot_update(void)
{
  if (!shot.is_active) {
    if (!key_is_down(KEY_A)) {
        return;
    }
    shot.is_active = 1;
    shot.is_hot = 1;
    obj_enable(shot.obj);
    shot.time = SHOT_RELOAD_TIME;
    shot.obj->pos.x = player.obj->pos.x;
    shot.obj->pos.y = player.obj->pos.y;

    int sin = fxdiv(int2fx(lu_sin(player.obj->rot * 2)), 4096);
    int cos = fxdiv(int2fx(lu_cos(player.obj->rot * 2)), 4096);
    sin = fxmul(sin, float2fx(PLAYER_MOVE_SCALE * 2));
    cos = fxmul(cos, float2fx(PLAYER_MOVE_SCALE * 2));

    shot.obj->vel.x = -sin;
    shot.obj->vel.y = -cos;
  }
  if (shot.time <= 0 && shot.is_hot == 0) {
    shot.is_active = 0;
    return;
  }
  shot.time--;

  int d_x = fx2int(shot.obj->pos.x);
  int d_y = fx2int(shot.obj->pos.y);

  // why don't we need SPRITE_OFFSET here?
  if (d_x >= SCREEN_WIDTH - SPRITE_SIZE ||
      d_x <= -SPRITE_SIZE ||
      d_y >= SCREEN_HEIGHT - SPRITE_SIZE ||
      d_y <= -SPRITE_SIZE) {
    shot.is_hot = 0;
    obj_disable(shot.obj);
    return;
  }

  obj_pos_update(shot.obj, 0);
}

void asteroid_update(struct ASTEROID* as)
{
  // negative: asteroid is destroyed
  if (as->destroyed_time < 0) {
    // reset asteroid
    if (as->destroyed_time < -AS_DESTROY_ANIM_COUNT * 10) {
      as->destroyed_time = qran_range(20, 200);
      obj_disable(as->obj);
      return;
    }
    as->obj->tile = TILE_POS(-as->destroyed_time / 10, 2);
    obj_tile_update(as->obj);
    as->destroyed_time--;
    return;
  }

  // positive: wait for asteroid to respawn
  if (as->destroyed_time > 1) {
    as->destroyed_time--;
    return;
  }

  // == 1: asteroid just respawned
  if (as->destroyed_time == 1) {
    as->destroyed_time = 0;
    obj_enable(as->obj);
    as->obj->tile = as->tile;
    obj_tile_update(as->obj);

    int px = fx2int(player.obj->pos.x) + SPRITE_OFFSET;

    if (px > SCREEN_WIDTH / 2) {
      as->obj->pos.x = int2fx(px - SCREEN_WIDTH / 2);
    } else {
      as->obj->pos.x = int2fx(px + SCREEN_WIDTH / 2);
    }
    as->obj->pos.y = 0 - int2fx(SPRITE_OFFSET);

    as->obj->vel.x = fxdiv(int2fx(qran_range(-100, 100)), ASTEROID_MOVE_SCALE);
    as->obj->vel.y = fxdiv(int2fx(qran_range(-100, 100)), ASTEROID_MOVE_SCALE);

    as->rot_vel = qran_range(-100, 100);
  }

  if (shot.is_hot && obj_check_coll(as->obj, shot.obj)) {
    as->destroyed_time = -1;
  }

  if (obj_check_coll(as->obj, player.obj)) {
    player.is_destroyed = 1;
  }

  as->obj->rot += as->rot_vel;

  obj_rot_update(as->obj);
  obj_pos_update(as->obj, 1);
}

void game_update(void)
{
  scroll_bg();
  player_update();
  shot_update();

  for (int i = 0; i < game_state.as_count; i++) {
    asteroid_update(&asteroids[i]);
  }

  obj_copy(obj_mem, obj_buffer, OBJ_COUNT);
  obj_aff_copy(obj_aff_mem, obj_aff_buffer, OBJ_COUNT);

  // TODO: fix size
  //tte_erase_rect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
  //tte_printf("#{es;P:0,0}FPS: %d:%d", fx2int(player.obj->acc.x), fx2int(player.obj->acc.y));
}

void game_init(void)
{
  //tte_erase_rect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
  objs_disable();
  sqran(timer);

  // init game state
  game_state.as_count = 2;

  // init player
  player.obj->pos.x = int2fx((SCREEN_WIDTH / 2) - SPRITE_OFFSET);
  player.obj->pos.y = int2fx((SCREEN_HEIGHT / 2) - SPRITE_OFFSET);
  player.is_destroyed = 0;
  player.obj->vel.x = 0;
  player.obj->vel.y = 0;
  player.obj->acc.x = 0;
  player.obj->acc.y = 0;
  player.obj->drag = float2fx(0.98);
  player.obj->tile = TILE_PLAYER;
  player.obj->rot = 0;
  obj_tile_update(player.obj);
  obj_enable(player.obj);

  // init asteroids
  for (int i = 0; i < ASTEROID_COUNT; i++) {
    struct ASTEROID *as = &asteroids[i];
    as->destroyed_time = 1;
  }
}

