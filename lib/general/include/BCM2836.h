#pragma once
#include <stdint.h>

typedef struct {
    volatile uint32_t ARM_CONTROL;          // 0x00
    volatile uint32_t reserved1[1];
    volatile uint32_t PRESCALER;            // 0x08 Core timer prescaler 
    volatile uint32_t GPU_INT_ROUTING;      // 0x0C GPU interrupts routing
    volatile uint32_t PM_ROUTING_SET;       // 0x10 Performance Monitor Interrupts routing-set
    volatile uint32_t PM_ROUTING_CLR;       // 0x14 Performance Monitor Interrupts routing-clear
    volatile uint32_t reserved2[1];
    volatile uint32_t TIMER_LS;             // 0x1C Core timer access LS 32 bits
    volatile uint32_t TIMER_MS;             // 0x20 Core timer access MS 32 bits
    volatile uint32_t INT_ROUTING;          // 0x24 Local Interrupt 0 [1-7] routing 
    volatile uint32_t reserved3[1];
    volatile uint32_t AXI_COUNT;            // 0x2C Axi outstanding counters
    volatile uint32_t AXI_IRQ;              // 0x30 Axi outstanding IRQ
    volatile uint32_t TIMER_CNTRL;          // 0x34 Local timer control & status
    volatile uint32_t TIMER_WRITE;          // 0x38 Local timer write flags
    volatile uint32_t reserved4[1];
// Timers interrupt control registers
    volatile uint32_t TIMER_CNTRL0;         // 0x40 Core0 timers Interrupt control
    volatile uint32_t TIMER_CNTRL1;         // 0x44 Core1 timers Interrupt control
    volatile uint32_t TIMER_CNTRL2;         // 0x48 Core2 timers Interrupt control
    volatile uint32_t TIMER_CNTRL3;         // 0x4C Core3 timers Interrupt control   
// Mailbox interrupt control registers
    volatile uint32_t MAILBOX_CNTRL0;       // 0x50 Core0 Mailboxes Interrupt control
    volatile uint32_t MAILBOX_CNTRL1;       // 0x54 Core1 Mailboxes Interrupt control
    volatile uint32_t MAILBOX_CNTRL2;       // 0x58 Core2 Mailboxes Interrupt control
    volatile uint32_t MAILBOX_CNTRL3;       // 0x5C Core3 Mailboxes Interrupt control
// IRQ & FIQ source registers
    volatile uint32_t IRQ_SOURCE0;          // 0x60 Core0 IRQ Source
    volatile uint32_t IRQ_SOURCE1;          // 0x64 Core1 IRQ Source
    volatile uint32_t IRQ_SOURCE2;          // 0x68 Core2 IRQ Source
    volatile uint32_t IRQ_SOURCE3;          // 0x6C Core3 IRQ Source
    volatile uint32_t FIQ_SOURCE0;          // 0x70 Core0 FIQ Source
    volatile uint32_t FIQ_SOURCE1;          // 0x74 Core1 FIQ Source
    volatile uint32_t FIQ_SOURCE2;          // 0x78 Core2 FIQ Source
    volatile uint32_t FIQ_SOURCE3;          // 0x7C Core3 FIQ Source
// Mailbox write-set registers (Write only)
// A write-set register allows you to set additional bits high. Bits which were already high are not affected.
// You set a bit high by writing the value '1' to it. Everywhere you write a '0' bit the original register contents
// remains unchanged.
    volatile uint32_t MBOX_SET00;           // 0x80 Mailbox 0 Core0 Set Register
    volatile uint32_t MBOX_SET01;           // 0x84 Mailbox 1 Core0 Set Register
    volatile uint32_t MBOX_SET02;           // 0x88 Mailbox 2 Core0 Set Register
    volatile uint32_t MBOX_SET03;           // 0x8C Mailbox 3 Core0 Set Register
    volatile uint32_t MBOX_SET04;           // 0x90 Mailbox 0 Core1 Set Register
    volatile uint32_t MBOX_SET05;           // 0x94 Mailbox 1 Core1 Set Register
    volatile uint32_t MBOX_SET06;           // 0x98 Mailbox 2 Core1 Set Register
    volatile uint32_t MBOX_SET07;           // 0x9C Mailbox 3 Core1 Set Register
    volatile uint32_t MBOX_SET08;           // 0xA0 Mailbox 0 Core2 Set Register
    volatile uint32_t MBOX_SET09;           // 0xA4 Mailbox 1 Core2 Set Register
    volatile uint32_t MBOX_SET10;           // 0xA8 Mailbox 2 Core2 Set Register
    volatile uint32_t MBOX_SET11;           // 0xAC Mailbox 3 Core2 Set Register
    volatile uint32_t MBOX_SET12;           // 0xB0 Mailbox 0 Core3 Set Register
    volatile uint32_t MBOX_SET13;           // 0xB4 Mailbox 1 Core3 Set Register
    volatile uint32_t MBOX_SET14;           // 0xB8 Mailbox 2 Core3 Set Register
    volatile uint32_t MBOX_SET15;           // 0xBC Mailbox 3 Core3 Set Register
// Mailbox write-clear registers (Read & Write)
// A write-clear register allows you to set additional bits low. Bits which were already low are not affected.
// You set a bit low by writing the value '1' to it. Everywhere you write a '0' bit the original register contents
// remains unchanged. Beware that you write a one to get a zero!
    volatile uint32_t MBOX_CLR00;           // 0xC0 Mailbox 00 Read & Clear Bit Register
    volatile uint32_t MBOX_CLR01;           // 0xC4 Mailbox 01 Read & Clear Bit Register
    volatile uint32_t MBOX_CLR02;           // 0xC8 Mailbox 02 Read & Clear Bit Register
    volatile uint32_t MBOX_CLR03;           // 0xCC Mailbox 03 Read & Clear Bit Register
    volatile uint32_t MBOX_CLR04;           // 0xD0 Mailbox 04 Read & Clear Bit Register
    volatile uint32_t MBOX_CLR05;           // 0xD4 Mailbox 05 Read & Clear Bit Register
    volatile uint32_t MBOX_CLR06;           // 0xD8 Mailbox 06 Read & Clear Bit Register
    volatile uint32_t MBOX_CLR07;           // 0xDC Mailbox 07 Read & Clear Bit Register
    volatile uint32_t MBOX_CLR08;           // 0xE0 Mailbox 08 Read & Clear Bit Register
    volatile uint32_t MBOX_CLR09;           // 0xE4 Mailbox 09 Read & Clear Bit Register
    volatile uint32_t MBOX_CLR10;           // 0xE8 Mailbox 10 Read & Clear Bit Register
    volatile uint32_t MBOX_CLR11;           // 0xEC Mailbox 11 Read & Clear Bit Register
    volatile uint32_t MBOX_CLR12;           // 0xF0 Mailbox 12 Read & Clear Bit Register
    volatile uint32_t MBOX_CLR13;           // 0xF4 Mailbox 13 Read & Clear Bit Register
    volatile uint32_t MBOX_CLR14;           // 0xF8 Mailbox 14 Read & Clear Bit Register
    volatile uint32_t MBOX_CLR15;           // 0xFC Mailbox 15 Read & Clear Bit Register
} interrupts_arm_local_regs_t;

extern volatile interrupts_arm_local_regs_t *INT_ARM_LOCAL;

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
