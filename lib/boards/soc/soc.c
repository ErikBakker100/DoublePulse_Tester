#include "include/soc.h"
#include "BCM2835/include/BCM2835.h"
#include "BCM2836/include/BCM2836.h"
#include "BCM2837/include/BCM2837.h"
#include "BCM2711/include/BCM2711.h"
#include "BCM2712/include/BCM2712.h"

soc_data_t soc;

uintptr_t memory_table[][4] = {
    { 0x20FFFFFFUL, 0x7E000000UL, 0xC0000000UL, 0x00000000UL}, // BCM2835
    { 0x3FFFFFFFUL, 0x7E000000UL, 0xC0000000UL, 0x40000000UL}, // BCM2836
    { 0x3FFFFFFFUL, 0x7E000000UL, 0xC0000000UL, 0x40000000UL}, // BCM2837
    { 0xFE3FFFFFUL, 0x7E000000UL, 0xC0000000UL, 0xFF800000UL}, // BCM2711
    { 0xFEFFFFFFUL, 0x7E000000UL, 0xC0000000UL, 0x00000000UL}, // BCM2712
    { 0x3FFFFFFFUL, 0x7E000000UL, 0xC0000000UL, 0x40000000UL}, // BCM2837B0
    { 0x3FFFFFFFUL, 0x7E000000UL, 0xC0000000UL, 0x40000000UL}, // RP3A0
    { 0x00000000UL, 0x00000000UL, 0x00000000UL, 0x00000000UL}  // UNKNOWN
};

const char* soc_name_table[] = {
    "Broadcom BCM2835",
    "Broadcom BCM2836",
    "Broadcom BCM2837",
    "Broadcom BCM2711",
    "Broadcom BCM2712",
    "Broadcom BCM2837B0",
    "Broadcom RP3A0",
    "Unknown SoC"
};

void soc_init(){
    soc = (soc_data_t){
        .base = cpu.part->mmio_base,
        .base_end = memory_table[cpu.part->soc][0],
        .gpu_io_base = memory_table[cpu.part->soc][1],
        .gpu_mem_base = memory_table[cpu.part->soc][2],
        .local_periph_base = memory_table[cpu.part->soc][3],
        .name = soc_name_table[cpu.part->soc]
    };
    switch (cpu.part->soc) {
        case BCM2835:
            BCM2835_init(&soc);
            break;
        case BCM2836:
            BCM2836_init(&soc);
            break;
        case BCM2837:
            BCM2837_init(&soc);
            break;
        case RP3A0:
            BCM2837_init(&soc);
            break;
        case BCM2837B0:
            BCM2837_init(&soc);
            break;
        case BCM2711:
            BCM2711_init(&soc);
            break;
        case BCM2712:
            BCM2712_init(&soc);
            break;
        case UNKNOWN:                       // board not found, error
            break;
    }
}
