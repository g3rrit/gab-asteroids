#include "menu.h"

#include "cmn.h"
#include "obj.h"

struct CURSOR cursor = {
  .obj = &objs[2]
};

void menu_update(void)
{
  scroll_bg();
  cursor.obj->tile ^= TIMER(8);
  obj_tile_update(cursor.obj);

  if (key_was_up(KEY_A) && key_is_down(KEY_A)) {
    scene = SCENE_GAME;
  }

  obj_copy(obj_mem, obj_buffer, OBJ_COUNT);
}

void menu_init(void)
{
  //tte_erase_rect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
  objs_disable();

  obj_enable(cursor.obj);
  cursor.obj->pos.x = int2fx(SCREEN_WIDTH / 2 - 60);
  cursor.obj->pos.y = int2fx(51);
  obj_pos_update(cursor.obj, 0);

  //tte_set_pos(75, 30);
  //tte_write("< ASTEROIDS >");

  //tte_set_pos(95, 60);
  //tte_write("start");

}
