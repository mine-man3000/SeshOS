#include "acpi.h"

struct limine_rsdp_request rsdp =
{
    .id = LIMINE_RSDP_REQUEST,
    .revision = 0,
};


void yes()
{
    printf("%s\n", (char *)rsdp.response->address);
}