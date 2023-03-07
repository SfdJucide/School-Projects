FLAGS= -Wall -Wextra -Werror
STD= -std=c11

all: s21_cat

s21_cat: cat.c
	gcc ${FLAGS} ${STD} -c cat.c
	gcc *.o -o s21_cat

clean:
	rm -rf *.o s21_cat