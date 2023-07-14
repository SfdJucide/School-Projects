#include <check.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "s21_matrix.h"

START_TEST(eq_fail_size) {
  matrix_t A, B;
  s21_create_matrix(1, 2, &A);
  s21_create_matrix(2, 2, &B);

  A.matrix[0][1] = 9;
  A.matrix[0][0] = 8;

  B.matrix[0][0] = 9;
  B.matrix[0][1] = 8;
  B.matrix[0][0] = 9;
  B.matrix[0][1] = 8;

  ck_assert_int_eq(0, s21_eq_matrix(&A, &B));

  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
}
END_TEST

START_TEST(eq_fail_num) {
  matrix_t A, B;
  s21_create_matrix(2, 2, &A);
  s21_create_matrix(2, 2, &B);

  A.matrix[0][0] = 9.969328400;
  A.matrix[0][1] = 8;
  A.matrix[0][0] = 9;
  A.matrix[0][1] = 8;

  B.matrix[0][0] = 9.969328399;
  B.matrix[0][1] = 8;
  B.matrix[0][0] = 9;
  B.matrix[0][1] = 8;

  ck_assert_int_eq(1, s21_eq_matrix(&A, &B));

  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
}
END_TEST

START_TEST(eq_ok) {
  matrix_t A, B;
  s21_create_matrix(2, 2, &A);
  s21_create_matrix(2, 2, &B);

  A.matrix[0][0] = 9.969328464;
  A.matrix[0][1] = 8;
  A.matrix[0][0] = 9;
  A.matrix[0][1] = 8;

  B.matrix[0][0] = 9.969328400;
  B.matrix[0][1] = 8;
  B.matrix[0][0] = 9;
  B.matrix[0][1] = 8;

  ck_assert_int_eq(1, s21_eq_matrix(&A, &B));

  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
}
END_TEST

START_TEST(add_ok) {
  matrix_t A, B, C, ANS;
  s21_create_matrix(3, 3, &A);
  s21_create_matrix(3, 3, &B);
  s21_create_matrix(3, 3, &ANS);

  A.matrix[0][0] = 1;
  A.matrix[0][1] = 2;
  A.matrix[0][2] = 3;
  A.matrix[1][0] = 1;
  A.matrix[1][1] = 2;
  A.matrix[1][2] = 3;
  A.matrix[2][0] = 1;
  A.matrix[2][1] = 2;
  A.matrix[2][2] = 3;

  B.matrix[0][0] = 3;
  B.matrix[0][1] = 2;
  B.matrix[0][2] = 1;
  B.matrix[1][0] = 3;
  B.matrix[1][1] = 2;
  B.matrix[1][2] = 1;
  B.matrix[2][0] = 3;
  B.matrix[2][1] = 2;
  B.matrix[2][2] = 1;

  ANS.matrix[0][0] = 4;
  ANS.matrix[0][1] = 4;
  ANS.matrix[0][2] = 4;
  ANS.matrix[1][0] = 4;
  ANS.matrix[1][1] = 4;
  ANS.matrix[1][2] = 4;
  ANS.matrix[2][0] = 4;
  ANS.matrix[2][1] = 4;
  ANS.matrix[2][2] = 4;

  ck_assert_int_eq(0, s21_sum_matrix(&A, &B, &C));
  ck_assert_int_eq(1, s21_eq_matrix(&C, &ANS));

  s21_remove_matrix(&C);
  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
  s21_remove_matrix(&ANS);
}
END_TEST

START_TEST(add_fail) {
  matrix_t A, B, C;
  s21_create_matrix(2, 2, &A);
  s21_create_matrix(1, 3, &B);

  A.matrix[0][0] = 1;
  A.matrix[0][1] = 2;
  A.matrix[1][0] = 1;
  A.matrix[1][1] = 2;

  B.matrix[0][0] = 3;
  B.matrix[0][1] = 2;
  B.matrix[0][2] = 1;

  ck_assert_int_eq(2, s21_sum_matrix(&A, &B, &C));

  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
}
END_TEST

