#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>      // isdigit

#include "my_utilities.h"
#include "dog_entry.h"

#define INP_BUF_SZ 100
#define RD_BUF_SZ 4096

#define DB_FILE_NAME "database.dog"

#define PROMPT "ENTER: "

void prompt(char *msg);
void strip_nl(char *str);

void dog_init_form(struct dog_entry *);

unsigned char pr_menu(void); // Print menu
off_t find_dog(int);         // returns offset into the file
void add_dog(int, off_t);    // Passed an fd and offset in file
int del_dog(int, off_t);     // ditto; returns new fd
void view_dog(int);          // Displays all dogs in DB

unsigned char pr_menu()
{
    printf("\n");
    printf("ENTER%5d %s\n", 1, "to add,");
    printf("%10d %s\n", 2, "to change,");
    printf("%10d %s\n", 3, "to delete,");
    printf("%10d %s\n", 4, "to view,");
    printf("%10d %s\n", 5, "to search,");
    printf("%10d %s\n", 6, "to exit");
    printf("%s", PROMPT);

    char c = getchar();
    getchar(); // Skip new line. 
    return (unsigned char)c;
}

struct dog_entry *current_dog = NULL;
char input[INP_BUF_SZ];

int main()
{
    int run = 1;

    while (run) {
        
        switch (pr_menu()) {
            case '1': {
                struct dog_entry dog;
                dog_init_form(&dog);
                print_dog(&dog);
            }
                break;
            case '2':
                break;
            case '3':
                break;
            case '4':
                break;
            case '5':
                break;
            case '6':
                printf("\nExiting...\n");
                run = 0;
                break;
            default:
                printf("\nInvalid input!\n");
                break;
        }
    }

    exit(0);
}

void dog_init_form(struct dog_entry *dp)
{
    int val;

    prompt("Enter the dog's NAME.");
    strncpy((char *) &dp->name, (const char *) &input, SZ_NAME);

    prompt("Enter the dog's BREED.");
    strncpy((char *) &dp->breed, (const char *) &input, SZ_BREED);

    prompt("Enter the dog's COLOR.");
    strncpy((char *) &dp->color, (const char *) &input, SZ_COLOR);

    while (1) {
        prompt("Enter the dog's WEIGHT. [pounds]");
        val = atoi((const char *) &input);
        if (val > 0)
            break;
        else
            printf("\nERROR: Please enter a valid weight [1, 2, ...].\n");
    }
    dp->weight = val;

    while (1) {
        prompt("Enter the dog's AGE. [years]");
        val = atoi((const char *) &input);
        if (val >= 0)
            break;
        else
            printf("\nERROR: Please enter a valid age [0, 1, 2, ...].\n");
    }
    dp->age = val;

    while (1) {
        prompt("Enter the dog's SEX. [M/F]");
        val = toupper(input[0]);
        if (val == 'M' || val == 'F')
            break;
        else
            printf("\nERROR: Please enter a valid sex character [M or F].\n");
    }
    dp->sex = (char) val;
}

//
// Prompts the user with `msg`.
// Stores input line in global `input` variable.
void prompt(char *msg)
{
    if (msg != NULL)
        printf("%s\n", msg);
    
    printf("%s", PROMPT);

    fgets(input, INP_BUF_SZ, stdin);
    strip_nl(input);
}

// 
//
//
//
off_t find_dog(int fd)
{
    off_t rec_start = 0;

    char filebuf[RD_BUF_SZ];
    int bytes_read;

    // STUB!
    // while ((bytes_read = read(fd, filebuf, RD_BUF_SZ)) != 0)
    //     for (int i = 0; i < bytes_read; i++)
    //         putchar(filebuf[i]);

    return rec_start;
}

void add_dog(int fd, off_t cursor)
{

}
