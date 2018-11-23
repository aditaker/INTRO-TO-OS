//Aditya Rungta
//Nachiket Kshatriya

#ifndef BUILD_SPEC_REPR_H
#define BUILD_SPEC_REPR_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct dependency {
    char *name;
    struct dependency *next;
} dependency;

typedef struct command {
    char *cmd;
    struct command *next;
} command;

typedef struct target {
    char *name;
    dependency *dependencies;
    command *commands;
    int commandCount, dependencyCount;
    struct target *next;
} target;

typedef struct targetList {
    target *head, *tail;
} targetList;

//Initialize empty list of targets.
void initTargetList(targetList *);

//Create a target struct from line containg a target.
target *createTarget(char *);

//Add target to the target list.
void addTargetToList(targetList *, target *);

//Add commands of a target to the target struct.
void addCommandToTarget(target *, command *);

//Find a target struct based on target name.
target *getTarget(targetList *, char *);

#endif
