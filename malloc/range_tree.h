#ifndef RANGE_TREE_H
#define RANGE_TREE_H

#include <stdlib.h>
#include <stdbool.h>

typedef struct Node{
	struct Node *parent;
	struct Node *left;
	struct Node *right;
	char color;
	void *address;
	int len;
	bool freed;
} Node;

extern Node **root;

void deleteNode(Node *);
void insertNode(void *, int);

Node *searchInitialAddress(Node *, void *);
Node *searchNonInitialAddress(Node *, void *);
Node *searchOverlappingRange(Node *, void *, int);
Node* searchValidRange(Node *, void *, int);

#endif
