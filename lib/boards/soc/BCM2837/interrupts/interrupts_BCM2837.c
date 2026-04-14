#include "include/interrupts_BCM2837.h"
#include "../../include/interrupts.h"
#include "../../include/uart.h"
#include "../../include/mailbox.h"
#include "../../cpu/include/cpu.h"
#include "../../include/timers.h"
#include "../../include/gpio.h"
#include "../../../../general/include/serial.h"
#include "../../../../general/include/config.h" // for BLINK_TIMER
#include "../../../../multi_core/include/core1.h"

// ------------------------------------------------------------------------------
// IRQ handlers for core0
// ------------------------------------------------------------------------------

void bcm2837_interrupts_init_core0(void) {
    // Disable interrupts
    interrupts->irq_disable();
    interrupts->fiq_disable();
    IC_2837->FIQ_CONTROL = 0; // Disable FIQs
    IC_2837->DISABLE_IRQS_BASIC = 0xFFFFFFFF; // Disable all basic IRQs
    IC_2837->DISABLE_IRQS[0] = 0xFFFFFFFF; // Disable all IRQs in bank 0
    IC_2837->DISABLE_IRQS[1] = 0xFFFFFFFF; // Disable all IRQs in bank 1
    // Mask all interrupts
    IC_2837->ENABLE_IRQS_BASIC = 0;
    IC_2837->ENABLE_IRQS[0] = 0;
    IC_2837->ENABLE_IRQS[1] = 0;
    // Enable UART and SYSTEM Timer interrupts
    IC_2837->ENABLE_IRQS[0] = (1 << 29); // Enable Mini UART (bit 61 overall, bit 29 in ENABLE_IRQS[1])
    IC_2837->ENABLE_IRQS[0] = (1 << 1); // Enable SYSTEM Timer C1 interrupt (bit 1 overall, bit 1 in ENABLE_IRQS[0])
    // ensure writes reach device before we enable interrupts
    dsb();
    isb();
    // Enable interrupts
    interrupts->irq_enable();
//    interrupts->fiq_enable();
 }

void bcm2837_irq_handler_core0(void) {
    if (AUX_2837->IRQ & 1) {  // Mini UART interrupt
        uart->rxc();
    }
    
    if (SYS_TMR_2837->CS & (1 << 1)) { // System Timer C1 interrupt
        SYS_TMR_2837->CS = (1 << 1);   // Clear the interrupt
        timer->clear(1);               // Clear timer 1 expiration flag
        gpio->toggle(STATUS_PIN);              // Toggle GPIO 21 for heart beat indication
        timer->set(1, BLINK_TIMER);        // Re-set timer 1 for 1 second
    }
}

void bcm2837_fiq_handler_core0(void) {
    if (AUX_2837->IRQ & 1) {  // Mini UART interrupt
        uart->rxc();
    }
    
    if (SYS_TMR_2837->CS & (1 << 1)) { // System Timer C1 interrupt
        SYS_TMR_2837->CS = (1 << 1);   // Clear the interrupt
        timer->clear(1);               // Clear timer 1 expiration flag
        gpio->toggle(STATUS_PIN);              // Toggle GPIO 21 for heart beat indication
        timer->set(1, BLINK_TIMER);        // Re-set timer 1 for 1 second
    }
}


// ----------------------------------------------------------------------------------
// IRQ handlers for core1
// ----------------------------------------------------------------------------------

void bcm2837_interrupts_init_core1(void) {
    // Disable interrupts
    interrupts->irq_disable();
    interrupts->fiq_disable();
    // Enable mailbox interrupts for this core
    CORE_MB_CTRL_2837->MAILBOX_CNTRL[1] = (MBOX0_IRQ); // IRQ voor mailbox 0 van core1 enabelen.
    // ensure writes reach device before we enable interrupts
    dsb();
    isb();
    // Enable interrupts
    interrupts->irq_enable();
    interrupts->fiq_enable();
}

void bcm2837_irq_handler_core1(void) {
    if(ISR_2837->IRQ_SOURCE[1] & INT_SRC_MBOX0) {
        mailbox0_core1(mailbox->read(0, 1));// Read mailbox 0 for core1
     }
}

void bcm2837_fiq_handler_core1(void) {
    if(ISR_2837->IRQ_SOURCE[1] & INT_SRC_MBOX0) {
        mailbox0_core1(mailbox->read(0, 1));// Read mailbox 0 for core1
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

const interrupts_ops_t bcm2837_interrupts_ops = {
    .init_core0         = bcm2837_interrupts_init_core0,
    .irq_handler_core0  = bcm2837_irq_handler_core0,
    .fiq_handler_core0  = bcm2837_fiq_handler_core0,
    .irq_handler_core1  = bcm2837_irq_handler_core1,
    .fiq_handler_core1  = bcm2837_fiq_handler_core1,
    .init_core1         = bcm2837_interrupts_init_core1,
    .irq_disable        = bcm2837_irq_disable,
    .fiq_disable        = bcm2837_fiq_disable,
    .irq_enable         = bcm2837_irq_enable,
    .fiq_enable         = bcm2837_fiq_enable
};

void bcm2837_interrupts_init(void)
{
    interrupts = &bcm2837_interrupts_ops;
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