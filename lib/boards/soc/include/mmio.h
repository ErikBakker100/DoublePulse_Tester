#pragma once
#include <stdint.h>
#include "../cpu/include/arm_partnum.h"
#include "soc.h"

/* MMIO (Memory-Mapped Input/Output) */
typedef struct {
    soc_t soc;
    uintptr_t base;
    uintptr_t base_end;
    uintptr_t gpu_io_base;
    uintptr_t gpu_mem_base;
    uintptr_t local_periph_base;
} mmio_t;

extern const mmio_t soc_table[];

void mmio_set(soc_t);
extern const mmio_t *mmio;
