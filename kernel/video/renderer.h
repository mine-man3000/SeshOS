#pragma once
#include "../limine.h"
#include "../string.h"
#include "../math.h"

extern struct limine_terminal *terminal;

extern uint32_t MouseCursorBuffer[19 * 19];
extern uint32_t MouseCursorBufferAfter[19 * 19];
extern bool MouseDrawn;

void printf(const char *format, ...);
void gPrint(char chr[], unsigned int xOff, unsigned int yOff, uint32_t Color);
void putChar(char c);
void DrawOverlayMouseCursor(uint8_t* mouseCursor, Point position, uint32_t colour);
void ClearMouseCursor(uint8_t* mouseCursor, Point position);