#include "build_spec_graph.h"

const int TRAVERSED = 0;
const int UNTRAVERSED = 1;
const int TRAVERSING = 2;

//Initialize empty node list.
void initNodeList(nodeList *list) {
    list->head = NULL;
    list->tail = NULL;
}

//Add a node to the end of node list.
void addNodeToList(nodeList *list, node *n) {
    if(list->tail == NULL) { //List is empty.
        list->head = n;
        list->tail = n;
        n->next = NULL;
    }
    else {  //Append node to the end of list.
        list->tail->next = n;
        list->tail = n;
    }
}

//Find a node based on node name.
node *getNode(nodeList *list, char *name) {
    node *tmp = list->head;
    while(tmp) {
        if(strcmp(tmp->name, name) == 0)
            return tmp;
        tmp = tmp->next;
    }
    return NULL;
}

/**
 * Create a new node for the given target/depedency if node does not exist.
 * Returns pointer to the newly created/existing node.
 */
node *createNode(targetList *tList, nodeList *nList, char *name) {
    int i;

    node *n = getNode(nList, name);   //Check if node already exists;
    if(n != NULL)
        return n;

    target *t = getTarget(tList, name);
    n = (node *)malloc(sizeof(node));
    if(n == NULL) {
        printf("Malloc failed.");
        exit(1);
    }
    addNodeToList(nList, n);
    n->name = name;
    n->status = UNTRAVERSED;

    if(t == NULL) {     //Check if the node is a target. Not a target => Leaf node in graph.
        n->dependencyCount = 0;
        n->commandCount = 0;
        return n;
    }
    n->dependencyCount = t->dependencyCount;
    n->commandCount = t->commandCount;

    n->commands = (char **)malloc(sizeof(char *) * n->commandCount);
    if(n->commands == NULL) {
        printf("Malloc failed.");
        exit(1);
    }
    command *c = t->commands;   //Copy commands from target struct to node struct.
    for(i = 0; i < n->commandCount; i++) {
        n->commands[i] = c->cmd;
        c = c->next;
    }

    dependency *d = t->dependencies;    //Add dependency nodes to array to dependencies.
    n->dependencies = (node **)malloc(sizeof(node *) * n->dependencyCount);
    if(n->dependencies == NULL) {
        printf("Malloc failed.");
        exit(1);
    }
    for(i = 0; i < n->dependencyCount; i++) {
        n->dependencies[i] = createNode(tList, nList, d->name);
        d = d->next;
    }
    return n;
}

//Create a graph from given list of targets.
nodeList *createGraph(targetList *tList) {

    //Create and initialize empty list for nodes in the graph.
	nodeList *nList = (nodeList *)malloc(sizeof(nodeList));
    if(nList == NULL) {
        printf("Malloc failed.");
        exit(1);
    }
	initNodeList(nList);

    target *t = tList->head;
    while(t) {
        createNode(tList, nList, t->name);
        t = t->next;
    }
    return nList;
}

//Do a depth first search of graph while maintaing the traversal status of nodes to detect cycles.
void DFS(node *n) {
    //printf("%s\n", n->name);
    if(n->status == UNTRAVERSED) {

        n->status = TRAVERSING;     //Start traversing a node.
        for(int i = 0; i < n->dependencyCount; i++) {
            if(n->dependencies[i]->status == TRAVERSING) {  //Partial traversed node detected.
                printf("Cyclic dependency detected\n");
                exit(1);
            }
            else if(n->dependencies[i]->status == UNTRAVERSED)
                DFS(n->dependencies[i]);
        }
        n->status = TRAVERSED;      //Node completely traversed.
    }
}

//Check if the graph contains cycles using DFS.
void chekcCyclicDependency(nodeList *nList) {
    node *n = nList->head;
    while(n) {
        DFS(n);
        n = n->next;
    }
}

//Get the last modified time of a file.
time_t lastModifiedTime(char *name) {
    struct stat sb;

    if(stat(name, &sb) < 0)
        return -1;
    return sb.st_mtime;
}

//Execute the commands of a given node if its dependencies have been modified after last build.
void buildNode(node *n) {
    int i, build = 0;
    time_t lastModified = lastModifiedTime(n->name);
    for(i = 0; i < n->dependencyCount; i++) {
        buildNode(n->dependencies[i]);
        //Check if dependency has been modified after last build.
        if(lastModified < lastModifiedTime(n->dependencies[i]->name)) {
            build = 1;
        }
    }
    if(n->dependencyCount == 0) {
        build = 1;
    }

    //Execute commands if dependencies have been modified.
    if(build) {
        for(i = 0; i < n->commandCount; i++) {
            executeCommand(n->commands[i]);
        }
    }
}

//Execute the make recipie for given target.
void buildGraph(nodeList *nList, char *target) {
    node *n = getNode(nList, target);
    if(n == NULL)
        printf("Build target not found\n");
    else
        buildNode(n);
}
