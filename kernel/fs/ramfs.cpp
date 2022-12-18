#include "ramfs.h"
#include "../string.h"
#include <stddef.h>

struct tar_header *headers[32];

unsigned int parse()
{
    unsigned int address = *((int*)(&initramfs->address));
    unsigned int i;
 
    for (i = 0; ; i++)
    {
        struct tar_header *header = (struct tar_header *)address;
        headers[i] = header;

        if (header->filename[0] == '\0')
            break;

        unsigned int size = getsize(header->size);
        address += ((size / 512) + 1) * 512;
        
        if (size % 512)
            address += 512;
    }
    return i;
}

unsigned int getsize(const char *in)
{
    unsigned int size = 0;
    unsigned int j;
    unsigned int count = 1;
 
    for (j = 11; j > 0; j--, count *= 8)
        size += ((in[j - 1] - '0') * count);
 
    return size;
}

char *readFile(char* fileToReadName)
{
    unsigned int address = *((int*)(&initramfs->address));
    struct tar_header *header = (struct tar_header *)address;

    for (int i = 0; headers[i]->filename != "\0"; i++)
    {
        if (mystrcmp(fileToReadName, headers[i]->filename))
        {
            header = headers[i];
            break;
        }
    }

    char *contents = &((char *) header)[512]; // pointer shenanigans (:

    return contents;
}

TwoStrings fullName;
TwoStrings nameInPath;
TwoStrings oldFullName;
TwoStrings oldNameInPath;

void lsReset(TwoStrings* a, TwoStrings* b)
{
    for (int i = 0; i < 100; i++) a->a[i] = '\0';
    for (int i = 0; i < 100; i++) a->b[i] = '\0';
    for (int i = 0; i < 100; i++) b->a[i] = '\0';
    for (int i = 0; i < 100; i++) b->b[i] = '\0';
}

void ls()
{
    for (int i = 1; headers[i]->filename[0] != 0; i++)
    {   
        for (int i = 0; i < 100; i++) fullName.a[i] =      '\0';
        for (int i = 0; i < 100; i++) fullName.b[i] =      '\0';
        for (int i = 0; i < 100; i++) nameInPath.a[i] =    '\0';
        for (int i = 0; i < 100; i++) nameInPath.b[i] =    '\0';
        for (int i = 0; i < 100; i++) oldFullName.a[i] =   '\0';
        for (int i = 0; i < 100; i++) oldFullName.b[i] =   '\0';
        for (int i = 0; i < 100; i++) oldNameInPath.a[i] = '\0';
        for (int i = 0; i < 100; i++) oldNameInPath.b[i] = '\0';

        fullName   = strsplit(headers[i]->filename, '/');
        nameInPath = strsplit(fullName.b, '/');

        if (i > 1)
        {   
            oldFullName   = strsplit(headers[i-1]->filename, '/');
            oldNameInPath = strsplit(oldFullName.b, '/');    
            if (!mystrcmp(oldNameInPath.a, nameInPath.a))
            {
                printf("%s\n", nameInPath.a);
            }
        }
        else
        {
            printf("%s\n", nameInPath.a);
        }
    }
}