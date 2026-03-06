#ifndef QUEUE_H
#define QUEUE_H

struct tempQueue{
 struct tempVector vec;
 int front_index;
};

void queue_push(struct tempQueue *q, int value);
void queue_pop((struct tempQueue *q);
int queue_size(struct tempQueue *q);
void queue_print(struct tempQueue *q);

#endif
