#include "mem.h"

volatile struct limine_memmap_request mmap {
    .id = LIMINE_MEMMAP_REQUEST,
    .revision = 0,
};

uint64_t getMemSize()
{
    uint64_t memSize = 0;
    for (int i = 0; i < mmap.response->entry_count; i++)
    {
        memSize += mmap.response->entries[i]->length;
    }
    return memSize; 
    mmap.response->entry_count;

}

void memset(void* base, uint8_t value, uint64_t length)
{
	for (uint64_t i = 0; i < length; i++)
        *(uint8_t*)((uint64_t)base + i) = value;
}

int numPages()
{
    int pageCount = 0;
    for (int i = 0; i < mmap.response->entry_count; i++)
    {
        if (mmap.response->entries[i]->length > 4096)
        {
            pageCount += mmap.response->entries[i]->length / 4194304;
        }
        else
        {
            pageCount++;
        }
    }
    return pageCount;
}