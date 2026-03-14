#include "include/uart.h"
#include "../include/boards.h"
#include "../../general/include/config.h"
#include "include/gpio.h"
#include "../../general/include/serial.h"
#include "include/mailbox.h"

uart_ops_t *uart;

void mu_set(volatile bcm2835_mu_regs_t *mu_regs, volatile bcm2835_aux_regs_t *aux_regs, uint32_t baudrate) {
    aux_regs->ENABLES |= 1;          // Enable Mini UART (bit 0)
    mu_regs->MU_CNTL = 0;            // Disable TX/RX
    mu_regs->MU_LCR = 3;             // 8-bit mode, DLAB=0 (FIFO's used)
    mu_regs->MU_MCR = 0;             // RTS disabled
    mu_regs->MU_IIR = 6;             // '0110' reset receive and transmit FIFO pointers, does not clear FIFO's !!!
    mu_regs->MU_BAUD = (((board.clock_rates[CORE_id]) + (4 * baudrate)) / (8 * baudrate)) - 1;
    // Set GPIO 14 and 15 to ALT5 (Mini UART)
    gpio->init_pin(14, GPIO_ALT5, PULL_NONE);
    gpio->init_pin(15, GPIO_ALT5, PULL_NONE);
    mu_regs->MU_CNTL = 3;            // Enable TX and RX
}

// Receive char's
bool mu_getc(volatile bcm2835_mu_regs_t *regs) {
    if (!(regs->MU_IIR & 0x04)) return false; // Receiver does not hold a valid byte
    while (regs->MU_LSR & 1) { 
        rx_put(regs->MU_IO & 0xFF);
    }
    return true;
}

// Transmit char 
void mu_putc(volatile bcm2835_mu_regs_t *regs, const char c) {
    do{asm volatile("nop");} while (!(regs->MU_LSR & 0x20)); // Wacht tot de TX FIFO ruimte heeft
   regs->MU_IO = c;              // write the character to the buffer
}

// Clear RX buffer mini-UART
void mu_flush_rx(volatile bcm2835_mu_regs_t *regs)
{
    while (regs->MU_LSR & 0x01) {
        (void)(regs->MU_IO & 0xFF);
    }
}