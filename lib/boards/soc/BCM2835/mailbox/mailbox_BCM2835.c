#include "../include/BCM2835.h"
#include "include/mailbox_BCM2835.h"
#include "../../include/mailbox.h"
#include "../../../../general/include/stdlib.h"

const mailboxes_ops_t bcm2835_mailbox = {
    .read   = NULL,
    .write  = NULL
};

void bcm2835_mailbox_init(void) {
    mailbox = &bcm2835_mailbox;
}