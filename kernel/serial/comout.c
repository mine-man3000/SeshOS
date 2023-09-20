/*
======================
The original version of this file is Copyright (C)Techflash Software, 2021 - 2023.
This is code from "Techflash OS", the hobbyist operating system developed by Techflash.
You may find the original code at https://github.com/techflashYT/Techflash-OS/blob/master/kernel/COM.c
======================
*/
#include <stdint.h>
#include <io.h>
#include <tty.h>

void comout(const char* input) {
    int i = 0;
    while (input[i] != '\0') {
        outb(0xE9, input[i]);
        i++;
    }
}

void comout_num(uint64_t num) {
    for (int i = 0; i < 16; i ++) {
        const char buf[2] = {"0123456789abcdef"[num >> 60], 0};
        comout(&buf[0]);
        num <<= 4;
    }
}

void COM_LogWrapper(const char ch, const uint16_t x, const uint16_t y, const uint32_t fgColor, const uint32_t bgColor) {
    (void)x;
	(void)y;
	(void)fgColor;
	(void)bgColor;
    comout((const char[]){ch, '\0'});

    if (ch == '\r') {
		TTY_CursorX = 0;
		return;
	}
	if (ch == '\n') {
		TTY_CursorY++;
		return;
	}
	TTY_CursorX++;

}
