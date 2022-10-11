#include <stdint.h>
#include <stddef.h>
#include "kernel.h" 
#include "idt/interrupts.h"
#include "video/window.h"
#include "memory/malloc.h"

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

    uint64_t totMem = 0;
    uint64_t useMem = 0;
    uint64_t freMem = 0;
    for (int i = 0; i < mmap.response->entry_count; i++)
    {
        uint64_t type = mmap.response->entries[i]->type;
        if (type == LIMINE_MEMMAP_USABLE || type == LIMINE_MEMMAP_ACPI_RECLAIMABLE || type == LIMINE_MEMMAP_BOOTLOADER_RECLAIMABLE)
        {
            freMem += mmap.response->entries[i]->length;
        }
        else
        {
            useMem += mmap.response->entries[i]->length;
        }
    }
    totMem = useMem + freMem;

    printf("%i KB/%i KB\n", useMem / 1024, totMem / 1024); 
    
    printf("====== STARTING MALLOC TEST 1 ======\r\n");
    printf("THIS TEST SHOULD NOT CAUSE LEAKS!\r\n");

    void *initPtr1 = malloc(64);
    void *lastPointer = NULL;
    for (uint32_t i = 0; i < 10000; i++)
    {
        printf("malloc iteration %i\r", i);
        void *pointer = malloc(64);
        lastPointer = pointer;
        free(pointer);
    }    
    printf("\r\n======= MALLOC TEST 1 RESULTS ======\r\n");
    printf("initial pointer: %x\n", initPtr1);
    printf("final pointer: %x\n", lastPointer);
    printf("====== STARTING MALLOC TEST 2 ======\r\n");
    printf("THIS TEST **SHOULD** CAUSE LEAKS!\r\n");
    printf("THIS TEST **SHOULD** CAUSE A **64 BYTE** LEAK **EVERY ITERATION**!\r\n");
    void *initPtr2 = malloc(64);
    void *lastPointer2 = NULL;
    for (uint32_t i = 0; i < 10000; i++)
    {
        printf("malloc iteration %i\r", i);
        void *pointer = malloc(64);
        void *pointer2 = malloc(64);
        lastPointer2 = pointer;
        free(pointer);
    }
    printf("\r\n======= MALLOC TEST 2 RESULTS ======\r\n");
    printf("initial pointer: %x\n", initPtr2);
    printf("final pointer: %x\n", lastPointer2);
    
    for (int i = 0; i < mmap.response->entry_count; i++)
    {
        uint64_t type = mmap.response->entries[i]->type;
        if (type == LIMINE_MEMMAP_USABLE || type == LIMINE_MEMMAP_ACPI_RECLAIMABLE || type == LIMINE_MEMMAP_BOOTLOADER_RECLAIMABLE)
        {
            freMem += mmap.response->entries[i]->length;
        }
        else
        {
            useMem += mmap.response->entries[i]->length;
        }
    }
    totMem = useMem + freMem;
    printf("%i KB/%i KB\n", useMem / 1024, totMem / 1024); 

    asm("cli\nhlt");

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