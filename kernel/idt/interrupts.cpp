#include "interrupts.h"
#include "../video/video.h"
#include "../userinput/keyboard.h"

__attribute__((interrupt)) void PageFault_Handler(interrupt_frame* frame){
    Panic("Page Fault Detected");
}

__attribute__((interrupt)) void DoubleFault_Handler(interrupt_frame* frame){
    Panic("Double Fault Detected");
}

__attribute__((interrupt)) void GPFault_Handler(interrupt_frame* frame){
    Panic("General Protection Fault Detected");
}

__attribute__((interrupt)) void Keyboard_Handler(interrupt_frame* frame){
    uint8_t scancode = inb(0x60);
    HandleKeyboard(scancode);
}

__attribute__((interrupt)) void MouseInt_Handler(interrupt_frame* frame){
    printf("MOUSE");
    uint8_t mouseData = inb(0x60);
    HandlePS2Mouse(mouseData);
}

__attribute__((interrupt)) void GetBuffer_Handler(interrupt_frame* frame)
{
    printf("Buffer Address: 0x");
    printf("%x", buffer->address);
    printf("\nWidth: ");
    printf("%x", buffer->width);
    printf("\nHeight: ");
    printf("%x", buffer->height);
    printf("\n");
}

__attribute__((interrupt)) void Test_Handler(interrupt_frame* frame)
{
    printf("\nNICE\n");
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