#include "s21_calc.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "s21_stack.h"

int calc(char *input, long double *result) {
  int code = OK;
  // validation
  code = validate_input(input);
  if (!code) {
    // filling res with only lexems
    char **res = calloc(255, sizeof(char *));
    for (int i = 0; i < 255; i++) {
      res[i] = calloc(100, sizeof(char));
    }
    get_lexems(input, res);

    // filling res2 with polish notation
    char **res2 = calloc(255, sizeof(char *));
    for (int i = 0; i < 255; i++) {
      res2[i] = calloc(100, sizeof(char));
    }
    get_polish_notation(res, res2);

    *result = calculate_lexems(res2);

    for (int i = 0; i < 255; i++) {
      free(res[i]);
    }
    free(res);
    for (int i = 0; i < 255; i++) {
      free(res2[i]);
    }
    free(res2);
  }
  return code;
}

void get_polish_notation(char **input, char **result) {
  Stack_sign stack;
  stack.length = 0;

  for (size_t i = 0; input[i][0] != '\0'; i++) {
    // printf("%s ->", input[i]);
    // printStack_sign(&stack);
    if (is_digit(input[i][0]) || input[i][0] == 'x') {
      strcpy(*result, input[i]);
      result++;
    } else if (!strcmp(input[i], "(") || !strcmp(input[i], "s") ||
               !strcmp(input[i], "c") || !strcmp(input[i], "t") ||
               !strcmp(input[i], "S") || !strcmp(input[i], "C") ||
               !strcmp(input[i], "T") || !strcmp(input[i], "q") ||
               !strcmp(input[i], "l") || !strcmp(input[i], "g")) {
      push_sign(&stack, input[i][0]);
    } else if (!strcmp(input[i], ")")) {
      while (stack.data[stack.length - 1] != '(') {
        **result = pop_sign(&stack);
        result++;
      }
      pop_sign(&stack);
      if (stack.length > 0) {
        char func = stack.data[stack.length - 1];
        if (func == 's' || func == 'c' || func == 't' || func == 'l' ||
            func == 'g' || func == 'q' || func == 'S' || func == 'C' ||
            func == 'T') {
          **result = pop_sign(&stack);
          result++;
        }
      }

    } else {
      if (stack.length == 0 || stack.data[stack.length - 1] == '(' ||
          priority(input[i][0]) > priority(stack.data[stack.length - 1]) ||
          (priority(input[i][0]) == priority(stack.data[stack.length - 1]) &&
           stack.data[stack.length - 1] == '%' &&
           (input[i][0] == '*' || input[i][0] == '/'))) {
        push_sign(&stack, input[i][0]);
      } else if (priority(input[i][0]) <=
                 priority(stack.data[stack.length - 1])) {
        do {
          **result = pop_sign(&stack);
          result++;
        } while (
            (priority(input[i][0]) <= priority(stack.data[stack.length - 1]) &&
             !(priority(input[i][0]) ==
                   priority(stack.data[stack.length - 1]) &&
               stack.data[stack.length - 1] == '%' &&
               (input[i][0] == '*' || input[i][0] == '/'))) &&
            stack.length > 0);
        push_sign(&stack, input[i][0]);
      }
    }
  }

  while (stack.length > 0) {
    **result = pop_sign(&stack);
    result++;
  }
}

int is_digit(char symbol) {
  char *nums = "1234567890.";
  return (strchr(nums, symbol) != NULL) ? 1 : 0;
}

int priority(char sign) {
  int priority = 0;
  switch (sign) {
    case '+':
    case '-':
      priority = 2;
      break;
    case '*':
    case '/':
    case '%':
      priority = 3;
      break;
    case '^':
      priority = 4;
      break;
    case 's':
    case 'c':
    case 't':
    case 'S':
    case 'C':
    case 'T':
    case 'q':
    case 'l':
    case 'g':
    case '~':
      priority = 5;
      break;
  }
  return priority;
}

int validate_input(char *input) {
  int code = OK;
  int no_num = 1;
  int count_opened_braces = 0;
  int count_closed_braces = 0;

  switch (input[0]) {
    case '*':
    case '/':
    case ')':
    case '^':
    case '%':
      code = ERROR;
      break;
  }
  switch (input[strlen(input) - 1]) {
    case '+':
    case '-':
    case '*':
    case '/':
    case '(':
    case '^':
    case '%':
      code = ERROR;
      break;
  }
  for (size_t i = 0; i < strlen(input); i++) {
    if (input[i] == '(') {
      count_opened_braces++;
    }
    if (input[i] == ')') {
      count_closed_braces++;
    }
    if (is_digit(input[i]) || input[i] == 'X') {
      no_num = 0;
    }
  }
  if (count_opened_braces != count_closed_braces || no_num) {
    code = ERROR;
  }
  return code;
}

