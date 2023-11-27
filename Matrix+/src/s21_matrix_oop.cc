#include "s21_matrix_oop.h"

S21Matrix::S21Matrix(int rows, int cols)
    : rows_(rows), cols_(cols), matrix_(nullptr) {
  if (rows <= 0 || cols <= 0) {
    throw std::length_error("Неверный размер матрицы!");
  }
  matrix_ = new double*[rows_];
  for (int i = 0; i < rows_; ++i) {
    matrix_[i] = new double[cols_];
  }
}

S21Matrix::S21Matrix(const S21Matrix& other)
    : rows_(other.rows_), cols_(other.cols_) {
  matrix_ = new double*[other.rows_];
  for (int i = 0; i < other.rows_; ++i) {
    matrix_[i] = new double[other.cols_];
    for (int j = 0; j < other.cols_; ++j) {
      matrix_[i][j] = other.matrix_[i][j];
    }
  }
}

S21Matrix& S21Matrix::operator=(const S21Matrix& other) {
  if (this != &other) {
    this->~S21Matrix();

    rows_ = other.rows_;
    cols_ = other.cols_;

    matrix_ = new double*[other.rows_];
    for (int i = 0; i < other.rows_; ++i) {
      matrix_[i] = new double[other.cols_];
      for (int j = 0; j < other.cols_; ++j) {
        matrix_[i][j] = other.matrix_[i][j];
      }
    }
  }
  return *(this);
}

S21Matrix::S21Matrix(S21Matrix&& other) noexcept
    : rows_(other.rows_), cols_(other.cols_), matrix_(other.matrix_) {
  other.matrix_ = nullptr;
  other.rows_ = 0;
  other.cols_ = 0;
}

S21Matrix& S21Matrix::operator=(S21Matrix&& other) noexcept {
  if (this != &other) {
    this->~S21Matrix();

    rows_ = other.rows_;
    cols_ = other.cols_;
    matrix_ = other.matrix_;

    other.rows_ = 0;
    other.cols_ = 0;
    other.matrix_ = nullptr;
  }
  return *(this);
}

S21Matrix::~S21Matrix() {
  if (matrix_) {
    for (int i = 0; i < rows_; ++i) {
      delete[] matrix_[i];
    }
    delete[] matrix_;
  }
}

int S21Matrix::GetRows() const noexcept { return this->rows_; }

int S21Matrix::GetCols() const noexcept { return this->cols_; }

void S21Matrix::SetRows(int value) { ResizeMatrix(value, cols_); }

void S21Matrix::SetCols(int value) { ResizeMatrix(rows_, value); }

void S21Matrix::ResizeMatrix(int new_rows, int new_cols) {
  if (new_rows <= 0 || new_cols <= 0) {
    throw std::length_error("Размеры матрицы должны быть больше нуля!");
  }

  double** new_matrix = new double*[new_rows];
  for (int i = 0; i < new_rows; ++i) {
    new_matrix[i] = new double[new_cols];
  }

  for (int i = 0; i < std::min(rows_, new_rows); ++i) {
    for (int j = 0; j < std::min(cols_, new_cols); ++j) {
      new_matrix[i][j] = matrix_[i][j];
    }
  }

  if (rows_ < new_rows) {
    for (int i = rows_; i < new_rows; ++i) {
      for (int j = 0; j < new_cols; ++j) {
        new_matrix[i][j] = 0.0;
      }
    }
  } else if (cols_ < new_cols) {
    for (int i = 0; i < new_rows; ++i) {
      for (int j = cols_; j < new_cols; ++j) {
        new_matrix[i][j] = 0.0;
      }
    }
  }

  for (int i = 0; i < rows_; ++i) {
    delete[] matrix_[i];
  }
  delete[] matrix_;
  rows_ = new_rows;
  cols_ = new_cols;
  matrix_ = new_matrix;
}

double& S21Matrix::operator()(int i, int j) {
  if (i < 0 || i >= rows_ || j < 0 || j >= cols_) {
    throw std::out_of_range("Индекс выходит за пределы матрицы");
  }
  return matrix_[i][j];
}

double& S21Matrix::operator()(int i, int j) const {
  if (i < 0 || i >= rows_ || j < 0 || j >= cols_) {
    throw std::out_of_range("Индекс выходит за пределы матрицы");
  }
  return matrix_[i][j];
}

bool S21Matrix::EqMatrix(const S21Matrix& other) const {
  bool equality = true;
  if (rows_ == other.rows_ && cols_ == other.cols_) {
    for (int i = 0; i < this->rows_; ++i) {
      for (int j = 0; j < this->cols_; ++j) {
        if (std::abs((*this)(i, j) - other(i, j)) > kEps) {
          equality = false;
        }
      }
    }
  } else {
    equality = false;
  }
  return equality;
}

bool S21Matrix::operator==(const S21Matrix& other) const {
  return (*this).EqMatrix(other);
}

void S21Matrix::SumMatrix(const S21Matrix& other) {
  if (rows_ != other.rows_ || cols_ != other.cols_) {
    throw std::length_error("Различная размерность матриц!");
  }
  for (int i = 0; i < this->rows_; ++i) {
    for (int j = 0; j < this->cols_; ++j) {
      (*this)(i, j) = (*this)(i, j) + other(i, j);
    }
  }
}

