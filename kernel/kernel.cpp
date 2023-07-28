#include <stdint.h>
#include <stddef.h>
#include "kernel.h" 
#include "idt/interrupts.h"
#include "video/window.h"
#include "audio/pcspk.h"
#include <cpuid.h>
#include "acpi/acpi.h"

void _comout(const char* input);

struct limine_file* initramfs;
struct limine_file* font;
struct limine_framebuffer* buffer;

volatile struct limine_smp_request smp {
    .id = LIMINE_SMP_REQUEST,
    .revision = 0,
};


extern "C" void _start(void)
{
    comout("\033[2J \033[H");
    
    init();

    comout("Hello COM\n");

    printf("Welcome to SeshOS\n\0");

    //newShell.shouldPrint = true;
    //newShell.PrintPrompt();

    // beep();

    while (1)
    {
        __asm__("hlt");
    }
    
}

void comout(const char* input)
{
    for (unsigned int i = 0; i < mystrlen(input); i++)
    {
        outb(0xE9, input[i]);
    }    
}

limine_goto_address halt() {
    asm("cli;hlt");
}

void Panic(const char *panicMessage)
{
    Clear(0);
    printf("Kernel Panic\n\n%s \n\nSeshOS has been shutdown to prevent damage to your computer", panicMessage);

    for (uint64_t i = 0; i < smp.response->cpu_count; i++) {
        smp.response->cpus[i]->goto_address = (limine_goto_address)halt;
    }
    comout("test!");
    asm("cli;hlt");
}