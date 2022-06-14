#pragma once
#include "../video/video.h"
#include "../video/renderer.h"

#define PIC1_COMMAND 0x20
#define mastr_pic_cmnd_port 0x20
#define slave_pic_cmnd_port 0xA0
#define mastr_pic_data_port 0x21
#define slave_pic_data_port 0xA1

#define icw1_init 0x10
#define icw1_icw4 0x01
#define icw4_8086 0x01

#define pic_eoi 0x20

void remap_interrupts_for_io();

void end_interrupts_for_mastr_io();
void end_interrupts_for_slave_io();

struct interrupt_frame;
__attribute__((interrupt)) void PageFault_Handler(interrupt_frame* frame);
__attribute__((interrupt)) void DoubleFault_Handler(interrupt_frame* frame);
__attribute__((interrupt)) void GPFault_Handler(interrupt_frame* frame);
__attribute__((interrupt)) void GetBuffer_Handler(interrupt_frame* frame);
__attribute__((interrupt)) void Keyboard_Handler(interrupt_frame* frame);
__attribute__((interrupt)) void Test_Handler(interrupt_frame* frame);

void RemapPIC();
void PIC_EndMaster();
void PIC_EndSlave();

void outb(uint16_t port, uint8_t value);
uint8_t inb(uint16_t port);
void outw (uint16_t port, uint16_t data);
uint16_t inw (uint16_t port);
void io_wait();