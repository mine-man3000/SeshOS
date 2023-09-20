#pragma once
#include <stdint.h>

typedef struct {
	char signature[8];
	uint8_t checksum;
	char OEMId[6];
	uint8_t revision;
	uint32_t RSDTAddress;
} __attribute__ ((packed)) RSDPDescriptor;

typedef struct {
	RSDPDescriptor firstPart;
	
	uint32_t len;
	uint64_t XSTDAddress;
	uint8_t extendedChecksum;
	uint8_t reserved[3];
} __attribute__ ((packed)) RSDPDescriptor20;

typedef struct {
	char signature[4];
	uint32_t length;
	uint8_t revision;
	uint8_t checksum;
	char OEMId[6];
	char OEMTableID[8];
	uint32_t OEMRevision;
	uint32_t creatorID;
	uint32_t creatorRevision;
} __attribute__ ((packed)) ACPISDTHeader;
typedef struct {
	ACPISDTHeader h;
	// (h.length - sizeof(h)) / 4
	uint32_t pointerToOtherSDT[];
} __attribute__ ((packed)) RSDT;

extern void ACPI_Init();