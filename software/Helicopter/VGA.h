#ifndef VGA_H_
#define VGA_H_

#include "drivers/inc/altera_up_avalon_video_pixel_buffer_dma.h"
#include "drivers/inc/altera_up_avalon_video_character_buffer_with_dma.h"

void InitVGA(const char* pixelBufferName, const char* charBufferName,  alt_up_char_buffer_dev** char_buffer, alt_up_pixel_buffer_dma_dev** pixel_buffer, int BUFFER0);
void SwapBuffers(int* bufferFlag, const int BUFFER0, const int BUFFER1, alt_up_pixel_buffer_dma_dev** pixel_buffer);
void DrawScore(alt_up_char_buffer_dev** char_buffer, int score);
void DrawFPS(alt_up_char_buffer_dev** char_buffer, double fps);

#endif
