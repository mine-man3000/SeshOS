#include "mem.h"

volatile struct limine_memmap_request mmap {
    .id = LIMINE_MEMMAP_REQUEST,
    .revision = 0,
};

uint64_t getMemSize()
{
    uint64_t memSize = 0;
    for (uint64_t i = 0; i < mmap.response->entry_count; i++)
    {
        memSize += mmap.response->entries[i]->length;
    }
    return memSize; 
}

extern "C" void *memset(void *b, int c, int len)
{
	uint8_t *p = (uint8_t*)b;
	while (len > 0)
    {
		*p = c;
		p++;
		len--;
	}
	return b;
}

extern "C" void *memcpy(void* destination, const void* source, size_t num)
{
	// Convert to char* for easy access
	char *src  = (char *)source;
	char *dest = (char *)destination;

	// Copy bytes
	for (size_t i = 0; i < num; i++)
    {
		dest[i] = src[i];
	}
	return destination;
}

int numPages()
{
    int pageCount = 0;
    for (uint64_t i = 0; i < mmap.response->entry_count; i++)
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