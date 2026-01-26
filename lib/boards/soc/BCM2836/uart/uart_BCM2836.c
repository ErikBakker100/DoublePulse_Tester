#include "include/uart_BCM2836.h"
#include "../gpio/include/gpio_BCM2836.h"
#include "../../../include/boards.h"
#include "../include/BCM2836.h"
#include "../../../../general/include/config.h"
#include "../../include/uart.h"
#include "../../include/gpio.h"

void bcm2836_mu_init() {
    AUX_2836->ENABLES |= 1;          // Enable Mini UART (bit 0)
    MU_2836->MU_CNTL = 0;            // Disable TX/RX
    MU_2836->MU_LCR = 3;             // 8-bit mode, DLAB=0 (FIFO's used)
    MU_2836->MU_MCR = 0;             // RTS disabled
    MU_2836->MU_IIR = 6;             // '0110' reset receive and transmit FIFO pointers, does not clear FIFO's !!!
    MU_2836->MU_BAUD = (((board_data->core_freq_mhz * 1000000) + (4 * board_data->baudrate)) / (8 * board_data->baudrate)) - 1;
    // Set GPIO 14 and 15 to ALT5 (Mini UART)
    gpio->init_pin(14, GPIO_ALT5, PULL_NONE);
    gpio->init_pin(15, GPIO_ALT5, PULL_NONE);
    MU_2836->MU_CNTL = 3;            // Enable TX and RX
}

// Receive char 
const char bcm2836_mu_getc(void) {
    if (!(MU_2836->MU_IIR & 0x04)) return 0; // Receiver does not hold a valid byte
//  if (MU_2836->MU_LSR & 0x01)       // Buffer not empty
    return (MU_2836->MU_IO & 0xFF);
}

// Transmit char 
void bcm2836_mu_putc(const char c) {
    do{asm volatile("nop");} while (!(MU_2836->MU_LSR & 0x20)); // Wacht tot de TX FIFO ruimte heeft
    MU_2836->MU_IO = c;              // write the character to the buffer
}

// Clear RX buffer mini-UART
void bcm2836_mu_flush_rx(void)
{
    while (MU_2836->MU_LSR & 0x01) {
        (void)(MU_2836->MU_IO & 0xFF);
    }
}

const uart_ops_t bcm2836_uart_ops = {
    .init = &bcm2836_mu_init,
    .putc = &bcm2836_mu_putc,
    .flush_rx = &bcm2836_mu_flush_rx,
    .getc = &bcm2836_mu_getc
};

void bcm2836_uart_Init(void)
{
    uart = &bcm2836_uart_ops;
}