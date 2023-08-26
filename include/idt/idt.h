#pragma once
#include <stdint.h>

#define int_ta_gate 0b10001110
#define int_ta_call_gate 0b10001100
#define int_ta_trap_gate 0b10001111

void create_idt();

typedef struct
{
	uint16_t offset_0;
	uint16_t selector;
	uint8_t stack_list_offset;
	uint8_t type_attribute;
	uint16_t offset_1;
	uint32_t offset_2;
	uint32_t ignore;
} InterruptEntry;

void set_interrupt_offset(InterruptEntry* int_ptr, uint64_t offset);
uint64_t get_interrupt_offset(InterruptEntry* int_ptr);

typedef struct __attribute__((packed))
{
	uint16_t limit;
	uint64_t offset;
} IDT;

extern IDT idt;