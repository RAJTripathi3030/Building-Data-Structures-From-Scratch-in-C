// THIS FILE CONTAINS THE SCRATCH IMPLEMENTATION OF QUEUE DATA STRUCTURE
#include <stdio.h>
#include "Header Files/vector.h"

struct tempQueue{
  struct tempVector vec;
  int front_index;
};

void queue_push(struct tempQueue *q, int value){
  vector_pushback(&(q->vec), value);
}

void queue_pop(struct tempQueue *q){
  int *curr_addr = q->vec.base_addr;
  int *new_addr = curr_addr + 1;
  q->vec.base_addr = new_addr;
  q->vec.size -= 1;
}

int queue_size(struct tempQueue *q){
  return q->vec.size;
}

void queue_print(struct tempQueue *q){
  vector_print(&(q->vec));
}

int main(){
  struct tempQueue q;
  q.front_index = 0;
  queue_push(&q, 10);
  queue_push(&q, 20);
  queue_push(&q, 30);
  queue_push(&q, 40);
  
  printf("The size of the current queue is : %d \n", queue_size(&q));
  queue_print(&q);
  printf("\n");

  queue_pop(&q);
  printf("The size of the current queue is : %d \n", queue_size(&q));
  queue_print(&q);

  printf("\n");

  queue_pop(&q);
  printf("The size of the current queue is : %d \n", queue_size(&q));
  queue_print(&q);

  printf("\n");

  return 0;
}
