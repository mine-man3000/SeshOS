#include "interrupts.h"
#include "../video/video.h"
#include "../userinput/keyboard.h"
#include "../debugout.h"

extern void Panic(const char *panicMessage, interrupt_frame *regs);

extern void PIC_EndMaster();
extern void PIC_EndSlave();

__attribute__((interrupt)) void DivideByZero_Handler(interrupt_frame* frame)
{
    Panic("You can't divide by 0 silly :P", frame);
}

__attribute__((interrupt)) void PageFault_Handler(interrupt_frame* frame)
{
    Panic("Page Fault Detected", frame);
}

__attribute__((interrupt)) void DoubleFault_Handler(interrupt_frame* frame)
{
    Panic("Double Fault Detected", frame);
}

__attribute__((interrupt)) void GPFault_Handler(interrupt_frame* frame)
{
    Panic("General Protection Fault Detected", frame);
}

__attribute__((interrupt)) void Keyboard_Handler(interrupt_frame* frame)
{
    uint8_t scancode = inb(0x60);
    HandleKeyboard(scancode);
    PIC_EndMaster();
}

__attribute__((interrupt)) void MouseInt_Handler(interrupt_frame* frame)
{
    uint8_t mouseData = inb(0x60);
    HandlePS2Mouse(mouseData);
    PIC_EndSlave();
}

__attribute__((interrupt)) void Test_Handler(interrupt_frame* frame)
{
    printf("NICE\n");
}

void outb(uint16_t port, uint8_t value)
{
    asm volatile("outb %0, %1"
        :
        : "a"(value), "Nd"(port));
}

uint16_t inw (uint16_t port)
{
    uint16_t result;
    asm("in %%dx, %%ax" : "=a" (result) : "d" (port));
    return result;
}

void outw (uint16_t port, uint16_t data)
{
    asm volatile("out %%ax, %%dx" : : "a" (data), "d" (port));
}

uint8_t inb(uint16_t port)
{
    uint8_t returnVal;
    asm volatile("inb %1, %0"
        : "=a"(returnVal)
        : "Nd"(port));
    return returnVal;
}

void io_wait()
{
    asm volatile("outb %%al, $0x80"
        :
        : "a"(0));
}

void PIC_EndMaster(){
    outb(PIC1_COMMAND, PIC_EOI);
}

void PIC_EndSlave(){
    outb(PIC2_COMMAND, PIC_EOI);
    outb(PIC1_COMMAND, PIC_EOI);
}