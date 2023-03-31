#include "mainwindow.h"

#include "./ui_mainwindow.h"

namespace s21 {
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  connect(ui->graphicsViewRight->horizontalScrollBar(),
          SIGNAL(valueChanged(int)),
          ui->graphicsViewLeft->horizontalScrollBar(), SLOT(setValue(int)));
  connect(ui->graphicsViewLeft->horizontalScrollBar(),
          SIGNAL(valueChanged(int)),
          ui->graphicsViewRight->horizontalScrollBar(), SLOT(setValue(int)));
  connect(ui->graphicsViewRight->verticalScrollBar(), SIGNAL(valueChanged(int)),
          ui->graphicsViewLeft->verticalScrollBar(), SLOT(setValue(int)));
  connect(ui->graphicsViewLeft->verticalScrollBar(), SIGNAL(valueChanged(int)),
          ui->graphicsViewRight->verticalScrollBar(), SLOT(setValue(int)));
}

MainWindow::~MainWindow() { delete ui; }

/**
 * @brief триггер для действия Load
 *
 */
void MainWindow::on_actionLoad_triggered() {
  QString filename = QFileDialog::getOpenFileName(
      this, tr("Load Image"), QString(), tr("Images (*.bmp)"));
  if (filename.isEmpty()) return;
  model::programData.filename = filename;
  if (!controller::image_validation()) return;
  QPixmap p(filename);
  if (!ui->graphicsViewLeft->scene()) {
    ui->graphicsViewLeft->setScene(new QGraphicsScene(this));
  }
  if (!ui->graphicsViewRight->scene()) {
    ui->graphicsViewRight->setScene(new QGraphicsScene(this));
  }
  ui->graphicsViewLeft->scene()->clear();
  ui->graphicsViewRight->scene()->clear();
  ui->graphicsViewLeft->setSceneRect(0, 0, p.width(), p.height());
  ui->graphicsViewRight->setSceneRect(0, 0, p.width(), p.height());
  ui->graphicsViewLeft->scene()->addPixmap(p);
  ui->graphicsViewRight->scene()->addPixmap(p);
  controller::tranferResultingImage(QImage(filename));
}

void MainWindow::action_routine(const std::vector<float> &filter) {
  QString reason;
  bool status{true};
  QPixmap qpm = controller::convolution(filter, reason, status);
  if (!status) {
    QMessageBox::warning(this, tr("Error"), reason);
    return;
  } else {
    ui->graphicsViewRight->scene()->addPixmap(qpm);
  }
}

/**
 * @brief триггер для действия Save
 *
 */
void MainWindow::on_actionSave_triggered() {
  auto filename =
      QFileDialog::getSaveFileName(this, tr("Save Image"), "image.bmp");
  if (!model::programData.isValidImage ||
      !model::programData.resultingImage.save(filename)) {
    QMessageBox::warning(this, tr("Error"), tr("Unable to save image."));
    return;
  }
}

/**
 * @brief триггер для действия Close
 *
 */
void MainWindow::on_actionClose_triggered() { QApplication::exit(0); }

/**
 * @brief триггер для действия Emboss
 *
 */
void MainWindow::on_actionEmboss_triggered() {
  action_routine(model::filter::emboss);
}

/**
 * @brief триггер для действия Sharpen
 *
 */
void MainWindow::on_actionSharpen_triggered() {
  action_routine(model::filter::sharpen);
}

/**
 * @brief триггер для действия Box Blur
 *
 */
void MainWindow::on_actionBox_Blur_triggered() {
  action_routine(model::filter::boxBlur);
}

/**
 * @brief триггер для действия Gaussian Blur
 *
 */
void MainWindow::on_actionGaussian_Blur_triggered() {
  action_routine(model::filter::gaussianBlur);
}

/**
 * @brief триггер для действия Leplacian Filter
 *
 */
void MainWindow::on_actionLeplacian_Filter_triggered() {
  action_routine(model::filter::leplacianFilter);
}

/**
 * @brief триггер для действия Prewwit Filter
 *
 */
void MainWindow::on_actionPrewwit_Filter_triggered() {
  action_routine(model::filter::sobelLeft);
}

/**
 * @brief триггер для действия Custom Filter
 *
 */
