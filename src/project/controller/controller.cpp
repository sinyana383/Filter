#include "controller.hpp"

/**
 * @brief функция ошибок
 *
 * @param reason_link ссылка на ошибку
 * @param reason текст ошибки
 * @param status статус выполнения
 * @return QPixmap
 */
QPixmap error(QString &reason_link, QString &&reason, bool &status) {
  status = false;
  reason_link = reason;
  return QPixmap();
}

/**
 * @brief валидирует изображение
 *
 * @return true, если изображение валидное
 * @return false, если изображение невалидное
 */
bool controller::image_validation() {
  QImage image_test;
  auto is_valid_image = image_test.load(model::programData.filename);
  model::programData.isValidImage =
      !(model::programData.filename.isEmpty() ||
        model::programData.filename.isNull() || !is_valid_image);
  return model::programData.isValidImage;
}

/**
 * @brief контроллер для пользовательского сверточного фильтра
 *
 * @param user_input пользовательский ввод
 * @param reason причина ошибки
 * @param status статус
 * @return QPixmap
 */
QPixmap controller::convolution(const QString &user_input, QString &reason,
                                bool &status) {
  bool ok;
  status = false;
  QStringList stringArray = user_input.split(',', Qt::SkipEmptyParts);
  QStringList::size_type n = stringArray.size();
  auto squared = QStringList::size_type(sqrt(n));
  if (!model::programData.isValidImage)
    return error(reason, QString("Invalid image or filename."), status);
  if (squared * squared != n || squared < 3 || squared > 15) {
    return error(reason, QString("Invalid size."), status);
  }
  auto custom_filter = std::vector<float>(stringArray.size());
  for (int i = 0; i < n; ++i) {
    custom_filter[i] = stringArray[i].toDouble(&ok);
    if (!ok) return error(reason, QString("Parsing error."), status);
  }
  status = true;
  model::filter::custom = custom_filter;
  return model::convolution::getResultingImage(model::filter::custom);
}

/**
 * @brief контроллер для выбранного фильтра
 *
 * @param filter выбранный фильтр
 * @param reason причина ошибки
 * @param status статус выполнения
 * @return QPixmap
 */
QPixmap controller::convolution(const std::vector<float> &filter,
                                QString &reason, bool &status) {
  if (!model::programData.isValidImage)
    return error(reason, QString("Invalid image."), status);
  return model::convolution::getResultingImage(filter);
}

/**
 * @brief Передача изображения в модель
 * @param img изображение
 */
void controller::tranferResultingImage(QImage &&img) {
  model::programData.resultingImage = img;
}
