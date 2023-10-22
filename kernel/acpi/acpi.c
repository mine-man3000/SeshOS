/*
======================
The original version of this file is Copyright (C)Techflash Software, 2021 - 2023.
This is code from "Techflash OS", the hobbyist operating system developed by Techflash.
You may find the original code at https://github.com/techflashYT/Techflash-OS/blob/old/kernel/acpi.c
======================
*/
#include <stdio.h>
#include <string.h>
#include <acpi/acpi.h>
#include <kernel.h>
#include <stdbool.h>

MODULE("X86ACPI");

MCFGHeader* mcfg;

volatile struct limine_rsdp_request rsdpReq = {
	.id = LIMINE_RSDP_REQUEST,
	.revision = 0
};
static bool doChecksum(ACPISDTHeader *tableHeader) {
	uint_fast32_t sum = 0;

	for (uint32_t i = 0; i < tableHeader->length; i++) {
		sum += ((char *)tableHeader)[i];
	}
	if (sum != 0) {
		char str[64];
		#if UINT_FAST32_MAX == UINT32_MAX
		sprintf(str, "Checksum wrong by %u", sum);
		#elif UINT_FAST32_MAX == UINT64_MAX
		sprintf(str, "Checksum wrong by %lu", sum);
		#else
		#error Unsupported uint_fast32_t size
		#endif
		log(MODNAME, str, LOGLEVEL_WARN);
	}
	return sum == 0;
}

void ACPI_Init() {
	uint8_t *p = rsdpReq.response->address;
	if (memcmp(p, "RSD PTR ", 8) != 0) {
		log(MODNAME, "Bad ACPI", LOGLEVEL_FATAL);
		return;
	}
	char str[64];
	sprintf(str, "Found ACPI Tables at address %p!", p);
	log(MODNAME, str, LOGLEVEL_INFO);

	RSDPDescriptor20 *rsdp = (RSDPDescriptor20 *)p;
	uint8_t rev = rsdp->firstPart.revision;

	if (rev == 0) {
		log(MODNAME, "ACPI Revision 1.0", LOGLEVEL_INFO);
	}
	else {
		log(MODNAME, "ACPI Revision >= 2.0", LOGLEVEL_INFO);
	}
	char OEMID[7] = {0};
	memcpy(OEMID, rsdp->firstPart.OEMId, 6);

	sprintf(str, "OEM ID: %s", OEMID);
	log(MODNAME, str, LOGLEVEL_INFO);


	RSDT *rsdt = (RSDT *)(uint64_t)rsdp->firstPart.RSDTAddress;
	char sig[5] = {0};
	memcpy(sig, rsdt->h.signature, 4);

	if (memcmp(rsdt->h.signature, "RSDT", 4) != 0) {
		registers_t regs;
		DUMPREGS(&regs);
		sprintf(str, "ACPI RSDT signature not correct!  Expected: \"RSDT\", got: \"%-4s\"", sig);
		log(MODNAME, str, LOGLEVEL_FATAL);
		panic("ACPI RSDT Signature invalid", &regs);
	}

	sprintf(str, "Found ACPI RSDT.  Signature: \"%-4s\"", sig);
	log (MODNAME, str, LOGLEVEL_INFO);
	if (doChecksum(&rsdt->h)) {
		log(MODNAME, "ACPI RSDT Checksum passed.", LOGLEVEL_INFO);
	}

	memcpy(OEMID, rsdt->h.OEMId, 6);
	sprintf(str, "RSDT OEM ID: %s", OEMID);
	log(MODNAME, str, LOGLEVEL_INFO);

	uint32_t OEMRev = rsdt->h.OEMRevision;
	sprintf(str, "RSDT OEM Revision: %u", OEMRev);
	log(MODNAME, str, LOGLEVEL_INFO);

	
	int entries = (rsdt->h.length - sizeof(rsdt->h)) / 4;

	for (int i = 0; i < entries; i++) {
		ACPISDTHeader *h = (ACPISDTHeader *)(uint64_t)rsdt->pointerToOtherSDT[i];
		char str[64];

		memcpy(sig, h->signature, 4);
		sprintf(str, "Found ACPI Entry: \"%-4s\"", sig);
		log(MODNAME, str, LOGLEVEL_INFO);
		if (memcmp(h->signature, "FACP", 4) == 0) {
			// TODO: Handle FACP
			log(MODNAME, "Found FACP", LOGLEVEL_INFO);
		}
		if (memcmp(h->signature, "MCFG", 4) == 0) {
			mcfg = h;
		}
	}
}