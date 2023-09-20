/*
======================
The original version of this file is Copyright (C)Techflash Software, 2021 - 2023.
This is code from "Techflash OS", the hobbyist operating system developed by Techflash.
You may find the original code at https://github.com/techflashYT/Techflash-OS/blob/master/kernel/bootloaders/limine/framebuffer.c
======================
*/
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpadded"
#include <limine.h>
#pragma GCC diagnostic pop

#include <tty/fbCon.h>
volatile struct limine_framebuffer_request fbRequest = {
	.id = LIMINE_FRAMEBUFFER_REQUEST,
	.revision = 0
};

static framebuffer_t fbInfoLocal;

framebuffer_t LM_GetFramebuffer() {
	struct limine_framebuffer *res = fbRequest.response->framebuffers[0];
	fbInfoLocal.ptr    = (uint32_t *)res->address;
	fbInfoLocal.width  = (uint32_t)res->width;
	fbInfoLocal.height = (uint32_t)res->height;

	fbInfoLocal.mode = FB_MODE_RGBA;

	return fbInfoLocal;
}