#include <stdio.h>
#include "drivers/inc/altera_up_avalon_video_character_buffer_with_dma.h"
#include "drivers/inc/altera_up_avalon_video_pixel_buffer_dma.h"
#include "Map.h"
#include "VGA.h"
#include "Helicopter.h"
#include "HAL/inc/sys/alt_alarm.h"
#include "HAL/inc/sys/alt_timestamp.h"
#include "io.h"
#include <altera_up_avalon_audio_and_video_config.h>
#include <altera_up_avalon_audio.h>
#include "sys/alt_irq.h"
#include "Menu.h"
#include "Obstacle.h"
#include <altera_up_sd_card_avalon_interface.h>
#include "SD.h"
#include "Person.h"
#include "system.h"

#define button2 (volatile char *) 0x00002070
#define FPS 20
#define push_buttons (volatile char*) 0x002820

/* define variables and constants for playing background music */
#define WAVHEADER_SIZE 44
#define SAMPLE_SIZE 96

// last song played
#define SONG_SIZE 235878

// middle song
#define SONG2_SIZE 899118

// first song
#define SONG3_SIZE 128100

int song_loop;
unsigned int song_temp[2];

int first_count;
unsigned int* song1;
unsigned int* first_song;

int second_count;
unsigned int song2[SONG2_SIZE]= {0};
unsigned int second_song[SAMPLE_SIZE]= {0};

int third_count;
unsigned int song3[SONG3_SIZE]= {0};
unsigned int third_song[SAMPLE_SIZE]= {0};

int k1 = 0;
int k2 = 0;
int k3 = 0;

unsigned int buf1[SONG_SIZE / 2] = { 0 };
unsigned int buf2[SONG2_SIZE / 2] = { 0 };
unsigned int buf3[SONG3_SIZE / 2] = { 0 };

volatile int flag=0;
volatile int flag1=0;
volatile int flag2=0;
alt_up_audio_dev * audio;

void wait(float);


void sound_setup()
{
alt_up_av_config_dev * av_config = alt_up_av_config_open_dev("/dev/audio_and_video_config_0");
while (!alt_up_av_config_read_ready(av_config)) {

}
audio = alt_up_audio_open_dev("/dev/audio_0");

// this is to disable the write interupt first because at the begineering there is nothing in the buffer
// and would have 122 or 123 spaces so the space is > 75% and a write interupt(1) would be generated so we want to disable it first
alt_up_audio_reset_audio_core(audio);

}

char openFileInSD( char* fileName, short int* file_handle_ptr )
{
short int file_handle;
alt_up_sd_card_dev *device_reference = NULL;
int connected = 0;

device_reference = alt_up_sd_card_open_dev("/dev/Altera_UP_SD_Card_Avalon_Interface_0");

if (device_reference != NULL) {

if ((connected == 0) && (alt_up_sd_card_is_Present())) {
printf("Card connected.\n");
if (alt_up_sd_card_is_FAT16()) {
printf("FAT16 file system detected.\n");

file_handle = alt_up_sd_card_fopen( fileName, false );
if ( file_handle == -1 )
printf( "Error: File could not be opened.\n" );
if ( file_handle == -2 )
printf( "Error: File is already open.\n" );

if ( file_handle != -1 && file_handle != -2 )
{
//printf( "SD Card successfully opened.\n" ); // debugging purpose
*file_handle_ptr = file_handle;
return 0;
}

} else {
printf("Unknown file system.\n");
}
connected = 1;
} else if ((connected == 1) && (alt_up_sd_card_is_Present() == false)) {
printf("Card disconnected.\n");
connected = 0;
}

}
return -1;
}

