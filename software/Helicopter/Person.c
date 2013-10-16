/*
 * Person.c
 *
 *  Created on: 2013-10-08
 *      Author: Jack Bauer
 */

#include "Person.h"
#include "Map.h"

void initPersonStart(struct Person* person)
{
	(*person).x = PERSON_STARTING_POSITION_X;
	(*person).hx = PERSON_STARTING_POSITION_X;
	(*person).y = PERSON_STARTING_POSITION_Y;
	(*person).hy = PERSON_STARTING_POSITION_Y;
}

void initPerson(struct Person* person, int x, int y)
{
	(*person).x = x;
	(*person).hx = x;
	(*person).y = y;
	(*person).hy = y;
}

void movePerson(struct Person* person)
{
	(*person).hx = (*person).x;
	(*person).x += 1;
}

//void clearPerson(struct Person* person, alt_up_pixel_buffer_dma_dev* pixel_buffer)


void drawPerson(struct Person* person, alt_up_pixel_buffer_dma_dev* pixel_buffer, int* runFlag)
{
	alt_up_pixel_buffer_dma_draw_line(pixel_buffer, (*person).hx-4, 175, (*person).hx, 175, 0xFFFF, 1);
	alt_up_pixel_buffer_dma_draw_line(pixel_buffer, (*person).hx+1, 172, (*person).hx+1, 177, 0xFFFF, 1);
	alt_up_pixel_buffer_dma_draw_line(pixel_buffer, (*person).hx, 172, (*person).hx, 169, 0xFFFF, 1);
	alt_up_pixel_buffer_dma_draw_line(pixel_buffer, (*person).hx+2, 172, (*person).hx+2, 169, 0xFFFF, 1);
	alt_up_pixel_buffer_dma_draw_line(pixel_buffer, (*person).hx, 169, (*person).hx+2, 169, 0xFFFF, 1);
	alt_up_pixel_buffer_dma_draw_line(pixel_buffer, (*person).hx-1, 179, (*person).hx+1, 177, 0xFFFF, 1);
	alt_up_pixel_buffer_dma_draw_line(pixel_buffer, (*person).hx+1, 177, (*person).hx+3, 179, 0xFFFF, 1);
	alt_up_pixel_buffer_dma_draw_line(pixel_buffer, (*person).hx-1, 176, (*person).hx-1, 176, 0xFFFF, 1);
	alt_up_pixel_buffer_dma_draw_line(pixel_buffer, (*person).hx+3, 174, (*person).hx+3, 174, 0xFFFF, 1);
	alt_up_pixel_buffer_dma_draw_line(pixel_buffer, (*person).hx-1, 174, (*person).hx-1, 174, 0xFFFF, 1);
	alt_up_pixel_buffer_dma_draw_line(pixel_buffer, (*person).hx+3, 176, (*person).hx+3, 176, 0xFFFF, 1);
	alt_up_pixel_buffer_dma_draw_line(pixel_buffer, (*person).x, 175, (*person).x + 4, 175, 0x964B00, 1);
	alt_up_pixel_buffer_dma_draw_line(pixel_buffer, (*person).x+2, 172, (*person).x+2, 177, 0x964B00, 1);
	alt_up_pixel_buffer_dma_draw_line(pixel_buffer, (*person).x+1, 172, (*person).x+1, 169, 0x964B00, 1);
	alt_up_pixel_buffer_dma_draw_line(pixel_buffer, (*person).x+3, 172, (*person).x+3, 169, 0x964B00, 1);
	alt_up_pixel_buffer_dma_draw_line(pixel_buffer, (*person).x+1, 169, (*person).x+3, 169, 0x964B00, 1);
	alt_up_pixel_buffer_dma_draw_line(pixel_buffer, (*person).x, 179, (*person).x+2, 177, 0x964B00, 1);
	alt_up_pixel_buffer_dma_draw_line(pixel_buffer, (*person).x+2, 177, (*person).x+4, 179, 0x964B00, 1);
	if(*runFlag == 1)
	{
		alt_up_pixel_buffer_dma_draw_line(pixel_buffer, (*person).x, 176, (*person).x, 176, 0x964B00, 1);
		alt_up_pixel_buffer_dma_draw_line(pixel_buffer, (*person).x+4, 174, (*person).x+4, 174, 0x964B00, 1);
	}
	else
	{
		alt_up_pixel_buffer_dma_draw_line(pixel_buffer, (*person).x, 174, (*person).x, 174, 0x964B00, 1);
		alt_up_pixel_buffer_dma_draw_line(pixel_buffer, (*person).x+4, 176, (*person).x+4, 176, 0x964B00, 1);
	}
}
void clearPerson(struct Person* person, alt_up_pixel_buffer_dma_dev* pixel_buffer)
{
	alt_up_pixel_buffer_dma_draw_rectangle(pixel_buffer, (*person).x-5, 169, (*person).x+8, 179, 0xFFFF, 1);
	alt_up_pixel_buffer_dma_draw_rectangle(pixel_buffer, (*person).x-4, 170, (*person).x+7, 178, 0xFFFF, 1);
	alt_up_pixel_buffer_dma_draw_rectangle(pixel_buffer, (*person).x-3, 171, (*person).x+6, 177, 0xFFFF, 1);
	alt_up_pixel_buffer_dma_draw_rectangle(pixel_buffer, (*person).x-2, 172, (*person).x+5, 176, 0xFFFF, 1);
	alt_up_pixel_buffer_dma_draw_rectangle(pixel_buffer, (*person).x-1, 173, (*person).x+4, 175, 0xFFFF, 1);
	alt_up_pixel_buffer_dma_draw_rectangle(pixel_buffer, (*person).x, 174, (*person).x+3, 174, 0xFFFF, 1);
}
