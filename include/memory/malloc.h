#pragma once

#include <stddef.h>
#include <stdbool.h>

extern void *malloc(size_t size);
extern void *realloc(char* ptr, size_t size);
extern void free(void *ptr);
extern void MEM_Init(unsigned char* addr);

extern bool kernieHeap_is_set_up;
extern bool have_allocated;
extern unsigned char *last_valid_address;
extern unsigned char *kernieHeap_space;
