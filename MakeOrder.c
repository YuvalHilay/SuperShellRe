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

//functios for MakeOrder
// Function to swap two numbers
void swap(char* x, char* y) {
    char t = *x; *x = *y; *y = t;
}

// Function to reverse `buffer[i…j]`
char* reverse(char* buffer, int i, int j)
{
    while (i < j) {
        swap(&buffer[i++], &buffer[j--]);
    }

    return buffer;
}
// Iterative function to implement `itoa()` function in C
char* itoa(int value, char* buffer, int base)
{
    // invalid input
    if (base < 2 || base > 32) {
        return buffer;
    }

    // consider the absolute value of the number
    int n = abs(value);

    int i = 0;
    while (n)
    {
        int r = n % base;

        if (r >= 10) {
            buffer[i++] = 65 + (r - 10);
        }
        else {
            buffer[i++] = 48 + r;
        }

        n = n / base;
    }

    // if the number is 0
    if (i == 0) {
        buffer[i++] = '0';
    }
    // If the base is 10 and the value is negative, the resulting string
       // is preceded with a minus sign (-)
       // With any other base, value is always considered unsigned
    if (value < 0 && base == 10) {
        buffer[i++] = '-';
    }

    buffer[i] = '\0'; // null terminate string

    // reverse the string and return it
    return reverse(buffer, 0, i - 1);
}

//func for MakeOrder
int getPricefororder(char** args)
{
    char buff[1024], restname[1024], product[1024], number[100], dishword[100];
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
            perror("Restaurant Not Found");
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
                if (strcmp(dishword, args[2]) == 0) {
                    return atoi(number);
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
    return -1;

}

int main(int argc, char** args)
{
    char dishname[30], decision[30], place[30], buff[30], number[30], str[30] = {0}, sum1[30], ntime[30] = {0}, temp[30] = {0};
    int order, sum = 0, flag = 0, len, i, t, k, price, chenge;
    char* deli[] = { "NULL","NULL","NULL" };
    //time_t tm;
    time_t t1 = time(NULL);
    struct tm tm = *localtime(&t1);
    if (args[2] == NULL)
    {
        printf("AdvShell: expected arguments after MakeOrder\n");
        return(-1);
    }
    else
    {
        strcpy(str, "Total ِ Price: ");
        strcpy(place, args[1]);
        strcat(place, "_Order");;
        strcat(place, "/");
        strcat(place, args[2]);
        strcat(place, ".txt");
        if ((order = open(place, O_WRONLY | O_CREAT, 0664)) == -1) { //open file
            perror("Restaurant Not Found!");
            return(-1);
        }

        if (write(order, args[1], strlen(args[1])) != strlen(args[1])) { //write the x.
            perror("write title dish1");
            return(-1);
        }
        if (write(order, " Order\n\n", strlen(" Order\n\n")) != strlen(" Order\n\n")) {//write Menu
            perror("write title2");
            return(-1);
        }
        printf("Insert your order (Finish to finish):\n");
        fgets(buff, sizeof(buff), stdin);
        while (flag == 0)//get dish name + price until stop
        {
            t = 0;
            k = 0;
            len = strlen(buff);
            if (strcmp(buff, "finish") == 10 || strcmp(buff, "Finish") == 10) {
                flag = 1;
            }
            else {
                for (i = 0; i < (len - 1); i++) {
                    if (buff[i] >= 48 && buff[i] <= 57) {
                        number[t] = buff[i];
                        t++;
                    }
                    else {
                        dishname[k] = buff[i];
                        k++;
                    }
                }
                number[t] = 0;
                dishname[k - 1] = 0;
                deli[1] = args[1];
                deli[2] = dishname;
                price = getPricefororder(deli);
                if (price == (-1)) {

                }
                else {
                    sum += price * atoi(number);
                }
                if (write(order, dishname, strlen(dishname)) != strlen(dishname)) { //write the x.
                    perror("write title dish1");
                    return(-1);
                }
                if (write(order, number, strlen(number)) != strlen(number)) { //write the x.
                    perror("write title dish1");
                    return(-1);
                }
                if (write(order, "\n", strlen("\n")) != strlen("\n")) { //write the x.
                    perror("write title dish1");
                    return(-1);
                }
                strcpy(buff, "");
                fgets(buff, sizeof(buff), stdin);
            }
        }
        printf("Total Price : %d NIS (Confirm to approve / else Cancle)\n", sum);
        fgets(decision, 30, stdin);
        if (strcmp(decision, "Confirm") == 10)
        {
            printf("Order created ! // %s.txt created in %s_Order dir with Read Mode.\n", args[2], args[1]);
            strcat(str, itoa(sum, sum1, 10));
            strcat(str, "NIS\n\n");
            if (write(order, str, strlen(str)) != strlen(str)) { //write the x.
                perror("write title dish1");
                return(-1);
            }
            strcpy(ntime, itoa(tm.tm_mday, temp, 10));
            strcpy(temp, "");
            strcat(ntime, "/");
            strcat(ntime, itoa(tm.tm_mon + 1, temp, 10));
            strcpy(temp, "");
            strcat(ntime, "/");
            strcat(ntime, itoa(tm.tm_year + 1900, temp, 10));
            strcat(ntime, " \n");
            if (write(order, ntime, strlen(ntime)) != strlen(ntime)) { //write the x.
                perror("write title dish1");
                return(-1);
            }
            if (chenge = chmod(place, 0444) == -1) {
                perror("chmod f");
                return(-1);
            }
        }
        else if (strcmp(decision, "Cancle") == 10) {
            printf("Order canceled.\n");
            remove(place);
        }


        close(order);
        return -1;
    }
}