S21Matrix S21Matrix::operator+(const S21Matrix& other) const {
  S21Matrix result(*this);
  result.SumMatrix(other);
  return result;
}

S21Matrix& S21Matrix::operator+=(const S21Matrix& other) {
  (*this).SumMatrix(other);
  return (*this);
}

void S21Matrix::SubMatrix(const S21Matrix& other) {
  if (rows_ != other.rows_ || cols_ != other.cols_) {
    throw std::length_error("Различная размерность матриц!");
  }
  for (int i = 0; i < this->rows_; ++i) {
    for (int j = 0; j < this->cols_; ++j) {
      (*this)(i, j) = (*this)(i, j) - other(i, j);
    }
  }
}

S21Matrix S21Matrix::operator-(const S21Matrix& other) const {
  S21Matrix result(*this);
  result.SubMatrix(other);
  return result;
}

S21Matrix& S21Matrix::operator-=(const S21Matrix& other) {
  (*this).SubMatrix(other);
  return (*this);
}

void S21Matrix::MulMatrix(const S21Matrix& other) {
  if (cols_ != other.rows_) {
    throw std::length_error(
        "Число столбцов первой матрицы не равно числу строк второй матрицы!");
  }
  double mul;
  S21Matrix tmp(rows_, other.cols_);
  for (int i = 0; i < this->rows_; ++i) {
    for (int j = 0; j < other.cols_; ++j) {
      mul = 0;
      for (int k = 0; k < this->cols_; ++k) {
        mul += (*this)(i, k) * other(k, j);
      }
      tmp(i, j) = mul;
    }
  }
  (*this) = tmp;
}

S21Matrix S21Matrix::operator*(const S21Matrix& other) const {
  S21Matrix result(*this);
  result.MulMatrix(other);
  return result;
}

S21Matrix& S21Matrix::operator*=(const S21Matrix& other) {
  (*this).MulMatrix(other);
  return (*this);
}

void S21Matrix::MulNumber(const double num) {
  for (int i = 0; i < this->rows_; ++i) {
    for (int j = 0; j < this->cols_; ++j) {
      (*this)(i, j) = (*this)(i, j) * num;
    }
  }
}

S21Matrix S21Matrix::operator*(const double num) const {
  S21Matrix result(*this);
  result.MulNumber(num);
  return result;
}

S21Matrix operator*(double num, S21Matrix& matrix) {
  S21Matrix result(matrix);
  result.MulNumber(num);
  return result;
}

S21Matrix& S21Matrix::operator*=(const double num) {
  (*this).MulNumber(num);
  return (*this);
}

S21Matrix S21Matrix::Transpose() const {
  S21Matrix result_matrix(this->cols_, this->rows_);
  for (int i = 0; i < this->rows_; ++i) {
    for (int j = 0; j < this->cols_; ++j) {
      result_matrix(j, i) = (*this)(i, j);
    }
  }
  return result_matrix;
}

S21Matrix S21Matrix::CalcComplements() const {
  if (rows_ != cols_) {
    throw std::length_error("Матрица не является квадратной!");
  }
  S21Matrix result(rows_, cols_);

  switch (rows_) {
    case 1:
      result(0, 0) = 1.0;
      break;
    default:
      S21Matrix minor(rows_ - 1, cols_ - 1);

      for (int i = 0; i < rows_; ++i) {
        for (int j = 0; j < cols_; ++j) {
          for (int row_index = 0, minor_i = 0; row_index < rows_; ++row_index) {
            if (row_index == i) {
              continue;
            }
            for (int col_index = 0, minor_j = 0; col_index < cols_;
                 ++col_index) {
              if (col_index == j) {
                continue;
              }
              minor(minor_i, minor_j) = (*this)(row_index, col_index);
              ++minor_j;
            }
            ++minor_i;
          }
          result(i, j) = minor.Determinant() * pow(-1.0, (i + j));
        }
      }
      break;
  }
  return result;
}

double S21Matrix::Determinant() const {
  if (rows_ != cols_) {
    throw std::length_error("Матрица не является квадратной!");
  }
  double determinant = 0;
  switch (rows_) {
    case 1:
      determinant = (*this)(0, 0);
      break;
    case 2:
      determinant =
          ((*this)(0, 0) * (*this)(1, 1)) - ((*this)(1, 0) * (*this)(0, 1));
      break;
    default:
      S21Matrix tmp(rows_ - 1, cols_ - 1);

      for (int i = 0; i < cols_; ++i) {
        for (int tmp_row_index = 0, row_index = 1; row_index < rows_;
             ++row_index, ++tmp_row_index) {
          for (int col_index = 0, tmp_col_index = 0; col_index < cols_;
               ++col_index) {
            if (col_index == i) {
              continue;
            }
            tmp(tmp_row_index, tmp_col_index) = (*this)(row_index, col_index);
            ++tmp_col_index;
          }
        }
        determinant += tmp.Determinant() * (*this)(0, i) * pow(-1.0, i);
      }
      break;
  }
  return determinant;
}

S21Matrix S21Matrix::InverseMatrix() const {
  double determinant = (*this).Determinant();
  if (std::abs(determinant) < kEps) {
    throw std::logic_error("Определитель матрицы равен 0!");
  }
  return (*this).CalcComplements().Transpose() * (1.0 / determinant);
}