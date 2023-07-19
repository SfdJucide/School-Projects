#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>

extern "C" {
#include "s21_calc.h"
}

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

 private:
  Ui::MainWindow *ui;

  bool is_dot_avaliable = true;
  bool isClickedX = false;
  QString space = "";
  double step = 0.01;
  QVector<double> x, y;

 private slots:
  void digit_numbers();
  void math_operations();
  void trigeometry_operations();
  void dot();
  void clear();
  void backspace();
  void braces();
  void calc_result();
  void X();
  void build_graph();
  void credit_calc();
  void on_spinBox_months_valueChanged(int arg1);
};
#endif  // MAINWINDOW_H
