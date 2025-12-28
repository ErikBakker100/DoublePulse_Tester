#include "include/multi_core.h"

// core_boot geeft pointer naar write bootadresregister van core 'core_id'
volatile core_reg_t *core_boot(int core_id) {
    return (volatile core_reg_t *)((uintptr_t)(BASE_ADDR + (CORE_STRIDE * core_id)));
}

/*
Het VBAR-register vertelt de CPU waar de exception/interrupt vector table staat.
Bij een exception (IRQ, FIQ, SVC, enz.) springt de CPU naar het adres dat in VBAR staat, plus een offset afhankelijk van het exception type.
Daarom moet je voor je kernel of bare-metal firmware VBAR instellen naar jouw eigen vector table voor die core.
*/
#if defined(__aarch64__) || defined(__AARCH64__)
void write_vbar(uint64_t addr) {
    asm volatile (
        "msr vbar_el1, %0" :: "r"(addr));
        asm volatile ("isb");
}
#else
void write_vbar(uint32_t addr) {
    asm volatile (
        "mcr p15, 0, %0, c12, c0, 0\n"  // schrijf VBAR
        "isb\n"                          // instruction sync barrier
        :: "r"(addr)
        : "memory"
    );
}
#endif