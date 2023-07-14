#include "s21_matrix.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

// UTILS

void print_matrix(matrix_t A) {
  for (int i = 0; i < A.rows; i++) {
    for (int j = 0; j < A.columns; j++) {
      printf("%.7f ", A.matrix[i][j]);
    }
    printf("\n");
  }
  printf("\n");
}

int simple_check_matrix(matrix_t *A) {
  int code = OK;
  if (A->columns <= 0 || A->rows <= 0 || !A) {
    code = INCORRECT_MATRIX;
  }
  return code;
}

// MAIN FUNCTIONS

int s21_create_matrix(int rows, int columns, matrix_t *result) {
  if (rows > 0 && columns > 0) {
    result->columns = columns;
    result->rows = rows;

    result->matrix = (double **)calloc(rows, sizeof(double *));
    for (int i = 0; i < rows; i++) {
      result->matrix[i] = (double *)calloc(columns, sizeof(double));
    }

  } else {
    return INCORRECT_MATRIX;
  }

  return OK;
}

void s21_remove_matrix(matrix_t *A) {
  for (int i = 0; i < A->rows; i++) {
    free(A->matrix[i]);
  }
  free(A->matrix);
}

int s21_eq_matrix(matrix_t *A, matrix_t *B) {
  int eq_code = SUCCESS;
  if (simple_check_matrix(A) || simple_check_matrix(B)) {
    return FAILURE;
  }
  int stop_flag = 0;
  if (A->columns == B->columns && A->rows == B->rows) {
    for (int i = 0; i < A->rows; i++) {
      if (stop_flag) break;

      for (int j = 0; j < A->columns; j++) {
        if (abs((int)(A->matrix[i][j] * pow(10, 7)) -
                (int)(B->matrix[i][j] * pow(10, 7))) != 0) {
          eq_code = FAILURE;
          stop_flag = 1;
          break;
        }
      }
    }

  } else {
    eq_code = FAILURE;
  }

  return eq_code;
}

int s21_sum_matrix(matrix_t *A, matrix_t *B, matrix_t *result) {
  int code_result = OK;
  if (simple_check_matrix(A) || simple_check_matrix(B)) {
    return INCORRECT_MATRIX;
  }
  if (A->columns == B->columns && A->rows == B->rows) {
    s21_create_matrix(A->rows, A->columns, result);
    for (int i = 0; i < A->rows; i++) {
      for (int j = 0; j < A->columns; j++) {
        result->matrix[i][j] = A->matrix[i][j] + B->matrix[i][j];
      }
    }

  } else {
    code_result = CALCULATION_ERROR;
  }

  return code_result;
}

int s21_sub_matrix(matrix_t *A, matrix_t *B, matrix_t *result) {
  int code_result = OK;
  if (simple_check_matrix(A) || simple_check_matrix(B)) {
    return INCORRECT_MATRIX;
  }
  if (A->columns == B->columns && A->rows == B->rows) {
    s21_create_matrix(A->rows, A->columns, result);
    for (int i = 0; i < A->rows; i++) {
      for (int j = 0; j < A->columns; j++) {
        result->matrix[i][j] = A->matrix[i][j] - B->matrix[i][j];
      }
    }

  } else {
    code_result = CALCULATION_ERROR;
  }

  return code_result;
}

int s21_mult_number(matrix_t *A, double number, matrix_t *result) {
  int code_result = OK;
  if (simple_check_matrix(A)) {
    return INCORRECT_MATRIX;
  }
  s21_create_matrix(A->rows, A->columns, result);
  for (int i = 0; i < A->rows; i++) {
    for (int j = 0; j < A->columns; j++) {
      result->matrix[i][j] = A->matrix[i][j] * number;
    }
  }

  return code_result;
}

int s21_mult_matrix(matrix_t *A, matrix_t *B, matrix_t *result) {
  int code_result = OK;
  double mul = 0;
  if (simple_check_matrix(A) || simple_check_matrix(B)) {
    return INCORRECT_MATRIX;
  }
  if (A->columns == B->rows) {
    s21_create_matrix(A->rows, B->columns, result);
    for (int i = 0; i < A->rows; i++) {
      for (int j = 0; j < B->columns; j++) {
        mul = 0;
        for (int k = 0; k < A->columns; k++) {
          mul += A->matrix[i][k] * B->matrix[k][j];
        }
        result->matrix[i][j] = mul;
      }
    }
  } else {
    code_result = CALCULATION_ERROR;
  }

  return code_result;
}

