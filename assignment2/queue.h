#include <semaphore.h>
#include <stdlib.h>
#define QSIZE 1

typedef struct queue {
    char **arr;
    int f, r;
	int enqueueCount, dequeueCount, enqueueBlockCount, dequeueBlockCount;
    sem_t full, empty;
} queue;

void init(queue *q);
void enqueue(queue *q, char *c);
char *dequeue(queue * q);