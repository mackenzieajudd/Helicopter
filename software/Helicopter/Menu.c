#include "Menu.h"

void DrawMenu(alt_up_pixel_buffer_dma_dev* pixel_buffer, alt_up_char_buffer_dev** char_buffer)
{
	int i = 0;

	for(i = 0; i < 320; i++)
	{
		alt_up_pixel_buffer_dma_draw_vline(pixel_buffer, i, 0, 239, 0x0000, 1);
	}

	alt_up_char_buffer_clear(*char_buffer);
	alt_up_char_buffer_string((*char_buffer), "G e t  T o  T h e  C h o p p a !", 23, 15);
	alt_up_char_buffer_string((*char_buffer), "Press <KEY1> To Start", 28, 40);
}

void DrawCountdown(alt_up_pixel_buffer_dma_dev* pixel_buffer, alt_up_char_buffer_dev** char_buffer, struct Person* person, struct Helicopter* helicopter, int* chopperFlag)
{
	const int PIXEL_BUFFER_BASE = 0x00080000;
	const int BACK_PIXEL_BUFFER_BASE = 0x000C0000;
	int buffer_flag = 0;
	int personFlag = 0;
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
		DrawHelicopterFancy(helicopter, pixel_buffer, chopperFlag);
		SwapBuffers(&buffer_flag, BACK_PIXEL_BUFFER_BASE, PIXEL_BUFFER_BASE, &pixel_buffer);
		DrawHelicopterFancy(helicopter, pixel_buffer, chopperFlag);
		SwapBuffers(&buffer_flag, BACK_PIXEL_BUFFER_BASE, PIXEL_BUFFER_BASE, &pixel_buffer);
		if(countdown >= 2)
		{
			while((*person).x < 155)
			{
				if((*person).x%10 == 0)
				{
					if(personFlag == 0)
						personFlag = 1;
					else
						personFlag = 0;
				}
				movePerson(person);
				drawPerson(person, pixel_buffer, &personFlag);
				SwapBuffers(&buffer_flag, BACK_PIXEL_BUFFER_BASE, PIXEL_BUFFER_BASE, &pixel_buffer);
				while((float)(alt_timestamp())/(float)(alt_timestamp_freq()) < 0.05);
			}
		}
		else if(countdown >= 1)
		{
			clearPerson(person, pixel_buffer);
			MoveHelicopterCountdown(helicopter, pixel_buffer);
			SwapBuffers(&buffer_flag, BACK_PIXEL_BUFFER_BASE, PIXEL_BUFFER_BASE, &pixel_buffer);
			clearPerson(person, pixel_buffer);
			MoveHelicopterCountdown(helicopter, pixel_buffer);
			SwapBuffers(&buffer_flag, BACK_PIXEL_BUFFER_BASE, PIXEL_BUFFER_BASE, &pixel_buffer);
			while((*helicopter).y1 > HELICOPTER_STARTING_POSITION_Y)
			{
				MoveHelicopterCountdown(helicopter, pixel_buffer);
				DrawHelicopterFancy(helicopter, pixel_buffer, chopperFlag);
				SwapBuffers(&buffer_flag, BACK_PIXEL_BUFFER_BASE, PIXEL_BUFFER_BASE, &pixel_buffer);
				while((float)(alt_timestamp())/(float)(alt_timestamp_freq()) < 0.6);
			}
			DrawHelicopterFancy(helicopter, pixel_buffer, chopperFlag);
			SwapBuffers(&buffer_flag, BACK_PIXEL_BUFFER_BASE, PIXEL_BUFFER_BASE, &pixel_buffer);
			InitHelicopter(helicopter, HELICOPTER_STARTING_POSITION_X, HELICOPTER_STARTING_POSITION_Y, HELICOPTER_STARTING_POSITION_X + HELICOPTER_SIZE_X, HELICOPTER_STARTING_POSITION_Y + HELICOPTER_SIZE_Y);
			DrawHelicopterFancy(helicopter, pixel_buffer, chopperFlag);
			SwapBuffers(&buffer_flag, BACK_PIXEL_BUFFER_BASE, PIXEL_BUFFER_BASE, &pixel_buffer);
		}
		else
		{
			while((float)(alt_timestamp())/(float)(alt_timestamp_freq()) < 1);
		}
	}
}

void DrawCrash(alt_up_pixel_buffer_dma_dev* pixel_buffer, alt_up_char_buffer_dev** char_buffer, int score, int* highScore)
{
	char scoreBuffer[15];
	char highScoreBuffer0[25];
	char highScoreBuffer1[25];
	char highScoreBuffer2[25];
	char high_score_names[5][3];
	char* name0;
	char* name1;
	char* name2;
	int high_scores[3];
	//Ordered high score names & numbers
	char high_score[35];
	//Variables that need to be set
	int new_high_score = score - STEP_SIZE;
	char new_high_score_name[3] = "NEW";
	char textfile[] = "scores.txt";

	int i = 0;

	Highscore( new_high_score, &new_high_score_name, &high_score_names, &high_scores, &textfile);
	write_txt(&textfile, &high_score_names, &high_scores);

	sprintf(scoreBuffer, "Score: %d", score - STEP_SIZE);
	sprintf(highScoreBuffer0, "3rd: %d", high_scores[0]);
	sprintf(highScoreBuffer1, "2nd: %d", high_scores[1]);
	sprintf(highScoreBuffer2, "1st: %d", high_scores[2]);

	for(i = 0; i < 320; i++)
	{
		alt_up_pixel_buffer_dma_draw_vline(pixel_buffer, i, 0, 239, 0x0000, 1);
	}

	alt_up_char_buffer_clear(*char_buffer);
	alt_up_char_buffer_string((*char_buffer), "Y o u  C r a s h e d !", 28, 15);
	alt_up_char_buffer_string((*char_buffer), scoreBuffer, 34, 30);
	alt_up_char_buffer_string((*char_buffer), highScoreBuffer2, 35, 35);
	alt_up_char_buffer_string((*char_buffer), highScoreBuffer1, 35, 37);
	alt_up_char_buffer_string((*char_buffer), highScoreBuffer0, 35, 39);
	//alt_up_char_buffer_string((*char_buffer), &high_score_names[0][0], 45, 35);
	//alt_up_char_buffer_string((*char_buffer), &high_score_names[1][0], 45, 37);
	//alt_up_char_buffer_string((*char_buffer), &high_score_names[2][0], 45, 39);
	alt_up_char_buffer_string((*char_buffer), "Press <KEY1> To Retry", 28, 45);
}
