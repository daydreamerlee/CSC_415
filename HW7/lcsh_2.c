/* CSC 415 HW7  improved SHELL*/

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
#define COMMAND_LENGTH 15
#define READ_ERROR 1
#define Empty_Input 2
#define LARGER_THAN_BUFFER_INPUT 3
#define DELIMITER " \t\n"


void lcsh();
int prompt_and_readinput(char*);
int parse(char*,char**,int*);
void copyArray (char**, char**, int ,int);

int main(int argc, char const *argv[])
{
	lcsh();
	return 0;
}

void lcsh(){
	pid_t pid;
	int process_status;
	int input_status;
	int total_index, special_index;
	char command_input[BUFFER_SIZE]; // User input Commands
	char* special_cmd;
	char* commands[COMMAND_LENGTH]; // parsed command and arguments
	char* cmd_firstpart[COMMAND_LENGTH];
	char* cmd_secondpart[COMMAND_LENGTH];

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
		
		/*Parse the arguments*/
		special_index = parse(command_input,commands, &total_index);
		special_cmd = commands [special_index];
		printf("%d %d %s\n",total_index, special_index,special_cmd); // for debug

		/*Save the first part and second part of the cmd, splitting at the position of special index*/
		if(special_index!=0){
				copyArray(commands, cmd_firstpart, 0, special_index);
				copyArray(commands, cmd_secondpart, special_index+1, total_index - special_index);
				cmd_firstpart[special_index]=NULL;
				cmd_secondpart[total_index - special_index]=NULL;
				printf("%s %s\n",commands[special_index], commands[special_index+1]); // for debug
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
			
			if(strcmp(special_cmd,"&")==0){

			}else if(strcmp(special_cmd,">")==0){

			}else if(strcmp(special_cmd,"<")==0){

			}else if(strcmp(special_cmd,">>")==0){

			}else if(strcmp(special_cmd,"|")==0){

			}

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

	return 0;
}

int parse(char* inputline, char ** parsed_cmds, int* total_index){
	int args=0;
	int special_index=0;
	char* cmd;
	cmd = strtok(inputline,DELIMITER);
	while (cmd != NULL){
		parsed_cmds = realloc (parsed_cmds,(args+2) * sizeof(char*));
		parsed_cmds[args] = cmd;
		if(strcmp(cmd,">")==0 || strcmp(cmd,"<")==0 
				|| strcmp(cmd,"|")==0 || strcmp(cmd,"&")==0
				|| strcmp(cmd,">>")==0)
		{
			special_index = args;
		}
		cmd = strtok (NULL,DELIMITER);
		args++;
	}
	parsed_cmds[args] = cmd;
	*total_index = args;
	return special_index;
}

void copyArray (char** input_array, char** output_array, int begin_index, int length){
	int i;
	for (i = 0; i < length; ++i)
	{
		output_array[i] = input_array[begin_index+i];
	}
}