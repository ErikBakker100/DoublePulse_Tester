#include "include/irq_BCM2837.h"
#include "../../include/irq.h"
#include "../../include/uart.h"
#include "../../include/mailbox.h"
#include "../../cpu/include/cpu.h"
#include "../../include/timers.h"
#include "../../include/gpio.h"
#include "../../../../general/include/serial.h"

void bcm2837_irq_disable(void);
void bcm2837_irq_enable(void);
void bcm2837_fiq_disable(void);
void bcm2837_fiq_enable(void);

// ------------------------------------------------------------------------------
// IRQ handlers for core0
// ------------------------------------------------------------------------------

void bcm2837_irq_init_core0(void) {
    // Disable interrupts
    bcm2837_irq_disable();
    IC_2837->FIQ_CONTROL = 0; // Disable FIQs
    IC_2837->DISABLE_IRQS_BASIC = 0xFFFFFFFF; // Disable all basic IRQs
    IC_2837->DISABLE_IRQS[0] = 0xFFFFFFFF; // Disable all IRQs in bank 0
    IC_2837->DISABLE_IRQS[1] = 0xFFFFFFFF; // Disable all IRQs in bank 1
    // Mask all interrupts
    IC_2837->ENABLE_IRQS_BASIC = 0;
    IC_2837->ENABLE_IRQS[0] = 0;
    IC_2837->ENABLE_IRQS[1] = 0;
    MU_2837->MU_IER  = ((1 << 0) | (1 << 2) | (1 << 3));     // mini UART RX interrupt enable, BCM2835 errata stelt dat bit 3:2 moeten worden ingesteld voor RX interrupt
    // Enable UART and SYSTEM Timer interrupts
    IC_2837->ENABLE_IRQS[0] = (1 << 29); // Enable Mini UART (bit 61 overall, bit 29 in ENABLE_IRQS[1])
    IC_2837->ENABLE_IRQS[0] = (1 << 1); // Enable SYSTEM Timer C1 interrupt (bit 1 overall, bit 1 in ENABLE_IRQS[0])
    // ensure writes reach device before we enable interrupts
    cpu_dsb();
    cpu_isb();
    // Enable interrupts
    bcm2837_irq_enable();
 }

void bcm2837_irq_handler_core0(void) {
    if (AUX_2837->IRQ & 1) {  // Mini UART interrupt
        rx_put(uart->getc());
    }
    if (SYS_TMR_2837->CS & (1 << 1)) { // System Timer C1 interrupt
        SYS_TMR_2837->CS = (1 << 1);   // Clear the interrupt
        timer->clear(1);               // Clear timer 1 expiration flag
        gpio->toggle(21);              // Toggle GPIO 21 for heart beat indication
        timer->set(1, 1000000);        // Re-set timer 1 for 1 second
    }
}

// ----------------------------------------------------------------------------------
// IRQ handlers for core1
// ----------------------------------------------------------------------------------

void bcm2837_irq_init_core1(void) {
    // Disable interrupts
    bcm2837_irq_disable();
    // Enable mailbox interrupts for this core
    CORE_MB_CTRL_2837->MAILBOX_CNTRL[1] = (MBOX0_IRQ | MBOX1_IRQ | MBOX2_IRQ | MBOX3_IRQ); // IRQ's voor alle mailboxen van core1 enabelen.
    // ensure writes reach device before we enable interrupts
    cpu_dsb();
    cpu_isb();
    // Enable interrupts
    bcm2837_irq_enable();
}

void bcm2837_irq_handler_core1(void) {
    if(ISR_2837->IRQ_SOURCE[1] & INT_SRC_MBOX0) {
        mailbox->read(0, 1);              // Read mailbox 0 for core1
        mailbox->write(0, 1, 0xFFFFFFFF); // Erase the bits read, and clear the IRQ
    }
    if(ISR_2837->IRQ_SOURCE[1] & INT_SRC_MBOX1) {
        mailbox->read(1, 1);              // Read mailbox 1 for core1
        mailbox->write(1, 1, 0xFFFFFFFF); // Erase the bits read, and clear the IRQ
    }
    if(ISR_2837->IRQ_SOURCE[1] & INT_SRC_MBOX2) {
        mailbox->read(2, 1);              // Read mailbox 2 for core1
        mailbox->write(2, 1, 0xFFFFFFFF); // Erase the bits read, and clear the IRQ
    }
    if(ISR_2837->IRQ_SOURCE[1] & INT_SRC_MBOX3) {
        mailbox->read(3, 1);              // Read mailbox 3 for core1
        mailbox->write(3, 1, 0xFFFFFFFF); // Erase the bits read, and clear the IRQ
    }
}

// ----------------------------------------------------------------------------------
// General IRQ routines
// ----------------------------------------------------------------------------------

void bcm2837_irq_disable(void) {
#ifdef __aarch64__
    asm volatile("msr daifset, #2" ::: "memory");
#else
    asm volatile("cpsid i" ::: "memory");
#endif
}

void bcm2837_fiq_disable(void) {
#ifdef __aarch64__
    asm volatile("msr daifset, #1" ::: "memory");
#else
    asm volatile("cpsid f" ::: "memory");
#endif
}

void bcm2837_irq_enable(void) {
#ifdef __aarch64__
    asm volatile("msr daifclr, #2" ::: "memory");
#else
    asm volatile("cpsie i" ::: "memory");
#endif
}

void bcm2837_fiq_enable(void) {
#ifdef __aarch64__
    asm volatile("msr daifclr, #1" ::: "memory");
#else
    asm volatile("cpsie f" ::: "memory");
#endif
}

const irq_ops_t bcm2837_irq_ops = {
    .init_core0     = bcm2837_irq_init_core0,
    .handler_core0  = bcm2837_irq_handler_core0,
    .handler_core1  = bcm2837_irq_handler_core1,
    .init_core1     = bcm2837_irq_init_core1,
    .disable        = bcm2837_irq_disable,
    .enable         = bcm2837_irq_enable
};

void bcm2837_irq_init(void)
{
    irq = &bcm2837_irq_ops;
}

/*
void gic_init(void) {
    INT_GICD->CTLR = 0;
    // Enable UART IRQ (57)
    INT_GICD->ISENABLER[57 / 32] = 1 << (57 % 32);
    INT_GICD->IPRIORITYR[57] = 0x80;
    INT_GICD->ITARGETSR[57] = 1; // CPU0

    INT_GICD->CTLR = 1;

    INT_GICC->PMR = 0xFF;
    INT_GICC->CTLR = 1;
}

uint32_t irq = INT_GICC->IAR & 0x3FF;

switch (irq) {
    case 57:  // UART (PL011)
        uart_irq_handler();
        break;
}

GICC->EOIR = irq;
*/