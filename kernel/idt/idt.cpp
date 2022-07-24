#include "idt.h"
#include "interrupts.h"

void set_interrupt_offset(InterruptEntry* int_ptr, uint64_t offset)
{
	int_ptr->offset_0 = (uint16_t)((offset & 0x000000000000FFFF));
	int_ptr->offset_1 = (uint16_t)((offset & 0x00000000FFFF0000) >> 16);
	int_ptr->offset_2 = (uint32_t)((offset & 0xFFFFFFFF00000000) >> 32);
}

uint64_t get_interrupt_offset(InterruptEntry* int_ptr)
{
	uint64_t offset = 0;

	offset |= (uint64_t)int_ptr->offset_0;
	offset |= (uint64_t)int_ptr->offset_1 << 16;
	offset |= (uint64_t)int_ptr->offset_2 << 32;

	return offset;
}

IDT idt;
uint8_t idt_page[4096] = { 0 };

void CreateEntry(int interruptNum, uint64_t Handler)
{
	InterruptEntry* entry = (InterruptEntry*)(idt.offset + interruptNum * sizeof(InterruptEntry));
	set_interrupt_offset(entry, Handler);
	entry->type_attribute = int_ta_gate;
	entry->selector = 0x08;
}

void create_idt()
{
	idt.limit = 0x0FFF;
	idt.offset = (uint64_t)&idt_page[0];

	CreateEntry(0xE,  (uint64_t)PageFault_Handler);
	CreateEntry(0x8,  (uint64_t)DoubleFault_Handler);
	CreateEntry(0xD,  (uint64_t)GPFault_Handler);
	CreateEntry(0x21, (uint64_t)Keyboard_Handler);
	CreateEntry(0x2C, (uint64_t)MouseInt_Handler);
	CreateEntry(0x69, (uint64_t)Test_Handler);

	asm("lidt %0" : : "m" (idt));
	remap_interrupts_for_io();
	
	outb(mastr_pic_data_port, 0b11111101);
	outb(slave_pic_data_port, 0b11111111);
		
	asm("sti");
}


void remap_interrupts_for_io()
{
	uint8_t mastr_data_value;
	uint8_t slave_data_value;

	mastr_data_value = inb(mastr_pic_data_port); io_wait();
	slave_data_value = inb(slave_pic_data_port); io_wait();

	outb(mastr_pic_cmnd_port, icw1_init | icw1_icw4); io_wait();
	outb(slave_pic_cmnd_port, icw1_init | icw1_icw4); io_wait();

	outb(mastr_pic_data_port, 0x20); io_wait();
	outb(slave_pic_data_port, 0x28); io_wait();

	outb(mastr_pic_data_port, 4); io_wait();
	outb(slave_pic_data_port, 2); io_wait();

	outb(mastr_pic_data_port, icw4_8086); io_wait();
	outb(slave_pic_data_port, icw4_8086); io_wait();

	outb(mastr_pic_data_port, mastr_data_value); io_wait();
	outb(slave_pic_data_port, slave_data_value); io_wait();
}

void end_interrupts_for_mastr_io()
{
	outb(mastr_pic_cmnd_port, pic_eoi);
}

void end_interrupts_for_slave_io()
{
	outb(slave_pic_cmnd_port, pic_eoi);
	end_interrupts_for_mastr_io();
}