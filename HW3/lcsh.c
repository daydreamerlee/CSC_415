/* CSC 415 HW3  SHELL*/

#include <sys/stat.h>
#include <sys/wait.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define BUFFER_SIZE 1024
#define ARGV_NUM_LIMIT 4
#define READ_ERROR 1
#define Empty_Input 2
#define LARGER_THAN_BUFFER_INPUT 3
#define DELIMITER " \t\n"


void lcsh();
int prompt_and_readinput(char*);
int parse(char*,char**);

int main(int argc, char const *argv[])
{
	lcsh();
	return 0;
}

void lcsh(){
	pid_t pid;
	int process_status;
	int input_status;
	int parse_status;
	char command_input[BUFFER_SIZE]; // User input Commands
	char* commands[ARGV_NUM_LIMIT+1]; // parsed command and arguments

	while(1)
	{
		/*Prompt_ And_ Input*/
		input_status = prompt_and_readinput(command_input);

		if (input_status == READ_ERROR)
		{
			fprintf(stderr, "Fail to read from stdin: %s\n", strerror(errno));
			continue;
		}

		if (input_status == Empty_Input)
		{
			fprintf(stderr, "Empty Commands !\n");
			continue;
		} 
		if (input_status == LARGER_THAN_BUFFER_INPUT)
		{
			fprintf(stderr, "Input is larger than BUFFER_SIZE, truncated input will be used, be careful!\n");
		}
		
		/*Parse the arguments*/
		parse_status = parse(command_input,commands);
		// printf("%d \n",parse_status); // for debug

		if (parse_status > ARGV_NUM_LIMIT)
		{
			fprintf(stderr, "%d arguments, Too Many Arguments!\n", parse_status);
			continue;
		}

		/*Execute commands*/
		if ((strcmp((command_input),"quit")==0) || (strcmp((command_input),"exit")==0))
		{
			break;
		}
	
		/*Fork and Exec*/	
		pid = fork();
		if(pid <0)
		{
			perror("Fork Failed:");
			exit(1);
		}

		if(pid == 0)
		{
			if(execvp(commands[0],commands)!=0)
				{
					perror("Exec Failed");
					exit(1);
				}
		}

		wait(&process_status);
	}
	printf("Shell teminated!\n");
	return;
}

int prompt_and_readinput(char* inputline){
	printf("lcsh>> ");;
	if(fgets(inputline,BUFFER_SIZE,stdin) == NULL){
			return READ_ERROR;
	}

	if(isspace(*inputline)){
		return Empty_Input;
	}

	if(inputline[BUFFER_SIZE - 1] == '\0' && inputline[BUFFER_SIZE - 2] != '\n'){ // still some extra data in stdin, flush the stdin
			int ch;
			while ((ch = fgetc(stdin)) != '\n' && ch != EOF) {
    }
			return LARGER_THAN_BUFFER_INPUT;
	}

	return 0;
}

int parse(char* inputline, char ** parsed_cmds){
	int args=0;
	char* cmd;
	cmd = strtok(inputline,DELIMITER);
	while (cmd != NULL){
		parsed_cmds[args] = cmd;
		cmd = strtok (NULL,DELIMITER);
		args++;
	}
	parsed_cmds[args] = cmd;
	return args;
}