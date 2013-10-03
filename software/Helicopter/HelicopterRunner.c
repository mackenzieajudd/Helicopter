#include <stdio.h>
#include "drivers/inc/altera_up_avalon_video_character_buffer_with_dma.h"
#include "drivers/inc/altera_up_avalon_video_pixel_buffer_dma.h"
#include "Map.h"
#include "VGA.h"
#include "Helicopter.h"
#include "HAL/inc/sys/alt_alarm.h"
#include "HAL/inc/sys/alt_timestamp.h"
#include "io.h"

#define button2 (volatile char *) 0x00004070

int main()
{
	const int PIXEL_BUFFER_BASE = 0x00080000;
	const int BACK_PIXEL_BUFFER_BASE = 0x000C0000;
	const char* PIXEL_BUFFER_NAME = "/dev/video_pixel_buffer_dma_0";
	const char* CHAR_BUFFER_NAME = "/dev/video_character_buffer_with_dma_0";
	int i;
	int j;
	int start_time;
	int end_time;
	int buffer_flag = 0;
	alt_up_pixel_buffer_dma_dev* pixel_buffer;
	alt_up_char_buffer_dev* char_buffer;
	double fps;
	FILE* mapFile;

	struct Helicopter maChoppa;
	struct Map myMap;

	//mapFile = fopen ("..\\Test_Map2.txt", "r");

	srand(time(NULL));
	//InitMapFromFile(&myMap, mapFile);
	InitMap(&myMap);
	InitHelicopter(&maChoppa);

	InitVGA(PIXEL_BUFFER_NAME, CHAR_BUFFER_NAME, &char_buffer, &pixel_buffer, BACK_PIXEL_BUFFER_BASE);

	DrawMap(&myMap, pixel_buffer);
	SwapBuffers(&buffer_flag, BACK_PIXEL_BUFFER_BASE, PIXEL_BUFFER_BASE, &pixel_buffer);
	DrawMap(&myMap, pixel_buffer);
	SwapBuffers(&buffer_flag, BACK_PIXEL_BUFFER_BASE, PIXEL_BUFFER_BASE, &pixel_buffer);

	while(*button2 != 0)
		printf("Button: %d\n", *button2);

	while(CheckForCollisions(&myMap, &maChoppa) != 1)
	{
		alt_timestamp_start();
		fps = 0;
		start_time = alt_timestamp();

		StepMapFlat(&myMap);

		DrawFlatMapQuick(&myMap, pixel_buffer);

		MoveHelicopter(&maChoppa, button2, pixel_buffer);

		DrawHelicopter(&maChoppa, pixel_buffer);

		DrawScore(&char_buffer, myMap.steps);

		SwapBuffers(&buffer_flag, BACK_PIXEL_BUFFER_BASE, PIXEL_BUFFER_BASE, &pixel_buffer);
		end_time = alt_timestamp();
		fps = (float)(end_time - start_time)/(float)alt_timestamp_freq();
		printf("FPS: %f\n", 1/fps);
		printf("Steps: %d\n", myMap.steps);
	}

  return 0;
}
