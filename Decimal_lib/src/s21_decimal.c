#include "s21_decimal.h"

#include <float.h>
#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "checking/checking.h"
#include "utils/s21_utils.h"

// Arithmetic Operators

int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int check = OK;
  init_decimal(result);
  int res_done_yet = 0;
  s21_big_decimal big_val_1 = {{0, 0, 0, 0, 0, 0, 0}};
  s21_big_decimal big_val_2 = {{0, 0, 0, 0, 0, 0, 0}};
  big_copy_bits(value_1, &big_val_1);
  big_val_1.bits[6] = value_1.bits[3];
  big_copy_bits(value_2, &big_val_2);
  big_val_2.bits[6] = value_2.bits[3];
  if (big_get_scale(big_val_1) != big_get_scale(big_val_2)) {
    check = big_scale_eq(&big_val_1, &big_val_2);
  }
  s21_big_decimal tmp_res = {{0, 0, 0, 0, 0, 0, 0}};
  if (big_get_sign(big_val_1) == 0 && big_get_sign(big_val_2) == 0) {
    //  если оба положительные
    big_bit_addition(&big_val_1, &big_val_2, &tmp_res, OK);
  } else if (big_get_sign(big_val_1) && big_get_sign(big_val_2) == 0) {
    //   val_1 - отрицательное, val_2 - положительное
    big_set_sign(&big_val_1, 0);
    if (big_s21_is_equal(big_val_1, big_val_2)) {  // взаимоуничтожаются
      init_decimal(result);
      res_done_yet = 1;
    } else if (big_s21_is_greater(big_val_1, big_val_2)) {
      // если отрицательное больше
      big_convert_to_addcode(&big_val_2);
      big_bit_addition(&big_val_1, &big_val_2, &tmp_res, EXTRA_CODE);
      big_set_sign(&tmp_res, 1);
    } else {
      // если положительное больше
      big_convert_to_addcode(&big_val_1);
      big_bit_addition(&big_val_1, &big_val_2, &tmp_res, EXTRA_CODE);
      big_set_sign(&tmp_res, 0);
    }
    big_set_scale(&tmp_res, big_get_scale(big_val_1));
  } else if (!big_get_sign(big_val_1) && big_get_sign(big_val_2)) {
    //   val_1 - положительное, val_2 - отрицательное
    big_set_sign(&big_val_2, 0);
    if (big_s21_is_equal(big_val_1, big_val_2)) {
      init_decimal(result);
      // res_done_yet = 1;
    } else if (big_s21_is_greater(big_val_2, big_val_1)) {
      // если отрицательное больше
      big_convert_to_addcode(&big_val_1);
      big_bit_addition(&big_val_1, &big_val_2, &tmp_res, EXTRA_CODE);
      big_set_sign(&tmp_res, 1);
    } else {
      // если положительное больше
      big_convert_to_addcode(&big_val_2);
      big_bit_addition(&big_val_1, &big_val_2, &tmp_res, EXTRA_CODE);
      big_set_sign(&tmp_res, 0);
    }
    big_set_scale(&tmp_res, big_get_scale(big_val_1));
  } else {
    // оба отрицательных
    big_set_sign(&big_val_1, 0);
    big_set_sign(&big_val_2, 0);
    big_bit_addition(&big_val_1, &big_val_2, &tmp_res, OK);
    big_set_sign(&tmp_res, 1);
  }
  if (!res_done_yet) {
    big_set_scale(&tmp_res, big_get_scale(big_val_1));
    s21_big_decimal ten = {{10, 0, 0, 0, 0, 0, 0}};
    s21_big_decimal remainder = {{0, 0, 0, 0, 0, 0, 0}};
    int bank_round = 0;
    while (tmp_res.bits[5] || tmp_res.bits[4] || tmp_res.bits[3]) {
      if (!tmp_res.bits[5] && !tmp_res.bits[4] && tmp_res.bits[3] &&
          big_get_scale(tmp_res) == 1) {
        bank_round = 1;
        break;
      }
      if (big_get_scale(tmp_res)) {
        big_div_only_bits(tmp_res, ten, &remainder, &tmp_res);
        big_set_scale(&tmp_res, big_get_scale(tmp_res) - 1);
      } else {
        check = (!big_get_sign(tmp_res)) ? TOO_BIG : TOO_SMALL;
        init_decimal(result);
        break;
      }
    }
    if (bank_round) {
      big_div_only_bits(tmp_res, ten, &remainder, &tmp_res);
      big_set_scale(&tmp_res, big_get_scale(tmp_res) - 1);
      s21_big_decimal five = {{5, 0, 0, 0, 0, 0, 0}};
      if (big_s21_is_greater(remainder, five)) {
        check = (!big_get_sign(tmp_res)) ? TOO_BIG : TOO_SMALL;
        init_decimal(result);
      } else {
        if (big_s21_is_equal(remainder, five)) {
          if (!big_get_bit(tmp_res, 0)) {
            check = (!big_get_sign(tmp_res)) ? TOO_BIG : TOO_SMALL;
            init_decimal(result);
          }
        }
      }
    }
    if (check == OK) {
      result->bits[0] = tmp_res.bits[0];
      result->bits[1] = tmp_res.bits[1];
      result->bits[2] = tmp_res.bits[2];
      result->bits[3] = tmp_res.bits[6];
    }
  }
  return check;
}

