/*
    my_utilities.h
*/

//#include <unistd.h>     // read write
#include <stdio.h>      // perror
#include <fcntl.h>      // open

int safe_open(char *, int);
void strip_nl(char *);

int safe_open(char *fname, int flags)
{
    int fd = open(fname, flags);

    if (fd == -1)
    {
        perror("Could not find database file");
        exit(-1);
    }

    return fd;
}

void strip_nl(char *str)
{
    size_t len = strlen(str);
    if (str[len - 1] == '\n')
        str[len - 1] = '\0';
}
