#pragma once
#include <stdint.h>
#include "BCM2835.h"
#include "BCM2836.h"

#ifndef CORE_FREQ
    #define CORE_FREQ 250
#endif

#ifndef BAUDRATE
    #define BAUDRATE 115200
#endif

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
typedef struct {
    uint32_t arch;   // 32 of 64
    uint32_t level;     // Exception level (EL0..EL3)
    char level_str[16]; // Mode string
    uint32_t irq_masked; // 1 = IRQ masked
    uint32_t fiq_masked; // 1 = FIQ masked
    char mode[6];    // Thumb or ARM mode (32-bit only)
} cpu_state_t;

extern cpu_state_t rpi_cpu;

void print_board_info(void);
void print_cpu_info(void);
void board_init(void);
void irq_disable(void);
void irq_enable(void);
void fiq_disable(void);
void fiq_enable(void);