int s21_transpose(matrix_t *A, matrix_t *result) {
  int code_result = OK;
  if (simple_check_matrix(A)) {
    return INCORRECT_MATRIX;
  }
  s21_create_matrix(A->columns, A->rows, result);
  for (int i = 0; i < A->rows; i++) {
    for (int j = 0; j < A->columns; j++) {
      result->matrix[j][i] = A->matrix[i][j];
    }
  }

  return code_result;
}

int s21_calc_complements(matrix_t *A, matrix_t *result) {
  if (simple_check_matrix(A)) {
    return INCORRECT_MATRIX;
  }
  int code_result = OK;
  if (A->columns == A->rows) {
    matrix_t minor_matrix;
    double det;
    s21_create_matrix(A->rows, A->columns, result);
    for (int i = 0; i < A->rows; i++) {
      for (int j = 0; j < A->columns; j++) {
        int minor_i = 0;
        int minor_j = 0;
        s21_create_matrix(A->rows - 1, A->columns - 1, &minor_matrix);

        for (int l = 0; l < A->rows; l++) {
          if (l == i) continue;
          minor_j = 0;
          for (int k = 0; k < A->columns; k++) {
            if (k == j) continue;

            minor_matrix.matrix[minor_i][minor_j] = A->matrix[l][k];
            minor_j++;
          }
          minor_i++;
        }

        det = 0;
        s21_determinant(&minor_matrix, &det);
        result->matrix[i][j] = det * pow(-1.0, (i + j));
        s21_remove_matrix(&minor_matrix);
      }
    }

  } else {
    code_result = CALCULATION_ERROR;
  }

  return code_result;
}

int s21_determinant(matrix_t *A, double *result) {
  if (simple_check_matrix(A)) {
    return INCORRECT_MATRIX;
  }
  int code_result = OK;
  if (A->columns == A->rows) {
    *result = 0;
    if (A->columns == 1) {
      *result = A->matrix[0][0];
    } else if (A->columns == 2) {
      *result = (A->matrix[0][0] * A->matrix[1][1]) -
                (A->matrix[1][0] * A->matrix[0][1]);
    } else {
      for (int i = 0; i < A->columns; i++) {
        matrix_t help_matrix;
        double help_det = 0;
        int help_colm_A = 0;
        int help_colm_res = 0;

        s21_create_matrix(A->rows - 1, A->columns - 1, &help_matrix);
        for (int k = 1, l = 0; k < A->rows;
             k++, l++) {  // заполнение матрицы алгебр. дополнений
          help_colm_A = 0;
          help_colm_res = 0;
          while (help_colm_A < A->columns) {
            if (help_colm_A == i) {
              help_colm_A++;
              continue;
            }
            help_matrix.matrix[l][help_colm_res] = A->matrix[k][help_colm_A];
            help_colm_res++;
            help_colm_A++;
          }
        }
        s21_determinant(&help_matrix, &help_det);
        *result += pow(-1.0, i) * A->matrix[0][i] * help_det;
        s21_remove_matrix(&help_matrix);
      }
    }
  } else {
    code_result = CALCULATION_ERROR;
  }

  return code_result;
}

int s21_inverse_matrix(matrix_t *A, matrix_t *result) {
  if (simple_check_matrix(A)) {
    return INCORRECT_MATRIX;
  }
  int code_result = OK;
  if (A->columns == A->rows) {
    double det;
    matrix_t tmp_matrix;
    matrix_t transpose_matrix;
    s21_determinant(A, &det);
    if (det != 0) {
      s21_calc_complements(A, &tmp_matrix);
      s21_transpose(&tmp_matrix, &transpose_matrix);
      s21_mult_number(&transpose_matrix, 1 / det, result);

      s21_remove_matrix(&transpose_matrix);
      s21_remove_matrix(&tmp_matrix);
    } else {
      code_result = CALCULATION_ERROR;
    }
  } else {
    code_result = CALCULATION_ERROR;
  }

  return code_result;
}
