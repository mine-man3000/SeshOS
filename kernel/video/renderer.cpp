#include "renderer.h"
#include "video.h"
#include <stdarg.h>

#define PSF1_MAGIC0 0x36
#define PSF1_MAGIC1 0x04

volatile struct limine_terminal_request terminal_request = {
    .id = LIMINE_TERMINAL_REQUEST,
    .revision = 0
};

void printf(const char* format, ...)
{
    va_list ap;
    va_start(ap, format);

    const char *ptr;
    const char *strPtr;

    int i = 0;
    for (ptr = format; *ptr != '\0'; ptr++)
    {
        if (*ptr == '%')
        {
            ptr++;
            switch (*ptr)
            {
                case 's':
                    strPtr = va_arg(ap, const char*);
                    terminal_request.response->write(terminal, strPtr, mystrlen(strPtr));
                    break;
                case '%':
                    putChar('%');
                    break;
                case 'i':
                    strPtr = to_string(va_arg(ap, uint64_t));
                    terminal_request.response->write(terminal, strPtr, mystrlen(strPtr));
                    break;
                case 'x':
                    strPtr = to_hstring(va_arg(ap, uint64_t));
                    terminal_request.response->write(terminal, "0x", 2);
                    terminal_request.response->write(terminal, strPtr, mystrlen(strPtr));
                    break;
            }
        }
        else
        {
            putChar(*ptr);
        }
        i++;
    }
    va_end(ap);
}

void putChar(char c)
{
    char yes[2] = {c, '\0'};
    terminal_request.response->write(terminal, yes, 2);
}