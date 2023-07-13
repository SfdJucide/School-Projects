#include "s21_math.h"

int s21_abs(int x) { return (x < 0) ? -x : x; }

long double s21_fabs(double x) { return (x < 0) ? -x : x; }

long double s21_fmod(double x, double y) {
  double result;
  int n;
  n = x / y;
  result = x - n * y;
  return result;
}

long double s21_floor(double x) {
  long double result = 0.;
  if (x == s21_INF || x == -s21_INF) {
    result = x;
  } else if (x != x) {
    result = x;
  } else if (x < 0 && ((long long int)x == 0)) {
    result = (long long int)x - 1;
  } else if (x == (long long int)x) {
    result = (long long int)x;
  } else {
    long long int tmp;
    tmp = (long long int)x;
    tmp < 0 ? tmp-- : tmp;
    result = (long double)tmp;
  }
  return result;
}

long double s21_ceil(double x) {
  long double result = 0.;
  if (x == s21_INF || x == -s21_INF) {
    result = x;
  } else if (x != x) {
    result = x;
  } else if ((x < 0 && ((long long int)x == 0)) || (x == (long long int)x)) {
    result = (long long int)x;
  } else {
    long long int tmp;
    tmp = (long long int)x;
    tmp < 0 ? tmp : tmp++;
    result = (long double)tmp;
  }
  return result;
}

long double s21_pow(double base, double exp) {
  long double res = 0.;
  long double copy = base;
  if (exp == 0) {
    res = 1;
  } else if (base == 1) {
    res = 1;
  } else if (base == s21_INF || (base == -s21_INF && s21_fmod(exp, 2) == 0)) {
    res = s21_INF;
  } else if (base < 0 && (long long int)exp != exp) {
    res = s21_NAN;
  } else if (copy < 0) {
    copy = -copy;
    res = s21_exp(exp * s21_log(copy));
    if (s21_fmod(exp, 2) != 0) {
      res = -res;  // четная/нечетная степень при отрицательном основании
    }
  } else {
    res = s21_exp(exp * s21_log(base));
  }
  return res;
}

long double s21_sqrt(double x) {
  long double answer = 0.;
  if (x < 0 || x != x) {
    answer = -s21_NAN;
  } else if (x == s21_INF) {
    answer = x;
  } else {
    long double S = x, a = 1, b = x;
    while (s21_fabs(a - b) > s21_EPS) {
      a = (a + b) / 2;
      b = S / a;
    }
    answer = (a + b) / 2;
  }
  return answer;
}

long double s21_exp(double x) {
  long double res = 1.;
  long double temp = 1.;
  long double i = 1.;
  int flag = 0;
  if (x < 0) {
    x *= -1;
    flag = 1;
  }
  while (s21_fabs(res) > s21_EPS) {
    res *= x / i;
    i += 1;
    temp += res;
    if (temp > s21_LDBL_MAX * s21_LDBL_MAX) {
      temp = s21_INF;
      break;
    }
  }
  if (flag == 1) {
    if (temp > s21_LDBL_MAX) {
      temp = 0;
    } else {
      temp = 1. / temp;
    }
  }
  if (temp > s21_LDBL_MAX) {
    temp = s21_INF;
  }
  return temp;
}

long double s21_cos(double x) {
  long double member, answer;
  x = s21_fmod(x, s21_TWO_PI);
  member = 1;
  answer = 1;
  if (s21_fabs(x) < s21_EPS) {
    answer = 1.;
  } else {
    for (long double i = 1.; s21_fabs(member) > s21_EPS && i < 50; i++) {
      member *= ((-1.) * x * x / (2. * i * (2. * i - 1.)));
      answer += member;
    }
  }
  return answer;
}

long double s21_sin(double x) {
  long double member, answer;
  x = s21_fmod(x, s21_TWO_PI);
  member = x;
  answer = x;
  if (s21_fabs(x) < s21_EPS) {
    answer = 0.;
  } else {
    for (long double i = 1.; s21_fabs(member) > s21_EPS && i < 50; i++) {
      member *= ((-1.) * x * x / (2. * i * (2. * i + 1.)));
      answer += member;
    }
  }
  return answer;
}

long double s21_tan(double x) {
  long double answer = 0;
  x = s21_fmod(x, s21_PI);
  if (x == 0) {
    answer = 0;
  } else {
    answer = s21_sin(x) / s21_cos(x);
  }
  return answer;
}

long double s21_log(double x) {
  long double ex_pow = 0;
  long double result = 0;
  long double compare = 0;
  long double y = x;

  if (x == 0) {
    result = -s21_INF;
  } else if (x < 0) {
    result = -s21_NAN;
  } else if (x == s21_INF) {
    result = s21_INF;
  } else {
    for (; y >= s21_E; y /= s21_E, ex_pow++) continue;
    for (double i = 0; i < 100; i++) {
      compare = result;

      result = compare + 2 * (y - s21_exp(compare)) / (y + s21_exp(compare));
    }
  }
  return (result + ex_pow);
}

long double s21_atan(double x) {
  long double sum_atan = 0;
  const long double s21_atan_1 = 0.7853981633974480L;
  if ((x) != x) {
    sum_atan = s21_NAN;
  } else if (x == 1) {
    sum_atan = s21_atan_1;
  } else if (x == -1) {
    sum_atan = -s21_atan_1;
  } else if (s21_fabs(x - s21_PI / 2) < s21_EPS) {
    sum_atan = 1.003884821853887214L;
  } else if (s21_fabs(x + s21_PI / 2) < s21_EPS) {
    sum_atan = -1.003884821853887214L;
  } else if (x == s21_INF || x == -s21_INF) {
    sum_atan = x < 0 ? -s21_PI / 2 : s21_PI / 2;
  } else if (-1. < x && x < 1.) {
    for (register int i = 0; i < 2000; i++) {
      sum_atan += s21_pow(-1, i) * s21_pow(x, 1 + (2 * i)) / (1 + (2 * i));
    }
  } else {
    for (register int i = 0; i < 2000; i++) {
      sum_atan += s21_pow(-1, i) * s21_pow(x, -1 - (2 * i)) / (1 + (2 * i));
    }
    sum_atan = s21_PI * s21_sqrt(x * x) / (2 * x) - sum_atan;
  }
  return sum_atan;
}

long double s21_acos(double x) {
  long double answer = 0.;
  if (x - 1. > 0 || x + 1. < 0) {
    answer = s21_NAN;
  } else if (s21_fabs(x - 1) < s21_EPS) {
    answer = 0.;
  } else if (s21_fabs(x) < s21_EPS) {
    answer = s21_PI / 2;
  } else if (s21_fabs(x + 1) < s21_EPS) {
    answer = s21_PI;
  } else {
    answer = s21_atan(s21_sqrt(1 - x * x) / x);
    if (x > -1. && x < 0.) {
      answer += s21_PI;
    }
  }
  return answer;
}

long double s21_asin(double x) {
  long double answer = x;
  if (x - 1. > 0 || x + 1. < 0) {
    answer = s21_NAN;
  } else if (s21_fabs(x) < s21_EPS) {
    answer = 0.;
  } else {
    answer = s21_atan(x / s21_sqrt(1 - x * x));
  }
  return answer;
}
