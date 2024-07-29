#include "score.h"

#include "cmn.h"
#include "obj.h"
#include "cursor.h"
#include "game.h"

#define CURSOR_AT_BACK 124

void score_update(void)
{
  scroll_bg();
  cursor.obj->tile ^= TIMER(8);
  obj_tile_update(cursor.obj);

  if (key_was_up(KEY_A) && key_is_down(KEY_A)) {
    scene = SCENE_MENU;
  }

  obj_copy(obj_mem, obj_buffer, OBJ_COUNT);
}

void score_init(void)
{
  objs_disable();

  obj_enable(cursor.obj);
  cursor.obj->pos.x = int2fx(SCREEN_WIDTH / 2 - 60);
  cursor.obj->pos.y = int2fx(CURSOR_AT_BACK);
  obj_pos_update(cursor.obj, 0);

  tte_set_pos(0, 40);
  tte_write("            SCORE:\n\n");
  tte_printf("              %d\n\n\n", game_state.score);
  tte_write("            LEVEL:\n\n");
  tte_printf("              %d\n", game_state.level + 1);

  tte_set_pos(0, 140);
  tte_write("            back              ");
}
