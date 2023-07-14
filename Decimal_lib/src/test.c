#include <check.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "checking/checking.h"
#include "s21_decimal.h"
#include "utils/s21_utils.h"

START_TEST(add) {
  s21_decimal dec1 = {{666, 0, 0, 0}};
  s21_decimal dec2 = {{9723, 0, 0, 0}};
  s21_decimal true_ans = {{10389, 0, 0, 0}};
  s21_decimal ans;
  ck_assert_int_eq(0, s21_add(dec1, dec2, &ans));
  ck_assert_int_eq(1, s21_is_equal(true_ans, ans));

  dec1 = create_decimal(34, 34, 34, 0);
  dec2 = create_decimal(66, 66, 66, 0);
  true_ans = create_decimal(100, 100, 100, 0);
  ck_assert_int_eq(0, s21_add(dec1, dec2, &ans));
  ck_assert_int_eq(1, s21_is_equal(true_ans, ans));

  dec1 = create_decimal(34, 34, 34, 0);
  dec2 = create_decimal(34, 34, 34, MINUS_SIGN);
  true_ans = create_decimal(0, 0, 0, 0);
  ck_assert_int_eq(0, s21_add(dec1, dec2, &ans));
  ck_assert_int_eq(1, s21_is_equal(true_ans, ans));

  dec1 = create_decimal(0, 0, 0, 0);
  dec2 = create_decimal(0, 0, 0, MINUS_SIGN);
  true_ans = create_decimal(0, 0, 0, 0);
  ck_assert_int_eq(0, s21_add(dec1, dec2, &ans));
  ck_assert_int_eq(1, s21_is_equal(true_ans, ans));

  dec1 = create_decimal(34, 34, 34, 0);
  dec2 = create_decimal(34, 34, 34, MINUS_SIGN);
  true_ans = create_decimal(0, 0, 0, 0);
  ck_assert_int_eq(0, s21_add(dec2, dec1, &ans));
  ck_assert_int_eq(1, s21_is_equal(true_ans, ans));

  dec1 = create_decimal(34, 34, 34, 0);
  dec2 = create_decimal(0, 0, 0, 0);
  true_ans = create_decimal(34, 34, 34, 0);
  ck_assert_int_eq(0, s21_add(dec1, dec2, &ans));
  ck_assert_int_eq(1, s21_is_equal(true_ans, ans));

  dec1 = create_decimal(34, 34, 34, MINUS_SIGN);
  dec2 = create_decimal(66, 66, 66, MINUS_SIGN);
  true_ans = create_decimal(100, 100, 100, MINUS_SIGN);
  ck_assert_int_eq(0, s21_add(dec1, dec2, &ans));
  ck_assert_int_eq(1, s21_is_equal(true_ans, ans));

  dec1 = create_decimal(34, 34, 34, MINUS_SIGN);
  dec2 = create_decimal(66, 66, 66, 0);
  true_ans = create_decimal(32, 32, 32, 0);
  ck_assert_int_eq(0, s21_add(dec1, dec2, &ans));
  ck_assert_int_eq(1, s21_is_equal(true_ans, ans));

  dec1 = create_decimal(34, 34, 34, 0);
  dec2 = create_decimal(66, 66, 66, MINUS_SIGN);
  true_ans = create_decimal(32, 32, 32, MINUS_SIGN);
  ck_assert_int_eq(0, s21_add(dec1, dec2, &ans));
  ck_assert_int_eq(1, s21_is_equal(true_ans, ans));

  dec1 = create_decimal(34, 34, 34, 0);
  dec2 = create_decimal(66, 66, 66, MINUS_SIGN);
  true_ans = create_decimal(32, 32, 32, MINUS_SIGN);
  ck_assert_int_eq(0, s21_add(dec2, dec1, &ans));
  ck_assert_int_eq(1, s21_is_equal(true_ans, ans));

  // НЕ целые числа

  dec1 = create_decimal(1, 0, 0, 0);
  dec2 = create_decimal(1002, 0, 0, DEFAULT_BASE * 3);
  true_ans = create_decimal(2002, 0, 0, DEFAULT_BASE * 3);
  ck_assert_int_eq(0, s21_add(dec1, dec2, &ans));
  ck_assert_int_eq(1, s21_is_equal(true_ans, ans));

  dec1 = create_decimal(613478421, 0, 0, DEFAULT_BASE * 3);
  dec2 = create_decimal(2056771366, 613, 0, DEFAULT_BASE * 6);
  true_ans = create_decimal(1354869038, 756, 0, DEFAULT_BASE * 6);
  ck_assert_int_eq(0, s21_add(dec1, dec2, &ans));
  ck_assert_int_eq(1, s21_is_equal(true_ans, ans));

  dec1 = create_decimal(5450, 0, 0, MINUS_SIGN);
  dec2 = create_decimal(550000, 0, 0, MINUS_SIGN + DEFAULT_BASE * 3);
  true_ans = create_decimal(6000000, 0, 0, MINUS_SIGN + DEFAULT_BASE * 3);
  ck_assert_int_eq(0, s21_add(dec1, dec2, &ans));
  ck_assert_int_eq(1, s21_is_equal(true_ans, ans));

  // Большие значения

  dec1 = create_decimal(4238529012, 4294967295, 0, 0);
  dec2 = create_decimal(56438283, 4294967295, 4294967294, 0);
  true_ans = create_decimal(4294967295, 4294967294, 4294967295, 0);
  ck_assert_int_eq(0, s21_add(dec1, dec2, &ans));
  ck_assert_int_eq(1, s21_is_equal(true_ans, ans));

  dec1 = create_decimal(4567567, 0, 0, DEFAULT_BASE * 3);
  dec2 = create_decimal(433, 0, 0, DEFAULT_BASE * 3);
  true_ans = create_decimal(4568, 0, 0, 0);
  ck_assert_int_eq(0, s21_add(dec1, dec2, &ans));
  ck_assert_int_eq(1, s21_is_equal(true_ans, ans));

  dec1 = create_decimal(4294967294, 4294967295, 4294967295, 0);
  dec2 = create_decimal(1, 0, 0, DEFAULT_BASE * 28);
  true_ans = create_decimal(4294967294, 4294967295, 4294967295, 0);
  ck_assert_int_eq(0, s21_add(dec1, dec2, &ans));
  ck_assert_int_eq(1, s21_is_equal(true_ans, ans));

  // Ошибки

  dec1 = create_decimal(4238529012, 4294967295, 4294967295, 0);
  dec2 = create_decimal(56438283, 4294967295, 4294967294, 0);
  true_ans = create_decimal(0, 0, 0, 0);
  ck_assert_int_eq(1, s21_add(dec1, dec2, &ans));
  ck_assert_int_eq(1, s21_is_equal(true_ans, ans));

  dec1 = create_decimal(4238529012, 4294967295, 4294967295, MINUS_SIGN);
  dec2 = create_decimal(56438283, 4294967295, 4294967294, MINUS_SIGN);
  true_ans = create_decimal(0, 0, 0, 0);
  ck_assert_int_eq(2, s21_add(dec1, dec2, &ans));
  ck_assert_int_eq(1, s21_is_equal(true_ans, ans));
}
END_TEST

