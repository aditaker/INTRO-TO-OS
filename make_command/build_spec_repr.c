//Aditya Rungta
//Nachiket Kshatriya

#include "build_spec_repr.h"

//Initialize empty target list.
void initTargetList(targetList *list) {
    list->head = NULL;
    list->tail = NULL;
}

//Add a target to the end of target list.
void addTargetToList(targetList *list, target *t) {
    if(list->tail == NULL) { //List is empty.
        list->head = t;
        list->tail = t;
        t->next = NULL;
    }
    else {  //Append target to the end of list.
        list->tail->next = t;
        list->tail = t;
    }
}

//Create a target struct from a line containing target.
target *createTarget(char *str) {
    dependency *d, *tmp;
    char *tmpToken, *token;

    target *t = (target *)malloc(sizeof(target)); //Create new target struct.
    if(t == NULL) {
        printf("Malloc failed.");
        exit(1);
    }
    t->dependencyCount = 0;
    t->commandCount = 0;
    t->dependencies = NULL;
    t->commands = NULL;
    
    tmpToken = strtok(str, " :");        //Parse the line to get target name.
    token = (char *)malloc(strlen(tmpToken));
    if(token == NULL) {
        printf("Malloc failed.");
        exit(1);
    }
    strcpy(token, tmpToken);
    t->name = token;
    
    while((tmpToken = strtok(NULL, " :")) != NULL) { //Parse the line to get dependencies.
        token = (char *)malloc(strlen(tmpToken));
        if(token == NULL) {
            printf("Malloc failed.");
            exit(1);
        }
        strcpy(token, tmpToken);
        d = (dependency *)malloc(sizeof(dependency));
        if(d == NULL) {
            printf("Malloc failed.");
            exit(1);
        }
        d->name = token;
        d->next = NULL;
        if(t->dependencies == NULL) {      //Add dependency to the target's dependencies list.
            t->dependencies = d;
            tmp = d;
        }
        else {
            tmp->next = d;
            tmp = tmp->next;
        }
        t->dependencyCount++;
    }
    return t;
}

//Add a command to the target's commands list.
void addCommandToTarget(target *t, command *c) {
    if(t->commands == NULL)
        t->commands = c;
    else {
        command *tmp = t->commands;
        while(tmp->next)
            tmp = tmp->next;
        tmp->next = c;
    }
    t->commandCount++;
}

//Find a target struct based on target name.
target *getTarget(targetList *list, char *name) {
    target *tmp = list->head;
    while(tmp) {
        if(strcmp(tmp->name, name) == 0)
            return tmp;
        tmp = tmp->next;
    }
    return NULL;
}

//Print a target struct.
void printTarget(target *t) {
    printf("Target Name: %s\n", t->name);
    printf("*****Dependencies*****\n");
    dependency *d = t->dependencies;
    while(d) {
        printf("%s\n", d->name);
        d = d->next;
    }
    printf("*****Commands*****\n");
    command *c = t->commands;
    while(c) {
        printf("%s\n", c->cmd);
        c = c->next;
    }
}

//Print the target list.
void printTargetList(targetList *list) {
    target *tmp = list->head;
    while(tmp) {
        printTarget(tmp);
        tmp = tmp->next;
    }
}
