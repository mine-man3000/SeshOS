#include <stdint.h>
#include <io.h>

void comout(const char* input) {
    int i = 0;
    while (input[i] != '\0') {
        outb(0xE9, input[i]);
        i++;
    }
}

void comout_num(uint64_t num) {
    for (int i = 0; i < 16; i ++) {
        const char buf[2] = {"0123456789abcdef"[num >> 60], 0};
        comout(&buf[0]);
        num <<= 4;
    }
}