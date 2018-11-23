//Aditya Rungta
//Nachiket Kshatriya

#ifndef BUILD_SPEC_GRAPH_H
#define BUILD_SPEC_GRAPH_H

#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "build_spec_repr.h"
#include "execute_command.h"

extern const int TRAVERSED, UNTRAVERSED, TRAVERSING;

typedef struct node {
    char *name;
    struct node **dependencies;
    char **commands;
    int status, dependencyCount, commandCount;
    struct node *next;
} node;

typedef struct nodeList {
    node *head, *tail;
} nodeList;


//Initialize empty node list.
void initNodeList(nodeList *);

//Create a graph from the list of nodes.
nodeList *createGraph(targetList *);

//Create a graph node for given target/dependency.
node *createNode(targetList *, nodeList *, char *);

//Check if the graph contains a dependency cycle.
void chekcCyclicDependency(nodeList *);

//Execute the make recipie for given target.
void buildGraph(nodeList *, char *);
#endif