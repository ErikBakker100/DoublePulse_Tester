#pragma once
#include <stdint.h>
#include "../../boards/soc/BCM2835/include/BCM2835.h"

// BASE_ADDR depends on architecture, set accordingly. Used for core_boot address table.
// Core	32-bit RDCLR (AArch32)	64-bit RDCLR (AArch64)	Verschil
//  0	0x4000008C 	            0xD8	starts automatically
//  1	0x4000009C +0x10	    0xE0	+0x08 +64 bits
//  2	0x400000AC +0x10	    0xE8	+0x08 +64 bits
//  3	0x400000BC +0x10	    0xF0	+0x08 +64 bits
// The auxiliary cores are initially asleep.
// They can be woken up (independently) by following the following steps.
// 1) Writing an address to jump to at:
//    - 0xe0 for core 2,
//    - 0xe8 for core 3,
//    - 0xf0 for core 4.
// 2) Issuing an "sev" instruction to force the core out of low-power mode.

#if defined(__aarch64__) || defined(__AARCH64__)
typedef volatile uint64_t core_reg_t;
#define BASE_ADDR   0xD8
#define CORE_STRIDE 0x08u          // 8 bytes per core
#else
typedef volatile uint32_t core_reg_t;
#define BASE_ADDR   0x4000008CUL
#define CORE_STRIDE 0x10u           // 16 bytes per core
#endif

// Pointer to core_boot register array to write the boot address for each core
volatile core_reg_t *core_boot(int core_id);
