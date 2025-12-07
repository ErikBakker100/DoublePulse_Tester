#pragma once
#include <stdint.h>

#define LOCAL_PERIPHERAL_BASE   0x4C000000UL

typedef struct {
    volatile uint32_t ARM_CONTROL_REG;          // 0x00
    uint32_t reserved1[1];
    volatile uint32_t CORE_IRQ_CONTROL_REG;     // 0x0C
    volatile uint32_t PMU_CONTROL_SET_REG;      // 0x10
    volatile uint32_t PMU_CONTROL_CLR_REG;      // 0x14
    uint32_t reserved2[4];
    volatile uint32_t PERI_IRQ_ROUTE0_REG;      // 0x24
    uint32_t reserved3[2];
    volatile uint32_t AXI_QUIET_TIME_REG;       // 0x30
    volatile uint32_t LOCAL_TIMER_CONTROL_REG;  // 0x34
    volatile uint32_t LOCAL_TIMER_IRQ_REG;      // 0x38
    uint32_t reserved4[1];
    volatile uint32_t TIMER_CNTRL0_REG;         // 0x40
    volatile uint32_t TIMER_CNTRL1_REG;         // 0x44
    volatile uint32_t TIMER_CNTRL2_REG;         // 0x48
    volatile uint32_t TIMER_CNTRL3_REG;         // 0x4C
    volatile uint32_t MAILBOX_CNTRL0_REG;       // 0x50
    volatile uint32_t MAILBOX_CNTRL1_REG;       // 0x54
    volatile uint32_t MAILBOX_CNTRL2_REG;       // 0x58
    volatile uint32_t MAILBOX_CNTRL3_REG;       // 0x5C
    volatile uint32_t IRQ_SOURCE0_REG;          // 0x60
    volatile uint32_t IRQ_SOURCE1_REG;          // 0x64
    volatile uint32_t IRQ_SOURCE2_REG;          // 0x68
    volatile uint32_t IRQ_SOURCE3_REG;          // 0x6C
    volatile uint32_t FIQ_SOURCE0_REG;          // 0x70
    volatile uint32_t FIQ_SOURCE1_REG;          // 0x74
    volatile uint32_t FIQ_SOURCE2_REG;          // 0x78
    volatile uint32_t FIQ_SOURCE3_REG;          // 0x7C
    volatile uint32_t MBOX_SET00_REG;           // 0x80 Mailbox 00 Set Bit Register
    volatile uint32_t MBOX_SET01_REG;           // 0x84 Mailbox 01 Set Bit Register
    volatile uint32_t MBOX_SET02_REG;           // 0x88 Mailbox 02 Set Bit Register
    volatile uint32_t MBOX_SET03_REG;           // 0x8C Mailbox 03 Set Bit Register
    volatile uint32_t MBOX_SET04_REG;           // 0x90 Mailbox 04 Set Bit Register
    volatile uint32_t MBOX_SET05_REG;           // 0x94 Mailbox 05 Set Bit Register
    volatile uint32_t MBOX_SET06_REG;           // 0x98 Mailbox 06 Set Bit Register
    volatile uint32_t MBOX_SET07_REG;           // 0x9C Mailbox 07 Set Bit Register
    volatile uint32_t MBOX_SET08_REG;           // 0xA0 Mailbox 08 Set Bit Register
    volatile uint32_t MBOX_SET09_REG;           // 0xA4 Mailbox 09 Set Bit Register
    volatile uint32_t MBOX_SET10_REG;           // 0xA8 Mailbox 10 Set Bit Register
    volatile uint32_t MBOX_SET11_REG;           // 0xAC Mailbox 11 Set Bit Register
    volatile uint32_t MBOX_SET12_REG;           // 0xB0 Mailbox 12 Set Bit Register
    volatile uint32_t MBOX_SET13_REG;           // 0xB4 Mailbox 13 Set Bit Register
    volatile uint32_t MBOX_SET14_REG;           // 0xB8 Mailbox 14 Set Bit Register
    volatile uint32_t MBOX_SET15_REG;           // 0xBC Mailbox 15 Set Bit Register
    volatile uint32_t MBOX_CLR00_REG;           // 0xC0 Mailbox 00 Read & Clear Bit Register
    volatile uint32_t MBOX_CLR01_REG;           // 0xC4 Mailbox 01 Read & Clear Bit Register
    volatile uint32_t MBOX_CLR02_REG;           // 0xC8 Mailbox 02 Read & Clear Bit Register
    volatile uint32_t MBOX_CLR03_REG;           // 0xCC Mailbox 03 Read & Clear Bit Register
    volatile uint32_t MBOX_CLR04_REG;           // 0xD0 Mailbox 04 Read & Clear Bit Register
    volatile uint32_t MBOX_CLR05_REG;           // 0xD4 Mailbox 05 Read & Clear Bit Register
    volatile uint32_t MBOX_CLR06_REG;           // 0xD8 Mailbox 06 Read & Clear Bit Register
    volatile uint32_t MBOX_CLR07_REG;           // 0xDC Mailbox 07 Read & Clear Bit Register
    volatile uint32_t MBOX_CLR08_REG;           // 0xE0 Mailbox 08 Read & Clear Bit Register
    volatile uint32_t MBOX_CLR09_REG;           // 0xE4 Mailbox 09 Read & Clear Bit Register
    volatile uint32_t MBOX_CLR10_REG;           // 0xE8 Mailbox 10 Read & Clear Bit Register
    volatile uint32_t MBOX_CLR11_REG;           // 0xEC Mailbox 11 Read & Clear Bit Register
    volatile uint32_t MBOX_CLR12_REG;           // 0xF0 Mailbox 12 Read & Clear Bit Register
    volatile uint32_t MBOX_CLR13_REG;           // 0xF4 Mailbox 13 Read & Clear Bit Register
    volatile uint32_t MBOX_CLR14_REG;           // 0xF8 Mailbox 14 Read & Clear Bit Register
    volatile uint32_t MBOX_CLR15_REG;           // 0xFC Mailbox 15 Read & Clear Bit Register
} interrupts_arm_local_regs_t;

