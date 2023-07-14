#ifndef UTILS_S21_UTILS_H_
#define UTILS_S21_UTILS_H_

#include "../s21_decimal.h"


/// @brief Печать числа decimal в десятичном виде
/// @param val число decimal
void print_decimal(s21_decimal val);

/// @brief Печать числа decimal в двоичном виде
/// @param val число decimal
void print_bin_decimal(s21_decimal val);

/// @brief Печать числа big_decimal в десятичном виде
/// @param val число big_decimal
void print_decimal2(s21_big_decimal val);

/// @brief Печать числа big_decimal в двоичном виде
/// @param val число big_decimal
void print_bin_big_decimal(s21_big_decimal val);

/**
 * @brief Функция переводит число децимал в доп.код
 * @param val_1 указатель на число децимал
 */
void convert_to_addcode(s21_decimal *val_1);

/**
 * @brief Функция переводит число децимал в доп.код
 * @param val_1 указатель на число децимал
 */
void big_convert_to_addcode(s21_big_decimal *val_1);

/**
 * @brief Инициализация децимала
 * @param val децимал
 */
void init_decimal(s21_decimal *val);

/**
 * @brief Инициализация децимала
 * @param val децимал
 */
void init_big_decimal(s21_big_decimal *val);

/// @brief Создание децимала
/// @param low_num число для первых 32 бит
/// @param mid_num число для вторых 32 бит
/// @param high_num число для третьих 32 бит
/// @param scale_info число для информационных бит
/// @return число децимал
s21_decimal create_decimal(unsigned int low_num, unsigned int mid_num,
                           unsigned int high_num, unsigned int scale_info);

/// @brief перевод из децимала во флоат
/// @param val число децимал
/// @return число флоат
float get_full_bits(s21_decimal val);

/**
 * @brief Возвращает значение i-го бита
 * @param val число децимал
 * @param i номер бита
 * @return значение указанного бита (0 - если бит равен нулю, 1 - bit on)
 */
int get_bit(s21_decimal val, int i);

/**
 * @brief Возвращает значение i-го бита
 * @param val число big децимал
 * @param i номер бита
 * @return значение указанного бита (0 - если бит равен нулю, 1 - bit on)
 */
int big_get_bit(s21_big_decimal val, int i);

/**
 * @brief Установить i-ый бит (вкл/выкл)
 * @param val указатель на число децимал
 * @param i номер бита
 * @param value значение бита (0 or 1)
 */
void set_bit(s21_decimal *val, int i, int value);

/**
 * @brief Установить i-ый бит (вкл/выкл)
 * @param val указатель на число big децимал
 * @param i номер бита
 * @param value значение бита (0 or 1)
 */
void big_set_bit(s21_big_decimal *val, int i, int value);

/**
 * @brief поиск первого значащего бита
 * @param val число типа децимал в котором ведется поиск
 * @return 0-95 - номер значащего бита, -1 - если все биты пустые
 */
int last_bit(s21_decimal val);

/**
 * @brief Получение степени числа децимал
 * @param val число децимал
 * @return значение степени числа децимал (int)
 */
int get_scale(s21_decimal val);

/**
 * @brief Получение степени числа децимал
 * @param val число децимал
 * @return значение степени числа децимал (int)
 */
int big_get_scale(s21_big_decimal val);

/**
 * @brief Установить необходимую степень числа децимал
 * @param val указатель на число децимал
 * @param scale значение степени числа
 */
void set_scale(s21_decimal *val, int scale);

/**
 * @brief Установить необходимую степень числа децимал
 * @param val указатель на число децимал
 * @param scale значение степени числа
 */
void big_set_scale(s21_big_decimal *val, int scale);

/**
 * @brief Уравнивает степени двух чисел типа децимал
 * @param val_1 указатель на число децимал
 * @param val_2 указатель на число децимал
 * @return код ошибки
 */
int scale_eq(s21_decimal *val_1, s21_decimal *val_2);

/**
 * @brief Уравнивает степени двух чисел типа децимал
 * @param val_1 указатель на число децимал
 * @param val_2 указатель на число децимал
 * @return error
 */
int big_scale_eq(s21_big_decimal *val_1, s21_big_decimal *val_2);

/**
 * @brief Проверка знака числа децимал
 * @param val число децимал
 * @return значение знака числа (0 - число положительное, 1 - отрицательное)
 */
int get_sign(s21_decimal val);

/**
 * @brief Проверка знака числа децимал
 * @param val число децимал
 * @return значение знака числа (0 - число положительное, 1 - отрицательное)
 */
int big_get_sign(s21_big_decimal val);

