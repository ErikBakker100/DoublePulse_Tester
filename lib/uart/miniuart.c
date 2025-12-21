#include "include/miniuart.h"
#include "../gpio/include/gpio.h"

void mu_init() {
    MU->ENABLES |= 1;               // Enable Mini UART (bit 0)
    MU->MU_CNTL_REG = 0;            // Disable TX/RX
    MU->MU_LCR_REG = 3;             // 8-bit mode, DLAB=0 (FIFO's used)
    MU->MU_MCR_REG = 0;             // RTS disabled
    MU->MU_IER_REG = 0;             // Disable IRQ's
    MU->MU_IIR_REG = 6;             // Clear receive and transmit FIFO
    MU->MU_BAUD_REG = (((rpi_board.core_freq_mhz * 1000000) + (4 * rpi_board.baudrate)) / (8 * rpi_board.baudrate)) - 1;
    // Set GPIO 14 and 15 to ALT5 (Mini UART)
    unsigned int ra = GPIO->FSEL[1];
    ra &= ~((7 << 12) | (7 << 15)); // Clear FSEL14/15
    ra |= (2 << 12) | (2 << 15);    // ALT5
    GPIO->FSEL[1] = ra;
    // Disable pull up/down for pins 14 and 15
    GPIO->PUD = 0;
    delay(150);
    GPIO->PUDCLK[0] = (1 << 14) | (1 << 15);
    delay(150);
    GPIO->PUDCLK[0] = 0;
    MU->MU_CNTL_REG = 3;            // Enable TX and RX
}

void mu_putc(const char c) {
    while (!(MU->MU_LSR_REG & 0x20)); // Wacht tot de TX FIFO ruimte heeft
    MU->MU_IO_REG = c;
}

void mu_puts(const char *s) {
    const char *p = (const char *)s;
    while (*p) {
        mu_putc((char)*p++);
    }
}

char mu_getc(void) {
    // Wacht tot er data beschikbaar is in de RX FIFO (bit 0 = data ready)
    while (!(MU->MU_LSR_REG & 0x41));   // 0xb100 0001 = FIFO holds at least one symbol, and transmitter empty
    // Lees de byte (laagste 8 bits)
    return (char)(MU->MU_IO_REG & 0xFF);
}

void mu_put_uint(const uint32_t x) {
    uint32_t n = x;
    char buf[10];
    int i = 0;

    if (n == 0) {
        mu_putc('0');
        return;
    }

    while (n > 0 && i < sizeof(buf) ) {
        buf[i++] = '0' + (n % 10);
        n /= 10;
    }

    // getal staat nu omgekeerd in buf[]
    while (i--) {
        mu_putc(buf[i]);
    }
}

int mu_try_recv(void) {
    if (MU->MU_LSR_REG & 0x01) {    // Data beschikbaar
        return MU->MU_IO_REG & 0xFF;
    } else {
        return -1; // Geen data beschikbaar
    }
}

static inline uint32_t timer_us(void)
{
    return TMR->CLO;
}

uint16_t mu_read_json(char * const buf, uint16_t maxlen, uint32_t timeout) {
    uint32_t start = timer_us();
    int c = 0, depth = 0;
    uint32_t start = timer_us();
    uint16_t i = 0;
    bool started = false;
    while (i < maxlen - 1) {
        c = mu_try_recv();
        if (c == -1) {
            if (wait++ >= timeout) {
                break; // Timeout
            }
            continue;
        } else {
            if (!started) {
                if (c == '{') {
                    started = true;
                    buf[i++] = c;
                }
                continue;
            }        
            if (c < 0x20 || c > 0x7E) {
                if (wait++ >= timeout) {
                    break; // Timeout
                }
                continue;
            }
            if (c == '}') {
                buf[i++] = (char)c;
                break;
            }
            buf[i++] = (char)c;
            wait = 0;
        }
    }
    buf[i] = '\0';
    return i;
}

void mu_put_hex32(const uint32_t value) {
    mu_putc('0');
    mu_putc('x');
    for (int i = 28; i >= 0; i -= 4) {
        char nibble = (value >> i) & 0xF;
        char c = (nibble < 10) ? ('0' + nibble) : ('A' + nibble - 10);
        mu_putc(c);
    }
}

void mu_put_hex64(const uint64_t value) {
    mu_putc('0');
    mu_putc('x');
    for (int i = 60; i >= 0; i -= 4) {
        char nibble = (value >> i) & 0xF;
        char c = (nibble < 10) ? ('0' + nibble) : ('A' + nibble - 10);
        mu_putc(c);
    }
}

void mu_put_bits32(const uint32_t value) {
    for (int i = 31; i >= 0; i--) {
        mu_putc((value & (1 << i)) ? '1' : '0');
        if (i % 8 == 0 && i != 0) mu_putc(' ');  // optionele spatie per byte
    }
}

void mu_put_bits64(const uint64_t value) {
    for (int i = 63; i >= 0; i--) {
        mu_putc((value & (1l << i)) ? '1' : '0');
        if (i % 8 == 0 && i != 0) mu_putc(' ');  // optionele spatie per byte
    }
}

