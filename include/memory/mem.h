#pragma once

#include <stdint.h>
#include <stddef.h>
#include <kernel.h>

extern volatile struct limine_memmap_request mmap;
extern uint64_t physical_kernel_start;

uint64_t getMemSize();
void *memset(void *b, int c, int len);
void *memcpy(void* destination, const void* source, size_t num);
int numPages();