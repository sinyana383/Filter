
#include "model.hpp"

/**
 * @brief s
 *
 * @param message сообщение об ошибке
 * @return int
 */
static int error(std::string const &message) {
  std::cout << message << std::endl;
  return -1;
}

/**
 * @brief Расширяем матрицу добавляя дефолтные значения к границам для
 * применения свертки
 * @param image - исходное изображение конвертированное в S21Matrix
 * @param offset - необходимый отступ с каждой стороны
 * @return S21Matrix - возвращаем исходную матрицу дополненную нулями для
 * корректной работы свертки
 */

s21::S21Matrix addDefaultValues(s21::S21Matrix image, int offset) {
  // вычисляем размер отступов новой матрицы исходя из размера фильтра (исходя
  // из того что ядро фильтра всегда квадратное)
  s21::S21Matrix result = s21::S21Matrix(image.getRows() + (offset * 2),
                                         image.getColumns() + (offset * 2));
  int l = 0;
  int k = 0;
  for (int i = offset; i <= image.getRows(); i++) {
    for (int j = offset; j <= image.getColumns(); j++) {
      result.setElement(i, j, image.getElement(k, l));
      l++;
    }
    k++;
    l = 0;
  }
  return result;
}

/**
 * @brief Получение матрицы размером ядра фильтра для проведения свертки
 * @param build_matrix - Матрица достроенная дефолтными значениями
 * @param row_pxl_idx - Индекс пикселя строки для применения свертки
 * @param col_pxl_idx - Индекс пикселя столбца для применения свертки
 * @param filter_size - Размер ядра свертки (3х3, 5х5, 7х7 и тд.)
 * @return S21Matrix - Возвращаем матрицу для центра которой применяем свертку
 */

s21::S21Matrix getFoldMatrix(s21::S21Matrix &build_matrix, int row_pxl_idx,
                             int col_pxl_idx, int filter_size) {
  s21::S21Matrix res = s21::S21Matrix(filter_size, filter_size);
  int k = row_pxl_idx;
  int l = col_pxl_idx;
  for (int i = 0; i < res.getRows(); i++) {
    for (int j = 0; j < res.getColumns(); j++) {
      res.setElement(i, j, build_matrix.getElement(k, l));
      l++;
    }
    k++;
    l = col_pxl_idx;
  }
  return res;
}

/**
 * @brief Функция свертки - на выходе матрица для изображения с примененным
 * фильтром
 * @param image - Исходное изображение конвертированное в S21Matrix
 * @param filter - Ядро свертки
 * @return - Изображение с примененным фильтром
 */

s21::S21Matrix foldExp(s21::S21Matrix image, s21::S21Matrix filter) {
  // размер ядра фильтра нечетный
  int offset = filter.getRows() / 2;
  s21::S21Matrix fold_matrix;
  float foldVal = 0;

  // получаем матрицу исходного изображения и достраиваем ее дефолтными
  // значениями
  s21::S21Matrix build_matrix = addDefaultValues(image, offset);

  // достаем области матрицы для применения свертки к каждому пикселю
  for (int i = 0; i < image.getRows(); i += offset) {
    for (int j = 0; j < image.getColumns(); j += offset) {
      fold_matrix = getFoldMatrix(build_matrix, i, j, filter.getRows());
      // Применяем функцию свертки и меняем значение в build_matrix
      foldVal = sum(scalarProduct(fold_matrix, filter));
      image.setElement(i, j, foldVal);
    }
  }

  return image;
}

/**
 * @brief Конвертация QImage в вектор
 * @param img - Исходное изображение в формате QImage
 * @return вектор <float> для конвертации в S21Matrix
 */

std::vector<std::vector<float>> imgToVectors(QImage const &img) {
  std::vector<std::vector<float>> res(3);

  int width = img.width();
  int height = img.height();
  res[RED].resize(width * height);
  res[GREEN].resize(width * height);
  res[BLUE].resize(width * height);

  for (int p = 0; p < width * height; ++p)
    img.pixelColor(p % width, p / width)
        .getRgbF(&res[RED][p], &res[GREEN][p], &res[BLUE][p]);
  return res;
}

