#pragma once
#include <stdint.h>

// IRQ
typedef struct {
    // IRQ handlers for core0
    void (*init_core0)(void);
    void (*handler_core0)(void);
    // IRQ handlers for core1
    void (*handler_core1)(void);   // IRQ handler for core1
    void (*init_core1)(void);      // Initialize IRQs for core1
    void (*disable)(void);
    void (*enable)(void);
} irq_ops_t;
extern const irq_ops_t *irq;

extern void irq_handler_core0(void);
extern void irq_handler_core1(void);
