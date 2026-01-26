#pragma once
#include <stdint.h>

void bcm2835_mu_init();
void bcm2835_mu_putc(const char c);
const char bcm2835_mu_getc(void);
void bcm2835_mu_flush_rx(void);
