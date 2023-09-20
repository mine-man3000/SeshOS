#include <memory/mem.h>

bool kernieHeap_is_set_up = false;
bool have_allocated = false;
unsigned char *last_valid_address;
unsigned char *kernieHeap_space;
volatile struct limine_memmap_request mmap = {
    .id = LIMINE_MEMMAP_REQUEST,
    .revision = 0,
};

uint64_t getMemSize() {
    uint64_t memSize = 0;
    for (uint64_t i = 0; i < mmap.response->entry_count; i++) {
        memSize += mmap.response->entries[i]->length;
    }
    return memSize; 
}

void *memset(void *b, int c, int len) {
	uint8_t *p = (uint8_t*)b;
	while (len > 0) {
		*p = c;
		p++;
		len--;
	}
	return b;
}
void *memcpy(void* destination, const void* source, size_t num) {
	// Convert to char* for easy access
	char *src  = (char *)source;
	char *dest = (char *)destination;

	// Copy bytes
	for (size_t i = 0; i < num; i++) {
		dest[i] = src[i];
	}
	return destination;
}

int memcmp(const void *s1, const void *s2, int_fast64_t len) {
	const uint8_t *p = s1;
	const uint8_t *q = s2;
	int charCompareStatus = 0;
	// If both pointer pointing same memory block
	if (s1 == s2) {
		return charCompareStatus;
	}
	while (len > 0) {
		if (*p != *q) {
			// compare the mismatching character
			charCompareStatus = (*p > *q) ? 1 : -1;
			break;
		}
		len--;
		p++;
		q++;
	}
	return charCompareStatus;
}

int numPages() {
    int pageCount = 0;
    for (uint64_t i = 0; i < mmap.response->entry_count; i++) {
        if (mmap.response->entries[i]->length > 4096) {
            pageCount += mmap.response->entries[i]->length / 4194304;
        }
        else {
            pageCount++;
        }
    }
    return pageCount;
}