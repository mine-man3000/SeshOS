#include <limine.h>
#include <mystring.h>
#include <video/video.h>

char uintTo_StringOutput[128];
const char *to_string(uint64_t value)
{
    uint8_t size;
    uint64_t sizeTest = value;
    while (sizeTest / 10 > 0)
    {
        sizeTest /= 10;
        size++;
    }

    uint8_t index = 0;
    while (value / 10 > 0)
    {
        uint8_t remainder = value % 10;
        value /= 10;
        uintTo_StringOutput[size - index] = remainder + '0';
        index++;
    }
    uint8_t remainder = value % 10;
    uintTo_StringOutput[size - index] = remainder + '0';
    uintTo_StringOutput[size + 1] = 0;
    return uintTo_StringOutput;
}

char hexTo_StringOutput[128];
const char *to_hstring(uint64_t value)
{
    uint64_t *valPtr = &value;
    uint8_t *ptr;
    uint8_t tmp;
    uint8_t size = 8 * 2 - 1;
    for (uint8_t i = 0; i < size; i++)
    {
        ptr = ((uint8_t *)valPtr + i);
        tmp = ((*ptr & 0xF0) >> 4);
        hexTo_StringOutput[size - (i * 2 + 1)] = tmp + (tmp > 9 ? 55 : '0');
        tmp = ((*ptr & 0x0F));
        hexTo_StringOutput[size - (i * 2)] = tmp + (tmp > 9 ? 55 : '0');
    }
    hexTo_StringOutput[size + 1] = 0;
    return hexTo_StringOutput;
}

unsigned int mystrlen(const char* str)
{
    unsigned int i = 0;
    for (; str[i] != '\0'; i++);
    return i;
}

bool mystrcmp(char *str1, char *str2) {
    while (true) {
        if (*str1 != *str2) {
            return false;
        } else if (*str1 == 0) { // we can skip checking str2 since theyre the same
            return true;
        }
        str1 ++;
        str2 ++;
    }
}

TwoStrings strsplit(char *str, char split)
{
    char p1[128];
    char p2[128];
    unsigned int i = 0;
    unsigned int j = 0;
    
    for (; i < mystrlen(str) && str[i] != split; i++)
    {
        p1[i] = str[i];
    }
    i++;
    p1[i - 1] = '\0';
    for (; i < mystrlen(str); j++)
    {
        p2[j] = str[i];
        i++;
    }
    j++;
    p2[j - 1] = '\0';
    TwoStrings yes;

    for (int i = 0; i < 128; i++)
    {
        yes.a[i] = p1[i];
        yes.b[i] = p2[i];
    }
    
    return yes;
}