#include <check.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "s21_math.h"

START_TEST(test_abs) {
  ck_assert_int_eq(abs(-54545), s21_abs(-54545));
  ck_assert_int_eq(abs(54545), s21_abs(54545));
  ck_assert_int_eq(abs(54545 / 6), s21_abs(54545 / 6));
  ck_assert_int_eq(abs(-54545 / 6), s21_abs(-54545 / 6));
  ck_assert_int_eq(abs(0), s21_abs(0));
  ck_assert_int_eq(abs(-0), s21_abs(-0));
  ck_assert_int_eq(abs(789743587), s21_abs(789743587));
  ck_assert_int_eq(abs(-789743587), s21_abs(-789743587));
}
END_TEST

START_TEST(test_fmod) {
  ck_assert_double_eq_tol(fmod(3.5, 1.1), s21_fmod(3.5, 1.1), 1e-6);
  ck_assert_double_eq_tol(fmod(-3.5, 1.1), s21_fmod(-3.5, 1.1), 1e-6);
  ck_assert_double_eq_tol(fmod(484848.84848, 1.1), s21_fmod(484848.84848, 1.1),
                          1e-6);
  ck_assert_double_eq_tol(fmod(-0.000001, 0.2), s21_fmod(-0.000001, 0.2), 1e-6);
  ck_assert_double_eq_tol(fmod(0.878364, 6.4332), s21_fmod(0.878364, 6.4332),
                          1e-6);
  ck_assert_double_eq_tol(fmod(-153682, -76.1), s21_fmod(-153682, -76.1), 1e-6);
}
END_TEST

START_TEST(test_pow) {
  ck_assert_double_eq_tol(pow(10, 3), s21_pow(10, 3), 1e-6);
  ck_assert_double_eq_tol(pow(10, 0), s21_pow(10, 0), 1e-6);
  ck_assert_double_eq_tol(pow(0, 10), s21_pow(0, 10), 1e-6);
  ck_assert_double_eq_tol(pow(0, 0), s21_pow(0, 0), 1e-6);
  ck_assert_double_eq_tol(pow(32, 0.6), s21_pow(32, 0.6), 1e-6);
  ck_assert_double_nan(s21_pow(-32, 0.6));
  ck_assert_double_eq_tol(pow(6.5, 3), s21_pow(6.5, 3), 1e-6);
  ck_assert_double_eq_tol(pow(6.5, -3), s21_pow(6.5, -3), 1e-6);
  ck_assert_double_eq_tol(pow(-6.5, -3), s21_pow(-6.5, -3), 1e-6);
  ck_assert_double_eq_tol(pow(2.2, 4.4), s21_pow(2.2, 4.4), 1e-6);
  ck_assert_double_eq(pow(s21_INF, 4.4), s21_pow(s21_INF, 4.4));
  ck_assert_double_eq(pow(1, s21_INF), s21_pow(1, s21_INF));
  ck_assert_double_eq(pow(0, s21_INF), s21_pow(0, s21_INF));
  ck_assert_double_eq(pow(s21_INF, 0), s21_pow(s21_INF, 0));
  ck_assert_double_eq(pow(5, -s21_INF), s21_pow(5, -s21_INF));
  ck_assert_double_eq(pow(s21_INF, s21_INF), s21_pow(s21_INF, s21_INF));
  ck_assert_double_eq(pow(-s21_INF, 2), s21_pow(-s21_INF, 2));
  ck_assert_double_nan(s21_pow(s21_NAN, 6));
  ck_assert_double_nan(s21_pow(9.45, s21_NAN));
}
END_TEST

START_TEST(test_fabs) {
  ck_assert_double_eq_tol(fabs(10.6), s21_fabs(10.6), 1e-6);
  ck_assert_double_eq_tol(fabs(-0.9), s21_fabs(-0.9), 1e-6);
  ck_assert_double_eq_tol(fabs(-1e-6), s21_fabs(-1e-6), 1e-6);
  ck_assert_double_eq(fabs(-s21_INF), s21_fabs(-s21_INF));
  ck_assert_double_eq(fabs(s21_INF), s21_fabs(s21_INF));
  ck_assert_double_nan(s21_fabs(-s21_NAN));
  ck_assert_double_eq_tol(fabs(0.0), s21_fabs(0.0), 1e-6);
  ck_assert_double_eq_tol(fabs(-0.0), s21_fabs(-0.0), 1e-6);
}
END_TEST

