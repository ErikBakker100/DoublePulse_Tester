#pragma once
#include <stdint.h>
#include "../architecture/include/architectures.h"
#include "../../../../general/include/config.h"
#include "../../include/soc.h"

typedef struct {
    uint8_t implementer;
    const char* name;
} implementer_t;
extern implementer_t implementers[];

static const implementer_t unknown_implementer = {
    .implementer= 0xFF,
    .name       = "Unknown implementer"
};

typedef struct {
    uint16_t        partnum;        // Bits[15:4]
    const char     *name;
} partnum_t;
extern partnum_t partnumbers[];

static const partnum_t unknown_cpu = {
    .partnum = 0x000,
    .name    = "Unknown CPU"
};

typedef struct {
    const implementer_t *implementer;             // bits [31:24]
    uint8_t variant;                        // bits [23:20] Variant (rev van core)
    architecture_t *architecture;           // bits [19:16]	Architecture (0xF = Cortex-M standard)
    const partnum_t *partno;                      // bits [15:4]  PartNo (core type)
    uint8_t revision;                       // bits [3:0]   Revision (minor core revision)
} cpu_id_t;

void cpu_info(soc_t soc);
extern cpu_id_t *cpu;

