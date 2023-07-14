#ifndef S21_MATRIX_H_
#define S21_MATRIX_H_

#define SUCCESS 1
#define FAILURE 0
#define EPS 1e-7

typedef enum {
  OK,
  INCORRECT_MATRIX,
  CALCULATION_ERROR,
} result_code;

typedef struct matrix_struct {
  double **matrix;
  int rows;
  int columns;
} matrix_t;

// UTILS

/**
 * @brief Печать матрицы
 * @param A матрица для печати
 */
void print_matrix(matrix_t A);

/**
 * @brief Простая проверка матрицы на корректную размерность
 * @param A указатель на матрицу для проверки
 * @return код ошибки
 */
int simple_check_matrix(matrix_t *A);

// MAIN FUNCTIONS

/**
 * @brief Создание матрицы
 * @param rows число строк
 * @param columns число столбцов
 * @param result указатель на матрицу результат
 * @return код выполнения
 */
int s21_create_matrix(int rows, int columns, matrix_t *result);

/**
 * @brief Очистка матрицы
 * @param A указатель на матрицу
 */
void s21_remove_matrix(matrix_t *A);

/**
 * @brief Сравнение матриц до 7 знака после запятой включительно
 * @param A указатель на матрицу
 * @param B указатель на матрицу
 * @return код сравнения
 */
int s21_eq_matrix(matrix_t *A, matrix_t *B);

/**
 * @brief Сложение матриц
 * @param A указатель на матрицу
 * @param B указатель на матрицу
 * @param result указатель на матрицу результат
 * @return код выполнения
 */
int s21_sum_matrix(matrix_t *A, matrix_t *B, matrix_t *result);

/**
 * @brief Вычитание матриц
 * @param A указатель на матрицу
 * @param B указатель на матрицу
 * @param result указатель на матрицу результат
 * @return код выполнения
 */
int s21_sub_matrix(matrix_t *A, matrix_t *B, matrix_t *result);

/**
 * @brief Умножение матрицы на число
 * @param A указатель на матрицу
 * @param number число
 * @param result указатель на матрицу результат
 * @return код выполнения
 */
int s21_mult_number(matrix_t *A, double number, matrix_t *result);

/**
 * @brief Умножение матриц
 * @param A указатель на матрицу
 * @param B указатель на матрицу
 * @param result указатель на матрицу результат
 * @return код выполнения
 */
int s21_mult_matrix(matrix_t *A, matrix_t *B, matrix_t *result);

/**
 * @brief Транспонирование матрицы
 * @param A указатель на матрицу
 * @param result указатель на матрицу результат
 * @return код выполнения
 */
int s21_transpose(matrix_t *A, matrix_t *result);

/**
 * @brief Минор матрицы и матрица алгебраических дополнений
 * @param A указатель на матрицу
 * @param result указатель на матрицу результат
 * @return код выполнения
 */
int s21_calc_complements(matrix_t *A, matrix_t *result);

/**
 * @brief Определитель матрицы
 * @param A указатель на матрицу
 * @param result указатель на число определитель
 * @return код выполнения
 */
int s21_determinant(matrix_t *A, double *result);

/**
 * @brief Обратная матрица
 * @param A указатель на матрицу
 * @param result указатель на матрицу результат
 * @return код выполнения
 */
int s21_inverse_matrix(matrix_t *A, matrix_t *result);

#endif
