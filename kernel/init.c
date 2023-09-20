#include <gdt/gdt.h>
#include <idt/idt.h>
#include <memory/mem.h>
#include <idt/interrupts.h>
#include <kernelDefines.h>
#include <init.h>
#include <tty/fbCon.h>
#include <stdio.h>
#include <memory/malloc.h>
#include <tty.h>
#include <acpi/acpi.h>
#include "flanterm/flanterm.h"
#include "flanterm/backends/fb.h"
#include <pci/pci.h>
#include <bootloader.h>

uint32_t defaultbg = 0x000000;
uint32_t defaultfg = 0xffffff;

void init() {
    TTY_SetWriteFunc(COM_LogWrapper);
    BOOT_CheckLoader();
    FBCON_Init();
    for (uint64_t i = 0; i < mmap.response->entry_count; i++) {
        uint64_t type = mmap.response->entries[i]->type;
        if (type == LIMINE_MEMMAP_USABLE) {
            MEM_Init((unsigned char*)mmap.response->entries[i]->base);
        }
    }
    initramfs = module.response->modules[0];

    GDTDescriptor gdtDescriptor;
    gdtDescriptor.Size = sizeof(GDT) - 1;
    gdtDescriptor.Offset = (uint64_t)&DefaultGDT;
    LoadGDT(&gdtDescriptor);

    comout("TETS\n");

    printf("[ %sOK %s] loading GDT\n", Green, White);

    create_idt();

    printf("[ %sOK %s] loading IDT\n", Green, White);

    physical_kernel_start = kernel_address_request.response->physical_base;
	virtual_kernel_start = kernel_address_request.response->virtual_base;

    hhdm_offset = hhdm_request.response->offset;

    printf("[ %sOK %s] buffers filled\n", Green, White);
    
    if (!initramfs) {
        printf("[ %sINFO%s ] ramdisk not present\n", Blue, White);
        panic("ERROR: no ramdisk found! how do you expect to be able to do anything without a disk?", NULL);
    }
    parse();
    printf("[ %sOK %s] ramdisk parsed\n", Green, White);

    //ACPI IS BROKEN
    ACPI_Init();
    
    printf("[ %sOK %s] ACPI Initialized\n", Green, White);
    
    // EnumeratePCI(mcfg);
    printf("[ %sOK %s] PCI Enumerated\n", Green, White);
}