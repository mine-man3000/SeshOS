#include <stdio.h>
#include <stdbool.h>

void panic(const char *panicMessage, registers_t *regs) {
    printf("Kernel Panic\n\n%s \n\nSeshOS has been shutdown to prevent damage to your computer", panicMessage);
    
    if (regs) {
        uint64_t cr2;
        asm("movq %%cr2, %0\r\n" : "=r" (cr2) : );
        printf("\n\nDebug Info:\nRAX: 0x%lx\nRBX: 0x%lx\nRCX: 0x%lx\nRDX: 0x%lx\nRDI: 0x%lx\nRSI: 0x%lx\nRIP: 0x%lx\nCR2: 0x%lx", regs->rax, regs->rbx, regs->rcx, regs->rdx, regs->rdi, regs->rsi, regs->rip, cr2);        
    }

    while (true) {
        asm("cli; hlt");
    }
}