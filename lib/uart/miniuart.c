#include "include/miniuart.h"
#include "../gpio/include/gpio.h"

void mu_init() {
    AUX->ENABLES |= 1;          // Enable Mini UART (bit 0)
    MU->MU_CNTL = 0;            // Disable TX/RX
    MU->MU_LCR = 3;             // 8-bit mode, DLAB=0 (FIFO's used)
    MU->MU_MCR = 0;             // RTS disabled
    MU->MU_IIR = 6;             // '0110' reset receive and transmit FIFO pointers, does not clear FIFO's !!!
    MU->MU_BAUD = (((rpi_board.core_freq_mhz * 1000000) + (4 * rpi_board.baudrate)) / (8 * rpi_board.baudrate)) - 1;
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
    MU->MU_CNTL = 3;            // Enable TX and RX
}

// **********************************************************************************
// Transmit functions 
// **********************************************************************************

void mu_putc(const char c) {
    do{asm volatile("nop");} while (!(MU->MU_LSR & 0x20)); // Wacht tot de TX FIFO ruimte heeft
    MU->MU_IO = c;              // write the character to the buffer
}

void mu_puts(const char *s) {
    while (*s) {
        mu_putc((char)*s++);
    }
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

void mu_put_bits8(const uint8_t value) {
    for (int i = 7; i >= 0; i--) {
        mu_putc((value & (1 << i)) ? '1' : '0');
    }
}

void mu_put_bits16(const uint16_t value) {
    for (int i = 15; i >= 0; i--) {
        mu_putc((value & (1 << i)) ? '1' : '0');
        if (i % 8 == 0 && i != 0) mu_putc('.');  // optionele spatie per byte
    }
}

void mu_put_bits32(const uint32_t value) {
    for (int i = 31; i >= 0; i--) {
        mu_putc((value & (1 << i)) ? '1' : '0');
        if (i % 8 == 0 && i != 0) mu_putc('.');  // optionele spatie per byte
    }
}

void mu_put_bits64(const uint64_t value) {
    for (int i = 63; i >= 0; i--) {
        mu_putc((value & (1l << i)) ? '1' : '0');
        if (i % 8 == 0 && i != 0) mu_putc('.');  // optionele spatie per byte
    }
}

// **********************************************************************************
// Receive functions 
// **********************************************************************************

volatile rx_buf_t *RX_BUF = {0};          // RX struct (ringbuffer) pointer

void mu_flush_rx(void)
{
    while (MU->MU_LSR & 0x01) {
        (void)(MU->MU_IO & 0xFF);
    }
}

bool rx_available(void) {
    return RX_BUF->head != RX_BUF->tail;
}

void rx_put(const uint8_t c) {
    uint16_t next = (RX_BUF->head + 1) % RX_BUF_SIZE;
    if (next != RX_BUF->tail) {          // drop byte if buffer full
        RX_BUF->buffer[RX_BUF->head] = c;
        RX_BUF->head = next;
    }
}

int16_t rx_get(void) {
    if (RX_BUF->head == RX_BUF->tail)
        return -1;
    uint8_t c = RX_BUF->buffer[RX_BUF->tail];
    RX_BUF->tail = (RX_BUF->tail + 1) % RX_BUF_SIZE;
    return c;
}

uint16_t mu_read_json(char * const buf, uint16_t maxlen, uint32_t timeout_us) {

    uint16_t i = 0;
    SYS_TMR->C[3] = (SYS_TMR->CLO + timeout_us); // set timeout

    buf[0] = '\0';

    while (!(SYS_TMR->CS & (1 << 3)) && (i < maxlen - 1)) { // while no timeout and buffer not full
        if (!rx_available()) continue;
        int16_t c = rx_get();
        if (c < 0) continue;

        buf[i++] = (char)c;
    }
   // Timeout or overflow → reset
   SYS_TMR->CS = (1 << 3); // reset timeout flag
   buf[i] = '\0';
   return i;
}
