#ifndef S21_MATH_H_
#define S21_MATH_H_

// PI value
#define s21_PI 3.141592653589793238
// double PI value
#define s21_TWO_PI 6.283185307
// E value
#define s21_E 2.71828182845905
// eps value
#define s21_EPS 1e-12
// long double max value
#define s21_LDBL_MAX 1E+37
// inf
#define s21_INF 1.0 / 0.0
// nan
#define s21_NAN 0.0 / 0.0


/// @brief computes absolute value of an integer value
/// @param x integer number
/// @return absolute value of x
int s21_abs(int x);

/// @brief computes arc cosine
/// @param x double number
/// @return arc cosine value of x
long double s21_acos(double x);

/// @brief computes arc sine
/// @param x double number
/// @return arc sine of x
long double s21_asin(double x);

/// @brief computes arc tangent
/// @param x double number
/// @return arc tangent of x
long double s21_atan(double x);

/// @brief returns the nearest integer not less than the given value
/// @param x double number
/// @return nearest integer not less than the x
long double s21_ceil(double x);

/// @brief computes cosine
/// @param x double number
/// @return cosine of x
long double s21_cos(double x);

/// @brief returns e raised to the given power
/// @param x double number
/// @return e raised to the x
long double s21_exp(double x);

/// @brief computes absolute value of a floating-point value
/// @param x double number
/// @return absolute value of x
long double s21_fabs(double x);

/// @brief returns the nearest integer not greater than the given value
/// @param x double number
/// @return nearest integer not greater than the x
long double s21_floor(double x);

/// @brief computes natural logarithm
/// @param x double number
/// @return natural logarithm of x
long double s21_log(double x);

/// @brief computes tangent
/// @param x double number
/// @return tangent of x
long double s21_tan(double x);

/// @brief computes sine
/// @param x double number
/// @return sine of x
long double s21_sin(double x);

/// @brief computes square root
/// @param x double number greater or equals then zero
/// @return  square root of x
long double s21_sqrt(double x);

/// @brief remainder of the floating-point division operation
/// @param x double number
/// @param y double number
/// @return  remainder of the floating-point division x and y
long double s21_fmod(double x, double y);

/// @brief raises a number to the given power
/// @param base double number
/// @param exp power
/// @return base^exp
long double s21_pow(double base, double exp);

#endif
