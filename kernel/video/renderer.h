#pragma once
#include "../limine.h"
#include "../string.h"

extern struct limine_terminal *terminal;

struct Point
{
    long X;
    long Y;
};

void printf(const char *format, ...);

struct font_header
{
    char magic0[1];
    char magic1[1];
};
void putChar(char c);