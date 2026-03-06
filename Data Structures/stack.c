#include <stdio.h>
#include "Header Files/vector.h"
/*THIS FILE CONTAINS THE SCRATCH IMPLEMENTATION OF STACK DATA STRUCTURE*/

// INIT A STACK CONTAINER 
struct tempStack{
  struct tempVector vec;
};

void stack_init(struct tempStack *s){
  vector_init(&(s->vec));
}

void stack_verify(struct tempStack *s){
  if(s->vec.base_addr != NULL){
    printf("Current capacity : %d \n", s->vec.capacity);
    printf("Current size : %d \n", s->vec.size);
  }else{
    printf("HAWWWWWWWWWWW \n");
  }
}

void stack_push(struct tempStack *s, int value){
  vector_pushback(&(s->vec), value); 
}

void stack_pop(struct tempStack *s){
  vector_popback(&(s->vec));
}

int stack_peek(struct tempStack *s){
  int index = s->vec.size - 1;
  return *(s->vec.base_addr + index);
}

int stack_size(struct tempStack *s){
  return s->vec.size;
}
void stack_print(struct tempStack *s){
  vector_print(&(s->vec));
}


int main(){
  struct tempStack s;
  stack_init(&s);
  stack_verify(&s);

  stack_push(&s,10);
  stack_push(&s,20);
  stack_push(&s,30);
  stack_push(&s,40);
  
  printf("The current stack size is : %d \n", stack_size(&s));

  stack_pop(&s);
  stack_pop(&s);
  printf("The topmost element is : %d \n", stack_peek(&s));
  printf("The current stack size is : %d \n", stack_size(&s));
 
  stack_print(&s);

  return 0;
}
