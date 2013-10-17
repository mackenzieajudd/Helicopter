#include <stdio.h>
#include <stdlib.h>
#include <altera_up_sd_card_avalon_interface.h>
#include "score.h"
#include <math.h>

/*
int main(void) {

	//File handling variables
	alt_up_sd_card_dev *device_reference = NULL;
	int connected = 0;
	device_reference = alt_up_sd_card_open_dev("/dev/Altera_UP_SD_Card_Avalon_Interface_0");

	//Stores all the high scores
	char high_score_names[3][3];
	int high_scores[3];

	//Ordered high score names & numbers
	char high_score[25];

	//Variables that need to be set
	int new_high_score = 600;
	char new_high_score_name[3] = "NEW";
	char textfile[] = "score.txt";

	if (device_reference != NULL) {

		if ((connected == 0) && (alt_up_sd_card_is_Present())) {
			printf("Card connected.\n");
			if (alt_up_sd_card_is_FAT16()) {
				printf("FAT16 file system detected.\n");
				//read_txt(&textfile); //prints out text content to console
				Highscore( new_high_score, &new_high_score_name, &high_score_names, &high_scores, &textfile);
				write_txt(&textfile, &high_score_names, &high_scores);
				print_array(&high_score_names, &high_scores, &high_score);
			}
			else {
				printf("Unknown file system.\n");
			}
			connected = 1;
		}
		else if ((connected == 1) && (alt_up_sd_card_is_Present() == false)) {
			printf("Card disconnected.\n");
			connected = 0;
		}


	}

	return 0;

}*/
void print_array(char (*new_names)[3], int *new_scores, char *high_score){
	int count=0;
	int row=2;
	int col=0;
	char temp[5];

	printf("print_array fnc\n");

	while( count < 35 && row >= 0 ){

		while( col < 5){
			high_score[count] = new_names[col][row];
			printf("%c",high_score[count]);
			col++;
			count++;
		}
		col = 0;
		sprintf(temp, "%d", new_scores[row]);
		high_score[count] = '\0';
		count++;

		while(col < 5){
		high_score[count] = temp[col];
		printf("%c",high_score[count]);
		count++;
		col++;
		}
		col=0;
		row--;
		high_score[count] = '\n';
		count++;
	}
}

void write_txt(char *file_name, char (*new_names)[3], int *new_scores){
	short int handle = alt_up_sd_card_fopen(file_name, 0);
	int i=0;
	int y=0;
	char temp[5];
	int temp_int=0;


	if(handle == 0)
		printf("File successfully opened in write txt.\n");
	else if(handle == -1){
		printf("File could not open in write text.\n");
	}
	else if(handle == -2) {
		printf("File already open in write txt \n");
	}

	while( i < 3 && handle > -1){
		sprintf(temp, "%d", new_scores[i]);
		y=0;
		temp_int = 0;
		//write score
		while( y < 5 && temp[y] != '\0' ){

			if( (new_scores[i] < (10.0*(pow(10.0,(3-temp_int))))) && (temp_int < 4)){
				alt_up_sd_card_write(handle, '0');
				//printf("score smaller than %f\n", (10.0*(pow(10.0,(3-temp_int)))));
			}
			else if(temp[y] == '0'){
				alt_up_sd_card_write(handle, '0');
				//printf("new_scores[%i] %c ",i,'0');
				y++;
			}
			else{
				alt_up_sd_card_write(handle, temp[y]);
				//printf("new_scores[%i] %c ",i,temp[y]);
				y++;
			}
			temp_int++;
		}
		y = 0;
		alt_up_sd_card_write(handle, '\0');
		//write name
		while( y < 5){
			alt_up_sd_card_write(handle, new_names[y][i]);
			//printf("%c ",new_names[y][i]);
			y++;
		}
		//printf("\n");
		//alt_up_sd_card_write(handle, '\n');
		alt_up_sd_card_write(handle, 13);
		alt_up_sd_card_write(handle, 10);
		i++;
	}
	alt_up_sd_card_fclose(handle);
}

