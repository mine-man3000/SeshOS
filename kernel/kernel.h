#pragma once
#include "limine.h"
#include "video/renderer.h"
#include "string.h"
#include "video/video.h"
#include "init.h"
#include "memory/mem.h"
#include "time.h"
#include "shell.h"
#include "idt/interrupts.h"

void comout(const char* input);
void comout_num(uint64_t num);

extern struct limine_module_request module;
extern struct limine_file* initramfs;
extern struct limine_file* font;
void Panic(const char *panicMessage, interrupt_frame *regs);