#include "include/arm_partnum.h"
#include "../../../general/include/stdlib.h"

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
/* ---------------- Cortex-C(MIDR) ------------------ */
    {0xD8A, "C1-Nano"},
    {0xD8B, "C1-Pro"},
    {0xD8C, "C1-Ultra"},
    {0xD90, "C1-Premium"},         

/* ---------------- Cortex-M (CPUID) ---------------- */
    {0xC20, "Cortex-M0"},
    {0xC60, "Cortex-M0+"},
    {0xC23, "Cortex-M3"},
    {0xC24, "Cortex-M4"},
    {0xC27, "Cortex-M7"},
    {0xD20, "Cortex-M23"},
    {0xD21, "Cortex-M33"},
    {0xD31, "Cortex-M35P"},
    {0xD52, "Cortex-M52"},
    {0xD55, "Cortex-M55"},
    {0xD85, "Cortex-M85"},      // ⚠ shares ID with X925

/* ---------------- Cortex-R (CP15) ---------------- */
    {0xC14, "Cortex-R4"},
    {0xC15, "Cortex-R5"},
    {0xC17, "Cortex-R7"},
    {0xC18, "Cortex-R8"},
    {0xD13, "Cortex-R52+"},
    {0xD14, "Cortex-R82AE"},
    {0xD15, "Cortex-R82"},

/* ---------------- Cortex-A (MIDR_EL1) ---------------- */
    {0xC09, "Cortex-A9"},
    {0xD01, "Cortex-A32"},
    {0xD02, "Cortex-A34"},
    {0xD03, "Cortex-A53"},
    {0xD04, "Cortex-A35"},
    {0xD05, "Cortex-A55"},
    {0xD06, "Cortex-A65"},
    {0xD43, "Cortex-A65AE"},
    {0xD07, "Cortex-A57"},
    {0xD08, "Cortex-A72"},
    {0xD09, "Cortex-A73"},
    {0xD0A, "Cortex-A75"},
    {0xD0B, "Cortex-A76"},
    {0xD0E, "Cortex-A76AE"},
    {0xD0D, "Cortex-A77"},
    {0xD41, "Cortex-A78"},
    {0xD42, "Cortex-A78AE"},
    {0xD4B, "Cortex-A78C",},
    {0xD47, "Cortex-A710",},      
    {0xD4D, "Cortex-A715",},
    {0xD46, "Cortex-A510",},
    {0xD80, "Cortex-A520",},
    {0xD88, "Cortex-A520AE"},
    {0xD81, "Cortex-A720",},
    {0xD89, "Cortex-A720AE"},
    {0xD87, "Cortex-A725",},
    {0xD8F, "Cortex-A320",},

/* ---------------- Cortex-X ---------------- */
    {0xD44, "Cortex-X1",  },
    {0xD4C, "Cortex-X1C"},
    {0xD48, "Cortex-X2",  },
    {0xD82, "Cortex-X4",  },
    {0xD85, "Cortex-X925",},         // ⚠ shares ID with M85

/* ---------------- Neoverse ---------------- */
    {0xD0C, "Neoverse-N1",},
    {0xD49, "Neoverse-N2",},
    {0xD40, "Neoverse-V1",},
    {0xD4A, "Neoverse-E1",},
    {0xD83, "Neoverse-V3AE"},
    {0xD84, "Neoverse-V3",},
    {0xD8E, "Neoverse-N3",}
};


void cpu_info(soc_t soc) {
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
    cpu->revision = reg & 0xF;
    uint16_t nr = (reg >> 4) & 0xFFF;
    cpu->partno = &unknown_cpu;
    for (uint8_t i = 0; i < ARRAY_SIZE(partnumbers); i++) {
        if (partnumbers[i].partnum == nr)
           cpu->partno = &partnumbers[i];
    }
    cpu->architecture = &architectures[(reg >> 16) & 0xF];
    cpu->variant = (reg >> 20) & 0xF;
    nr = (reg >> 24) & 0xFF;
    cpu->implementer = &unknown_implementer;
    for (uint8_t i = 0; i < ARRAY_SIZE(implementers); i++) {
        if (implementers[i].implementer == nr)
           cpu->implementer = &implementers[i];
    }
}

cpu_id_t *cpu;