START_TEST(test_floor) {
  ck_assert_double_eq_tol(floor(10.6666666), s21_floor(10.66666666), 1e-6);
  ck_assert_double_eq_tol(floor(-10093.89436), s21_floor(-10093.89436), 1e-6);
  ck_assert_double_eq_tol(floor(9.45), s21_floor(9.45), 1e-6);
  ck_assert_double_eq_tol(floor(9), s21_floor(9), 1e-6);
  ck_assert_double_eq_tol(floor(-8.549), s21_floor(-8.549), 1e-6);
  ck_assert_double_eq_tol(floor(-0.549), s21_floor(-0.549), 1e-6);
  ck_assert_double_eq_tol(floor(0.555553423), s21_floor(0.555553423), 1e-6);
  ck_assert_double_eq(floor(s21_INF), s21_floor(s21_INF));
  ck_assert_double_eq(floor(-s21_INF), s21_floor(-s21_INF));
  ck_assert_double_nan(s21_floor(s21_NAN));
  ck_assert_double_nan(s21_floor(-s21_NAN));
}
END_TEST

START_TEST(test_ceil) {
  ck_assert_double_eq_tol(ceil(10.6666666), s21_ceil(10.66666666), 1e-6);
  ck_assert_double_eq_tol(ceil(-10093.89436), s21_ceil(-10093.89436), 1e-6);
  ck_assert_double_eq_tol(ceil(9.45), s21_ceil(9.45), 1e-6);
  ck_assert_double_eq_tol(ceil(-8.549), s21_ceil(-8.549), 1e-6);
  ck_assert_double_eq_tol(ceil(-0.549), s21_ceil(-0.549), 1e-6);
  ck_assert_double_eq_tol(ceil(0.555553423), s21_ceil(0.555553423), 1e-6);
  ck_assert_double_eq(ceil(s21_INF), s21_ceil(s21_INF));
  ck_assert_double_eq(ceil(-s21_INF), s21_ceil(-s21_INF));
  ck_assert_double_nan(s21_ceil(s21_NAN));
  ck_assert_double_nan(s21_ceil(-s21_NAN));
}
END_TEST

START_TEST(test_sqrt) {
  ck_assert_double_eq_tol(sqrt(153687.673452), s21_sqrt(153687.673452), 1e-6);
  ck_assert_double_eq_tol(sqrt(10.772346), s21_sqrt(10.772346), 1e-6);
  ck_assert_double_nan(s21_sqrt(-0.92323));
  ck_assert_double_nan(s21_sqrt(-1e-6));
  ck_assert_double_eq_tol(sqrt(1e-6), s21_sqrt(1e-6), 1e-6);
  ck_assert_double_nan(s21_sqrt(-s21_INF));
  ck_assert_double_eq(sqrt(s21_INF), s21_sqrt(s21_INF));
  ck_assert_double_nan(s21_sqrt(-s21_NAN));
  ck_assert_double_eq_tol(sqrt(0.0), s21_sqrt(0.0), 1e-6);
  ck_assert_double_eq_tol(sqrt(-0.0), s21_sqrt(-0.0), 1e-6);
  ck_assert_double_eq_tol(sqrt(256), s21_sqrt(256), 1e-6);
  for (double i = 0; i <= 1000; i += 4) {
    ck_assert_ldouble_eq_tol(sqrt(i), s21_sqrt(i), 1e-6);
  }
}
END_TEST

START_TEST(test_log) {
  ck_assert_double_eq_tol(log(153687.673452), s21_log(153687.673452), 1e-6);
  ck_assert_double_eq_tol(log(10.772346), s21_log(10.772346), 1e-6);
  ck_assert_double_nan(s21_log(-0.92323));
  ck_assert_double_nan(s21_log(-1e-6));
  ck_assert_double_eq_tol(log(1e-6), s21_log(1e-6), 1e-6);
  ck_assert_double_nan(s21_log(-s21_INF));
  ck_assert_double_eq(log(s21_INF), s21_log(s21_INF));
  ck_assert_double_nan(s21_log(-s21_NAN));
  ck_assert_double_eq(log(0.0), s21_log(0.0));
  ck_assert_double_eq(log(-0.0), s21_log(-0.0));
  ck_assert_double_eq_tol(log(256), s21_log(256), 1e-6);
  for (double i = 1; i <= 1000; i += 4) {
    ck_assert_ldouble_eq_tol(log(i), s21_log(i), 1e-6);
  }
}
END_TEST