START_TEST(sub_ok) {
  matrix_t A, B, C, ANS;
  s21_create_matrix(3, 3, &A);
  s21_create_matrix(3, 3, &B);
  s21_create_matrix(3, 3, &ANS);

  A.matrix[0][0] = 1;
  A.matrix[0][1] = 2;
  A.matrix[0][2] = 3;
  A.matrix[1][0] = 1;
  A.matrix[1][1] = 2;
  A.matrix[1][2] = 3;
  A.matrix[2][0] = 1;
  A.matrix[2][1] = 2;
  A.matrix[2][2] = 3;

  B.matrix[0][0] = 3;
  B.matrix[0][1] = 2;
  B.matrix[0][2] = 1;
  B.matrix[1][0] = 3;
  B.matrix[1][1] = 2;
  B.matrix[1][2] = 1;
  B.matrix[2][0] = 3;
  B.matrix[2][1] = 2;
  B.matrix[2][2] = 1;

  ANS.matrix[0][0] = -2;
  ANS.matrix[0][1] = 0;
  ANS.matrix[0][2] = 2;
  ANS.matrix[1][0] = -2;
  ANS.matrix[1][1] = 0;
  ANS.matrix[1][2] = 2;
  ANS.matrix[2][0] = -2;
  ANS.matrix[2][1] = 0;
  ANS.matrix[2][2] = 2;

  ck_assert_int_eq(0, s21_sub_matrix(&A, &B, &C));
  ck_assert_int_eq(1, s21_eq_matrix(&C, &ANS));

  s21_remove_matrix(&C);
  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
  s21_remove_matrix(&ANS);
}
END_TEST

START_TEST(sub_fail) {
  matrix_t A, B, C;
  s21_create_matrix(2, 2, &A);
  s21_create_matrix(1, 3, &B);

  A.matrix[0][0] = 1;
  A.matrix[0][1] = 2;
  A.matrix[1][0] = 1;
  A.matrix[1][1] = 2;

  B.matrix[0][0] = 3;
  B.matrix[0][1] = 2;
  B.matrix[0][2] = 1;

  ck_assert_int_eq(2, s21_sub_matrix(&A, &B, &C));

  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
}
END_TEST

START_TEST(mul_num) {
  matrix_t A, C, ANS;
  double b = 0.5;
  s21_create_matrix(3, 3, &A);
  s21_create_matrix(3, 3, &ANS);

  A.matrix[0][0] = 1;
  A.matrix[0][1] = 2;
  A.matrix[0][2] = 3;
  A.matrix[1][0] = 4;
  A.matrix[1][1] = 5;
  A.matrix[1][2] = 6;
  A.matrix[2][0] = 7;
  A.matrix[2][1] = 8;
  A.matrix[2][2] = 9;

  ANS.matrix[0][0] = 0.5;
  ANS.matrix[0][1] = 1;
  ANS.matrix[0][2] = 1.5;
  ANS.matrix[1][0] = 2;
  ANS.matrix[1][1] = 2.5;
  ANS.matrix[1][2] = 3;
  ANS.matrix[2][0] = 3.5;
  ANS.matrix[2][1] = 4;
  ANS.matrix[2][2] = 4.5;

  ck_assert_int_eq(0, s21_mult_number(&A, b, &C));
  ck_assert_int_eq(1, s21_eq_matrix(&C, &ANS));

  s21_remove_matrix(&C);
  s21_remove_matrix(&A);
  s21_remove_matrix(&ANS);
}
END_TEST

START_TEST(mul_ok) {
  matrix_t A, B, C, ANS;
  s21_create_matrix(2, 3, &A);
  s21_create_matrix(3, 2, &B);
  s21_create_matrix(2, 2, &ANS);

  A.matrix[0][0] = 1;
  A.matrix[0][1] = 2;
  A.matrix[0][2] = -3;
  A.matrix[1][0] = 1;
  A.matrix[1][1] = -2;
  A.matrix[1][2] = 3;

  B.matrix[0][0] = 3;
  B.matrix[0][1] = 2;
  B.matrix[1][0] = 3;
  B.matrix[1][1] = 2;
  B.matrix[2][0] = 3;
  B.matrix[2][1] = 2;

  ANS.matrix[0][0] = 0;
  ANS.matrix[0][1] = 0;
  ANS.matrix[1][0] = 6;
  ANS.matrix[1][1] = 4;

  ck_assert_int_eq(0, s21_mult_matrix(&A, &B, &C));
  ck_assert_int_eq(1, s21_eq_matrix(&C, &ANS));

  s21_remove_matrix(&C);
  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
  s21_remove_matrix(&ANS);
}
END_TEST

