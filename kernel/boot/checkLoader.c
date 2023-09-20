/*
======================
The original version of this file is Copyright (C)Techflash Software, 2021 - 2023.
This is code from "Techflash OS", the hobbyist operating system developed by Techflash.
You may find the original code at https://github.com/techflashYT/Techflash-OS/blob/master/kernel/bootloaders/check.c
======================
*/
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <limine.h>
#include <bootloader.h>

uint_fast8_t BOOT_LoaderID;
char BOOT_LoaderName[64];
char BOOT_LoaderVersion[64];

extern struct limine_bootloader_info_response *LM_Check();
MODULE("BOOTDTCT");

void BOOT_CheckLoader() {
	log(MODNAME, "=== Autodetecting Bootloader ===", LOGLEVEL_INFO);

	char str[128];
	strcpy(str, "Limine... ");
	struct limine_bootloader_info_response *limineData = LM_Check();

	if (limineData != NULL) {
		strcat(str, "YES");
		log(MODNAME, str, LOGLEVEL_INFO);

		sprintf(str, "Found Limine compatible bootloader, named \"%s\", with version \"%s\"", limineData->name, limineData->version);
		log(MODNAME, str, LOGLEVEL_INFO);

		if (strlen(limineData->name) <= 64) {
			strcpy(BOOT_LoaderName, limineData->name);
		}
		else {
			memcpy(BOOT_LoaderName, limineData->name, 64);
		}

		if (strlen(limineData->version) <= 64) {
			strcpy(BOOT_LoaderVersion, limineData->version);
		}
		else {
			memcpy(BOOT_LoaderVersion, limineData->version, 64);
		}

		BOOT_LoaderID = BOOT_LoaderID_LimineCompatible;	
		
		return;
	}
	strcat(str, "NO");
	log(MODNAME, str, LOGLEVEL_INFO);

	// TODO: Check for other bootloaders

	log(MODNAME, "No known bootloader detected!  Disabling bootloader-specific features.", LOGLEVEL_WARN);
}