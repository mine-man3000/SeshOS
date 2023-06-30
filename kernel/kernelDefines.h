#pragma once

#include "limine.h"

volatile struct limine_hhdm_request hhdm_request = {
    .id = LIMINE_HHDM_REQUEST,
    .revision = 0
};
uint64_t hhdm_offset;

volatile struct limine_kernel_address_request kernel_address_request = {
    .id = LIMINE_KERNEL_ADDRESS_REQUEST,
    .revision = 0
};
uint64_t physical_kernel_start;
uint64_t virtual_kernel_start;
 
struct limine_module_request module = {
    .id = LIMINE_MODULE_REQUEST,
    .revision = 0
};
