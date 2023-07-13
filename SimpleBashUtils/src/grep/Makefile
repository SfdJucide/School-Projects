FLAGS= -Wall -Wextra -Werror -std=c11

all: s21_grep

s21_grep: main.c grep.c grep.h
	gcc ${FLAGS} -c grep.c main.c
	gcc *.o -o s21_grep
	gcc *.o -o test_grep/s21_grep

test: s21_grep
	cd test_grep && ./test_func_grep.sh
	cd ../

clean:
	rm -rf *.o s21_grep test_cat/s21_grep