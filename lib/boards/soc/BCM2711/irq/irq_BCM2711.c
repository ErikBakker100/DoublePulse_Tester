#include "include/irq_BCM2711.h"

#include "../../include/irq.h"
#include "../../include/uart.h"
#include "../../include/mailbox.h"
#include "../../cpu/include/cpu.h"
#include "../../include/timers.h"
#include "../../include/gpio.h"
#include "../../include/irq.h"
#include "../timers/include/timers_BCM2711.h"
#include "../uart/include/uart_BCM2711.h"
#include "../mailbox/include/mailbox_BCM2711.h"
#include "../../../../general/include/serial.h"
#include "../../../../general/include/config.h" // for BLINK_TIMER
#include "../../../../multi_core/include/core1.h"

static irq_slot_t irq_table[256];

// ------------------------------------------------------------------------------
// GIC-400 helpers
// ------------------------------------------------------------------------------
void bcm2711_gic400_irq_enable(uint32_t irq_id, uint8_t cpu_mask) {
    // Stel target CPU in (1 = Core 0, 2 = Core 1, 4 = Core 2, 8 = Core 3)
    INT_GICD_2711->ITARGETSR[irq_id] = cpu_mask;
}

// Koppel een driver-functie aan een specifiek ID.
void bcm2711_gic400_register_handler(uint32_t irq_id, irq_handler_t handler, void *data) {
    if (irq_id < 256) {
    irq_table[irq_id].handler = handler;
    irq_table[irq_id].data = data;
    }
}

// ------------------------------------------------------------------------------
// IRQ handlers for core0
// ------------------------------------------------------------------------------

void bcm2711_irq_init_core0(void) {
    irq->disable();                         // Disable interrupts
    INT_GICD_2711->CTLR = 0;                // Schakel de Distributor uit tijdens config
    INT_GICC_2711->PMR = 0xFF;              // Priority Mask: laat alle interrupts met prioriteit 0-255 door
    INT_GICC_2711->BPR = 0x3;               // 0x3 of 0x7 negeren subprioriteiten.
    static uint8_t data = 1;
    bcm2711_gic400_register_handler(GIC_IRQ_TIMER1, bcm2711_timer_callback, &data);
    bcm2711_gic400_irq_enable(GIC_IRQ_TIMER1, 1); // Enable timer interrupt voor Core 0
    data = 0;
    bcm2711_gic400_register_handler(GIC_IRQ_MINI_UART, bcm2711_uart_callback, &data);
    bcm2711_gic400_irq_enable(GIC_IRQ_MINI_UART, 1); // Enable mini UART interrupt voor Core 0
    INT_GICC_2711->CTLR = 1;                // Schakel de CPU Interface in
    INT_GICD_2711->CTLR = 1;                // Schakel de Distributor weer in
    irq->enable();                          // Enable interrupts
 }

void bcm2711_irq_handler_core0(void) {
    uint32_t iar = INT_GICC_2711->IAR;      // Acknowledge de interrupt en krijg het ID
    uint32_t irq_id = iar & 0x3FF; 
    // Roep de geregistreerde handler aan
    if (irq_id < 256 && irq_table[irq_id].handler) {
        irq_table[irq_id].handler(irq_table[irq_id].data);
    }
    INT_GICC_2711->EOIR = iar;              // End of Interrupt schrijven om de interrupt te de-acknowledgen
}

// ----------------------------------------------------------------------------------
// IRQ handlers for core1
// ----------------------------------------------------------------------------------

void bcm2711_irq_init_core1(void) {
    irq->disable();                         // Disable interrupts
    INT_GICD_2711->CTLR = 0;                // Schakel de Distributor uit tijdens config
    INT_GICC_2711->PMR = 0xFF;              // Priority Mask: laat alle interrupts met prioriteit 0-255 door
    INT_GICC_2711->BPR = 0x3;               // 0x3 of 0x7 negeren subprioriteiten.
    static uint8_t data;                           
    bcm2711_gic400_register_handler(GIC_IRQ_MBOX0, bcm2711_mailbox_irq_handler, &data);
    bcm2711_gic400_irq_enable(GIC_IRQ_MBOX0, 2); // Enable mailbox0 interrupt voor Core 1
    INT_GICC_2711->CTLR = 1;                // Schakel de CPU Interface in
    INT_GICD_2711->CTLR = 1;                // Schakel de Distributor weer in
    irq->enable();                          // Enable interrupts
}

void bcm2711_irq_handler_core1(void) {
    uint32_t iar = INT_GICC_2711->IAR;      // Acknowledge de interrupt en krijg het ID
    uint32_t irq_id = iar & 0x3FF; 
    // Roep de geregistreerde handler aan
    if (irq_id >= GIC_IRQ_MBOX0 && irq_id <= GIC_IRQ_MBOX3) {
        uint8_t mailbox_num = irq_id - GIC_IRQ_MBOX0;
        mailbox->read(mailbox_num, 1);      // Read mailbox voor core1
    }
    INT_GICC_2711->EOIR = iar;              // End of Interrupt schrijven om de interrupt te de-acknowledgen
}

// ----------------------------------------------------------------------------------
// General IRQ routines
// ----------------------------------------------------------------------------------

void bcm2711_irq_disable(void) {
#ifdef __aarch64__
    asm volatile("msr daifset, #2" ::: "memory");
#else
    asm volatile("cpsid i" ::: "memory");
#endif
}

void bcm2711_fiq_disable(void) {
#ifdef __aarch64__
    asm volatile("msr daifset, #1" ::: "memory");
#else
    asm volatile("cpsid f" ::: "memory");
#endif
}

void bcm2711_irq_enable(void) {
#ifdef __aarch64__
    asm volatile("msr daifclr, #2" ::: "memory");
#else
    asm volatile("cpsie i" ::: "memory");
#endif
}

void bcm2711_fiq_enable(void) {
#ifdef __aarch64__
    asm volatile("msr daifclr, #1" ::: "memory");
#else
    asm volatile("cpsie f" ::: "memory");
#endif
}

void bcm2711_gic400_init_distributor_disable (void) {
    INT_GICD_2711->CTLR = 0; // Disable GIC Distributor
}

void bcm2711_gic400_init_distributor_enable (void) {
    INT_GICD_2711->CTLR = 1; // Enable GIC Distributor
}

const irq_ops_t bcm2711_irq_ops = {
    .init_core0     = bcm2711_irq_init_core0,
    .handler_core0  = bcm2711_irq_handler_core0,
    .handler_core1  = bcm2711_irq_handler_core1,
    .init_core1     = bcm2711_irq_init_core1,
    .disable        = bcm2711_irq_disable,
    .enable         = bcm2711_irq_enable
};

void bcm2711_irq_init(void)
{
    irq = &bcm2711_irq_ops;
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