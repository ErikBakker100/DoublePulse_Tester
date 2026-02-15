#pragma once
#include <stdint.h>
#include "../cpu/include/cpu.h"

extern const char* soc_name_table[];

typedef struct {
    uintptr_t base;
    uintptr_t base_end;
    uintptr_t gpu_io_base;
    uintptr_t gpu_mem_base;
    uintptr_t local_periph_base;
    const char* name;
} soc_data_t;
extern soc_data_t soc;

void soc_init(void);