/**
 * @brief Установка знака числа децимал
 * @param val число децимал
 * @param sign устанавливаемый знак (0 - число положительное, 1 - отрицательное)
 */
void set_sign(s21_decimal *val, int sign);

/**
 * @brief Установка знака числа децимал
 * @param val число децимал
 * @param sign устанавливаемый знак (0 - число положительное, 1 - отрицательное)
 */
void big_set_sign(s21_big_decimal *val, int sign);

/**
 * @brief побитовый сдвиг влево
 * @param val число децимал
 * @param move_value количество сдвигов влево
 * @return код ошибки
 */
int move_left(s21_decimal *val, int move_value);

/**
 * @brief побитовый сдвиг влево
 * @param val число децимал
 * @param move_value количество сдвигов влево
 * @return код ошибки
 */
int big_move_left(s21_big_decimal *val, int move_value);

/**
 * @brief складывает биты, не трогая скейл
 * @param val_1 первое слагаемое
 * @param val_2 второе слагаемое
 * @param result указатель на decimal(результат сложения)
 * @param code ок - сложение, extra - вычитание
 * @return код ошибки
 */
int bit_addition(s21_decimal *val_1, s21_decimal *val_2, s21_decimal *result,
                 int code);

/**
 * @brief складывает биты, не трогая скейл
 * @param val_1 первое слагаемое
 * @param val_2 второе слагаемое
 * @param result указатель на decimal(результат сложения)
 * @param code ок - сложение, extra - вычитание
 * @return код ошибки
 */
int big_bit_addition(s21_big_decimal *val_1, s21_big_decimal *val_2,
                     s21_big_decimal *result, int code);

/**
 * @brief Являются ли числа нулями
 * @param val_1 Первое число децимал
 * @param val_2 Второе число децимал
 * @return 0 - являются (только когда оба),
 * 1 - не являются (по крайней мере одно)
 */
int zero_check(s21_decimal val_1, s21_decimal val_2);

/**
 * @brief Являются ли числа нулями
 * @param val_1 Первое число децимал
 * @param val_2 Второе число децимал
 * @return 0 - являются (только когда оба),
 * 1 - не являются (по крайней мере одно)
 */
int big_zero_check(s21_big_decimal val_1, s21_big_decimal val_2);

/**
 * @brief Проверка чисел на отрицательное значение
 * @param val_1 Первое число децимал
 * @param val_2 Второе число децимал
 * @return 0 - оба одного знака, 1 - первое положительное, -1 - второе
 * положительное
 */
int is_negative(s21_decimal *val_1, s21_decimal *val_2);

/**
 * @brief копирует только первые 96 битов (без скейла)
 * @param src откуда
 * @param dest куда
 */
void copy_bits(s21_decimal src, s21_decimal *dest);

/**
 * @brief копирует только первые 96 битов (без скейла)
 * @param src откуда
 * @param dest куда
 */
void big_copy_bits(s21_decimal src, s21_big_decimal *dest);

/**
 * @brief Деление битов игнорируется степень
 * @param value_1 число децимал
 * @param value_2 число децимал
 * @param div_remainder указатель на остаток от деления
 * @param result указатель на результат деления (целая часть)
 */
void div_only_bits(s21_decimal value_1, s21_decimal value_2,
                   s21_decimal *div_remainder, s21_decimal *result);

/**
 * @brief Деление битов игнорируется степень
 * @param value_1 число децимал
 * @param value_2 число децимал
 * @param div_remainder указатель на остаток от деления
 * @param result указатель на результат деления (целая часть)
 */
void big_div_only_bits(s21_big_decimal value_1, s21_big_decimal value_2,
                       s21_big_decimal *div_remainder, s21_big_decimal *result);

/// @brief Перевод числа из флоата в децимал
/// @param num число в формате флоат
/// @param result указатель на число в формате децимал
void float_to_bin(float num, s21_decimal *result);

/// @brief Проверяет что числа big_decimal равны
/// @param first число big_decimal
/// @param second число big_decimal
/// @return 1 - числа равны, иначе - 0
int big_s21_is_equal(s21_big_decimal first, s21_big_decimal second);

/// @brief Проверяет что первое число big_decimal строго больше второго
/// @param first число big_decimal
/// @param second число big_decimal
/// @return 1 - first > second, иначе - 0
int big_s21_is_greater(s21_big_decimal first, s21_big_decimal second);

/// @brief Проверяет что первое число big_decimal больше или равно второму
/// @param first число big_decimal
/// @param second число big_decimal
/// @return 1 - first >= second, иначе - 0
int big_s21_is_greater_or_equal(s21_big_decimal first, s21_big_decimal second);

#endif