void read_txt(char *file_name){
	short int handle = alt_up_sd_card_fopen(file_name,0);
	short content = alt_up_sd_card_read(handle);
	printf("start reading....\n");
	while(content > -1){
		printf("%c",content);
		content = alt_up_sd_card_read(handle);
	}

	alt_up_sd_card_fclose(handle);
}

void getScore(int * scores, char (*names)[3], char *textfile){
	//Open Scores saved in text file, saves scores and names to two arrays

	short int handle = alt_up_sd_card_fopen(textfile,0);
	short content=0;

	unsigned int x;
	unsigned int y;
	int temp;

	//File open check via Handle
	if( handle > 0 )
		printf("Txt file opened\n");
	else if( handle < 0)
		printf("Text file could not open in get score\n");

	x = 0;

	while( x < 3  && content > -1){
		temp = 0;
		y = 0;

		while(content < 32 ){
			content = alt_up_sd_card_read(handle);
		}
		//copy score
		while(content > -1 && y < 5){

			temp += (content - '0') * pow(10,(4-y));


			//printf("%i\n",temp);
			y++;
			content = alt_up_sd_card_read(handle);
		}

		scores[x] = temp;
		printf("score%i:%i",x,temp);
		y = 0;

		//copy name
		printf("	name%i:",x);
		while( content > -1 && y < 5){

			while(content < 33 && content > -1){ //skip spaces in names
				content = alt_up_sd_card_read(handle);
			}
			if(content == -1){
				break;
			}
			names[y][x] = content;
			printf("%c",content);
			y++;
			content = alt_up_sd_card_read(handle);
		}
		printf("\n");
		x++;

	}
	alt_up_sd_card_fclose(handle);
}

void Highscore(int new_score, char* new_name, char (*new_names)[3], int* new_scores, char *textfile)
//Checks if the input score replace 1 of 3 high scores
{
	int old_scores[3];
	char old_names[5][3];

	int i;
	printf("GetScore\n");
	getScore( &old_scores, &old_names, textfile);

	if( old_scores[0] <= new_score ){


		if( old_scores[1] <= new_score){

			if(old_scores[2] <= new_score){
				//new highest score
				printf("Highest Score!\n");
				i = 0;
				while( i < 2 ){
					new_scores[i] = old_scores[i+1];
					CopyNameList(&old_names, new_names, i, i+1);
					i++;
				}
				new_scores[2]= new_score;
				//ask user to input name, write the name in new_names[][2]
				CopyName(new_names, new_name, 2);

			}
			else{
				//New high score is second highest
				printf("Second Place!\n");
				i = 0;
				while(i < 3){
					if(i == 1){
						new_scores[i-1] = old_scores[i];
						CopyNameList(&old_names, new_names, i-1, i);
					}
					if(i == 2){
						new_scores[i] = old_scores[i];
						CopyNameList(&old_names, new_names, i, i);
					}
					i++;
				}
				new_scores[1] = new_score;
				//ask user to input name, write in new_name[][1]
				CopyName(new_names, new_name, 1);
			}
		}

		else{
			//New high score is third highest
			printf("Third Place!\n");
			i = 1;
			while(i < 3){
				new_scores[i] = old_scores[i];
				CopyNameList(&old_names, new_names, i, i);
				i++;
			}
			new_scores[0] = new_score;
			//ask user to input name, write the name in new_name[][0]
			CopyName(new_names, new_name, 0);
		}
	}
	else{
		printf("\nNo new High Score :(\n");
		i = 0;
		while(i < 3){
			new_scores[i] = old_scores[i];
			CopyNameList(&old_names, new_names, i, i);
			i++;
		}
	}
}

void CopyNameList (char (*old_names)[3], char (*new_names)[3], int new_loc, int old_loc){
	int i = 0;
	char temp;
	while (i < 5){
		temp = old_names[i][old_loc];
		new_names[i][new_loc] = temp;
		//printf("name%i,%i: %c",i, new_loc, temp);
		//printf("	array NameXY: %c\n", new_names[i][new_loc]);
		i++;
	}
}

void CopyName ( char (*new_names)[3], char *new_name, int pos){
	int i = 0;
	while( i < 5){
		new_names[i][pos] = new_name[i];
		i++;
	}
}
