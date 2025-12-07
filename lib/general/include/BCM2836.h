#pragma once
#include <stdint.h>

typedef struct {
    volatile uint32_t ARM_CONTROL_REG;          // 0x00
    volatile uint32_t reserved1[1];
    volatile uint32_t PRESCALER_REG;            // 0x08 Core timer prescaler 
    volatile uint32_t GPU_INT_ROUTING_REG;      // 0x0C GPU interrupts routing
    volatile uint32_t PM_ROUTING_SET;           // 0x10 Performance Monitor Interrupts routing-set
    volatile uint32_t PM_ROUTING_CLR;           // 0x14 Performance Monitor Interrupts routing-clear
    volatile uint32_t reserved2[1];
    volatile uint32_t TIMER_LS;                 // 0x1C Core timer access LS 32 bits
    volatile uint32_t TIMER_MS;                 // 0x20 Core timer access MS 32 bits
    volatile uint32_t INT_ROUTING_REG;          // 0x24 Local Interrupt 0 [1-7] routing 
    volatile uint32_t reserved3[1];
    volatile uint32_t AXI_COUNT_REG;            // 0x2C Axi outstanding counters
    volatile uint32_t AXI_IRQ_REG;              // 0x30 Axi outstanding IRQ
    volatile uint32_t TIMER_CNTRL_REG;          // 0x34 Local timer control & status
    volatile uint32_t TIMER_WRITE_REG;          // 0x38 Local timer write flags
    volatile uint32_t reserved4[1];
// Timers interrupt control registers
    volatile uint32_t TIMER_CNTRL0_REG;         // 0x40 Core0 timers Interrupt control
    volatile uint32_t TIMER_CNTRL1_REG;         // 0x44 Core1 timers Interrupt control
    volatile uint32_t TIMER_CNTRL2_REG;         // 0x48 Core2 timers Interrupt control
    volatile uint32_t TIMER_CNTRL3_REG;         // 0x4C Core3 timers Interrupt control   
// Mailbox interrupt control registers
    volatile uint32_t MAILBOX_CNTRL0_REG;       // 0x50 Core0 Mailboxes Interrupt control
    volatile uint32_t MAILBOX_CNTRL1_REG;       // 0x54 Core1 Mailboxes Interrupt control
    volatile uint32_t MAILBOX_CNTRL2_REG;       // 0x58 Core2 Mailboxes Interrupt control
    volatile uint32_t MAILBOX_CNTRL3_REG;       // 0x5C Core3 Mailboxes Interrupt control
// IRQ & FIQ source registers
    volatile uint32_t IRQ_SOURCE0_REG;         // 0x60 Core0 IRQ Source
    volatile uint32_t IRQ_SOURCE1_REG;         // 0x64 Core1 IRQ Source
    volatile uint32_t IRQ_SOURCE2_REG;         // 0x68 Core2 IRQ Source
    volatile uint32_t IRQ_SOURCE3_REG;         // 0x6C Core3 IRQ Source
    volatile uint32_t FIQ_SOURCE0_REG;         // 0x70 Core0 FIQ Source
    volatile uint32_t FIQ_SOURCE1_REG;         // 0x74 Core1 FIQ Source
    volatile uint32_t FIQ_SOURCE2_REG;         // 0x78 Core2 FIQ Source
    volatile uint32_t FIQ_SOURCE3_REG;         // 0x7C Core3 FIQ Source
// Mailbox write-set registers (Write only)
// A write-set register allows you to set additional bits high. Bits which were already high are not affected.
// You set a bit high by writing the value '1' to it. Everywhere you write a '0' bit the original register contents
// remains unchanged.
    volatile uint32_t MBOX_SET00_REG;           // 0x80 Mailbox 0 Core0 Set Register
    volatile uint32_t MBOX_SET01_REG;           // 0x84 Mailbox 1 Core0 Set Register
    volatile uint32_t MBOX_SET02_REG;           // 0x88 Mailbox 2 Core0 Set Register
    volatile uint32_t MBOX_SET03_REG;           // 0x8C Mailbox 3 Core0 Set Register
    volatile uint32_t MBOX_SET04_REG;           // 0x90 Mailbox 0 Core1 Set Register
    volatile uint32_t MBOX_SET05_REG;           // 0x94 Mailbox 1 Core1 Set Register
    volatile uint32_t MBOX_SET06_REG;           // 0x98 Mailbox 2 Core1 Set Register
    volatile uint32_t MBOX_SET07_REG;           // 0x9C Mailbox 3 Core1 Set Register
    volatile uint32_t MBOX_SET08_REG;           // 0xA0 Mailbox 0 Core2 Set Register
    volatile uint32_t MBOX_SET09_REG;           // 0xA4 Mailbox 1 Core2 Set Register
    volatile uint32_t MBOX_SET10_REG;           // 0xA8 Mailbox 2 Core2 Set Register
    volatile uint32_t MBOX_SET11_REG;           // 0xAC Mailbox 3 Core2 Set Register
    volatile uint32_t MBOX_SET12_REG;           // 0xB0 Mailbox 0 Core3 Set Register
    volatile uint32_t MBOX_SET13_REG;           // 0xB4 Mailbox 1 Core3 Set Register
    volatile uint32_t MBOX_SET14_REG;           // 0xB8 Mailbox 2 Core3 Set Register
    volatile uint32_t MBOX_SET15_REG;           // 0xBC Mailbox 3 Core3 Set Register
// Mailbox write-clear registers (Read & Write)
// A write-clear register allows you to set additional bits low. Bits which were already low are not affected.
// You set a bit low by writing the value '1' to it. Everywhere you write a '0' bit the original register contents
// remains unchanged. Beware that you write a one to get a zero!
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

#define LOCAL_TIMER_IRQ         (1 << 29)
#define TIMER0_IRQ              (1 << 0)
#define TIMER1_IRQ              (1 << 1)
#define TIMER2_IRQ              (1 << 2)
#define TIMER3_IRQ              (1 << 3)
#define TIMER0_FIQ              (1 << 4)
#define TIMER1_FIQ              (1 << 5)
#define TIMER2_FIQ              (1 << 6)
#define TIMER3_FIQ              (1 << 7)

#define MBOX0_IRQ               (1 << 0)
#define MBOX1_IRQ               (1 << 1)
#define MBOX2_IRQ               (1 << 2)
#define MBOX3_IRQ               (1 << 3)
#define MBOX0_FIQ               (1 << 4)
#define MBOX1_FIQ               (1 << 5)
#define MBOX2_FIQ               (1 << 6)
#define MBOX3_FIQ               (1 << 7)
//  Core interrupt sources
#define INT_SRC_TIMER0          (1 << 0)    // CNTPS Secure physical timer
#define INT_SRC_TIMER1          (1 << 1)    // CNTPS Non-secure physical timer
#define INT_SRC_TIMER2          (1 << 2)    // CNTHP Hypervisor timer
#define INT_SRC_TIMER3          (1 << 3)    // CNTV Virtual timer
#define INT_SRC_MBOX0           (1 << 4)
#define INT_SRC_MBOX1           (1 << 5)
#define INT_SRC_MBOX2           (1 << 6)
#define INT_SRC_MBOX3           (1 << 7)
#define INT_SRC_GPU             (1 << 8)
#define INT_SRC_PMU             (1 << 9)
#define AXI                     (1 << 10)
#define INT_SRC_LOCAL_TIMER     (1 << 11) // Local timer interrupt

void BCM2836_Init(uintptr_t mmio_base);
// Initializes the BCM2836 peripherals base address pointers
