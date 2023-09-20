/*
======================
The original version of this file is Copyright (C)Techflash Software, 2021 - 2023.
This is code from "Techflash OS", the hobbyist operating system developed by Techflash.
You may find the original code at https://github.com/techflashYT/Techflash-OS/blob/master/include/kernel/log.h
======================
*/

#pragma once
typedef struct {
	uint64_t rdi, rsi, rbp, rbx, rdx, rcx, rax;
	uint64_t r8, r9, r10, r11, r12, r13, r14, r15;
	uint64_t intNo, errCode;
	uint64_t rip, cs, rflags, rsp, ss;
} __attribute__((packed)) registers_t;