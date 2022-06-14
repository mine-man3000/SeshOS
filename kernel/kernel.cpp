#include <stdint.h>
#include <stddef.h>
#include "kernel.h" 
#include "acpi/acpi.h"

void comout(const char* input);

struct limine_file* initramfs;
struct limine_file* font;
struct limine_framebuffer* buffer;
struct limine_terminal *terminal;

struct limine_smp_request smp {
    .id = LIMINE_SMP_REQUEST,
    .revision = 0,
};

limine_goto_address cpua();
limine_goto_address cpub();

extern "C" void _start(void)
{
    if (terminal_request.response == NULL || terminal_request.response->terminal_count < 1)
    {
        done();
    }

    init();

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

    newShell->PrintPrompt();    

    done();
}

void comout(const char* input)
{
    for (int i = 0; i < mystrlen(input); i++)
    {
        outb(0xE9, input[i]);
    }    
} 