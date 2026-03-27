#include "../include/BCM2711.h"
#include "include/mailbox_BCM2711.h"
#include "../../include/mailbox.h"
#include "../../../../general/include/stdlib.h"
#include "../../../../multi_core/include/core1.h"

uint32_t bcm2711_mailbox_read(uint8_t mailbox, uint8_t core) {
    return 0; // Reading from the mailbox is done via interrupts, so this function can be a stub.
}

void bcm2711_mailbox_write(uint8_t mailbox, uint8_t core, uint32_t val) {
    // GICD_SGIR: Software Generated Interrupt Register
    // TargetListFilter (bits 25:24) = 00 (gebruik TargetList)
    // CPUTargetList (bits 23:16) = 0x02 (Core 1 is de tweede bit)
    // SGIINTID (bits 3:0) = de gewenste ID (bijv. 0)
    dmb();
    INT_GICD_2711->SGIR = (1 << (16 + core)) | (mailbox & 0xF);
    dmb();
}

const mailboxes_ops_t bcm2711_mailbox = {
    .read   = bcm2711_mailbox_read,
    .write  = bcm2711_mailbox_write
};

void bcm2711_mailbox_init(void) {
    mailbox = &bcm2711_mailbox;
}

// --------------------------------------------------------------
// Mailbox wrapper functions
// --------------------------------------------------------------
void bcm2711_mailbox_irq_handler(void *data) {
    uint32_t result = mailbox->read(0, 1); // Read mailbox 0 for core1
    mailbox0_core1(result);
}

// --------------------------------------------------------------
// Video Core Mailbox functions
// --------------------------------------------------------------

//
// Write data to the videocore mailbox.
//
void bcm2711_mailbox_vc_write(uint8_t channel, uint32_t addres28) {
    while (MAILBOX_VC_2711->STATUS & MAIL_FULL); // Wait till the mailbox is empty
    dmb();
    MAILBOX_VC_2711->WRITE = (addres28 & 0xFFFFFFF0) | (channel & 0xF); // Combine data (28:0) with channel (3:0 bits)
}

//
// Read data from the videocore mailbox.
//
uint32_t bcm2711_mailbox_vc_read(uint8_t channel) {
    for (;;) {                              // Loop until we receive something from the requested channel
		while ((MAILBOX_VC_2711->STATUS & MAIL_EMPTY) != 0);  // Wait for data
		volatile uint32_t address = MAILBOX_VC_2711->READ; // Read the data
        dmb();                              // Make sure that the CPU will not read from its Cache, but out of RAM.
		uint8_t readChannel = address & 0xF;
		if (readChannel == channel)         // Return if it's for the requested channel
			return address & 0xFFFFFFF0;
	}
}

const mailbox_vc_ops_t bcm2711_mailbox_vc_ops = {
    .read   = bcm2711_mailbox_vc_read,
    .write  = bcm2711_mailbox_vc_write,
};

void bcm2711_mailbox_vc_init(void) {
    mailbox_vc = &bcm2711_mailbox_vc_ops;
}
