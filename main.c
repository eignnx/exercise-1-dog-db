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

struct dog_entry current_dog;
char input[INP_BUF_SZ];

int main()
{
    int run = 1;
    int fd;

    while (run) {
        
        switch (pr_menu()) {
            case '1': { // Add dog.
                print_header("ADD DOG RECORD");

                dog_init_form(&current_dog);
                print_header("RECORD TO BE ADDED");
                print_dog(&current_dog);

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
            case '2': // Change dog.
                break;
            case '3': // Delete dog.
                break;
            case '4': // View all dogs.
                break;
            case '5': { // Search for a dog.
                print_header("SEARCH FOR DOG RECORD");
                prompt("Enter a dog's name to search for.");
                strncpy(current_dog.name, input, SZ_NAME);

                fd = safe_open(
                    DB_FILE_NAME,
                    O_RDONLY,
                    "Could not find db file"
                );

                int loc = find_dog(fd);

                if (loc < 0) {
                    print_header("RECORD DOES NOT EXIST");
                } else {
                    print_header("RECORD FOUND");
                    print_dog(&current_dog);

                    DBG_NUM(loc);
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

    return (unsigned char)input[0];
}

//
// Prompts the user with `msg`.
// Stores input line in global `input` variable.
void prompt(const char *msg)
{
    if (msg != NULL)
        printf("%s\n", msg);

    printf("%s", PROMPT);
    printf("\n"); // Flush output somehow...

    read_line(STDIN_FD, input, INP_BUF_SZ);
}

void print_header(const char *msg)
{
    printf("\n\n-%s-\n\n", msg);
}

void print_error(const char *msg)
{
    printf("\nERROR: %s\n\n", msg);
}

void dog_init_form(struct dog_entry *dp)
{
    int val;

    print_header("ADD DOG");

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
            print_error("Please enter a valid weight [1, 2, ...].");
    }
    dp->weight = val;

    while (1) {
        prompt("Enter the dog's AGE. [years]");
        val = atoi((const char *) &input);
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

#define MAX_REC_LN_LEN 100

//
// Finds the dog record in the DB that has the same name as
// `current_dog`. Returns the offset into the DB file where
// the found dog is located, or -1 if not found.
off_t find_dog(int fd)
{
    off_t line_no = 0;

    int found = 0;

    char *d_name = current_dog.name;

    char line[MAX_REC_LN_LEN];
    size_t line_len;

    char *tokp;

    do {
        line_len = read_line(fd, line, MAX_REC_LN_LEN);
        tokp = strtok(line, "\t");
        
        // DBG_STR(line);
        // DBG_STR(tokp);

        if (strncmp(tokp, d_name, strlen(d_name)) == 0) {
            return line_no;
        }

        line_no++;

    } while (!found && line_len > 0);

    return -1;
}

void add_dog(int fd, off_t cursor)
{
    char buf[IO_BUF_SZ];
    // char *end = buf; // Try keeping track of end, write to disk once.

    sprintf(buf, "%s\t", current_dog.name);
    write(fd, buf, strlen(buf));
    sprintf(buf, "%s\t", current_dog.breed);
    write(fd, buf, strlen(buf));
    sprintf(buf, "%s\t", current_dog.color);
    write(fd, buf, strlen(buf));
    sprintf(buf, "%d\t", current_dog.weight);
    write(fd, buf, strlen(buf));
    sprintf(buf, "%d\t", current_dog.age);
    write(fd, buf, strlen(buf));
    sprintf(buf, "%c\n", current_dog.sex);
    write(fd, buf, strlen(buf));
}
