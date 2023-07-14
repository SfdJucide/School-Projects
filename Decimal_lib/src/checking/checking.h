#ifndef CHECKING_CHECKING_H_
#define CHECKING_CHECKING_H_

#include "../s21_decimal.h"

/// @brief Проверка является ли число меньше минимального decimal
/// @param val число
/// @return 1 - если число меньше, иначе - 0
int is_too_small(float val);

/// @brief Проверка является ли число больше максимального decimal
/// @param val число
/// @return 1 - если число больше, иначе - 0
int is_too_big(float val);

#endif
