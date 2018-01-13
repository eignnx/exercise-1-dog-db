#include <stdio.h>

#define SZ_NAME 32
#define SZ_BREED 32
#define SZ_COLOR 16

struct dog_entry
{
    char name[SZ_NAME];
    char breed[SZ_BREED];
    char color[SZ_COLOR];
    unsigned short weight;
    unsigned char age;
    char sex;
};

#define REC_SIZE sizeof(struct dog_entry)

void print_dog(struct dog_entry *dp)
{
    printf("NAME:   %s\n", dp->name);
    printf("BREED:  %s\n", dp->breed);
    printf("COLOR:  %s\n", dp->color);
    printf("WEIGHT: %d\n", dp->weight);
    printf("AGE:    %d\n", dp->age);
    printf("SEX:    %c\n", dp->sex);
}