#include "../include/BCM2836.h"
#include "include/mailbox_BCM2836.h"
#include "../../include/mailbox.h"
#include "../../../../general/include/stdlib.h"

static inline uint8_t validate_mailbox(uint8_t mailbox, uint8_t core) {
    if (mailbox > 3) mailbox = 3;
    if (core > 3) core = 3;
    return (core * 4) + mailbox;
}

uint32_t bcm2836_mailbox_read(uint8_t mailbox, uint8_t core) {
    uint8_t nr = validate_mailbox(mailbox, core);
    uint32_t val = MAILBOX_2836->MBOX_CLR[nr];
    MAILBOX_2836->MBOX_CLR[nr] = 0xFFFFFFFF;              // Clear all bits
    return val;
}

void bcm2836_mailbox_write(uint8_t mailbox, uint8_t core, uint32_t val) {
    uint8_t nr = validate_mailbox(mailbox, core);
    MAILBOX_2836->MBOX_CLR[nr] = 0xFFFFFFFF;              // Clear mailbox, writing a '1' clears that bit.
    MAILBOX_2836->MBOX_SET[nr] = val;
}

const mailboxes_ops_t bcm2836_mailbox = {
    .read   = bcm2836_mailbox_read,
    .write  = bcm2836_mailbox_write
};

void bcm2836_mailbox_init(void) {
    mailbox = &bcm2836_mailbox;
}
// --------------------------------------------------------------
// Video Core Mailbox functions
// --------------------------------------------------------------

//
// Write data to the videocore mailbox.
//
void bcm2836_mailbox_vc_write(uint8_t channel, uint32_t addres28) {
    bcm283x_mailbox_vc_write(MAILBOX_VC_2836, channel, addres28);
}

//
// Read data from the videocore mailbox.
//
uint32_t bcm2836_mailbox_vc_read(uint8_t channel) {
    return bcm283x_mailbox_vc_read(MAILBOX_VC_2836, channel);
}

const mailbox_vc_ops_t bcm2836_mailbox_vc_ops = {
    .read   = bcm2836_mailbox_vc_read,
    .write  = bcm2836_mailbox_vc_write,
};

void bcm2836_mailbox_vc_init(void) {
    mailbox_vc = &bcm2836_mailbox_vc_ops;
}

