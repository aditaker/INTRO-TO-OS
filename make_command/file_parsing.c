#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <ctype.h> 
#include<sys/wait.h> 
#include<unistd.h> 
#define BUFFER_SIZE 1024
#define DEPENDENCY_SIZE 16

struct Target{
	struct Target* next;
	char* target_name;
	char **dependencyList;
	char **commandsList;
	int num_commands;
	int is_explored;
	char *color;
} Target;

struct Node{
	struct Target *target;
	struct Node **neighbors;
	struct Node* next;
} Node;

struct Target* parse_file() {
	FILE *fp = fopen("makefile", "r");
	char *line = NULL;
	ssize_t read;
	size_t len = 0;
	char tab = '\t';
	int line_num = 0;
	char *ptr;
	int i, index;
	struct Target* ans = (struct Target*)malloc(sizeof(struct Target));
	struct Target* prev = ans;
	while ((read = getline(&line, &len, fp)) != -1) {
		line_num++;
		i = 0;
		if(line[i] == '\n')
			continue;

        if(line[0] == tab) {
        	if(ans->next == NULL) {
        		fprintf(stderr,"Invalid line %d\n", line_num);
        		exit(1);
        	}
        	//command
    		char *str = (char*)malloc(BUFFER_SIZE);
    		strcpy(str, line);
    		while(isblank(str[0]))
    			str++;
    		char *pos;
    		if ((pos=strchr(str, '\n')) != NULL)
    			*pos = '\0';
    		prev->commandsList[prev->num_commands] = str;
    		prev->num_commands++;
        }
        else if(line[0] == ' ') {
        	fprintf(stderr,"Invalid line %d\n", line_num);
    		exit(1);
        }
        else if( strchr(line, ':') && (ptr = strtok(line, ":")) ) {
        	struct Target* target = (struct Target*)malloc(sizeof(struct Target));
        	target->target_name = (char*)malloc(BUFFER_SIZE);
        	strcpy( target->target_name, ptr); 
        	target->dependencyList = (char**)malloc(sizeof(char*) * DEPENDENCY_SIZE);   
        	target->commandsList = (char**)malloc(sizeof(char*) * BUFFER_SIZE);
        	target->color = (char*)malloc(BUFFER_SIZE);
        	target->num_commands = 0;
        	target->is_explored = 0;
        	target->color = "WHITE";
        	for(int i = 0; i < DEPENDENCY_SIZE; i++)
        		 target->dependencyList[i] = NULL;
    		for(int i = 0; i < BUFFER_SIZE; i++)
        		 target->commandsList[i] = NULL;
        	index = 0;
        	while(ptr) {
        		ptr = strtok(NULL, " \n");
        		if(ptr) {
	        		char *str = (char*)malloc(BUFFER_SIZE);
	        		strcpy(str, ptr);
	    			target->dependencyList[index] = str;
	    			index++;
    			}
        	}
        	prev->next = target;
        	prev = target;
        }
        else {
        	fprintf(stderr,"Invalid line %d\n", line_num);
        	exit(1);
        }
    }
    prev->next = NULL;
    return ans->next;
}

struct Node* createUtil(struct Node *node, struct Target *target_list) {
	int i = 0;
	int j = 0;
	struct Target *target_temp;
	
	node->neighbors = (struct Node **)malloc(sizeof(struct Node*) * DEPENDENCY_SIZE);
	while(node->target->dependencyList[i]) {

		target_temp = target_list;
		while(target_temp != NULL && strcmp(target_temp->target_name, node->target->dependencyList[i]))
			target_temp = target_temp->next;
		//This means that the dependency is a target
		if(target_temp) {
			node->neighbors[j] = (struct Node*)malloc(sizeof(struct Node));
			node->neighbors[j]->target = (struct Target *)malloc(sizeof(struct Target));
			node->neighbors[j]->target = target_temp;
			if(target_temp->is_explored == 0) {
				target_temp->is_explored = 1;
				createUtil(node->neighbors[j], target_list);
			}
			j++;
		}
		i++;
	}
	return node;
}


void executeCommand(char *command) {
	int pid = fork();
	if(pid == 0) {
		char *token;
		char **argv = (char **)malloc(sizeof(char*)*BUFFER_SIZE);
		token = strtok(command, " ");
		int j = 0;
		while( token != NULL ) {
			argv[j] = (char *)malloc(BUFFER_SIZE);
			argv[j++] = token;
			token = strtok(NULL, " ");
		}
		argv[j] = NULL;
		printf("%s\n",argv[0]);
		execvp(argv[0], argv);
		printf("Invalid command\n");
    	exit(1);
	}
	else {
		wait(NULL);
		return;
	}
}


void runBottomUp(struct Node *root) {
	// printf("%s\n",root->target->target_name);
	int i = 0;
	root->target->color = "GRAY";
	while(root->neighbors[i]) {
		if(strcmp(root->neighbors[i]->target->color, "GRAY") == 0) {
			fprintf(stderr,"Error : Circular Dependency\n");
			exit(1);
		}
		if(strcmp(root->neighbors[i]->target->color, "WHITE") == 0)
			runBottomUp(root->neighbors[i]);
		i++;
	}
	//Execute the commands here
	root->target->color = "BLACK";
	int j = 0;
	while(j < root->target->num_commands) {
		executeCommand(root->target->commandsList[j]);
		j++;
	}
	
	// root->target
}


void createGraphFromTargets(char *target_arg) {
	struct Target *target_list;
	target_list = parse_file();
	struct Target *target = target_list;
	while(target != NULL && strcmp(target->target_name, target_arg)) 
		target = target->next;

	if(target == NULL) {
		fprintf(stderr, "Invalid target : %s\n", target_arg);
		exit(1);
	}
	struct Node* node = (struct Node*)malloc(sizeof(struct Node));
	node->target = (struct Target *)malloc(sizeof(struct Target));
	node->target = target;
	struct Node *root = createUtil(node, target_list);
	runBottomUp(root);
}



void DFS() {

}


int main(int argc, char **argv) {
	createGraphFromTargets(argv[1]);
	// struct Target *target_list;
	// target_list = parse_file();
	// while(target_list) {
	// 	printf("Target : ");
	// 	printf("%s ", target_list->target_name);
	// 	int i = 0;
	// 	printf("--------- Dependencies : ");
	// 	while(target_list->dependencyList[i]) {
	// 		printf("%s ",target_list->dependencyList[i]);
	// 		i++;
	// 	}
	// 	printf("\n");
	// 	printf("Commands :\n");
	// 	int j = 0;
	// 	while(j < target_list->num_commands) {
	// 		printf("%s",target_list->commandsList[j]);
	// 		j++;
	// 	}
	// 	printf("\n");
	// 	target_list = target_list->next;
	// }
	return 0;
}