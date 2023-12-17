#include <stdint.h>

typedef struct {
    uint64_t signature;
    uint32_t revision;
    uint32_t headerSize;
    uint32_t checksum;
    uint32_t reserved;
    uint64_t LBA;
    uint64_t LBAAlt;
    uint64_t firstUsableBlock;
    uint64_t lastUsableBlock;
    uint64_t guidPartOne;
    uint64_t guidPartTwo;
    uint64_t startLBA;
    uint32_t numParts;
    uint32_t entrySize;
    uint32_t CRC32;
    uint8_t a[420]; //hehe... nice
} GPTTable;