int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  init_decimal(result);
  s21_big_decimal val_1 = {{0, 0, 0, 0, 0, 0, 0}};
  s21_big_decimal val_2 = {{0, 0, 0, 0, 0, 0, 0}};
  big_copy_bits(value_1, &val_1);
  big_copy_bits(value_2, &val_2);
  val_1.bits[6] = value_1.bits[3];
  val_2.bits[6] = value_2.bits[3];
  if (big_get_scale(val_1) != big_get_scale(val_2)) {
    big_scale_eq(&val_1, &val_2);
  }
  // print_bin_big_decimal(val_1);
  // print_bin_big_decimal(val_2);
  int check = OK;
  int res_sign = 1;
  int res_done = 0;
  s21_big_decimal tmp_res = {{0, 0, 0, 0, 0, 0, 0}};
  if (big_get_sign(val_1) != big_get_sign(val_2)) {
    // разные знаки, значит ++ или --
    res_sign = big_get_sign(val_1);
    big_set_sign(&val_1, 0);
    big_set_sign(&val_2, 0);
    big_bit_addition(&val_1, &val_2, &tmp_res, OK);
    big_set_sign(&tmp_res, res_sign);
  } else {
    // знаки одинаковые, значит +- или -+
    if (big_s21_is_equal(val_1, val_2)) {
      // числа одинаковые, значит взаимоуничтожаются
      init_decimal(result);
      res_done = 1;
    } else {
      // числа разные - решаем что из чего и какой знак
      big_set_sign(&val_1, 0);
      big_set_sign(&val_2, 0);
      //  смотрим какое из чисел больше по модулю
      if (big_s21_is_greater(val_1, val_2)) {
        // меньшее из двух переведем в доп код и сложим
        big_convert_to_addcode(&val_2);
        big_bit_addition(&val_1, &val_2, &tmp_res, EXTRA_CODE);
        res_sign = (get_sign(value_1) == 0) ? 0 : 1;
        big_set_sign(&tmp_res, res_sign);
      } else {
        // меньшее из двух переведем в доп код и сложим
        big_convert_to_addcode(&val_1);
        big_bit_addition(&val_1, &val_2, &tmp_res, EXTRA_CODE);
        res_sign = (get_sign(value_1) == 0) ? 1 : 0;
        big_set_sign(&tmp_res, res_sign);
      }
    }
  }
  if (!res_done) {
    big_set_scale(&tmp_res, big_get_scale(val_1));
    s21_big_decimal ten = {{10, 0, 0, 0, 0, 0, 0}};
    s21_big_decimal remainder = {{0, 0, 0, 0, 0, 0, 0}};
    int bank_round = 0;
    while (tmp_res.bits[5] || tmp_res.bits[4] || tmp_res.bits[3]) {
      if (!tmp_res.bits[5] && !tmp_res.bits[4] && tmp_res.bits[3] &&
          big_get_scale(tmp_res) == 1) {
        bank_round = 1;
        break;
      }
      if (big_get_scale(tmp_res)) {
        big_div_only_bits(tmp_res, ten, &remainder, &tmp_res);
        big_set_scale(&tmp_res, big_get_scale(tmp_res) - 1);
      } else {
        check = (!big_get_sign(tmp_res)) ? TOO_BIG : TOO_SMALL;
        init_decimal(result);
        break;
      }
    }
    if (bank_round) {
      big_div_only_bits(tmp_res, ten, &remainder, &tmp_res);
      big_set_scale(&tmp_res, big_get_scale(tmp_res) - 1);
      s21_big_decimal five = {{5, 0, 0, 0, 0, 0, 0}};
      if (big_s21_is_greater(remainder, five)) {
        check = (!big_get_sign(tmp_res)) ? TOO_BIG : TOO_SMALL;
        init_decimal(result);
      } else {
        if (big_s21_is_equal(remainder, five)) {
          if (!big_get_bit(tmp_res, 0)) {
            check = (!big_get_sign(tmp_res)) ? TOO_BIG : TOO_SMALL;
            init_decimal(result);
          }
        }
      }
    }
    if (check == OK) {
      result->bits[0] = tmp_res.bits[0];
      result->bits[1] = tmp_res.bits[1];
      result->bits[2] = tmp_res.bits[2];
      result->bits[3] = tmp_res.bits[6];
    }
  }
  return check;
}

