/*
======================
The original version of this file is Copyright (C)Techflash Software, 2021 - 2023.
This is code from "Techflash OS", the hobbyist operating system developed by Techflash.
You may find the original code at https://github.com/techflashYT/Techflash-OS/blob/master/include/kernel/tty.h
======================
*/
#include <stdint.h>
#include <stdbool.h>

typedef void (*TTY_WriteFunc_t)(const char ch, const uint16_t x, const uint16_t y, const uint32_t fgColor, const uint32_t bgColor);

extern uint16_t TTY_CursorX;
extern uint16_t TTY_CursorY;
extern uint16_t TTY_Width;
extern uint16_t TTY_Height;

extern uint32_t TTY_Color;
extern uint32_t TTY_BgColor;

extern bool     TTY_Bold;

extern TTY_WriteFunc_t TTY_WriteFunc;

extern void TTY_SetWriteFunc(TTY_WriteFunc_t func);
extern void TTY_Write(const char ch, const uint16_t x, const uint16_t y, const uint32_t fgColor, const uint32_t bgColor);
extern bool TTY_HandleEsc(char ch);
