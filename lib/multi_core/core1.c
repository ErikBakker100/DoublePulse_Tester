#include "include/core1.h"
#include "../general/include/config.h"

void irq_handler_mailbox0_core1(void);
void irq_handler_mailbox1_core1(void);
void irq_handler_mailbox2_core1(void);
void irq_handler_mailbox3_core1(void);

unsigned long delay1 = 0; // holds Pulse Width1 delay
unsigned long delay2 = 0; // holds Inter Pulse width delay
unsigned long delay3 = 0; // holds Pulse Width2 delay
unsigned long delay4 = 0; // holds Pulse Interval delay

/*
// Start core1
// voor 32 bit : asm volatile("dsb sy; sev" ::: "memory");
// omdat Core1 in WFE staat (Wait For Event) moet asm code worden uitgevoerd om te activeren. 'dsb' is een Data Synchronization Barrier 
// instructie die ervoor zorgt dat alle voorgaande geheugenoperaties zijn voltooid voordat verder wordt gegaan. 'sev' is de Set Event instructie
// die een event signaleert, waardoor een core die in WFE (Wait For Event) staat, wordt gewekt en verder kan gaan met uitvoeren.
// 'ishst' specificeert dat de DSB-operatie betrekking heeft op alle inner-shareable geheugenlocaties en dat het effect van de DSB-operatie zichtbaar moet zijn
// voor alle inner-shareable caches en buffers voordat de instructie verder gaat.
*/

// Delay loop
volatile static void delay(int count) {
    while (count--) asm volatile("nop");
}

void start_core1(void) {
    *core_boot(1) = (core_reg_t)(uintptr_t)&core_entry_1;
#if defined(__aarch64__) || defined(__AARCH64__)
    asm volatile ("dsb ishst");
#else
    asm volatile ("dsb");
#endif
    asm volatile ("sev"); // stuur event om core wakker te maken
}

// Entry point for core1
void core_main_1() {
    uint32_t bank = OUTPUT_PIN<32?0:1;
    volatile uint32_t *gpio_on = &GPIO->SET[bank];
    volatile uint32_t *gpio_off = &GPIO->CLR[bank];
    uint32_t mask = OUTPUT_PIN<32?(1u << OUTPUT_PIN):(1u << (OUTPUT_PIN - 32));
    
    irq_init_core1();

    while (1) {
        *gpio_on = mask;
        delay(delay1); // PulseWidth1
        *gpio_off = mask;
        delay(delay2); // interPulseDelay
        *gpio_on = mask;
        delay(delay3); // PulseWith2
        *gpio_off = mask;
        delay(delay4); // Pulseinterval
   }
}

void irq_init_core1(void) {
    // Disable interrupts
    irq_disable();
    // Clear all mailbox interrupts
    INT_ARM_LOCAL_REGS->MBOX_CLR04_REG = 0xFFFFFFFF; // Wis alle bits in mailbox 1 van core1
    INT_ARM_LOCAL_REGS->MBOX_CLR05_REG = 0xFFFFFFFF; // Wis alle bits in mailbox 2 van core1
    INT_ARM_LOCAL_REGS->MBOX_CLR06_REG = 0xFFFFFFFF; // Wis alle bits in mailbox 3 van core1
    INT_ARM_LOCAL_REGS->MBOX_CLR07_REG = 0xFFFFFFFF; // Wis alle bits in mailbox 4 van core1
    // Enable mailbox interrupts for this core
    INT_ARM_LOCAL_REGS->MAILBOX_CNTRL1_REG = (MBOX0_IRQ | MBOX1_IRQ | MBOX2_IRQ | MBOX3_IRQ); // IRQ's voor alle mailboxen van core1 enabelen.
    // ensure writes reach device before we enable interrupts
    __asm__ volatile ("dsb sy":::"memory");
    __asm__ volatile ("isb":::"memory");
    // Enable interrupts
    irq_enable();
}

void irq_handler_core1(void) {
    if(INT_ARM_LOCAL_REGS->IRQ_SOURCE1_REG && INT_SRC_MBOX0) {
        irq_handler_mailbox0_core1();
        INT_ARM_LOCAL_REGS->MBOX_CLR04_REG = 0xFFFFFFFF; // Wis de gelezen bits, en clear de IRQ daarmee
    }
    if(INT_ARM_LOCAL_REGS->IRQ_SOURCE1_REG && INT_SRC_MBOX1) {
        irq_handler_mailbox1_core1();
        INT_ARM_LOCAL_REGS->MBOX_CLR05_REG = 0xFFFFFFFF; // Wis de gelezen bits, en clear de IRQ daarmee
    }
    if(INT_ARM_LOCAL_REGS->IRQ_SOURCE1_REG && INT_SRC_MBOX2) {
        irq_handler_mailbox2_core1();
        INT_ARM_LOCAL_REGS->MBOX_CLR06_REG = 0xFFFFFFFF; // Wis de gelezen bits, en clear de IRQ daarmee
    }
    if(INT_ARM_LOCAL_REGS->IRQ_SOURCE1_REG && INT_SRC_MBOX3) {
        irq_handler_mailbox3_core1();
        INT_ARM_LOCAL_REGS->MBOX_CLR07_REG = 0xFFFFFFFF; // Wis de gelezen bits, en clear de IRQ daarmee
    }
}

void irq_handler_mailbox0_core1(void) {
    mu_puts("MBOX0 IRQ Core1: ");
    delay1 = INT_ARM_LOCAL_REGS->MBOX_CLR04_REG;
    mu_put_uint(INT_ARM_LOCAL_REGS->MBOX_CLR04_REG);
    mu_puts("\r\n");
}

void irq_handler_mailbox1_core1(void) {
    mu_puts("MBOX1 IRQ Core1: ");
    delay2 = INT_ARM_LOCAL_REGS->MBOX_CLR05_REG;
    mu_put_uint(INT_ARM_LOCAL_REGS->MBOX_CLR05_REG);
    mu_puts("\r\n");

}

void irq_handler_mailbox2_core1(void) {
    mu_puts("MBOX2 IRQ Core1: ");
    delay3 = INT_ARM_LOCAL_REGS->MBOX_CLR06_REG;
    mu_put_uint(INT_ARM_LOCAL_REGS->MBOX_CLR06_REG);
    mu_puts("\r\n");

}

void irq_handler_mailbox3_core1(void) {
    mu_puts("MBOX3 IRQ Core1: ");
    delay4 = INT_ARM_LOCAL_REGS->MBOX_CLR07_REG;
    mu_put_uint(INT_ARM_LOCAL_REGS->MBOX_CLR07_REG);
    mu_puts("\r\n");
}
