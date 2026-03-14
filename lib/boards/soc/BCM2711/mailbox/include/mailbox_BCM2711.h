#pragma once
#include <stdint.h>

extern void bcm2711_mailbox_vc_init(void);
extern void bcm2711_mailbox_init(void);
extern uint32_t bcm2711_mailbox_read(uint8_t mailbox, uint8_t core);
extern void bcm2711_mailbox_write(uint8_t mailbox, uint8_t core, uint32_t val);
extern void bcm2711_mailbox_vc_write(uint8_t channel, uint32_t addres28);
extern uint32_t bcm2711_mailbox_vc_read(uint8_t channel);

void bcm2711_mailbox_irq_handler(void *);