START_TEST(sub) {
  s21_decimal dec1 = {{3234, 0, 0, 0}};
  s21_decimal dec2 = {{72, 0, 0, 0}};
  s21_decimal true_ans = {{3162, 0, 0, 0}};
  s21_decimal ans;
  ck_assert_int_eq(0, s21_sub(dec1, dec2, &ans));
  ck_assert_int_eq(1, s21_is_equal(true_ans, ans));

  dec1 = create_decimal(13, 0, 0, 0);
  dec2 = create_decimal(23, 0, 0, 0);
  true_ans = create_decimal(10, 0, 0, MINUS_SIGN);
  ck_assert_int_eq(0, s21_sub(dec1, dec2, &ans));
  ck_assert_int_eq(1, s21_is_equal(true_ans, ans));

  dec1 = create_decimal(532167, 0, 0, MINUS_SIGN);
  dec2 = create_decimal(123456, 0, 0, MINUS_SIGN);
  true_ans = create_decimal(408711, 0, 0, MINUS_SIGN);
  ck_assert_int_eq(0, s21_sub(dec1, dec2, &ans));
  ck_assert_int_eq(1, s21_is_equal(true_ans, ans));

  dec1 = create_decimal(61287, 0, 0, MINUS_SIGN);
  dec2 = create_decimal(234787, 0, 0, MINUS_SIGN);
  true_ans = create_decimal(173500, 0, 0, 0);
  ck_assert_int_eq(0, s21_sub(dec1, dec2, &ans));
  ck_assert_int_eq(1, s21_is_equal(true_ans, ans));

  dec1 = create_decimal(567, 45, 34676, 0);
  dec2 = create_decimal(23, 3, 66, 0);
  true_ans = create_decimal(544, 42, 34610, 0);
  ck_assert_int_eq(0, s21_sub(dec1, dec2, &ans));
  ck_assert_int_eq(1, s21_is_equal(true_ans, ans));

  dec1 = create_decimal(34, 34, 34, 0);
  dec2 = create_decimal(0, 0, 0, 0);
  true_ans = create_decimal(34, 34, 34, 0);
  ck_assert_int_eq(0, s21_sub(dec1, dec2, &ans));
  ck_assert_int_eq(1, s21_is_equal(true_ans, ans));

  dec1 = create_decimal(0, 0, 1, 0);
  dec2 = create_decimal(3467, 345346, 0, 0);
  true_ans = create_decimal(4294963829, 4294621949, 0, 0);
  ck_assert_int_eq(0, s21_sub(dec1, dec2, &ans));
  ck_assert_int_eq(1, s21_is_equal(true_ans, ans));

  dec1 = create_decimal(34, 34, 34, 0);
  dec2 = create_decimal(66, 66, 66, 0);
  true_ans = create_decimal(32, 32, 32, MINUS_SIGN);
  ck_assert_int_eq(0, s21_sub(dec1, dec2, &ans));
  ck_assert_int_eq(1, s21_is_equal(true_ans, ans));

  // НЕ целые числа

  dec1 = create_decimal(1, 0, 0, 0);
  dec2 = create_decimal(1002, 0, 0, DEFAULT_BASE * 3);
  true_ans = create_decimal(2, 0, 0, MINUS_SIGN + DEFAULT_BASE * 3);
  ck_assert_int_eq(0, s21_sub(dec1, dec2, &ans));
  ck_assert_int_eq(1, s21_is_equal(true_ans, ans));

  dec1 = create_decimal(532167, 0, 0, DEFAULT_BASE * 3);
  dec2 = create_decimal(1236, 0, 0, DEFAULT_BASE * 15);
  true_ans = create_decimal(607087404, 123904785, 0, DEFAULT_BASE * 15);
  ck_assert_int_eq(0, s21_sub(dec1, dec2, &ans));
  ck_assert_int_eq(1, s21_is_equal(true_ans, ans));

  dec1 = create_decimal(100, 0, 0, 0);
  dec2 = create_decimal(1, 0, 0, DEFAULT_BASE * 28);
  true_ans = create_decimal(
      0b00001111111111111111111111111111, 0b00111110001001010000001001100001,
      0b00100000010011111100111001011110, DEFAULT_BASE * 26);
  ck_assert_int_eq(0, s21_sub(dec1, dec2, &ans));
  ck_assert_int_eq(1, s21_is_equal(true_ans, ans));

  // Большие значения

  dec1 = create_decimal(4294967295, 4294967295, 4294967295, 0);
  dec2 = create_decimal(6, 0, 0, DEFAULT_BASE);
  true_ans = create_decimal(4294967294, 4294967295, 4294967295, 0);
  ck_assert_int_eq(0, s21_sub(dec1, dec2, &ans));
  ck_assert_int_eq(1, s21_is_equal(true_ans, ans));

  // Ошибки

  dec1 = create_decimal(4238529012, 4294967295, 4294967295, 0);
  dec2 = create_decimal(56438283, 4294967295, 4294967294, MINUS_SIGN);
  true_ans = create_decimal(0, 0, 0, 0);
  ck_assert_int_eq(1, s21_sub(dec1, dec2, &ans));
  ck_assert_int_eq(1, s21_is_equal(true_ans, ans));

  dec1 = create_decimal(4238529012, 4294967295, 4294967295, MINUS_SIGN);
  dec2 = create_decimal(56438283, 4294967295, 4294967294, 0);
  true_ans = create_decimal(0, 0, 0, 0);
  ck_assert_int_eq(2, s21_sub(dec1, dec2, &ans));
  ck_assert_int_eq(1, s21_is_equal(true_ans, ans));
}
END_TEST