void get_lexems(char *input, char **result) {
  for (size_t i = 0; i < strlen(input); i++) {
    if (input[i] == ' ') {
      continue;
    } else if (strchr("+-", input[i]) != NULL) {
      if (i == 0 || !strcmp(*(--result), "(")) {
        if (i != 0) {
          result++;
        }
        if (input[i] == '+') {
          continue;
        } else {
          strcpy(*result, "~");
          result++;
        }
      } else {
        if (i != 0) {
          result++;
        }
        strncpy(*result, &input[i], 1);
        result++;
      }
    } else if (strchr("^*/()x%%", input[i]) != NULL) {
      strncpy(*result, &input[i], 1);
      result++;
    } else if (input[i] == 'l' && input[i + 1] == 'n') {
      strcpy(*result, "l");
      result++;
      i += 1;
    } else if (input[i] == 'c' && input[i + 1] == 'o' && input[i + 2] == 's') {
      strcpy(*result, "c");
      result++;
      i += 2;
    } else if (input[i] == 's' && input[i + 1] == 'i' && input[i + 2] == 'n') {
      strcpy(*result, "s");
      result++;
      i += 2;
    } else if (input[i] == 't' && input[i + 1] == 'a' && input[i + 2] == 'n') {
      strcpy(*result, "t");
      result++;
      i += 2;
    } else if (input[i] == 'l' && input[i + 1] == 'o' && input[i + 2] == 'g') {
      strcpy(*result, "g");
      result++;
      i += 2;
    } else if (input[i] == 'a' && input[i + 1] == 'c' && input[i + 2] == 'o' &&
               input[i + 3] == 's') {
      strcpy(*result, "C");
      result++;
      i += 3;
    } else if (input[i] == 'a' && input[i + 1] == 's' && input[i + 2] == 'i' &&
               input[i + 3] == 'n') {
      strcpy(*result, "S");
      result++;
      i += 3;
    } else if (input[i] == 'a' && input[i + 1] == 't' && input[i + 2] == 'a' &&
               input[i + 3] == 'n') {
      strcpy(*result, "T");
      result++;
      i += 3;
    } else if (input[i] == 's' && input[i + 1] == 'q' && input[i + 2] == 'r' &&
               input[i + 3] == 't') {
      strcpy(*result, "q");
      result++;
      i += 3;
    } else if (is_digit(input[i])) {
      while (is_digit(input[i])) {
        strncat(*result, &input[i], 1);
        i++;
      }
      result++;
      i--;
    }
  }
}

long double calculate_lexems(char **input) {
  Stack_digit stack;
  stack.length = 0;
  char *endptr;
  long double a, b;

  for (int i = 0; input[i][0] != '\0'; i++) {
    if (is_binary_operation(input[i][0])) {
      a = pop_digit(&stack);
      b = pop_digit(&stack);
      push_digit(&stack, calc_binary(input[i][0], a, b));
    } else if (is_unary_operation(input[i][0])) {
      a = pop_digit(&stack);
      push_digit(&stack, calc_unary(input[i][0], a));
    } else {
      push_digit(&stack, strtold(input[i], &endptr));
    }
  }
  return pop_digit(&stack);
}

long double calc_binary(char operation, long double a, long double b) {
  long double res;
  switch (operation) {
    case '+':
      res = b + a;
      break;
    case '-':
      res = b - a;
      break;
    case '*':
      res = b * a;
      break;
    case '/':
      res = b / a;
      break;
    case '^':
      res = pow(b, a);
      break;
    case '%':
      res = (long int)b % (long int)a;
      break;
  }
  return res;
}

long double calc_unary(char operation, long double a) {
  long double res;
  switch (operation) {
    case '~':
      res = a * (-1);
      break;
    case 's':
      res = sin(a);
      break;
    case 'c':
      res = cos(a);
      break;
    case 't':
      res = tan(a);
      break;
    case 'S':
      res = asin(a);
      break;
    case 'C':
      res = acos(a);
      break;
    case 'T':
      res = atan(a);
      break;
    case 'q':
      res = sqrt(a);
      break;
    case 'l':
      res = log(a);
      break;
    case 'g':
      res = log10(a);
      break;
  }
  return res;
}

int is_binary_operation(char symbol) {
  int res = 0;
  switch (symbol) {
    case '+':
    case '-':
    case '*':
    case '/':
    case '%':
    case '^':
      res = 1;
      break;
  }
  return res;
}

int is_unary_operation(char symbol) {
  int res = 0;
  switch (symbol) {
    case '~':
    case 's':
    case 'c':
    case 't':
    case 'l':
    case 'g':
    case 'q':
    case 'S':
    case 'C':
    case 'T':
      res = 1;
      break;
  }
  return res;
}

void calculate_credit(long double total_sum, long double persent, int months,
                      char type, int month_number, long double *monthly_payment,
                      long double *overpayment, long double *total_payment) {
  long double bet, koef;
  long double sd = total_sum / months;
  long double sp = 0;
  long double sp_sum = 0;

  double months_days[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

  switch (type) {
    case 'a':
      bet = persent / 1200;
      koef = bet * powl(1 + bet, months) / (powl(1 + bet, months) - 1);
      *monthly_payment = total_sum * koef;
      *total_payment = *monthly_payment * months;
      *overpayment = *total_payment - total_sum;
      break;

    case 'd':
      *total_payment = 0;
      for (int i = 0; i < months; i++) {
        sp = (total_sum - sp_sum) * (persent / 100) * (months_days[i % 12]) /
             365;
        *total_payment += sp + sd;
        sp_sum += sd;
      }
      sp_sum = 0;
      for (int i = 0; i < month_number; i++) {
        sp = (total_sum - sp_sum) * (persent / 100) * (months_days[i % 12]) /
             365;
        sp_sum += sd;
      }

      *monthly_payment = sd + sp;
      *overpayment = *total_payment - total_sum;
      break;
  }
}