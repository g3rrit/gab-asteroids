#include "menu.h"

#include "cmn.h"
#include "obj.h"
#include "cursor.h"
#include "audio.h"

#define CURSOR_AT_START 53
#define CURSOR_AT_HELP 78

int hover_start = 1;

void menu_update(void)
{
  scroll_bg();
  cursor.obj->tile ^= TIMER(8);
  obj_tile_update(cursor.obj);

  if (key_was_up(KEY_A) && key_is_down(KEY_A)) {
    audio_play_note(1, NOTE_C, 1);
    if (hover_start) {
      scene = SCENE_GAME;
    } else {
      scene = SCENE_HELP;
    }
  }

  if (key_was_up(KEY_DOWN) && key_is_down(KEY_DOWN)) {
    audio_play_note(1, NOTE_C, 0);
    cursor.obj->pos.y = int2fx(CURSOR_AT_HELP);
    obj_pos_update(cursor.obj, 0);
    hover_start = 0;
  }

  if (key_was_up(KEY_UP) && key_is_down(KEY_UP)) {
    audio_play_note(1, NOTE_C, 0);
    cursor.obj->pos.y = int2fx(CURSOR_AT_START);
    obj_pos_update(cursor.obj, 0);
    hover_start = 1;
  }

  obj_copy(obj_mem, obj_buffer, OBJ_COUNT);
}

void menu_init(void)
{
  audio_stop();
  hover_start = 1;
  objs_disable();

  obj_enable(cursor.obj);
  cursor.obj->pos.x = int2fx(SCREEN_WIDTH / 2 - 60);
  cursor.obj->pos.y = int2fx(CURSOR_AT_START);
  obj_pos_update(cursor.obj, 0);

  tte_set_pos(0, 30);
  tte_write("        < ASTEROIDS >        ");

  tte_set_pos(0, 70);
  tte_write("            start             ");

  tte_set_pos(0, 95);
  tte_write("            help              ");
}
