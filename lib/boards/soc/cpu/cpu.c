#include "include/cpu.h"
#include "../../../general/include/stdlib.h"

partnum_t partnum = {
    .partnum = 0x000,
    .name    = "Unknown CPU",
    .soc     = UNKNOWN,
    .mmio_base = 0
};

implementer_t implementer = {
    .implementer = 0xFF,
    .name = "Unknown implementer"
};

cpu_data_t cpu = {
    .implementer = &implementer,
    .rNpM = {0},
    .architecture = NULL,
    .part = &partnum
};

implementer_t implementers[] = {
    {0x00, "Reserved for software use"},
    {0x41, "ARM Ltd."},
    {0x42, "Broadcom"},
    {0x43, "Cavium"},
    {0x44, "Digital Equipment Corporation"},
    {0x46, "Fujitsu Ltd."},
    {0x49, "Infineon"},
    {0x4D, "Motorola or Freescale Semiconductor Inc."},
    {0x4E, "NVIDIA Corporation"},
    {0x50, "Applied Micro Circuits Corporation"},
    {0x51, "Qualcomm Inc."},
    {0x56, "Marvell International Ltd."},
    {0x69, "Intel Corporation"},
    {0xC0, "Ampere Computing"}
};

partnum_t partnumbers[] = {
/* ---------------- Legacy & Pi 1 ------------------ */
    {0xB76, "ARM1176JZF-S", BCM2835,    0x20000000}, // Pi 1, Zero, Zero W
    
/* ---------------- Cortex-C(MIDR) ------------------ */
    {0xD8A, "C1-Nano",      UNKNOWN,    0},
    {0xD8B, "C1-Pro",       UNKNOWN,    0},
    {0xD8C, "C1-Ultra",     UNKNOWN,    0},
    {0xD90, "C1-Premium",   UNKNOWN,    0},         

/* ---------------- Cortex-M (CPUID) ---------------- */
    {0xC20, "Cortex-M0",    UNKNOWN,    0},
    {0xC60, "Cortex-M0+",   UNKNOWN,    0},
    {0xC23, "Cortex-M3",    UNKNOWN,    0},
    {0xC24, "Cortex-M4",    UNKNOWN,    0},
    {0xC27, "Cortex-M7",    UNKNOWN,    0},
    {0xD20, "Cortex-M23",   UNKNOWN,    0},
    {0xD21, "Cortex-M33",   UNKNOWN,    0},
    {0xD31, "Cortex-M35P",  UNKNOWN,    0},
    {0xD52, "Cortex-M52",   UNKNOWN,    0},
    {0xD55, "Cortex-M55",   UNKNOWN,    0},
    {0xD85, "Cortex-M85",   UNKNOWN,    0},      // ⚠ shares ID with X925

/* ---------------- Cortex-R (CP15) ---------------- */
    {0xC14, "Cortex-R4",    UNKNOWN,    0},
    {0xC15, "Cortex-R5",    UNKNOWN,    0},
    {0xC17, "Cortex-R7",    UNKNOWN,    0},
    {0xC18, "Cortex-R8",    UNKNOWN,    0},
    {0xD13, "Cortex-R52+",  UNKNOWN,    0},
    {0xD14, "Cortex-R82AE", UNKNOWN,    0},
    {0xD15, "Cortex-R82",   UNKNOWN,    0},

/* ---------------- Cortex-A (MIDR_EL1) ---------------- */
    {0xC07, "Cortex-A7",    BCM2836,    0x3F000000}, // Pi 2 V1.1
    {0xC09, "Cortex-A9",    UNKNOWN,    0},
    {0xD01, "Cortex-A32",   UNKNOWN,    0},
    {0xD02, "Cortex-A34",   UNKNOWN,    0},
    {0xD03, "Cortex-A53",   BCM2837,    0x3F000000}, // Pi 3, Zero 2 W (RP3A0), Pi 2 V1.2
    {0xD04, "Cortex-A35",   UNKNOWN,    0},
    {0xD05, "Cortex-A55",   UNKNOWN,    0},
    {0xD06, "Cortex-A65",   UNKNOWN,    0},
    {0xD43, "Cortex-A65AE", UNKNOWN,    0},
    {0xD07, "Cortex-A57",   UNKNOWN,    0},
    {0xD08, "Cortex-A72",   BCM2711,    0xFE000000}, // Pi 4, CM4, Pi 400
    {0xD09, "Cortex-A73",   UNKNOWN,    0},
    {0xD0A, "Cortex-A75",   UNKNOWN,    0},
    {0xD0B, "Cortex-A76",   BCM2712,    0x1F000000}, // Pi 5 (Base offset in 32-bit mode)
    {0xD0E, "Cortex-A76AE", UNKNOWN,    0},
    {0xD0D, "Cortex-A77",   UNKNOWN,    0},
    {0xD41, "Cortex-A78",   UNKNOWN,    0},
    {0xD42, "Cortex-A78AE", UNKNOWN,    0},
    {0xD4B, "Cortex-A78C",  UNKNOWN,    0},
    {0xD47, "Cortex-A710",  UNKNOWN,    0},      
    {0xD4D, "Cortex-A715",  UNKNOWN,    0},
    {0xD46, "Cortex-A510",  UNKNOWN,    0},
    {0xD80, "Cortex-A520",  UNKNOWN,    0},
    {0xD88, "Cortex-A520AE",UNKNOWN,    0},
    {0xD81, "Cortex-A720",  UNKNOWN,    0},
    {0xD89, "Cortex-A720AE",UNKNOWN,    0},
    {0xD87, "Cortex-A725",  UNKNOWN,    0},
    {0xD8F, "Cortex-A320",  UNKNOWN,    0},

/* ---------------- Cortex-X ---------------- */
    {0xD44, "Cortex-X1",    UNKNOWN,    0},
    {0xD4C, "Cortex-X1C",   UNKNOWN,    0},
    {0xD48, "Cortex-X2",    UNKNOWN,    0},
    {0xD82, "Cortex-X4",    UNKNOWN,    0},
    {0xD85, "Cortex-X925",  UNKNOWN,    0},         // ⚠ shares ID with M85

/* ---------------- Neoverse ---------------- */
    {0xD0C, "Neoverse-N1",  UNKNOWN,    0},
    {0xD49, "Neoverse-N2",  UNKNOWN,    0},
    {0xD40, "Neoverse-V1",  UNKNOWN,    0},
    {0xD4A, "Neoverse-E1",  UNKNOWN,    0},
    {0xD83, "Neoverse-V3AE",UNKNOWN,    0},
    {0xD84, "Neoverse-V3",  UNKNOWN,    0},
    {0xD8E, "Neoverse-N3",  UNKNOWN,    0},
};