START_TEST(test_exp) {
  ck_assert_double_eq_tol(exp(0), s21_exp(0), 1e-6);
  ck_assert_double_eq_tol(exp(1.0), s21_exp(1.0), 1e-6);
  ck_assert_double_eq_tol(exp(-1.0), s21_exp(-1.0), 1e-6);
  ck_assert_double_eq_tol(exp(0.4567893), s21_exp(0.4567893), 1e-6);
  ck_assert_double_eq_tol(exp(10), s21_exp(10), 1e-6);
  ck_assert_double_eq_tol(exp(-10), s21_exp(-10), 1e-6);
  ck_assert_double_eq(exp(s21_INF), s21_exp(s21_INF));
  ck_assert_double_eq(exp(-s21_INF), s21_exp(-s21_INF));
  ck_assert_double_nan(s21_exp(-s21_NAN));
  ck_assert_double_nan(s21_exp(s21_NAN));
  ck_assert_double_eq_tol(exp(-45.4567893), s21_exp(-45.4567893), 1e-6);
  ck_assert_double_eq_tol(exp(-341), s21_exp(-341), 1e-6);
  ck_assert_double_eq_tol(exp(45.4567893), s21_exp(45.4567893), 1e-6);
  ck_assert_double_eq_tol(exp(71), s21_exp(71), 1e-6);
  for (double i = -20; i < 20; i += 0.1)
    ck_assert_ldouble_eq_tol(s21_exp(i), exp(i), 1e-6);
}
END_TEST

START_TEST(test_sin) {
  ck_assert_double_eq_tol(sin(0), s21_sin(0), 1e-6);
  ck_assert_double_eq_tol(sin(1), s21_sin(1), 1e-6);
  ck_assert_double_eq_tol(sin(30 * s21_PI / 180), s21_sin(30 * s21_PI / 180),
                          1e-6);
  ck_assert_double_eq_tol(sin(60 * s21_PI / 360), s21_sin(60 * s21_PI / 360),
                          1e-6);
  ck_assert_double_eq_tol(sin(-s21_PI / 360), s21_sin(-s21_PI / 360), 1e-6);
  ck_assert_double_eq_tol(sin(s21_PI / 6), s21_sin(s21_PI / 6), 1e-6);
  ck_assert_double_eq_tol(sin(s21_TWO_PI), s21_sin(s21_TWO_PI), 1e-6);
  ck_assert_double_eq_tol(sin(1e-6), s21_sin(1e-6), 1e-6);
  ck_assert_double_eq_tol(sin(-1e-6), s21_sin(-1e-6), 1e-6);
  ck_assert_double_nan(s21_sin(s21_INF));
  ck_assert_double_nan(s21_sin(-s21_INF));
  ck_assert_double_nan(s21_sin(s21_NAN));
  for (double i = -100; i <= 100; i += 0.001) {
    ck_assert_ldouble_eq_tol(sin(i), s21_sin(i), 1e-6);
  }
  ck_assert_double_eq_tol(sin(-3456.3453), s21_sin(-3456.3453), 1e-6);
  ck_assert_double_eq_tol(sin(36456.8774678), s21_sin(36456.8774678), 1e-6);
}
END_TEST

