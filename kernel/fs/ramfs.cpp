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

void readFile(char* fileToReadName)
{
    unsigned int address = *((int*)(&initramfs->address));
    struct tar_header *header = (struct tar_header *)address;

    unsigned int size = getsize(header->size);

    int fileCount = 0;
    int fileToRead = 0;
    for (int i = 0; headers[i]->filename != NULL; i++)
    {
        fileCount++;
        if (mystrcmp(fileToReadName, headers[i]->filename))
        {
            fileToRead = fileCount;
            printf("File Found!\n");
        }
    }

    if (fileToRead == 0)
    {
        printf("File %s not found :(\n", fileToReadName);
        return;
    }

    int offset = ((((size / 512) + 1) * 512) + ((fileToRead) * 512));

    if (mystrcmp(fileToReadName, (((char *)initramfs->address) + offset)))
    {
        offset += 512;
    }
    else
    {
        for (int i = 0; headers[i]->filename != NULL; i++)
        {
            if (mystrcmp(headers[i]->filename, (((char *)initramfs->address) + offset)))
            {
                offset -= 512;
            }
        }
    }

    printf("%s", (((char *)initramfs->address) + offset));
}

void ls()
{
    for (int i = 1; headers[i]->filename != NULL; i++)
    {
        TwoStrings fullName   = strsplit(headers[i]->filename, '/');
        TwoStrings nameInPath = strsplit(fullName.b, '/');
        
        comout("\nfullName.a = ");
        comout(fullName.a);
        comout("\nfullName.b = ");
        comout(fullName.b);
        comout("\nnameInPath.a = ");
        comout(nameInPath.a);
        comout("\nnameInPath.b = ");
        comout(nameInPath.b);
        comout("\n");
        if (i > 1)
        {
            TwoStrings oldFullName   = strsplit(headers[i-1]->filename, '/');
            TwoStrings oldNameInPath = strsplit(oldFullName.b, '/');    
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