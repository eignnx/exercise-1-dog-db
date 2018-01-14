/*
    my_utilities.h
*/

#include <unistd.h>     // read write
#include <stdio.h>      // perror
#include <fcntl.h>      // open

int safe_open(char *, int, const char *);
void strip_nl(char *);
size_t read_line(int, char *, size_t);

int safe_open(char *fname, int flags, const char *msg)
{
    int fd = open(fname, flags);

    if (fd < 0) {
        perror(msg);
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

// Byte-by-byte reader of a single `\n` terminated line from `fd`.
// Returns the `strlen` of `dest` once finished.
size_t read_line(int fd, char *dest, size_t dest_cap)
{
    char c;

    char *destp = dest;
    char *last = dest + dest_cap - 1; // Reserve space for null terminator.

    while (read(fd, (void *) &c, 1) != 0)
        if (destp > last) {
            perror("Destination string out of space.");
            close(fd);
            exit(-1);
        } else if (c == '\n') {
            *destp = '\0';
            return destp - dest;
        } else {
            *destp = c;
            destp++;
        }
}
