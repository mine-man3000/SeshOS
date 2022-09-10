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

    printf("CPU count: %i\n", smp.response->cpu_count);

    //smp.response->cpus[1]->goto_address = test();

    for (int i = 0; i < smp.response->cpu_count; i++)
    {
        printf("CPU %i goto address %x\n", i ,smp.response->cpus[i]->goto_address);
    }    

    comout("Hello COM\n");

    printf("Welcome to SeshOS\n\0");
    
    asm volatile ("int $0x69");

    uint64_t totalMem = 0;

    for (int i = 0; i < mmap.response->entry_count; i++)
    {
        totalMem += mmap.response->entries[i]->length;
    }

    printf("total memory: %i MB\n", totalMem / 1024 / 1024);

    comout("Hello COM\n");

    printf("total pages: %i\n", numPages());

    time NewTime = GetTime();

    newShell.shouldPrint = true;
    newShell.PrintPrompt();
    
    drawRect(0, 0, buffer->width, buffer->height, 0xD97F1956);
    Window yes = Window(10, 10, 200, 200, "Hello World        ", 0x12345678);
    Window no = Window(100, 100, 200, 200, "1234567890!@#$%^&*()", 0x12345678);
    drawRect(0, buffer->height - 40, buffer->width, 40, 0xff888888);

    newShell.shouldPrint = false;

    done();
}

void comout(const char* input)
{
    for (int i = 0; i < mystrlen(input); i++)
    {
        outb(0xE9, input[i]);
    }    
}