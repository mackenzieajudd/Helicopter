#include "Map.h"

void InitMap(struct Map* map)
{
	int startingFloor = HEIGHT_MIN - BASE_DIFFUCULTY;
	int startingCeiling = HEIGHT_MAX + BASE_DIFFUCULTY;
	int i = 0;
	int j = 0;

	(*map).startingIndex = 0;
	(*map).steps = 0;
	
	for(i = 0; i < 320; i++)
	{
		for(j = 0; j < 240; j++)
		{
			if(j >= startingFloor || j <= startingCeiling)
				(*map).map[i][j] = *GROUND;
			else
				(*map).map[i][j] = *AIR;
		}
	}
}

void InitMapFromFile(struct Map* map, FILE* mapFile)
{
	int i;
	int j;

	for(i = 0; i < 320; i++)
	{
		for(j = 0; i < 240; i++)
		{
			(*map).map[i][j] = fgetc(mapFile);
		}
	}
}

void StepMap(struct Map* map)
{
	int startingCeiling;
	int startingFloor;
	int i = 0;
	int j;
	
	startingFloor = HEIGHT_MIN - (GetRandHeight((*map).steps, 0));
	startingCeiling = GetRandHeight((*map).steps, 0);

	for(j = 0; j < STEP_SIZE; j++)
	{
		for(i = 0; i < 240; i++)
		{
			if(i >= startingFloor || i <= startingCeiling)
				(*map).map[(*map).startingIndex][i] = *GROUND;
			else
				(*map).map[(*map).startingIndex][i] = *AIR;
		}

		if((*map).startingIndex == LENGTH_MAX)
			(*map).startingIndex = LENGTH_MIN;
		else
			(*map).startingIndex++;
	}

	(*map).steps++;
}

void StepMapAlternatingObstacles(struct Map* map, int* altCounter, int* mode, struct Obstacle* obstacles[])
{
	int startingCeiling;
	int startingFloor;
	int i = 0;
	int j;
	struct Obstacle obs;

	if((*map).steps % 160 == 0)
	{
		*altCounter = 160;
		if((*mode) == 1)
			(*mode) = 0;
		else
			(*mode) = 1;
	}

	if(*altCounter > 0 && (*mode) == 0)
	{
		startingFloor = HEIGHT_MIN - BASE_DIFFUCULTY - 20 +  abs((*altCounter)-80)/4;
		startingCeiling = BASE_DIFFUCULTY - 20 + abs((*altCounter)-80)/4;
		(*altCounter)--;
	}
	else if(*altCounter > 0 && (*mode) == 1)
	{
		startingFloor = HEIGHT_MIN - BASE_DIFFUCULTY - 20 +  abs((*altCounter)-80)/4;
		startingCeiling = BASE_DIFFUCULTY + 20 - abs((*altCounter)-80)/4;
		(*altCounter)--;
	}
	else
	{
		startingFloor = HEIGHT_MIN - BASE_DIFFUCULTY;
		startingCeiling = BASE_DIFFUCULTY;
	}

	if((*map).steps % 30 == 0)
	{
		if((*mode) == 1)
		{
			InitObstacle(&obs, 320, startingCeiling + 10, 330, startingCeiling + 20);
			AddObstacle(obstacles, &obs);
		}
		else
		{
			InitObstacle(&obs, 320, startingFloor - 20, 330, startingFloor - 10);
			AddObstacle(obstacles, &obs);
		}
	}

	for(j = 0; j < STEP_SIZE; j++)
	{
		for(i = 0; i < 240; i++)
		{
			if(i >= startingFloor || i <= startingCeiling)
				(*map).map[(*map).startingIndex][i] = *GROUND;
			else
				(*map).map[(*map).startingIndex][i] = *AIR;
		}

		if((*map).startingIndex == LENGTH_MAX)
			(*map).startingIndex = LENGTH_MIN;
		else
			(*map).startingIndex++;
	}

	(*map).steps++;
}

void StepMapFlat(struct Map* map)
{
	int startingCeiling;
	int startingFloor;
	int i = 0;
	int j;

	startingFloor = HEIGHT_MIN - ((int)(*map).steps/100 + BASE_DIFFUCULTY);
	startingCeiling = BASE_DIFFUCULTY + (int)(*map).steps/100;

	for(j = 0; j < STEP_SIZE; j++)
	{
		for(i = 0; i < 240; i++)
		{
			if(i >= startingFloor || i <= startingCeiling)
				(*map).map[(*map).startingIndex][i] = *GROUND;
			else
				(*map).map[(*map).startingIndex][i] = *AIR;
		}

		if((*map).startingIndex == LENGTH_MAX)
			(*map).startingIndex = LENGTH_MIN;
		else
			(*map).startingIndex++;
	}

	(*map).steps++;
}

int GetRandHeight(int steps, int difficulty)
{
	return rand() % (BASE_DIFFUCULTY) + (BASE_DIFFUCULTY/2 + steps/100);
}

void DrawMap(struct Map* map, alt_up_pixel_buffer_dma_dev* pixel_buffer)
{
	int iterations = 0;
	int i = 0;
	int j = 0;

	for(i = HEIGHT_MAX; i <= HEIGHT_MIN; i++)
	{
		for(j = (*map).startingIndex; iterations < 320; j++)
		{
			if((*map).map[j][i] == *AIR)
				alt_up_pixel_buffer_dma_draw(pixel_buffer, 0xFFFF, iterations, i);

			else
				alt_up_pixel_buffer_dma_draw(pixel_buffer, 0x0000, iterations, i);

			iterations++;

			if (j == LENGTH_MAX)
				j = LENGTH_MIN - 1;
		}
		iterations = 0;
	}
}

