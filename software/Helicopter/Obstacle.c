#include "Obstacle.h"

void InitObstacle(struct Obstacle* obstacle, int x1, int y1, int x2, int y2)
{
	(*obstacle).null = 1;

	(*obstacle).x1 = x1;
	(*obstacle).y1 = y1;
	(*obstacle).x2 = x2;
	(*obstacle).y2 = y2;

	(*obstacle).h1x1 = x1;
	(*obstacle).h1y1 = y1;
	(*obstacle).h1x2 = x2;
	(*obstacle).h1y2 = y2;

	(*obstacle).h2x1 = x1;
	(*obstacle).h2y1 = y1;
	(*obstacle).h2x2 = x2;
	(*obstacle).h2y2 = y2;

	(*obstacle).vX = -1 * STEP_SIZE;
	(*obstacle).vY = 0;
}

void MoveObstacle(struct Obstacle* obstacle)
{
	(*obstacle).h2x1 = (*obstacle).h1x1;
	(*obstacle).h2y1 = (*obstacle).h1y1;
	(*obstacle).h2x2 = (*obstacle).h1x2;
	(*obstacle).h2y2 = (*obstacle).h1y2;

	(*obstacle).h1x1 = (*obstacle).x1;
	(*obstacle).h1y1 = (*obstacle).y1;
	(*obstacle).h1x2 = (*obstacle).x2;
	(*obstacle).h1y2 = (*obstacle).y2;

	(*obstacle).x1 += (*obstacle).vX;
	(*obstacle).y1 += -1 * (*obstacle).vY;
	(*obstacle).x2 += (*obstacle).vX;
	(*obstacle).y2 += -1 * (*obstacle).vY;

	if((*obstacle).x1 <= 0)
		(*obstacle).x1 = 0;
	if((*obstacle).x2 <= 0)
		(*obstacle).x2 = 0;

}

void DrawObstacle(struct Obstacle* obstacle, alt_up_pixel_buffer_dma_dev* pixel_buffer)
{
	int i;

	for(i = (*obstacle).h2x1; i < (*obstacle).h2x2; i++)
	{
		alt_up_pixel_buffer_dma_draw_line(pixel_buffer, i, (*obstacle).h2y1, i, (*obstacle).h2y2, BACK_COLOR, 1);
	}
	for(i = (*obstacle).x1; i < (*obstacle).x2; i++)
	{
		alt_up_pixel_buffer_dma_draw_line(pixel_buffer, i, (*obstacle).y1, i, (*obstacle).y2, GROUND_COLOR, 1);
	}
}

void ManageObstacles(struct Obstacle* obstacles[], alt_up_pixel_buffer_dma_dev* pixel_buffer)
{
	int i;

	for(i = 0; i < NUM_OBSTACLES; i++)
	{
		if((*obstacles)[i].null != 0)
		{
			if(((*obstacles)[i]).x2 == 0 && ((*obstacles)[i]).h1x2 == 0 && ((*obstacles)[i]).h2x2 == 0)
			{
				alt_up_pixel_buffer_dma_draw_rectangle(pixel_buffer, (*obstacles)[i].h2x1, (*obstacles)[i].h2y1, (*obstacles)[i].h2x2, (*obstacles)[i].h2y2, BACK_COLOR, 0);
				alt_up_pixel_buffer_dma_draw_rectangle(pixel_buffer, (*obstacles)[i].h2x1, (*obstacles)[i].h2y1, (*obstacles)[i].h2x2, (*obstacles)[i].h2y2, BACK_COLOR, 1);
				(*obstacles)[i].null = 0;
			}
		}
	}
}

void MoveObstacles(struct Obstacle* obstacles[])
{
	int i;

		for(i = 0; i < NUM_OBSTACLES; i++)
		{
			if((*obstacles)[i].null != 0)
			{
				MoveObstacle(&((*obstacles)[i]));
			}
		}
}

void DrawObstacles(struct Obstacle* obstacles[], alt_up_pixel_buffer_dma_dev* pixel_buffer)
{
	int i;

	for(i = 0; i < NUM_OBSTACLES; i++)
	{
		if((*obstacles)[i].null != 0)
		{
			DrawObstacle(&((*obstacles)[i]), pixel_buffer);
		}
	}
}

void InitObstacles(struct Obstacle* obstacles[])
{
	int i;
	struct Obstacle nullObstacle;
	InitObstacle(&nullObstacle, 0, 0, 0, 0);
	nullObstacle.null = 0;

	for(i = 0; i < NUM_OBSTACLES; i++)
	{
		CopyByValue(&(*obstacles)[i], &nullObstacle);
	}
}

void AddObstacle(struct Obstacle* obstacles[], struct Obstacle* obstacle)
{
	int i;

	for(i = 0; (*obstacles)[i].null != 0 && i < NUM_OBSTACLES; i++);
	CopyByValue(&(*obstacles)[i], &(*obstacle));
}

void CopyByValue(struct Obstacle* ob1, struct Obstacle* ob2)
{
	(*ob1).null = (*ob2).null;

	(*ob1).x1 = (*ob2).x1;
	(*ob1).y1 = (*ob2).y1;
	(*ob1).x2 = (*ob2).x2;
	(*ob1).y2 = (*ob2).y2;

	(*ob1).h1x1 = (*ob2).h1x1;
	(*ob1).h1y1 = (*ob2).h1y1;
	(*ob1).h1x2 = (*ob2).h1x2;
	(*ob1).h1y2 = (*ob2).h1y2;

	(*ob1).h2x1 = (*ob2).h2x1;
	(*ob1).h2y1 = (*ob2).h2y1;
	(*ob1).h2x2 = (*ob2).h2x2;
	(*ob1).h2y2 = (*ob2).h2y2;

	(*ob1).vX = (*ob2).vX;
	(*ob1).vY = (*ob2).vY;
}

