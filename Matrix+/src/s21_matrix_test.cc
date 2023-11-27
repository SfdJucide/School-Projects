#include <gtest/gtest.h>

#include "s21_matrix_oop.h"

TEST(S21MatrixTest, Constructor) {
  S21Matrix M(2, 2);

  ASSERT_EQ(M.GetRows(), 2);
  ASSERT_EQ(M.GetCols(), 2);
  ASSERT_THROW(S21Matrix A(-2, 2), std::length_error);
}

TEST(S21MatrixTest, Constructor2) {
  S21Matrix M(2, 2);
  S21Matrix N(M);

  ASSERT_EQ(N.GetRows(), 2);
  ASSERT_EQ(N.GetCols(), 2);
}

TEST(S21MatrixTest, Move) {
  S21Matrix M(2, 3);

  for (int i = 0; i < M.GetRows(); i++) {
    for (int j = 0; j < M.GetCols(); j++) {
      M(i, j) = i + j;
    }
  }

  S21Matrix copyM = M;
  S21Matrix M2 = std::move(M);

  EXPECT_EQ(copyM == M2, true);
}

TEST(S21MatrixTest, Assignment) {
  S21Matrix M(2, 2);
  M(0, 0) = 1.0;
  M(0, 1) = 0.0;
  M(1, 0) = 0.0;
  M(1, 1) = 1.0;

  S21Matrix N = M;

  EXPECT_EQ(N.GetRows(), 2);
  EXPECT_EQ(N.GetCols(), 2);
  EXPECT_EQ(N(0, 0), 1.0);
  EXPECT_EQ(N(0, 1), 0.0);
  EXPECT_EQ(N(1, 0), 0.0);
  EXPECT_EQ(N(1, 1), 1.0);
}

TEST(S21MatrixTest, Mutator) {
  S21Matrix M(1, 1);
  S21Matrix N(10, 10);

  M.SetRows(5);
  M.SetCols(2);

  N.SetRows(1);

  ASSERT_EQ(M.GetRows(), 5);
  ASSERT_EQ(M.GetCols(), 2);
  ASSERT_EQ(N.GetRows(), 1);
  ASSERT_EQ(N.GetCols(), 10);
  ASSERT_THROW(N.SetCols(-3), std::length_error);
  ASSERT_FLOAT_EQ(M(3, 0), 0.0);
}

TEST(S21MatrixTest, Indexing) {
  S21Matrix M(2, 2);
  const S21Matrix N(2, 2);
  M(0, 0) = 10;
  N(1, 1) = 7;

  ASSERT_EQ(N(1, 1), 7);
  ASSERT_EQ(M(0, 0), 10);
  ASSERT_THROW(N(10, 1), std::out_of_range);
  ASSERT_THROW(M(2, -3), std::out_of_range);
}

TEST(S21MatrixTest, EqMatrix) {
  S21Matrix A(2, 2);
  S21Matrix B(2, 2);
  S21Matrix C(2, 2);
  S21Matrix D(2, 2);
  S21Matrix E(7, 1);

  for (int i = 0; i < A.GetRows(); i++) {
    for (int j = 0; j < A.GetCols(); j++) {
      A(i, j) = 10;
      B(i, j) = 10;
      C(i, j) = 10.00000000000456;
      D(i, j) = 10.65;
    }
  }

  ASSERT_TRUE(A.EqMatrix(B));
  ASSERT_TRUE(A.EqMatrix(C));
  ASSERT_FALSE(A.EqMatrix(D));
  ASSERT_FALSE(A.EqMatrix(E));

  ASSERT_TRUE(A == B);
  ASSERT_TRUE(A == C);
  ASSERT_FALSE(A == D);
  ASSERT_FALSE(A == E);
}

TEST(S21MatrixTest, SumMatrix) {
  S21Matrix A(3, 3);
  S21Matrix B(A);
  S21Matrix C(A);
  S21Matrix D(2, 3);

  S21Matrix ANS1(3, 3);
  S21Matrix ANS2(3, 3);

  for (int i = 0; i < A.GetRows(); i++) {
    for (int j = 0; j < A.GetCols(); j++) {
      A(i, j) = i * j + 1;
      B(i, j) = i * j + 1;
      C(i, j) = 7;
      ANS1(i, j) = 2 * (i * j + 1);
      ANS2(i, j) = 7 + 2 * (i * j + 1);
    }
  }

  ASSERT_TRUE(A + B == ANS1);
  ASSERT_TRUE(A + B + C == ANS2);

  A.SumMatrix(B);
  ASSERT_TRUE(A == ANS1);
  A += C;
  ASSERT_TRUE(A == ANS2);
  ASSERT_THROW(A.SumMatrix(D), std::length_error);
}

