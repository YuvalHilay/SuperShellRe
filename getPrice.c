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
    char buff[1024], restname[1024], product[1024], number[100], dishword[100], temp[100];
    int rest, rbytes, i = 0, j = 0, k = 0, remain;
    if (args[2] == NULL)
    {
        printf("AdvShell: expected arguments after getPrice\n");
        return(-1);
    }
    else
    {
        strcpy(restname, args[1]);
        strcat(restname, ".txt");
        if ((rest = open(restname, O_RDONLY)) == -1) { //open file
            perror("Restaurant Not Found!");
            return(-1);
        }
        strcpy(product, args[2]);
        if ((rbytes = read(rest, buff, 1024)) == -1) {//read from file
            perror("read 1");
            return(-1);
        }
        while (rbytes > 0) {
            remain = rbytes;
            i = 0;
            while (remain > 0) {
                k = 0;
                j = 0;
                strcpy(dishword, "");
                while (buff[i] != '\n' && buff[i] != '.') {
                    if (buff[i] == ' ' && strlen(dishword) == 0)
                    {
                        i++;
                        remain--;
                        continue;
                    }
                    dishword[j] = buff[i];
                    j++;
                    i++;
                    remain--;
                }
                dishword[j - 1] = 0;
                while (buff[i] != '\n') {
                    if (buff[i] >= 48 && buff[i] <= 57) {
                        number[k] = buff[i];
                        k++;
                    }
                    i++;
                    remain--;
                }
                number[k] = 0;
                if (args[3] != NULL)
                {
                    strcpy(temp, "");
                    strcat(temp, args[2]);
                    strcat(temp, " ");
                    strcat(temp, args[3]);
                }
                else
                {
                    strcpy(temp, "");
                    strcat(temp, args[2]);
                }
                if (strcmp(dishword, temp) == 0) {
                    printf("%s NIS\n", number);
                    return 0;
                }
                i++;
                remain--;


            }
            if ((rbytes = read(rest, buff, 1024)) == -1) {//read from file
                perror("read 1");
                return(-1);
            }
        }
        close(rest);
    }
    printf("dish Not Found! \n");
    return -1;

}