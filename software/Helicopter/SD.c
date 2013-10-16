#include <stdio.h>
#include <altera_up_sd_card_avalon_interface.h>
#include <SD.h>


/*
int main(void) {
	alt_up_sd_card_dev *device_reference = NULL;
	int connected = 0;
	device_reference = alt_up_sd_card_open_dev("/dev/Altera_UP_SD_Card_Avalon_Interface_0");

	char first_file[32];
	char file[32];
	//	char file_name[32];
//	unsigned char content = '0';
	int handle;


	if (device_reference != NULL) {

		if ((connected == 0) && (alt_up_sd_card_is_Present())) {
			printf("Card connected.\n");
			if (alt_up_sd_card_is_FAT16()) {
				printf("FAT16 file system detected.\n");

				print_directory(&file, &first_file);
				handle = InitMapFromSDCARD(&file);
			//	handle = read_txt(&file);
				//write_txt(&file, handle, content);
			//	read_txt(&file);
			} else {
				printf("Unknown file system.\n");
			}
			connected = 1;
		} else if ((connected == 1) && (alt_up_sd_card_is_Present() == false)) {
			printf("Card disconnected.\n");
			connected = 0;
		}


	}

	return 0;
}
*/

void print_directory(char * file, char * first_file) {

	if(alt_up_sd_card_find_first(0, first_file) == 0);
	printf("%s\n", first_file);
	while(alt_up_sd_card_find_next(file) == 0)
		printf("%s\n", file);
}

int InitMapFromSDCARD(char * file_name){
	int i;
	int j;
	char map[320][240];
	char David;

	short int handle = alt_up_sd_card_fopen(file_name, 0);
	if(handle == 0)
		printf("File successfully opened.\n");
	else if(handle == -1){
		printf("File could not open.\n");
		return;
	}
	else if(handle == -2) {
		printf("File already open.\n");
		return;
	}

	while( handle != -1){

		for(i = 0; i < 320; i++)
		{
			for(j = 0; i < 240; i++)
			{
				map[i][j] = alt_up_sd_card_read(handle);
			}
		}

		for(i = 0; i < 320; i++)
				{
					for(j = 0; i < 240; i++)
					{
						David = map[i][j];
						printf("%c",David);
					}
					printf("\n");
				}
	}
	return handle;
}