/**
 * @brief - Меняет исходное изображение
 * @param img  - Исходное изображение
 * @param vectorImg - изображение в векторе формат RGB
 */

void changeImg(QImage &img, std::vector<std::vector<float>> const &vectorImg) {
  QColor color;

  for (int x = 0; x < img.width(); ++x) {
    for (int y = 0; y < img.height(); ++y) {
      color = img.pixelColor(x, y);
      color.setRgbF(vectorImg[RED][x + y * img.width()],
                    vectorImg[GREEN][x + y * img.width()],
                    vectorImg[BLUE][x + y * img.width()]);
      img.setPixelColor(x, y, color);
    }
  }
}

using namespace model;

/**
 * @brief - получение финального изображения и передача в контроллер
 * @param filter - ядро свертки
 * @return - результат работы свертки
 */

QPixmap convolution::getResultingImage(const std::vector<float> &filter) {
  QImage img(model::programData.filename);
  std::vector<std::vector<float>> vectorImage = imgToVectors(img);

  s21::S21Matrix imgRED =
      s21::S21Matrix(img.height(), img.width(), vectorImage[RED]);
  s21::S21Matrix imgGREEN =
      s21::S21Matrix(img.height(), img.width(), vectorImage[GREEN]);
  s21::S21Matrix imgBLUE =
      s21::S21Matrix(img.height(), img.width(), vectorImage[BLUE]);
  s21::S21Matrix test = s21::S21Matrix(3, 3, filter);

  s21::S21Matrix resRED = foldExp(imgRED, test);
  s21::S21Matrix resGREEN = foldExp(imgGREEN, test);
  s21::S21Matrix resBLUE = foldExp(imgBLUE, test);

  vectorImage[RED] = unpack(resRED);
  vectorImage[GREEN] = unpack(resGREEN);
  vectorImage[BLUE] = unpack(resBLUE);

  changeImg(img, vectorImage);

  model::programData.resultingImage = QImage(img);
  if (model::programData.resultingImage.isNull())
    std::cerr << "error saving image\n";

  return QPixmap::fromImage(img);
}

/**
 * @brief - базовый фильтр
 * @param img - исходное изображение
 * @param type - тип фильтра
 */

void simple::grayscale(QImage &img, char type) {
  QColor color;
  float r, g, b;
  float c;

  for (int x = 0; x < img.width(); ++x) {
    for (int y = 0; y < img.height(); ++y) {
      color = img.pixelColor(x, y);
      color.getRgbF(&r, &g, &b);

      switch (type) {
        case AVERAGE:
          c = r * 0.333f + g * 0.333f + b * 0.333f;
          break;
        case LUMA:
          c = r * 0.299f + g * 0.587f +
              b * 0.114f;  // немного отличается, видимо, из-за float
          break;
        case DISSAT:
          c = (std::min(std::min(r, g), b) + std::max(std::max(r, g), b)) / 2;
          break;
        default:
          c = r * 0.299f + g * 0.587f + b * 0.114f;
      }
      color.setRgbF(c, c, c);
      img.setPixelColor(x, y, color);
    }
  }
}

/**
 * @brief - базовый фильтр негатив
 * @param img - исходное изображение
 */

void simple::negative(QImage &img) {
  QColor color;
  float r, g, b;

  for (int x = 0; x < img.width(); ++x) {
    for (int y = 0; y < img.height(); ++y) {
      color = img.pixelColor(x, y);
      color.getRgbF(&r, &g, &b);
      color.setRgbF(1.0f - r, 1.0f - g, 1.0f - b);
      img.setPixelColor(x, y, color);
    }
  }
}

/**
 * @brief - базовый фильтр тонирование
 * @param img - исходное изображение
 * @param tone - цвет тонирования
 */

void simple::toning(QImage &img, QColor tone) {
  QColor color;
  float r, g, b;
  float rT, gT, bT;

  grayscale(img, LUMA);
  tone.getRgbF(&rT, &gT, &bT);
  for (int x = 0; x < img.width(); ++x) {
    for (int y = 0; y < img.height(); ++y) {
      color = img.pixelColor(x, y);
      color.getRgbF(&r, &g, &b);
      color.setRgbF(rT * r, gT * g, bT * b);
      img.setPixelColor(x, y, color);
    }
  }
}
