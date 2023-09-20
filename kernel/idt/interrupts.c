#include <idt/interrupts.h>
#include <video/video.h>
#include <userinput/keyboard.h>
#include <debugout.h>
#include <io.h>
#include <stdio.h>

extern void panic(const char *panicMessage, interrupt_frame *regs);

extern void PIC_EndMaster();
extern void PIC_EndSlave();

__attribute__((interrupt)) void DivideByZero_Handler(interrupt_frame* frame) {
    panic("You can't divide by 0 silly :P", frame);
}

__attribute__((interrupt)) void PageFault_Handler(interrupt_frame* frame) {
    panic("Page Fault Detected", frame);
}

__attribute__((interrupt)) void DoubleFault_Handler(interrupt_frame* frame) {
    panic("Double Fault Detected", frame);
}

__attribute__((interrupt)) void GPFault_Handler(interrupt_frame* frame) {
    panic("General Protection Fault Detected", frame);
}

__attribute__((interrupt)) void Keyboard_Handler(interrupt_frame* frame) {
    uint8_t scancode = inb(0x60);
    HandleKeyboard(scancode);
    PIC_EndMaster();
}

__attribute__((interrupt)) void Test_Handler(interrupt_frame* frame) {
    printf("NICE\n");
}

void PIC_EndMaster(){
    outb(PIC1_COMMAND, PIC_EOI);
}

void PIC_EndSlave(){
    outb(PIC2_COMMAND, PIC_EOI);
    outb(PIC1_COMMAND, PIC_EOI);
}