void audio_isr(void * context, unsigned int irq_id) {

if (flag1 == 1) {
// fill 96 samples
//for (song_loop = 0; song_loop < SAMPLE_SIZE; song_loop++) {
//song_temp[0] = song1[first_count];
//song_temp[1] = song1[first_count + 1];

//first_count += 2;

if (k1 >= (SONG_SIZE) / 2) {
alt_irq_disable(AUDIO_0_IRQ);
}
//}
alt_up_audio_write_fifo(audio, &buf1[k1], SAMPLE_SIZE,
ALT_UP_AUDIO_LEFT);
alt_up_audio_write_fifo(audio, &buf1[k1], SAMPLE_SIZE,
ALT_UP_AUDIO_RIGHT);

k1 = k1 + 96;

}

else if (flag == 1) {

if (k2 >= (SONG2_SIZE)/2) {
k2 = 15000;
}
/*// dont need to repeat crash sound
if(second_count >= SONG_SIZE - 20000)
second_count = 0;*/

//printf("%d,%d \n", second_song[60], second_song[50]);
alt_up_audio_write_fifo(audio, &buf2[k2], SAMPLE_SIZE,
ALT_UP_AUDIO_LEFT);
alt_up_audio_write_fifo(audio, &buf2[k2], SAMPLE_SIZE,
ALT_UP_AUDIO_RIGHT);

k2 = k2 + 96;
}

else {

if (k3 >= (SONG3_SIZE)/2) {
flag = 1;
}

//printf("%d,%d \n", second_song[60], second_song[50]);
alt_up_audio_write_fifo(audio, &buf3[k3], SAMPLE_SIZE,
ALT_UP_AUDIO_LEFT);
alt_up_audio_write_fifo(audio, &buf3[k3], SAMPLE_SIZE,
ALT_UP_AUDIO_RIGHT);
k3 = k3 + 96;
}
}


void load_songs_from_sd() {
song1 = NULL;
short int file_handle;
int i;

if (openFileInSD("bomb.wav", &file_handle) == 0) {
song1 = (unsigned int*) malloc(SONG_SIZE * sizeof(unsigned int));

if (song1 == NULL) {

printf("malloc failed\n");
alt_up_sd_card_fclose(file_handle);
}

for (i = 0; i < WAVHEADER_SIZE; i++)
alt_up_sd_card_read(file_handle);

for (i = 0; i < SONG_SIZE; i++) {
song1[i] = alt_up_sd_card_read(file_handle);

}
alt_up_sd_card_fclose(file_handle);

first_song = (unsigned int*) malloc(SAMPLE_SIZE * sizeof(unsigned int));

for (song_loop = 0; song_loop < (SONG_SIZE / 2); song_loop++) {
song_temp[0] = song1[first_count];
song_temp[1] = song1[first_count + 1];

first_count += 2;

buf1[song_loop] = ((song_temp[1] <<  8)| song_temp[0]);
}

if (openFileInSD("back.wav", &file_handle) == 0) {

for (i = 0; i < WAVHEADER_SIZE; i++)
alt_up_sd_card_read(file_handle);

for (i = 0; i < SONG2_SIZE; i++) {
song2[i] = alt_up_sd_card_read(file_handle);

}
alt_up_sd_card_fclose(file_handle);

//first_song = (unsigned int*)malloc(SAMPLE_SIZE*sizeof(unsigned int));
for (song_loop = 0; song_loop < (SONG2_SIZE / 2); song_loop++) {
song_temp[0] = song2[second_count];
song_temp[1] = song2[second_count + 1];

second_count += 2;

buf2[song_loop] = ((song_temp[1] <<  8)| song_temp[0]);
}
}

if (openFileInSD("chop.wav", &file_handle) == 0) {

for (i = 0; i < WAVHEADER_SIZE; i++)
alt_up_sd_card_read(file_handle);

for (i = 0; i < SONG3_SIZE; i++) {
song3[i] = alt_up_sd_card_read(file_handle);

}
alt_up_sd_card_fclose(file_handle);

//first_song = (unsigned int*)malloc(SAMPLE_SIZE*sizeof(unsigned int));
for (song_loop = 0; song_loop < (SONG3_SIZE / 2); song_loop++) {
song_temp[0] = song3[third_count];
song_temp[1] = song3[third_count + 1];

third_count += 2;

buf3[song_loop] = ((song_temp[1] << 8) | song_temp[0]);
}
}

}
}

