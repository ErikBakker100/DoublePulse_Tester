#pragma once

#include <stdint.h>
#include <stdbool.h>
#include "..\..\general\include\rpi.h"
#include "..\..\general\include\BCM2835.h"

#define RING_SIZE 2048  /* maak het power-of-two */
#define RING_MASK (RING_SIZE - 1)

typedef struct {
    uint32_t head; /* index waar producers schrijven (next free) */
    uint32_t tail; /* index waar consumer leest (next data) */
    uint8_t  buf[RING_SIZE];
} ring_t;

/* in gedeeld RAM, gealigneerd */
extern volatile ring_t uart_ring __attribute__((aligned(64)));

void mu_init();
void mu_putc(uint8_t c);
void mu_puts(const char *s);
uint8_t mu_getc(void);
void mu_put_uint(uint32_t n);
uint16_t mu_read_line_blocking(char *buf, uint16_t maxlen, uint32_t timeout);
void mu_put_hex32(uint32_t value);
void mu_put_hex64(uint64_t value);
void mu_put_bits32(uint32_t value);
void mu_put_bits64(uint64_t value);
void uart_transmitter_loop(void);

