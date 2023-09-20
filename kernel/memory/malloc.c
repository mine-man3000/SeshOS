#include <memory/malloc.h>	
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <video/renderer.h>
#include <kernel.h>

typedef struct {
    bool free;
    size_t size;
} mem_control_blk;
    
void *aligned_ptr(char *ptr) {
    while ((uintptr_t) ptr % sizeof(int) != 0) {
        ptr++;
    }

    return ptr;
}

void *malloc(size_t size) {
    if (!kernieHeap_is_set_up) {
        last_valid_address = kernieHeap_space;
        kernieHeap_is_set_up = true;
    }

    unsigned char *current_ptr = kernieHeap_space;
    char *allocated_location = 0;

    size += sizeof(mem_control_blk);
    mem_control_blk *mcb = (mem_control_blk *)current_ptr;

    if (have_allocated) {
        while (current_ptr != last_valid_address) {
            if (mcb->free && mcb->size >= size) {
                mcb->free = false;
                allocated_location = (char*)current_ptr;
                break;
            }

            current_ptr += mcb->size;
            current_ptr = (unsigned char*)aligned_ptr((char*)current_ptr);
            comout((const char*)current_ptr);
        }
    }

    if (!allocated_location) {
        allocated_location = (char*)last_valid_address;
        last_valid_address += size;

        mcb = (mem_control_blk*)allocated_location;
        mcb->free = false;
        mcb->size = size;
    }

    allocated_location += sizeof(mem_control_blk);

    return allocated_location;
}

void *realloc(char* ptr, size_t size) {
    if (!ptr) {
        return malloc(size);
    }

    if (!size && ptr) {
        free(ptr);
        return NULL;
    }

    void *new_ptr = malloc(size);

    mem_control_blk *mcb = (mem_control_blk *)(ptr - sizeof(mem_control_blk));

    memcpy(new_ptr, ptr, mcb->size);
    free(ptr);

    return new_ptr;
}

void free(void* ptr) {
    if (!ptr) return;

    mem_control_blk *mcb = (mem_control_blk *)((char*)ptr - sizeof(mem_control_blk));
    if (!mcb->size) return;

    mcb->free = true;
}

void MEM_Init(unsigned char* addr) {
    kernieHeap_space = addr;
}