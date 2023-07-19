#include "s21_stack.h"

#include <stdio.h>
#include <stdlib.h>

void printStack_digit(Stack_digit *stack) {
  int i;
  printf("stack (%d) > ", stack->length);
  for (i = 0; i < stack->length; i++) {
    printf("%Lf ", stack->data[i]);
  }
  printf("\n");
}

void push_digit(Stack_digit *stack, long double value) {
  if (stack->length >= STACK_MAX_SIZE) {
    exit(STACK_OVERFLOW);
  }
  stack->data[stack->length] = value;
  stack->length++;
}

long double pop_digit(Stack_digit *stack) {
  if (stack->length == 0) {
    exit(STACK_UNDERFLOW);
  }
  stack->length--;
  return stack->data[stack->length];
}

void printStack_sign(Stack_sign *stack) {
  int i;
  printf("stack (%d) > ", stack->length);
  for (i = 0; i < stack->length; i++) {
    printf("%c ", stack->data[i]);
  }
  printf("\n");
}

void push_sign(Stack_sign *stack, char value) {
  if (stack->length >= STACK_MAX_SIZE) {
    exit(STACK_OVERFLOW);
  }
  stack->data[stack->length] = value;
  stack->length++;
}

char pop_sign(Stack_sign *stack) {
  if (stack->length == 0) {
    exit(STACK_UNDERFLOW);
  }
  stack->length--;
  return stack->data[stack->length];
}
