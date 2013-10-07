#include "Sound.h"

alt_up_audio_dev * sound_setup()
{
 alt_up_av_config_dev * av_config = alt_up_av_config_open_dev("/dev/audio_and_video_config_0");
  while (!alt_up_av_config_read_ready(av_config)) {

  }
  alt_up_audio_dev * audio = alt_up_audio_open_dev("/dev/audio_0");

  // this is to disable the write interupt first because at the begineering there is nothing in the buffer
  // so the space is > 75% and a write interupt(1) would be generated so we want to disable it first
  alt_up_audio_disable_write_interrupt(audio);

  return audio;
}

void PlayChoppaSound(alt_up_audio_dev * audio, unsigned int buf[], unsigned int buf1[])
{
	int soundFlag;

	if (alt_up_audio_write_fifo_space(audio, ALT_UP_AUDIO_LEFT) > 89){

	 soundFlag = alt_up_audio_write_interrupt_pending(audio);

	 if(soundFlag ==1)
	 {
		 alt_up_audio_write_fifo(audio, buf1, 2, ALT_UP_AUDIO_LEFT);
		 alt_up_audio_write_fifo(audio, buf1, 2, ALT_UP_AUDIO_RIGHT);
	 }
	 else
	 {
	 alt_up_audio_write_fifo(audio, buf, 7, ALT_UP_AUDIO_LEFT);
	 alt_up_audio_write_fifo(audio, buf, 7, ALT_UP_AUDIO_RIGHT);
	 int num = alt_up_audio_write_fifo_space(audio, ALT_UP_AUDIO_LEFT);
							 printf("%d\n", num);

	 }
 }
}
