#include "../include/BCM2837.h"
#include "include/mailbox_BCM2837.h"
#include "../../include/mailbox.h"
#include "../../../../general/include/stdlib.h"

static inline uint8_t validate_mailbox(uint8_t mailbox, uint8_t core) {
    if (mailbox > 3) mailbox = 3;
    if (core > 3) core = 3;
    return (core * 4) + mailbox;
}

uint32_t bcm2837_mailbox_read(uint8_t mailbox, uint8_t core) {
    uint8_t nr = validate_mailbox(mailbox, core);
    dmb();
    uint32_t val = MAILBOX_2837->MBOX_CLR[nr];
    MAILBOX_2837->MBOX_CLR[nr] = 0xFFFFFFFF;              // Clear all bits
    dmb();
    return val;
}

void bcm2837_mailbox_write(uint8_t mailbox, uint8_t core, uint32_t val) {
    uint8_t nr = validate_mailbox(mailbox, core);
    MAILBOX_2837->MBOX_CLR[nr] = 0xFFFFFFFF;              // Clear mailbox, writing a '1' clears that bit.
    dmb();
    MAILBOX_2837->MBOX_SET[nr] = val;
    dmb();
}

const mailboxes_ops_t bcm2837_mailbox = {
    .read   = bcm2837_mailbox_read,
    .write  = bcm2837_mailbox_write
};

void bcm2837_mailbox_init(void) {
    mailbox = &bcm2837_mailbox;
}
// --------------------------------------------------------------
// Video Core Mailbox functions
// --------------------------------------------------------------

//
// Write data to the videocore mailbox.
//
void bcm2837_mailbox_vc_write(uint8_t channel, uint32_t addres28) {
    while (MAILBOX_VC_2837->STATUS & MAIL_FULL); // Wait till the mailbox is empty
    dmb();
    MAILBOX_VC_2837->WRITE = (addres28 & 0xFFFFFFF0) | (channel & 0xF); // Combine data (28:0) with channel (3:0 bits)
}

//
// Read data from the videocore mailbox.
//
uint32_t bcm2837_mailbox_vc_read(uint8_t channel) {
    for (;;) {                              // Loop until we receive something from the requested channel
		while ((MAILBOX_VC_2837->STATUS & MAIL_EMPTY) != 0);  // Wait for data
		volatile uint32_t address = MAILBOX_VC_2837->READ; // Read the data
        dmb();                              // Make sure that the CPU will not read from its Cache, but out of RAM.
		uint8_t readChannel = address & 0xF;
		if (readChannel == channel)         // Return if it's for the requested channel
			return address & 0xFFFFFFF0;
	}
}

const mailbox_vc_ops_t bcm2837_mailbox_vc_ops = {
    .read   = bcm2837_mailbox_vc_read,
    .write  = bcm2837_mailbox_vc_write,
};

void bcm2837_mailbox_vc_init(void) {
    mailbox_vc = &bcm2837_mailbox_vc_ops;
}
