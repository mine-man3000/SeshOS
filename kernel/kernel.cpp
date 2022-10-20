#include <stdint.h>
#include <stddef.h>
#include "kernel.h" 
#include "idt/interrupts.h"
#include "video/window.h"
#include "memory/malloc.h"
#include "audio/pcspk.h"
#include <cpuid.h>

void _comout(const char* input);

struct limine_file* initramfs;
struct limine_file* font;
struct limine_framebuffer* buffer;
struct limine_terminal *terminal;

volatile struct limine_smp_request smp {
    .id = LIMINE_SMP_REQUEST,
    .revision = 0,
};

kernie_heap heap;

limine_goto_address testA()
{
    while (1)
    {
        comout("uwu\n");
        sleep(1);
    }    
}

limine_goto_address testB()
{
    while (1)
    {
        comout("owo\n");
        sleep(1);
    }    
}

extern "C" void _start(void)
{
    if (terminal_request.response == NULL || terminal_request.response->terminal_count < 1)
    {
        asm("cli\nhlt");
    }

    comout("\033[2J \033[H");
    
    for (uint64_t i = 0; i < mmap.response->entry_count; i++)
    {
        uint64_t type = mmap.response->entries[i]->type;
        if (type == LIMINE_MEMMAP_USABLE)
        {
            kernie_heap::the()->init((unsigned char*)mmap.response->entries[i]->base);
        }
    }

    init();

    smp.response->cpus[0]->goto_address = testB();
    smp.response->cpus[1]->goto_address = testA();

    comout("Hello COM\n");

    printf("Welcome to SeshOS\n\0");

    newShell.shouldPrint = true;
    newShell.PrintPrompt();

    // beep();

    while (1)
    {
        __asm__("hlt");
    }
}

void comout(const char* input)
{
    for (int i = 0; i < mystrlen(input); i++)
    {
        outb(0xE9, input[i]);
    }    
}