START_TEST(mul_fail) {
  matrix_t A, B, C;
  s21_create_matrix(2, 2, &A);
  s21_create_matrix(1, 3, &B);

  A.matrix[0][0] = 1;
  A.matrix[0][1] = 2;
  A.matrix[1][0] = 1;
  A.matrix[1][1] = 2;

  B.matrix[0][0] = 3;
  B.matrix[0][1] = 2;
  B.matrix[0][2] = 1;

  ck_assert_int_eq(2, s21_mult_matrix(&A, &B, &C));

  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
}
END_TEST

START_TEST(trans) {
  matrix_t A, C, ANS;
  s21_create_matrix(1, 4, &A);
  s21_create_matrix(4, 1, &ANS);

  A.matrix[0][0] = 1;
  A.matrix[0][1] = 2;
  A.matrix[0][2] = 3;
  A.matrix[0][3] = 4;

  ANS.matrix[0][0] = 1;
  ANS.matrix[1][0] = 2;
  ANS.matrix[2][0] = 3;
  ANS.matrix[3][0] = 4;

  ck_assert_int_eq(0, s21_transpose(&A, &C));
  ck_assert_int_eq(1, s21_eq_matrix(&C, &ANS));

  s21_remove_matrix(&C);
  s21_remove_matrix(&A);
  s21_remove_matrix(&ANS);
}
END_TEST

START_TEST(det_ok_1) {
  matrix_t A;
  double res = 0;
  s21_create_matrix(4, 4, &A);

  A.matrix[0][0] = 1;
  A.matrix[0][1] = 2;
  A.matrix[0][2] = 3;
  A.matrix[0][3] = 3;
  A.matrix[1][0] = 0;
  A.matrix[1][1] = 4;
  A.matrix[1][2] = 2;
  A.matrix[1][3] = 2;
  A.matrix[2][0] = 5;
  A.matrix[2][1] = 2;
  A.matrix[2][2] = 1;
  A.matrix[2][3] = 1;
  A.matrix[3][0] = 7;
  A.matrix[3][1] = -9;
  A.matrix[3][2] = 4;
  A.matrix[3][3] = -8;

  ck_assert_int_eq(0, s21_determinant(&A, &res));
  ck_assert_double_eq(480.0, res);

  s21_remove_matrix(&A);
}
END_TEST

START_TEST(det_ok_2) {
  matrix_t A;
  double res = 0;
  s21_create_matrix(2, 2, &A);

  A.matrix[0][0] = 4;
  A.matrix[0][1] = 2;
  A.matrix[1][0] = 2;
  A.matrix[1][1] = 1;

  ck_assert_int_eq(0, s21_determinant(&A, &res));
  ck_assert_double_eq(0.0, res);

  s21_remove_matrix(&A);
}
END_TEST

START_TEST(det_ok_3) {
  matrix_t A;
  double res = 0;
  s21_create_matrix(1, 1, &A);

  A.matrix[0][0] = 4;

  ck_assert_int_eq(0, s21_determinant(&A, &res));
  ck_assert_double_eq(4.0, res);

  s21_remove_matrix(&A);
}
END_TEST

START_TEST(det_fail) {
  matrix_t A;
  double res = 480;
  s21_create_matrix(3, 4, &A);

  A.matrix[0][0] = 1;
  A.matrix[0][1] = 2;
  A.matrix[0][2] = 3;
  A.matrix[0][3] = 3;
  A.matrix[1][0] = 0;
  A.matrix[1][1] = 4;
  A.matrix[1][2] = 2;
  A.matrix[1][3] = 2;
  A.matrix[2][0] = 5;
  A.matrix[2][1] = 2;
  A.matrix[2][2] = 1;
  A.matrix[2][3] = 1;

  ck_assert_int_eq(2, s21_determinant(&A, &res));

  s21_remove_matrix(&A);
}
END_TEST

START_TEST(minors_ok_1) {
  matrix_t A, C, ANS;
  s21_create_matrix(3, 3, &A);
  s21_create_matrix(3, 3, &ANS);

  A.matrix[0][0] = 1;
  A.matrix[0][1] = 2;
  A.matrix[0][2] = 3;
  A.matrix[1][0] = 0;
  A.matrix[1][1] = 4;
  A.matrix[1][2] = 2;
  A.matrix[2][0] = 5;
  A.matrix[2][1] = 2;
  A.matrix[2][2] = 1;

  ANS.matrix[0][0] = 0;
  ANS.matrix[0][1] = 10;
  ANS.matrix[0][2] = -20;
  ANS.matrix[1][0] = 4;
  ANS.matrix[1][1] = -14;
  ANS.matrix[1][2] = 8;
  ANS.matrix[2][0] = -8;
  ANS.matrix[2][1] = -2;
  ANS.matrix[2][2] = 4;

  ck_assert_int_eq(0, s21_calc_complements(&A, &C));
  ck_assert_int_eq(1, s21_eq_matrix(&C, &ANS));

  s21_remove_matrix(&C);
  s21_remove_matrix(&A);
  s21_remove_matrix(&ANS);
}
END_TEST

