#include <stdint.h>
void outb(uint16_t port, uint8_t value) {
    asm volatile("outb %0, %1"
        :
        : "a"(value), "Nd"(port));
}

uint8_t inb(uint16_t port) {
    uint8_t returnVal;
    asm volatile("inb %1, %0"
        : "=a"(returnVal)
        : "Nd"(port));
    return returnVal;
}

uint16_t inw (uint16_t port) {
    uint16_t result;
    asm("in %%dx, %%ax" : "=a" (result) : "d" (port));
    return result;
}

void outw (uint16_t port, uint16_t data) {
    asm volatile("out %%ax, %%dx" : : "a" (data), "d" (port));
}

uint32_t inl (uint32_t port) {
    uint16_t result;
    asm("in %%edx, %%eax" : "=a" (result) : "d" (port));
    return result;
}

void outl (uint16_t port, uint32_t data) {
    asm volatile("out %%eax, %%edx" : : "a" (data), "d" (port));
}

void io_wait() {
    asm volatile("outb %%al, $0x80"
        :
        : "a"(0));
}