/*
======================
The original version of this file is Copyright (C)Techflash Software, 2021 - 2023.
This is code from "Techflash OS", the hobbyist operating system developed by Techflash.
You may find the original code at https://github.com/techflashYT/Techflash-OS/blob/master/kernel/tty.c
======================
*/
#include <stdint.h>
#include <stdbool.h>
#include <tty.h>
uint16_t TTY_CursorX;
uint16_t TTY_CursorY;
uint16_t TTY_Width;
uint16_t TTY_Height;

uint32_t TTY_Color;
uint32_t TTY_BgColor;

bool     TTY_Bold;

TTY_WriteFunc_t TTY_WriteFunc;

void TTY_SetWriteFunc(TTY_WriteFunc_t func) {
	TTY_WriteFunc = func;	
}
void TTY_Write(const char ch, const uint16_t x, const uint16_t y, const uint32_t fgColor, const uint32_t bgColor) {
	TTY_WriteFunc(ch, x, y, fgColor, bgColor);
}