START_TEST(minors_ok_2) {
  matrix_t A, C, ANS;
  s21_create_matrix(4, 4, &A);
  s21_create_matrix(4, 4, &ANS);

  A.matrix[0][0] = 1;
  A.matrix[0][1] = 2;
  A.matrix[0][2] = 3;
  A.matrix[0][3] = 3;
  A.matrix[1][0] = 0;
  A.matrix[1][1] = 4;
  A.matrix[1][2] = 2;
  A.matrix[1][3] = 2;
  A.matrix[2][0] = 5;
  A.matrix[2][1] = 2;
  A.matrix[2][2] = 1;
  A.matrix[2][3] = 1;
  A.matrix[3][0] = 7;
  A.matrix[3][1] = -9;
  A.matrix[3][2] = 4;
  A.matrix[3][3] = -8;

  ANS.matrix[0][0] = 0;
  ANS.matrix[0][1] = -120;
  ANS.matrix[0][2] = 70;
  ANS.matrix[0][3] = 170;
  ANS.matrix[1][0] = -48;
  ANS.matrix[1][1] = 168;
  ANS.matrix[1][2] = 90;
  ANS.matrix[1][3] = -186;
  ANS.matrix[2][0] = 96;
  ANS.matrix[2][1] = 24;
  ANS.matrix[2][2] = -70;
  ANS.matrix[2][3] = 22;
  ANS.matrix[3][0] = 0;
  ANS.matrix[3][1] = 0;
  ANS.matrix[3][2] = 40;
  ANS.matrix[3][3] = -40;

  ck_assert_int_eq(0, s21_calc_complements(&A, &C));
  ck_assert_int_eq(1, s21_eq_matrix(&C, &ANS));

  s21_remove_matrix(&C);
  s21_remove_matrix(&A);
  s21_remove_matrix(&ANS);
}
END_TEST

START_TEST(minors_fail) {
  matrix_t A, C;
  s21_create_matrix(3, 2, &A);

  A.matrix[0][0] = 1;
  A.matrix[0][1] = 2;
  A.matrix[1][0] = 0;
  A.matrix[1][1] = 4;
  A.matrix[2][0] = 5;
  A.matrix[2][1] = 2;

  ck_assert_int_eq(2, s21_calc_complements(&A, &C));

  s21_remove_matrix(&A);
}
END_TEST

START_TEST(invert_fail) {
  matrix_t A, C;
  s21_create_matrix(3, 2, &A);

  A.matrix[0][0] = 1;
  A.matrix[0][1] = 2;
  A.matrix[1][0] = 0;
  A.matrix[1][1] = 4;
  A.matrix[2][0] = 5;
  A.matrix[2][1] = 2;

  ck_assert_int_eq(2, s21_inverse_matrix(&A, &C));

  s21_remove_matrix(&A);
}
END_TEST

START_TEST(invert_fail_zero) {
  matrix_t A, C;
  s21_create_matrix(2, 2, &A);

  A.matrix[0][0] = 4;
  A.matrix[0][1] = 2;
  A.matrix[1][0] = 2;
  A.matrix[1][1] = 1;

  ck_assert_int_eq(2, s21_inverse_matrix(&A, &C));

  s21_remove_matrix(&A);
}
END_TEST

START_TEST(invert_ok_1) {
  matrix_t A, C, ANS;
  s21_create_matrix(3, 3, &A);
  s21_create_matrix(3, 3, &ANS);

  A.matrix[0][0] = 2;
  A.matrix[0][1] = 5;
  A.matrix[0][2] = 7;
  A.matrix[1][0] = 6;
  A.matrix[1][1] = 3;
  A.matrix[1][2] = 4;
  A.matrix[2][0] = 5;
  A.matrix[2][1] = -2;
  A.matrix[2][2] = -3;

  ANS.matrix[0][0] = 1;
  ANS.matrix[0][1] = -1;
  ANS.matrix[0][2] = 1;
  ANS.matrix[1][0] = -38;
  ANS.matrix[1][1] = 41;
  ANS.matrix[1][2] = -34;
  ANS.matrix[2][0] = 27;
  ANS.matrix[2][1] = -29;
  ANS.matrix[2][2] = 24;

  ck_assert_int_eq(0, s21_inverse_matrix(&A, &C));
  ck_assert_int_eq(1, s21_eq_matrix(&C, &ANS));

  s21_remove_matrix(&C);
  s21_remove_matrix(&A);
  s21_remove_matrix(&ANS);
}
END_TEST

