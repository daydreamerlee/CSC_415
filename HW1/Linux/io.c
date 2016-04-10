#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#define  MY_NAME "Cheng Li"

int main(int argc, char const *argv[])
{
	char output_string[35];
	snprintf(output_string,35,"Hello 415, My name is %s !\n", MY_NAME);	
	write(1,output_string, 35);/* code */

	
	return 0;
}