extern volatile interrupts_arm_local_regs_t *INT_ARM_LOCAL_REGS;

// Mailbox write-set registers (Write only)
// A write-set register allows you to set additional bits high. Bits which were already high are not affected.
// You set a bit high by writing the value '1' to it. Everywhere you write a '0' bit the original register contents
// remains unchanged.
typedef struct {
    volatile uint32_t IRQ0_PENDING0_REG;        // 0x200 ARM Core 0 IRQ Enabled Interrupt Pending bits [31:0]
    volatile uint32_t IRQ0_PENDING1_REG;        // 0x204 ARM Core 0 IRQ Enabled Interrupt pending bits [63:32]
    volatile uint32_t IRQ0_PENDING2_REG;        // 0x208 ARM Core 0 IRQ Enabled Interrupt pending bits [79:64]
    uint32_t reserved1[1];
    volatile uint32_t IRQ0_SET_EN_0_REG;        // 0x210 Write to Set ARM Core 0 IRQ enable bits [31:0]
    volatile uint32_t IRQ0_SET_EN_1_REG;        // 0x214 Write to Set ARM Core 0 IRQ enable bits [63:32]
    volatile uint32_t IRQ0_SET_EN_2_REG;        // 0x218 Write to Set ARM Core 0 IRQ enable bits[79:64]
    uint32_t reserved2[1];
    volatile uint32_t IRQ0_CLR_EN_0_REG;        // 0x220 Write to Clear ARM Core 0 IRQ enable bits [31:0]
    volatile uint32_t IRQ0_CLR_EN_1_REG;        // 0x224 Write to Clear ARM Core 0 IRQ enable bits [63:32]
    volatile uint32_t IRQ0_CLR_EN_2_REG;        // 0x228 Write to Clear ARM Core 0 IRQ enable bits [79:64]
    uint32_t reserved3[1];
    volatile uint32_t IRQ_STATUS0_REG;          // 0x230 IRQ_STATUS0 // Interrupt Line bits [31:0]
    volatile uint32_t IRQ_STATUS1_REG;          // 0x234 IRQ_STATUS1 // Interrupt Line bits [63:32]
    volatile uint32_t IRQ_STATUS2_REG;          // 0x238 IRQ_STATUS2 // Interrupt Line bits [79:64]
    uint32_t reserved4[1];
    volatile uint32_t IRQ1_PENDING0_REG;        // 0x240 ARM Core 1 IRQ Enabled Interrupt pending bits [31:0]
    volatile uint32_t IRQ1_PENDING1_REG;        // 0x244 ARM Core 1 IRQ Enabled Interrupt pending bits [63:32]
    volatile uint32_t IRQ1_PENDING2_REG;        // 0x248 ARM Core 1 IRQ Enabled Interrupt pending bits [79:64]
    uint32_t reserved5[1];
    volatile uint32_t IRQ1_SET_EN_0_REG;        // 0x250 Write to Set ARM Core 1 IRQ enable bits [31:0]
    volatile uint32_t IRQ1_SET_EN_1_REG;        // 0x254 Write to Set ARM Core 1 IRQ enable bits [63:32]
    volatile uint32_t IRQ1_SET_EN_2_REG;        // 0x258 Write to Set ARM Core 1 IRQ enable bits[79:64]
    uint32_t reserved6[1];
    volatile uint32_t IRQ1_CLR_EN_0_REG;        // 0x260 Write to Clear ARM Core 1 IRQ enable bits [31:0]
    volatile uint32_t IRQ1_CLR_EN_1_REG;        // 0x264 Write to Clear ARM Core 1 IRQ enable bits [63:32]
    volatile uint32_t IRQ1_CLR_EN_2_REG;        // 0x268 Write to Clear ARM Core 1 IRQ enable bits [79:64]
    uint32_t reserved7[5];
    volatile uint32_t IRQ2_PENDING0_REG;        // 0x280 ARM Core 2 IRQ Enabled Interrupt pending bits [31:0]
    volatile uint32_t IRQ2_PENDING1_REG;        // 0x284 ARM Core 2 IRQ Enabled Interrupt pending bits [63:32]
    volatile uint32_t IRQ2_PENDING2_REG;        // 0x288 ARM Core 2 IRQ Enabled Interrupt pending bits [79:64]
    uint32_t reserved8[1]; 
    volatile uint32_t IRQ2_SET_EN_0_REG;        // 0x290 Write to Set ARM Core 2 IRQ enable bits [31:0]
    volatile uint32_t IRQ2_SET_EN_1_REG;        // 0x294 Write to Set ARM Core 2 IRQ enable bits [63:32]
    volatile uint32_t IRQ2_SET_EN_2_REG;        // 0x298 Write to Set ARM Core 2 IRQ enable bits[79:64]
    uint32_t reserved9[1];
    volatile uint32_t IRQ2_CLR_EN_0_REG;        // 0x2A0 Write to Clear ARM Core 2 IRQ enable bits [31:0]
    volatile uint32_t IRQ2_CLR_EN_1_REG;        // 0x2A4 Write to Clear ARM Core 2 IRQ enable bits [63:32]
    volatile uint32_t IRQ2_CLR_EN_2_REG;        // 0x2A8 Write to Clear ARM Core 2 IRQ enable bits [79:64]
    uint32_t reserved10[5];
    volatile uint32_t IRQ3_PENDING0_REG;        // 0x2C0 ARM Core 3 IRQ Enabled Interrupt pending bits [31:0]
    volatile uint32_t IRQ3_PENDING1_REG;        // 0x2C4 ARM Core 3 IRQ Enabled Interrupt pending bits [63:32]
    volatile uint32_t IRQ3_PENDING2_REG;        // 0x2C8 ARM Core 3 IRQ Enabled Interrupt pending bits [79:64]
    uint32_t reserved11[1];
    volatile uint32_t IRQ3_SET_EN_0_REG;        // 0x2D0 Write to Set ARM Core 3 IRQ enable bits [31:0]
    volatile uint32_t IRQ3_SET_EN_1_REG;        // 0x2D4 Write to Set ARM Core 3 IRQ enable bits [63:32]
    volatile uint32_t IRQ3_SET_EN_2_REG;        // 0x2D8 Write to Set ARM Core 3 IRQ enable bits[79:64]
    uint32_t reserved12[1];
    volatile uint32_t IRQ3_CLR_EN_0_REG;        // 0x2E0 Write to Clear ARM Core 3 IRQ enable bits [31:0]
    volatile uint32_t IRQ3_CLR_EN_1_REG;        // 0x2E4 Write to Clear ARM Core 3 IRQ enable bits [63:32]
    volatile uint32_t IRQ3_CLR_EN_2_REG;        // 0x2E8 Write to Clear ARM Core 3 IRQ enable bits [79:64]
    uint32_t reserved13[5];
    volatile uint32_t FIQ0_PENDING0_REG;        // 0x300 ARM Core 0 FIQ Enabled Interrupt pending bits [31:0]
    volatile uint32_t FIQ0_PENDING1_REG;        // 0x304 ARM Core 0 FIQ Enabled Interrupt pending bits [63:32]
    volatile uint32_t FIQ0_PENDING2_REG;        // 0x308 ARM Core 0 FIQ Enabled Interrupt pending bits [79:64]
    uint32_t reserved14[1];
    volatile uint32_t FIQ0_SET_EN_0_REG;        // 0x310 Write to Set ARM Core 0 FIQ enable bits [31:0]
    volatile uint32_t FIQ0_SET_EN_1_REG;        // 0x314 Write to Set ARM Core 0 FIQ enable bits [63:32]
    volatile uint32_t FIQ0_SET_EN_2_REG;        // 0x318 Write to Set ARM Core 0 FIQ enable bits[79:64]
    uint32_t reserved15[1];
    volatile uint32_t FIQ0_CLR_EN_0_REG;        // 0x320 Write to Clear ARM Core 0 FIQ enable bits [31:0]
    volatile uint32_t FIQ0_CLR_EN_1_REG;        // 0x324 Write to Clear ARM Core 0 FIQ enable bits [63:32]
    volatile uint32_t FIQ0_CLR_EN_2_REG;        // 0x328 Write to Clear ARM Core 0 FIQ enable bits [79:64]
    uint32_t reserved16[5];
    volatile uint32_t FIQ1_PENDING0_REG;        // 0x340 ARM Core 1 FIQ Enabled Interrupt pending bits [31:0]
    volatile uint32_t FIQ1_PENDING1_REG;        // 0x344 ARM Core 1 FIQ Enabled Interrupt pending bits [63:32]
    volatile uint32_t FIQ1_PENDING2_REG;        // 0x348 ARM Core 1 FIQ Enabled Interrupt pending bits [79:64]
    uint32_t reserved17[1];
    volatile uint32_t FIQ1_SET_EN_0_REG;        // 0x350 Write to Set ARM Core 1 FIQ enable bits [31:0]
    volatile uint32_t FIQ1_SET_EN_1_REG;        // 0x354 Write to Set ARM Core 1 FIQ enable bits [63:32]
    volatile uint32_t FIQ1_SET_EN_2_REG;        // 0x358 Write to Set ARM Core 1 FIQ enable bits[79:64]
    uint32_t reserved18[1];
    volatile uint32_t FIQ1_CLR_EN_0_REG;        // 0x360 Write to Clear ARM Core 1 FIQ enable bits [31:0]
    volatile uint32_t FIQ1_CLR_EN_1_REG;        // 0x364 Write to Clear ARM Core 1 FIQ enable bits [63:32]
    volatile uint32_t FIQ1_CLR_EN_2_REG;        // 0x368 Write to Clear ARM Core 1 FIQ enable bits [79:64]
    uint32_t reserved19[5];
    volatile uint32_t FIQ2_PENDING0_REG;        // 0x380 ARM Core 2 FIQ Enabled Interrupt pending bits [31:0]
    volatile uint32_t FIQ2_PENDING1_REG;        // 0x384 ARM Core 2 FIQ Enabled Interrupt pending bits [63:32]
    volatile uint32_t FIQ2_PENDING2_REG;        // 0x388 ARM Core 2 FIQ Enabled Interrupt pending bits [79:64]
    uint32_t reserved20[1];
    volatile uint32_t FIQ2_SET_EN_0_REG;        // 0x390 Write to Set ARM Core 2 FIQ enable bits [31:0]
    volatile uint32_t FIQ2_SET_EN_1_REG;        // 0x394 Write to Set ARM Core 2 FIQ enable bits [63:32]
    volatile uint32_t FIQ2_SET_EN_2_REG;        // 0x398 Write to Set ARM Core 2 FIQ enable bits[79:64]
    uint32_t reserved21[1];
    volatile uint32_t FIQ2_CLR_EN_0_REG;        // 0x3A0 Write to Clear ARM Core 2 FIQ enable bits [31:0]
    volatile uint32_t FIQ2_CLR_EN_1_REG;        // 0x3A4 Write to Clear ARM Core 2 FIQ enable bits [63:32]
    volatile uint32_t FIQ2_CLR_EN_2_REG;        // 0x3A8 Write to Clear ARM Core 2 FIQ enable bits [79:64]
    uint32_t reserved22[5];
    volatile uint32_t FIQ3_PENDING0_REG;        // 0x3C0 ARM Core 3 FIQ Enabled Interrupt pending bits [31:0]
    volatile uint32_t FIQ3_PENDING1_REG;        // 0x3C4 ARM Core 3 FIQ Enabled Interrupt pending bits [63:32]
    volatile uint32_t FIQ3_PENDING2_REG;        // 0x3C8 ARM Core 3 FIQ Enabled Interrupt pending bits [79:64]
    uint32_t reserved23[1];
    volatile uint32_t FIQ3_SET_EN_0_REG;        // 0x3D0 Write to Set ARM Core 3 FIQ enable bits [31:0]
    volatile uint32_t FIQ3_SET_EN_1_REG;        // 0x3D4 Write to Set ARM Core 3 FIQ enable bits [63:32]
    volatile uint32_t FIQ3_SET_EN_2_REG;        // 0x3D8 Write to Set ARM Core 3 FIQ enable bits[79:64]
    uint32_t reserved24[1];
    volatile uint32_t FIQ3_CLR_EN_0_REG;    // 0x3E0 Write to Clear ARM Core 3 FIQ enable bits [31:0]
    volatile uint32_t FIQ3_CLR_EN_1_REG;    // 0x3E4 Write to Clear ARM Core 3 FIQ enable bits [63:32]
    volatile uint32_t FIQ3_CLR_EN_2_REG;    // 0x3E8 Write to Clear ARM Core 3 FIQ enable bits [79:64]
    uint32_t reserved25[1];
    volatile uint32_t SWIRQ_SET_REG;      // 0x3F0 Write to Set Software Interrupt sources
    volatile uint32_t SWIRQ_CLEAR_REG;    // 0x3F4 Write to Clear Software Interrupt sources
} interrupts_armc_regs_t;

extern volatile interrupts_armc_regs_t *INT_ARMC_REGS;
