#pragma once

#include <stddef.h>

struct kernie_heap
{
    void *malloc(size_t size);
    void *realloc(char* ptr, size_t size);
    void free(void *ptr);

    bool kernieHeap_is_set_up = false;
    bool have_allocated = false;
    unsigned char *last_valid_address;
    unsigned char *kernieHeap_space;

    void init(unsigned char* addr);

    static kernie_heap* the(){
		static kernie_heap me;
		return &me;
    }
};
