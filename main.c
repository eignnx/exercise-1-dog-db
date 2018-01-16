#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>      // isdigit

#include "my_utilities.h"
#include "dog_entry.h"

#define STDIN_FD 0

#define INP_BUF_SZ 100
#define IO_BUF_SZ 4096

#define DB_FILE_NAME "database.dog"
#define MAX_RECORD_NUM 1000

#define PROMPT "ENTER: "

void prompt(const char *msg);
void strip_nl(char *str);

void print_header(const char *);
void print_error(const char *);

void dog_init_form(struct dog_entry *);

unsigned char pr_menu(void); // Print menu
off_t find_dog(int);         // returns offset into the file
void add_dog(int, off_t);    // Passed an fd and offset in file
int del_dog(int, off_t);     // ditto; returns new fd
void view_dog(int);          // Displays all dogs in DB

void load_dog(int, off_t);

char input[INP_BUF_SZ];

struct dog_entry temp_dog;

int main()
{
    int run = 1;
    int fd;

    while (run) {
        
        switch (pr_menu()) {
            
            case '1': { // Add dog.
                print_header("ADD DOG RECORD");

                print_header("ADD DOG");

                dog_init_form(&temp_dog);
                print_header("RECORD TO BE ADDED");
                print_dog(&temp_dog);

                char save;

                while (1) {
                    prompt("Would you like to save this record to disk? [Y/N]");
                    save = toupper(input[0]);
                    if (save == 'Y' | save == 'N')
                        break;
                    else
                        print_error("Please enter a valid response [Y, N].");
                }

                if (save == 'Y') {
                    fd = safe_open(
                        DB_FILE_NAME,
                        O_CREAT | O_WRONLY,
                        "Could not find/create db file"
                    );

                    lseek(fd, (off_t) 0, SEEK_END);
                    add_dog(fd, (off_t) 0);
                    close(fd);

                    print_header("RECORD SAVED");
                } else
                    print_header("RECORD DISCARDED");
            }
                break;
            
            case '2': { // Change dog.
                prompt("Enter the NAME of the dog to be modified.");
                strncpy(temp_dog.name, input, SZ_NAME);

                int fd = safe_open(
                    DB_FILE_NAME,
                    O_RDWR | O_CREAT,
                    "Could not open DB file"
                );

                off_t loc = find_dog(fd);
            
                if (loc < 0) {
                    print_header("RECORD DOES NOT EXIST");
                    printf("No dog named '%s' in database.\n", temp_dog.name);
                } else {
                    load_dog(fd, loc);

                    print_header("CURRENT RECORD STORED");
                    print_dog(&temp_dog);
                    print_header("ENTER REPLACEMENT DATA");
                    dog_init_form(&temp_dog);
                    print_header("RECORD TO BE ADDED");
                    print_dog(&temp_dog);
                    prompt("Are you sure you want to overwrite previous data? [Y/N]");

                    int save;
                    while (1) {
                        prompt("Would you like to save this record to disk? [Y/N]");
                        save = toupper(input[0]);
                        if (save == 'Y' | save == 'N')
                            break;
                        else
                            print_error("Please enter a valid response [Y, N].");
                    }

                    if (save == 'Y') {
                        lseek(fd, (off_t) 0, SEEK_SET);
                        add_dog(fd, loc);
                        print_header("RECORD SAVED");
                    } else
                        print_header("RECORD DISCARDED");
                }

                close(fd);
            }
                break;
            
            case '3': {// Delete dog.
                print_header("FEATURE NOT IMPLEMENTED");
            }
                break;
            
            case '4': { // View all dogs.

                int fd = safe_open(
                    DB_FILE_NAME,
                    O_CREAT | O_RDONLY,
                    "Could not open DB file"
                );

                print_header("SHOWING ALL RECORDS IN DATABASE");

                view_dog(fd);
                close(fd);
            }
                break;
            
            case '5': { // Search for a dog.
                print_header("SEARCH FOR DOG RECORD");
                prompt("Enter a dog's NAME to search for.");
                strncpy(temp_dog.name, input, SZ_NAME);

                fd = safe_open(
                    DB_FILE_NAME,
                    O_RDONLY,
                    "Could not find db file"
                );

                int loc = find_dog(fd);

                if (loc < 0) {
                    print_header("RECORD DOES NOT EXIST");
                    printf("No dog named '%s' in database.", temp_dog.name);
                } else {
                    print_header("RECORD FOUND");
                    load_dog(fd, loc);
                    print_dog(&temp_dog);
                }

                close(fd);
            }
                break;

            case '6': // Exit program.
                printf("\nExiting...\n");
                run = 0;
                break;
            
            default:
                print_error("Invalid input!");
                break;
        }
    }

    exit(0);
}

