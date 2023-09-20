/*
======================
The original version of this file is Copyright (C)Techflash Software, 2021 - 2023.
This is code from "Techflash OS", the hobbyist operating system developed by Techflash.
You may find the original code at https://github.com/techflashYT/Techflash-OS/blob/master/kernel/tty/fbCon.c
======================
*/

#include <tty/fbCon.h>
#include <tty.h>
#include <bootloader.h>
#include <stddef.h>
#include <stdbool.h>
#include <limine.h>
#include <stdio.h>

framebuffer_t fbCon;

MODULE("FBCON");
extern framebuffer_t LM_GetFramebuffer();

static bool esc = false;

void FBCON_Write(const char ch, const uint16_t x, const uint16_t y, const uint32_t fgColor, const uint32_t bgColor) {
	if (ch == '\x1b') {
		esc = true;
		return;
	}
	if (esc) {
		esc = !TTY_HandleEsc(ch);
		return;
	}
	if (ch == '\r') {
		TTY_CursorX = 0;
		return;
	}
	if (ch == '\b') {
		if (TTY_CursorX == 0) {
			return;
		}
		TTY_CursorX--;
		putchar(' ');

		TTY_CursorX--;
		return;
	}
	if (ch == '\n') {
		TTY_CursorX = 0;
		TTY_CursorY++;
		return;
	}

	uint8_t *charData = font[(uint8_t)ch];
	uint32_t offset = ((y * 16) * fbCon.width) + (x * 8);
	for (uint_fast8_t i = 0; i != 16; i++) {
		for (uint_fast8_t j = 7; j != 0; j--) {
			uint32_t color = bgColor;
			if (charData[i] & (1 << j)) {
				color = fgColor;
				if (TTY_Bold) {
					// FIXME: for VGA yellow, we need to add 0xAA to the green for some reason
					if (color == COLOR_YELLOW) {
						color = 0xFFFF55;
						goto done;
					}
					// increase the brightness of the color by incresing all but the first byte of the uint32_t value
					for (uint_fast8_t k = 0; k != 4; k++) {
						uint8_t originalByte = ((uint8_t*)&color)[k];
						((uint8_t *)&color)[k] += 0x55;

						// Check for overflow
						if (((uint8_t *)&color)[k] < originalByte) {
							((uint8_t *)&color)[k] = 0xFF;
						}
					}
				}
			}
			done:
			fbCon.ptr[offset + (7 - j) + (i * fbCon.width)] = color;
		}
	}
	
	TTY_CursorX++;
	if (TTY_CursorX >= TTY_Width) {
		TTY_CursorX = 0;
		TTY_CursorY++;
	}
	if (TTY_CursorY > TTY_Height) {
		// TODO: Scroll TTY
	}
}

framebuffer_t FBCON_Init() {
	framebuffer_t ret;
	if (BOOT_LoaderID == BOOT_LoaderID_LimineCompatible) {
		ret = LM_GetFramebuffer();
	}
	else {
		log(MODNAME, "Unknown bootloader.  Unable to set framebuffer!  Keeping print method to default, you will probably not have video output!", LOGLEVEL_FATAL);
		ret = (framebuffer_t){.ptr = NULL, .width = 0, .height = 0, .mode = 0};
	}
	fbCon.width  = ret.width;
	fbCon.height = ret.height;
	fbCon.mode   = ret.mode;
	fbCon.ptr    = ret.ptr;


	TTY_CursorX  = 0;
	TTY_CursorY  = 0;
	TTY_Width    = (uint16_t)(ret.width  / 8);
	TTY_Height   = (uint16_t)(ret.height / 16);
	TTY_Color    = COLOR_LGRAY;
	TTY_Bold     = false;

	if (ret.ptr != 0) {
		log(MODNAME, "Setting TTY Write func", LOGLEVEL_DEBUG);
		TTY_SetWriteFunc(FBCON_Write);
		log(MODNAME, "Done setting TTY Write func", LOGLEVEL_DEBUG);
	}

	return ret;
}