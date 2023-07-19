#include <check.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "s21_calc.h"

START_TEST(test_1) {
  char* input = "12.2345+(-235)";
  long double res = 0;
  long double true_res = 12.2345 + (-235);
  calc(input, &res);
  ck_assert_ldouble_eq_tol(true_res, res, 1e-06);
}
END_TEST

START_TEST(test_2) {
  char* input = "12.3-2";
  long double res = 0;
  long double true_res = 12.3 - 2;
  calc(input, &res);
  ck_assert_ldouble_eq_tol(true_res, res, 1e-06);
}
END_TEST

START_TEST(test_3) {
  char* input = "9 / 0";
  long double res = 0;
  calc(input, &res);
  ck_assert_ldouble_infinite(res);
}
END_TEST

START_TEST(test_4) {
  char* input = "-8 + (-sin(7)) * (-(78 + (3 - 8)))";
  long double res = 0;
  long double true_res = -8 + (-sin(7)) * (-(78 + (3 - 8)));
  calc(input, &res);
  ck_assert_ldouble_eq_tol(true_res, res, 1e-06);
}
END_TEST

START_TEST(test_5) {
  char* input = "7 % 3";
  long double res = 0;
  long double true_res = 7 % 3;
  calc(input, &res);
  ck_assert_ldouble_eq_tol(true_res, res, 1e-06);
}
END_TEST

START_TEST(test_6) {
  char* input = "+78- 6.342 / (-sin(5) - ln(6^5) * tan(1))";
  long double res = 0;
  long double true_res = 78.4880873472;
  calc(input, &res);
  ck_assert_ldouble_eq_tol(true_res, res, 1e-06);
}
END_TEST

START_TEST(test_7) {
  char* input = "6^(2) + 7";
  long double res = 0;
  long double true_res = pow(6, 2) + 7;
  calc(input, &res);
  ck_assert_ldouble_eq_tol(true_res, res, 1e-06);
}
END_TEST

START_TEST(test_8) {
  char* input = "- log(3.4) / sqrt(9)";
  long double res = 0;
  long double true_res = -0.177159639;
  calc(input, &res);
  ck_assert_ldouble_eq_tol(true_res, res, 1e-06);
}
END_TEST

START_TEST(test_9) {
  char* input = "cos(8) - acos(1) * asin(0.4) / atan(78)";
  long double res = 0;
  long double true_res = cos(8) - acos(1) * asin(0.4) / atan(78);
  calc(input, &res);
  ck_assert_ldouble_eq_tol(true_res, res, 1e-06);
}
END_TEST

START_TEST(test_10) {
  char* input = "*ec4";
  long double res = 0;
  ck_assert_int_eq(calc(input, &res), 1);
}
END_TEST

START_TEST(test_11) {
  char* input = "998+ 9 -";
  long double res = 0;
  ck_assert_int_eq(calc(input, &res), 1);
}
END_TEST

START_TEST(test_12) {
  char* input = "()";
  long double res = 0;
  ck_assert_int_eq(calc(input, &res), 1);
}
END_TEST

START_TEST(test_13) {
  long double S = 670000;
  long double p = 5;
  int m = 15;
  char t = 'a';
  long double mp = 0;
  long double ov = 0;
  long double tp = 0;
  calculate_credit(S, p, m, t, 1, &mp, &ov, &tp);
  ck_assert_ldouble_eq_tol(mp, 46170.0, 1e-02);
  ck_assert_ldouble_eq_tol(ov, 22550.0, 1e-02);
  ck_assert_ldouble_eq_tol(tp, 692550.0, 1e-02);
}
END_TEST

START_TEST(test_14) {
  long double S = 670000;
  long double p = 5;
  int m = 15;
  int m_n = 3;
  char t = 'd';
  long double mp = 0;
  long double ov = 0;
  long double tp = 0;
  calculate_credit(S, p, m, t, m_n, &mp, &ov, &tp);
  ck_assert_ldouble_eq_tol(mp, 47132.511416, 1e-02);
  ck_assert_ldouble_eq_tol(ov, 22259.908676, 1e-01);
  ck_assert_ldouble_eq_tol(tp, 692259.908676, 1e-01);
}
END_TEST

Suite* calc_suite(void) {
  Suite* s = suite_create("SmartCalc_testcase");
  TCase* tc_calc = tcase_create("Core");

  suite_add_tcase(s, tc_calc);
  tcase_add_test(tc_calc, test_1);
  tcase_add_test(tc_calc, test_2);
  tcase_add_test(tc_calc, test_3);
  tcase_add_test(tc_calc, test_4);
  tcase_add_test(tc_calc, test_5);
  tcase_add_test(tc_calc, test_6);
  tcase_add_test(tc_calc, test_7);
  tcase_add_test(tc_calc, test_8);
  tcase_add_test(tc_calc, test_9);
  tcase_add_test(tc_calc, test_10);
  tcase_add_test(tc_calc, test_11);
  tcase_add_test(tc_calc, test_12);
  tcase_add_test(tc_calc, test_13);
  tcase_add_test(tc_calc, test_14);

  return s;
}

int main() {
  int number_failed;
  Suite* s;
  SRunner* sr;

  s = calc_suite();
  sr = srunner_create(s);

  srunner_set_fork_status(sr, CK_NOFORK);
  srunner_run_all(sr, CK_NORMAL);
  number_failed = srunner_ntests_failed(sr);
  srunner_free(sr);
  return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
