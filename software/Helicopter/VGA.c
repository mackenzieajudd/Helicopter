#include "VGA.h"

void InitVGA(const char* pixelBufferName, const char* charBufferName,  alt_up_char_buffer_dev** char_buffer, alt_up_pixel_buffer_dma_dev** pixel_buffer, int BUFFER0)
{
	(*pixel_buffer) = alt_up_pixel_buffer_dma_open_dev(pixelBufferName);
	alt_up_pixel_buffer_dma_change_back_buffer_address((*pixel_buffer), BUFFER0);

	*char_buffer = alt_up_char_buffer_open_dev(charBufferName);
	alt_up_char_buffer_init(*char_buffer);
	alt_up_char_buffer_clear(*char_buffer);
}


void SwapBuffers(int* bufferFlag, const int BUFFER0, const int BUFFER1, alt_up_pixel_buffer_dma_dev** pixel_buffer)
{
	alt_up_pixel_buffer_dma_swap_buffers((*pixel_buffer));
	while(alt_up_pixel_buffer_dma_check_swap_buffers_status((*pixel_buffer)));

	if((*bufferFlag) == 0)
	{
		alt_up_pixel_buffer_dma_change_back_buffer_address((*pixel_buffer), BUFFER1);
		(*bufferFlag) = 1;
	}
	else
	{
		alt_up_pixel_buffer_dma_change_back_buffer_address((*pixel_buffer), BUFFER0);
		(*bufferFlag) = 0;
	}
}

void DrawScore(alt_up_char_buffer_dev** char_buffer, int score)
{
	char scoreBuffer[10];
	sprintf(scoreBuffer, "%d", score);
	alt_up_char_buffer_string((*char_buffer), scoreBuffer, 0, 0);
}
