FLAGS= -Wall -Wextra -Werror -std=c11
UNAME := $(shell uname)
ifeq ($(UNAME),Linux)
    check_flags= -lcheck -lm -lpthread -lrt -lsubunit
else
	check_flags= -lcheck -lm -lpthread
endif

all: s21_decimal.a

style:
	clang-format -i -style=google *.c
	clang-format -style=google -n *.c

s21_decimal.a: s21_decimal s21_utils s21_checking s21_utils_extra
	ar rc s21_decimal.a *.o
	ranlib s21_decimal.a

s21_decimal: s21_decimal.c s21_decimal.h
	gcc ${FLAGS} -c s21_decimal.c

s21_utils: utils/s21_utils.c utils/s21_utils.h
	gcc ${FLAGS} -c utils/s21_utils.c

s21_utils_extra: utils/s21_extra_utils.c utils/s21_utils.h
	gcc ${FLAGS} -c utils/s21_extra_utils.c

s21_checking: checking/checking.c checking/checking.h
	gcc ${FLAGS} -c checking/checking.c

test: clean s21_decimal.a test.c
	gcc ${FLAGS} -c test.c
	gcc -c -fprofile-arcs -ftest-coverage s21_decimal.c
	gcc -fprofile-arcs -ftest-coverage checking/checking.c utils/s21_extra_utils.c utils/s21_utils.c s21_decimal.c test.o ${check_flags} -o run_tests
	./run_tests

gcov_report: test
	mkdir report
	gcov -f *.gcno
	gcovr -r . --html --html-details -o report/report.html

clean:
	rm -rf *.o *.gcov *.gcno *.gcda test run_tests s21_decimal s21_decimal.a run