int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int check = OK;
  init_decimal(result);
  s21_big_decimal val_1 = {{0, 0, 0, 0, 0, 0, 0}};
  s21_big_decimal val_2 = {{0, 0, 0, 0, 0, 0, 0}};
  s21_big_decimal big_result = {{0, 0, 0, 0, 0, 0, 0}};
  big_copy_bits(value_1, &val_1);
  big_copy_bits(value_2, &val_2);
  val_1.bits[6] = value_1.bits[3];
  val_2.bits[6] = value_2.bits[3];

  big_set_scale(&big_result, big_get_scale(val_1) + big_get_scale(val_2));
  big_set_sign(&big_result, big_get_sign(val_1) ^ big_get_sign(val_2));

  for (int i = 0; i < 5; i++) {
    for (int j = 0; j < 32; j++) {
      if (big_get_bit(val_2, j + i * 32)) {
        big_bit_addition(&big_result, &val_1, &big_result, OK);
        // print_bin_big_decimal(big_result);
      }
      big_move_left(&val_1, 1);
    }
  }
  // print_bin_big_decimal(big_result);

  s21_big_decimal ten = {{10, 0, 0, 0, 0, 0, 0}};
  s21_big_decimal remainder = {{0, 0, 0, 0, 0, 0, 0}};
  int bank_round = 0;
  while (big_result.bits[5] || big_result.bits[4] || big_result.bits[3] ||
         big_get_scale(big_result) > 28) {
    if ((!big_result.bits[5] && !big_result.bits[4] && big_result.bits[3] &&
         big_get_scale(big_result) == 1) ||
        (big_get_scale(big_result) == 29)) {
      bank_round = 1;
      break;
    }
    if (big_get_scale(big_result)) {
      big_div_only_bits(big_result, ten, &remainder, &big_result);
      big_set_scale(&big_result, big_get_scale(big_result) - 1);
    } else {
      check = (!big_get_sign(big_result)) ? TOO_BIG : TOO_SMALL;
      init_decimal(result);
      break;
    }
  }
  if (bank_round) {
    big_div_only_bits(big_result, ten, &remainder, &big_result);
    big_set_scale(&big_result, big_get_scale(big_result) - 1);
    s21_big_decimal five = {{5, 0, 0, 0, 0, 0, 0}};
    s21_big_decimal one = {{1, 0, 0, 0, 0, 0, 0}};
    if (big_s21_is_greater(remainder, five)) {
      if (big_get_scale(big_result) == 28) {
        big_bit_addition(&big_result, &one, &big_result, OK);
      } else {
        check = (!big_get_sign(big_result)) ? TOO_BIG : TOO_SMALL;
        init_decimal(result);
      }
    } else {
      if (big_s21_is_equal(remainder, five)) {
        if (big_get_scale(big_result) == 28) {
          big_bit_addition(&big_result, &one, &big_result, OK);
        } else {
          if (!big_get_bit(big_result, 0)) {
            check = (!big_get_sign(big_result)) ? TOO_BIG : TOO_SMALL;
            init_decimal(result);
          }
        }
      }
    }
  }
  if (check == OK) {
    result->bits[0] = big_result.bits[0];
    result->bits[1] = big_result.bits[1];
    result->bits[2] = big_result.bits[2];
    result->bits[3] = big_result.bits[6];
  }
  return check;
}

