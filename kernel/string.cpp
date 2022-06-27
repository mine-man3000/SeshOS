#include "limine.h"
#include "string.h"
#include "video/video.h"

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

int mystrlen(const char* str)
{
    int i = 0;
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
    TwoStrings yes;
    int i = 0;
    int j = 0;
    
    for (; i < mystrlen(str) && str[i] != split; i++)
    {
        yes.a[i] = str[i];
    }
    yes.a[i + 1] = '\0';
    for (; i < mystrlen(str); j++)
    {
        yes.b[j] = str[i];
        i++;
    }
    yes.b[j + 1] = '\0';
    
    return yes;
}