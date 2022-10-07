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
        if (header->filename[0] == '\0')
            break;

        unsigned int size = getsize(header->size);
        headers[i] = header;
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

    for (int i = 0; headers[i]->filename != NULL; i++)
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
    for (int i = 0; i < 100; i++) a->a[i] = NULL;
    for (int i = 0; i < 100; i++) a->b[i] = NULL;
    for (int i = 0; i < 100; i++) b->a[i] = NULL;
    for (int i = 0; i < 100; i++) b->b[i] = NULL;
}

void ls()
{
    for (int i = 1; headers[i]->filename != NULL; i++)
    {   
        for (int i = 0; i < 100; i++) fullName.a[i] = NULL;
        for (int i = 0; i < 100; i++) fullName.b[i] = NULL;
        for (int i = 0; i < 100; i++) nameInPath.a[i] = NULL;
        for (int i = 0; i < 100; i++) nameInPath.b[i] = NULL;
        for (int i = 0; i < 100; i++) oldFullName.a[i] = NULL;
        for (int i = 0; i < 100; i++) oldFullName.b[i] = NULL;
        for (int i = 0; i < 100; i++) oldNameInPath.a[i] = NULL;
        for (int i = 0; i < 100; i++) oldNameInPath.b[i] = NULL;

        fullName   = strsplit(headers[i]->filename, '/');
        nameInPath = strsplit(fullName.b, '/');

        comout("fullname.a  ");
        comout(fullName.a);
        comout("\nfullname.b  ");
        comout(fullName.b);
        comout("\nnameInPath.a  ");
        comout(nameInPath.a);
        comout("\nnameInPath.b  ");
        comout(nameInPath.b);
        comout("\n");

        if (i > 1)
        {   
            oldFullName   = strsplit(headers[i-1]->filename, '/');
            oldNameInPath = strsplit(oldFullName.b, '/');    
            if (!mystrcmp(oldNameInPath.a, nameInPath.a))
            {
                printf("'%s'\n", nameInPath.a);
            }
        }
        else
        {
            printf("'%s'\n", nameInPath.a);
        }
    }
}