int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int check = OK;
  init_decimal(result);
  int res_done_yet = 1;
  s21_big_decimal val_1 = {{0, 0, 0, 0, 0, 0, 0}};
  s21_big_decimal val_2 = {{0, 0, 0, 0, 0, 0, 0}};
  big_copy_bits(value_1, &val_1);
  val_1.bits[6] = value_1.bits[3];
  big_copy_bits(value_2, &val_2);
  val_2.bits[6] = value_2.bits[3];
  big_set_sign(&val_1, 0);
  big_set_sign(&val_2, 0);
  s21_big_decimal one = {{1, 0, 0, 0, 0, 0, 0}};
  s21_big_decimal ten = {{10, 0, 0, 0, 0, 0, 0}};
  s21_big_decimal zero = {{0, 0, 0, 0, 0, 0, 0}};
  if (big_s21_is_equal(val_1, zero)) {
    init_decimal(result);
    res_done_yet = 0;
  } else if (big_s21_is_equal(val_2, zero)) {
    init_decimal(result);
    check = DIVISION_BY_ZERO;
  } else if (big_s21_is_equal(val_2, one)) {
    *result = value_1;
  } else {
    if (big_s21_is_equal(val_1, val_2)) {
      s21_decimal one = {{1, 0, 0, 0}};
      *result = one;
    } else {
      if (big_s21_is_greater(val_1, val_2)) {
        if (big_get_scale(val_1) != big_get_scale(val_2)) {
          big_scale_eq(&val_1, &val_2);
        }
      }
      s21_big_decimal tmp_res = {{0, 0, 0, 0, 0, 0, 0}};
      s21_big_decimal remainder = {{0, 0, 0, 0, 0, 0, 0}};
      big_div_only_bits(val_1, val_2, &remainder, &tmp_res);
      int scale_plus_count = 0;
      int check_too_small = 0;
      s21_big_decimal support_val_1 = val_1;
      s21_big_decimal support_val_2 = val_1;
      while (big_s21_is_equal(tmp_res, zero) ||
             !big_s21_is_equal(remainder, zero)) {
        if (scale_plus_count < 28) {
          // умножаем val_1 на 10
          big_move_left(&support_val_1, 3);
          big_move_left(&support_val_2, 1);
          big_bit_addition(&support_val_1, &support_val_2, &val_1, OK);
          scale_plus_count++;
          support_val_1 = val_1;
          support_val_2 = val_1;
          big_div_only_bits(val_1, val_2, &remainder, &tmp_res);
        } else {
          if (big_s21_is_equal(tmp_res, zero)) {
            check_too_small = 1;
          }
          break;
        }
      }
      if (big_s21_is_greater(val_2, val_1)) {
        if (get_scale(value_1) < get_scale(value_2)) {
          scale_plus_count = scale_plus_count + get_scale(value_1);
        }
      }
      while (tmp_res.bits[3] || tmp_res.bits[4] || tmp_res.bits[5]) {
        if (scale_plus_count) {
          big_div_only_bits(tmp_res, ten, &remainder, &tmp_res);
          scale_plus_count--;
        } else {
          check = TOO_BIG;
          init_decimal(result);
          break;
        }
      }
      if (scale_plus_count >= 28 && check_too_small) {
        if (scale_plus_count >= 28 && big_s21_is_equal(tmp_res, zero)) {
          check = TOO_SMALL;
          init_decimal(result);
        } else {
          while (tmp_res.bits[0] || tmp_res.bits[1] || tmp_res.bits[2]) {
            if (scale_plus_count >= 28 && big_s21_is_equal(tmp_res, zero)) {
              check = TOO_SMALL;
              init_decimal(result);
              break;
            } else {
              big_div_only_bits(tmp_res, ten, &remainder, &tmp_res);
              scale_plus_count--;
            }
          }
        }
      }
      big_set_scale(&tmp_res, scale_plus_count);
      if (check == OK && res_done_yet) {
        s21_big_decimal five = {{5, 0, 0, 0, 0, 0, 0}};
        if (big_s21_is_greater_or_equal(remainder, tmp_res)) {
          if (big_s21_is_greater(remainder, five)) {
            if (!big_get_bit(tmp_res, 0)) {
              big_bit_addition(&tmp_res, &one, &tmp_res, OK);
            }
          } else {
            big_bit_addition(&tmp_res, &one, &tmp_res, OK);
          }
        }
        result->bits[0] = tmp_res.bits[0];
        result->bits[1] = tmp_res.bits[1];
        result->bits[2] = tmp_res.bits[2];
        int sign_res = ((get_sign(value_1) && get_sign(value_2)) ||
                        (!get_sign(value_1) && !get_sign(value_2)))
                           ? 0
                           : 1;
        big_set_sign(&tmp_res, sign_res);
        result->bits[3] = tmp_res.bits[6];
      }
    }
  }
  return check;
}

