#include <pthread.h>
#include <stdio.h>

#include "queue.h"
#include "string_processors.h"

int main()
{
	//Declarations
	pthread_t t1, t2, t3, t4;
    Queue *q1, *q2, *q3;
    TwoQueues q12, q23;

    //Creating all the queues
    q1 = CreateStringQueue(QSIZE);
    q2 = CreateStringQueue(QSIZE);
    q3 = CreateStringQueue(QSIZE);
    

    q12.qLeft = q1;
    q12.qRight = q2;
    q23.qLeft = q2;
    q23.qRight = q3;

    //Creating Reader, Munch1, Munch2 and Writer threads
	pthread_create(&t1, NULL, (void *) &Reader, (void *)q1);
	pthread_create(&t2, NULL, (void *) &Munch1, (void *)&q12);
	pthread_create(&t3, NULL, (void *) &Munch2, (void *)&q23);
	pthread_create(&t4, NULL, (void *) &Writer, (void *)q3);
	
	//Waiting for threads to finish
	pthread_join(t1, NULL);
	pthread_join(t2, NULL);
	pthread_join(t3, NULL);
	pthread_join(t4, NULL);
	
	printf("\n***** Queue 1 stats: *****\n");
	PrintQueueStats(q1);
	
	printf("\n***** Queue 2 stats: *****\n");
	PrintQueueStats(q2);
	
	printf("\n***** Queue 3 stats: *****\n");
	PrintQueueStats(q3);
	
	return 0;
}
