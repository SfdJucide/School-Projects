#ifndef SRC_S21_MATRIX_OOP_H_
#define SRC_S21_MATRIX_OOP_H_

#include <cmath>
#include <iostream>

const double kEps = 1e-8;

class S21Matrix {
 public:
  S21Matrix() = default;
  S21Matrix(int rows, int cols);

  S21Matrix(const S21Matrix& other);
  S21Matrix& operator=(const S21Matrix& other);

  S21Matrix(S21Matrix&& other) noexcept;
  S21Matrix& operator=(S21Matrix&& other) noexcept;

  ~S21Matrix();

  int GetRows() const noexcept;
  int GetCols() const noexcept;

  void SetRows(int value);
  void SetCols(int value);

  double& operator()(int i, int j);
  double& operator()(int i, int j) const;

  bool EqMatrix(const S21Matrix& other) const;
  bool operator==(const S21Matrix& other) const;

  void SumMatrix(const S21Matrix& other);
  S21Matrix operator+(const S21Matrix& other) const;
  S21Matrix& operator+=(const S21Matrix& other);

  void SubMatrix(const S21Matrix& other);
  S21Matrix operator-(const S21Matrix& other) const;
  S21Matrix& operator-=(const S21Matrix& other);

  void MulMatrix(const S21Matrix& other);
  S21Matrix operator*(const S21Matrix& other) const;
  S21Matrix& operator*=(const S21Matrix& other);

  void MulNumber(const double num);
  S21Matrix operator*(const double num) const;
  friend S21Matrix operator*(double num, S21Matrix& matrix);
  S21Matrix& operator*=(const double num);

  S21Matrix Transpose() const;
  S21Matrix CalcComplements() const;
  double Determinant() const;
  S21Matrix InverseMatrix() const;

 private:
  int rows_ = 0, cols_ = 0;
  double** matrix_ = nullptr;
  void ResizeMatrix(int new_rows, int new_cols);
};

#endif  // SRC_S21_MATRIX_OOP_H_