#ifndef HELICOPTER_H_
#define HELICOPTER_H_

#define HELICOPTER_STARTING_POSITION_X 155
#define HELICOPTER_STARTING_POSITION_Y 115
#define HELICOPTER_SIZE_X 6
#define HELICOPTER_SIZE_Y 5
#define HELICOPTER_STARTING_ACCELERATION_Y -1

#define HELICOPTER_COLOUR 0x6600FF

#include "Map.h"
#include "Obstacle.h"

struct Helicopter
{
	int x1;
	int x2;
	int y1;
	int y2;

	//For keeping track of historical positions
	int h1x1;
	int h1x2;
	int h1y1;
	int h1y2;
	int h2x1;
	int h2x2;
	int h2y1;
	int h2y2;

	int aX;
	int aY;

	int vX;
	int vY;
};

void InitHelicopter(struct Helicopter* helicopter, int x1, int y1, int x2, int y2);
int CheckForCollisions(struct Map* map, struct Helicopter* helicopter);
void MoveHelicopter(struct Helicopter* helicopter, char* input, alt_up_pixel_buffer_dma_dev* pixel_buffer);
void DrawHelicopter(struct Helicopter* helicopter, alt_up_pixel_buffer_dma_dev* pixel_buffer);
void DrawHelicopterFancy(struct Helicopter* helicopter, alt_up_pixel_buffer_dma_dev* pixel_buffer, int* chopperFlag);
int CheckForObstacleCollisions(struct Obstacle* obstacles[], struct Helicopter* helicopter);

#endif
