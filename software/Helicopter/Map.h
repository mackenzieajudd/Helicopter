#ifndef MAP_H_
#define MAP_H_

#define GROUND "0"
#define AIR "1"
#define BASE_DIFFUCULTY 90
#define HEIGHT_MAX 0
#define HEIGHT_MIN 239
#define LENGTH_MAX 319
#define LENGTH_MIN 0
#define STEP_SIZE 3
#define MAX_FLUX 7
#define SEGMENT_LENGTH 700

//#define GROUND_COLOR 0x8A22
#define GROUND_COLOR 0x40E0
#define BACK_COLOR 0x000000

#define FILE0 "Map0.txt"
#define FILE1 "Map2.txt"
#define FILE2 "Map4.txt"
#define FILE3 "Map2.txt"
#define FILE4 "Map4.txt"
#define FILE5 "Map3.txt"

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "drivers/inc/altera_up_avalon_video_character_buffer_with_dma.h"
#include "drivers/inc/altera_up_avalon_video_pixel_buffer_dma.h"
#include "math.h"
#include "Obstacle.h"
#include "SD.h"
#include <altera_up_sd_card_avalon_interface.h>

struct Map
{
	char map[LENGTH_MAX + 1][HEIGHT_MIN + 1];
	int startingIndex;
	int steps;
};

int getRoof(struct Map* map, int x);
int getFloor(struct Map* map, int x);
void InitMap(struct Map* map);
void StepMap(struct Map* map);
void StepMapFlat(struct Map* map);
int GetRandHeight(int steps, int difficulty);
void StepMapAlternatingObstacles(struct Map* map, int* altCounter, int* mode, struct Obstacle* obstacles[]);
void StepBestMapEver(struct Map* map, int* mode, struct Obstacle* obstacles[]);
void InitMapFromFile(struct Map* map, char* fileName, int* mapHandle);
void StepMapFromFile(struct Map* map, int* mapHandle, struct Obstacle* obstacles[], int* mapPointer);
void SwapMaps(int* mapHandle, int* mapPointer);

void DrawMap(struct Map* map, alt_up_pixel_buffer_dma_dev* pixel_buffer);
void DrawMapQuick(struct Map* map, alt_up_pixel_buffer_dma_dev* pixel_buffer);
void DrawFlatMapQuick(struct Map* map, alt_up_pixel_buffer_dma_dev* pixel_buffer);
void DrawMapInConsole(struct Map* map);

#endif