int s21_mod(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int check = OK;
  init_decimal(result);
  int res_done_yet = 1;
  int sign_val_1 = get_sign(value_1);
  set_sign(&value_1, 0);
  int sign_val_2 = get_sign(value_2);
  set_sign(&value_2, 0);
  s21_decimal one = {{1, 0, 0, 0}};
  s21_decimal zero = {{0, 0, 0, 0}};
  if (s21_is_equal(value_1, zero)) {
    init_decimal(result);
    res_done_yet = 0;
  } else if ((s21_is_equal(value_2, one) && s21_is_greater(value_1, value_2)) ||
             s21_is_equal(value_2, value_1)) {
    *result = one;
  } else if (s21_is_equal(value_2, zero)) {
    init_decimal(result);
    check = DIVISION_BY_ZERO;
    res_done_yet = 0;
  } else if (s21_is_greater(value_1, value_2)) {
    if (!scale_eq(&value_1, &value_2)) {
      s21_decimal tmp_res = {{0, 0, 0, 0}};
      div_only_bits(value_1, value_2, result, &tmp_res);
    } else {
      init_decimal(result);
      res_done_yet = 0;
    }
  } else {
    *result = value_1;
  }
  if (res_done_yet) {
    int res_sign = (sign_val_1 <= sign_val_2) ? 0 : 1;
    set_sign(result, res_sign);
    set_scale(result, get_scale(value_1));
  }
  return check;
}

// Comparison Operators

int s21_is_greater(s21_decimal first, s21_decimal second) {
  int result = -1;
  int who_is_negative = is_negative(&first, &second);
  if (who_is_negative == 1) {
    result = 1;
  } else if (who_is_negative == -1) {
    result = 0;
  } else {
    if (get_scale(first) != get_scale(second)) {
      // result = (get_scale(first) > get_scale(second)) ? 1 : 0;
      scale_eq(&first, &second);
    }
    for (int i = 95; i >= 0 && result == -1; i--) {
      int bit_val_1 = get_bit(first, i);
      int bit_val_2 = get_bit(second, i);
      if (bit_val_1 && !bit_val_2) {
        result = 1;
      } else if (!bit_val_1 && bit_val_2) {
        result = 0;
      }
      if (result != -1) {
        if (get_sign(first) && get_sign(second)) {
          result = !result;
        }
      }
    }
  }

  if (s21_is_equal(first, second)) {
    result = 0;
  }
  return result;
}

int s21_is_less(s21_decimal first, s21_decimal second) {
  int result = 0;
  if (!s21_is_greater(first, second)) {
    result = 1;
  }
  if (s21_is_equal(first, second)) {
    result = 0;
  }
  return result;
}

int s21_is_equal(s21_decimal first, s21_decimal second) {
  int result = 1;
  int who_is_neg = is_negative(&first, &second);
  if (who_is_neg == 0 || !zero_check(first, second)) {
    if (get_scale(first) != get_scale(second)) {
      scale_eq(&first, &second);
    }
    for (int i = 0; i < 3; i++) {
      if (first.bits[i] != second.bits[i]) {
        result = 0;
        break;
      }
    }
  } else {
    result = 0;
  }
  return result;
}

