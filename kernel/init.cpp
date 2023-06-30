#include "gdt/gdt.h"
#include "idt/idt.h"
#include "memory/mem.h"
#include "idt/interrupts.h"
#include "kernelDefines.h"
#include "init.h"
#include "video/renderer.h"
#include "memory/malloc.h"
#include "acpi/acpi.h"

extern "C" void disablePIC();

kernie_heap heap;

void init()
{

    for (uint64_t i = 0; i < mmap.response->entry_count; i++)
    {
        uint64_t type = mmap.response->entries[i]->type;
        if (type == LIMINE_MEMMAP_USABLE)
        {
            kernie_heap::the()->init((unsigned char*)mmap.response->entries[i]->base);
        }
    }

    GDTDescriptor gdtDescriptor;
    gdtDescriptor.Size = sizeof(GDT) - 1;
    gdtDescriptor.Offset = (uint64_t)&DefaultGDT;
    LoadGDT(&gdtDescriptor);

    comout("TETS\n");

    printf("[ %sOK %s] loading GDT\n", Green, White);

    create_idt();
    InitPS2Mouse();

    printf("[ %sOK %s] loading IDT\n", Green, White);

    physical_kernel_start = kernel_address_request.response->physical_base;
	virtual_kernel_start = kernel_address_request.response->virtual_base;

    hhdm_offset = hhdm_request.response->offset;

    buffer = framebuffer_request.response->framebuffers[0];  
    terminal = terminal_request.response->terminals[0];

    initramfs = module.response->modules[0];
    font = module.response->modules[1];

    printf("[ %sOK %s] buffers filled\n", Green, White);

    parse();

    printf("[ %sOK %s] ramdisk parsed\n", Green, White);

    initACPI();

    printf("[ %sOK %s] ACPI Initialized\n", Green, White);
}