# Smart-Calc v1.0

## Introduction

In this project I implement an extended version of the usual calculator, which can be found in the standard applications of each operating system in the C programming language using structured programming. 
In addition to basic arithmetic operations such as add/subtract and multiply/divide, calculator have the ability to calculate arithmetic expressions by following the order, 
as well as some mathematical functions (sine, cosine, logarithm, etc.).

Program can even build graphs of given expression!

![calculating-proccess](https://github.com/SfdJucide/School-Projects/blob/master/SmartCalc_v1.0/screenshots/SmartCalc_preview.gif)

And also program provides a special mode "credit calculator".

## How to use

### Installation

To build this project cd src folder and use `make install` command. The application is built in the folder s21_calc.
Then you can open application using `make run` command.

### Test coverage

You can enter `make test` command to run test or you can enter `make gcov_report` to generate report of the coverage of **C**-functions

### Documentation and Archivation

To generate documentation of all project run `make dvi` command, this will open docs in your browser.
Also you can archive the project and use it in your needs.
