#ifndef MALLOC537_H
#define MALLOC537_H

#include <stdio.h>
#include <stdlib.h>
#include "range_tree.h"

void *malloc537(size_t);
void free537(void *);
void *realloc537(void *, size_t );
void memcheck537(void *, size_t );

#endif
