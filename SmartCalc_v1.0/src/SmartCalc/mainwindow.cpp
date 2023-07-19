#include "mainwindow.h"

#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  ui->widget->addGraph();

  connect(ui->pushButton_0, SIGNAL(clicked()), this, SLOT(digit_numbers()));
  connect(ui->pushButton_1, SIGNAL(clicked()), this, SLOT(digit_numbers()));
  connect(ui->pushButton_2, SIGNAL(clicked()), this, SLOT(digit_numbers()));
  connect(ui->pushButton_3, SIGNAL(clicked()), this, SLOT(digit_numbers()));
  connect(ui->pushButton_4, SIGNAL(clicked()), this, SLOT(digit_numbers()));
  connect(ui->pushButton_5, SIGNAL(clicked()), this, SLOT(digit_numbers()));
  connect(ui->pushButton_6, SIGNAL(clicked()), this, SLOT(digit_numbers()));
  connect(ui->pushButton_7, SIGNAL(clicked()), this, SLOT(digit_numbers()));
  connect(ui->pushButton_8, SIGNAL(clicked()), this, SLOT(digit_numbers()));
  connect(ui->pushButton_9, SIGNAL(clicked()), this, SLOT(digit_numbers()));

  connect(ui->pushButton_X, SIGNAL(clicked()), this, SLOT(X()));

  connect(ui->pushButton_dot, SIGNAL(clicked()), this, SLOT(dot()));

  connect(ui->pushButton_div, SIGNAL(clicked()), this, SLOT(math_operations()));
  connect(ui->pushButton_mul, SIGNAL(clicked()), this, SLOT(math_operations()));
  connect(ui->pushButton_sum, SIGNAL(clicked()), this, SLOT(math_operations()));
  connect(ui->pushButton_sub, SIGNAL(clicked()), this, SLOT(math_operations()));
  connect(ui->pushButton_mod, SIGNAL(clicked()), this, SLOT(math_operations()));
  connect(ui->pushButton_scale, SIGNAL(clicked()), this,
          SLOT(math_operations()));

  connect(ui->pushButton_cos, SIGNAL(clicked()), this,
          SLOT(trigeometry_operations()));
  connect(ui->pushButton_sin, SIGNAL(clicked()), this,
          SLOT(trigeometry_operations()));
  connect(ui->pushButton_tan, SIGNAL(clicked()), this,
          SLOT(trigeometry_operations()));
  connect(ui->pushButton_acos, SIGNAL(clicked()), this,
          SLOT(trigeometry_operations()));
  connect(ui->pushButton_asin, SIGNAL(clicked()), this,
          SLOT(trigeometry_operations()));
  connect(ui->pushButton_atan, SIGNAL(clicked()), this,
          SLOT(trigeometry_operations()));
  connect(ui->pushButton_ln, SIGNAL(clicked()), this,
          SLOT(trigeometry_operations()));
  connect(ui->pushButton_log, SIGNAL(clicked()), this,
          SLOT(trigeometry_operations()));
  connect(ui->pushButton_sqrt, SIGNAL(clicked()), this,
          SLOT(trigeometry_operations()));

  connect(ui->pushButton_backspace, SIGNAL(clicked()), this, SLOT(backspace()));
  connect(ui->pushButton_clear, SIGNAL(clicked()), this, SLOT(clear()));

  connect(ui->pushButton_opened_brace, SIGNAL(clicked()), this, SLOT(braces()));
  connect(ui->pushButton_closed_brace, SIGNAL(clicked()), this, SLOT(braces()));

  connect(ui->pushButton_result, SIGNAL(clicked()), this, SLOT(calc_result()));

  connect(ui->pushButton_build_graph, SIGNAL(clicked()), this,
          SLOT(build_graph()));

  connect(ui->pushButton_calc_credit, SIGNAL(clicked()), this,
          SLOT(credit_calc()));
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::digit_numbers() {
  QPushButton *button = (QPushButton *)(sender());
  if (ui->label->text() != "0") {
    ui->label->setText(ui->label->text() + button->text());
  } else {
    ui->label->setText(button->text());
  }
}

void MainWindow::X() {
  QPushButton *button = (QPushButton *)(sender());
  is_dot_avaliable = false;
  isClickedX = true;
  if (ui->label->text() != "0") {
    ui->label->setText(ui->label->text() + button->text());
  } else {
    ui->label->setText(button->text());
  }
}

void MainWindow::dot() {
  QPushButton *button = (QPushButton *)(sender());
  if (is_dot_avaliable) {
    bool zero = false;
    QString array[] = {"+", "-", "/", "*", "%", "^", "("};
    for (int i = 0; i < 8; i++) {
      if (ui->label->text().endsWith(array[i])) {
        zero = true;
        break;
      }
    }
    if (zero) {
      ui->label->setText(ui->label->text() + "0" + button->text());
    } else {
      ui->label->setText(ui->label->text() + button->text());
    }
    is_dot_avaliable = false;
  }
}

void MainWindow::clear() {
  is_dot_avaliable = true;
  isClickedX = false;
  ui->label->setText("0");
}

void MainWindow::backspace() {
  if (ui->label->text() != "0") {
    if (ui->label->text().endsWith(".")) {
      is_dot_avaliable = true;
    }
    if (ui->label->text().endsWith("X")) {
      isClickedX = false;
    }
    if (ui->label->text().length() == 1) {
      ui->label->setText(ui->label->text().chopped(1));
      ui->label->setText("0");
    } else {
      ui->label->setText(ui->label->text().chopped(1));
    }
  }
}