int main()
{
	const int PIXEL_BUFFER_BASE = 0x00080000;
	const int BACK_PIXEL_BUFFER_BASE = 0x000C0000;
	const char* PIXEL_BUFFER_NAME = "/dev/video_pixel_buffer_dma_0";
	const char* CHAR_BUFFER_NAME = "/dev/video_character_buffer_with_dma_0";

	alt_up_sd_card_dev *device_reference;
	int connected = 0;
	char* startingFile = "MAP0.TXT";
	int mapPointer = 0;
	int mapHandle;

	int chopperFlag = 0;
	int start_time;
	int end_time;
	int highScore = 0;
	int buffer_flag = 0;
	alt_up_pixel_buffer_dma_dev* pixel_buffer;
	alt_up_char_buffer_dev* char_buffer;
	double fps;
	struct Helicopter maChoppa;
	struct Person person;
	struct Map myMap;
	struct Obstacle obstacles[NUM_OBSTACLES];
	unsigned int buf [7];
	unsigned int buf1 [2];

	device_reference = alt_up_sd_card_open_dev("/dev/Altera_UP_SD_Card_Avalon_Interface_0");

	sound_setup();

	load_songs_from_sd();
	alt_up_audio_enable_write_interrupt(audio);
	alt_irq_register(AUDIO_0_IRQ, 0, (alt_isr_func)audio_isr);
	alt_irq_disable(AUDIO_0_IRQ);

	buf[0] = 0x00000FFF;
	buf[1] = 0x00000FFF;
	buf[2] = 0x00000FFF;
	buf[3] = 0x00000FFF;
	buf[4] = 0x00000FFF;
	buf[5] = 0x00000FFF;
	buf[6] = 0x00000001;
	buf1[0] = 0x00000000;
	buf1[1] = 0x00000000;

	srand(time(NULL));

	if (device_reference != NULL)
	{
		if ((connected == 0) && (alt_up_sd_card_is_Present()))
		{
			printf("Card connected.\n");
			if (alt_up_sd_card_is_FAT16())
			{
				printf("FAT16 file system detected.\n");
			}
			else
			{
				printf("Unknown file system.\n");
			}
			connected = 1;
		}
		else if ((connected == 1) && (alt_up_sd_card_is_Present() == false))
		{
			printf("Card disconnected.\n");
			connected = 0;
		}
	}

	InitVGA(PIXEL_BUFFER_NAME, CHAR_BUFFER_NAME, &char_buffer, &pixel_buffer, BACK_PIXEL_BUFFER_BASE);
	DrawMenu(pixel_buffer, &char_buffer);
	SwapBuffers(&buffer_flag, BACK_PIXEL_BUFFER_BASE, PIXEL_BUFFER_BASE, &pixel_buffer);

	while(*button2 != 0);

	alt_up_char_buffer_clear(char_buffer);

	while(1 == 1)
	{
		//InitMap(&myMap);
		InitMapFromFile(&myMap, startingFile, &mapHandle);

		InitObstacles(&obstacles);

		InitHelicopter(&maChoppa, 155, 173, 161,178);
		initPersonStart(&person);

		DrawMap(&myMap, pixel_buffer);
		SwapBuffers(&buffer_flag, BACK_PIXEL_BUFFER_BASE, PIXEL_BUFFER_BASE, &pixel_buffer);
		DrawMap(&myMap, pixel_buffer);
		SwapBuffers(&buffer_flag, BACK_PIXEL_BUFFER_BASE, PIXEL_BUFFER_BASE, &pixel_buffer);


		first_count=0;
		second_count=0;
		third_count=0;
		k1 = 0;
		k2 = 0;
		k3 = 0;
		flag=0;
		flag1= 0;
		alt_irq_enable(AUDIO_0_IRQ);

		DrawHelicopterFancy(&maChoppa, pixel_buffer, &chopperFlag);
		SwapBuffers(&buffer_flag, BACK_PIXEL_BUFFER_BASE, PIXEL_BUFFER_BASE, &pixel_buffer);
		DrawCountdown(pixel_buffer, &char_buffer, &person, &maChoppa, &chopperFlag);

		alt_up_char_buffer_clear(char_buffer);

		alt_timestamp_start();

		while(1 == 1)
		{
			fps = 0;

			if((float)(alt_timestamp())/(float)(alt_timestamp_freq()) > (float)1/(float)FPS)
			{
				start_time = alt_timestamp();

				//StepBestMapEver(&myMap, &mode, &obstacles);
				StepMapFromFile(&myMap, &mapHandle, &obstacles, &mapPointer);
				DrawFlatMapQuick(&myMap, pixel_buffer);

				MoveHelicopter(&maChoppa, button2, pixel_buffer);

				MoveObstacles(&obstacles);
				ManageObstacles(&obstacles, pixel_buffer);
				DrawObstacles(&obstacles, pixel_buffer);

				if(CheckForCollisions(&myMap, &maChoppa) == 1 || CheckForObstacleCollisions(&obstacles, &maChoppa) == 1)
				{
					flag1= 1;

					DrawHelicopterFancy(&maChoppa, pixel_buffer, &chopperFlag);
					SwapBuffers(&buffer_flag, BACK_PIXEL_BUFFER_BASE, PIXEL_BUFFER_BASE, &pixel_buffer);

					alt_up_pixel_buffer_dma_draw_rectangle(pixel_buffer, maChoppa.x1 + 2, maChoppa.y1 + 2, maChoppa.x2 - 2, maChoppa.y2 - 2, 0xF000, 0);
					wait(0.15);

					alt_up_pixel_buffer_dma_draw_rectangle(pixel_buffer, maChoppa.x1 + 1, maChoppa.y1 + 1, maChoppa.x2 - 1, maChoppa.y2 - 1, 0xF000, 0);
					wait(0.15);

					alt_up_pixel_buffer_dma_draw_rectangle(pixel_buffer, maChoppa.x1, maChoppa.y1, maChoppa.x2, maChoppa.y2, 0xF000, 0);
					wait(0.15);

					alt_up_pixel_buffer_dma_draw_rectangle(pixel_buffer, maChoppa.x1 - 1, maChoppa.y1 - 1, maChoppa.x2 + 1, maChoppa.y2 + 1, 0xF000, 0);
					wait(0.15);

					alt_up_pixel_buffer_dma_draw_rectangle(pixel_buffer, maChoppa.x1 - 2, maChoppa.y1 - 2, maChoppa.x2 + 2, maChoppa.y2 + 2, 0xF000, 0);
					wait(0.15);

					alt_up_pixel_buffer_dma_draw_rectangle(pixel_buffer, maChoppa.x1 + 2, maChoppa.y1 + 2, maChoppa.x2 - 2, maChoppa.y2 - 2, BACK_COLOR, 0);
					wait(0.15);

					alt_up_pixel_buffer_dma_draw_rectangle(pixel_buffer, maChoppa.x1 + 1, maChoppa.y1 + 1, maChoppa.x2 - 1, maChoppa.y2 - 1, BACK_COLOR, 0);
					wait(0.15);

					alt_up_pixel_buffer_dma_draw_rectangle(pixel_buffer, maChoppa.x1, maChoppa.y1, maChoppa.x2, maChoppa.y2, BACK_COLOR, 0);
					wait(0.15);

					alt_up_pixel_buffer_dma_draw_rectangle(pixel_buffer, maChoppa.x1 - 1, maChoppa.y1 - 1, maChoppa.x2 + 1, maChoppa.y2 + 1, BACK_COLOR, 0);
					wait(0.15);

					alt_up_pixel_buffer_dma_draw_rectangle(pixel_buffer, maChoppa.x1 - 2, maChoppa.y1 - 2, maChoppa.x2 + 2, maChoppa.y2 + 2, BACK_COLOR, 0);
					wait(0.15);

					break;
				}
				DrawHelicopterFancy(&maChoppa, pixel_buffer, &chopperFlag);

				DrawScore(&char_buffer, myMap.steps);

				SwapBuffers(&buffer_flag, BACK_PIXEL_BUFFER_BASE, PIXEL_BUFFER_BASE, &pixel_buffer);

				end_time = alt_timestamp();
				fps = (float)1/((float)(end_time - start_time)/(float)alt_timestamp_freq());
				printf("MAX FPS: %f\n", fps);
				if( fps < FPS)
				{
					DrawFPS(&char_buffer, fps);
				}
				else
				{
					DrawFPS(&char_buffer, FPS);
				}

				alt_timestamp_start();
			}
		}

		wait(1);

		DrawCrash(pixel_buffer, &char_buffer, myMap.steps, &highScore);
		SwapBuffers(&buffer_flag, BACK_PIXEL_BUFFER_BASE, PIXEL_BUFFER_BASE, &pixel_buffer);
		alt_timestamp_start();
		while((float)(alt_timestamp())/(float)(alt_timestamp_freq()) < 2);

		while(*button2 != 0);
		alt_up_char_buffer_clear(char_buffer);

	}

  return 0;
}

void wait(float seconds)
{
	alt_timestamp_start();

	while((float)(alt_timestamp())/(float)(alt_timestamp_freq()) < seconds)
	{
	}
}
