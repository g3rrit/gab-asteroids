#ifndef DEFS_H
#define DEFS_H

#define SPRITE_SIZE 16

#define SCREEN_WIDTH 240
#define SCREEN_HEIGHT 160

#define PLAYER_ROT_SCALE 400 // higher means faster rotation
#define PLAYER_MOVE_SCALE 0.008 // higher means faster movement
#define PLAYER_MAX_ACC 0.1 // higher means faster movement

#define ACCELERATION_SCALE 100 // higher means slower deceleration

#define ASTEROID_MOVE_SCALE 10000

#define SHOT_RELOAD_TIME 50


#define TILE_POS(x, y) ((x) + ((y) * 16))


#define TILE_PLAYER TILE_POS(0, 0)
#define TILE_PLAYER_MOVE_END TILE_POS(3, 0)

#define TILE_AS1    TILE_POS(0, 1)
#define TILE_AS2    TILE_POS(1, 1)
#define TILE_AS3    TILE_POS(2, 1)
#define TILE_AS4    TILE_POS(3, 1)
#define TILE_AS5    TILE_POS(4, 1)
#define TILE_AS6    TILE_POS(5, 1)
#define TILE_AS7    TILE_POS(6, 1)
#define TILE_AS8    TILE_POS(7, 1)

#define TILE_AS_DESTROY TILE_POS(0, 2)
#define TILE_AS_DESTROY_END TILE_POS(2, 2)
#define AS_DESTROY_ANIM_COUNT 3

#define TILE_SHOT TILE_POS(0, 3)

#define TILE_PLAYER_DESTROY TILE_POS(0, 4)
#define TILE_PLAYER_DESTROY_END TILE_POS(5, 4)

#define TILE_CURSOR TILE_POS(0, 5)
#define TILE_CURSOR_END TILE_POS(1, 5)

#define OBJ_PLAYER 0
#define OBJ_AS1    1
#define OBJ_AS2    2
#define OBJ_AS3    3
#define OBJ_AS4    4
#define OBJ_AS5    5
#define OBJ_AS6    6
#define OBJ_AS7    7
#define OBJ_AS8    8

#define SPRITE_OFFSET ((SPRITE_SIZE / 2) + SPRITE_SIZE)

#endif /* DEFS_H */
