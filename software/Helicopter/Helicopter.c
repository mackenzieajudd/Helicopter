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
	int transformedI;

	for ( i = (*helicopter).x1; i < (*helicopter).x2; i++)
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

			if((*map).map[transformedI][j] != *AIR)
			{
				return 1;
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

	(*helicopter).x1 += (*helicopter).vX / 8;
	(*helicopter).y1 += -1 * (*helicopter).vY / 8;
	(*helicopter).x2 += (*helicopter).vX / 8;
	(*helicopter).y2 += -1 * (*helicopter).vY / 8;

	(*helicopter).vX += (*helicopter).aX;
	(*helicopter).vY += (*helicopter).aY;
}
