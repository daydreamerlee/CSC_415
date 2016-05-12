/*
  =====================================================================================================
  Course:			CSC 415 Operating System Principles, Spring 2016
  Instructor:		John Baldwin
  Author:			Xiaoyu Zhou (SFSU ID: 916403532)
  Assignment6: 		This program is to extend shell/command-line interpreter from HW3. The new
				functionality include executing commands in the background using the & symbol; 
				redirecting the standard input, standard output and standard error streams to 
				or from files using the <, > and 2> symbols; 
				redirecting and appending the standard output and standard error streams to files 
				using the >> and 2>> symbols; 
				piping the standard output of one process to the standard input of another using
				the | symbol. 
				To simplify parsing, it's assumed that the symbols are separated from other arguments 
				by whitespace. 
  =====================================================================================================
*/

#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

#define BUFFERSIZE 1024
#define MAX_TOKENS 8

int n_parameters;
char path[BUFFERSIZE];
int background = 0; //0: foreground  1: background
char * fileName;

int fd; //file descriptor for redirection
int fd_replaced; //standard input/output that'll be replaced
int fd_backup; //backup of file descriptor that'll be replaced

int exit_status;

typedef enum EXEC_TYPE {
	NULL_TYPE, 		//do nothing
	BASIC,  		//without redirect or background
	FILE_TO_STDIN, 		//redirecting the standard input from files using <
	STDOUT_TO_FILE, 	//redirecting the standard output to files using >
	STDERR_TO_FILE, 	//redirecting the standard error streams to files using 2>
	STDOUT_APPENDTO_FILE, 	//redirecting and appending standard output to files using >>
	STDERR_APPENDTO_FILE, 	//redirecting and appending standard error streams to files using 2>>
} EXEC_TYPE;

EXEC_TYPE exec_type;

void check_error(int return_value, char* error_message) {
    if(return_value < 0) {
    	perror(error_message);
		exit(1);
    }
}

EXEC_TYPE determine_whato_exec(char ** args) {
    EXEC_TYPE exec_type = BASIC;
    if(n_parameters == 0) {
    	return NULL_TYPE;
    } 
    if(n_parameters == 1) {
    	return BASIC;
    }
    if(strcmp(args[n_parameters-1], "&") == 0) {
    	background = 1;
    	args[n_parameters-1] = NULL;
    	n_parameters--;
    }
    if(n_parameters < 3) {
    	return BASIC;
    }
    fileName = args[n_parameters - 1];
    if(strcmp(args[n_parameters-2], "<") == 0) {
    	exec_type = FILE_TO_STDIN;
    } else if(strcmp(args[n_parameters-2], ">") == 0) {
    	exec_type = STDOUT_TO_FILE;
    } else if(strcmp(args[n_parameters-2], "2>") == 0) {
    	exec_type = STDERR_TO_FILE;
    } else if(strcmp(args[n_parameters-2], ">>") == 0) {
    	exec_type = STDOUT_APPENDTO_FILE;
    } else if(strcmp(args[n_parameters-2], "2>>") == 0) {
    	exec_type = STDERR_APPENDTO_FILE;
    }

    if(exec_type != BASIC) {
    	args[n_parameters-2] = NULL;
    }

    return exec_type;
}

void open_file() {
	if(exec_type == FILE_TO_STDIN) {
		fd = open(fileName, O_RDONLY);
	}
	if(exec_type == STDOUT_TO_FILE || exec_type == STDERR_TO_FILE) {
		fd = open(fileName, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
	}
	if(exec_type == STDOUT_APPENDTO_FILE || exec_type == STDERR_APPENDTO_FILE) {
		fd = open(fileName, O_WRONLY | O_CREAT | O_APPEND);
	}

}
void replace_std_io() {
	if(exec_type == FILE_TO_STDIN) {
		fd_replaced = STDIN_FILENO;
	}
	if(exec_type == STDOUT_TO_FILE || exec_type == STDOUT_APPENDTO_FILE) {
		fd_replaced = STDOUT_FILENO;
	}
	if(exec_type == STDERR_TO_FILE || exec_type == STDERR_APPENDTO_FILE) {
		fd_replaced = STDERR_FILENO;
	}

	fd_backup = dup(fd_replaced);
	check_error(fd_backup, "error in dup(fd_replaced)");
	check_error(dup2(fd, fd_replaced), "error in dup2(fd, fd_replaced)");
}

int main(int argc, char ** argv) {
    char *cmd, *args[MAX_TOKENS];
    int index = 0;
    int exit_status;

    while(1)  {
		printf("Xiaoyu's Shell: ");

		if(fgets(path, BUFFERSIZE, stdin) == NULL || *path == '\n') {
		    printf("Empty command.\n");
		    continue;
		}

		//VERY IMPORTANT TO RESET INDEX TO 0!!
		index = 0;
		cmd = strtok(path, "  \t\n");
		if(strcmp(cmd, "exit") == 0) {
		    break;
		}
		while(cmd != NULL && index < (MAX_TOKENS-1)) {
		    args[index++] = cmd;
		    cmd = strtok(NULL, " \t\n");
		}
		args[index] = NULL;
		n_parameters = index;
		
		if(index == (MAX_TOKENS-1) && cmd != NULL) {
		    printf("Too many arguments\n");
		    cmd = NULL;
		    continue;
		}

		exec_type = determine_whato_exec(args);

		if(exec_type == NULL_TYPE) {
			continue;
		}

		if(exec_type != BASIC) {
			open_file();
			if(fd < 0) {
				printf("error in open_file()\n");
				continue;
			}

			replace_std_io();
		}

		background = 0;	  //initially, in foreground
		pid_t pid;
		// int status = 0;
		pid = fork();
		if(pid < 0) {
		    printf("fork fails\n");
		    return 0;
		}
		else if(pid == 0) { //in child process
		    // int result = execvp(args[0], args);
		    // status = 1;
		    // return result;
		    check_error(execvp(args[0], args), "error in execvp method");
		}
		else { //in parent process
			if(background == 0) {
				check_error(waitpid(pid, &exit_status, 0), "error in waitpid() method");
			}
			else {
				printf("run in background pid is: %d\n", pid);
			}
		    

		    if(exec_type != BASIC) {
		    	check_error(dup2(fd_backup, fd_replaced), "error in dup2 method");
		    	check_error(close(fd), "error in close(fd)");
		    	check_error(close(fd_backup), "error in close(fd_backup)");
		    }

		}
    }
    return 0;
}
