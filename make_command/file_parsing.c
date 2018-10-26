#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <ctype.h> 
#define TARGET_SIZE 1024
#define DEPENDENCY_SIZE 16
#define COMMAND_SIZE 16

struct list{
	struct list* next;
	char* target;
	char **dependencyList;
	char **commandsList;
	int num_commands;
} list;

struct list* parse_file() {
	FILE *fp = fopen("makefile", "r");
	char *line = NULL;
	ssize_t read;
	size_t len = 0;
	char tab = '\t';
	int line_num = 0;
	char *ptr;
	int i, index;
	struct list* ans = (struct list*)malloc(sizeof(struct list));
	struct list* prev = ans;
	while ((read = getline(&line, &len, fp)) != -1) {
		line_num++;
		i = 0;
		while(isblank(line[i])) 
			i++;

		if(line[i] == '\n')
			continue;

        if(line[0] == tab) {
        	//command
    		char *str = (char*)malloc(TARGET_SIZE);
    		strcpy(str, line);
    		while(isblank(str[0]))
    			str++;
    		prev->commandsList[prev->num_commands] = str;
    		prev->num_commands++;
        }
        else if( strchr(line, ':') && (ptr = strtok(line, ":")) ) {
        	struct list* target_list = (struct list*)malloc(sizeof(struct list));
        	target_list->target = (char*)malloc(TARGET_SIZE);
        	strcpy( target_list->target, ptr); 
        	target_list->dependencyList = (char**)malloc(sizeof(char*) * DEPENDENCY_SIZE);   
        	target_list->commandsList = (char**)malloc(sizeof(char*) * COMMAND_SIZE);
        	target_list->num_commands = 0;
        	for(int i = 0; i < DEPENDENCY_SIZE; i++)
        		 target_list->dependencyList[i] = NULL;
    		for(int i = 0; i < COMMAND_SIZE; i++)
        		 target_list->commandsList[i] = NULL;
        	index = 0;
        	while(ptr) {
        		ptr = strtok(NULL, " \n");
        		if(ptr) {
	        		char *str = (char*)malloc(TARGET_SIZE);
	        		strcpy(str, ptr);
	    			target_list->dependencyList[index] = str;
	    			index++;
    			}
        	}
        	prev->next = target_list;
        	prev = target_list;
        }
        else {
        	fprintf(stderr,"Invalid line %d\n", line_num);
        	exit(1);
        }
    }
    prev->next = NULL;
    return ans->next;
}


int main() {
	struct list *target_list;
	target_list = parse_file();
	while(target_list) {
		printf("%s ", target_list->target);
		int i = 0;
		while(target_list->dependencyList[i]) {
			printf("%s ",target_list->dependencyList[i]);
			i++;
		}
		printf("\n");
		printf("Commands :\n");
		int j = 0;
		while(j < target_list->num_commands) {
			printf("%s",target_list->commandsList[j]);
			j++;
		}
		target_list = target_list->next;
	}
	return 0;
}