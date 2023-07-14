FLAGS= -Wall -Wextra -Werror -std=c11
UNAME := $(shell uname)
ifeq ($(UNAME),Linux)
    check_flags= -lcheck -lm -lpthread -lrt -lsubunit
else
	check_flags= -lcheck -lm -lpthread
endif


all: s21_matrix.a

style:
	clang-format -style=google -n *.c

s21_matrix.a: s21_matrix
	ar rc s21_matrix.a *.o
	ranlib s21_matrix.a

s21_matrix: s21_matrix.c s21_matrix.h
	gcc ${FLAGS} -c s21_matrix.c

test: clean s21_matrix.a test.c
	gcc ${FLAGS} -c test.c
	gcc -c -fprofile-arcs -ftest-coverage s21_matrix.c
	gcc -fprofile-arcs -ftest-coverage s21_matrix.c test.o ${check_flags} -o run_tests
	./run_tests

gcov_report: test
	mkdir -p report
	gcov -f *.gcno
	gcovr -r . --html --html-details -o report/report.html

clean:
	rm -rf *.o *.gcov *.gcno *.gcda test run_tests s21_matrix s21_matrix.a run
