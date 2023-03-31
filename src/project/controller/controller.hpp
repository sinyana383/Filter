#ifndef CONTROLLER_HPP
#define CONTROLLER_HPP

#define EDIT_NEGATIVE 1
#define EDIT_TONING 2
#define EDIT_GRAYSCALE 3

#include <QColorDialog>
#include <QImage>
#include <QPixmap>

#include "model.hpp"

QPixmap error(QString &reason_link, QString &&reason, bool &status);
namespace controller {
bool image_validation();

/**
 * @brief контроллер для simple фильтров
 *
 * @param t список параметров для контроллера
 * @return QPixmap
 */
template <std::size_t N, typename Head, typename... Tail>
QPixmap simple(std::tuple<Head &, Tail &...> &&t) {
  QString &reason = std::get<N - 2>(t);
  bool &status = std::get<N - 1>(t);
  auto f = std::get<0>(t);
  if (!model::programData.isValidImage)
    return error(reason, QString("Invalid image."), status);
  QImage image(model::programData.filename);
  if constexpr (N == 4) {
    f(image, std::get<1>(t));
  } else {
    f(image);
  }
  model::programData.resultingImage = QImage(image);
  return QPixmap::fromImage(image);
}

QPixmap convolution(const QString &user_input, QString &reason, bool &status);
QPixmap convolution(const std::vector<float> &filter, QString &reason,
                    bool &status);
void tranferResultingImage(QImage &&img);
}  // namespace controller

#endif
