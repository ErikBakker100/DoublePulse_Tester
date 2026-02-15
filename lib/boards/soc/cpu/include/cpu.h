#pragma once
#include <stdint.h>
#include "../architecture/include/architectures.h"
#include "../../../../general/include/config.h"

typedef struct {
    uint8_t implementer;
    const char* name;
} implementer_t;
extern implementer_t implementer;

static const implementer_t unknown_implementer = {
    .implementer= 0xFF,
    .name       = "Unknown implementer"
};

typedef struct {
    uint16_t        partnum;                // Bits[15:4]
    const char     *name;
} partnum_t;
extern partnum_t partnum;

static const partnum_t unknown_cpu = {
    .partnum = 0x000,
    .name    = "Unknown CPU"
};

typedef struct {
    const implementer_t *implementer;       // bits [31:24]
    char rNpM[5];                           // bits [23:20] Variant (major core revision)
    architecture_t *architecture;           // bits [19:16]	Architecture (0xF = Cortex-M standard)
    const partnum_t *part;                // bits [15:4]  PartNo (core type)
} cpu_data_t;
extern cpu_data_t cpu;

void cpu_init(void);
void cpu_info(void);

void clearBss(void);
void cpu_dsb(void);
void cpu_isb(void);
void cpu_sev(void);
void cpu_wfe(void);