#ifndef OBSTACLE_H_
#define OBSTACLE_H_

#include "Map.h"
#include "Helicopter.h"

#define NUM_OBSTACLES 10

struct Obstacle
{
	int null;

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

	int vX;
	int vY;
};

void InitObstacle(struct Obstacle* obstacle, int x1, int y1, int x2, int y2);
void InitObstacles(struct Obstacle* obstacles[]);
void DrawObstacle(struct Obstacle* obstacle, alt_up_pixel_buffer_dma_dev* pixel_buffer);
void DrawObstacles(struct Obstacle* obstacles[], alt_up_pixel_buffer_dma_dev* pixel_buffer);
void MoveObstacle(struct Obstacle* obstacle);
void MoveObstacles(struct Obstacle* obstacles[]);
void ManageObstacles(struct Obstacle* obstacles[], alt_up_pixel_buffer_dma_dev* pixel_buffer);
void AddObstacle(struct Obstacle* obstacles[], struct Obstacle* obstacle);
void CopyByValue(struct Obstacle* ob1, struct Obstacle* ob2);


#endif
