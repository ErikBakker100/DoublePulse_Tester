#include "../include/BCM2835.h"
#include "include/uart_BCM2835.h"
#include "../../include/uart.h"

void bcm2835_mu_set(uint32_t baudrate) {
    mu_set(MU_2835, AUX_2835, baudrate);
}

bool bcm2835_mu_getc(void) {
    return mu_getc(MU_2835);
}

void bcm2835_mu_putc(const char c) {
    mu_putc(MU_2835, c); 
}

void bcm2835_mu_flush_rx(void) {
    mu_flush_rx(MU_2835);
}

uart_ops_t bcm2835_uart_ops = {
    .set = &bcm2835_mu_set,
    .putc = &bcm2835_mu_putc,
    .flush_rx = &bcm2835_mu_flush_rx,
    .getc = &bcm2835_mu_getc
};

void bcm2835_uart_init(void)
{
    uart = &bcm2835_uart_ops;
}