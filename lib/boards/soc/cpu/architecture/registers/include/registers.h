#pragma once

typedef enum {
    CPUID,   // Cortex-M
    MIDR,    // Cortex-A (EL1)
    CP15     // Cortex-R / oudere A (AArch32)
} id_source_t;

