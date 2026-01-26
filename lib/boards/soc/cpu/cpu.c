#include <stdint.h>
#include "include/cpu.h"

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