#include <math.h>
#include <stdio.h>
#include <string.h>

#include "s21_utils.h"

void init_big_decimal(s21_big_decimal *val) {
  memset(&(val->bits), 0, sizeof(val->bits));
}

int big_get_bit(s21_big_decimal val, int i) {
  int res = 0;
  if (i / 32 < 7) {
    int mask = 1 << (i % 32);
    res = val.bits[i / 32] & mask;
  } else {
    res = -1;
  }
  return res;
}

void big_set_bit(s21_big_decimal *val, int i, int value) {
  int mask = 1 << (i % 32);
  if (i / 32 < 7) {
    if (value) {
      val->bits[i / 32] |= mask;
    } else {
      val->bits[i / 32] &= ~mask;
    }
  }
}

int big_get_sign(s21_big_decimal val) {
  int mask = 1 << 31;
  return !!(val.bits[6] & mask);
}

void big_set_sign(s21_big_decimal *val, int sign) {
  int mask = 1 << 31;
  if (sign) {
    val->bits[6] |= mask;
  } else {
    val->bits[6] &= ~mask;
  }
}

int big_get_scale(s21_big_decimal val) {
  int res = val.bits[6] >> 16 & 255;
  return res;
}

void big_set_scale(s21_big_decimal *val, int scale) {
  if (scale >= 0) {
    for (int i = 16; i <= 23; ++i) {
      big_set_bit(val, i + 192, scale % 2);
      scale /= 2;
    }
  }
}

void big_convert_to_addcode(s21_big_decimal *val_1) {
  s21_big_decimal result = {{0, 0, 0, 0, 0, 0, 0}};
  s21_big_decimal one = {{1, 0, 0, 0, 0, 0, 0}};
  // инвертируем мантиссу
  val_1->bits[low] = ~val_1->bits[low];
  val_1->bits[mid] = ~val_1->bits[mid];
  val_1->bits[high] = ~val_1->bits[high];
  val_1->bits[3] = ~val_1->bits[3];
  val_1->bits[4] = ~val_1->bits[4];
  val_1->bits[5] = ~val_1->bits[5];
  // прибавляем 1
  big_bit_addition(val_1, &one, &result, OK);
  val_1->bits[low] = result.bits[low];
  val_1->bits[mid] = result.bits[mid];
  val_1->bits[high] = result.bits[high];
  val_1->bits[3] = result.bits[3];
  val_1->bits[4] = result.bits[4];
  val_1->bits[5] = result.bits[5];
}

int big_scale_eq(s21_big_decimal *val_1, s21_big_decimal *val_2) {
  // s21_big_decimal *big = NULL;
  int check = OK;
  s21_big_decimal *small = NULL;
  if (big_get_scale(*val_1) > big_get_scale(*val_2)) {
    // big = val_1;
    small = val_2;
  } else {
    // big = val_2;
    small = val_1;
  }
  int scaleSmall = 0;
  // int check_tmp = OK;
  s21_big_decimal tmp1 = {{0, 0, 0, 0, 0, 0, 0}};
  while ((big_get_scale(*val_1) != big_get_scale(*val_2))) {
    // повышение скейла
    scaleSmall = big_get_scale(*small);
    tmp1 = *small;
    // умножение на 10
    for (int i = 0; i < 9; i++) {
      check = big_bit_addition(small, &tmp1, small, OK);
      if (check == TOO_BIG) {
        break;
      }
      // printf("!!!!!\n");
    }
    // if (check_tmp == OK) {
    big_set_scale(small, scaleSmall + 1);
    //}
  }
  return check;
}

void big_copy_bits(s21_decimal src, s21_big_decimal *dest) {
  dest->bits[low] = src.bits[low];
  dest->bits[mid] = src.bits[mid];
  dest->bits[high] = src.bits[high];
}

int big_move_left(s21_big_decimal *val, int move_value) {
  // int lastbit = big_last_bit(*val);
  int err = OK;
  // if (lastbit + move_value > 159) {
  //   err = TOO_BIG;
  // } else {
  for (int i = 0; i < move_value; i++) {
    int val_31bit = big_get_bit(*val, 31);
    int val_63bit = big_get_bit(*val, 63);
    int val_95bit = big_get_bit(*val, 95);
    int val_127bit = big_get_bit(*val, 127);
    int val_159bit = big_get_bit(*val, 159);
    val->bits[0] <<= 1;
    val->bits[1] <<= 1;
    val->bits[2] <<= 1;
    val->bits[3] <<= 1;
    val->bits[4] <<= 1;
    val->bits[5] <<= 1;
    if (val_31bit) {
      big_set_bit(val, 32, 1);
    }
    if (val_63bit) {
      big_set_bit(val, 64, 1);
    }
    if (val_95bit) {
      big_set_bit(val, 96, 1);
    }
    if (val_127bit) {
      big_set_bit(val, 128, 1);
    }
    if (val_159bit) {
      big_set_bit(val, 160, 1);
    }
  }
  return err;
}

