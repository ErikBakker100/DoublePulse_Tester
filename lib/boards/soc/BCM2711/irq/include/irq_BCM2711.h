#pragma once
#include "../../include/BCM2711.h"

#define GIC_IRQ_TIMER1     97
#define GIC_IRQ_TIMER3     99
#define GIC_IRQ_USB        124
#define GIC_IRQ_MINI_UART  125
#define GIC_IRQ_I2C1       151
#define GIC_IRQ_UART0      153
#define GIC_IRQ_MBOX0      65
#define GIC_IRQ_MBOX1      66
#define GIC_IRQ_MBOX2      67
#define GIC_IRQ_MBOX3      68

typedef void (*irq_handler_t)(void *data);
typedef struct {
    irq_handler_t handler;
    void *data;
} irq_slot_t;

void bcm2711_irq_init(void);