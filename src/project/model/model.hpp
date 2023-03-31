#ifndef MODEL_HPP
#define MODEL_HPP

#include <QImage>
#include <QPixmap>
#include <QString>
#include <iostream>
#include <string>
#include <vector>

#include "s21_matrix.h"
#define RED 0
#define GREEN 1
#define BLUE 2
#define AVERAGE 'a'
#define LUMA 'l'
#define DISSAT 'd'

s21::S21Matrix addDefaultValues(s21::S21Matrix image, int offset);
s21::S21Matrix getFoldMatrix(s21::S21Matrix &build_matrix, int row_pxl_idx,
                             int col_pxl_idx, int filter_size);
s21::S21Matrix foldExp(s21::S21Matrix image, s21::S21Matrix filter);
std::vector<std::vector<float>> imgToVectors(QImage const &img);
void changeImg(QImage &img, std::vector<std::vector<float>> const &vectorImg);

namespace s21 {
struct ProgramData {
  static inline QImage resultingImage{nullptr};
  static inline bool isValidImage{false};
  static inline QString filename{};
};
}  // namespace s21

namespace model {
static s21::ProgramData programData;
namespace simple {
void grayscale(QImage &img, char type);
void negative(QImage &img);
void toning(QImage &img, QColor tone);
}  // namespace simple

namespace convolution {
QPixmap getResultingImage(const std::vector<float> &filter);
}  // namespace convolution

namespace filter {
static const std::vector<float> emboss{-2, -1, 0, -1, 1, 1, 0, 1, 2};
static const std::vector<float> sharpen{0, -1, 0, -1, 5, -1, 0, -1, 0};
static const std::vector<float> boxBlur{0.111, 0.111, 0.111, 0.111, 0.111,
                                        0.111, 0.111, 0.111, 0.111};
static const std::vector<float> gaussianBlur{1 / 16.0, 2 / 16.0, 1 / 16.0,
                                             2 / 16.0, 4 / 16.0, 2 / 16.0,
                                             1 / 16.0, 2 / 16.0, 1 / 16.0};
static const std::vector<float> leplacianFilter{-1, -1, -1, -1, 8,
                                                -1, -1, -1, -1};
static const std::vector<float> sobelLeft{1, 0, -1, 2, 0, -2, 1, 0, -1};
static std::vector<float> custom;
}  // namespace filter

}  // namespace model

#endif
