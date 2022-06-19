#pragma once
#include "limine.h"
#include "video/renderer.h"
#include "idt/interrupts.h"
#include "string.h"
#include "video/video.h"
#include "init.h"
#include "memory/mem.h"
#include "time.h"
#include "shell.h"

void comout(const char* input, ...);

extern struct limine_module_request module;
extern struct limine_file* initramfs;

static void done(void)
{
    for (;;)
    {
        __asm__("hlt");
    }
}