#pragma once

#include <stdint.h>
#include <memory/mem.h>
#include <video/renderer.h>
#include <string.h>
#include <acpi/acpi.h>

typedef struct {
    uint16_t VendorID;
    uint16_t DeviceID;
    uint16_t Command;
    uint16_t Status;
    uint8_t RevisionID;
    uint8_t ProgIF;
    uint8_t Subclass;
    uint8_t Class;
    uint8_t CacheLineSize;
    uint8_t LatencyTimer;
    uint8_t HeaderType;
    uint8_t BIST;
} __attribute__((packed)) PCIDeviceHeader;

typedef struct {
    PCIDeviceHeader Header;
    uint32_t BAR0;
    uint32_t BAR1;
    uint32_t BAR2;
    uint32_t BAR3;
    uint32_t BAR4;
    uint32_t BAR5;
    uint32_t CardbusCISPtr;
    uint16_t SubsystemVendorID;
    uint16_t SubsystemID;
    uint32_t ExpansionROMBaseAddr;
    uint8_t CapabilitiesPtr;
    uint8_t Rsv0;
    uint16_t Rsv1;
    uint32_t Rsv2;
    uint8_t InterruptLine;
    uint8_t InterruptPin;
    uint8_t MinGrant;
    uint8_t MaxLatency;
} __attribute__((packed)) PCIHeader0;

void EnumeratePCI(MCFGHeader *mcfg);

extern const char *DeviceClasses[];

const char *GetVendorName(uint16_t vendorID);
const char *GetDeviceName(uint16_t vendorID, uint16_t deviceID);
const char *GetSubclassName(uint8_t classCode, uint8_t subclassCode);
const char *GetProgIFName(uint8_t classCode, uint8_t subclassCode, uint8_t progIF);