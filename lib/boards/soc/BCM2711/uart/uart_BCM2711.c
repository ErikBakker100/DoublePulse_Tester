#include "../include/BCM2711.h"
#include "include/uart_BCM2711.h"
#include "../../include/uart.h"

void bcm2711_uart_callback(void *data){
    // Handle UART interrupt (e.g., read received data)
    uart->getc();
}

void bcm2711_mu_set(uint32_t baudrate) {
    mu_set(MU_2711, AUX_2711, baudrate);
}

// Receive char's
bool bcm2711_mu_getc(void) {
    return mu_getc(MU_2711);
}

// Transmit char 
void bcm2711_mu_putc(const char c) {
    mu_putc(MU_2711, c); 
}

// Clear RX buffer mini-UART
void bcm2711_mu_flush_rx(void) {
    mu_flush_rx(MU_2711);
}

uart_ops_t bcm2711_uart_ops = {
    .set = &bcm2711_mu_set,
    .putc = &bcm2711_mu_putc,
    .flush_rx = &bcm2711_mu_flush_rx,
    .getc = &bcm2711_mu_getc
};

void bcm2711_uart_init(void)
{
    uart = &bcm2711_uart_ops;
}