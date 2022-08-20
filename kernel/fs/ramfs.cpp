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

    int fileToRead = 0;
    for (int i = 0; headers[i]->filename != NULL; i++)
    {
        if (mystrcmp(fileToReadName, headers[i]->filename))
        {
            fileToRead = i;
            header = headers[i];
            break;
        }
    }

    char *contents = &((char *) header)[512]; // pointer shenanigans (:

    return contents;
}

TwoStrings fullName;
TwoStrings nameInPath;

void ls()
{
    for (int i = 1; headers[i]->filename != NULL; i++)
    {        
        for (int i = 0; i < (sizeof(fullName.a) + 1); i++)
        {
            fullName.a[i] = 0;
        }
        for (int i = 0; i < (sizeof(fullName.b) + 1); i++)
        {
            fullName.b[i] = 0;
        }
        for (int i = 0; i < (sizeof(nameInPath.a) + 1); i++)
        {
            nameInPath.a[i] = 0;
        }
        for (int i = 0; i < (sizeof(nameInPath.b) + 1); i++)
        {
            nameInPath.b[i] = 0;
        }
        fullName   = strsplit(headers[i]->filename, '/');
        nameInPath = strsplit(fullName.b, '/');
        
        if (i > 1)
        {
            TwoStrings oldFullName   = strsplit(headers[i-1]->filename, '/');
            TwoStrings oldNameInPath = strsplit(oldFullName.b, '/');    
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