#include "Menu.h"

void DrawMenu(alt_up_pixel_buffer_dma_dev* pixel_buffer, alt_up_char_buffer_dev** char_buffer)
{
	int i = 0;

	for(i = 0; i < 320; i++)
	{
		alt_up_pixel_buffer_dma_draw_vline(pixel_buffer, i, 0, 239, 0x0000, 1);
	}

	alt_up_char_buffer_clear(*char_buffer);
	alt_up_char_buffer_string((*char_buffer), "G e t  T o  T h e  C h o p p a !", 25, 15);
	alt_up_char_buffer_string((*char_buffer), "Press <KEY1> To Start", 28, 40);
}

void DrawCountdown(alt_up_pixel_buffer_dma_dev* pixel_buffer, alt_up_char_buffer_dev** char_buffer)
{
	int countdown = 3;
	char countdownBuffer[1];

	alt_timestamp_start();

	while(countdown >= 0)
	{
		sprintf(countdownBuffer, "%d", countdown);
		alt_up_char_buffer_clear(*char_buffer);

		if(countdown == 0)
		{
			alt_up_char_buffer_string((*char_buffer), "Go!", 40, 5);
		}
		else
		{
			alt_up_char_buffer_string((*char_buffer), countdownBuffer, 40, 5);
		}
		countdown--;

		alt_timestamp_start();
		while((float)(alt_timestamp())/(float)(alt_timestamp_freq()) < 1)
		{
		}
	}
}

void DrawCrash(alt_up_pixel_buffer_dma_dev* pixel_buffer, alt_up_char_buffer_dev** char_buffer, int score)
{
	char scoreBuffer[15];
	int i = 0;
	sprintf(scoreBuffer, "Score: %d", score);
	for(i = 0; i < 320; i++)
	{
		alt_up_pixel_buffer_dma_draw_vline(pixel_buffer, i, 0, 239, 0x0000, 1);
	}

	alt_up_char_buffer_clear(*char_buffer);
	alt_up_char_buffer_string((*char_buffer), "Y o u  C r a s h e d !", 28, 15);
	alt_up_char_buffer_string((*char_buffer), scoreBuffer, 34, 30);
	alt_up_char_buffer_string((*char_buffer), "Press <KEY1> To Retry", 28, 40);
}
