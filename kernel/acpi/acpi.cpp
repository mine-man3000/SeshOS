#include "acpi.h"

volatile struct limine_rsdp_request rsdp = {
    .id = LIMINE_RSDP_REQUEST,
    .revision = 0
};

void* FindTable(SDTHeader* sdtHeader, char* signature) {

    int entries = (sdtHeader->Length - sizeof(SDTHeader)) / 8;

    for (int t = 0; t < entries; t++) {
        SDTHeader* newSDTHeader = (SDTHeader*)(uint64_t)(sdtHeader + sizeof(SDTHeader) + (t * 8));
            for (int i = 0; i < 4; i++) {
                if (newSDTHeader->Signature[i] != signature[i]) {
                    break;
                }
            if (i == 3) return newSDTHeader;
        }
    }
    return 0;
}

void initACPI() {
    unsigned int address = *((int*)(&rsdp.response->address));
    struct RSDP2 *RSDP = (struct RSDP2 *)address;

    RSDP->OEMID[6] = '\0';

    SDTHeader* xsdt = (SDTHeader*)(RSDP->XSDTAddress);
    comout("xsdt aquired\n");
    SDTHeader* rsdt = (SDTHeader*)(RSDP->RSDTAddress);    
    comout("rsdt aquired\n");
    MCFGHeader* mcfg = (MCFGHeader*)FindTable(xsdt, (char*)"MCFG");
    comout("mcfg aquired\n");
    FADT* fadt = (FADT*)FindTable(rsdt, (char*)"FACP");
    comout("fadt aquired\n");

    comout("fadt is ");
    comout_num((uint64_t) fadt);
    comout("\n");

    outb(fadt->SMI_CommandPort,fadt->AcpiEnable);
}