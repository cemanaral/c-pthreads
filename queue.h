#ifndef QUEUE_H_
#define QUEUE_H_

struct queue* createQueue();
void queueAdd(struct queue* queue, char* fileName);
char* queueRemove(struct queue* queue);
#endif