START_TEST(invert_ok_2) {
  matrix_t A, C, ANS;
  s21_create_matrix(4, 4, &A);
  s21_create_matrix(4, 4, &ANS);

  A.matrix[0][0] = 1;
  A.matrix[0][1] = 2;
  A.matrix[0][2] = 3;
  A.matrix[0][3] = 3;
  A.matrix[1][0] = 0;
  A.matrix[1][1] = 4;
  A.matrix[1][2] = 2;
  A.matrix[1][3] = 2;
  A.matrix[2][0] = 5;
  A.matrix[2][1] = 2;
  A.matrix[2][2] = 1;
  A.matrix[2][3] = 1;
  A.matrix[3][0] = 7;
  A.matrix[3][1] = -9;
  A.matrix[3][2] = 4;
  A.matrix[3][3] = -8;

  ANS.matrix[0][0] = 0;
  ANS.matrix[0][1] = -0.1;
  ANS.matrix[0][2] = 0.2;
  ANS.matrix[0][3] = -0;
  ANS.matrix[1][0] = -0.25;
  ANS.matrix[1][1] = 0.35;
  ANS.matrix[1][2] = 0.05;
  ANS.matrix[1][3] = 0;
  ANS.matrix[2][0] = 0.1458333;
  ANS.matrix[2][1] = 0.1875;
  ANS.matrix[2][2] = -0.1458333;
  ANS.matrix[2][3] = 0.0833333;
  ANS.matrix[3][0] = 0.3541666;
  ANS.matrix[3][1] = -0.3875;
  ANS.matrix[3][2] = 0.0458333;
  ANS.matrix[3][3] = -0.0833333;

  ck_assert_int_eq(0, s21_inverse_matrix(&A, &C));
  ck_assert_int_eq(1, s21_eq_matrix(&C, &ANS));

  s21_remove_matrix(&C);
  s21_remove_matrix(&A);
  s21_remove_matrix(&ANS);
}
END_TEST

Suite *matrix_suite(void) {
  Suite *s = suite_create("Matrix_testcase");
  TCase *tc_matrix = tcase_create("Core");

  suite_add_tcase(s, tc_matrix);
  tcase_add_test(tc_matrix, eq_ok);
  tcase_add_test(tc_matrix, eq_fail_size);
  tcase_add_test(tc_matrix, eq_fail_num);
  tcase_add_test(tc_matrix, add_ok);
  tcase_add_test(tc_matrix, add_fail);
  tcase_add_test(tc_matrix, sub_ok);
  tcase_add_test(tc_matrix, sub_fail);
  tcase_add_test(tc_matrix, mul_num);
  tcase_add_test(tc_matrix, mul_ok);
  tcase_add_test(tc_matrix, mul_fail);
  tcase_add_test(tc_matrix, trans);
  tcase_add_test(tc_matrix, det_fail);
  tcase_add_test(tc_matrix, det_ok_1);
  tcase_add_test(tc_matrix, det_ok_2);
  tcase_add_test(tc_matrix, det_ok_3);
  tcase_add_test(tc_matrix, minors_fail);
  tcase_add_test(tc_matrix, minors_ok_1);
  tcase_add_test(tc_matrix, minors_ok_2);
  tcase_add_test(tc_matrix, invert_fail);
  tcase_add_test(tc_matrix, invert_fail_zero);
  tcase_add_test(tc_matrix, invert_ok_1);
  tcase_add_test(tc_matrix, invert_ok_2);

  return s;
}

int main() {
  int numBer_failed;
  Suite *s;
  SRunner *sr;

  s = matrix_suite();
  sr = srunner_create(s);

  srunner_set_fork_status(sr, CK_NOFORK);
  srunner_run_all(sr, CK_NORMAL);
  numBer_failed = srunner_ntests_failed(sr);
  srunner_free(sr);
  return (numBer_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
