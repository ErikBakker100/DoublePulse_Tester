#pragma once
#pragma once

#include <stdint.h>
#include <stdbool.h>

#define RX_BUF_SIZE 256                     // Size of the RX buffer for UART reception

/* RX ringbuffer */
typedef struct {
    volatile uint16_t head;
    volatile uint16_t tail;
    char buffer[RX_BUF_SIZE];
} rx_buf_t;

void mu_puts(const char *s);
//char mu_getc(void);
void mu_put_uint(const uint32_t n);
uint16_t read_json(char * const buf, uint16_t maxlen, uint32_t timeout);
void mu_put_hex32(const uint32_t value);
void mu_put_hex64(const uint64_t value);
void mu_put_bits8(const uint8_t value);
void mu_put_bits16(const uint16_t value);
void mu_put_bits32(const uint32_t value);
void mu_put_bits64(const uint64_t value);
void rx_put(const uint8_t c);
bool rx_available(void);
int16_t rx_get(void);