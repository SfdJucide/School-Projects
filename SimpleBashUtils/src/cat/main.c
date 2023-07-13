#include "cat.h"

int main(int argc, char *argv[])
{
  char *flags[10];
  char *files[100];
  int flags_counter = 0;
  int files_counter = 0;
  get_args(argc, argv, flags, files, &flags_counter, &files_counter);

  for (int i = 0; i < files_counter; i++) {
    cat(files[i], flags, flags_counter);
  }
  return 0;
}
