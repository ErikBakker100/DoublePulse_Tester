#include "include/uartmini.h"
#include "../gpio/include/gpio.h"

extern void delay(unsigned int);

void uart_init() {
    // Disable the UART before configuring it
    AUX_ENABLES = 0;

    AUX_ENABLES = 1;

    AUX_MU_CNTL_REG = 0;           // Disable TX/RX
    AUX_MU_LCR_REG = 3;            // 8-bit mode
    AUX_MU_MCR_REG = 0;            // No flow control
    AUX_MU_IER_REG = 0;            // Disable interrupts
    AUX_MU_IIR_REG = 0xC6;         // Clear FIFOs
    AUX_MU_BAUD_REG = ((CORE_FREQ * 1000000) / (8 * BAUDRATE)) - 1; // Set baud rate

    // Set GPIO 14 and 15 to ALT5 (Mini UART)
    unsigned int ra = GPFSEL1;
    ra &= ~(7 << 12);   // Clear FSEL14
    ra |= 2 << 12;      // ALT5
    ra &= ~(7 << 15);   // Clear FSEL15
    ra |= 2 << 15;      // ALT5
    GPFSEL1 = ra;

    // Disable pull up/down for pins 14 and 15
    GPPUD = 0;
    delay(150);
    GPPUDCLK0 = (1 << 14) | (1 << 15);
    delay(150);
    GPPUDCLK0 = 0;

    AUX_MU_CNTL_REG = 3; // Enable TX and RX
}

void uart_send(unsigned int c) {
    while (!(AUX_MU_LSR_REG & 0x20));
    AUX_MU_IO_REG = c;
}

void uart_puts(const char *s) {
    while (*s) {
        uart_send(*s++);
    }
}

void uart_put_uint(unsigned int n) {
    char buf[10];
    int i = 0;

    if (n == 0) {
        uart_send('0');
        return;
    }

    while (n > 0 && i < sizeof(buf) ) {
        buf[i++] = '0' + (n % 10);
        n /= 10;
    }

    // getal staat nu omgekeerd in buf[]
    while (i--) {
        uart_send(buf[i]);
    }
}

int uart_try_recv(void) {
    if (AUX_MU_LSR_REG & 0x01) {
        return AUX_MU_IO_REG & 0xFF;
    } else {
        return -1; // Geen data beschikbaar
    }
}
    
int uart_read_line_blocking(char *buf, int maxlen, unsigned int timeout) {
    int c = uart_try_recv();
    int i = 0;
    if (c == -1) {
        buf[i] = '\0';
        return 0; // Geen data beschikbaar om te starten
    }
    buf[i++] = (char)c;
    unsigned int wait = 0;
    while (i < maxlen - 1) {
        // Wacht op volgend karakter met timeout
        do {
            c = uart_try_recv();
            if (wait++ >= timeout) {
                buf[i] = '\0';
                return i; // Timeout tijdens lezen
            }
        } while (c == -1);
        buf[i++] = (char)c;
        if (c == '\n') break;
        wait = 0; // Reset timeout na succesvol karakter
    }
    buf[i] = '\0'; // Sluit string af
    if (i == maxlen - 1) {
        buf[i] = '\0'; // Zorg ervoor dat de buffer altijd een null-terminator heeft
    }
    return i;
}
