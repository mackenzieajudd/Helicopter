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
#include "Menu.h"
#include "Obstacle.h"

#define button2 (volatile char *) 0x00004470
#define FPS 30

void wait(float);

int main()
{
	const int PIXEL_BUFFER_BASE = 0x00080000;
	const int BACK_PIXEL_BUFFER_BASE = 0x000C0000;
	const char* PIXEL_BUFFER_NAME = "/dev/video_pixel_buffer_dma_0";
	const char* CHAR_BUFFER_NAME = "/dev/video_character_buffer_with_dma_0";
	int i = 0;
	int j = 0;
	int mode = 0;
	int chopperFlag = 0;
	int start_time;
	int end_time;
	int highScore = 0;
	int buffer_flag = 0;
	alt_up_pixel_buffer_dma_dev* pixel_buffer;
	alt_up_char_buffer_dev* char_buffer;
	double fps;
	FILE* mapFile;
	struct Helicopter maChoppa;
	struct Map myMap;
	struct Obstacle block0;
	struct Obstacle block1;
	struct Obstacle block2;
	struct Obstacle obstacles[NUM_OBSTACLES];
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

	InitVGA(PIXEL_BUFFER_NAME, CHAR_BUFFER_NAME, &char_buffer, &pixel_buffer, BACK_PIXEL_BUFFER_BASE);
	DrawMenu(pixel_buffer, &char_buffer);
	SwapBuffers(&buffer_flag, BACK_PIXEL_BUFFER_BASE, PIXEL_BUFFER_BASE, &pixel_buffer);
	while(*button2 != 0)
				printf("Button: %d\n", *button2);
	alt_up_char_buffer_clear(char_buffer);

	while(1 == 1)
	{
		InitMap(&myMap);

		InitObstacles(&obstacles);

		InitHelicopter(&maChoppa, HELICOPTER_STARTING_POSITION_X, HELICOPTER_STARTING_POSITION_Y, HELICOPTER_STARTING_POSITION_X + HELICOPTER_SIZE_X, HELICOPTER_STARTING_POSITION_Y + HELICOPTER_SIZE_Y);

		DrawMap(&myMap, pixel_buffer);
		SwapBuffers(&buffer_flag, BACK_PIXEL_BUFFER_BASE, PIXEL_BUFFER_BASE, &pixel_buffer);
		DrawMap(&myMap, pixel_buffer);
		SwapBuffers(&buffer_flag, BACK_PIXEL_BUFFER_BASE, PIXEL_BUFFER_BASE, &pixel_buffer);

		DrawHelicopterFancy(&maChoppa, pixel_buffer, &chopperFlag);
		SwapBuffers(&buffer_flag, BACK_PIXEL_BUFFER_BASE, PIXEL_BUFFER_BASE, &pixel_buffer);
		DrawCountdown(pixel_buffer, &char_buffer);

		alt_up_char_buffer_clear(char_buffer);

		alt_timestamp_start();

		while(1 == 1)
		{
			fps = 0;

			if((float)(alt_timestamp())/(float)(alt_timestamp_freq()) > (float)1/(float)FPS)
			{
				start_time = alt_timestamp();

				StepMapAlternatingObstacles(&myMap, &i, &mode, &obstacles);
				DrawFlatMapQuick(&myMap, pixel_buffer);

				MoveHelicopter(&maChoppa, button2, pixel_buffer);

				MoveObstacles(&obstacles);
				ManageObstacles(&obstacles, pixel_buffer);
				DrawObstacles(&obstacles, pixel_buffer);

				if(CheckForCollisions(&myMap, &maChoppa) == 1 || CheckForObstacleCollisions(&obstacles, &maChoppa) == 1)
				{
					DrawHelicopterFancy(&maChoppa, pixel_buffer, &chopperFlag);
					SwapBuffers(&buffer_flag, BACK_PIXEL_BUFFER_BASE, PIXEL_BUFFER_BASE, &pixel_buffer);

					alt_up_pixel_buffer_dma_draw_rectangle(pixel_buffer, maChoppa.x1 + 2, maChoppa.y1 + 2, maChoppa.x2 - 2, maChoppa.y2 - 2, 0xF000, 0);
					wait(0.15);

					alt_up_pixel_buffer_dma_draw_rectangle(pixel_buffer, maChoppa.x1 + 1, maChoppa.y1 + 1, maChoppa.x2 - 1, maChoppa.y2 - 1, 0xF000, 0);
					wait(0.15);

					alt_up_pixel_buffer_dma_draw_rectangle(pixel_buffer, maChoppa.x1, maChoppa.y1, maChoppa.x2, maChoppa.y2, 0xF000, 0);
					wait(0.15);

					alt_up_pixel_buffer_dma_draw_rectangle(pixel_buffer, maChoppa.x1 - 1, maChoppa.y1 - 1, maChoppa.x2 + 1, maChoppa.y2 + 1, 0xF000, 0);
					wait(0.15);

					alt_up_pixel_buffer_dma_draw_rectangle(pixel_buffer, maChoppa.x1 - 2, maChoppa.y1 - 2, maChoppa.x2 + 2, maChoppa.y2 + 2, 0xF000, 0);
					wait(0.15);

					alt_up_pixel_buffer_dma_draw_rectangle(pixel_buffer, maChoppa.x1 + 2, maChoppa.y1 + 2, maChoppa.x2 - 2, maChoppa.y2 - 2, 0xFFFF, 0);
					wait(0.15);

					alt_up_pixel_buffer_dma_draw_rectangle(pixel_buffer, maChoppa.x1 + 1, maChoppa.y1 + 1, maChoppa.x2 - 1, maChoppa.y2 - 1, 0xFFFF, 0);
					wait(0.15);

					alt_up_pixel_buffer_dma_draw_rectangle(pixel_buffer, maChoppa.x1, maChoppa.y1, maChoppa.x2, maChoppa.y2, 0xFFFF, 0);
					wait(0.15);

					alt_up_pixel_buffer_dma_draw_rectangle(pixel_buffer, maChoppa.x1 - 1, maChoppa.y1 - 1, maChoppa.x2 + 1, maChoppa.y2 + 1, 0xFFFF, 0);
					wait(0.15);

					alt_up_pixel_buffer_dma_draw_rectangle(pixel_buffer, maChoppa.x1 - 2, maChoppa.y1 - 2, maChoppa.x2 + 2, maChoppa.y2 + 2, 0xFFFF, 0);
					wait(0.15);

					break;
				}
				DrawHelicopterFancy(&maChoppa, pixel_buffer, &chopperFlag);

				PlayChoppaSound(audio, buf, buf1);

				DrawScore(&char_buffer, myMap.steps);

				SwapBuffers(&buffer_flag, BACK_PIXEL_BUFFER_BASE, PIXEL_BUFFER_BASE, &pixel_buffer);

				end_time = alt_timestamp();
				fps = (float)1/((float)(end_time - start_time)/(float)alt_timestamp_freq());
				printf("MAX FPS: %f\n", fps);
				if( fps < FPS)
				{
					DrawFPS(&char_buffer, fps);
				}
				else
				{
					DrawFPS(&char_buffer, FPS);
				}

				alt_timestamp_start();
			}
		}

		wait(1);

		DrawCrash(pixel_buffer, &char_buffer, myMap.steps, &highScore);
		SwapBuffers(&buffer_flag, BACK_PIXEL_BUFFER_BASE, PIXEL_BUFFER_BASE, &pixel_buffer);
		alt_timestamp_start();
		while((float)(alt_timestamp())/(float)(alt_timestamp_freq()) < 2)
		{
		}

		while(*button2 != 0)
					printf("Button: %d\n", *button2);
		alt_up_char_buffer_clear(char_buffer);

	}

  return 0;
}

void wait(float seconds)
{
	alt_timestamp_start();

	while((float)(alt_timestamp())/(float)(alt_timestamp_freq()) < seconds)
	{
	}
}