START_TEST(mul) {
  s21_decimal dec1 = {{34563456, 0, 0, MINUS_SIGN}};
  s21_decimal dec2 = {{364287140, 0, 0, MINUS_SIGN}};
  s21_decimal true_ans = {{3783984640, 2931575, 0, 0}};
  s21_decimal ans;
  ck_assert_int_eq(0, s21_mul(dec1, dec2, &ans));
  ck_assert_int_eq(1, s21_is_equal(true_ans, ans));

  dec1 = create_decimal(1256, 0, 0, 0);
  dec2 = create_decimal(1000, 0, 0, 0);
  true_ans = create_decimal(1256000, 0, 0, 0);
  ck_assert_int_eq(0, s21_mul(dec1, dec2, &ans));
  ck_assert_int_eq(1, s21_is_equal(true_ans, ans));

  dec1 = create_decimal(1256, 0, 0, DEFAULT_BASE * 4);
  dec2 = create_decimal(673455, 0, 0, DEFAULT_BASE * 3);
  true_ans = create_decimal(84585948, 0, 0, DEFAULT_BASE * 6);
  ck_assert_int_eq(0, s21_mul(dec1, dec2, &ans));
  ck_assert_int_eq(1, s21_is_equal(true_ans, ans));

  // Переполнения

  dec1 = create_decimal(4294967295, 4294967295, 2147483647, 0);
  dec2 = create_decimal(2, 0, 0, 0);
  true_ans = create_decimal(4294967294, 4294967295, 4294967295, 0);
  ck_assert_int_eq(0, s21_mul(dec1, dec2, &ans));
  ck_assert_int_eq(1, s21_is_equal(true_ans, ans));

  dec1 = create_decimal(137900165, 69122625, 1, DEFAULT_BASE * 20);
  dec2 = create_decimal(46246921, 0, 0, DEFAULT_BASE * 15);
  true_ans = create_decimal(2090836749, 3002702997, 4, DEFAULT_BASE * 28);
  ck_assert_int_eq(0, s21_mul(dec1, dec2, &ans));
  ck_assert_int_eq(1, s21_is_equal(true_ans, ans));

  dec1 = create_decimal(0, 2, 0, DEFAULT_BASE * 20);
  dec2 = create_decimal(0, 2, 0, DEFAULT_BASE * 10);
  true_ans = create_decimal(3607772529, 171798691, 0, DEFAULT_BASE * 28);
  ck_assert_int_eq(0, s21_mul(dec1, dec2, &ans));
  ck_assert_int_eq(1, s21_is_equal(true_ans, ans));

  // Ошибки

  dec1 = create_decimal(0, 0, 4294967295, 0);
  dec2 = create_decimal(2, 0, 0, 0);
  true_ans = create_decimal(0, 0, 0, 0);
  ck_assert_int_eq(1, s21_mul(dec1, dec2, &ans));
  ck_assert_int_eq(1, s21_is_equal(true_ans, ans));

  dec1 = create_decimal(0, 0, 4294967295, MINUS_SIGN);
  dec2 = create_decimal(2, 0, 0, 0);
  true_ans = create_decimal(0, 0, 0, 0);
  ck_assert_int_eq(2, s21_mul(dec1, dec2, &ans));
  ck_assert_int_eq(1, s21_is_equal(true_ans, ans));
}
END_TEST

START_TEST(delenie) {
  s21_decimal dec1 = {{65536, 0, 0, 0}};
  s21_decimal dec2 = {{2, 0, 0, 0}};
  s21_decimal true_ans = {{32768, 0, 0, 0}};
  s21_decimal ans;
  ck_assert_int_eq(0, s21_div(dec1, dec2, &ans));
  ck_assert_int_eq(1, s21_is_equal(true_ans, ans));

  dec1 = create_decimal(9, 0, 0, DEFAULT_BASE);
  dec2 = create_decimal(6, 0, 0, DEFAULT_BASE);
  true_ans = create_decimal(15, 0, 0, DEFAULT_BASE);
  ck_assert_int_eq(0, s21_div(dec1, dec2, &ans));
  ck_assert_int_eq(1, s21_is_equal(true_ans, ans));

  dec1 = create_decimal(9, 0, 0, MINUS_SIGN + DEFAULT_BASE);
  dec2 = create_decimal(6, 0, 0, DEFAULT_BASE);
  true_ans = create_decimal(15, 0, 0, MINUS_SIGN + DEFAULT_BASE);
  ck_assert_int_eq(0, s21_div(dec1, dec2, &ans));
  ck_assert_int_eq(1, s21_is_equal(true_ans, ans));

  dec1 = create_decimal(14, 0, 0, DEFAULT_BASE);
  dec2 = create_decimal(9, 0, 0, DEFAULT_BASE);
  true_ans =
      create_decimal(3280877796, 1621842184, 843268356, DEFAULT_BASE * 28);
  ck_assert_int_eq(0, s21_div(dec1, dec2, &ans));
  ck_assert_int_eq(1, s21_is_equal(true_ans, ans));

  dec1 = create_decimal(0, 0, 0, DEFAULT_BASE);
  dec2 = create_decimal(345, 0, 453, 0);
  true_ans = create_decimal(0, 0, 0, 0);
  ck_assert_int_eq(0, s21_div(dec1, dec2, &ans));
  ck_assert_int_eq(1, s21_is_equal(true_ans, ans));

  dec1 = create_decimal(4294967295, 4294967295, 4294967295, 0);
  dec2 = create_decimal(4294967295, 4294967295, 4294967295, 0);
  true_ans = create_decimal(1, 0, 0, 0);
  ck_assert_int_eq(0, s21_div(dec1, dec2, &ans));
  ck_assert_int_eq(1, s21_is_equal(true_ans, ans));

  dec1 = create_decimal(1188260494, 2547073670, 423, DEFAULT_BASE * 12);
  dec2 = create_decimal(1658453339, 2896612263, 67010, DEFAULT_BASE * 12);
  true_ans = create_decimal(2998137797, 614149364, 3426771, DEFAULT_BASE * 28);
  ck_assert_int_eq(0, s21_div(dec1, dec2, &ans));
  ck_assert_int_eq(1, s21_is_equal(true_ans, ans));

  // Ошибки

  dec1 = create_decimal(2814903, 0, 0, DEFAULT_BASE);
  dec2 = create_decimal(0, 0, 0, 0);
  true_ans = create_decimal(0, 0, 0, 0);
  ck_assert_int_eq(3, s21_div(dec1, dec2, &ans));
  ck_assert_int_eq(1, s21_is_equal(true_ans, ans));

  dec1 = create_decimal(4294967294, 4294967295, 4294967295, 0);
  dec2 = create_decimal(1, 0, 0, DEFAULT_BASE * 18);
  true_ans = create_decimal(0, 0, 0, 0);
  ck_assert_int_eq(1, s21_div(dec1, dec2, &ans));
  ck_assert_int_eq(1, s21_is_equal(true_ans, ans));

  dec1 = create_decimal(1, 0, 0, DEFAULT_BASE * 18);
  dec2 = create_decimal(4294967294, 4294967295, 4294967295, 0);
  true_ans = create_decimal(0, 0, 0, 0);
  ck_assert_int_eq(2, s21_div(dec1, dec2, &ans));
  ck_assert_int_eq(1, s21_is_equal(true_ans, ans));
}
END_TEST

