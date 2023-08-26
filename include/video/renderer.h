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
void putChar(char c);
void DrawOverlayMouseCursor(uint8_t* mouseCursor, Point position, uint32_t colour);
void ClearMouseCursor(uint8_t* mouseCursor, Point position);