TEST(S21MatrixTest, SubMatrix) {
  S21Matrix A(3, 3);
  S21Matrix B(A);
  S21Matrix C(A);
  S21Matrix D(2, 3);

  S21Matrix ANS1(3, 3);
  S21Matrix ANS2(3, 3);

  for (int i = 0; i < A.GetRows(); i++) {
    for (int j = 0; j < A.GetCols(); j++) {
      A(i, j) = i * j + 1;
      B(i, j) = i * j + 1;
      C(i, j) = 7;
      ANS1(i, j) = 0;
      ANS2(i, j) = -7;
    }
  }

  ASSERT_TRUE(A - B == ANS1);
  ASSERT_TRUE(A - (B + C) == ANS2);

  A.SubMatrix(B);
  ASSERT_TRUE(A == ANS1);
  A -= C;
  ASSERT_TRUE(A == ANS2);
  ASSERT_THROW(A.SubMatrix(D), std::length_error);
}

TEST(S21MatrixTest, MulNumber) {
  S21Matrix A(3, 3);
  S21Matrix B(A);

  S21Matrix ANS1(3, 3);
  S21Matrix ANS2(3, 3);

  for (int i = 0; i < A.GetRows(); i++) {
    for (int j = 0; j < A.GetCols(); j++) {
      A(i, j) = 1;
      B(i, j) = 1;
      ANS1(i, j) = 0;
      ANS2(i, j) = -7.78;
    }
  }

  ASSERT_TRUE(A * 0 == ANS1);
  ASSERT_TRUE(-7.78 * A == ANS2);

  A.MulNumber(0);
  ASSERT_TRUE(A == ANS1);
  B *= -7.78;
  ASSERT_TRUE(B == ANS2);
}

TEST(S21MatrixTest, MulMatrix) {
  S21Matrix A(2, 3);
  S21Matrix B(3, 2);
  S21Matrix C(2, 2);
  S21Matrix D(2, 3);

  S21Matrix ANS1(2, 2);
  S21Matrix ANS2(3, 3);
  S21Matrix ANS3(2, 2);

  for (int i = 0; i < A.GetRows(); i++) {
    for (int j = 0; j < A.GetCols(); j++) {
      A(i, j) = i + j + 2;
      B(j, i) = i * j + 1;
    }
  }
  ANS1(0, 0) = 9;
  ANS1(0, 1) = 20;
  ANS1(1, 0) = 12;
  ANS1(1, 1) = 26;

  C(0, 0) = 1;
  C(0, 1) = 2.7;
  C(1, 0) = -3;
  C(1, 1) = 4;

  ANS2(0, 0) = 5;
  ANS2(0, 1) = 7;
  ANS2(0, 2) = 9;
  ANS2(1, 0) = 8;
  ANS2(1, 1) = 11;
  ANS2(1, 2) = 14;
  ANS2(2, 0) = 11;
  ANS2(2, 1) = 15;
  ANS2(2, 2) = 19;

  ANS3(0, 0) = -51;
  ANS3(0, 1) = 104.3;
  ANS3(1, 0) = -66;
  ANS3(1, 1) = 136.4;

  ASSERT_TRUE(A * B == ANS1);
  ASSERT_TRUE(B * A == ANS2);

  A.MulMatrix(B);
  ASSERT_TRUE(A == ANS1);
  A *= C;
  ASSERT_TRUE(A == ANS3);
  ASSERT_THROW(D.MulMatrix(C), std::length_error);
}

TEST(S21MatrixTest, Transpose) {
  S21Matrix A(2, 3);
  S21Matrix B(3, 2);

  A(0, 0) = 87;
  A(0, 1) = 7;
  A(0, 2) = 45;
  A(1, 0) = 2;
  A(1, 1) = -4.7;
  A(1, 2) = 8;

  B(0, 0) = 87;
  B(0, 1) = 2;
  B(1, 0) = 7;
  B(1, 1) = -4.7;
  B(2, 0) = 45;
  B(2, 1) = 8;

  ASSERT_TRUE(A.Transpose() == B);
}

