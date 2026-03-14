#include "include/irq_BCM2836.h"
#include "../../include/irq.h"
#include "../../include/uart.h"
#include "../../include/mailbox.h"
#include "../../cpu/include/cpu.h"
#include "../../../../general/include/serial.h"
#include "../../../../general/include/config.h" // for BLINK_TIMER
#include "../../../../multi_core/include/core1.h"
#include "../../../../general/include/stdlib.h"

// ------------------------------------------------------------------------------
// IRQ handlers for core0
// ------------------------------------------------------------------------------

void bcm2836_irq_init_core0(void) {
    // Disable interrupts
    irq->disable();
    IC_2836->FIQ_CONTROL = 0; // Disable FIQs
    IC_2836->DISABLE_IRQS_BASIC = 0xFFFFFFFF; // Disable all basic IRQs
    IC_2836->DISABLE_IRQS[0] = 0xFFFFFFFF; // Disable all IRQs in bank 0
    IC_2836->DISABLE_IRQS[1] = 0xFFFFFFFF; // Disable all IRQs in bank 1
    // Mask all interrupts
    IC_2836->ENABLE_IRQS_BASIC = 0;
    IC_2836->ENABLE_IRQS[0] = 0;
    IC_2836->ENABLE_IRQS[1] = 0;
    MU_2836->MU_IER  = ((1 << 0) | (1 << 2) | (1 << 3));     // mini UART RX interrupt enable, BCM2835 errata stelt dat bit 3:2 moeten worden ingesteld voor RX interrupt
    // Enable UART and SYSTEM Timer interrupts
    IC_2836->ENABLE_IRQS[0] = (1 << 29); // Enable Mini UART (bit 61 overall, bit 29 in ENABLE_IRQS[1])
//    IC_2836->ENABLE_IRQS[0] = (1 << 1); // Enable SYSTEM Timer C1 interrupt (bit 1 overall, bit 1 in ENABLE_IRQS[0])
    // ensure writes reach device before we enable interrupts
    dsb();
    isb();
    // Enable interrupts
    irq->enable();
 }

void bcm2836_irq_handler_core0(void) {
    if (AUX_2836->IRQ & 1) {  // Mini UART interrupt
        rx_put(uart->getc());
    }
}

// ----------------------------------------------------------------------------------
// IRQ handlers for core1
// ----------------------------------------------------------------------------------

void bcm2836_irq_init_core1(void) {
    // Disable interrupts
    irq->disable();
    // Enable mailbox interrupts for this core
    CORE_MB_CTRL_2836->MAILBOX_CNTRL[1] = (MBOX0_IRQ); // IRQ voor mailbox 0 van core1 enabelen.
    // ensure writes reach device before we enable interrupts
    dsb();
    isb();
    // Enable interrupts
    irq->enable();
}

void bcm2836_irq_handler_core1(void) {
    if(ISR_2836->IRQ_SOURCE[1] & INT_SRC_MBOX0) {
        mailbox0_core1(mailbox->read(0, 1));// Read mailbox 0 for core1
    }
}

// ----------------------------------------------------------------------------------
// General IRQ routines
// ----------------------------------------------------------------------------------

void bcm2836_irq_disable(void) {
#ifdef __aarch64__
    asm volatile("msr daifset, #2" ::: "memory");
#else
    asm volatile("cpsid i" ::: "memory");
#endif
}

void bcm2836_fiq_disable(void) {
#ifdef __aarch64__
    asm volatile("msr daifset, #1" ::: "memory");
#else
    asm volatile("cpsid f" ::: "memory");
#endif
}

void bcm2836_irq_enable(void) {
#ifdef __aarch64__
    asm volatile("msr daifclr, #2" ::: "memory");
#else
    asm volatile("cpsie i" ::: "memory");
#endif
}

void bcm2836_fiq_enable(void) {
#ifdef __aarch64__
    asm volatile("msr daifclr, #1" ::: "memory");
#else
    asm volatile("cpsie f" ::: "memory");
#endif
}

const irq_ops_t bcm2836_irq_ops = {
    .init_core0     = bcm2836_irq_init_core0,
    .handler_core0  = bcm2836_irq_handler_core0,
    .handler_core1  = bcm2836_irq_handler_core1,
    .init_core1     = bcm2836_irq_init_core1,
    .disable        = bcm2836_irq_disable,
    .enable         = bcm2836_irq_enable
};

void bcm2836_irq_init(void)
{
    irq = &bcm2836_irq_ops;
}
