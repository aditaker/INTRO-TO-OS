#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define BUFFR_SIZE 1024
void replace_space_with_asterisk(char* input) {
	int i = 0;
	int len = strlen(input);
	while(i != len) {
		if(input[i] == ' ') 
			input[i] = '*';
		i++;
	}
}

void convert_lowercase_to_uppercase(char* input) {
	int i = 0;
	int len = strlen(input);
	while(i != len) {
		if(input[i] >= 97 && input[i] <= 122) 
			input[i] = input[i] - 32;
		i++;
	}
}

int main() {
	char input[BUFFR_SIZE] = "HEllo! What's up? ";
	replace_space_with_asterisk(input);
	printf("%s\n",input);
	convert_lowercase_to_uppercase(input);
	printf("%s\n",input);
	return 0;
}