void big_div_only_bits(s21_big_decimal value_1, s21_big_decimal value_2,
                       s21_big_decimal *div_remainder,
                       s21_big_decimal *result) {
  int tmp = 1;
  int check = 1;
  int scale_val_1 = big_get_scale(value_1);
  // int scale_val_2 = big_get_scale(value_2);
  big_set_scale(&value_1, 0);
  big_set_scale(&value_2, 0);
  int sign_val_1 = big_get_sign(value_1);
  big_set_sign(&value_1, 0);
  // int sign_val_2 = big_get_sign(value_2);
  big_set_sign(&value_2, 0);
  if (big_s21_is_greater_or_equal(value_1, value_2)) {
    s21_big_decimal sum = {{0, 0, 0, 0, 0, 0, 0}};
    s21_big_decimal t_val = {{0, 0, 0, 0, 0, 0, 0}};
    s21_big_decimal t_val_sup = {{0, 0, 0, 0, 0, 0, 0}};
    s21_big_decimal dif = {{0, 0, 0, 0, 0, 0, 0}};
    while (check) {
      t_val_sup = value_2;
      int i = 0;
      while (tmp) {
        t_val = value_2;
        big_move_left(&t_val, i);
        if (big_s21_is_greater(t_val, value_1)) {
          tmp = 0;
          big_move_left(&t_val_sup, i - 1);
          t_val = t_val_sup;
          s21_big_decimal one = {{1, 0, 0, 0, 0, 0, 0}};
          big_move_left(&one, i - 1);
          s21_big_decimal sum_sup = one;
          big_bit_addition(&sum, &sum_sup, &sum, OK);
        }
        i++;
      }
      tmp = 1;
      big_convert_to_addcode(&t_val);
      big_bit_addition(&value_1, &t_val, &dif, EXTRA_CODE);
      if (big_s21_is_greater_or_equal(dif, value_2)) {
        value_1 = dif;
      } else {
        check = 0;
        *div_remainder = dif;
        *result = sum;
      }
    }
    big_set_scale(result, scale_val_1);
    big_set_sign(result, sign_val_1);
  } else {
    init_big_decimal(result);
    *div_remainder = value_1;
  }
}

int big_bit_addition(s21_big_decimal *val_1, s21_big_decimal *val_2,
                     s21_big_decimal *result, int code) {
  int err = OK;
  int one_in_head = 0;
  for (int i = 0; i < 192; i++) {
    int bit_val_1 = big_get_bit(*val_1, i);
    int bit_val_2 = big_get_bit(*val_2, i);
    if (bit_val_1 == 0 && bit_val_2 == 0) {
      if (one_in_head) {
        big_set_bit(result, i, 1);
        one_in_head = 0;
      } else {
        big_set_bit(result, i, 0);
      }
    } else if (bit_val_1 != bit_val_2) {
      if (one_in_head) {
        big_set_bit(result, i, 0);
        one_in_head = 1;
      } else {
        big_set_bit(result, i, 1);
      }
    } else if (bit_val_1 && bit_val_2) {
      if (one_in_head) {
        big_set_bit(result, i, 1);
        one_in_head = 1;
      } else {
        big_set_bit(result, i, 0);
        one_in_head = 1;
      }
    }
    if (i == 191 && one_in_head == 1 && code != EXTRA_CODE) {
      err = TOO_BIG;
    }
  }
  return err;
}

int big_zero_check(s21_big_decimal val_1, s21_big_decimal val_2) {
  int is_zero = 1;
  if (!val_1.bits[0] && !val_2.bits[0] && !val_1.bits[1] && !val_2.bits[1] &&
      !val_1.bits[2] && !val_2.bits[2] && !val_1.bits[3] && !val_2.bits[3] &&
      !val_1.bits[4] && !val_2.bits[4] && !val_1.bits[5] && !val_2.bits[5]) {
    is_zero = 0;
  }
  return is_zero;
}

int big_s21_is_equal(s21_big_decimal first, s21_big_decimal second) {
  int result = 1;
  if ((big_get_sign(first) == big_get_sign(second)) ||
      !big_zero_check(first, second)) {
    if (big_get_scale(first) != big_get_scale(second)) {
      big_scale_eq(&first, &second);
    }
    for (int i = 0; i < 6; i++) {
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

int big_s21_is_greater(s21_big_decimal first, s21_big_decimal second) {
  int result = -1;
  if (big_get_sign(first) == 0 && big_get_sign(second) == 1) {
    result = 1;
  } else if (big_get_sign(first) == 1 && big_get_sign(second) == 0) {
    result = 0;
  } else {
    if (big_get_scale(first) != big_get_scale(second)) {
      // result = (get_scale(first) > get_scale(second)) ? 1 : 0;
      big_scale_eq(&first, &second);
    }
    for (int i = 191; i >= 0 && result == -1; i--) {
      int bit_val_1 = big_get_bit(first, i);
      int bit_val_2 = big_get_bit(second, i);
      if (bit_val_1 && !bit_val_2) {
        result = 1;
      } else if (!bit_val_1 && bit_val_2) {
        result = 0;
      }
      if (result != -1) {
        if (big_get_sign(first) && big_get_sign(second)) {
          result = !result;
        }
      }
    }
  }

  if (big_s21_is_equal(first, second)) {
    result = 0;
  }
  return result;
}

int big_s21_is_greater_or_equal(s21_big_decimal first, s21_big_decimal second) {
  int res = 0;
  if (big_s21_is_greater(first, second) || big_s21_is_equal(first, second)) {
    res = 1;
  }
  return res;
}

void print_decimal2(s21_big_decimal val) {
  for (int k = 0; k < 7; k++) {
    printf("%u ", val.bits[k]);
  }
  printf("\n");
}

void print_bin_big_decimal(s21_big_decimal val) {
  for (int i = 0; i < 7; i++) {
    for (int j = 31; j >= 0; j--) {
      if (val.bits[i] & (1 << j)) {
        printf("1");
      } else {
        printf("0");
      }
    }
    printf(" ");
  }

  printf("\n");
}
