FLAGS=-std=c11 -Wall -Wextra -Werror
TEST_FILE_NAME=SmartCalc/test.c
GCOV_FLAGS=-fprofile-arcs -ftest-coverage
UNAME := $(shell uname)
ifeq ($(UNAME),Linux)
    TEST_LIBS= -lcheck -lm -lpthread -lrt -lsubunit
else
	TEST_LIBS= -lcheck -lm -lpthread
endif

ARCHIVE=SmartCalc_v1.0

SOURCES=SmartCalc/s21_calc.c SmartCalc/s21_stack.c
OBJECTS=$(SOURCES:.c=.o)

all: install

run:
	open -Fn s21_calc/SmartCalc.app

install: build
	cd s21_calc && $(MAKE)

uninstall:
	rm -rf s21_calc

build: uninstall
	mkdir s21_calc
	cd SmartCalc && cmake -S ./ -B ../s21_calc

dist: install
	@tar -cf ${ARCHIVE}.tar s21_calc/*

dvi:
	rm -rf docs
	mkdir -p docs
	doxygen Doxyfile
	@mv html latex docs
	open docs/html/index.html

dvi_open:
	open docs/html/index.html

test: clean ${TEST_FILE_NAME}
	gcc ${FLAGS} -c ${TEST_FILE_NAME}
	gcc -c ${GCOV_FLAGS} ${SOURCES}
	gcc ${GCOV_FLAGS} ${SOURCES} test.o ${TEST_LIBS} -o run_tests
	./run_tests

gcov_report: test
	mkdir -p report
	gcov -f *.gcno
	gcovr -r . --html --html-details -o report/report.html
	rm -rf *.gcov *.gcno *.gcda
	open report/report.html

clean:
	rm -rf *.o *.gcov *.gcno *.gcda *.tar test run_tests .DS_Store

style:
	clang-format -style=google -n SmartCalc/*.cpp SmartCalc/*.c
