#ifndef STRING_PROCESSORS_H
#define STRING_PROCESSORS_H

#include <ctype.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern const int BUFFERSIZE;

//Reads a string from stdin and enqueues it to a queue
void Reader(void *);

//Dequeues a string from a queue and replaces spaces with *
//Then enqueues the string to another queue
void Munch1(void *);

//Dequeues a string from a queue and converts lowercase to uppercase
//Then enqueues the string to another queue
void Munch2(void *);

//Dequeues a string from a queue and writes it to standard output.
void Writer(void *);

#endif
