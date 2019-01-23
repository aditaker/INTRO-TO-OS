#ifndef QUEUE_H
#define QUEUE_H

#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>

extern const int QSIZE;

typedef struct Queue {
    char **arr;
    int front, rear;
	int enqueueCount, dequeueCount, enqueueBlockCount, dequeueBlockCount;
    sem_t full, empty;
} Queue;

typedef struct TwoQueues {
	Queue *qLeft, *qRight;
} TwoQueues;

//Creates a queue for storing strings
Queue *CreateStringQueue(int);

//Enqueues a string to the queue
void EnqueueString(Queue *, char *);

//Dequeues a string from the queue
char *DequeueString(Queue *);

//Prints all queue stats
void PrintQueueStats(Queue *);

#endif
