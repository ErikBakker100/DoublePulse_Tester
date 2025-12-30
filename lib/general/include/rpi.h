#pragma once

#include <stdint.h>
#include "config.h"
#include "BCM2835.h"
#include "BCM2836.h"
typedef struct {
    const char *name;
    uintptr_t mmio_base;
    uint32_t mmio_base_end;
    int32_t baudrate;
    int16_t core_freq_mhz;
    uint32_t soc_type;     // 0 = onbekend, 2836/2837/2838 etc.
    uint32_t gpu_io_base;
    uint32_t gpu_mem_base;
}rpi_board_t;

extern rpi_board_t rpi_board;

void board_init(void);
void irq_disable(void);
void irq_enable(void);
void fiq_disable(void);
void fiq_enable(void);
void clearBss(void);
void cpu_dsb(void);
void cpu_isb(void);
void cpu_sev(void);
void cpu_wfe(void);