START_TEST(mod) {
  s21_decimal dec1 = {{10, 0, 0, 0}};
  s21_decimal dec2 = {{4, 0, 0, 0}};
  s21_decimal true_ans = {{2, 0, 0, 0}};
  s21_decimal ans;
  ck_assert_int_eq(0, s21_mod(dec1, dec2, &ans));
  ck_assert_int_eq(1, s21_is_equal(true_ans, ans));

  dec1 = create_decimal(4036421147, 382, 0, DEFAULT_BASE * 4);
  dec2 = create_decimal(2945631615, 658, 0, MINUS_SIGN + DEFAULT_BASE * 6);
  true_ans = create_decimal(867247046, 90, 0, DEFAULT_BASE * 6);
  ck_assert_int_eq(0, s21_mod(dec1, dec2, &ans));
  ck_assert_int_eq(1, s21_is_equal(true_ans, ans));

  dec1 =
      create_decimal(2891627761, 3474792174, 8, MINUS_SIGN + DEFAULT_BASE * 5);
  dec2 = create_decimal(1870970698, 1989793225, 0, DEFAULT_BASE * 11);
  true_ans =
      create_decimal(1717205652, 1236686312, 0, MINUS_SIGN + DEFAULT_BASE * 11);
  ck_assert_int_eq(0, s21_mod(dec1, dec2, &ans));
  ck_assert_int_eq(1, s21_is_equal(true_ans, ans));

  dec1 = create_decimal(4294967295, 4294967295, 4294967295, 0);
  dec2 = create_decimal(1, 0, 0, DEFAULT_BASE * 3);
  true_ans = create_decimal(0, 0, 0, 0);
  ck_assert_int_eq(0, s21_mod(dec1, dec2, &ans));
  ck_assert_int_eq(1, s21_is_equal(true_ans, ans));

  dec1 = create_decimal(0, 0, 0, DEFAULT_BASE * 3);
  dec2 = create_decimal(217948, 0, 0, DEFAULT_BASE * 3);
  true_ans = create_decimal(0, 0, 0, 0);
  ck_assert_int_eq(0, s21_mod(dec1, dec2, &ans));
  ck_assert_int_eq(1, s21_is_equal(true_ans, ans));

  // Ошибки

  dec1 = create_decimal(4294967295, 4294967295, 4294967295, 0);
  dec2 = create_decimal(0, 0, 0, DEFAULT_BASE * 3);
  true_ans = create_decimal(0, 0, 0, 0);
  ck_assert_int_eq(3, s21_mod(dec1, dec2, &ans));
  ck_assert_int_eq(1, s21_is_equal(true_ans, ans));
}
END_TEST

START_TEST(is_equal) {
  s21_decimal dec1 = {{2938, 365, 2324, MINUS_SIGN + DEFAULT_BASE}};
  s21_decimal dec2 = {{2938, 365, 2324, MINUS_SIGN + DEFAULT_BASE}};
  ck_assert_int_eq(1, s21_is_equal(dec1, dec2));

  dec1 = create_decimal(2938, 365, 2324, DEFAULT_BASE * 25);
  dec2 = create_decimal(2938, 365, 2324, DEFAULT_BASE * 25);
  ck_assert_int_eq(1, s21_is_equal(dec1, dec2));

  dec1 = create_decimal(0, 0, 0, 0);
  dec2 = create_decimal(0, 0, 0, MINUS_SIGN);
  ck_assert_int_eq(1, s21_is_equal(dec1, dec2));

  dec1 = create_decimal(7, 0, 7, 0);
  dec2 = create_decimal(0, 0, 0, 0);
  ck_assert_int_eq(0, s21_is_equal(dec1, dec2));

  dec1 = create_decimal(34, 0, 0, 0);
  dec2 = create_decimal(34, 0, 0, MINUS_SIGN);
  ck_assert_int_eq(0, s21_is_equal(dec1, dec2));

  dec1 = create_decimal(35000, 0, 0, DEFAULT_BASE * 3);
  dec2 = create_decimal(35, 0, 0, 0);
  ck_assert_int_eq(1, s21_is_equal(dec1, dec2));

  dec1 = create_decimal(3500, 2200, 0, DEFAULT_BASE * 2);
  dec2 = create_decimal(35, 22, 0, 0);
  ck_assert_int_eq(1, s21_is_equal(dec1, dec2));
}
END_TEST

START_TEST(is_not_equal) {
  s21_decimal dec1 = {{2938, 365, 2324, MINUS_SIGN + DEFAULT_BASE}};
  s21_decimal dec2 = {{2938, 365, 2324, MINUS_SIGN + DEFAULT_BASE}};
  ck_assert_int_eq(0, s21_is_not_equal(dec1, dec2));

  dec1 = create_decimal(2938, 365, 2324, DEFAULT_BASE * 25);
  dec2 = create_decimal(2938, 365, 2324, DEFAULT_BASE * 25);
  ck_assert_int_eq(0, s21_is_not_equal(dec1, dec2));

  dec1 = create_decimal(0, 0, 0, 0);
  dec2 = create_decimal(0, 0, 0, MINUS_SIGN);
  ck_assert_int_eq(0, s21_is_not_equal(dec1, dec2));

  dec1 = create_decimal(7, 0, 7, 0);
  dec2 = create_decimal(0, 0, 0, 0);
  ck_assert_int_eq(1, s21_is_not_equal(dec1, dec2));

  dec1 = create_decimal(34, 0, 0, 0);
  dec2 = create_decimal(34, 0, 0, MINUS_SIGN);
  ck_assert_int_eq(1, s21_is_not_equal(dec1, dec2));

  dec1 = create_decimal(35000, 0, 0, DEFAULT_BASE * 3);
  dec2 = create_decimal(35, 0, 0, 0);
  ck_assert_int_eq(0, s21_is_not_equal(dec1, dec2));
}
END_TEST

