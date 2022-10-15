#pragma once

#include <stddef.h>

struct kernie_heap
{
    void *malloc(size_t size);
    void *realloc(void *ptr, size_t size);
    void free(void *ptr);

    bool heap_is_set_up = false;
    bool have_allocated = false;
    unsigned char *last_valid_address;
    unsigned char *heap_space;
};