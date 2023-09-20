/*
======================
The original version of this file is Copyright (C)Techflash Software, 2021 - 2023.
This is code from "Techflash OS", the hobbyist operating system developed by Techflash.
You may find the original code at https://github.com/techflashYT/Techflash-OS/blob/master/include/kernel/log.h
Another portion of this code is from https://github.com/techflashYT/Techflash-OS/blob/master/include/misc/defines.h
Another portion of this code is from https://github.com/techflashYT/Techflash-OS/blob/master/include/kernel/registers.h
======================
*/


#include <stdint.h>
// info that will almost never be useful except for when initially creating the code.
#define LOGLEVEL_VERBOSE 0
// info that could potentially be useful if having issues
#define LOGLEVEL_DEBUG   1
// info that could potentially be useful to an average user
#define LOGLEVEL_INFO    2
// something unexpected happened, this is a warning the user
#define LOGLEVEL_WARN    3
// something bad happened, we need to tell the user
#define LOGLEVEL_ERROR   4
// something EXTREMELY bad happened, this will usually be extended info just before a kernel panic.
#define LOGLEVEL_FATAL   5
extern void log(const char module[8], const char *msg, const uint8_t level);


#define MODULE(x) static const char *MODNAME = (x)
#define errorColor   0xFFFF4444
#define warningColor 0xFFFFD866

#define BUILD_BUG_ON(condition) ((void)sizeof(char[1 - 2*!!(condition)]))


#ifdef __x86_64__
#include <arch/x86/registers.h>
#endif

static inline void DUMPREGS(registers_t *regs) {
	asm ("mov %%rax,   %0" : "=r" (regs->rax));
	asm ("mov %%rbx,   %0" : "=r" (regs->rbx));
	asm ("mov %%rcx,   %0" : "=r" (regs->rcx));
	asm ("mov %%rdx,   %0" : "=r" (regs->rdx));
	asm ("mov %%rdi,   %0" : "=r" (regs->rdi));
	asm ("mov %%rsi,   %0" : "=r" (regs->rsi));
	asm ("mov %%rbp,   %0" : "=r" (regs->rbp));
	asm ("mov %%rsp,   %0" : "=r" (regs->rsp));
	asm ("mov %%r8,    %0" : "=r" (regs->r8));
	asm ("mov %%r9,    %0" : "=r" (regs->r9));
	asm ("mov %%r10,   %0" : "=r" (regs->r10));
	asm ("mov %%r11,   %0" : "=r" (regs->r11));
	asm ("mov %%r12,   %0" : "=r" (regs->r12));
	asm ("mov %%r13,   %0" : "=r" (regs->r13));
	asm ("mov %%r14,   %0" : "=r" (regs->r14));
	asm ("mov %%r15,   %0" : "=r" (regs->r15));
	asm ("mov %%cs,    %0" : "=r" (regs->cs));
	asm ("mov %%ss,    %0" : "=r" (regs->ss));
	asm ("sub $8, %%rsp; pushfq; popq %0; add $8, %%rsp" : "=r" (regs->rflags));
	asm ("lea (%%rip), %0" : "=r" (regs->rip));
	regs->intNo = 0x0;
	regs->errCode = 0x0;
}