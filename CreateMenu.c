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
	char filename[30] = {0}, typedish[30] = {0}, dishname[30] = {0}, dirname[30] = {0}, typenum[10] = {0}, numdot[30] = {0};
	int i = 0, x, dir, rest;
	char* token;
	int len = 0, t = 0;
	if (args[2] == NULL)
	{
		printf("AdvShell: expected arguments after CreateMenu\n");
		return(-1);
	}
	else
	{
		strcpy(filename, args[1]);//create the string for directory and file
		strcat(filename, ".txt");
		strcpy(dirname, args[1]);
		strcat(dirname, "_Order");
		if ((dir = mkdir(dirname, 0777)) == -1)//open the dir
		{
			perror("open directory 1");
			return(-1);
		}
		if ((rest = open(filename, O_WRONLY | O_CREAT, 0664)) == -1) { //open file
			perror("open wr");
			return(-1);
		}
		if (write(rest, args[1], strlen(args[1])) != strlen(args[1])) { //write resterunt name
			perror("write title1");
			return(-1);
		}
		if (write(rest, " Menu\n\n", strlen(" Menu\n\n")) != strlen(" Menu\n\n")) {//write Menu
			perror("write title2");
			return(-1);
		}
		while (i < atoi(args[2])) //numver of time to run
		{
			typenum[0] = (char)(i + 97);// create the x. before dish type
			typenum[1] = '.';
			typenum[2] = ' ';
			if (write(rest, typenum, strlen(typenum)) != strlen(typenum)) { //write the x.
				perror("write title dish1");
				return(-1);
			}
			printf("Insert Type Dish %c:\n", (char)(i + 97)); //get type dish
			fgets(typedish, 30, stdin);
			if (write(rest, typedish, strlen(typedish)) != strlen(typedish)) {//write typedish 
				perror("write title dish1");
				return(-1);
			}
			x = 1; //count numer of dishes
			printf("Insert dish name %d:\n", x);
			while (strcmp(fgets(dishname, 30, stdin), "Stop") != 10)//get dish name + price until stop
			{
				dishname[strcspn(dishname, "\n")] = 0;
				if (write(rest, "  ", strlen("  ")) != strlen("  ")) {//write spaces
					perror("write title2");
					return(-1);
				}
				token = strtok(dishname, " ");//seprate input by spaces
				/* walk through other tokens */
				while (token != NULL) {
					if ((token[0]) > 48 && (token[0]) <= 57) {//check if  we are the price
						for (t = 0; t < (20 - len); t++)//create string we the correct anpunt of dots
						{
							strcat(numdot, ".");
						}
						strcat(numdot, " ");
						if (write(rest, numdot, strlen(numdot)) != strlen(numdot)) {//write the dots
							perror("write dish1");
							return(-1);
						}
						if (write(rest, token, strlen(token)) != strlen(token)) {//write the price
							perror("write dish1");
							return(-1);
						}
						if (write(rest, "NIS", strlen("NIS")) != strlen("NIS")) {//write the price
							perror("write dish1");
							return(-1);
						}
					}
					else
					{
						len = len + strlen(token);//count len for calc dots
						if (write(rest, token, strlen(token)) != strlen(token)) {// write token that not the price
							perror("write dish1");
							return(-1);
						}
						if (write(rest, " ", strlen(" ")) != strlen(" ")) {// write token that not the price
							perror("write dish1");
							return(-1);
						}
					}
					token = strtok(NULL, " ");
				}
				strcpy(numdot, "");//intilize dor next dish
				len = 0;
				if (write(rest, "\n", strlen("\n")) != strlen("\n")) {//write enter
					perror("write title2");
					return(-1);
				}
				x++;//count the dishes
				printf("Insert dish name %d:\n", x); //take next dish
			}
			if (write(rest, "\n", strlen("\n")) != strlen("\n")) {
				perror("write title2");
				return(-1);
			}
			i++;
		}
		if (write(rest, "\n\n\t\tBon Appetit \n", strlen("\n\n\t\tBon Appetit \n")) != strlen("\n\n\t\tBon Appetit \n")) {
			perror("write bonap");
			return(-1);
		}
		printf("Successfully created // %s created , %s dir created.\n", filename, dirname);
	}
	close(rest);
	return 1;
}