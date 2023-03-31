#include "mainwindow.h"
#include "model.hpp"

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  s21::MainWindow w;
  w.show();
  return a.exec();
}
