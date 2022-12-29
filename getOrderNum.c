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
    char path[30];
    DIR* dir;
    struct dirent* ent;
    long count = 0;
    if (args[1] == NULL)
    {
        printf("AdvShell: expected arguments after getMenu\n");
        return(-1);
    }
    strcpy(path, "/home/braude/");
    strcat(path, args[1]);
    strcat(path, "_Order");
    if ((dir = opendir(path)) == NULL) {
        perror("Cannot open .");
        return(-1);
    }
    while ((ent = readdir(dir)) != NULL)
    {
        count = count + 1;// add when recognized file or father folder
    }
    closedir(dir);
    printf("%ld orders\n", count - 2);//count all txt files without 2 folders of father
    return 1;
}