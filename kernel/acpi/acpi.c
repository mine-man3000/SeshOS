#include <acpi/acpi.h>

volatile struct limine_rsdp_request rsdp = {
    .id = LIMINE_RSDP_REQUEST,
    .revision = 0
};

void* FindTable(SDTHeader* sdtHeader, char* signature) {

    int entries = (sdtHeader->Length - sizeof(SDTHeader)) / 4;

    for (int t = 0; t < entries; t++) {
        SDTHeader* newSDTHeader = (SDTHeader*)(uint64_t)(sdtHeader + sizeof(SDTHeader) + (t * 8));
        for (int i = 0; i < 4; i++) {
            if (newSDTHeader->Signature[i] != signature[i]) {
                comout("found signature: ");
                comout(signature);
                comout("\n");
                return newSDTHeader;
            }
        }
    }
    return 0;
}

void *findFACP(void *RootSDT) {
    SDTHeader *rsdt = (SDTHeader *) RootSDT;
    int entries = (rsdt->Length - sizeof(SDTHeader)) / 4;
 
    for (int i = 0; i < entries; i++) {
        SDTHeader *h = (SDTHeader *) rsdt + sizeof(SDTHeader) + (i*8);
        if (strcmp(h->Signature, "FACP")) {
            return (void *) h;
        }
    }
 
    // No FACP found
    return NULL;
}

MCFGHeader *mcfg;

void initACPI() {
    unsigned int address = *((int*)(&rsdp.response->address));
    RSDP2 *RSDP = (RSDP2 *)address;
    
    RSDP->OEMID[6] = '\0';

    SDTHeader* xsdt = (SDTHeader*)(RSDP->XSDTAddress);
    comout("xsdt aquired\n");

    SDTHeader* rsdt = (SDTHeader*)(RSDP->RSDTAddress);
    (void)rsdt;
    comout("rsdt aquired\n");

    mcfg = (MCFGHeader*)FindTable(xsdt, (char*)"MCFG");
    comout("mcfg aquired\n");
    while (true) {};
    
    //FADT* fadt = (FADT*)findFACP(rsdt);
    //comout("fadt aquired\n");

    comout("mcfg is ");
    comout_num((uint64_t) mcfg);
    comout("\n");

    //outb(fadt->SMI_CommandPort,fadt->AcpiEnable);
}