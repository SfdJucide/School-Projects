#include "grep.h"


void grep(char *file_name, char *pattern[100], char flags[100], int j,
          int n_files, int ptrns) {
  int count = 0;
  char file_out[N] = {'\0'};
  int is_pattern, count_of_in;
  int l_printed = 0;
  int num_of_str = 0;
  FILE *cat_file = fopen(file_name, "r");
  int v = 0;
  int c = 0;
  int i = 0;
  int l = 0;
  int n = 0;
  int h = 0;
  int s = 0;
  int o = 0;

  for (int u = 0; u < j; u++) {
    if (flags[u] == 'v') {
      v = 1;
    } else if (flags[u] == 'c') {
      c = 1;
    } else if (flags[u] == 'i') {
      i = 1;
    } else if (flags[u] == 'l') {
      l = 1;
    } else if (flags[u] == 'n') {
      n = 1;
    } else if (flags[u] == 'h') {
      h = 1;
    } else if (flags[u] == 's') {
      s = 1;
    } else if (flags[u] == 'o') {
      o = 1;
    }
  }

  if (cat_file) {
    while (!feof(cat_file)) {
      num_of_str++;
      fgets(file_out, 1000, cat_file);
      if (file_out[0] != '\0') {
        count_of_in = 0;
        for (int m = 0; m < ptrns; m++) {
          is_pattern = !find_regex(file_out, pattern[m], i);
          if (is_pattern) {
            count_of_in++;
          }

          if (c) {
            if (v) {
              if (!is_pattern && count_of_in == 0 && m == ptrns - 1) {
                count++;
                if (!l && !v) {
                  break;
                }
              }
            } else {
              if (is_pattern) {
                count++;
                if (!l) {
                  break;
                }
              }
            }
          }
          if (l) {
            if ((is_pattern && !v) ||
                (v && !is_pattern && count_of_in == 0 && m == ptrns - 1)) {
              l_printed = 1;
              break;
            }
          }
          if (is_pattern && !c && !v) {
            if (n) {
              if (!o) {
                (n_files > 1 && !h)
                    ? printf("%s:%d:%s", file_name, num_of_str, file_out)
                    : printf("%d:%s", num_of_str, file_out);
              } else {
                flag_o(file_out, pattern, file_name, i, n, h, num_of_str, ptrns,
                       n_files);
              }
            } else {
              if (!o) {
                (n_files > 1 && !h) ? printf("%s:%s", file_name, file_out)
                                    : printf("%s", file_out);
              } else {
                flag_o(file_out, pattern, file_name, i, n, h, num_of_str, ptrns,
                       n_files);
              }
            }
            break;
          }
        }
        if (l_printed) {
          break;
        }

        if (!is_pattern && count_of_in == 0 && v && strlen(file_out) > 0 &&
            !c) {
          if (n) {
            (n_files > 1 && !h)
                ? printf("%s:%d:%s", file_name, num_of_str, file_out)
                : printf("%d:%s", num_of_str, file_out);  // output for -v
          } else {
            (n_files > 1 && !h) ? printf("%s:%s", file_name, file_out)
                                : printf("%s", file_out);  // output for -v
          }
        }

        if (!o || v) {
          if (((file_out[strlen(file_out) - 1] != '\n') && (is_pattern) && !c &&
               !v) ||
              (v && count_of_in == 0 &&
               file_out[strlen(file_out) - 1] != '\n' && !c)) {
            printf("\n");
          }
        }

        clean(file_out);
      }
    }

    if (c) {
      (n_files > 1 && !h) ? printf("%s:%d\n", file_name, count)
                          : printf("%d\n", count);
    }
    if (l_printed) {
      printf("%s\n", file_name);
    }

    fclose(cat_file);
  } else if (!s) {
    fprintf(stderr, "grep: %s: No such file or directory\n", file_name);
  }
}

void get_args(int argc, char *argv[argc], char flags[100], char *patterns[100],
              char *files[100], int *j, int *k, int *l, int *free_arr,
              int *arr_idx) {
  for (int i = 1; i < argc; i++) {
    if (argv[i][0] == '-') {
      int y = i;
      for (size_t p = 1; p < strlen(argv[y]); p++) {
        switch (argv[y][p]) {
          case 'e':
            i++;
            y++;
            patterns[*k] = argv[y];
            *k += 1;
            y--;
            break;
          case 'f':;
            FILE *tmp_f = fopen(argv[++i], "r");
            if (tmp_f) {
              while (!feof(tmp_f)) {
                patterns[*k] = malloc(sizeof(char) * N);
                free_arr[*arr_idx] = *k;
                *arr_idx += 1;
                fgets(patterns[*k], N, tmp_f);
                if (patterns[*k][strlen(patterns[*k]) - 1] == '\n') {
                  patterns[*k][strlen(patterns[*k]) - 1] = '\0';
                }
                *k += 1;
              }
            }
            fclose(tmp_f);
            break;

          default:
            flags[*j] = argv[y][p];
            *j += 1;
            break;
        }
      }

    } else {
      if (*k == 0) {
        patterns[*k] = argv[i];
        *k += 1;
      } else {
        files[*l] = argv[i];
        *l += 1;
      }
    }
  }
}

int find_regex(char *file_out, char *pattern, int i) {
  regex_t re;
  int val;
  regcomp(&re, pattern, (i) ? REG_ICASE : REG_EXTENDED);
  val = regexec(&re, file_out, 0, NULL, 0);
  regfree(&re);
  return val;
}

void clean(char *var) {
  int i = 0;
  while (var[i] != '\0') {
    var[i] = '\0';
    i++;
  }
}

void flag_o(char *find_str, char *pattern[100], char *file_name, int i, int n,
            int h, int num_of_str, int ptrns, int n_files) {
  int n_printed = 0;
  for (int l = 0; l < ptrns; l++) {
    regex_t re;
    char out[100] = {'\0'};
    regmatch_t pmatch[100];
    int status;
    char *ps = find_str;

    int eflag;
    int len_out;

    status = regcomp(&re, pattern[l], (i) ? REG_ICASE : REG_EXTENDED);
    eflag = 0;

    /* Найти все соответствия */
    while ((status = regexec(&re, ps, 1, pmatch, eflag)) == 0) {
      len_out = pmatch[0].rm_eo - pmatch[0].rm_so;
      strncpy(out, ps + pmatch[0].rm_so, len_out);
      if (n && !n_printed) {
        (n_files > 1 && !h) ? printf("%s:%d:%s\n", file_name, num_of_str, out)
                            : printf("%d:%s\n", num_of_str, out);
        n_printed = 1;
      } else {
        (n_files > 1 && !h) ? printf("%s:%s\n", file_name, out)
                            : printf("%s\n", out);
      }
      ps += pmatch[0].rm_eo;
      eflag = REG_NOTBOL;
    }

    regfree(&re);
  }
}