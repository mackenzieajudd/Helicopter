#ifndef SOUND_H_
#define SOUND_H_

#include <altera_up_avalon_audio_and_video_config.h>
#include <altera_up_avalon_audio.h>

alt_up_audio_dev * sound_setup();
void PlayChoppaSound(alt_up_audio_dev * audio, unsigned int buf[], unsigned int buf1[]);

#endif
