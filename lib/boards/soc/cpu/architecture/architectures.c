#include "include/architectures.h"

architecture_t architectures[] = {
    {0x0, "Pre-ARMv4",  CP15, ARCH_A32},    // old ARM
    {0x1, "ARMv4",      CP15, ARCH_A32},
    {0x2, "ARMv4T",     CP15, ARCH_A32},
    {0x3, "ARMv5",      CP15, ARCH_A32},
    {0x4, "ARMv5T",     CP15, ARCH_A32},
    {0x5, "ARMv5TE",    CP15, ARCH_A32},
    {0x6, "ARMv5TEJ",   CP15, ARCH_A32},
    {0x7, "ARMv6",      CP15, ARCH_A32},
    {0x8, "ARMv6KZ",    CP15, ARCH_A32},
    {0x9, "ARMv6T2",    CP15, ARCH_A32},
    {0xA, "ARMv7-A",    MIDR, ARCH_A32},    // AArch32 Cortex-A
    {0xB, "ARMv7E-M",   CPUID, ARCH_M},     // Cortex-M4/M3
    {0xC, "ARMv8-A",    MIDR, ARCH_A64},    // 64-bit EL1
    {0xD, "ARMv8.1-A",  MIDR, ARCH_A64},
    {0xE, "ARMv8.2-A",  MIDR, ARCH_A64},
    {0xF, "ARMv8.3-A",  MIDR, ARCH_A64}
};