int s21_is_not_equal(s21_decimal first, s21_decimal second) {
  int result = 1;
  if (s21_is_equal(first, second)) {
    result = 0;
  }

  return result;
}

int s21_is_greater_or_equal(s21_decimal first, s21_decimal second) {
  int res = 0;
  if (s21_is_greater(first, second) || s21_is_equal(first, second)) {
    res = 1;
  }
  return res;
}

int s21_is_less_or_equal(s21_decimal first, s21_decimal second) {
  int res = 0;
  if (s21_is_less(first, second) || s21_is_equal(first, second)) {
    res = 1;
  }
  return res;
}

// Convertors and parsers

int s21_from_int_to_decimal(int src, s21_decimal *dst) {
  init_decimal(dst);
  if (src < 0) {
    dst->bits[3] = MINUS_SIGN;
    src *= -1;
  }
  dst->bits[0] = src;
  return OK;
}

int s21_from_decimal_to_int(s21_decimal src, int *dst) {
  int negative = 1;
  int return_code = CONVERTATION_OK;
  int exp = get_scale(src);
  if (get_sign(src)) {
    negative = -1;
  }
  if (exp == 0) {  // без скейла
    unsigned long int num;
    if (src.bits[mid] == 0 && src.bits[high] == 0) {
      num = src.bits[low];
    } else {
      return_code = CONVERTATION_ERROR;
    }

    if (num > INT_MAX) {
      return_code = CONVERTATION_ERROR;
    } else {
      *dst = num * negative;
    }
  } else {  // со скейлом
    float tmp = 0;
    char full_num[96] = {'\0'};
    sprintf(full_num, "%.0f", get_full_bits(src));
    int k = 0;
    for (int i = strlen(full_num) - 1; i >= 0; i--) {
      if (exp == 0) {
        tmp += (full_num[i] - 48) * pow(10, k);
        k++;
      } else {
        exp--;
      }
    }

    float int_32max = 2147483647.0f;
    if (fabs(tmp) > int_32max) {
      return_code = CONVERTATION_ERROR;
    } else {
      *dst = (int)tmp * negative;
    }
  }

  return return_code;
}

int s21_from_float_to_decimal(float src, s21_decimal *dst) {
  init_decimal(dst);
  if (src == 0) {
    return OK;
  }
  char str_decimal[100] = {'\0'};
  int return_code = OK;
  int exp = 0;
  float float_no_exp = 0;
  if (is_too_big(fabs(src)) || is_too_small(fabs(src))) {
    return CONVERTATION_ERROR;
  }
  if (src < 0) {
    dst->bits[3] = MINUS_SIGN;
    src *= -1;
  }
  sprintf(str_decimal, "%.6e", src);
  // printf("%s\n", str_decimal);

  int exp_flag = 1, sign;
  int exp_pow = 0, float_pow = 0;
  for (int i = strlen(str_decimal) - 1; i >= 0; i--) {
    if (str_decimal[i] == '.') {
      continue;
    } else if (str_decimal[i] == '+' || str_decimal[i] == '-') {
      sign = (str_decimal[i] - 44) * -1;
    } else if (str_decimal[i] == 'e') {
      exp_flag = 0;
    } else {
      if (exp_flag) {
        exp += (str_decimal[i] - 48) * pow(10, exp_pow);
        exp_pow++;
      } else {
        float_no_exp += (str_decimal[i] - 48) * pow(10, float_pow);
        float_pow++;
      }
    }
  }
  // printf("%d %d\n", exp, sign);
  // printf("%f\n", float_no_exp);

  if (exp == 0) {
    while ((int)float_no_exp % 10 == 0) {
      float_no_exp /= 10;
      exp++;
    }
    dst->bits[low] = (int)float_no_exp;
    set_scale(dst, 6 - exp);
  } else {
    if (sign < 0) {
      while ((int)float_no_exp % 10 == 0) {
        float_no_exp /= 10;
        exp--;
      }
      dst->bits[low] = (int)float_no_exp;
      set_scale(dst, exp + 6);
    } else {
      if (exp >= 6) {
        while (exp - 6 > 0) {
          float_no_exp *= 10;
          exp--;
        }
        if (is_too_big(fabs(float_no_exp))) {
          return_code = CONVERTATION_ERROR;
        } else {
          float_to_bin(float_no_exp, dst);
        }
      } else {
        while ((int)float_no_exp % 10 == 0) {
          float_no_exp /= 10;
        }
        dst->bits[low] = (int)float_no_exp;
        int float_len = sprintf(str_decimal, "%d", (int)float_no_exp);
        // printf("%d %d - %d - 1 = %d\n", (int)float_no_exp, float_len, exp,
        // float_len - (exp + 1));
        set_scale(dst, float_len - (exp + 1));
      }
    }
  }

  if (get_scale(*dst) > 28) {
    return_code = CONVERTATION_ERROR;
  }
  // либо конвериторовать 7 цифр децимала и умножать децималы на 10^нужное
  // кол-во нулей
  return return_code;
}