START_TEST(test_cos) {
  ck_assert_double_eq_tol(cos(0), s21_cos(0), 1e-6);
  ck_assert_double_eq_tol(cos(1), s21_cos(1), 1e-6);
  ck_assert_double_eq_tol(cos(30 * s21_PI / 180), s21_cos(30 * s21_PI / 180),
                          1e-6);
  ck_assert_double_eq_tol(cos(60 * s21_PI / 360), s21_cos(60 * s21_PI / 360),
                          1e-6);
  ck_assert_double_eq_tol(cos(-s21_PI / 360), s21_cos(-s21_PI / 360), 1e-6);
  ck_assert_double_eq_tol(cos(s21_PI / 6), s21_cos(s21_PI / 6), 1e-6);
  ck_assert_double_eq_tol(cos(s21_TWO_PI), s21_cos(s21_TWO_PI), 1e-6);
  ck_assert_double_eq_tol(cos(1e-6), s21_cos(1e-6), 1e-6);
  ck_assert_double_eq_tol(cos(-1e-6), s21_cos(-1e-6), 1e-6);
  ck_assert_double_nan(s21_cos(s21_INF));
  ck_assert_double_nan(s21_cos(-s21_INF));
  ck_assert_double_nan(s21_cos(s21_NAN));
  for (double i = -100; i <= 100; i += 0.001) {
    ck_assert_ldouble_eq_tol(cos(i), s21_cos(i), 1e-6);
  }
  ck_assert_double_eq_tol(cos(-3456.3453), s21_cos(-3456.3453), 1e-6);
  ck_assert_double_eq_tol(cos(3656.468), s21_cos(3656.468), 1e-6);
}
END_TEST

START_TEST(test_tan) {
  ck_assert_double_eq_tol(tan(0), s21_tan(0), 1e-6);
  ck_assert_double_eq_tol(tan(1), s21_tan(1), 1e-6);
  ck_assert_double_eq_tol(tan(s21_PI / 3), s21_tan(s21_PI / 3), 1e-6);
  ck_assert_double_eq_tol(tan(s21_PI / 4), s21_tan(s21_PI / 4), 1e-6);
  ck_assert_double_eq_tol(tan(s21_PI / 6), s21_tan(s21_PI / 6), 1e-6);
  ck_assert_double_eq_tol(tan(-s21_PI / 3), s21_tan(-s21_PI / 3), 1e-6);
  ck_assert_double_eq_tol(tan(-s21_PI / 4), s21_tan(-s21_PI / 4), 1e-6);
  ck_assert_double_eq_tol(tan(-s21_PI / 6), s21_tan(-s21_PI / 6), 1e-6);
  ck_assert_double_eq_tol(tan(-s21_PI / 360), s21_tan(-s21_PI / 360), 1e-6);
  ck_assert_double_eq_tol(tan(s21_TWO_PI), s21_tan(s21_TWO_PI), 1e-6);
  ck_assert_double_eq_tol(tan(1e-6), s21_tan(1e-6), 1e-6);
  ck_assert_double_eq_tol(tan(-1e-6), s21_tan(-1e-6), 1e-6);
  ck_assert_double_nan(s21_tan(s21_INF));
  ck_assert_double_nan(s21_tan(-s21_INF));
  ck_assert_double_nan(s21_tan(s21_NAN));
  for (double i = -100; i <= 100; i += 0.01) {
    ck_assert_ldouble_eq_tol(tan(i), s21_tan(i), 1e-6);
  }
  ck_assert_double_eq_tol(tan(-3456.3453), s21_tan(-3456.3453), 1e-6);
  ck_assert_double_eq_tol(tan(36456.8774678), s21_tan(36456.8774678), 1e-6);
}
END_TEST

START_TEST(test_asin) {
  ck_assert_double_eq_tol(asin(0), s21_asin(0), 1e-6);
  ck_assert_double_eq_tol(asin(1), s21_asin(1), 1e-6);
  ck_assert_double_eq_tol(asin(0.345), s21_asin(0.345), 1e-6);
  ck_assert_double_eq_tol(asin(-0.345), s21_asin(-0.345), 1e-6);
  ck_assert_double_eq_tol(asin(-0.99999999), s21_asin(-0.99999999), 1e-6);
  ck_assert_double_eq_tol(asin(0.99999999), s21_asin(0.99999999), 1e-6);
  ck_assert_double_eq_tol(asin(1e-6), s21_asin(1e-6), 1e-6);
  ck_assert_double_eq_tol(asin(-1e-6), s21_asin(-1e-6), 1e-6);
  ck_assert_double_nan(s21_asin(5));
  ck_assert_double_nan(s21_asin(-5));
  ck_assert_double_nan(s21_asin(s21_INF));
  ck_assert_double_nan(s21_asin(-s21_INF));
  ck_assert_double_nan(s21_asin(s21_NAN));
  for (double i = -1; i <= 1; i += 0.1) {
    ck_assert_ldouble_eq_tol(asin(i), s21_asin(i), 1e-6);
  }
}
END_TEST

