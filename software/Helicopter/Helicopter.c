#include "Helicopter.h"
#include "Map.h"

void InitHelicopter(struct Helicopter* helicopter, int x1, int y1, int x2, int y2)
{
	(*helicopter).x1 = x1;
	(*helicopter).y1 = y1;
	(*helicopter).x2 = x2;
	(*helicopter).y2 = y2;

	(*helicopter).h1x1 = x1;
	(*helicopter).h1y1 = y1;
	(*helicopter).h1x2 = x2;
	(*helicopter).h1y2 = y2;

	(*helicopter).h2x1 = x1;
	(*helicopter).h2y1 = y1;
	(*helicopter).h2x2 = x2;
	(*helicopter).h2y2 = y2;

	(*helicopter).aX = 0;
	(*helicopter).aY = HELICOPTER_STARTING_ACCELERATION_Y;
	(*helicopter).vX = 0;
	(*helicopter).vY = 0;
}

int CheckForCollisions(struct Map* map, struct Helicopter* helicopter)
{
	int i;
	int j;
	int n;
	int collisionHeight = 0;
	int calcCollision = 0;
	int transformedI = 0;
	for(i = (*helicopter).x1; i < (*helicopter).x2; i++)
	{
		for(j = (*helicopter).y1; j < (*helicopter).y2; j++)
		{
			if(i + (*map).startingIndex > LENGTH_MAX)
			{
				transformedI = i + (*map).startingIndex - LENGTH_MAX;
			}
			else
			{
				transformedI = i + (*map).startingIndex;
			}
		}
		if((*map).map[transformedI][(*helicopter).y1] == *GROUND || (*map).map[transformedI][(*helicopter).y2] == *GROUND)
			calcCollision = 1;
		if(calcCollision == 1)
		{
			if((*helicopter).y1 <= 120) //check top
			{
				collisionHeight = HEIGHT_MAX;
				for(n = HEIGHT_MAX; (*map).map[transformedI][n] == *GROUND && n < 120; n++){}
				if(n > collisionHeight)
				{
					collisionHeight = n;
				}
			}
			else //check bottom
			{
				collisionHeight = HEIGHT_MIN;
				for(n = HEIGHT_MIN; (*map).map[transformedI][n] != *AIR && n > 120; n--){}
				if(n < collisionHeight)
				{
					collisionHeight = n;
				}
			}
		}
	}
	if(calcCollision)
	{
		if(collisionHeight <= 120)
		{
			(*helicopter).y1 = collisionHeight;
			(*helicopter).y2 = (*helicopter).y1 + 5;
			return 1;
		}
		else if(collisionHeight > 120)
		{
			(*helicopter).y2 = collisionHeight;
			(*helicopter).y1 = (*helicopter).y2 - 5;
			return 1;
		}
	}
	return 0;
}

int CheckForObstacleCollisions(struct Obstacle* obstacles[], struct Helicopter* helicopter)
{
	int i = 0;
	int j = 0;
	int k = 0;

	for(i = 0; i < NUM_OBSTACLES; i++)
	{
		if((*obstacles)[i].null != 0)
		{
			for(k = (*helicopter).x1; k <= (*helicopter).x2; k++)
			{
				for(j = (*helicopter).y1; j <= (*helicopter).y2; j++)
				{
					if(k >= (*obstacles)[i].x1 && k <= (*obstacles)[i].x2 && j <= (*obstacles)[i].y2 && j >= (*obstacles)[i].y1)
						return 1;
				}
			}
		}
	}
	return 0;
}

void DrawHelicopter(struct Helicopter* helicopter, alt_up_pixel_buffer_dma_dev* pixel_buffer)
{
	alt_up_pixel_buffer_dma_draw_rectangle(pixel_buffer, (*helicopter).h2x1, (*helicopter).h2y1, (*helicopter).h2x2, (*helicopter).h2y2, 0xFFFF, 1);
	alt_up_pixel_buffer_dma_draw_rectangle(pixel_buffer, (*helicopter).x1, (*helicopter).y1, (*helicopter).x2, (*helicopter).y2, 0xFF00, 1);
}

