#ifndef CAT_H_
#define CAT_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// max count of chars in file
#define N 1000

// cleaning tmp string
void clean(char *str);

// main function for cat
void cat(char *file_name, char *flags[10], int flags_count);

// parsing flags
void get_flags(char *flags[10], int flags_count, int *e, int *n, int *b, int *t, int *s,
               int *v);

// parsing args from command line
void get_args(int argc, char *argv[argc], char *flags[10], char *files[100],
              int *flags_counter, int *files_counter);

// squeezing blanks if flag -s is active
void squeeze_blank(char *file_out, int *empty_flag);

// replacing chars in string with given char
void replace_char(char *file_out, char symb);

// add line number before printing line
void add_numering(char *file_out, int *str_num, char mode);

// show all char as visible symbols
void show_nonprinting(char *file_out);

#endif