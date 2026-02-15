#include "../include/BCM2836.h"
#include "include/uart_BCM2836.h"
#include "../../include/uart.h"

void bcm2836_mu_set(uint32_t baudrate) {
    mu_set(MU_2836, AUX_2836, baudrate);
}

// Receive char's
bool bcm2836_mu_getc(void) {
    return mu_getc(MU_2836);
}

// Transmit char 
void bcm2836_mu_putc(const char c) {
    mu_putc(MU_2836, c); 
}

// Clear RX buffer mini-UART
void bcm2836_mu_flush_rx(void) {
    mu_flush_rx(MU_2836);
}

uart_ops_t bcm2836_uart_ops = {
    .set = &bcm2836_mu_set,
    .putc = &bcm2836_mu_putc,
    .flush_rx = &bcm2836_mu_flush_rx,
    .getc = &bcm2836_mu_getc
};

void bcm2836_uart_init(void)
{
    uart = &bcm2836_uart_ops;
}