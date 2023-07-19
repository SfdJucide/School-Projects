#ifndef SRC_S21_STACK_H_
#define SRC_S21_STACK_H_

#define STACK_MAX_SIZE 255
#define STACK_OVERFLOW -100
#define STACK_UNDERFLOW -101

/**
 * @brief Структура стека для чисел
 */
typedef struct {
  long double data[STACK_MAX_SIZE];
  int length;
} Stack_digit;

/**
 * @brief Печать стека
 * @param stack указатель на стек
 */
void printStack_digit(Stack_digit *stack);
/**
 * @brief Добавление элемента на вершину стека
 * @param stack указатель на стек
 * @param value значение
 */
void push_digit(Stack_digit *stack, long double value);
/**
 * @brief Выдергивание элемента из вершины стека
 * @param stack указатель на стек
 * @return верхний элемент стека
 */
long double pop_digit(Stack_digit *stack);

/**
 * @brief Структура стека для знаков
 */
typedef struct {
  char data[STACK_MAX_SIZE];
  int length;
} Stack_sign;

/**
 * @brief Печать стека
 * @param stack указатель на стек
 */
void printStack_sign(Stack_sign *stack);
/**
 * @brief Добавление элемента на вершину стека
 * @param stack указатель на стек
 * @param value значение
 */
void push_sign(Stack_sign *stack, char value);
/**
 * @brief Выдергивание элемента из вершины стека
 * @param stack указатель на стек
 * @return верхний элемент стека
 */
char pop_sign(Stack_sign *stack);

#endif