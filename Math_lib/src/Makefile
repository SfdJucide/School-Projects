FLAGS= -Wall -Wextra -Werror -std=c11
UNAME := $(shell uname)
ifeq ($(UNAME),Linux)
    check_flags= -lcheck -lm -lpthread -lrt -lsubunit
else
	check_flags= -lcheck -lm -lpthread
endif

all: s21_math.a

style:
	clang-format -i *.c
	clang-format -n *.c

s21_math.a: s21_math
	ar rc s21_math.a s21_*.o
	ranlib s21_math.a

s21_math: s21_math.c s21_math.h
	gcc ${FLAGS} -c s21_math.c

test: s21_math.a test.c
	gcc ${FLAGS} -c test.c
	gcc -c -fprofile-arcs -ftest-coverage s21_math.c
	gcc -fprofile-arcs -ftest-coverage s21_math.c test.o ${check_flags} -o run_tests
	./run_tests

gcov_report: test
	./run_tests
	mkdir -p report
	gcov -f *.gcno
	gcovr -r . --html --html-details -o report/report.html

clean:
	rm -rf *.o *.gcov *.gcno *.gcda test run_tests s21_math s21_math.a
