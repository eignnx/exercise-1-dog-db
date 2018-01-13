#include<unistd.h>

#define SZ_NAME 32
#define SZ_BREED 32
#define SZ_COLOR 16

struct dog_entry {
   char name [SZ_NAME];
   char breed [SZ_BREED];
   char color [SZ_COLOR];
   unsigned short weight;
   unsigned char age;
   char sex;
};

unsigned char pr_menu(void); // Print menu
void add_dog(int, off_t);    // Passed an fd and offset in file
int del_dog(int, off_t);     // ditto; returns new fd
void view_dog(int);          // Displays all dogs in DB
off_t find_dog(int);         // returns offset into the file

#define REC_SIZE sizeof(struct dog_entry)