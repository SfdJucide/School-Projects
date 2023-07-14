## Matrix Library
Implementation of the simple matrix.h library.

### Matrix structure in C language:

```c
typedef struct matrix_struct {
    double** matrix;
    int rows;
    int columns;
} matrix_t;
```

## Matrix operations

### Creating matrices (create_matrix)

```c
int s21_create_matrix(int rows, int columns, matrix_t *result);
```

### Cleaning of matrices (remove_matrix)

```c
void s21_remove_matrix(matrix_t *A);
```

### Matrix comparison (eq_matrix)

```c
#define SUCCESS 1
#define FAILURE 0

int s21_eq_matrix(matrix_t *A, matrix_t *B);
```

### Adding (sum_matrix) and subtracting matrices (sub_matrix)

```c
int s21_sum_matrix(matrix_t *A, matrix_t *B, matrix_t *result);
int s21_sub_matrix(matrix_t *A, matrix_t *B, matrix_t *result);
```

### Matrix multiplication by scalar (mult_number). Multiplication of two matrices (mult_matrix)

```c
int s21_mult_number(matrix_t *A, double number, matrix_t *result);
int s21_mult_matrix(matrix_t *A, matrix_t *B, matrix_t *result);
```

### Matrix transpose (transpose)

```c
int s21_transpose(matrix_t *A, matrix_t *result);
```

### Minor of matrix and matrix of algebraic complements (calc_complements)

```c
int s21_calc_complements(matrix_t *A, matrix_t *result);
```

### Matrix determinant

```c
int s21_determinant(matrix_t *A, double *result);
```

### Inverse of the matrix (inverse_matrix)

```c
int s21_inverse_matrix(matrix_t *A, matrix_t *result);
```


## How to use

To compile the project as static library run `make`, now this can be used in other programs as simple libary.

To run matrix tests with check.h run (you need to install check.h before this step)

`make test`

There are a lot of tests for each function, even with failure and errors cases.

You also can create html-report to see covering functions with tests:

`make gcov_report`