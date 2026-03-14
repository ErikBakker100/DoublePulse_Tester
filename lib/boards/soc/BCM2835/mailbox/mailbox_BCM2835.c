#include "../include/BCM2835.h"
#include "include/mailbox_BCM2835.h"
#include "../../include/mailbox.h"
#include "../../../../general/include/stdlib.h"

//
// Write data to the videocore mailbox.
//
void bcm2835_mailbox_vc_write(uint8_t channel, uint32_t addres28) {
    while (MAILBOX_VC_2835->STATUS & MAIL_FULL); // Wait till the mailbox is empty
    dmb();
    MAILBOX_VC_2835->WRITE = (addres28 & 0xFFFFFFF0) | (channel & 0xF); // Combine data (28:0) with channel (3:0 bits)
}

//
// Read data from the videocore mailbox.
//
uint32_t bcm2835_mailbox_vc_read(uint8_t channel) {
    for (;;) {                              // Loop until we receive something from the requested channel
		while ((MAILBOX_VC_2835->STATUS & MAIL_EMPTY) != 0);  // Wait for data
		volatile uint32_t address = MAILBOX_VC_2835->READ; // Read the data
        dmb();                              // Make sure that the CPU will not read from its Cache, but out of RAM.
		uint8_t readChannel = address & 0xF;
		if (readChannel == channel)         // Return if it's for the requested channel
			return address & 0xFFFFFFF0;
	}
}

const mailbox_vc_ops_t bcm2835_mailbox_vc_ops = {
    .read   = bcm2835_mailbox_vc_read,
    .write  = bcm2835_mailbox_vc_write,
};

void bcm2835_mailbox_vc_init(void)
{
    mailbox_vc = &bcm2835_mailbox_vc_ops;
}
