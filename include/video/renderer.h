#pragma once
#include <limine.h>
#include <mystring.h>
#include <math.h>

extern struct limine_terminal *terminal;

extern uint32_t MouseCursorBuffer[16 * 16];
extern uint32_t MouseCursorBufferAfter[16 * 16];
extern bool MouseDrawn;

void printf(const char *format, ...);
void gPrint(const char chr[], unsigned int xOff, unsigned int yOff, uint32_t Color);
void putchar(char c);
void DrawOverlayMouseCursor(uint8_t* mouseCursor, point_t position, uint32_t colour);
void ClearMouseCursor(uint8_t* mouseCursor, point_t position);

#define Green "\e[32m"
#define Blue  "\e[34m"
#define White "\e[1;37m"