void DrawMapQuick(struct Map* map, alt_up_pixel_buffer_dma_dev* pixel_buffer)
{
	int iterations = 0;
	int i = 0;
	int j = 0;

	int maxFlux = BASE_DIFFUCULTY + (BASE_DIFFUCULTY/2 + (*map).steps/100);
	int minFlux = (BASE_DIFFUCULTY/2 + (*map).steps/100);

	for(i = minFlux; i <= maxFlux; i++)
	{
		for(j = (*map).startingIndex; iterations < 320; j++)
		{
			if((*map).map[j][i] != (*map).map[j-(2 * STEP_SIZE)][i] || j - (2 *STEP_SIZE) < (*map).startingIndex || j < (2 *STEP_SIZE))
			{
				if((*map).map[j][i] == *AIR)
					alt_up_pixel_buffer_dma_draw(pixel_buffer, 0xFFFF, iterations, i);

				else
					alt_up_pixel_buffer_dma_draw(pixel_buffer, 0x0000, iterations, i);
			}

			iterations++;

			if (j == LENGTH_MAX)
				j = LENGTH_MIN - 1;
		}
		iterations = 0;
	}

	for(i = HEIGHT_MIN - minFlux; i >= HEIGHT_MIN - maxFlux; i--)
	{
		for(j = (*map).startingIndex; iterations < 320; j++)
		{
			if((*map).map[j][i] != (*map).map[j-(2 * STEP_SIZE)][i] || j - (2 *STEP_SIZE) < (*map).startingIndex || j < (2 *STEP_SIZE))
			{
				if((*map).map[j][i] == *AIR)
					alt_up_pixel_buffer_dma_draw(pixel_buffer, 0xFFFF, iterations, i);

				else
					alt_up_pixel_buffer_dma_draw(pixel_buffer, 0x0000, iterations, i);
			}

			iterations++;

			if (j == LENGTH_MAX)
				j = LENGTH_MIN - 1;
		}
		iterations = 0;
	}
}

int getRoof(struct Map* map, int x)
{
	int i;

	for(i = HEIGHT_MAX; (*map).map[x][i] != *AIR; i++)
	{
	}

	return i - 1;
}

int getFloor(struct Map* map, int x)
{
	int i;

	for(i = HEIGHT_MIN; (*map).map[x][i] != *AIR; i--)
	{
	}

	return i + 1;
}

void DrawFlatMapQuick(struct Map* map, alt_up_pixel_buffer_dma_dev* pixel_buffer)
{
	int iterations = 0;
	int i = 0;
	int j = 0;

	int flux = 3;
	int tempRoof;
	int tempFloor;

	for(i = (*map).startingIndex; iterations < 320; i++)
	{
		tempRoof = getRoof(map, i);

		for(j = tempRoof - flux; j < tempRoof + flux; j++)
		{
			if((*map).map[i][j] != (*map).map[i-(2 * STEP_SIZE)][j] || i - (2 * STEP_SIZE) < (*map).startingIndex || i < (2 *STEP_SIZE))
			{
				if((*map).map[i][j] == *AIR)
					alt_up_pixel_buffer_dma_draw(pixel_buffer, 0xFFFF, iterations, j);

				else
					alt_up_pixel_buffer_dma_draw(pixel_buffer, 0x0000, iterations, j);
			}
		}

		iterations++;

		if (i == LENGTH_MAX)
			i = LENGTH_MIN - 1;
	}

	iterations = 0;

	for(i = (*map).startingIndex; iterations < 320; i++)
	{
		tempFloor = getFloor(map, i);

		for(j = tempFloor + flux; j > tempFloor - flux; j--)
		{
			if((*map).map[i][j] != (*map).map[i-(2 * STEP_SIZE)][j] || i - (2 * STEP_SIZE) < (*map).startingIndex || i < (2 *STEP_SIZE))
			{
				if((*map).map[i][j] == *AIR)
					alt_up_pixel_buffer_dma_draw(pixel_buffer, 0xFFFF, iterations, j);

				else
					alt_up_pixel_buffer_dma_draw(pixel_buffer, 0x0000, iterations, j);
			}
		}

		iterations++;

		if (i == LENGTH_MAX)
			i = LENGTH_MIN - 1;
	}
}

void DrawMapInConsole(struct Map* map)
{
	int iterations = 0;
	int i = 0;
	int j = 0;

	for(i = (*map).startingIndex; iterations < 320; i++)
	{
		for(j = 0; j < 240; j++)
		{
			printf((*map).map[i][j]);
		}

		printf("\n");

		iterations++;

		if (i == LENGTH_MAX)
			i = LENGTH_MIN;
	}
}

/*
void DrawMapInTextFile(struct Map* map, char* filePath)
{
	int iterations = 0;
	int i = 0;
	int j = 0;
	FILE *f;
	fopen_s(&f, filePath, "w");

	if (f == NULL)
	{
		printf("Error opening file!\n");
	}

	for(i = HEIGHT_MAX; i <= HEIGHT_MIN; i++)
	{
		for(j = (*map).startingIndex; iterations < 320; j++)
		{
			fprintf(f, "%s", (*map).map[j][i]);

			iterations++;

			if (j == LENGTH_MAX)
				j = LENGTH_MIN - 1;
		}

		fprintf(f, "\n");

		iterations = 0;
	}

	fclose(f);
}
*/
