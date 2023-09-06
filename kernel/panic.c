#include <idt/interrupts.h>
void Panic(const char *panicMessage, interrupt_frame *regs) {
    printf("Kernel Panic\n\n%s \n\nSeshOS has been shutdown to prevent damage to your computer", panicMessage);
    
    if (regs) {
        uint64_t cr2;
        asm("movq %%cr2, %0\r\n" : "=r" (cr2) : );
        printf("\n\nDebug Info:\nRAX: %x\nRBX: %x\nRCX: %x\nRDX: %x\nRDI: %x\nRSI: %x\nRIP: %x\nCR2: %x", regs->rax, regs->rbx, regs->rcx, regs->rdx, regs->rdi, regs->rsi, regs->rip, cr2);        
    }

    while (true) {
        asm("cli; hlt");
    }
}