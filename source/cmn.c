#include "cmn.h"

enum SCENE scene = SCENE_MENU;
enum SCENE old_scene = SCENE_START;

u32 timer_last = 0;
u32 timer = 1;

void scroll_bg(void)
{
  static int x;
  static int y;
  x += TIMER(32) * 1;
  y += TIMER(32) * 1;
  REG_BG0HOFS = x;
  REG_BG0VOFS = y;
}
