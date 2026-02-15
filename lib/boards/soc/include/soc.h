#pragma once
#include <stdint.h>
#include "../cpu/include/cpu.h"

typedef enum {
    BCM2835,
    BCM2836,
    BCM2837,
    BCM2711,
    BCM2712,
    BCM2837B0,
    RP3A0,
    UNKNOWN
}soc_list_t;

extern const char* soc_name_table[];

/* MMIO (Memory-Mapped Input/Output) */
typedef struct {
    uintptr_t base;
    uintptr_t base_end;
    uintptr_t gpu_io_base;
    uintptr_t gpu_mem_base;
    uintptr_t local_periph_base;
} peripheral_t;
extern peripheral_t mmio;

typedef struct {
    peripheral_t* mmio;
    cpu_data_t* cpu;
    const char* name;
} soc_data_t;
extern soc_data_t soc;

void soc_init(soc_list_t id);
void soc_info(soc_list_t id);
