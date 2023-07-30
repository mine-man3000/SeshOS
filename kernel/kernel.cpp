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
    //comout("\033[2J \033[H");
    
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

void comout_num(uint64_t num)
{
    for (int i = 0; i < 16; i ++)
    {
        const char buf[2] = {"0123456789abcdef"[num >> 60], 0};
        comout(&buf[0]);
        num <<= 4;
    }
}

limine_goto_address halt() {
    asm("cli;hlt");
}

void Panic(const char *panicMessage, interrupt_frame *regs)
{
    Clear(0);
    uint64_t cr2;
    asm("movq %%cr2, %0\r\n" : "=r" (cr2) : );\
    printf("Kernel Panic\n\n%s \n\nSeshOS has been shutdown to prevent damage to your computer\
\n\nDebug Info:\nRAX: %x\nRBX: %x\nRCX: %x\nRDX: %x\nRDI: %x\nRSI: %x\nRIP: %x\nCR2: %x", panicMessage, regs->rax, regs->rbx, regs->rcx, regs->rdx, regs->rdi, regs->rsi, regs->rip, cr2);
    while (true) {
        asm("cli; hlt");
    }
}