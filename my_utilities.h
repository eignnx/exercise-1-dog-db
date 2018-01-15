/*
    my_utilities.h
*/

#include <stdio.h>      // printf, perror
#include <fcntl.h>      // open

#define _DBG_(fmt_str, value) printf("[%s:%d: %s = " fmt_str "]\n", __FILE__, __LINE__, #value, value)
#define DBG_STR(value) _DBG_("\"%s\"", value)
#define DBG_NUM(value) _DBG_("%d", value)
#define DBG_CHAR(value) _DBG_("'%c'", value)

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
