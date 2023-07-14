#include "s21_utils.h"

#include <math.h>
#include <stdio.h>
#include <string.h>

void print_decimal(s21_decimal val) {
  for (int k = 0; k < 4; k++) {
    printf("%u ", val.bits[k]);
  }
  printf("\n");
}

void print_bin_decimal(s21_decimal val) {
  for (int i = 0; i < 4; i++) {
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

void init_decimal(s21_decimal *val) {
  memset(&(val->bits), 0, sizeof(val->bits));
}

int get_bit(s21_decimal val, int i) {
  int res = 0;
  if (i / 32 < 4) {
    int mask = 1 << (i % 32);
    res = val.bits[i / 32] & mask;
  } else {
    res = -1;
  }
  return res;
}

float get_full_bits(s21_decimal val) {
  int bin_num[96] = {0};
  int k = 0;
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 32; j++) {
      if (val.bits[i] & (1 << j)) {
        bin_num[k] = 1;
      }
      k++;
    }
  }

  float num = 0;
  for (k = 0; k < 96; k++) {
    if (bin_num[k] == 1) {
      num += bin_num[k] * pow(2, k);
    }
  }

  return num;
}

void set_bit(s21_decimal *val, int i, int value) {
  int mask = 1 << (i % 32);
  if (i / 32 < 7) {
    if (value) {
      val->bits[i / 32] |= mask;
    } else {
      val->bits[i / 32] &= ~mask;
    }
  }
}

int last_bit(s21_decimal val) {
  int last_bit = 95;
  while (last_bit >= 0 && !get_bit(val, last_bit)) {
    last_bit--;
  }
  return last_bit;
}

int get_sign(s21_decimal val) {
  int mask = 1 << 31;
  return !!(val.bits[scale] & mask);
}

void set_sign(s21_decimal *val, int sign) {
  int mask = 1 << 31;
  if (sign) {
    val->bits[scale] |= mask;
  } else {
    val->bits[scale] &= ~mask;
  }
}

int get_scale(s21_decimal val) {
  int res = val.bits[scale] >> 16 & 255;
  return res;
}

void set_scale(s21_decimal *val, int scale) {
  if (scale >= 0 && scale <= 28) {
    for (int i = 16; i <= 23; ++i) {
      set_bit(val, i + 96, scale % 2);
      scale /= 2;
    }
  }
}

void convert_to_addcode(s21_decimal *val_1) {
  s21_decimal result = {{0, 0, 0, 0}};
  s21_decimal add_1 = {{1, 0, 0, 0}};
  // инвертируем мантиссу
  val_1->bits[low] = ~val_1->bits[low];
  val_1->bits[mid] = ~val_1->bits[mid];
  val_1->bits[high] = ~val_1->bits[high];
  // прибавляем 1
  bit_addition(val_1, &add_1, &result, OK);
  val_1->bits[low] = result.bits[low];
  val_1->bits[mid] = result.bits[mid];
  val_1->bits[high] = result.bits[high];
}

int scale_eq(s21_decimal *val_1, s21_decimal *val_2) {
  s21_decimal *small = NULL;
  if (get_scale(*val_1) > get_scale(*val_2)) {
    small = val_2;
  } else {
    small = val_1;
  }
  int scaleSmall = 0;
  int check_tmp = OK;
  s21_decimal tmp1 = {{0, 0, 0, 0}};
  while ((get_scale(*val_1) != get_scale(*val_2)) && check_tmp == OK &&
         get_scale(*small) <= 28) {
    // повышение скейла
    scaleSmall = get_scale(*small);
    tmp1 = *small;
    // умножение на 10
    for (int i = 0; i < 9 && check_tmp == OK; i++) {
      check_tmp = bit_addition(small, &tmp1, small, OK);
    }
    if (check_tmp == OK) {
      set_scale(small, scaleSmall + 1);
    }
  }
  return check_tmp;
}

s21_decimal create_decimal(unsigned int low_num, unsigned int mid_num,
                           unsigned int high_num, unsigned int scale_info) {
  s21_decimal d;
  init_decimal(&d);
  d.bits[low] = low_num;
  d.bits[mid] = mid_num;
  d.bits[high] = high_num;
  d.bits[scale] = scale_info;
  return d;
}

void copy_bits(s21_decimal src, s21_decimal *dest) {
  dest->bits[low] = src.bits[low];
  dest->bits[mid] = src.bits[mid];
  dest->bits[high] = src.bits[high];
}

