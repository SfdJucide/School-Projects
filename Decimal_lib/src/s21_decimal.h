#ifndef S21_DECIMAL_H_
#define S21_DECIMAL_H_

#define MINUS_SIGN 2147483648  // 32 бит числа
#define TOTAL_BITS 32
#define S21_MAX_UINT 4294967295  // 2 в 32 степени
#define DEFAULT_BASE 65536  // степень числа по умолчанию - 1

#define is_inf(x) __builtin_isinf(x)
#define is_nan(x) __builtin_isnan(x)

typedef enum {
  OK,
  TOO_BIG,
  TOO_SMALL,
  DIVISION_BY_ZERO,
  EXTRA_CODE
} value_type_t;

enum { low, mid, high, scale };

enum Convertation { CONVERTATION_OK, CONVERTATION_ERROR };

typedef struct {
  unsigned int bits[4];
} s21_decimal;

typedef struct {
  unsigned int bits[7];
} s21_big_decimal;

// ФУНКЦИИ АРИФМЕТИКИ

/**
 * @brief Сложение двух чисел децимал
 * @param value_1 число децимал
 * @param value_2 число децимал
 * @param result указатель на результат сложения в децимал
 * @return код ошибки
 */
int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
/**
 * @brief Вычитание двух чисел децимал
 * @param value_1 число децимал
 * @param value_2 число децимал
 * @param result указатель на результат вычитания в децимал
 * @return код ошибки
 */
int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
/**
 * @brief Умножение двух чисел децимал
 * @param value_1 число децимал
 * @param value_2 число децимал
 * @param result указатель на результат умножения в децимал
 * @return код ошибки
 */
int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
/**
 * @brief Деление двух чисел децимал
 * @param value_1 делимое децимал
 * @param value_2 делитель децимал
 * @param result указатель на результат деления в децимал
 * @return код ошибки
 */
int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
/**
 * @brief Остаток от деления двух чисел децимал
 * @param value_1 делимое децимал
 * @param value_2 делитель децимал
 * @param result указатель на результат получения остатка от деления в децимал
 * @return код ошибки
 */
int s21_mod(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);

// ФУНКЦИИ СРАВНЕНИЯ

/**
 * @brief Равны ли два числа децимал
 * @param first Первое число децимал
 * @param second Второе число децимал
 * @return 0 - first != second, 1 - first == second
 */
int s21_is_equal(s21_decimal first, s21_decimal second);
/**
 * @brief Не равны ли два числа децимал
 * @param first Первое число децимал
 * @param second Второе число децимал
 * @return 0 - first == second, 1 - first != second
 */
int s21_is_not_equal(s21_decimal first, s21_decimal second);
/**
 * @brief Первое число больше ли второго
 * @param first Первое число децимал
 * @param second Второе число децимал
 * @return 0 - second >= first, 1 - first > second
 */
int s21_is_greater(s21_decimal first, s21_decimal second);
/**
 * @brief Первое число больше или равно ли второго
 * @param first Первое число децимал
 * @param second Второе число децимал
 * @return 0 - second > first, 1 - first >= second
 */
int s21_is_greater_or_equal(s21_decimal first, s21_decimal second);
/**
 * @brief Первое число меньше ли второго
 * @param first Первое число децимал
 * @param second Второе число децимал
 * @return 0 - first >= second , 1 - first < second
 */
int s21_is_less(s21_decimal first, s21_decimal second);
/**
 * @brief Первое число меньше ли второго
 * @param first Первое число децимал
 * @param second Второе число децимал
 * @return 0 - first >= second , 1 - first < second
 */
int s21_is_less_or_equal(s21_decimal first, s21_decimal second);

// ФУНКЦИИ ПРЕОБРАЗОВАНИЯ

/**
 * @brief Перевод из int в децимал
 * @param src число int
 * @param dst указатель на число децимал
 * @return 0 - OK, 1 - ошибка конвертации
 */
int s21_from_int_to_decimal(int src, s21_decimal *dst);
/**
 * @brief Перевод из float в децимал
 * @param src число float
 * @param dst указатель на число децимал
 * @return 0 - OK, 1 - ошибка конвертации
 */
int s21_from_float_to_decimal(float src, s21_decimal *dst);
/**
 * @brief Перевод из децимал в int
 * @param src число децимал
 * @param dst указатель на число int
 * @return 0 - OK, 1 - ошибка конвертации
 */
int s21_from_decimal_to_int(s21_decimal src, int *dst);
/**
 * @brief Перевод из децимал в float
 * @param src число децимал
 * @param dst указатель на число float
 * @return 0 - OK, 1 - ошибка конвертации
 */
int s21_from_decimal_to_float(s21_decimal src, float *dst);

// ДОПОЛНИТЕЛЬНЫЕ ФУНКЦИИ

/**
 * @brief Округляет число decimal до ближайшего целого
 * @param value число децимал
 * @param result указатель на округленный decimal
 * @return 0 - ОК, 1 - ошибка вычисления
 */
int s21_round(s21_decimal value, s21_decimal *result);
/**
 * @brief Округляет число decimal до ближайшего целого числа в сторону
 * отрицательной бесконечности
 * @param value число децимал
 * @param result указатель на округленный decimal
 * @return 0 - ОК, 1 - ошибка вычисления
 */
int s21_floor(s21_decimal value, s21_decimal *result);
/**
 * @brief Умножает указанный Decimal на -1.
 * @param value число децимал
 * @param result указатель на decimal * (-1)
 * @return 0 - ОК, 1 - ошибка вычисления
 */
int s21_negate(s21_decimal value, s21_decimal *result);
/**
 * @brief Отбрасывает дробные цифры указанного Decimal, включая конечные нули
 * @param value число децимал
 * @param result указатель на decimal несущий только целые цифры
 * @return 0 - ОК, 1 - ошибка вычисления
 */
int s21_truncate(s21_decimal value, s21_decimal *result);

#endif
