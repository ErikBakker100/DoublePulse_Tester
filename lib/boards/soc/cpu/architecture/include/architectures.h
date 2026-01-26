#pragma once
#include <stdint.h>
#include <stddef.h>
#include "../registers/include/registers.h"

typedef enum {
    ARCH_M,     // Cortex-M
    ARCH_A32,   // Cortex-A AArch32
    ARCH_A64,   // Cortex-A AArch64
    ARCH_R      // Cortex-R
} arch_t;

typedef struct {
    uint8_t nr;                 // Architecture nr
    const char* name;           // Name
    id_source_t reg;            // Detection register
    arch_t arch;                // Architecture profile (M / A32 / A64 / R)
} architecture_t;

extern architecture_t architectures[];