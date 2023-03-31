#include <gtest/gtest.h>

#include "../model/model.hpp"
#include "../model/s21_matrix.h"

class kernelFixture : public ::testing::Test {
 protected:
  virtual void TearDown() override {
    delete img;
    delete filter;
  }
  void SetUp() override {
    img = new s21::S21Matrix();
    filter = new s21::S21Matrix();
  }
  s21::S21Matrix *img;
  s21::S21Matrix *filter;
  // Тестовая матрица 1
  std::vector<float> imgArr{25, 80, 110, 5,  80, 100, 120, 7,
                            92, 60, 30,  25, 93, 97,  23,  6};
  // Тестовое ядро 3х3
  std::vector<float> kernel3{0, -1, 0, -1, 5, -1, 0, -1, 0};
  // Тестовый офсет
  int offset = 1;
};

// Достройка матрицы изображения нулями по краям
TEST_F(kernelFixture, defValuesTest) {
  std::cout << "\033[0;38;5;220mSTART DEFAULT VALUE MATRIX TEST\033[0m"
            << std::endl;

  *img = s21::S21Matrix(4, 4, imgArr);
  s21::S21Matrix def = s21::S21Matrix(4 + (offset * 2), 4 + (offset * 2));
  def = addDefaultValues(*img, offset);

  EXPECT_EQ(def.getColumns(), def.getRows());
  EXPECT_EQ(def.getColumns(), img->getColumns() + (offset * 2));
  EXPECT_EQ(def.getRows(), img->getRows() + (offset * 2));

  std::cout << "\033[0;38;5;42mCHECK DEFAULT VALUES\033[0;38;5;203m"
            << std::endl;
  // Проверим угловые дефолтные значения новой матрицы
  // значение верхнего левого угла матрицы равно 0
  EXPECT_EQ(def.getElement(0, 0), 0);
  // значение верхнего правого угла матрицы равно 0
  EXPECT_EQ(def.getElement(0, def.getColumns() - 1), 0);
  // значение нижнего левого угла матрицы равно 0
  EXPECT_EQ(def.getElement(def.getRows() - 1, 0), 0);
  // значение нижнего правого угла матрицы равно 0
  EXPECT_EQ(def.getElement(def.getRows() - 1, def.getColumns() - 1), 0);

  std::cout << "\033[0;38;5;42mCHECK VALUES IN MATRIX\033[0;38;5;203m"
            << std::endl;
  // Проверим угловые(не дефолтные) значения матрицы
  // значение верхнего левого угла равно 1
  EXPECT_EQ(def.getElement(offset, offset), 25);
  // значение верхнего правого угла равно 1
  EXPECT_EQ(def.getElement(offset, def.getColumns() - offset - 1), 5);
  // значение нижнего левого угла равно 1
  EXPECT_EQ(def.getElement((def.getRows() - 1) - offset, offset), 93);
  // значение нижнего правого угла равно 1
  EXPECT_EQ(def.getElement((def.getRows() - 1) - offset,
                           (def.getColumns() - 1) - offset),
            6);
  std::cout << "\033[0;38;5;220mFINISH DEFAULT VALUE MATRIX TEST\n\033[0m"
            << std::endl;
}

TEST_F(kernelFixture, getFoldMatrixTest) {
  std::cout << "\033[0;38;5;220mSTART GET FOLD MATRIX TEST\033[0m" << std::endl;

  *img = s21::S21Matrix(4, 4, imgArr);
  // создание матрицы с дефолтными значениями
  s21::S21Matrix def = s21::S21Matrix(4 + (offset * 2), 4 + (offset * 2));
  def = addDefaultValues(*img, offset);

  std::cout << "\033[0;38;5;42mGET VALUES FROM SRC MATRIX\033[0;38;5;203m"
            << std::endl;
  // получение исходной матрицы из матрицы дополненной нулями
  s21::S21Matrix result = getFoldMatrix(def, offset, offset, 3);

  std::cout << "\033[0;38;5;42mCOMPARE RESULTS\033[0;38;5;203m" << std::endl;
  // сравнение матриц
  EXPECT_TRUE(result == *img);

  std::cout << "\033[0;38;5;220mFINISH GET FOLD MATRIX TEST\n\033[0m\n"
            << std::endl;
}

TEST_F(kernelFixture, foldExpTest) {
  std::cout << "\033[0;38;5;220mSTART FOLD EXPRESSION TEST\033[0m" << std::endl;

  *img = s21::S21Matrix(4, 4, imgArr);
  *filter = s21::S21Matrix(3, 3, kernel3);
  std::vector<float> check{-35, 165, 345, -92, 183, 160, 353, -115,
                           227, -19, -78, 82,  276, 309, -18, -18};
  s21::S21Matrix checkMtrx = s21::S21Matrix(4, 4, check);

  std::cout << "\033[0;38;5;42mRUN FOLD FUNCTION\033[0;38;5;203m" << std::endl;

  s21::S21Matrix result = foldExp(*img, *filter);

  std::cout << "\033[0;38;5;42mCHECK FOLD EXPRESSION RESULT\033[0;38;5;203m"
            << std::endl;

  EXPECT_TRUE(result == checkMtrx);

  std::cout << "\033[0;38;5;220mFINISH FOLD EXPRESSION TEST\n\033[0m\n"
            << std::endl;
}
