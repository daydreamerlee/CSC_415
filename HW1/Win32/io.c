#include <windows.h>
#include <stdio.h>

#define MY_NAME "Cheng Li"

void main(int argc, char const *argv[])
{	
	char oputput_string[35];

	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	snprintf(oputput_string,35,"Hello 415, I am %s ! \n", MY_NAME);
	WriteFile(handle,oputput_string,35,NULL,NULL);

	/* code */
	
}