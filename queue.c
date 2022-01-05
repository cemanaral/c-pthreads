#include <string.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

#include "queue.h"

struct queue {
    char** array;

    int currentSize;
    int maxSize;
    
    int head;
    int back;
};

struct queue* createQueue() {
    struct queue* queue = (struct queue*) malloc(sizeof(struct queue));
    queue->maxSize = 100000;
    queue->head = queue->currentSize = 0;
    queue->back = 100000 - 1;
    queue->array = (char**) malloc(queue->maxSize);
    return queue;
}

void queueAdd(struct queue* queue, char* fileName) {
    if (queue->currentSize==queue->maxSize)
        return;
    queue->back = (queue->back + 1) % queue->maxSize;   
    queue->array[queue->back] = strdup(fileName);
    queue->currentSize = queue->currentSize + 1;
}

char* queueRemove(struct queue* queue) {
    if (queue->currentSize) {
        char* item = queue->array[queue->head];
        queue->head = (queue->head + 1)% queue->maxSize;
        queue->currentSize = queue->currentSize - 1;
        return item;
    }
    else {
        return NULL;
    }
    
}
