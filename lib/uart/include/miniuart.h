#pragma once

#include <stdint.h>
#include <stdbool.h>
#include "..\..\general\include\rpi.h"
#include "..\..\general\include\BCM2835.h"
#include "..\..\general\include\stdlib.h"

void mu_init();
void mu_putc(const char c);
void mu_puts(const char *s);
char mu_getc(void);
void mu_put_uint(const uint32_t n);
uint16_t mu_read_json(char * const buf, uint16_t maxlen, uint32_t timeout);
void mu_put_hex32(const uint32_t value);
void mu_put_hex64(const uint64_t value);
void mu_put_bits32(const uint32_t value);
void mu_put_bits64(const uint64_t value);


