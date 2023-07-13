#include "cat.h"


void cat(char *file_name, char *flags[10], int flags_count) {
  int e = 0;
  int n = 0;
  int b = 0;
  int t = 0;
  int s = 0;
  int v = 0;
  int empty_flag = 0;
  int line_num = 0;
  char file_out[N] = {'\0'};
  FILE *cat_file = fopen(file_name, "r");

  get_flags(flags, flags_count, &e, &n, &b, &t, &s, &v);

  if (cat_file) {
    while (!feof(cat_file)) {
      fgets(file_out, 1000, cat_file);
      if (file_out[0] != '\0') {
        if (s) {
          squeeze_blank(file_out, &empty_flag);
        }
        if (t) {
          replace_char(file_out, '\t');
        }
        if ((n || b) && empty_flag < 2) {
          if (b) {
            add_numering(file_out, &line_num, 'b');
          }
          else if (n) {
            add_numering(file_out, &line_num, 'n');
          }
        }
        if (e) {
          replace_char(file_out, '\n');
        }

        if (v) {
          show_nonprinting(file_out);
        }

        if (empty_flag < 2) {
          printf("%s", file_out);
        }

        clean(file_out);
      }
      else {
        break;
      }
    }
    fclose(cat_file);
  }
  else {
    fprintf(stderr, "cat: %s: No such file or directory\n", file_name);
  }
}

void clean(char *str) {
  int i = 0;
  while (str[i] != '\0')
  {
    str[i] = '\0';
    i++;
  }
}

void get_flags(char *flags[10], int flags_count, int *e, int *n, int *b, int *t, int *s,
               int *v) {
  for (int i = 0; i < flags_count; i++) {
    if (!strcmp(flags[i], "-e") || !strcmp(flags[i], "-E")) {
      if (!strcmp(flags[i], "-e")) {
        *v = 1;
      }
      *e = 1;
    }
    else if (!strcmp(flags[i], "-n") || !strcmp(flags[i], "--number")) {
      *n = 1;
    }
    else if (!strcmp(flags[i], "-b") ||
             !strcmp(flags[i], "--number-nonblank")) {
      *b = 1;
    }
    else if (!strcmp(flags[i], "-t") || !strcmp(flags[i], "-T")) {
      if (!strcmp(flags[i], "-t")) {
        *v = 1;
      }
      *t = 1;
    }
    else if (!strcmp(flags[i], "-s") ||
             !strcmp(flags[i], "--squeeze-blank")) {
      *s = 1;
    }
    else if (!strcmp(flags[i], "-v")) {
      *v = 1;
    }
  }
}

void squeeze_blank(char *file_out, int *empty_flag) {
  if (file_out[0] == '\n') {
    *empty_flag += 1;
  }
  else {
    *empty_flag = 0;
  }
}

void replace_char(char *file_out, char symb) {
  char *ptr;
  int f = 1;
  switch (symb) {
  case '\n':
    ptr = strchr(file_out, '\n');
    if (ptr) {
      memset(ptr, '$', 1);
      memset(++ptr, '\n', 1);
    }
    break;

  case '\t':
    while (ptr || f) {
      f = 0;
      ptr = strchr(file_out, '\t');
      char tmp[N];
      if (ptr) {
        memset(ptr, '^', 1);
        ptr++;
        strcpy(tmp, ptr);
        memset(ptr, 'I', 1);
        ptr++;
        strcpy(ptr, tmp);
      }
    }
    break;
  }
}

void add_numering(char *file_out, int *str_num, char mode) {
  char tmp[N];
  char num[100];
  char *ptr = tmp;
  if (!(file_out[0] == '\n') || mode == 'n') {
    *str_num += 1;
    sprintf(num, "%6.d\t", (*str_num));
    strcpy(ptr, num);
    ptr += strlen(num);
  }
  strcpy(ptr, file_out);
  strcpy(file_out, tmp);
}

void show_nonprinting(char *file_out) {
  char tmp[N] = {'\0'};
  char *ptr = tmp;
  int f = 0;
  size_t i;
  char *h = strchr(file_out, '\0');
  if (*++h > 0) {
    f = 1;
  }
  for (i = 0; i < strlen(file_out) + f * 3; i++) {
    if (((int)file_out[i] <= 31 && (int)file_out[i] != 9 &&
         (int)file_out[i] != 10) ||
        (int)file_out[i] == 127) {
      memset(ptr, '^', 1);
      ptr++;
      if ((int)file_out[i] == 127) {
        memset(ptr, (char)(((int)file_out[i] + 64) & 127), 1);
      }
      else {
        memset(ptr, (char)((int)file_out[i] + 64), 1);
      }
      ptr++;
    }
    else {
      memset(ptr, file_out[i], 1);
      ptr++;
    }
  }
  strcpy(file_out, tmp);
}

void get_args(int argc, char *argv[argc], char *flags[10], char *files[100],
              int *flags_counter, int *files_counter) {
  int f_flag = 1;
  for (int i = 1; i < argc; i++) {
    if (f_flag & (!strcmp(argv[i], "-n") || !strcmp(argv[i], "-b") ||
                  !strcmp(argv[i], "-e") || !strcmp(argv[i], "-v") ||
                  !strcmp(argv[i], "-t") || !strcmp(argv[i], "-s") ||
                  !strcmp(argv[i], "--number") || !strcmp(argv[i], "-T") ||
                  !strcmp(argv[i], "-E") || !strcmp(argv[i], "--number-nonblank") ||
                  !strcmp(argv[i], "--squeeze-blank"))) {
      flags[*flags_counter] = argv[i];
      *flags_counter += 1;
    }
    else {
      files[*files_counter] = argv[i];
      *files_counter += 1;
      f_flag = 0;
    }
  }
}
