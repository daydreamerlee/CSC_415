/**HW2 
*	Copy.c
*	By Cheng LI
*	
**/

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>

#define BUF_SIZE 1024

int main(int argc, char const *argv[])
{
	int input_fd, output_fd; 	// Input and output file descriptor
	ssize_t bytes_in=0, bytes_out=0, bytes_read_all=0; // Num of bytes returned
	char buffer[BUF_SIZE]; // Buffer

	/*Check the proper usesage*/

	if (argc!=3)
	{
		printf("Usage: mycp file1 file2 \n"); 
		return 1;
	}

	/*Open input file*/
	input_fd = open(argv[1],O_RDONLY);
	if (input_fd<0)
	{
		perror("Open the input file");
		return 1;
	}

	/*Create the output file*/
	output_fd = open(argv[2],O_CREAT|O_TRUNC|O_WRONLY, 0644);
	if (output_fd < 0)
	{
		perror("Open the output file");
		return 2;
	}

	/*Copy process*/
	while((bytes_in=read(input_fd, buffer, BUF_SIZE)) > 0)
	{
		bytes_out = write(output_fd, buffer, (ssize_t)bytes_in);
		if (bytes_in != bytes_out)
		{
			perror("Write error");
			return 4;
		}
		bytes_read_all += bytes_out;
	}

	printf("File copy finished, %u bytes copied.\n", bytes_read_all);

	close(input_fd);
	close(output_fd);


	return 0;

}