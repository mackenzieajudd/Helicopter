#ifndef MENU_H_
#define MENU_H_

#include "HAL/inc/sys/alt_timestamp.h"
#include "drivers/inc/altera_up_avalon_video_character_buffer_with_dma.h"
#include "drivers/inc/altera_up_avalon_video_pixel_buffer_dma.h"
#include <stdio.h>
#include "VGA.h"
#include "Helicopter.h"
#include "Person.h"


void DrawMenu(alt_up_pixel_buffer_dma_dev* pixel_buffer, alt_up_char_buffer_dev** char_buffer);
void DrawCrash(alt_up_pixel_buffer_dma_dev* pixel_buffer, alt_up_char_buffer_dev** char_buffer, int score, int* highScore);
void DrawCountdown(alt_up_pixel_buffer_dma_dev* pixel_buffer, alt_up_char_buffer_dev** char_buffer, struct Person* person, struct Helicopter* helicopter, int* chopperFlag);

#endif
