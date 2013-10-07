#include <stdio.h>
#include "drivers/inc/altera_up_avalon_video_character_buffer_with_dma.h"
#include "drivers/inc/altera_up_avalon_video_pixel_buffer_dma.h"
#include "Map.h"
#include "VGA.h"
#include "Helicopter.h"
#include "HAL/inc/sys/alt_alarm.h"
#include "HAL/inc/sys/alt_timestamp.h"
#include "io.h"
#include <altera_up_avalon_audio_and_video_config.h>
#include <altera_up_avalon_audio.h>
#include "Sound.h"

#define button2 (volatile char *) 0x00004070
#define FPS 20

int main()
{
	const int PIXEL_BUFFER_BASE = 0x00080000;
	const int BACK_PIXEL_BUFFER_BASE = 0x000C0000;
	const char* PIXEL_BUFFER_NAME = "/dev/video_pixel_buffer_dma_0";
	const char* CHAR_BUFFER_NAME = "/dev/video_character_buffer_with_dma_0";
	int i = 0;
	int j = 0;
	int start_time;
	int end_time;
	int buffer_flag = 0;
	alt_up_pixel_buffer_dma_dev* pixel_buffer;
	alt_up_char_buffer_dev* char_buffer;
	double fps;
	FILE* mapFile;

	struct Helicopter maChoppa;
	struct Helicopter h1;
	struct Helicopter h2;
	struct Helicopter h3;
	struct Helicopter h4;
	struct Map myMap;

	 unsigned int buf [7];
	 unsigned int buf1 [2];
	 alt_up_audio_dev * audio = sound_setup();

	 buf[0] = 0x00000FFF;
	 buf[1] = 0x00000FFF;
	 buf[2] = 0x00000FFF;
	 buf[3] = 0x00000FFF;
	 buf[4] = 0x00000FFF;
	 buf[5] = 0x00000FFF;
	 buf[6] = 0x00000001;

	 buf1[0] = 0x00000000;
	 buf1[1] = 0x00000000;

	srand(time(NULL));
	//InitMapFromFile(&myMap, mapFile);
	InitMap(&myMap);
	InitHelicopter(&maChoppa, HELICOPTER_STARTING_POSITION_X, HELICOPTER_STARTING_POSITION_Y, HELICOPTER_STARTING_POSITION_X + HELICOPTER_SIZE_X, HELICOPTER_STARTING_POSITION_Y + HELICOPTER_SIZE_Y);

	InitHelicopter(&h1, HELICOPTER_STARTING_POSITION_X, HELICOPTER_STARTING_POSITION_Y, HELICOPTER_STARTING_POSITION_X + HELICOPTER_SIZE_X, HELICOPTER_STARTING_POSITION_Y + HELICOPTER_SIZE_Y);

	InitVGA(PIXEL_BUFFER_NAME, CHAR_BUFFER_NAME, &char_buffer, &pixel_buffer, BACK_PIXEL_BUFFER_BASE);

	DrawMap(&myMap, pixel_buffer);
	SwapBuffers(&buffer_flag, BACK_PIXEL_BUFFER_BASE, PIXEL_BUFFER_BASE, &pixel_buffer);
	DrawMap(&myMap, pixel_buffer);
	SwapBuffers(&buffer_flag, BACK_PIXEL_BUFFER_BASE, PIXEL_BUFFER_BASE, &pixel_buffer);

	while(*button2 != 0)
		printf("Button: %d\n", *button2);

	alt_timestamp_start();

	while(CheckForCollisions(&myMap, &maChoppa) != 1)
	{
		fps = 0;
		start_time = alt_timestamp();

		if((float)(alt_timestamp())/(float)(alt_timestamp_freq()) > (float)1/(float)FPS)
		{
			StepMapAlternating(&myMap, &i);

			DrawFlatMapQuick(&myMap, pixel_buffer);

			MoveHelicopter(&maChoppa, button2, pixel_buffer);

			DrawHelicopter(&maChoppa, pixel_buffer);

			PlayChoppaSound(audio, buf, buf1);

			DrawScore(&char_buffer, myMap.steps);
			DrawFPS(&char_buffer, FPS);

			SwapBuffers(&buffer_flag, BACK_PIXEL_BUFFER_BASE, PIXEL_BUFFER_BASE, &pixel_buffer);

			alt_timestamp_start();
		}

		end_time = alt_timestamp();
		//fps = (float)(end_time - start_time)/(float)alt_timestamp_freq();
		//printf("Clock: %f\n", (float)(alt_timestamp())/(float)(alt_timestamp_freq()));
		//printf("FPS: %f\n", (float)1/(float)FPS);
		//printf("FPS2: %f\n", (float)alt_timestamp());
		//printf("FPS3: %f\n", (float)alt_timestamp_freq());

		//printf("Steps: %d\n", myMap.steps);
	}

  return 0;
}
