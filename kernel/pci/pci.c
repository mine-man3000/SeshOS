#include <pci/pci.h>
#include <disks/sata.h>
#include <acpi/acpi.h>
#include <stdio.h>

void EnumerateFunction(uint64_t deviceAddress, uint64_t function) {
	uint64_t offset = function << 12;
	uint64_t functionAddress = deviceAddress + offset;

	PCIDeviceHeader *pciDeviceHeader = (PCIDeviceHeader *)functionAddress;

	if (pciDeviceHeader->DeviceID == 0)
		return;

	if (pciDeviceHeader->DeviceID == 0xFFFF)
		return;

	printf("%s / %s / %s / %s / %s\n", GetVendorName(pciDeviceHeader->VendorID), GetDeviceName(pciDeviceHeader->VendorID, pciDeviceHeader->DeviceID), DeviceClasses[pciDeviceHeader->Class], GetSubclassName(pciDeviceHeader->Class, pciDeviceHeader->Subclass), GetProgIFName(pciDeviceHeader->Class, pciDeviceHeader->Subclass, pciDeviceHeader->ProgIF));

	switch (pciDeviceHeader->Class) {
		case 0x01: { // mass storage controller
			switch (pciDeviceHeader->Subclass) {
				case 0x06: { // Serial ATA
					switch (pciDeviceHeader->ProgIF) {
						case 0x01: { // AHCI 1.0 device
							AHCIDriver_Init(pciDeviceHeader);
							//new AHCI::AHCIDriver(pciDeviceHeader);
							break;
						}
					}
				}
			}
		}
	}
}

void EnumerateDevice(uint64_t busAddress, uint64_t device) {
	uint64_t offset = device << 15;

	uint64_t deviceAddress = busAddress + offset;

	PCIDeviceHeader *pciDeviceHeader = (PCIDeviceHeader *)deviceAddress;

	if (pciDeviceHeader->DeviceID == 0) {
		return;
	}
	if (pciDeviceHeader->DeviceID == 0xFFFF) {
		return;
	}

	for (uint64_t function = 0; function < 8; function++) {
		EnumerateFunction(deviceAddress, function);
	}
}

void EnumerateBus(uint64_t baseAddress, uint64_t bus) {
	uint64_t offset = bus << 20;

	uint64_t busAddress = baseAddress + offset;

	PCIDeviceHeader *pciDeviceHeader = (PCIDeviceHeader *)busAddress;


	if (pciDeviceHeader->DeviceID == 0) {
		return;
	}
	if (pciDeviceHeader->DeviceID == 0xFFFF) {
		return;
	}

	for (uint64_t device = 0; device < 32; device++) {
		EnumerateDevice(busAddress, device);
	}
}

void EnumeratePCI(MCFGHeader *mcfg) {

    if(!mcfg) {
        panic("MCFG was not found!", NULL);
    }

	int entries = ((mcfg->Header.length) - sizeof(MCFGHeader)) / sizeof(DeviceConfig);
	for (int t = 0; t < entries; t++) {
		DeviceConfig *newDeviceConfig = (DeviceConfig *)((uint64_t)mcfg + sizeof(MCFGHeader) + (sizeof(DeviceConfig) * t));

		for (uint64_t bus = newDeviceConfig->StartBus; bus < newDeviceConfig->EndBus; bus++) {
			EnumerateBus(newDeviceConfig->BaseAddress, bus);
		}
	}
}
