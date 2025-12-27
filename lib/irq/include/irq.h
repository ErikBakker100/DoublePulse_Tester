#pragma once
#include "../../general/include/BCM2835.h"
#include "../../general/include/BCM2836.h"

void timer_test_irq(void);

// IRQ handlers for core0
void irq_init_core0(void);
void irq_handler_core0(void);

// IRQ handlers for core1
void irq_handler_core1(void);   // IRQ handler for core1
void irq_init_core1(void);      // Initialize IRQs for core1
