#include "../include/BCM2837.h"
#include "include/uart_BCM2837.h"
#include "../../include/uart.h"

void bcm2837_mu_set(uint32_t baudrate) {
    mu_set(MU_2837, AUX_2837, baudrate);
}

// Receive char's
bool bcm2837_mu_getc(void) {
    return mu_getc(MU_2837);
}

// Transmit char 
void bcm2837_mu_putc(const char c) {
    mu_putc(MU_2837, c); 
}

// Clear RX buffer mini-UART
void bcm2837_mu_flush_rx(void) {
    mu_flush_rx(MU_2837);
}

uart_ops_t bcm2837_uart_ops = {
    .set = &bcm2837_mu_set,
    .putc = &bcm2837_mu_putc,
    .flush_rx = &bcm2837_mu_flush_rx,
    .getc = &bcm2837_mu_getc
};

void bcm2837_uart_init(void)
{
    uart = &bcm2837_uart_ops;
}