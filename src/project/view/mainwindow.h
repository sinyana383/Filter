#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QApplication>
#include <QFileDialog>
#include <QGraphicsScene>
#include <QImage>
#include <QInputDialog>
#include <QMainWindow>
#include <QMessageBox>
#include <QScrollBar>
#include <QString>
#include <iostream>

#include "controller.hpp"
#include "model.hpp"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

namespace s21 {
class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();
  bool loadFile(const QString &);

 private:
  Ui::MainWindow *ui;
  QImage image;

  void action_routine(const std::vector<float> &filter);

 private slots:
  void on_actionLoad_triggered();
  void on_actionSave_triggered();
  void on_actionClose_triggered();
  void on_actionEmboss_triggered();
  void on_actionSharpen_triggered();
  void on_actionGaussian_Blur_triggered();
  void on_actionBox_Blur_triggered();
  void on_actionLeplacian_Filter_triggered();
  void on_actionPrewwit_Filter_triggered();
  void on_actionCustom_Filter_triggered();
  void on_actionNegative_triggered();
  void on_actionGrayscale_triggered();
  void on_actionToning_triggered();
  void on_loadButton_clicked();
  void on_saveButton_clicked();
  void on_filterBoxBlurButton_clicked();
  void on_filterCustomButton_clicked();
  void on_filterEmbosButton_clicked();
  void on_filterGaussianBlurButton_clicked();
  void on_filterLeplicalButton_clicked();
  void on_filterPrewwitButton_clicked();
  void on_filterSharpenButton_clicked();
};
}  // namespace s21

#endif  // MAINWINDOW_H
