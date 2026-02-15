#include "include/soc.h"
#include "BCM2835/include/BCM2835.h"
#include "BCM2836/include/BCM2836.h"
#include "BCM2837/include/BCM2837.h"
#include "BCM2711/include/BCM2711.h"
#include "BCM2712/include/BCM2712.h"

soc_data_t soc;
peripheral_t mmio;

peripheral_t mmio_table[] = {
    { 0x20000000UL, 0x20FFFFFFUL, 0x7E000000UL, 0xC0000000UL, 0x00000000UL}, // BCM2835
    { 0x3F000000UL, 0x3FFFFFFFUL, 0x7E000000UL, 0xC0000000UL, 0x40000000UL}, // BCM2836
    { 0x3F000000UL, 0x3FFFFFFFUL, 0x7E000000UL, 0xC0000000UL, 0x40000000UL}, // BCM2837
    { 0xFE000000UL, 0xFE3FFFFFUL, 0x7E000000UL, 0xC0000000UL, 0xFF800000UL}, // BCM2711
    { 0xFE000000UL, 0xFEFFFFFFUL, 0x7E000000UL, 0xC0000000UL, 0x00000000UL}, // BCM2712
    { 0x3F000000UL, 0x3FFFFFFFUL, 0x7E000000UL, 0xC0000000UL, 0x40000000UL}, // BCM2837B0
    { 0x3F000000UL, 0x3FFFFFFFUL, 0x7E000000UL, 0xC0000000UL, 0x40000000UL}, // RP3A0
    { 0x00000000UL, 0x00000000UL, 0x00000000UL, 0x00000000UL, 0x00000000UL}  // UNKNOWN
};

const char* soc_name_table[] = {
    "Broadcom BCM2835",
    "Broadcom BCM2836",
    "Broadcom BCM2837",
    "Broadcom BCM2711",
    "Broadcom BCM2712",
    "Broadcom BCM2837B0",
    "Broadcom RP3A0 = BCM2837",
    "Unknown SoC"
};

void mmio_set(soc_list_t id) {
    soc.mmio = &mmio_table[id];
}

void soc_init(soc_list_t id){
    soc.mmio = &mmio;
    soc.cpu = &cpu;
    mmio_set(id);             // First set the peripheral base, based on the SOC in use.
    switch (id) {
        case BCM2835:
            BCM2835_init(soc.mmio);
            break;
        case BCM2836:
            BCM2836_init(soc.mmio);
            break;
        case BCM2837:
            BCM2837_init(soc.mmio);
            break;
        case RP3A0:
            BCM2837_init(soc.mmio);
            break;
        case BCM2837B0:
            BCM2837_init(soc.mmio);
            break;
        case BCM2711:
            BCM2711_init(soc.mmio);
            break;
        case BCM2712:
            BCM2712_init(soc.mmio);
            break;
        case UNKNOWN:                       // board not found, error
            break;
    }
    cpu_init();
}

void soc_info(soc_list_t id) {
    soc.name = soc_name_table[id];
    cpu_info();
};
