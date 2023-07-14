#include "checking.h"

#include <stdio.h>

int is_too_small(float val) {
  int result = 0;
  int scale = 0;
  while ((int)val == 0) {
    scale++;
    val *= 10;
  }
  if (scale > 28) result = 1;
  return result;
}

int is_too_big(float val) {
  int result = 0;
  if (val - 79228162514264337593543950335.0 > 0) {
    result = 1;
  }
  return result;
}