int move_left(s21_decimal *val, int move_value) {
  int lastbit = last_bit(*val);
  int err = OK;
  if (lastbit + move_value > 95) {
    err = TOO_BIG;
  } else {
    for (int i = 0; i < move_value; i++) {
      int val_31bit = get_bit(*val, 31);
      int val_63bit = get_bit(*val, 63);
      val->bits[low] <<= 1;
      val->bits[mid] <<= 1;
      val->bits[high] <<= 1;
      if (val_31bit) {
        set_bit(val, 32, 1);
      }
      if (val_63bit) {
        set_bit(val, 64, 1);
      }
    }
  }
  return err;
}

int zero_check(s21_decimal val_1, s21_decimal val_2) {
  int is_zero = 1;
  if (!val_1.bits[low] && !val_2.bits[low] && !val_1.bits[mid] &&
      !val_2.bits[mid] && !val_1.bits[high] && !val_2.bits[high]) {
    is_zero = 0;
  }
  return is_zero;
}

int is_negative(s21_decimal *val_1, s21_decimal *val_2) {
  int result = 0;
  if (get_sign(*val_1) == 0 && get_sign(*val_2)) {
    result = 1;
  }
  if (get_sign(*val_1) && get_sign(*val_2) == 0) {
    result = -1;
  }
  return result;
}

void div_only_bits(s21_decimal value_1, s21_decimal value_2,
                   s21_decimal *div_remainder, s21_decimal *result) {
  int tmp = 1;
  int check = 1;
  int scale_val_1 = get_scale(value_1);
  set_scale(&value_1, 0);
  set_scale(&value_2, 0);
  int sign_val_1 = get_sign(value_1);
  set_sign(&value_1, 0);
  set_sign(&value_2, 0);
  if (s21_is_greater_or_equal(value_1, value_2)) {
    s21_decimal sum = {{0, 0, 0, 0}};
    s21_decimal t_val = {{0, 0, 0, 0}};
    s21_decimal t_val_sup = {{0, 0, 0, 0}};
    s21_decimal dif = {{0, 0, 0, 0}};
    while (check) {
      t_val_sup = value_2;
      int i = 0;
      while (tmp) {
        t_val = value_2;
        move_left(&t_val, i);
        if (s21_is_greater(t_val, value_1)) {
          tmp = 0;
          move_left(&t_val_sup, i - 1);
          t_val = t_val_sup;
          s21_decimal one = {{1, 0, 0, 0}};
          move_left(&one, i - 1);
          s21_decimal sum_sup = one;
          s21_add(sum, sum_sup, &sum);
        }
        i++;
      }
      tmp = 1;
      s21_sub(value_1, t_val, &dif);
      if (s21_is_greater_or_equal(dif, value_2)) {
        value_1 = dif;
      } else {
        check = 0;
        *div_remainder = dif;
        *result = sum;
      }
    }
    set_scale(result, scale_val_1);
    set_sign(result, sign_val_1);
  } else {
    init_decimal(result);
    *div_remainder = value_1;
  }
}

int bit_addition(s21_decimal *val_1, s21_decimal *val_2, s21_decimal *result,
                 int code) {
  int err = OK;
  int one_in_head = 0;
  for (int i = 0; i < 96; i++) {
    int bit_val_1 = get_bit(*val_1, i);
    int bit_val_2 = get_bit(*val_2, i);
    if (bit_val_1 == 0 && bit_val_2 == 0) {
      if (one_in_head) {
        set_bit(result, i, 1);
        one_in_head = 0;
      } else {
        set_bit(result, i, 0);
      }
    } else if (bit_val_1 != bit_val_2) {
      if (one_in_head) {
        set_bit(result, i, 0);
        one_in_head = 1;
      } else {
        set_bit(result, i, 1);
      }
    } else if (bit_val_1 && bit_val_2) {
      if (one_in_head) {
        set_bit(result, i, 1);
        one_in_head = 1;
      } else {
        set_bit(result, i, 0);
        one_in_head = 1;
      }
    }
    if (i == 95 && one_in_head == 1 && code != EXTRA_CODE) {
      err = TOO_BIG;
    }
  }
  return err;
}

void float_to_bin(float num, s21_decimal *result) {
  int i = 0;
  while (num > 0) {
    set_bit(result, i, (long long int)num % 2);
    i++;
    num /= 2;
  }
}
