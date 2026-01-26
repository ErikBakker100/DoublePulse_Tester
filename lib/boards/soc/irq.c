#include "include/irq.h"

const irq_ops_t *irq;

//
// C Wrappers for IRQ handlers called from assembly
//
void irq_handler_core0(void)
{
    if (irq && irq->handler_core0) {
        irq->handler_core0();
    }
}

void irq_handler_core1(void)
{
    if (irq && irq->handler_core1) {
        irq->handler_core1();
    }
}