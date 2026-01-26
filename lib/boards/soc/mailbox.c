#include "include/mailbox.h"

const mailbox_vc_ops_t *mailbox_vc;
const mailboxes_ops_t *mailbox;

volatile uint32_t mailbox_buffer[8] __attribute__((aligned(16)));