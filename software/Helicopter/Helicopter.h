#ifndef HELICOPTER_H_
#define HELICOPTER_H_

#define HELICOPTER_STARTING_POSITION_X 155
#define HELICOPTER_STARTING_POSITION_Y 115
#define HELICOPTER_SIZE_X 5
#define HELICOPTER_SIZE_Y 5
#define HELICOPTER_STARTING_ACCELERATION_Y -1

#include "Map.h"

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

void InitHelicopter(struct Helicopter* helicopter);
int CheckForCollisions(struct Map* map, struct Helicopter* helicopter);
void MoveHelicopter(struct Helicopter* helicopter, char* input, alt_up_pixel_buffer_dma_dev* pixel_buffer);
void DrawHelicopter(struct Helicopter* helicopter, alt_up_pixel_buffer_dma_dev* pixel_buffer);

#endif
