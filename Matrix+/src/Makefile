CXX = gcc
CXXFLAGS = -Wall -Wextra -Werror -std=c++17 -fprofile-arcs -ftest-coverage
UNAME := $(shell uname)
ifeq ($(UNAME), Linux)
    LIBS = -lgtest -lgtest_main -pthread -lm -lstdc++
else
	LIBS = -lgtest -lgtest_main -pthread
endif

all: test

check_style:
	cp ../materials/linters/.clang-format .
	clang-format -style=google -n *.cc *.h
	rm .clang-format

s21_matrix_oop.a: s21_matrix_oop.o
	ar rcs s21_matrix_oop.a s21_matrix_oop.o

s21_matrix_oop.o: s21_matrix_oop.cc s21_matrix_oop.h
	$(CXX) $(CXXFLAGS) -c s21_matrix_oop.cc -o s21_matrix_oop.o

test: s21_matrix_oop.a s21_matrix_test.o s21_matrix_oop.o
	$(CXX) $(CXXFLAGS) s21_matrix_test.o s21_matrix_oop.o $(LIBS) -o test
	./test

s21_matrix_test.o: s21_matrix_test.cc s21_matrix_oop.h
	$(CXX) $(CXXFLAGS) -c s21_matrix_test.cc -o s21_matrix_test.o

valgrind: test
	valgrind --tool=memcheck --leak-check=yes ./test

gcov_report: test
	mkdir -p report
	gcovr -r . -e s21_matrix_test.cc --html --html-details -o report/report.html

clean:
	rm -rf *.o *.gcov *.gcno *.gcda test s21_matrix_oop s21_matrix_oop.a report