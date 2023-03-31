#include "s21_matrix.h"

namespace s21 {
/**
 * @brief - Пустой конструктор для матриц
 */
S21Matrix::S21Matrix() : rows_cnt(0), columns_cnt(0), matrix(nullptr) {}

/**
 * @brief - конструктор для матрицы
 * @param rows - количество строк
 * @param columns - количество столбцов
 */
S21Matrix::S21Matrix(const int rows, const int columns) {
  if (rows < 1 || columns < 1) throw std::out_of_range("wrong matrix size");
  rows_cnt = rows;
  columns_cnt = columns;
  matrix = new float *[rows];
  for (int i = 0; i < rows; ++i) {
    matrix[i] = new float[columns];
  }

  for (int i = 0; i < rows; i++)
    for (int j = 0; j < columns; j++) matrix[i][j] = 0;
}
/**
 * @brief - Конструктор для матрицы с вектором изображеия
 * @param rows - количество строк
 * @param columns - количество столбцов
 * @param image - вектор с изображением
 */
S21Matrix::S21Matrix(const int rows, const int columns,
                     std::vector<float> image) {
  if (rows < 1 || columns < 1) throw std::out_of_range("wrong matrix size");
  rows_cnt = rows;
  columns_cnt = columns;
  matrix = new float *[rows];
  for (int i = 0; i < rows; ++i) {
    matrix[i] = new float[columns];
  }

  for (int i = 0; i < rows; i++)
    for (int j = 0; j < columns; j++) matrix[i][j] = 0;

  for (int i = 0; i < rows; i++)
    for (int j = 0; j < columns; j++) matrix[i][j] = image[j + i * columns];
}

/**
 * @brief - Копирование матрицы
 * @param rhs - экземпляр справа
 */
S21Matrix::S21Matrix(const S21Matrix &rhs) {
  rows_cnt = rhs.rows_cnt;
  columns_cnt = rhs.columns_cnt;
  matrix = new float *[rows_cnt];
  for (int i = 0; i < rows_cnt; i++) matrix[i] = new float[columns_cnt];

  for (int i = 0; i < rows_cnt; i++)
    for (int j = 0; j < columns_cnt; j++) matrix[i][j] = rhs.matrix[i][j];
}

/**
 * @brief - оператор присваивания
 * @param rhs - экземпляр справа
 * @return - вернет скопированную матрицу
 */
S21Matrix &S21Matrix::operator=(const S21Matrix &rhs) {
  if (this != &rhs) {
    for (int i = 0; i < rows_cnt; i++) delete[] matrix[i];
    delete[] matrix;
    rows_cnt = rhs.rows_cnt;
    columns_cnt = rhs.columns_cnt;
    matrix = new float *[rows_cnt];
    for (int i = 0; i < rows_cnt; i++) matrix[i] = new float[columns_cnt];

    for (int i = 0; i < rows_cnt; i++)
      for (int j = 0; j < columns_cnt; j++) matrix[i][j] = rhs.matrix[i][j];
  }
  return *this;
}

/**
 * @brief - переопределение оператора сравнивания
 * @param rhs - экземпляр справа
 * @return вернет true если матрицы равны
 */
bool S21Matrix::operator==(const S21Matrix &rhs) const {
  for (int i = 0; i < rows_cnt; i++)
    for (int j = 0; j < columns_cnt; j++)
      if (matrix[i][j] != rhs.matrix[i][j]) return false;

  return true;
}

/**
 * @brief - деструктор
 */
S21Matrix::~S21Matrix() {
  for (int i = 0; i < rows_cnt; i++) delete[] matrix[i];
  delete[] matrix;
}

/**
 * @brief - Получение значения элемента вектора по индексам
 * @param row - Индекс в строке
 * @param column - Индекс в столбце
 * @return - Вернет значение элемента float
 */
float S21Matrix::getElement(int row, int column) {
  if (matrix == nullptr) throw std::out_of_range("nil ptr");
  return matrix[row][column];
}

/**
 * @brief - Установка значения элемента по индексам
 * @param row - индекс в строке
 * @param column - индекс в столбце
 * @param value - значение которое необходимо установить
 */
void S21Matrix::setElement(int row, int column, float value) {
  if (matrix == nullptr) throw std::out_of_range("nil ptr");
  matrix[row][column] = value;
}

/**
 * @brief - Получение количества строк
 * @return - количество строк
 */
int S21Matrix::getRows() { return rows_cnt; }

/**
 * @brief - Получение количества стролбцов
 * @return - количество столбцов
 */
int S21Matrix::getColumns() { return columns_cnt; }

/**
 * @brief - функция распечатки матрицы (для отладки)
 */
void S21Matrix::print() {
  for (int i = 0; i < rows_cnt; i++) {
    for (int j = 0; j < columns_cnt; j++) {
      std::cout << matrix[i][j] << " ";
    }
    std::cout << "\n";
  }
}
}  // namespace s21

/**
 * @brief - Сумма значений элементов матрицы
 * @param a - Матрица
 * @return - Сумма ее значений float
 */
float sum(s21::S21Matrix a) {
  float res = 0;
  for (int i = 0; i < a.getRows(); i++)
    for (int j = 0; j < a.getColumns(); j++) res += a.getElement(i, j);
  return res;
}

/**
 * @brief - Сколярное произведение матриц
 * @param a - матрица а
 * @param b - матрица б
 * @return - Матрица со значениями сколярного произведения
 */
s21::S21Matrix scalarProduct(s21::S21Matrix &a, s21::S21Matrix &b) {
  if (a.getRows() != b.getRows() || a.getColumns() != b.getColumns())
    throw std::out_of_range("not equals matrix");
  s21::S21Matrix res = s21::S21Matrix(a.getRows(), a.getColumns());
  float value = 0;
  for (int i = 0; i < res.getRows(); i++)
    for (int j = 0; j < res.getColumns(); j++) {
      value = a.getElement(i, j) * b.getElement(i, j);
      res.setElement(i, j, value);
    }
  return res;
}

/**
 * @brief - Получение вектора float из матрицы которая хранит изображение
 * @param img - матрица с изображением
 * @return - вектор float
 */
std::vector<float> unpack(s21::S21Matrix &img) {
  std::vector<float> result(img.getRows() * img.getColumns());
  int idx = 0;

  for (int i = 0; i < img.getRows(); i++) {
    for (int j = 0; j < img.getColumns(); j++) {
      result[idx] = img.getElement(i, j);
      idx++;
    }
  }
  return result;
}