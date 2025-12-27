#include "include/BCM2836.h"

volatile interrupts_arm_local_regs_t *INT_ARM_LOCAL;

void BCM2836_Init(uintptr_t mmio_base) {
    INT_ARM_LOCAL = (volatile interrupts_arm_local_regs_t *)(0x40000000);
}