#pragma once
#include <stdint.h>
#include "../architecture/include/architectures.h"
#include "../../../../general/include/config.h"

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

typedef struct {
    uint8_t implementer;
    const char* name;
} implementer_t;
extern implementer_t implementer;

typedef struct {
    uint16_t        partnum;                // Bits[15:4]
    const char     *name;
    soc_list_t soc;
    uintptr_t mmio_base;
} partnum_t;
extern partnum_t partnum;

typedef struct {
    const implementer_t *implementer;       // bits [31:24]
    char rNpM[5];                           // bits [23:20] Variant (major core revision)
    architecture_t *architecture;           // bits [19:16]	Architecture (0xF = Cortex-M standard)
    const partnum_t *part;                // bits [15:4]  PartNo (core type)
} cpu_data_t;
extern cpu_data_t cpu;

void cpu_init(void);
void clearBss(void);
