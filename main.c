#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>      // open
#include <ctype.h>      // isdigit

#include "dog_entry.h"

#define INP_BUF_SZ 100
#define RD_BUF_SZ 4096

#define PROMPT "=> "

void prompt(char *msg);
void strip_nl(char *str);

unsigned char pr_menu(void); // Print menu
void add_dog(int, off_t);    // Passed an fd and offset in file
int del_dog(int, off_t);     // ditto; returns new fd
void view_dog(int);          // Displays all dogs in DB
off_t find_dog(int);         // returns offset into the file

int main()
{
    char input[INP_BUF_SZ];
    
    
    // char filebuf[RD_BUF_SZ];
    // int bytes_read;

    // int fd = open("database.dog", O_RDWR);

    // if (fd == -1) {
    //     perror("Could not find database.dog");
    //     exit(-1);
    // }

    // while ((bytes_read = read(fd, filebuf, RD_BUF_SZ)) != 0)
    //     for (int i = 0; i < bytes_read; i++)
    //         putchar(filebuf[i]);

    
    prompt("Enter a command.");
    while (fgets(input, INP_BUF_SZ, stdin) != NULL) {
        strip_nl(input);
        
        //printf(">> '%s'\n", input);
        
        printf(">> '%c'\n", pr_menu());
        //prompt(NULL);
    }

    exit(0);
}

void prompt(char *msg)
{
    if (msg != NULL)
        printf("%s\n", msg);
    
    printf("%s", PROMPT);
}

void strip_nl(char *str)
{
    size_t len = strlen(str);
    if (str[len - 1] == '\n')
        str[len - 1] = '\0';
}

unsigned char pr_menu()
{
    printf("ENTER%5d %s\n", 1, "to add,");
    printf("%10d %s\n",     2, "to change,");
    printf("%10d %s\n",     3, "to delete,");
    printf("%10d %s\n",     4, "to view,");
    printf("%10d %s\n",     5, "to search,");
    printf("%10d %s\n",     6, "to exit");
    printf("ENTER: ");

    int c = getchar();
    int i = c - '0';
    if (isdigit(c) && i <= 6 && i >= 1)
        return (unsigned char) c;
    else
        return '\0';
}