int s21_from_decimal_to_float(s21_decimal src, float *dst) {
  int negative = 1;
  int return_code = CONVERTATION_OK;
  int exp = get_scale(src);
  if (get_sign(src)) {
    negative = -1;
  }
  *dst = get_full_bits(src);
  while (exp > 0) {
    *dst /= 10.0;
    exp--;
  }
  *dst *= negative;
  return return_code;
}

// Another functions

int s21_floor(s21_decimal value, s21_decimal *result) {
  int error = OK;
  init_decimal(result);
  int val_sign = get_sign(value);
  set_sign(&value, 0);
  int val_scale = get_scale(value);
  set_scale(&value, 0);
  s21_decimal one = {{1, 0, 0, 0}};
  s21_decimal ten = {{10, 0, 0, 0}};
  s21_decimal tmp_remainder = {{0, 0, 0, 0}};
  for (int i = val_scale; i > 0; i--) {
    if (s21_is_greater_or_equal(value, ten)) {
      div_only_bits(value, ten, &tmp_remainder, &value);
    } else {
      tmp_remainder = value;
      init_decimal(&value);
      break;
    }
  }
  if (val_sign) {
    s21_add(value, one, result);
    set_sign(result, 1);
  } else {
    *result = value;
  }
  return error;
}

int s21_truncate(s21_decimal value, s21_decimal *result) {
  int check = OK;
  init_decimal(result);
  s21_decimal ten = {{10, 0, 0, 0}};
  s21_decimal tmp_remainder = {{0, 0, 0, 0}};
  int val_sign = get_sign(value);
  set_sign(&value, 0);
  int scale = get_scale(value);
  set_scale(&value, 0);
  if (!scale) {
    *result = value;
  } else {
    if (s21_is_greater_or_equal(value, ten)) {
      div_only_bits(value, ten, &tmp_remainder, result);
      for (int i = scale - 1; i > 0; i--) {
        if (s21_is_greater_or_equal(value, ten)) {
          div_only_bits(*result, ten, &tmp_remainder, result);
        } else {
          tmp_remainder = value;
          init_decimal(&value);
          *result = value;
          break;
        }
      }
    } else {
      tmp_remainder = value;
      init_decimal(&value);
      *result = value;
    }
  }
  if (val_sign) {
    set_sign(result, 1);
  }
  return check;
}

int s21_round(s21_decimal value, s21_decimal *result) {
  int err = OK;
  init_decimal(result);
  s21_decimal one = {{1, 0, 0, 0}};
  s21_decimal five = {{5, 0, 0, DEFAULT_BASE}};
  int val_sign = get_sign(value);
  set_sign(&value, 0);
  s21_decimal val_trunc = {{0, 0, 0, 0}};
  s21_truncate(value, &val_trunc);
  s21_decimal tmp_remainder = {{0, 0, 0, 0}};
  s21_sub(value, val_trunc, &tmp_remainder);
  if (s21_is_greater_or_equal(tmp_remainder, five)) {
    s21_add(val_trunc, one, result);
  } else {
    *result = val_trunc;
  }
  set_sign(result, val_sign);
  return err;
}

int s21_negate(s21_decimal value, s21_decimal *res) {
  int return_code = OK;
  init_decimal(res);
  if (res) {
    value.bits[3] ^= MINUS_SIGN;
    *res = value;
  } else {
    return_code = 1;
  }
  return return_code;
}
