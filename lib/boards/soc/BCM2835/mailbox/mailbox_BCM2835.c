#include "../include/BCM2835.h"
#include "include/mailbox_BCM2835.h"
#include "../../include/mailbox.h"

//
// Write data to the videocore mailbox.
//
void bcm2835_mailbox_vc_write(uint8_t channel, uint32_t addres28) {
    bcm283x_mailbox_vc_write(MAILBOX_VC_2835, channel, addres28);
}

//
// Read data from the videocore mailbox.
//
uint32_t bcm2835_mailbox_vc_read(uint8_t channel) {
    return bcm283x_mailbox_vc_read(MAILBOX_VC_2835, channel);
}

const mailbox_vc_ops_t bcm2835_mailbox_vc_ops = {
    .read   = bcm2835_mailbox_vc_read,
    .write  = bcm2835_mailbox_vc_write,
};

void bcm2835_mailbox_vc_init(void)
{
    mailbox_vc = &bcm2835_mailbox_vc_ops;
}