void DrawHelicopterFancy(struct Helicopter* helicopter, alt_up_pixel_buffer_dma_dev* pixel_buffer, int* chopperFlag)
{
	int i = 0;

	for(i = (*helicopter).h2x1; i <= (*helicopter).h2x2; i++)
	{
		alt_up_pixel_buffer_dma_draw_line(pixel_buffer, i, (*helicopter).h2y1, i, (*helicopter).h2y2, BACK_COLOR, 1);
	}

	if(*chopperFlag == 1)
	{
		alt_up_pixel_buffer_dma_draw_line(pixel_buffer, (*helicopter).x1 + 2, (*helicopter).y1, (*helicopter).x1 + 4, (*helicopter).y1, HELICOPTER_ROTER, 1);
		alt_up_pixel_buffer_dma_draw_line(pixel_buffer, (*helicopter).x1 + 5, (*helicopter).y1, (*helicopter).x2, (*helicopter).y1, HELICOPTER_COLOUR2, 1);
		alt_up_pixel_buffer_dma_draw(pixel_buffer, HELICOPTER_ROTER, (*helicopter).x1, (*helicopter).y1 + 1);
		alt_up_pixel_buffer_dma_draw(pixel_buffer, HELICOPTER_COLOUR2, (*helicopter).x1, (*helicopter).y1 + 3);
		*chopperFlag = 0;
	}
	else
	{
		alt_up_pixel_buffer_dma_draw_line(pixel_buffer, (*helicopter).x1 + 2, (*helicopter).y1, (*helicopter).x1 + 4, (*helicopter).y1, HELICOPTER_COLOUR2, 1);
		alt_up_pixel_buffer_dma_draw_line(pixel_buffer, (*helicopter).x1 + 5, (*helicopter).y1, (*helicopter).x2, (*helicopter).y1, HELICOPTER_ROTER, 1);
		alt_up_pixel_buffer_dma_draw(pixel_buffer, HELICOPTER_COLOUR2, (*helicopter).x1, (*helicopter).y1 + 1);
		alt_up_pixel_buffer_dma_draw(pixel_buffer, HELICOPTER_ROTER, (*helicopter).x1, (*helicopter).y1 + 3);
		*chopperFlag = 1;
	}
	alt_up_pixel_buffer_dma_draw(pixel_buffer, HELICOPTER_COLOUR, (*helicopter).x1 + 4, (*helicopter).y1 + 1);
	alt_up_pixel_buffer_dma_draw_rectangle(pixel_buffer, (*helicopter).x1 + 2, (*helicopter).y1 + 2, (*helicopter).x2, (*helicopter).y2, HELICOPTER_COLOUR, 1);
	alt_up_pixel_buffer_dma_draw_rectangle(pixel_buffer, (*helicopter).x1 + 3, (*helicopter).y1 + 3, (*helicopter).x2 - 1, (*helicopter).y2 - 1, HELICOPTER_COLOUR, 1);
	alt_up_pixel_buffer_dma_draw(pixel_buffer, 0x964B00, (*helicopter).x1 + 5, (*helicopter).y1 + 4);
	alt_up_pixel_buffer_dma_draw_line(pixel_buffer, (*helicopter).x1, (*helicopter).y1 + 2, (*helicopter).x1 + 2, (*helicopter).y1 + 2, HELICOPTER_COLOUR, 1);
}

void MoveHelicopter(struct Helicopter* helicopter, char* input, alt_up_pixel_buffer_dma_dev* pixel_buffer)
{
	if(*input == 0)
		(*helicopter).aY = 1;
	else
		(*helicopter).aY = -1;

	//Keeping track of historical positions in order to erase helicopter later...
	//Unsure if there is a better way
	(*helicopter).h2x1 = (*helicopter).h1x1;
	(*helicopter).h2y1 = (*helicopter).h1y1;
	(*helicopter).h2x2 = (*helicopter).h1x2;
	(*helicopter).h2y2 = (*helicopter).h1y2;

	(*helicopter).h1x1 = (*helicopter).x1;
	(*helicopter).h1y1 = (*helicopter).y1;
	(*helicopter).h1x2 = (*helicopter).x2;
	(*helicopter).h1y2 = (*helicopter).y2;

	(*helicopter).x1 += (*helicopter).vX / 2;
	(*helicopter).y1 += -1 * (*helicopter).vY / 2;
	(*helicopter).x2 += (*helicopter).vX / 2;
	(*helicopter).y2 += -1 * (*helicopter).vY / 2;

	(*helicopter).vX += (*helicopter).aX;
	(*helicopter).vY += (*helicopter).aY;
}

void MoveHelicopterCountdown(struct Helicopter* helicopter, alt_up_pixel_buffer_dma_dev* pixel_buffer)
{
	(*helicopter).h2y1 = (*helicopter).h1y1;
	(*helicopter).h2y2 = (*helicopter).h1y2;
	(*helicopter).h1y1 = (*helicopter).y1;
	(*helicopter).h1y2 = (*helicopter).y2;
	(*helicopter).y1--;
	(*helicopter).y2--;
}
