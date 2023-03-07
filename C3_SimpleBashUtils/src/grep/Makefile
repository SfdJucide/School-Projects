FLAGS= -Wall -Wextra -Werror
STD= -std=c11
all: s21_grep

s21_grep: s21_grep.c
	gcc ${FLAGS} ${STD} -c s21_grep.c
	gcc *.o -o s21_grep

clean:
	rm -rf *.o s21_grep