START_TEST(test_acos) {
  ck_assert_double_eq_tol(acos(0), s21_acos(0), 1e-6);
  ck_assert_double_eq_tol(acos(1), s21_acos(1), 1e-6);
  ck_assert_double_eq_tol(acos(0.345), s21_acos(0.345), 1e-6);
  ck_assert_double_eq_tol(acos(-0.345), s21_acos(-0.345), 1e-6);
  ck_assert_double_eq_tol(acos(-0.99999999), s21_acos(-0.99999999), 1e-6);
  ck_assert_double_eq_tol(acos(0.99999999), s21_acos(0.99999999), 1e-6);
  ck_assert_double_eq_tol(acos(1e-6), s21_acos(1e-6), 1e-6);
  ck_assert_double_eq_tol(acos(-1e-6), s21_acos(-1e-6), 1e-6);
  ck_assert_double_nan(s21_acos(5));
  ck_assert_double_nan(s21_acos(-5));
  ck_assert_double_nan(s21_acos(s21_INF));
  ck_assert_double_nan(s21_acos(-s21_INF));
  ck_assert_double_nan(s21_acos(s21_NAN));
  for (double i = -1; i <= 1; i += 0.1) {
    ck_assert_ldouble_eq_tol(acos(i), s21_acos(i), 1e-6);
  }
}
END_TEST

START_TEST(test_atan) {
  ck_assert_double_eq_tol(atan(0), s21_atan(0), 1e-6);
  ck_assert_double_eq_tol(atan(1), s21_atan(1), 1e-6);
  ck_assert_double_eq_tol(atan(-1), s21_atan(-1), 1e-6);
  ck_assert_double_eq_tol(atan(s21_PI / 2), s21_atan(s21_PI / 2), 1e-6);
  ck_assert_double_eq_tol(atan(-s21_PI / 2), s21_atan(-s21_PI / 2), 1e-6);
  ck_assert_double_eq_tol(atan(456), s21_atan(456), 1e-6);
  ck_assert_double_eq_tol(atan(-456), s21_atan(-456), 1e-6);
  ck_assert_double_eq_tol(atan(234.458), s21_atan(234.458), 1e-6);
  ck_assert_double_eq_tol(atan(-234.458), s21_atan(-234.458), 1e-6);
  ck_assert_double_eq_tol(atan(0.345), s21_atan(0.345), 1e-6);
  ck_assert_double_eq_tol(atan(-0.345), s21_atan(-0.345), 1e-6);
  ck_assert_double_eq_tol(atan(1e-6), s21_atan(1e-6), 1e-6);
  ck_assert_double_eq_tol(atan(-1e-6), s21_atan(-1e-6), 1e-6);
  ck_assert_double_eq_tol(atan(s21_INF), s21_atan(s21_INF), 1e-6);
  ck_assert_double_eq_tol(atan(-s21_INF), s21_atan(-s21_INF), 1e-6);
  ck_assert_double_nan(s21_atan(s21_NAN));
}
END_TEST

Suite *math_suite(void) {
  Suite *s = suite_create("Math_testcase");
  TCase *tc_math = tcase_create("Core");

  tcase_add_test(tc_math, test_abs);
  tcase_add_test(tc_math, test_fmod);
  tcase_add_test(tc_math, test_sqrt);
  tcase_add_test(tc_math, test_fabs);
  tcase_add_test(tc_math, test_floor);
  tcase_add_test(tc_math, test_ceil);
  tcase_add_test(tc_math, test_exp);
  tcase_add_test(tc_math, test_sin);
  tcase_add_test(tc_math, test_cos);
  tcase_add_test(tc_math, test_tan);
  tcase_add_test(tc_math, test_pow);
  tcase_add_test(tc_math, test_log);
  tcase_add_test(tc_math, test_atan);
  tcase_add_test(tc_math, test_acos);
  tcase_add_test(tc_math, test_asin);

  suite_add_tcase(s, tc_math);

  return s;
}

int main() {
  int number_failed;
  Suite *s;
  SRunner *sr;

  s = math_suite();
  sr = srunner_create(s);

  srunner_run_all(sr, CK_NORMAL);
  number_failed = srunner_ntests_failed(sr);
  srunner_free(sr);
  return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
