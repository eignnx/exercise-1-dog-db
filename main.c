#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<stdlib.h>
#include "dog_entry.h"

#define INP_BUF_SZ 100

void prompt(char *msg);
void strip_ln(char *str);

unsigned char pr_menu(void); // Print menu
void add_dog(int, off_t);    // Passed an fd and offset in file
int del_dog(int, off_t);     // ditto; returns new fd
void view_dog(int);          // Displays all dogs in DB
off_t find_dog(int);         // returns offset into the file

int main()
{
    char input[INP_BUF_SZ];
    
    prompt("Enter a command.");
    while (fgets(input, INP_BUF_SZ, stdin) != NULL) {
        strip_ln(input);
        
        printf(">> '%s'\n", input);
        
        prompt(NULL);
    }
}

void prompt(char *msg)
{
    if (msg != NULL)
        printf("%s\n", msg);
    
    printf("=> ");
}

void strip_ln(char *str)
{
    size_t len = strlen(str);
    if (str[len - 1] == '\n')
        str[len - 1] = '\0';
}