#pragma once

#include <stdint.h>
#include <stdarg.h>
#include <stdbool.h>


extern volatile struct limine_terminal_request terminal_request;

struct TwoStrings
{
    char a[128];
    char b[128];
};

/**
 * @brief gets the length of the provided string
 * 
 * @param str the string to get the length of
 * @return length of the string 
 */
unsigned int mystrlen(const char* str);

/**
 * @brief turns the givin value into a string
 * 
 * @param value the value to turn into a string
 * @return the uint64 givin in string form 
 */
const char *to_string(uint64_t value);

/**
 * @brief turns the givin value into a hex string
 * 
 * @param value the value to turn into a hex string
 * @return the uint64 givin in hex string form 
 */
const char *to_hstring(uint64_t value);

/**
 * @brief checks to see if the given strings are the same
 * 
 * @param str1 
 * @param str2 
 * @return true or false depending if the strings are the same 
 */
bool mystrcmp(char* str1, char* str2);

/**
 * @brief splits the string at the specified character
 * 
 * @param str the string to split
 * @param split the character to split them at
 * @return the two strings in a TwoStrings struct 
 */
TwoStrings strsplit(char *str, char split);