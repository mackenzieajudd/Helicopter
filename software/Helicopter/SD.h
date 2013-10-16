#ifndef SD_H_
#define SD_H_

#define GROUND "0"
#define AIR "1"
#define BASE_DIFFUCULTY 60
#define HEIGHT_MAX 0
#define HEIGHT_MIN 239
#define LENGTH_MAX 319
#define LENGTH_MIN 0
#define STEP_SIZE 4

void print_directory();
int read_txt(file_name);
void write_txt(file_name, handle, content);
int InitMapFromSDCARD(char * file_name);

#endif
