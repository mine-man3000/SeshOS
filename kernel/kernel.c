#include <stdint.h>
#include <stddef.h>
#include <kernel.h> 
#include <idt/interrupts.h>
#include <audio/pcspk.h>
#include <cpuid.h>
#include <acpi/acpi.h>
#include <stdio.h>


void * __gxx_personality_v0=0;
void * _Unwind_Resume =0;
void _comout(const char* input);

struct limine_file* initramfs;

volatile struct limine_smp_request smp = {
    .id = LIMINE_SMP_REQUEST,
    .revision = 0,
};


void _start(void) {
    init();

    comout("Hello COM\n");

    printf("Welcome to SeshOS\n");

    // beep();

    while (1) {
        __asm__("hlt");
    }
    
}

limine_goto_address halt() {
    asm("cli;hlt");
}