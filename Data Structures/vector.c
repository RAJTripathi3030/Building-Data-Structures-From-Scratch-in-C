// THIS FILE CONTAINS THE SCRATCH IMPLEMENTATION OF A VECTOR IN C LANGUAGE
#include <stdio.h>
#include <stdlib.h>

struct tempVector{
  int size; // CURRENT ELEMENT.
  int capacity; // THE SPACE ALLOCATED TO THE VECTOR IN THE MEMORY.
  int* base_addr; // THIS POINTS TO THE EMPTY SPACE WE FOUND IN THE MEMORY
};

void vector_init(struct tempVector *v){
  v->size = 0;
  v->capacity = 4;
  v->base_addr = malloc(v->capacity * sizeof(int));
}

void vector_pushback(struct tempVector *v, int value){
  if(v->size == v->capacity){
    v->capacity = v->capacity * 2;
    int* tempPtr = realloc(v->base_addr, sizeof(int)*v->capacity);
    if(tempPtr == NULL){
      printf("There was no empty blocks which could be found. Hence push operation unsuccessful...");
      return;
    }
    else{
      v->base_addr = tempPtr;
    }
  }

  *(v->base_addr + v->size) = value;
  v->size += 1;
  return;
}

void vector_popback(struct tempVector *v){
  if(v->size > 0){
    v->size -= 1;
  }
  return;
}

int vector_size(struct tempVector *v){
  return v->size;
}

void vector_print(struct tempVector *v){
  for(int i = 0; i < v->size; i++){
    printf("%d ", *(v->base_addr + i));
  }
  return;
}
// When you use assign, the vector forgets its old elements, destroys them, and takes on a new set of values. It also automatically changes its size to 
// fit the new content.

// There are three main ways to use it:

// 1. The "Fill" Version (Repeat a value)
// This is used when you want the vector to have a specific number of elements, all set to the same value.
// syntax: v.assign(n, value);
// What it does: It makes the vector n elements long and fills every spot with value.

// 2. The "Range" Version (Copy from somewhere else)
// This is used to copy a specific range of elements from another container (like another vector or an array).
// Syntax: v.assign(start_iterator, end_iterator);
// What it does: It takes everything from the start up to (but not including) the end and puts it into the vector.

// 3. The "Initializer List" Version (Direct list)
// Added in C++11, this allows you to pass a list of values inside curly braces {}.
// Syntax: v.assign({val1, val2, val3});
// What it does: It clears the vector and fills it with exactly these numbers.

/*
  1. Old Data is Gone: Any data that was in the vector before calling assign is deleted.

  2. Size Changes: If you assign 10 items to a vector that had 2, its size becomes 10.

  3. Invalidates Iterators: Because assign might move the vector to a new spot in memory (if it needs more space), 
  any old "bookmarks" (iterators or pointers) you had for that vector will stop working and could crash your program if you try to use them.

  4. Performance: It is generally faster than using a loop to push_back many items because it can often allocate all the memory it needs in one go.
*/

void vector_assign(){
  return;
}

