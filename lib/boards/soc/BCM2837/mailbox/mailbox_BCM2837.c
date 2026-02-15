#include "../include/BCM2837.h"
#include "include/mailbox_BCM2837.h"
#include "../../include/mailbox.h"

uint32_t bcm2837_mailbox_read(uint8_t mailbox, uint8_t core) {
    // Mailbox Read
    return 0;
}

void bcm2837_mailbox_write(uint8_t mailbox, uint8_t core, uint32_t val) {
    // Mailbox Write
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

/* Data Memory Barrier */
static inline void dmb(void) {
    #if defined(__arm__) && (__ARM_ARCH <= 6)
        // ARMv6 (Pi 1 / Zero)
        __asm__ __volatile__ ("mcr p15, 0, %0, c7, c10, 5" : : "r" (0) : "memory");
    #else
        // ARMv7 en ARMv8 (Pi 2, 3, 4, 5 in zowel 32-bit als 64-bit)
        __asm__ __volatile__ ("dmb sy" : : : "memory");
    #endif
}

//
// Write data to the videocore mailbox.
//
void bcm2837_mailbox_vc_write(uint8_t channel, uint32_t addres28) {
    bcm283x_mailbox_vc_write(MAILBOX_VC_2837, channel, addres28);
}

//
// Read data from the videocore mailbox.
//
uint32_t bcm2837_mailbox_vc_read(uint8_t channel) {
    return bcm283x_mailbox_vc_read(MAILBOX_VC_2837, channel);
}

const mailbox_vc_ops_t bcm2837_mailbox_vc_ops = {
    .read   = bcm2837_mailbox_vc_read,
    .write  = bcm2837_mailbox_vc_write,
};

void bcm2837_mailbox_vc_init(void) {
    mailbox_vc = &bcm2837_mailbox_vc_ops;
}
