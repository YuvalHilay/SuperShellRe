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

char SHELL_NAME[50] = "AdvShell>";

// Function that read a line from command into the line
char* readl()
{
	char* line = (char*)malloc(sizeof(char) * 1024); // Dynamically Allocate line
	char c;
	int pos = 0, linesize = 1024;
	if (!line) // line Allocation Failed
	{
		printf("\nline Allocation Error.");
		exit(EXIT_FAILURE);
	}
	while (1)
	{
		c = getchar();
		if (c == EOF || c == '\n') // If End of File or New line, put /0
		{
			line[pos] = '\0';
			return line;
		}
		else
		{
			line[pos] = c;
		}
		pos++;
		// If pos bigger than line do realloc
		if (pos >= linesize)
		{
			linesize += 1024;
			line = realloc(line, sizeof(char) * linesize);
			if (!line) // line Allocation Failed
			{
				printf("\nBuffer Allocation Error.");
				exit(EXIT_FAILURE);
			}
		}
	}

}
// Function that split a line into commands
char** parse(char* line)
{
	char** tokens = (char**)malloc(sizeof(char*) * 64);
	char* token;
	char delim[10] = " \t\n\r\a";
	int pos = 0, bufsize = 64;
	if (!tokens)
	{
		printf("\nBuffer Allocation Error.");
		exit(EXIT_FAILURE);
	}
	token = strtok(line, delim);
	while (token != NULL)
	{
		tokens[pos] = token;
		pos++;
		if (pos >= bufsize)
		{
			bufsize += 64;
			line = realloc(line, bufsize * sizeof(char*));
			if (!line) // Buffer Allocation Failed
			{
				printf("\nBuffer Allocation Error.");
				exit(EXIT_FAILURE);
			}
		}
		token = strtok(NULL, delim);
	}
	tokens[pos] = NULL;
	return tokens;
}

void Error_Msg(char* str)
{
	printf("\n%s", str);
	exit(1);
}

// cd function
int AdvShell_cd(char** args)
{
	if (args[1] == NULL)
	{
		printf("AdvShell: expected argument after cd\n");
	}
	else
	{
		if (chdir(args[1]) != 0)
		{
			perror("AdvShell: ");
		}
	}
	return 1;
}

// Function to create child process and run command
int AdvShellLaunch(char** args)
{
	char buffer[30];
	pid_t pid, wpid;
	int status;
	pid = fork();
	if (strcmp(args[0], "CreateMenu") == 0 || strcmp(args[0], "getMenu") == 0 || strcmp(args[0], "getPrice") == 0 || strcmp(args[0], "getOrderNum") == 0 || strcmp(args[0], "MakeOrder") == 0)
	{
		strcpy(buffer, "./");
		strcat(buffer, args[0]);
	}
	else
	{
		strcpy(buffer, "");
		strcat(buffer, args[0]);
	}
	if (pid == 0)
	{
		// The Child Process
		if (execvp(buffer, args) == -1)
		{
			perror("AdvShell: Not Supported ");
		}
		exit(EXIT_FAILURE);
	}
	else if (pid < 0)
	{
		//Forking Error
		perror("AdvShell: ");
	}
	else
	{
		// The Parent Process
		do
		{
			wpid = waitpid(pid, &status, WUNTRACED);
		} while (!WIFEXITED(status) && !WIFSIGNALED(status));
	}
	return 1;
}

// Function to execute command from terminal
int execShell(char** args)
{
	int ret, i;
	char path[30];
	pid_t pid, wpid;
	int status;
	if (args[0] == NULL)
	{
		// Empty command
		return 1;
	}
	if (strcmp(args[0], "cd") == 0) // Check if user function matches cd
	{
		return AdvShell_cd(args); // Call AdvShell_cd
	}

	ret = AdvShellLaunch(args);
	return ret;
}

// When myShell is called Interactively
int AdvShellInteract()
{
	char* line;
	char** args;
	printf("%s> ", SHELL_NAME);
	line = readl();
	while (strcmp(line, "exit") != 0)
	{
		args = parse(line);
		execShell(args);
		free(line);
		free(args);
		printf("%s> ", SHELL_NAME);
		line = readl();
	}
	printf("Goodbye...\n");
	return 1;
}

int main(int argc, char** argv)
{
	// Parsing commands Interactive mode or Script Mode
	if (argc == 1)
		AdvShellInteract();
	else
		printf("\nInvalid Number of Arguments");

	// Exit the Shell
	return 0;
}