TEST(S21MatrixTest, CalcComplements) {
  S21Matrix A(1, 1);
  S21Matrix C(3, 3);
  S21Matrix E(3, 2);

  S21Matrix ANS1(1, 1);
  S21Matrix ANS2(3, 3);

  A(0, 0) = 87;

  C(0, 0) = 1;
  C(0, 1) = 3;
  C(0, 2) = 7;
  C(1, 0) = 0.34;
  C(1, 1) = 6;
  C(1, 2) = 3;
  C(2, 0) = -23;
  C(2, 1) = 3;
  C(2, 2) = 7.8;

  ANS1(0, 0) = 1.0;

  ANS2(0, 0) = 37.8;
  ANS2(0, 1) = -71.652;
  ANS2(0, 2) = 139.02;
  ANS2(1, 0) = -2.4;
  ANS2(1, 1) = 168.8;
  ANS2(1, 2) = -72;
  ANS2(2, 0) = -33;
  ANS2(2, 1) = -0.62;
  ANS2(2, 2) = 4.98;

  ASSERT_TRUE(A.CalcComplements() == ANS1);
  ASSERT_TRUE(C.CalcComplements() == ANS2);
  ASSERT_THROW(E.CalcComplements(), std::length_error);
}

TEST(S21MatrixTest, Determinant) {
  S21Matrix A(1, 1);
  S21Matrix B(2, 2);
  S21Matrix C(3, 3);
  S21Matrix D(4, 4);
  S21Matrix E(3, 2);

  A(0, 0) = 87;

  B(0, 0) = 87;
  B(0, 1) = 2;
  B(1, 0) = 7;
  B(1, 1) = -4.7;

  C(0, 0) = 1;
  C(0, 1) = 3;
  C(0, 2) = 7;
  C(1, 0) = 0.34;
  C(1, 1) = 3;
  C(1, 2) = 7;
  C(2, 0) = -23;
  C(2, 1) = 3;
  C(2, 2) = 7.8;

  D(0, 0) = 1;
  D(0, 1) = 3;
  D(0, 2) = 7;
  D(0, 3) = 7;
  D(1, 0) = 0.34;
  D(1, 1) = 3;
  D(1, 2) = 7;
  D(1, 3) = 7;
  D(2, 0) = -23;
  D(2, 1) = 3;
  D(2, 2) = 7.8;
  D(2, 3) = -99.8;
  D(3, 0) = 0;
  D(3, 1) = 4;
  D(3, 2) = 0.8;
  D(3, 3) = 1;

  ASSERT_FLOAT_EQ(A.Determinant(), 87);
  ASSERT_FLOAT_EQ(B.Determinant(), -422.9);
  ASSERT_FLOAT_EQ(C.Determinant(), 1.584);
  ASSERT_FLOAT_EQ(D.Determinant(), -1817.6928);
  ASSERT_THROW(E.Determinant(), std::length_error);
}

TEST(S21MatrixTest, InverseMatrix) {
  S21Matrix A(1, 1);
  S21Matrix B(2, 2);
  S21Matrix C(3, 3);
  S21Matrix E(3, 2);

  S21Matrix ANS1(1, 1);
  S21Matrix ANS2(3, 3);

  A(0, 0) = 87;

  B(0, 0) = 3;
  B(0, 1) = 6;
  B(1, 0) = 2;
  B(1, 1) = 4;

  C(0, 0) = 1;
  C(0, 1) = 2;
  C(0, 2) = 3;
  C(1, 0) = 4;
  C(1, 1) = 2;
  C(1, 2) = 6;
  C(2, 0) = 6;
  C(2, 1) = 8;
  C(2, 2) = 10;

  ANS1(0, 0) = 1.0 / 87;

  ANS2(0, 0) = -7. / 6;
  ANS2(0, 1) = 1. / 6;
  ANS2(0, 2) = 1. / 4;
  ANS2(1, 0) = -1. / 6;
  ANS2(1, 1) = -1. / 3;
  ANS2(1, 2) = 1. / 4;
  ANS2(2, 0) = 5. / 6;
  ANS2(2, 1) = 1. / 6;
  ANS2(2, 2) = -1. / 4;

  ASSERT_TRUE(A.InverseMatrix() == ANS1);
  ASSERT_TRUE(C.InverseMatrix() == ANS2);
  ASSERT_THROW(B.InverseMatrix(), std::logic_error);
  ASSERT_THROW(E.InverseMatrix(), std::length_error);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
