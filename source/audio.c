#include "audio.h"

#include "cmn.h"

#include "level1.h"
#include "level2.h"
#include "level3.h"
#include "level4.h"
#include "level5.h"
#include "level6.h"
#include "level7.h"
#include "level8.h"

#define AUDIO_LOOP_OFFSET 0

unsigned int audio_vblanks_remaining = 0;
unsigned int audio_vblanks_total = 0;

vu32 const tracks[] = {
  (vu32)level1,
  (vu32)level2,
  (vu32)level3,
  (vu32)level4,
  (vu32)level5,
  (vu32)level6,
  (vu32)level7,
  (vu32)level8
};

#define TRACK_COUNT (sizeof(tracks) / sizeof(tracks[0]))

int track_index = 0;
int audio_active = 0;

void init_audio(void)
{
  // turn sound on
  REG_SNDSTAT = SSTAT_ENABLE;
  // snd1 on left/right ; both full volume
  REG_SNDDMGCNT = SDMG_BUILD_LR(SDMG_SQR1 | SDMG_SQR2, 7);
  // DMG ratio to 100%
  REG_SNDDSCNT = SDS_AR | SDS_AL | SDS_ARESET | SDS_DMG100;

  // no sweep
  REG_SND1SWEEP= SSW_OFF;

  REG_SND1CNT= SSQR_ENV_BUILD(12, 0, 2) | SSQR_DUTY1_8;
  REG_SND2CNT= SSQR_ENV_BUILD(7, 0, 1) | SSQR_DUTY1_8;
}

void audio_start(void)
{
  audio_active = 1;

#define CLOCK 16777216
#define CYCLES_PER_BLANK 280806

  unsigned short ticks_per_sample = CLOCK / 8000;
  audio_vblanks_remaining = level1_bytes * ticks_per_sample * (1.0 / CYCLES_PER_BLANK);
  audio_vblanks_total = audio_vblanks_remaining;

  REG_TM0D = 65536 - ticks_per_sample;
  REG_TM0CNT = TM_ENABLE | TM_FREQ_1;

  REG_DMA1SAD = tracks[track_index];
  REG_DMA1DAD = (vu32)(REG_BASE+0x00A0); // FIFO_A
  REG_DMA1CNT = DMA_DST_FIXED | DMA_REPEAT | DMA_32 | DMA_AT_SPECIAL | DMA_ENABLE;

  REG_SNDDSCNT = SDS_AR | SDS_AL | SDS_ARESET;
}

void audio_stop(void)
{
  audio_active = 0;
  REG_DMA1CNT = 0;
}

void audio_play_note(int channel, int note, int octave)
{
  switch (channel) {
    default:
    case 0:
      REG_SND1FREQ = SFREQ_RESET | SND_RATE(note, octave);
      break;
    case 1:
      REG_SND2FREQ = SFREQ_RESET | SND_RATE(note, octave);
      break;
  }
}

void audio_restart(void)
{
  if (audio_active == 0) {
    return;
  }

  audio_vblanks_remaining = audio_vblanks_total;

  REG_DMA1CNT = 0; // It is important to disable DMA before resetting it
  REG_DMA1SAD = tracks[track_index];
  REG_DMA1DAD = (vu32)(REG_BASE+0x00A0); // FIFO_A
  REG_DMA1CNT = DMA_DST_FIXED | DMA_REPEAT | DMA_32 | DMA_AT_SPECIAL | DMA_ENABLE;
}

void audio_update(void)
{
  if (audio_active == 0) {
    return;
  }

  if (audio_vblanks_remaining <= AUDIO_LOOP_OFFSET) {
    audio_restart();
  } else {
    audio_vblanks_remaining--;
  }
}

void audio_next(void)
{
  track_index = track_index >= TRACK_COUNT - 1 ? track_index : track_index + 1;
}

void audio_reset(void)
{
  track_index = 0;
}