void MainWindow::braces() {
  QPushButton *button = (QPushButton *)(sender());
  if (button->text() == ")") {
    is_dot_avaliable = false;
  }
  if (ui->label->text() == "0") {
    ui->label->setText(button->text());
  } else {
    ui->label->setText(ui->label->text() + button->text());
  }
}

void MainWindow::math_operations() {
  QPushButton *button = (QPushButton *)(sender());
  is_dot_avaliable = true;
  bool is_operator_avaliable = true;
  QString array[] = {"+", "-", "/", "*", "%", ".", "^", "("};
  for (int i = 0; i < 8; i++) {
    if (ui->label->text().endsWith(array[i])) {
      is_operator_avaliable = false;
      break;
    }
  }
  if (is_operator_avaliable ||
      (ui->label->text().endsWith("(") &&
       (button->text() == "-" || button->text() == "+"))) {
    if (button->text() == "^") {
      ui->label->setText(ui->label->text() + button->text() + "(");
    } else if (button->text() == "÷") {
      ui->label->setText(ui->label->text() + "/");
    } else if (button->text() == "×") {
      ui->label->setText(ui->label->text() + "*");
    } else {
      if (ui->label->text() == "0" &&
          (button->text() == "-" || button->text() == "+")) {
        ui->label->setText(button->text());
      } else {
        ui->label->setText(ui->label->text() + button->text());
      }
    }
  }
}

void MainWindow::trigeometry_operations() {
  QPushButton *button = (QPushButton *)(sender());
  if (ui->label->text() == "0") {
    if (button->text() == "√") {
      ui->label->setText("sqrt(");
    } else {
      ui->label->setText(button->text() + "(");
    }
  } else {
    if (button->text() == "√") {
      ui->label->setText(ui->label->text() + "sqrt(");
    } else {
      ui->label->setText(ui->label->text() + button->text() + "(");
    }
  }
}

void MainWindow::calc_result() {
  QString str = ui->label->text();
  if (isClickedX) {
    std::string X = std::to_string(ui->spinBox_x->value());
    std::string input_real_string = "";
    input_real_string = str.toStdString();
    size_t pos = 0;
    for (;;) {
      pos = input_real_string.find("X", pos);
      if (pos == std::string::npos) break;
      input_real_string.replace(pos, 1, X);
      pos += X.size();
    }
    str = QString::fromStdString(input_real_string);
  }
  QByteArray str_bit = str.toLocal8Bit();
  char *input_str = str_bit.data();

  long double result;
  if (!calc(input_str, &result)) {
    QString str_output = QString::number(result, 'g', 7);
    if (str_output == "nan") {
      ui->label->setText("Error");
    } else {
      ui->label->setText(str_output);
    }
  } else {
    ui->label->setText("Error");
  }
}

void MainWindow::build_graph() {
  QString str = ui->label->text();
  if (str.contains("X")) {
    ui->widget->graph(0)->data()->clear();
    x.clear();
    y.clear();

    double min_x = ui->spinBox_min->value();
    double max_x = ui->spinBox_max->value();
    double min_y = ui->spinBox_min_Y->value();
    double max_y = ui->spinBox_max_Y->value();

    ui->widget->xAxis->setRange(min_x, max_x);
    ui->widget->yAxis->setRange(min_y, max_y);

    while (min_x < max_x) {
      if (std::fabs(min_x - 0) < 1e-8) {
        min_x = 0;
      }
      QString tmp = str;
      long double y_tmp_value = 0;
      tmp.replace("X", "(" + QString::number(min_x) + ")");
      QByteArray tmp_bit = tmp.toLocal8Bit();
      char *input_tmp = tmp_bit.data();
      x.push_back(min_x);
      calc(input_tmp, &y_tmp_value);
      y.push_back(y_tmp_value);
      min_x += step;
    }

    ui->widget->addGraph();
    ui->widget->graph(0)->setPen(QColor(Qt::black));
    ui->widget->graph(0)->addData(x, y);
    ui->widget->replot();
  } else {
    ui->label->setText("Add X to expression");
  }
}

void MainWindow::credit_calc() {
  long double total_sum = ui->doubleSpinBox_total_sum->value();
  long double percents = ui->doubleSpinBox_percents->value();
  int months = ui->spinBox_months->value();
  bool an = ui->radioButton_ann->isChecked();
  bool diff = ui->radioButton_diff->isChecked();
  int month_num = ui->spinBox_month->value();
  long double total_payment, overpayment, monthly_payment;
  char type = 'n';
  if (an)
    type = 'a';
  else if (diff)
    type = 'd';

  calculate_credit(total_sum, percents, months, type, month_num,
                   &monthly_payment, &overpayment, &total_payment);
  if (type != 'n') {
    ui->textEdit_monthly_payment->setText(
        QString::number(monthly_payment, 'f', 2));
    ui->textEdit_overpayment->setText(QString::number(overpayment, 'f', 2));
    ui->textEdit_total_payment->setText(QString::number(total_payment, 'f', 2));
  }
}

void MainWindow::on_spinBox_months_valueChanged(int arg1) {
  ui->spinBox_month->setMinimum(1);
  ui->spinBox_month->setMaximum(arg1);
}
