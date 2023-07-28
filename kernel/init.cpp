#include "gdt/gdt.h"
#include "idt/idt.h"
#include "memory/mem.h"
#include "idt/interrupts.h"
#include "kernelDefines.h"
#include "init.h"
#include "video/renderer.h"
#include "memory/malloc.h"
#include "acpi/acpi.h"
#include "flanterm/flanterm.h"
#include "flanterm/backends/fb.h"

extern "C" void disablePIC();

kernie_heap kernieHeap;

static void *internalAlloc(size_t size) {
    kernieHeap.malloc(size);
}

static void internalFree(void *ptr, size_t) {
    kernieHeap.free(ptr);
}

uint32_t defaultbg = 0x1b1c1b;
uint32_t defaultfg = 0xffffff;

struct flanterm_context *ft_ctx;

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

    comout("got here");

    ft_ctx = flanterm_fb_init(
        internalAlloc, internalFree, (uint32_t*)buffer->address, buffer->width, buffer->height, buffer->pitch, NULL, NULL, NULL, &defaultbg, &defaultfg, NULL, NULL, NULL, 0, 0, 1, 1, 1, 0
    );

    if(ft_ctx) {
        comout("ft_ctx isn't NULL");
    }
    else {
        comout("ft_ctx is NULL");
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

    initramfs = module.response->modules[0];
    font = module.response->modules[1];

    printf("[ %sOK %s] buffers filled\n", Green, White);

    parse();

    printf("[ %sOK %s] ramdisk parsed\n", Green, White);

    initACPI();

    printf("[ %sOK %s] ACPI Initialized\n", Green, White);
}