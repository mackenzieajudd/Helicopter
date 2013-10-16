/*
 * Person.h
 *
 *  Created on: 2013-10-08
 *      Author: Jack Bauer
 */

#ifndef PERSON_H_
#define PERSON_H_

#include "Map.h"

#define PERSON_STARTING_POSITION_X 0
#define PERSON_STARTING_POSITION_Y 175

struct Person
{
	int x;
	int y;
	int hx;
	int hy;
};

void initPersonStart(struct Person* person);
void initPerson(struct Person* person, int x, int y);
void movePerson(struct Person* person);
void drawPerson(struct Person* person, alt_up_pixel_buffer_dma_dev* pixel_buffer, int* runFlag);
void clearPerson(struct Person* person, alt_up_pixel_buffer_dma_dev* pixel_buffer);
void clearPersonHistorical(struct Person* person, alt_up_pixel_buffer_dma_dev* pixel_buffer);

#endif /* PERSON_H_ */
