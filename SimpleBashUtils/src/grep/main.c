#include "grep.h"

int main(int argc, char *argv[]) {
  char flags[100];
  char *patterns[100];
  char *files[100];
  int j = 0;  // flags
  int k = 0;  // patterns
  int l = 0;  // files

  int *arr = (int *)malloc(sizeof(int) * 100);
  int yo = 0;
  get_args(argc, argv, flags, patterns, files, &j, &k, &l, arr, &yo);

  for (int i = 0; i < l; i++) {
    grep(files[i], patterns, flags, j, l, k);
  }

  for (int i = 0; i < yo; i++) {
    free(patterns[arr[i]]);
  }

  free(arr);

  return 0;
}