void MainWindow::on_actionCustom_Filter_triggered() {
  QPixmap qpixmap;
  QString reason;
  bool ok, status;

  QString text = QInputDialog::getText(
      this, tr("Convolution matrix"),
      tr("Enter NxN size comma separated integer array\n where N < 16\n\
      Example: '-1,2,3,1,-2,3,1,2,-3'"),
      QLineEdit::Normal, QString(""), &ok);
  if (!ok) {
    QMessageBox::warning(this, tr("Error"), tr("Parsing error."));
    return;
  } else if (text.isEmpty()) {
    QMessageBox::warning(this, tr("Error"), tr("Empty input."));
    return;
  } else {
    qpixmap = controller::convolution(text, reason, status);
    if (!status) {
      QMessageBox::warning(this, tr("Error"), reason);
      return;
    } else {
      ui->graphicsViewRight->scene()->addPixmap(qpixmap);
    }
  }
  ui->graphicsViewRight->scene()->addPixmap(qpixmap);
}

/**
 * @brief триггер для действия Negative
 *
 */
void MainWindow::on_actionNegative_triggered() {
  QString reason;
  bool status{true};
  QPixmap qpm = controller::simple<3>(
      std::tuple<void (&)(QImage & img), QString &, bool &>{
          model::simple::negative, reason, status});
  if (!status) {
    QMessageBox::warning(this, tr("Error"), reason);
    return;
  }
  ui->graphicsViewRight->scene()->addPixmap(qpm);
}

/**
 * @brief триггер для действия Grayscale
 *
 */
void MainWindow::on_actionGrayscale_triggered() {
  QString reason;
  bool status{true}, ok{false};
  char type_short;
  const QStringList opts{"Average", "Luma", "Dissat"};
  auto type = QInputDialog::getItem(this, "Grayscale", "Grayscale test", opts,
                                    0, false, &ok);
  if (!ok) {
    return;
  }
  type_short = type[0].toLower().toLatin1();
  QPixmap qpm = controller::simple<4>(
      std::tuple<void (&)(QImage &, char), char &, QString &, bool &>{
          model::simple::grayscale, type_short, reason, status});
  if (!status) {
    QMessageBox::warning(this, tr("Error"), reason);
    return;
  }
  ui->graphicsViewRight->scene()->addPixmap(qpm);
}

/**
 * @brief триггер для действия Toning
 *
 */
void MainWindow::on_actionToning_triggered() {
  QString reason;
  bool status{true};
  QColor tone = QColorDialog::getColor();
  if (!tone.isValid()) return;
  QPixmap qpm = controller::simple<4>(
      std::tuple<void (&)(QImage &, QColor), QColor &, QString &, bool &>{
          model::simple::toning, tone, reason, status});
  if (!status) {
    QMessageBox::warning(this, tr("Error"), reason);
    return;
  } else {
    ui->graphicsViewRight->scene()->addPixmap(qpm);
  }
}

/**
 * @brief триггер для кнопки Load
 *
 */
void MainWindow::on_loadButton_clicked() { on_actionLoad_triggered(); }

/**
 * @brief триггер для кнопки Box Blur
 *
 */
void MainWindow::on_filterBoxBlurButton_clicked() {
  on_actionBox_Blur_triggered();
}

/**
 * @brief триггер для кнопки Custom Filter
 *
 */
void MainWindow::on_filterCustomButton_clicked() {
  on_actionCustom_Filter_triggered();
}

/**
 * @brief триггер для кнопки Emboss Button
 *
 */
void MainWindow::on_filterEmbosButton_clicked() { on_actionEmboss_triggered(); }

/**
 * @brief триггер для кнопки Gaussian Blur
 *
 */
void MainWindow::on_filterGaussianBlurButton_clicked() {
  on_actionGaussian_Blur_triggered();
}

/**
 * @brief триггер для кнопки Leplical Filter
 *
 */
void MainWindow::on_filterLeplicalButton_clicked() {
  on_actionLeplacian_Filter_triggered();
}

/**
 * @brief триггер для кнопки Prewwit Filter
 *
 */
void MainWindow::on_filterPrewwitButton_clicked() {
  on_actionPrewwit_Filter_triggered();
}

/**
 * @brief триггер для кнопки Sharpen
 *
 */
void MainWindow::on_filterSharpenButton_clicked() {
  on_actionSharpen_triggered();
}

/**
 * @brief триггер для кнопки Save
 *
 */
void MainWindow::on_saveButton_clicked() { on_actionSave_triggered(); }
}  // namespace s21