void cpu_init(void) {
    uint32_t reg;

#if defined(__ARM_ARCH_8M__) || defined(__ARM_ARCH_7M__)
    // Cortex-M
    reg = CPUID_REG;

#elif defined(__aarch64__)
    // Cortex-A 64-bit
    asm volatile ("mrs %0, midr_el1" : "=r"(reg));
#else
    // Cortex-A/R 32-bit
    asm volatile ("mrc p15, 0, %0, c0, c0, 0" : "=r"(reg));
#endif
/*
[31:24]	Implementor (0x41 = ARM)
[23:20]	Variant (rev van core)
[19:16]	Architecture (0xF = Cortex-M standard)
[15:4]	PartNo (core type, bv 0xC20=M0, 0xC23=M3, 0xC24=M4, …)
[3:0]	Revision (minor core revision)
*/
    uint16_t nr = (reg >> 4) & 0xFFF;
    for (uint8_t i = 0; i < ARRAY_SIZE(partnumbers); i++) {
        if (partnumbers[i].partnum == nr) {
           cpu.part = &partnumbers[i];
           break;
        }
    }
    cpu.architecture = &architectures[(reg >> 16) & 0xF];
    uint8_t var_val = (reg >> 20) & 0xF; // Extract bits [23:20]
    uint8_t rev_val = (reg >> 0)  & 0xF; // Extract bits [3:0]
    cpu.rNpM[0] = 'r';
    cpu.rNpM[1] = (var_val < 10) ? (var_val + '0') : (var_val - 10 + 'A');
    cpu.rNpM[2] = 'p';
    cpu.rNpM[3] = (rev_val < 10) ? (rev_val + '0') : (rev_val - 10 + 'A');
    cpu.rNpM[4] = 0;
    nr = (reg >> 24) & 0xFF;
    for (uint8_t i = 0; i < ARRAY_SIZE(implementers); i++) {
        if (implementers[i].implementer == nr) {
           cpu.implementer = &implementers[i];
           break;
        }
    }
}

void clearBss(void) {
    extern uint8_t __bss_start[];
    extern uint8_t __bss_end[];

    uint8_t *p = __bss_start;
    uint8_t *end = __bss_end;

    while (p < end) {
        *p++ = 0;
    }
}
