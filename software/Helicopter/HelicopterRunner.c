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

	//mapFile = fopen ("..\\Test_Map2.txt", "r");

	srand(time(NULL));
	//InitMapFromFile(&myMap, mapFile);
	InitMap(&myMap);
	InitHelicopter(&maChoppa, HELICOPTER_STARTING_POSITION_X, HELICOPTER_STARTING_POSITION_Y, HELICOPTER_STARTING_POSITION_X + HELICOPTER_SIZE_X, HELICOPTER_STARTING_POSITION_Y + HELICOPTER_SIZE_Y);

	InitHelicopter(&h1, HELICOPTER_STARTING_POSITION_X, HELICOPTER_STARTING_POSITION_Y, HELICOPTER_STARTING_POSITION_X + HELICOPTER_SIZE_X, HELICOPTER_STARTING_POSITION_Y + HELICOPTER_SIZE_Y);
	//InitHelicopter(&h2, HELICOPTER_STARTING_POSITION_X + 10, HELICOPTER_STARTING_POSITION_Y, HELICOPTER_STARTING_POSITION_X + HELICOPTER_SIZE_X + 10, HELICOPTER_STARTING_POSITION_Y + HELICOPTER_SIZE_Y);
	//InitHelicopter(&h3, HELICOPTER_STARTING_POSITION_X + 20, HELICOPTER_STARTING_POSITION_Y, HELICOPTER_STARTING_POSITION_X + HELICOPTER_SIZE_X + 20, HELICOPTER_STARTING_POSITION_Y + HELICOPTER_SIZE_Y);
	//InitHelicopter(&h4, HELICOPTER_STARTING_POSITION_X - 10, HELICOPTER_STARTING_POSITION_Y, HELICOPTER_STARTING_POSITION_X + HELICOPTER_SIZE_X - 10, HELICOPTER_STARTING_POSITION_Y + HELICOPTER_SIZE_Y);

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

		//StepMapFlat(&myMap);
		StepMapAlternating(&myMap, &i);

		DrawFlatMapQuick(&myMap, pixel_buffer);

		MoveHelicopter(&maChoppa, button2, pixel_buffer);
		//MoveHelicopter(&h1, button2, pixel_buffer);
		//MoveHelicopter(&h2, button2, pixel_buffer);
		//MoveHelicopter(&h3, button2, pixel_buffer);
		//MoveHelicopter(&h4, button2, pixel_buffer);

		DrawHelicopter(&maChoppa, pixel_buffer);
		//DrawHelicopter(&h1, pixel_buffer);
		//DrawHelicopter(&h2, pixel_buffer);
		//DrawHelicopter(&h3, pixel_buffer);
		//DrawHelicopter(&h4, pixel_buffer);

		DrawScore(&char_buffer, myMap.steps);

		SwapBuffers(&buffer_flag, BACK_PIXEL_BUFFER_BASE, PIXEL_BUFFER_BASE, &pixel_buffer);
		end_time = alt_timestamp();
		fps = (float)(end_time - start_time)/(float)alt_timestamp_freq();
		printf("FPS: %f\n", 1/fps);
		DrawFPS(&char_buffer, 1/fps);
		printf("Steps: %d\n", myMap.steps);
	}

  return 0;
}
