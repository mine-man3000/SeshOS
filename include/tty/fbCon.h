/*
======================
The original version of this file is Copyright (C)Techflash Software, 2021 - 2023.
This is code from "Techflash OS", the hobbyist operating system developed by Techflash.
You may find the original code at https://github.com/techflashYT/Techflash-OS/blob/master/include/kernel/tty/fbCon.h
======================
*/

typedef struct {
	uint32_t width;
	uint32_t height;

	enum {
		FB_MODE_ARGB,
		FB_MODE_RGBA,
		FB_MODE_BGRA
	} mode;

	uint32_t padding1;
	uint32_t *ptr;
} framebuffer_t;

extern uint8_t font[256][16];

extern framebuffer_t fbCon;
extern framebuffer_t FBCON_Init();

#define COLOR_WHITE  0xFFFFFFFF
#define COLOR_BLACK  0xFF000000
#define COLOR_RED    0xFFAA0000
#define COLOR_GREEN  0xFF00AA00
#define COLOR_BLUE   0xFF0000AA
#define COLOR_CYAN   0xFF00AAAA
#define COLOR_PURPLE 0xFFAA00AA
#define COLOR_LGRAY  0xFFAAAAAA
#define COLOR_YELLOW 0xFFAA5500