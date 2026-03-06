// vector.h 
#ifndef VECTOR_H
#define VECTOR_H
#include<stdlib.h>

struct tempVector{
  int size;
  int capacity;
  int* base_addr;
};

void vector_init(struct tempVector *v);
void vector_pushback(struct tempVector *v, int value);
void vector_popback(struct tempVector *v);
int vector_size(struct tempVector *v);
void vector_print(struct tempVector *v);

#endif