START_TEST(is_greater) {
  s21_decimal dec1 = {{11, 1, 1, 0}};
  s21_decimal dec2 = {{1, 1, 1, 0}};
  ck_assert_int_eq(1, s21_is_greater(dec1, dec2));

  dec1 = create_decimal(0, 0, 100, 0);
  dec2 = create_decimal(345345, 4586548, 0, 0);
  ck_assert_int_eq(1, s21_is_greater(dec1, dec2));

  dec1 = create_decimal(0, 0, 0, 0);
  dec2 = create_decimal(0, 0, 0, 0);
  ck_assert_int_eq(0, s21_is_greater(dec1, dec2));

  dec1 = create_decimal(0, 0, 0, MINUS_SIGN);
  dec2 = create_decimal(0, 0, 0, 0);
  ck_assert_int_eq(0, s21_is_greater(dec1, dec2));

  dec1 = create_decimal(9, 45, 0, 0);
  dec2 = create_decimal(5, 0, 345, MINUS_SIGN);
  ck_assert_int_eq(1, s21_is_greater(dec1, dec2));

  dec1 = create_decimal(9, 45, 0, 0);
  dec2 = create_decimal(5, 0, 345, 0);
  ck_assert_int_eq(0, s21_is_greater(dec1, dec2));

  dec1 = create_decimal(78, 0, 0, MINUS_SIGN);
  dec2 = create_decimal(0, 0, 0, 0);
  ck_assert_int_eq(0, s21_is_greater(dec1, dec2));

  dec1 = create_decimal(78000, 0, 0, DEFAULT_BASE * 3);
  dec2 = create_decimal(780, 0, 0, 0);
  ck_assert_int_eq(0, s21_is_greater(dec1, dec2));
}
END_TEST

START_TEST(is_greater_or_eq) {
  s21_decimal dec1 = {{11, 1, 1, 0}};
  s21_decimal dec2 = {{1, 1, 1, 0}};
  ck_assert_int_eq(1, s21_is_greater_or_equal(dec1, dec2));

  dec1 = create_decimal(0, 0, 100, 0);
  dec2 = create_decimal(345345, 4586548, 0, 0);
  ck_assert_int_eq(1, s21_is_greater_or_equal(dec1, dec2));

  dec1 = create_decimal(0, 0, 0, 0);
  dec2 = create_decimal(0, 0, 0, 0);
  ck_assert_int_eq(1, s21_is_greater_or_equal(dec1, dec2));

  dec1 = create_decimal(0, 0, 0, MINUS_SIGN);
  dec2 = create_decimal(0, 0, 0, 0);
  ck_assert_int_eq(1, s21_is_greater_or_equal(dec1, dec2));

  dec1 = create_decimal(9, 45, 0, 0);
  dec2 = create_decimal(5, 0, 345, 0);
  ck_assert_int_eq(0, s21_is_greater_or_equal(dec1, dec2));

  dec1 = create_decimal(78, 0, 0, MINUS_SIGN);
  dec2 = create_decimal(0, 0, 0, 0);
  ck_assert_int_eq(0, s21_is_greater_or_equal(dec1, dec2));

  dec1 = create_decimal(78000, 0, 0, DEFAULT_BASE * 3);
  dec2 = create_decimal(780, 0, 0, 0);
  ck_assert_int_eq(0, s21_is_greater_or_equal(dec1, dec2));

  dec1 = create_decimal(78000, 0, 0, DEFAULT_BASE * 4);
  dec2 = create_decimal(78000, 0, 0, DEFAULT_BASE * 4);
  ck_assert_int_eq(1, s21_is_greater_or_equal(dec1, dec2));

  dec1 = create_decimal(3500, 0, 0, DEFAULT_BASE * 3);
  dec2 = create_decimal(35, 0, 0, 0);
  ck_assert_int_eq(0, s21_is_greater_or_equal(dec1, dec2));
}
END_TEST

START_TEST(is_less) {
  s21_decimal dec1 = {{11, 1, 1, 0}};
  s21_decimal dec2 = {{1, 1, 1, 0}};
  ck_assert_int_eq(0, s21_is_less(dec1, dec2));

  dec1 = create_decimal(0, 0, 100, 0);
  dec2 = create_decimal(345345, 4586548, 0, 0);
  ck_assert_int_eq(0, s21_is_less(dec1, dec2));

  dec1 = create_decimal(0, 0, 0, 0);
  dec2 = create_decimal(0, 0, 0, 0);
  ck_assert_int_eq(0, s21_is_less(dec1, dec2));

  dec1 = create_decimal(0, 0, 0, MINUS_SIGN);
  dec2 = create_decimal(0, 0, 0, 0);
  ck_assert_int_eq(0, s21_is_less(dec1, dec2));

  dec1 = create_decimal(9, 45, 0, 0);
  dec2 = create_decimal(5, 0, 345, 0);
  ck_assert_int_eq(1, s21_is_less(dec1, dec2));

  dec1 = create_decimal(78, 0, 0, MINUS_SIGN);
  dec2 = create_decimal(0, 0, 0, 0);
  ck_assert_int_eq(1, s21_is_less(dec1, dec2));

  dec1 = create_decimal(78000, 0, 0, DEFAULT_BASE * 4);
  dec2 = create_decimal(780, 0, 0, 0);
  ck_assert_int_eq(1, s21_is_less(dec1, dec2));
}
END_TEST

START_TEST(is_less_or_eq) {
  s21_decimal dec1 = {{11, 1, 1, 0}};
  s21_decimal dec2 = {{1, 1, 1, 0}};
  ck_assert_int_eq(0, s21_is_less_or_equal(dec1, dec2));

  dec1 = create_decimal(0, 0, 100, 0);
  dec2 = create_decimal(345345, 4586548, 0, 0);
  ck_assert_int_eq(0, s21_is_less_or_equal(dec1, dec2));

  dec1 = create_decimal(0, 0, 0, 0);
  dec2 = create_decimal(0, 0, 0, 0);
  ck_assert_int_eq(1, s21_is_less_or_equal(dec1, dec2));

  dec1 = create_decimal(0, 0, 0, MINUS_SIGN);
  dec2 = create_decimal(0, 0, 0, 0);
  ck_assert_int_eq(1, s21_is_less_or_equal(dec1, dec2));

  dec1 = create_decimal(9, 45, 0, 0);
  dec2 = create_decimal(5, 0, 345, 0);
  ck_assert_int_eq(1, s21_is_less_or_equal(dec1, dec2));

  dec1 = create_decimal(78, 0, 0, MINUS_SIGN);
  dec2 = create_decimal(0, 0, 0, 0);
  ck_assert_int_eq(1, s21_is_less_or_equal(dec1, dec2));

  dec1 = create_decimal(78000, 0, 0, DEFAULT_BASE * 4);
  dec2 = create_decimal(780, 0, 0, 0);
  ck_assert_int_eq(1, s21_is_less_or_equal(dec1, dec2));

  dec1 = create_decimal(78000, 0, 0, DEFAULT_BASE * 4);
  dec2 = create_decimal(78000, 0, 0, DEFAULT_BASE * 4);
  ck_assert_int_eq(1, s21_is_less_or_equal(dec1, dec2));

  dec1 = create_decimal(35000, 0, 0, DEFAULT_BASE * 3);
  dec2 = create_decimal(35, 0, 0, 0);
  ck_assert_int_eq(1, s21_is_less_or_equal(dec1, dec2));
}
END_TEST

