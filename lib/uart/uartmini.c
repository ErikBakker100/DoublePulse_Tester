#include "include/uartmini.h"
#include "../gpio/include/gpio.h"

volatile aux_regs_t *AUX;

void uart_init() {
    AUX = (volatile aux_regs_t *)(rpi_board.mmio_base + 0x215000); // set AUX base address pointer
    // Disable the UART before configuring it
    AUX->ENABLES = 0;
    // Enable Mini UART (bit 0)
    AUX->ENABLES |= 1;

    AUX->MU_CNTL_REG = 0;           // Disable TX/RX
    AUX->MU_LCR_REG = 3;            // 8-bit mode
    AUX->MU_MCR_REG = 0;            // No flow control
    AUX->MU_IER_REG = 0;            // Disable interrupts
    AUX->MU_IIR_REG = 0xC1;         // enable FIFO, clear
    AUX->MU_BAUD_REG = (((rpi_board.core_freq_mhz * 1000000) + (4 * rpi_board.baudrate)) / (8 * rpi_board.baudrate)) - 1;
    // Set GPIO 14 and 15 to ALT5 (Mini UART)
    unsigned int ra = GPIO->FSEL[1];
    ra &= ~((7 << 12) | (7 << 15));   // Clear FSEL14/15
    ra |= (2 << 12) | (2 << 15);      // ALT5
    GPIO->FSEL[1] = ra;

    // Disable pull up/down for pins 14 and 15
    GPIO->PUD = 0;
    delay(150);
    GPIO->PUDCLK[0] = (1 << 14) | (1 << 15);
    delay(150);
    GPIO->PUDCLK[0] = 0;

    AUX->MU_CNTL_REG = 3; // Enable TX and RX
}

void uart_putc(uint8_t c) {
    while (!(AUX->MU_LSR_REG & 0x20));
    AUX->MU_IO_REG = c;
}

void uart_puts(const char *s) {
    while (*s) {
        uart_putc(*s++);
    }
}

uint8_t uart_getc(void) {
    // Wacht tot er data beschikbaar is in de RX FIFO (bit 0 = data ready)
    while (!(AUX->MU_LSR_REG & 0x01));
    // Lees de byte (laagste 8 bits)
    return (uint8_t)(AUX->MU_IO_REG & 0xFF);
}

void uart_put_uint(uint32_t n) {
    uint8_t buf[10];
    int i = 0;

    if (n == 0) {
        uart_putc('0');
        return;
    }

    while (n > 0 && i < sizeof(buf) ) {
        buf[i++] = '0' + (n % 10);
        n /= 10;
    }

    // getal staat nu omgekeerd in buf[]
    while (i--) {
        uart_putc(buf[i]);
    }
}

uint8_t uart_try_recv(void) {
    if (AUX->MU_LSR_REG & 0x01) {
        return (char)AUX->MU_IO_REG & 0xFF;
    } else {
        return -1; // Geen data beschikbaar
    }
}

uint16_t uart_read_line_blocking(char *buf, uint16_t maxlen, uint32_t timeout) {
    int i = 0, c;
    unsigned int wait = 0;

    while (i < maxlen - 1) {
        c = uart_try_recv();
        if (c == -1) {
            if (wait++ >= timeout) break;
        } else {
            buf[i++] = (char)c;
            if (c == '\n' || c == '\r') break;
            wait = 0;
        }
    }
    buf[i] = '\0';
    return i;
}

