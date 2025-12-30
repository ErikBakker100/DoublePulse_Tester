#include "include/irq.h"
#include "../multi_core/include/core1.h"
#include "../uart/include/miniuart.h"
#include "../gpio/include/gpio.h"

void timer_hart_beat(void)
{
    uint32_t t = SYS_TMR->CLO;
    SYS_TMR->C[1] = (t + 500000);   // 0,5s
    gpio_toggle(21); // toggle hart beat indicator
}

// **********************************************************************************
// IRQ handlers for core0
// **********************************************************************************

void irq_init_core0(void) {
    // Disable interrupts
    irq_disable();
    INT->FIQ_CONTROL = 0; // Disable FIQs
    INT->DISABLE_IRQS_BASIC = 0xFFFFFFFF; // Disable all basic IRQs
    INT->DISABLE_IRQS[0] = 0xFFFFFFFF; // Disable all IRQs in bank 0
    INT->DISABLE_IRQS[1] = 0xFFFFFFFF; // Disable all IRQs in bank 1
    // Mask all interrupts
    INT->ENABLE_IRQS_BASIC = 0;
    INT->ENABLE_IRQS[0] = 0;
    INT->ENABLE_IRQS[1] = 0;
    MU->MU_IER  = ((1 << 0) | (1 << 2) | (1 << 3));     // mini UART RX interrupt enable, BCM2835 errata stelt dat bit 3:2 moeten worden ingesteld voor RX interrupt
    // Enable UART and SYSTEM Timer interrupts
    INT->ENABLE_IRQS[0] = (1 << 29); // Enable Mini UART (bit 61 overall, bit 29 in ENABLE_IRQS[1])
    INT->ENABLE_IRQS[0] = (1 << 1); // Enable SYSTEM Timer C1 interrupt (bit 1 overall, bit 1 in ENABLE_IRQS[0])
    // ensure writes reach device before we enable interrupts
    cpu_dsb();
    cpu_isb();
    // Enable interrupts
    timer_hart_beat();                     // start timer IRQ for testing
    irq_enable();
 }

void irq_handler_core0(void){

    if (AUX->IRQ & 1) {  // Mini UART interrupt
        if (MU->MU_IIR & 0x04) {            // Receiver holds valid byte
            while (MU->MU_LSR & 0x01) {
                gpio_toggle(21);   // wanneer de code hier komt dan worden bytes ontvangen, laat zien door de gpio te togglen
                char c = MU->MU_IO & 0xFF;
                rx_put(c);
                gpio_toggle(21);   // wanneer de code hier komt dan worden bytes ontvangen, laat zien door de gpio te togglen
            }
        }   
    }
    // SYSTEM Timer interrupt?
    if (SYS_TMR->CS & (1 << 1)) { // Timer 1 interrupt
        SYS_TMR->CS = (1 << 1); // Acknowledge the timer interrupt
    }
    timer_hart_beat();       // restart timer
}

// **********************************************************************************
// IRQ handlers for core1
// **********************************************************************************

void irq_init_core1(void) {
    // Disable interrupts
    irq_disable();
    // Clear all mailbox interrupts
    INT_ARM_LOCAL->MBOX_CLR04 = 0xFFFFFFFF; // Wis alle bits in mailbox 1 van core1
    INT_ARM_LOCAL->MBOX_CLR05 = 0xFFFFFFFF; // Wis alle bits in mailbox 2 van core1
    INT_ARM_LOCAL->MBOX_CLR06 = 0xFFFFFFFF; // Wis alle bits in mailbox 3 van core1
    INT_ARM_LOCAL->MBOX_CLR07 = 0xFFFFFFFF; // Wis alle bits in mailbox 4 van core1
    // Enable mailbox interrupts for this core
    INT_ARM_LOCAL->MAILBOX_CNTRL1 = (MBOX0_IRQ | MBOX1_IRQ | MBOX2_IRQ | MBOX3_IRQ); // IRQ's voor alle mailboxen van core1 enabelen.
    // ensure writes reach device before we enable interrupts
    cpu_dsb();
    cpu_isb();
    // Enable interrupts
    irq_enable();
}

void irq_handler_core1(void) {
    if(INT_ARM_LOCAL->IRQ_SOURCE1 & INT_SRC_MBOX0) {
        mailbox0_core1(INT_ARM_LOCAL->MBOX_CLR04);
        INT_ARM_LOCAL->MBOX_CLR04 = 0xFFFFFFFF; // Wis de gelezen bits, en clear de IRQ daarmee
    }
    if(INT_ARM_LOCAL->IRQ_SOURCE1 & INT_SRC_MBOX1) {
        mailbox1_core1(INT_ARM_LOCAL->MBOX_CLR05);
        INT_ARM_LOCAL->MBOX_CLR05 = 0xFFFFFFFF; // Wis de gelezen bits, en clear de IRQ daarmee
    }
    if(INT_ARM_LOCAL->IRQ_SOURCE1 & INT_SRC_MBOX2) {
        mailbox2_core1(INT_ARM_LOCAL->MBOX_CLR06);
        INT_ARM_LOCAL->MBOX_CLR06 = 0xFFFFFFFF; // Wis de gelezen bits, en clear de IRQ daarmee
    }
    if(INT_ARM_LOCAL->IRQ_SOURCE1 & INT_SRC_MBOX3) {
        mailbox3_core1(INT_ARM_LOCAL->MBOX_CLR07);
        INT_ARM_LOCAL->MBOX_CLR07 = 0xFFFFFFFF; // Wis de gelezen bits, en clear de IRQ daarmee
    }
}