START_TEST(from_decimal_to_int) {
  s21_decimal dec = {{0, 0, 0, 0}};
  int ans;
  int true_ans = 0;
  ck_assert_int_eq(0, s21_from_decimal_to_int(dec, &ans));
  ck_assert_int_eq(ans, true_ans);

  dec = create_decimal(9384, 0, 0, MINUS_SIGN);
  true_ans = -9384;
  ck_assert_int_eq(0, s21_from_decimal_to_int(dec, &ans));
  ck_assert_int_eq(ans, true_ans);

  dec = create_decimal(INT32_MAX, 0, 0, 0);
  true_ans = INT32_MAX;
  ck_assert_int_eq(0, s21_from_decimal_to_int(dec, &ans));
  ck_assert_int_eq(ans, true_ans);

  // С ДРОБНОЙ ЧАСТЬЮ
  dec = create_decimal(903, 0, 0, DEFAULT_BASE * 2);
  true_ans = 9;
  ck_assert_int_eq(0, s21_from_decimal_to_int(dec, &ans));
  ck_assert_int_eq(ans, true_ans);

  dec = create_decimal(567349, 4787346, 34345, DEFAULT_BASE * 21);
  true_ans = 633;
  ck_assert_int_eq(0, s21_from_decimal_to_int(dec, &ans));
  ck_assert_int_eq(ans, true_ans);

  dec = create_decimal(9, 0, 0, DEFAULT_BASE);
  true_ans = 0;
  ck_assert_int_eq(0, s21_from_decimal_to_int(dec, &ans));
  ck_assert_int_eq(ans, true_ans);

  // ОШИБКИ КОНВЕРТАЦИИ
  dec = create_decimal(UINT32_MAX, 0, 0, 0);
  ck_assert_int_eq(1, s21_from_decimal_to_int(dec, &ans));

  dec = create_decimal(45, 565675, 345, 0);
  ck_assert_int_eq(1, s21_from_decimal_to_int(dec, &ans));
}
END_TEST

START_TEST(from_int_to_decimal) {
  int x = -8374;
  s21_decimal ans;
  s21_decimal true_ans = {{8374, 0, 0, MINUS_SIGN}};

  ck_assert_int_eq(0, s21_from_int_to_decimal(x, &ans));
  ck_assert_int_eq(1, s21_is_equal(ans, true_ans));

  x = 0;
  true_ans = create_decimal(0, 0, 0, 0);

  ck_assert_int_eq(0, s21_from_int_to_decimal(x, &ans));
  ck_assert_int_eq(1, s21_is_equal(ans, true_ans));

  x = INT32_MAX;
  true_ans = create_decimal(INT32_MAX, 0, 0, 0);

  ck_assert_int_eq(0, s21_from_int_to_decimal(x, &ans));
  ck_assert_int_eq(1, s21_is_equal(ans, true_ans));

  x = -INT32_MAX;
  true_ans = create_decimal(INT32_MAX, 0, 0, MINUS_SIGN);

  ck_assert_int_eq(0, s21_from_int_to_decimal(x, &ans));
  ck_assert_int_eq(1, s21_is_equal(ans, true_ans));
}
END_TEST

START_TEST(from_decimal_to_float) {
  s21_decimal dec = {{0, 0, 0, 0}};
  float ans;
  float true_ans = 0;
  ck_assert_int_eq(0, s21_from_decimal_to_float(dec, &ans));
  ck_assert_float_eq(ans, true_ans);

  dec = create_decimal(9384, 0, 0, MINUS_SIGN);
  true_ans = -9384;
  ck_assert_int_eq(0, s21_from_decimal_to_float(dec, &ans));
  ck_assert_float_eq(ans, true_ans);

  dec = create_decimal(9384, 5643, 39846, 0);
  true_ans = 735028964385267294151848.0f;
  ck_assert_int_eq(0, s21_from_decimal_to_float(dec, &ans));
  ck_assert_float_eq(ans, true_ans);

  dec = create_decimal(0, 453, 0, MINUS_SIGN);
  true_ans = -1945620185088.0f;
  ck_assert_int_eq(0, s21_from_decimal_to_float(dec, &ans));
  ck_assert_float_eq(ans, true_ans);

  dec = create_decimal(4, 0, 0, DEFAULT_BASE * 7);
  true_ans = 0.0000004f;
  ck_assert_int_eq(0, s21_from_decimal_to_float(dec, &ans));
  ck_assert_float_eq_tol(ans, true_ans, 1e-7);

  dec = create_decimal(347684574, 347684574, 32988346, DEFAULT_BASE * 27);
  true_ans = 0.608527577570274067214843614f;
  ck_assert_int_eq(0, s21_from_decimal_to_float(dec, &ans));
  ck_assert_float_eq_tol(ans, true_ans, 1e-7);

  dec = create_decimal(347684574, 347684574, 32988346, DEFAULT_BASE * 22);
  true_ans = 60852.7577570274067214843614f;
  ck_assert_int_eq(0, s21_from_decimal_to_float(dec, &ans));
  ck_assert_float_eq_tol(ans, true_ans, 1e-7);

  dec = create_decimal(UINT32_MAX, UINT32_MAX, UINT32_MAX, 0);
  true_ans = 79228162514264337593543950335.0f;
  ck_assert_int_eq(0, s21_from_decimal_to_float(dec, &ans));
  ck_assert_float_eq(ans, true_ans);
}
END_TEST

