#include "help.h"

#include "cmn.h"
#include "obj.h"
#include "cursor.h"
#include "audio.h"

#define CURSOR_AT_BACK 124

void help_update(void)
{
  scroll_bg();
  cursor.obj->tile ^= TIMER(8);
  obj_tile_update(cursor.obj);

  if (key_was_up(KEY_A) && key_is_down(KEY_A)) {
    audio_play_note(1, NOTE_C, 1);
    scene = SCENE_MENU;
  }

  obj_copy(obj_mem, obj_buffer, OBJ_COUNT);
}

void help_init(void)
{
  audio_stop();
  objs_disable();

  obj_enable(cursor.obj);
  cursor.obj->pos.x = int2fx(SCREEN_WIDTH / 2 - 60);
  cursor.obj->pos.y = int2fx(CURSOR_AT_BACK);
  obj_pos_update(cursor.obj, 0);

  tte_set_pos(0, 20);
  tte_write(" Try to avoid the asteroids\n");
  tte_write(" by moving your spaceship\n");
  tte_write(" with the UP/LEFT/RIGHT keys\n\n");
  tte_write(" Shot the asteroids by\n");
  tte_write(" pressing A or B\n\n");
  tte_write(" The music gets progressively more tuneful up to level 7\n\n\n");
  tte_write("     Music inspired by\n\n");
  tte_write("   \"LOOK MUM NO COMPUTER\"");


  tte_set_pos(0, 140);
  tte_write("            back              ");
}
