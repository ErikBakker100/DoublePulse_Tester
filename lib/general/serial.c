#include "include/serial.h"
#include "../boards/soc/include/uart.h"

volatile rx_buf_t *RX_BUF = {0};          // RX struct (ringbuffer) pointer

void mu_puts(const char *s) {
    while (*s) {
        uart->putc((char)*s++);
    }
}

void mu_put_uint(const uint32_t x) {
    uint32_t n = x;
    char buf[10];
    int i = 0;

    if (n == 0) {
        uart->putc('0');
        return;
    }

    while (n > 0 && i < sizeof(buf) ) {
        buf[i++] = '0' + (n % 10);
        n /= 10;
    }

    // getal staat nu omgekeerd in buf[]
    while (i--) {
        uart->putc(buf[i]);
    }
}

void mu_put_hex32(const uint32_t value) {
    uart->putc('0');
    uart->putc('x');
    for (int i = 28; i >= 0; i -= 4) {
        char nibble = (value >> i) & 0xF;
        char c = (nibble < 10) ? ('0' + nibble) : ('A' + nibble - 10);
        uart->putc(c);
    }
}

void mu_put_hex64(const uint64_t value) {
    uart->putc('0');
    uart->putc('x');
    for (int i = 60; i >= 0; i -= 4) {
        char nibble = (value >> i) & 0xF;
        char c = (nibble < 10) ? ('0' + nibble) : ('A' + nibble - 10);
        uart->putc(c);
    }
}

void mu_put_bits8(const uint8_t value) {
    for (int i = 7; i >= 0; i--) {
        uart->putc((value & (1 << i)) ? '1' : '0');
    }
}

void mu_put_bits16(const uint16_t value) {
    for (int i = 15; i >= 0; i--) {
        uart->putc((value & (1 << i)) ? '1' : '0');
        if (i % 8 == 0 && i != 0) uart->putc('.');  // optionele spatie per byte
    }
}

void mu_put_bits32(const uint32_t value) {
    for (int i = 31; i >= 0; i--) {
        uart->putc((value & (1 << i)) ? '1' : '0');
        if (i % 8 == 0 && i != 0) uart->putc('.');  // optionele spatie per byte
    }
}

void mu_put_bits64(const uint64_t value) {
    for (int i = 63; i >= 0; i--) {
        uart->putc((value & (1l << i)) ? '1' : '0');
        if (i % 8 == 0 && i != 0) uart->putc('.');  // optionele spatie per byte
    }
}

// **********************************************************************************
// Receive functions 
// **********************************************************************************
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