START_TEST(from_float_to_decimal) {
  s21_decimal ans;
  float num = 9.34f;
  s21_decimal true_ans = {{934, 0, 0, DEFAULT_BASE * 2}};
  ck_assert_int_eq(0, s21_from_float_to_decimal(num, &ans));
  ck_assert_int_eq(1, s21_is_equal(ans, true_ans));

  num = 0.0f;
  true_ans = create_decimal(0, 0, 0, 0);
  ck_assert_int_eq(0, s21_from_float_to_decimal(num, &ans));
  ck_assert_int_eq(1, s21_is_equal(ans, true_ans));

  num = 654.0f;
  true_ans = create_decimal(654, 0, 0, 0);
  ck_assert_int_eq(0, s21_from_float_to_decimal(num, &ans));
  ck_assert_int_eq(1, s21_is_equal(ans, true_ans));

  num = 65465523.0f;
  true_ans = create_decimal(65465520, 0, 0, 0);
  ck_assert_int_eq(0, s21_from_float_to_decimal(num, &ans));
  ck_assert_int_eq(1, s21_is_equal(ans, true_ans));

  num = -67.234f;
  true_ans = create_decimal(67234, 0, 0, MINUS_SIGN + DEFAULT_BASE * 3);
  ck_assert_int_eq(0, s21_from_float_to_decimal(num, &ans));
  ck_assert_int_eq(1, s21_is_equal(ans, true_ans));

  num = 0.233435344f;
  true_ans = create_decimal(2334353, 0, 0, DEFAULT_BASE * 7);
  ck_assert_int_eq(0, s21_from_float_to_decimal(num, &ans));
  ck_assert_int_eq(1, s21_is_equal(ans, true_ans));

  num = 0.00000000000000000000044f;
  true_ans = create_decimal(44, 0, 0, DEFAULT_BASE * 23);
  ck_assert_int_eq(0, s21_from_float_to_decimal(num, &ans));
  ck_assert_int_eq(1, s21_is_equal(ans, true_ans));

  num = 498763853453456465523.0f;
  true_ans = create_decimal(
      0b00000000000000000000000000000000, 0b00001001101111010000000000000000,
      0b00000000000000000000000000011011, 0b00000000000000000000000000000000);
  ck_assert_int_eq(0, s21_from_float_to_decimal(num, &ans));
  ck_assert_int_eq(1, s21_is_equal(ans, true_ans));

  num = 79228162514264337593543950335.0f;
  true_ans = create_decimal(
      0b00000000000000000000000000000000, 0b00000000000000000000000000000000,
      0b11111111111111111111111100000000, 0b00000000000000000000000000000000);
  ck_assert_int_eq(0, s21_from_float_to_decimal(num, &ans));
  ck_assert_int_eq(1, s21_is_equal(ans, true_ans));

  // ОШИБКИ КОНВЕРТАЦИИ

  num = 0.000000000000000000000000000000000000067;
  true_ans = create_decimal(0, 0, 0, 0);
  ck_assert_int_eq(1, s21_from_float_to_decimal(num, &ans));
  ck_assert_int_eq(1, s21_is_equal(ans, true_ans));

  num = INFINITY;
  true_ans = create_decimal(0, 0, 0, 0);
  ck_assert_int_eq(1, s21_from_float_to_decimal(num, &ans));
  ck_assert_int_eq(1, s21_is_equal(ans, true_ans));

  num = -INFINITY;
  true_ans = create_decimal(0, 0, 0, 0);
  ck_assert_int_eq(1, s21_from_float_to_decimal(num, &ans));
  ck_assert_int_eq(1, s21_is_equal(ans, true_ans));

  // num = 79228162514264337593543950777.0f;
  // true_ans = create_decimal(0, 0, 0, 0);
  // ck_assert_int_eq(1, s21_from_float_to_decimal(num, &ans));
  // ck_assert_int_eq(1, s21_is_equal(ans, true_ans));
}
END_TEST

START_TEST(floor21) {
  s21_decimal dec = {
      {178532910, 4081246736, 2119837296, MINUS_SIGN + DEFAULT_BASE * 27}};
  s21_decimal ans;
  s21_decimal true_ans = {{40, 0, 0, MINUS_SIGN}};
  ck_assert_int_eq(0, s21_floor(dec, &ans));
  ck_assert_int_eq(1, s21_is_equal(ans, true_ans));

  dec = create_decimal(598769227, 2903911868, 4948073, DEFAULT_BASE * 26);
  true_ans = create_decimal(0, 0, 0, 0);
  ck_assert_int_eq(0, s21_floor(dec, &ans));
  ck_assert_int_eq(1, s21_is_equal(ans, true_ans));

  dec = create_decimal(598769227, 2903911868, 4948073,
                       MINUS_SIGN + DEFAULT_BASE * 26);
  true_ans = create_decimal(1, 0, 0, MINUS_SIGN);
  ck_assert_int_eq(0, s21_floor(dec, &ans));
  ck_assert_int_eq(1, s21_is_equal(ans, true_ans));

  dec = create_decimal(915492088, 614240, 0, MINUS_SIGN + DEFAULT_BASE * 10);
  true_ans = create_decimal(263815, 0, 0, MINUS_SIGN);
  ck_assert_int_eq(0, s21_floor(dec, &ans));
  ck_assert_int_eq(1, s21_is_equal(ans, true_ans));

  dec = create_decimal(56, 56, 56, 0);
  true_ans = create_decimal(56, 56, 56, 0);
  ck_assert_int_eq(0, s21_floor(dec, &ans));
  ck_assert_int_eq(1, s21_is_equal(ans, true_ans));

  dec = create_decimal(784662527, 11641532, 0, DEFAULT_BASE * 17);
  true_ans = create_decimal(0, 0, 0, 0);
  ck_assert_int_eq(0, s21_floor(dec, &ans));
  ck_assert_int_eq(1, s21_is_equal(ans, true_ans));

  dec = create_decimal(5, 0, 0, DEFAULT_BASE);
  true_ans = create_decimal(0, 0, 0, 0);
  ck_assert_int_eq(0, s21_floor(dec, &ans));
  ck_assert_int_eq(1, s21_is_equal(ans, true_ans));
}
END_TEST

