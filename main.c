#include<unistd.h>
#include "dog_entry.h"

unsigned char pr_menu(void); // Print menu
void add_dog(int, off_t);    // Passed an fd and offset in file
int del_dog(int, off_t);     // ditto; returns new fd
void view_dog(int);          // Displays all dogs in DB
off_t find_dog(int);         // returns offset into the file

