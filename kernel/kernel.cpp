#include <stdint.h>
#include <stddef.h>
#include "kernel.h" 
#include "idt/interrupts.h"
#include "video/window.h"

void _comout(const char* input);

struct limine_file* initramfs;
struct limine_file* font;
struct limine_framebuffer* buffer;
struct limine_terminal *terminal;

volatile struct limine_smp_request smp {
    .id = LIMINE_SMP_REQUEST,
    .revision = 0,
};

limine_goto_address test()
{
    printf("Hello from the Second core!");
    while (1)
    {
        if (MousePosition.X > 2)
        {
            comout("j");
        }
    }    
} 

extern "C" void _start(void)
{
    if (terminal_request.response == NULL || terminal_request.response->terminal_count < 1)
    {
        done();
    }

    comout("\033[2J \033[H");
    init();

    comout("Hello COM\n");

    printf("Welcome to SeshOS\n\0");

    newShell.shouldPrint = true;
    newShell.PrintPrompt();

    done();
}

void comout(const char* input)
{
    for (int i = 0; i < mystrlen(input); i++)
    {
        outb(0xE9, input[i]);
    }    
}