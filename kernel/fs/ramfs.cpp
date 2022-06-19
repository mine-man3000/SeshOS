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
        if (mystrcmp(fileToReadName, headers[i]->filename))
        {
            fileToRead = fileCount;
            printf("File Found!\n");
        }
        fileCount++;
    }

    if (fileToRead == 0)
    {
        printf("File %s not found :(\n", fileToReadName);
        return;
    }

    int offset = ((((size / 512) + 1) * 512) + ((fileToRead) * 512));

    if (mystrlen((((char *)initramfs->address) + offset)) < getsize(headers[fileToRead]->size))
    {
        offset -= 512;
    }
    else if (mystrcmp(fileToReadName, (((char *)initramfs->address) + offset)))
    {
        offset += 512;
    }
    else if (mystrcmp((((char *)initramfs->address) + offset), headers[fileToRead + 1]->filename))
    {
        for (int i = 0; headers[i]->filename != NULL; i++)
        {
            offset -= 512;
        }
    }
    //figure out how to see if I am reading 512 * 3 bytes behind and fix the offset to reflect this
    //how am i going to do this? I have 0 idea...
    
    comout("offset is 0x");
    comout(to_hstring((uint64_t)offset));
    comout("\n");
    printf("%s", (((char *)initramfs->address) + offset));
}

void ls()
{
    for (int i = 1; headers[i]->filename != NULL; i++)
    {
        TwoStrings fullName   = strsplit(headers[i]->filename, '/');
        TwoStrings nameInPath = strsplit(fullName.b, '/');
        
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