START_TEST(round21) {
  s21_decimal dec = {
      {178532910, 4081246736, 2119837296, MINUS_SIGN + DEFAULT_BASE * 27}};
  s21_decimal ans;
  s21_decimal true_ans = {{39, 0, 0, MINUS_SIGN}};
  ck_assert_int_eq(0, s21_round(dec, &ans));
  ck_assert_int_eq(1, s21_is_equal(ans, true_ans));

  dec = create_decimal(598769227, 2903911868, 4948073, DEFAULT_BASE * 26);
  true_ans = create_decimal(1, 0, 0, 0);
  ck_assert_int_eq(0, s21_round(dec, &ans));
  ck_assert_int_eq(1, s21_is_equal(ans, true_ans));

  dec = create_decimal(598769227, 2903911868, 4948073,
                       MINUS_SIGN + DEFAULT_BASE * 26);
  true_ans = create_decimal(1, 0, 0, MINUS_SIGN);
  ck_assert_int_eq(0, s21_round(dec, &ans));
  ck_assert_int_eq(1, s21_is_equal(ans, true_ans));

  dec = create_decimal(915492088, 614240, 0, MINUS_SIGN + DEFAULT_BASE * 10);
  true_ans = create_decimal(263814, 0, 0, MINUS_SIGN);
  ck_assert_int_eq(0, s21_round(dec, &ans));
  ck_assert_int_eq(1, s21_is_equal(ans, true_ans));

  dec = create_decimal(56, 56, 56, 0);
  true_ans = create_decimal(56, 56, 56, 0);
  ck_assert_int_eq(0, s21_round(dec, &ans));
  ck_assert_int_eq(1, s21_is_equal(ans, true_ans));

  dec = create_decimal(784662527, 11641532, 0, DEFAULT_BASE * 17);
  true_ans = create_decimal(0, 0, 0, 0);
  ck_assert_int_eq(0, s21_round(dec, &ans));
  ck_assert_int_eq(1, s21_is_equal(ans, true_ans));

  dec = create_decimal(5, 0, 0, DEFAULT_BASE);
  true_ans = create_decimal(1, 0, 0, 0);
  ck_assert_int_eq(0, s21_round(dec, &ans));
  ck_assert_int_eq(1, s21_is_equal(ans, true_ans));
}
END_TEST

START_TEST(truncate) {
  s21_decimal dec = {
      {178532910, 4081246736, 2119837296, MINUS_SIGN + DEFAULT_BASE * 27}};
  s21_decimal ans;
  s21_decimal true_ans = {{39, 0, 0, MINUS_SIGN}};
  ck_assert_int_eq(0, s21_truncate(dec, &ans));
  ck_assert_int_eq(1, s21_is_equal(ans, true_ans));

  dec = create_decimal(613478421, 0, 0, DEFAULT_BASE * 3);
  true_ans = create_decimal(613478, 0, 0, 0);
  ck_assert_int_eq(0, s21_truncate(dec, &ans));
  ck_assert_int_eq(1, s21_is_equal(ans, true_ans));

  dec = create_decimal(1000, 0, 0, DEFAULT_BASE * 3);
  true_ans = create_decimal(1, 0, 0, 0);
  ck_assert_int_eq(0, s21_truncate(dec, &ans));
  ck_assert_int_eq(1, s21_is_equal(ans, true_ans));

  dec = create_decimal(56, 56, 56, 0);
  true_ans = create_decimal(56, 56, 56, 0);
  ck_assert_int_eq(0, s21_truncate(dec, &ans));
  ck_assert_int_eq(1, s21_is_equal(ans, true_ans));

  dec = create_decimal(0, 0, 0, DEFAULT_BASE * 22);
  true_ans = create_decimal(0, 0, 0, 0);
  ck_assert_int_eq(0, s21_truncate(dec, &ans));
  ck_assert_int_eq(1, s21_is_equal(ans, true_ans));

  dec = create_decimal(45, 0, 0, DEFAULT_BASE * 22);
  true_ans = create_decimal(0, 0, 0, 0);
  ck_assert_int_eq(0, s21_truncate(dec, &ans));
  ck_assert_int_eq(1, s21_is_equal(ans, true_ans));
}
END_TEST

START_TEST(negate) {
  s21_decimal dec = {{2938, 365, 2324, MINUS_SIGN + DEFAULT_BASE}};
  s21_decimal ans;
  s21_decimal true_ans = {{2938, 365, 2324, DEFAULT_BASE}};
  ck_assert_int_eq(0, s21_negate(dec, &ans));
  ck_assert_int_eq(1, s21_is_equal(ans, true_ans));

  dec = create_decimal(2938, 365, 2324, DEFAULT_BASE);
  true_ans = create_decimal(2938, 365, 2324, MINUS_SIGN + DEFAULT_BASE);
  ck_assert_int_eq(0, s21_negate(dec, &ans));
  ck_assert_int_eq(1, s21_is_equal(ans, true_ans));

  dec = create_decimal(0, 0, 0, 0);
  true_ans = create_decimal(0, 0, 0, MINUS_SIGN);
  ck_assert_int_eq(0, s21_negate(dec, &ans));
  ck_assert_int_eq(1, s21_is_equal(ans, true_ans));
}
END_TEST

Suite *decimal_suite(void) {
  Suite *s = suite_create("Decimal_testcase");
  TCase *tc_decimal = tcase_create("Core");

  suite_add_tcase(s, tc_decimal);
  tcase_add_test(tc_decimal, add);
  tcase_add_test(tc_decimal, sub);
  tcase_add_test(tc_decimal, mul);
  tcase_add_test(tc_decimal, delenie);
  tcase_add_test(tc_decimal, mod);
  tcase_add_test(tc_decimal, is_equal);
  tcase_add_test(tc_decimal, is_not_equal);
  tcase_add_test(tc_decimal, is_greater);
  tcase_add_test(tc_decimal, is_less);
  tcase_add_test(tc_decimal, is_less_or_eq);
  tcase_add_test(tc_decimal, is_greater_or_eq);
  tcase_add_test(tc_decimal, from_decimal_to_int);
  tcase_add_test(tc_decimal, from_int_to_decimal);
  tcase_add_test(tc_decimal, from_decimal_to_float);
  tcase_add_test(tc_decimal, from_float_to_decimal);
  tcase_add_test(tc_decimal, floor21);
  tcase_add_test(tc_decimal, round21);
  tcase_add_test(tc_decimal, truncate);
  tcase_add_test(tc_decimal, negate);

  return s;
}

int main() {
  int number_failed;
  Suite *s;
  SRunner *sr;

  s = decimal_suite();
  sr = srunner_create(s);

  srunner_set_fork_status(sr, CK_NOFORK);
  srunner_run_all(sr, CK_NORMAL);
  number_failed = srunner_ntests_failed(sr);
  srunner_free(sr);
  return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
