#ifndef S21_MATRIX_H
#define S21_MATRIX_H
#include <iostream>
#include <vector>

namespace s21 {
class S21Matrix {
 public:
  S21Matrix();
  S21Matrix(const int rows, const int columns);
  S21Matrix(const int rows, const int columns, std::vector<float> image);
  S21Matrix(S21Matrix const &rhs);
  ~S21Matrix();
  S21Matrix &operator=(S21Matrix const &rhs);
  bool operator==(S21Matrix const &rhs) const;
  float getElement(int row, int column);
  void setElement(int row, int column, float value);
  void print();
  int getRows();
  int getColumns();

 private:
  int rows_cnt;
  int columns_cnt;
  float **matrix;
};

}  // namespace s21
s21::S21Matrix scalarProduct(s21::S21Matrix &a, s21::S21Matrix &b);
float sum(s21::S21Matrix a);
std::vector<float> unpack(s21::S21Matrix &img);


#endif
