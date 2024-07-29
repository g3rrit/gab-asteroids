#include "cmn.h"

#include "obj.h"
#include "game.h"
#include "menu.h"

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
  memcpy32(&tile_mem[1][0], bgTiles, bgTilesLen / 4);
  memcpy32(pal_bg_mem, bgPal, bgPalLen / 4);
  REG_BG0CNT = BG_CBB(1) | BG_SBB(16) | BG_8BPP | BG_REG_32x32;
  for (int i = 0; i < 32 * 32; i++) {
    se_mem[16][i] = qran_range(0, 64);
  }
}

void init_text(void)
{
  //tte_init_chr4c_b4_default(0, BG_CBB(0) | BG_SBB(28));
  //tte_init_con();
  //tte_set_margins(8, 128, 232, 160);
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
}

struct {
  void (*init)(void);
  void (*update)(void);
} SCENES[2] = {
  {menu_init, menu_update},
  {game_init, game_update},
};

int main(void)
{
  init();

  while (1) {
    VBlankIntrWait();
    timer_last = timer;
    timer++;

    key_poll();

    if (old_scene != scene) {
      SCENES[scene].init();
      old_scene = scene;
    }

    SCENES[scene].update();
  }
}
