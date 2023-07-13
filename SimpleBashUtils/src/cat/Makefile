FLAGS= -Wall -Wextra -Werror -std=c11

all: s21_cat

s21_cat: main.c cat.c cat.h
	gcc ${FLAGS} -c cat.c main.c
	gcc *.o -o s21_cat
	gcc *.o -o test_cat/s21_cat

test: s21_cat
	cd test_cat && ./test_func_cat.sh
	cd ../

clean:
	rm -rf *.o s21_cat test_cat/s21_cat