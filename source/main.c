#include "cmn.h"

#include "obj.h"
#include "game.h"
#include "menu.h"
#include "help.h"
#include "score.h"

#include "audio.h"

void init_sprites(void)
{
  memcpy32(&tile_mem[4][0], spritemapTiles, spritemapTilesLen / 4);
  memcpy32(pal_obj_mem, spritemapPal, spritemapPalLen / 4);

  // iterate over objects
  for (int i = 0; i < OBJ_COUNT; i++) {
    obj_init(&objs[i], i);
  }
}

void init_bg(void)
{
  memcpy32(&tile_mem[2][0], bgTiles, bgTilesLen / 4);
  memcpy32(pal_bg_mem, bgPal, bgPalLen / 4);
  REG_BG1CNT = BG_CBB(2) | BG_SBB(24) | BG_8BPP | BG_REG_32x32;
  for (int i = 0; i < 32 * 32; i++) {
    se_mem[24][i] = qran_range(0, 64);
  }
}

void init_text(void)
{
  tte_init_se_default(0, BG_CBB(0) | BG_SBB(8) | BG_PRIO(0));
  tte_init_con();
}

void init(void)
{
  irq_init(NULL);
  irq_enable(II_VBLANK);

  REG_DISPCNT = DCNT_MODE0 | DCNT_BG0 | DCNT_BG1 | DCNT_OBJ | DCNT_OBJ_2D;
  oam_init(obj_buffer, 128);

  init_sprites();
  init_bg();
  init_text(); // init text last so that it writes its colors into the palette last
  init_audio();
}

struct {
  void (*init)(void);
  void (*update)(void);
} SCENES[4] = {
  {menu_init, menu_update},
  {game_init, game_update},
  {help_init, help_update},
  {score_init, score_update},
};

int main(void)
{
  init();

  while (1) {
    VBlankIntrWait();
    timer_last = timer;
    timer++;

    audio_update();

    key_poll();

    if (old_scene != scene) {
      tte_erase_rect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
      SCENES[scene].init();
      old_scene = scene;
    }

    SCENES[scene].update();
  }
}
