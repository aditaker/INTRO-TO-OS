#include "execute_command.h"
#include "file_parser.h"

//Execute the given command using fork-exec.
void executeCommand(char *command) {
    char *tmp = (char *)malloc(BUFFERSIZE);
	if(tmp == NULL) {
        printf("Malloc failed.");
        exit(1);
    }
    strcpy(tmp, command);
    printf("%s\n", command);
    pid_t pid = fork();
	if(pid == 0) {
		char *str = (char *)malloc(BUFFERSIZE);
		if(str == NULL) {
			printf("Malloc failed.");
			exit(1);
		}
        strcpy(str, command);
		char **argv = (char **)malloc(sizeof(char *) * BUFFERSIZE);
		if(argv == NULL) {
			printf("Malloc failed.");
			exit(1);
		}
		char *token = strtok(str, " ");
		if(token == NULL) {			//Command does not contain any arguments.
			execlp(command, command, NULL);	//Execute the command.
        	exit(1);
		}
		int j = 0;
		while(token != NULL) {
			argv[j++] = token;	//Create an array of arguments.
			token = strtok(NULL, " ");
		}
		argv[j] = NULL;
		execvp(argv[0], argv);	//Execute the command.
        exit(1);
	}
	else {
        int status;
		wait(&status);		//Wait for child to exit.
        if(status != 0) {
            printf("Failed to execute command: %s\n", tmp);
            exit(1);
        }
        free(tmp);
	}
}