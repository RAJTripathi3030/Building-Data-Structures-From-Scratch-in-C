#ifndef STACK_H
#define STACK_H
#include<stdio.h>
#include<stdlib.h>

struct tempStack{
  struct tempVector vec; 
}
void stack_init(struct tempStack *s);
void stack_verify(struct tempStack *s);
void stack_push(struct tempStack *s, int value);
void stack_pop(struct tempStack *s);
int stack_peek(struct tempStack *s);
int stack_size(struct tempStack *s);
void stack_print(struct tempStack *s);

#endif
