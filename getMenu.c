#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include <unistd.h> 
#include <sys/wait.h> 
#include <fcntl.h>
#include<time.h>
#include <dirent.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>


int main(int argc, char** args)
{
	int rest, rbytes;
	char filename[30], input[1024] = {0};
	if (args[1] == NULL)
	{
		printf("AdvShell: expected arguments after getMenu\n");
		return(-1);
	}
	else
	{
		strcpy(filename, args[1]);
		strcat(filename, ".txt");
		if ((rest = open(filename, O_RDONLY)) == -1) { //open file
			perror("open wr");
			return(-1);
		}
		if ((rbytes = read(rest, input, 1024)) == -1) {//read from file
			perror("read 1");
			return(-1);
		}
		while (rbytes > 0)//take input until the end of file
		{
			printf("%s", input);
			if ((rbytes = read(rest, input, 1024)) == -1) {//read from file
				perror("read 1");
				return(-1);
			}
		}
	}
	close(rest);
	return 1;
}