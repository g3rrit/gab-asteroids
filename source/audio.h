#ifndef AUDIO_H
#define AUDIO_H

void init_audio(void);

void audio_start(void);

void audio_stop(void);

void audio_play_note(int channel, int note, int octave);

void audio_update(void);

void audio_next(void);

void audio_reset(void);


#endif /* AUDIO_H */
