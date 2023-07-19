#ifndef SRC_S21_CALC_H_
#define SRC_S21_CALC_H_

/**
 * @brief Коды ошибок
 */
enum Codes {
  OK,
  ERROR,
};

/**
 * @brief Преобразование инфиксной записи в польскую нотацию
 * @param input выражение в инфиксной записи
 * @param result выражение в польской нотации
 */
void get_polish_notation(char **input, char **result);

/**
 * @brief Проверка является ли символ числом
 * @param symbol символ
 * @return 1 - если символ является числом, иначе - 0
 */
int is_digit(char symbol);

/**
 * @brief Вычисление приоритета операции
 * @param sign символ операции
 * @return Приоритет операции
 */
int priority(char sign);

/**
 * @brief Валидация входного выражения
 * @param input входная строка
 * @return код ошибки
 */
int validate_input(char *input);

/**
 * @brief Преобработка входного выражения
 * @param input входная строка
 * @param result выходная строка
 */
void get_lexems(char *input, char **result);

/**
 * @brief Вычисление выражения в польской нотации
 * @param input выражения в польской нотации
 * @return результат выражения
 */
long double calculate_lexems(char **input);

/**
 * @brief Проверка является ли символ бинарной операцией
 * @param symbol символ операции
 * @return 1 - если символ является бинарной операцией, иначе - 0
 */
int is_binary_operation(char symbol);

/**
 * @brief Проверка является ли символ унарной операцией
 * @param symbol символ операции
 * @return 1 - если символ является унарной операцией, иначе - 0
 */
int is_unary_operation(char symbol);

/**
 * @brief Вычисление бинарной операции между 2 числами
 * @param operation символ операции
 * @param a первое число
 * @param b второе число
 * @return результат операции
 */
long double calc_binary(char operation, long double a, long double b);

/**
 * @brief Вычисление унарной операции между 2 числами
 * @param operation символ операции
 * @param a число
 * @return результат операции
 */
long double calc_unary(char operation, long double a);

/**
 * @brief Вычисление выражения
 * @param input выражение
 * @param result указатель на результат
 * @return код ошибки
 */
int calc(char *input, long double *result);

/**
 * @brief Вычисление параметров кредита
 * @param total_sum общая сумма кредита
 * @param persent процентная ставка
 * @param months срок (в месяцах)
 * @param type тип (аннуитетный, дифференцированный)
 * @param month номер месяца
 * @param monthly_payment ежемесячный платеж
 * @param overpayment переплата по кредиту
 * @param total_payment общая выплата
 */
void calculate_credit(long double total_sum, long double persent, int months,
                      char type, int month, long double *monthly_payment,
                      long double *overpayment, long double *total_payment);

#endif