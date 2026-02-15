#include "include/cpu.h"
#include "../../../general/include/stdlib.h"

cpu_data_t cpu;
partnum_t partnum;
implementer_t implementer;

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

void cpu_init(void) {
    cpu.part = &partnum;
    cpu.implementer = &implementer;
}


void cpu_info(void) {
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
    cpu.part = &unknown_cpu;
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
    cpu.implementer = &unknown_implementer;
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

void cpu_dsb(void)
{
#if defined(__aarch64__)
    /* ARMv8-A AArch64 */
    __asm__ volatile ("dsb ishst" ::: "memory");

#elif defined(__ARM_ARCH_8A__)
    /* ARMv8-A AArch32 (Cortex-A53 in 32-bit mode) */
    __asm__ volatile ("dsb ishst" ::: "memory");

#elif defined(__ARM_ARCH_7A__) || defined(__ARM_ARCH_7R__)
    /* ARMv7-A / ARMv7-R */
    __asm__ volatile ("dsb ishst" ::: "memory");

#elif defined(__ARM_ARCH_7M__) || defined(__ARM_ARCH_7EM__)
    /* Cortex-M (geen shareability domains) */
    __asm__ volatile ("dsb" ::: "memory");

#elif defined(__ARM_ARCH_6__) || defined(__ARM_ARCH_6J__) || \
      defined(__ARM_ARCH_6K__) || defined(__ARM_ARCH_6Z__) || \
      defined(__ARM_ARCH_6ZK__) || defined(__ARM_ARCH_6KZ__)
    /* ARMv6 barrier via CP15 */
    __asm__ volatile ( "mcr p15, 0, r0, c7, c10, 4\n" ::: "memory");
#else
#error "cpu_dsb: unsupported architecture"
#endif
}

void cpu_isb(void)
{
#if defined(__aarch64__)
    /* ARMv8-A AArch64 */
    __asm__ volatile ("isb" ::: "memory");

#elif defined(__ARM_ARCH_8A__)
    /* ARMv8-A AArch32 (Cortex-A53 in 32-bit mode) */
    __asm__ volatile ("isb" ::: "memory");

#elif defined(__ARM_ARCH_7A__) || defined(__ARM_ARCH_7R__)
    /* ARMv7-A / ARMv7-R */
    __asm__ volatile ("isb" ::: "memory");

#elif defined(__ARM_ARCH_7M__) || defined(__ARM_ARCH_7EM__)
    /* Cortex-M */
    __asm__ volatile ("isb" ::: "memory");

#elif defined(__ARM_ARCH_6__) || defined(__ARM_ARCH_6J__) || \
      defined(__ARM_ARCH_6K__) || defined(__ARM_ARCH_6Z__) || \
      defined(__ARM_ARCH_6ZK__) || defined(__ARM_ARCH_6KZ__)
    /* ARMv6 ISB via CP15 */
    __asm__ volatile ( "mcr p15, 0, r0, c7, c5, 4\n" ::: "memory");

#else
#   error "cpu_isb: unsupported architecture"
#endif
}

void cpu_sev(void)
{
    __asm__ volatile ("sev");
}

void cpu_wfe(void)
{
    __asm__ volatile ("wfe");
}
