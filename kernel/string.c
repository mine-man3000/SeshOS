/*
======================
The original version of this file is Copyright (C)Techflash Software, 2021 - 2023.
This is code from "Techflash OS", the hobbyist operating system developed by Techflash.
You may find the original code at https://github.com/techflashYT/Techflash-OS/blob/master/libc/string/
======================
*/
#include <limine.h>
#include <string.h>
#include <video/video.h>

char hexTo_StringOutput[128];
const char *to_hstring(uint64_t value) {
    uint64_t *valPtr = &value;
    uint8_t *ptr;
    uint8_t tmp;
    uint8_t size = 8 * 2 - 1;
    for (uint8_t i = 0; i < size; i++) {
        ptr = ((uint8_t *)valPtr + i);
        tmp = ((*ptr & 0xF0) >> 4);
        hexTo_StringOutput[size - (i * 2 + 1)] = tmp + (tmp > 9 ? 55 : '0');
        tmp = ((*ptr & 0x0F));
        hexTo_StringOutput[size - (i * 2)] = tmp + (tmp > 9 ? 55 : '0');
    }
    hexTo_StringOutput[size + 1] = 0;
    return hexTo_StringOutput;
}

size_t strlen(const char* str) {
    unsigned int i = 0;
    for (; str[i] != '\0'; i++);
    return i;
}

int strcmp(char *str1, char *str2) {
	while (*str1) {
		// if characters differ, or end of the second string is reached
		if (*str1 != *str2) {
			break;
		}
 
		// move to the next pair of characters
		str1++;
		str2++;
	}
 
	// return the ASCII difference after converting `char*` to `unsigned char*`
	return *(const uint8_t *)str1 - *(const uint8_t *)str2;
}
size_t strnlen_s(const char* str, size_t strsz) {
	const char* s;
	for (s = str; *s && strsz--; ++s);
	return (s - str);
}


TwoStrings strsplit(char *str, char split) {
    char p1[128];
    char p2[128];
    unsigned int i = 0;
    unsigned int j = 0;
    
    for (; i < strlen(str) && str[i] != split; i++) {
        p1[i] = str[i];
    }
    i++;
    p1[i - 1] = '\0';
    for (; i < strlen(str); j++) {
        p2[j] = str[i];
        i++;
    }
    j++;
    p2[j - 1] = '\0';
    TwoStrings yes;

    for (int i = 0; i < 128; i++) {
        yes.a[i] = p1[i];
        yes.b[i] = p2[i];
    }
    
    return yes;
}

int isdigit(int ch) {
	return (ch >= '0') && (ch <= '9');
}

#include <string.h>
char *strcat(char* restrict destination, const char* restrict source) {
	// ptr to end of dest
	char *ptr = destination + strlen(destination);
	
	// append to dest
	while (*source != '\0') {
		*ptr++ = *source++;
	}

	// null terminate
	*ptr = '\0';

	// return dest
	return destination;
}


char *strstr(const char *haystack, const char *needle) {
	size_t size = strlen(needle);

	while (*haystack) {
		if (!memcmp(haystack, needle, size)) {
			return (char *)haystack;
		}
		haystack++;
	}
	return NULL;
}


char* strcpy(char* destination, const char* source) {
	// return if no memory is allocated to the destination
	if (destination == NULL) {
		return NULL;
	}
 
	// take a pointer pointing to the beginning of the destination string
	char *ptr = destination;
 
	// copy the C-string pointed by source into the array
	// pointed by destination
	while (*source != '\0') {
		*destination = *source;
		destination++;
		source++;
	}
 
	// include the terminating null character
	*destination = '\0';
 
	// the destination is returned by standard `strcpy()`
	return ptr;
}


int atoi(const char *nptr) {
	// Initialize result
	int res = 0;

	// Iterate through all characters
	// of input string and update result
	// take ASCII character of corresponding digit and
	// subtract the code from '0' to get numerical
	// value and multiply res by 10 to shuffle
	// digits left to update running total
	for (int i = 0; nptr[i] != '\0'; ++i) {
		res = res * 10 + nptr[i] - '0';
	}

	// return result.
	return res;
}
