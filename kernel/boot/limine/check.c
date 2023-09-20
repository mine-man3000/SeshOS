/*
======================
The original version of this file is Copyright (C)Techflash Software, 2021 - 2023.
This is code from "Techflash OS", the hobbyist operating system developed by Techflash.
You may find the original code at https://github.com/techflashYT/Techflash-OS/blob/master/kernel/bootloaders/limine/check.c
======================
*/
#include <limine.h>
#include <stdio.h>
#include <stdbool.h>
struct limine_bootloader_info_request bootloaderInfo = {
	.id = LIMINE_BOOTLOADER_INFO_REQUEST,
	.revision = 0
};


struct limine_bootloader_info_response *LM_Check() {
	// Check if we were actually booted by Limine.  Simple, see if we got a response from the "get info" request
	if (bootloaderInfo.response == NULL) {
		return NULL;
	}
	return bootloaderInfo.response;
}