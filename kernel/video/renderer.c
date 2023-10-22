#include <video/renderer.h>
#include <video/video.h>
#include <tty/fbCon.h>
#include <tty.h>
#include <stdarg.h>
#include <kernel.h>
#include <memory/malloc.h>

#define PSF1_MAGIC0 0x36
#define PSF1_MAGIC1 0x04

void putchar(char c) {
    TTY_Write(c, TTY_CursorX, TTY_CursorY, TTY_Color, TTY_BgColor);
}
int puts(const char *string) {
    const char *orig = string;
    while (*string != '\0') {
        putchar(*string);
        string++;
    }
    return string - orig;
}