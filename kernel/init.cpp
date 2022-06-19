#include "gdt/gdt.h"
#include "idt/idt.h"
#include "memory/mem.h"
#include "idt/interrupts.h"
#include "kernelDefines.h"
#include "init.h"
#include "video/renderer.h"

extern "C" void disablePIC();

void init()
{
    GDTDescriptor gdtDescriptor;
    gdtDescriptor.Size = sizeof(GDT) - 1;
    gdtDescriptor.Offset = (uint64_t)&DefaultGDT;
    LoadGDT(&gdtDescriptor);


    comout("TETS\n");

    printf("[ %sOK %s] loading GDT\n", Green, White);

    create_idt();

    outb(mastr_pic_data_port, 0b11111101);
	outb(slave_pic_data_port, 0b11111111);

    asm("sti");

    printf("[ %sOK %s] loading IDT\n", Green, White);

    physical_kernel_start = kernel_address_request.response->physical_base;
	virtual_kernel_start = kernel_address_request.response->virtual_base;

    hhdm_offset = hhdm_request.response->offset;

    buffer = framebuffer_request.response->framebuffers[0];  
    terminal = terminal_request.response->terminals[0];

    initramfs = module.response->modules[0];

    printf("[ %sOK %s] buffers filled\n", Green, White);

    parse();

    printf("[ %sOK %s] ramdisk parsed\n", Green, White);
}