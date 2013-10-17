#ifndef SCORE_H_
#define SCORE_H_

void print_array(char (*new_names)[3], int *new_scores, char *high_score);
void write_txt(char *file_name, char (*new_names)[3], int *new_scores);
void read_txt(char *file_name);
void getScore(int * scores, char (*names)[3], char *textfile);
void Highscore(int new_score, char* new_name, char (*new_names)[3], int* new_scores, char *textfile);
void CopyNameList (char (*old_names)[3], char (*new_names)[3], int new_loc, int old_loc);
void CopyName ( char (*new_names)[3], char *new_name, int pos);

#endif
