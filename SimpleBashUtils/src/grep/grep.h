#ifndef GREP_H_
#define GREP_H_

#include <ctype.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// max count of chars in file
#define N 1000

// parse args from command line
void get_args(int argc, char *argv[argc], char flags[100], char *patterns[100],
              char *files[100], int *j, int *k, int *l, int *free_arr,
              int *arr_idx);

// main function for grep
void grep(char *file_name, char *pattern[100], char flags[100], int j,
          int n_files, int ptrns);

// special action when flag -o is active
void flag_o(char *find_str, char *pattern[100], char *file_name, int i, int n,
            int h, int num_of_str, int ptrns, int n_files);

// get lines with regex
int find_regex(char *file_out, char *pattern, int i);

// clean tmp string
void clean(char *var);

#endif