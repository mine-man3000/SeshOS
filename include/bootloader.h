/*
======================
The original version of this file is Copyright (C)Techflash Software, 2021 - 2023.
This is code from "Techflash OS", the hobbyist operating system developed by Techflash.
You may find the original code at https://github.com/techflashYT/Techflash-OS/blob/master/include/kernel/bootloader.h
======================
*/

#include <stdint.h>
extern uint_fast8_t BOOT_LoaderID;
extern char BOOT_LoaderName[64];
extern char BOOT_LoaderVersion[64];

#define BOOT_LoaderID_Unknown 0
#define BOOT_LoaderID_LimineCompatible 1


extern void BOOT_CheckLoader();