unsigned char pr_menu()
{
    printf("\n");
    printf("ENTER%5d %s\n", 1, "to add,");
    printf("%10d %s\n", 2, "to change,");
    printf("%10d %s\n", 3, "to delete,");
    printf("%10d %s\n", 4, "to view,");
    printf("%10d %s\n", 5, "to search,");
    printf("%10d %s\n", 6, "to exit");
    prompt(NULL);

    return (unsigned char) input[0];
}

//
// Prompts the user with `msg`. Stores null-terminated
// input line in global variable `input`.
void prompt(const char *msg)
{
    if (msg != NULL)
        printf("%s\n", msg);

    printf("%s", PROMPT);

    fgets(input, INP_BUF_SZ, stdin);
    strip_nl(input);
}

void print_header(const char *msg)
{
    printf("\n\n-%s-\n\n", msg);
}

void print_error(const char *msg)
{
    printf("\nERROR: %s\n\n", msg);
}

//
// Given a pointer to a `dog_entry`, initializes
// the dog based on user input.
void dog_init_form(struct dog_entry *dp)
{
    int val;

    prompt("Enter the dog's NAME.");
    strncpy((char *) dp->name, (const char *) input, SZ_NAME);

    prompt("Enter the dog's BREED.");
    strncpy((char *) dp->breed, (const char *) input, SZ_BREED);

    prompt("Enter the dog's COLOR.");
    strncpy((char *) dp->color, (const char *) input, SZ_COLOR);

    while (1) {
        prompt("Enter the dog's WEIGHT. [pounds]");
        val = atoi((const char *) input);
        if (val > 0)
            break;
        else
            print_error("Please enter a valid weight [1, 2, ...].");
    }
    dp->weight = val;

    while (1) {
        prompt("Enter the dog's AGE. [years]");
        val = atoi((const char *) input);
        if (val >= 0)
            break;
        else
            print_error("Please enter a valid age [0, 1, 2, ...].");
    }
    dp->age = val;

    while (1) {
        prompt("Enter the dog's SEX. [M/F]");
        val = toupper(input[0]);
        if (val == 'M' || val == 'F')
            break;
        else
            print_error("Please enter a valid sex character [M or F].");
    }
    dp->sex = (char) val;
}

//
// Finds the dog record in the DB that has the same name as
// `temp_dog`. Returns the offset into the DB file where
// the found dog is located, or -1 if not found.
off_t find_dog(int fd)
{
    off_t off = 0;
    char name[SZ_NAME];
    strncpy(name, temp_dog.name, SZ_NAME);

    while (read(fd, (void *) &temp_dog, REC_SIZE) == REC_SIZE)
        if (strncmp(temp_dog.name, name, SZ_NAME) == 0)
            return off;
        else
            off += REC_SIZE;

    strncpy(temp_dog.name, name, SZ_NAME); // So that search term can be referenced later
    return (off_t) -1;
}

//
//
// Adds copy of `temp_dog` to DB at offset `off`.
void add_dog(int fd, off_t off)
{
    lseek(fd, off, SEEK_CUR);
    write(fd, (void *) &temp_dog, REC_SIZE);
}

//
//
// Loads the record at offset `off`, stores value in `temp_dog`.
void load_dog(int fd, off_t off)
{
    lseek(fd, off, SEEK_SET);
    read(fd, (void *) &temp_dog, REC_SIZE);
}

//
//
// Displays all dog records in the DB.
void view_dog(int fd)
{
    while (read(fd, (void *) &temp_dog, REC_SIZE) == REC_SIZE)
        print_dog(&temp_dog);
}
