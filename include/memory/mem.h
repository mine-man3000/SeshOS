#pragma once

#include <stdint.h>
#include <stddef.h>
#include <kernel.h>

extern volatile struct limine_memmap_request mmap;
extern uint64_t physical_kernel_start;

uint64_t getMemSize();
int numPages();