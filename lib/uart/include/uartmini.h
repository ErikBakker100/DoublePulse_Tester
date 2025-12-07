#pragma once

#include <stdint.h>
#include "..\..\general\include\rpi.h"

typedef struct {
    volatile uint32_t IRQ;
    volatile uint32_t ENABLES;
    uint32_t reserved1[14];
    volatile uint32_t MU_IO_REG;
    volatile uint32_t MU_IER_REG;
    volatile uint32_t MU_IIR_REG;
    volatile uint32_t MU_LCR_REG;
    volatile uint32_t MU_MCR_REG;
    volatile uint32_t MU_LSR_REG;
    volatile uint32_t MU_MSR_REG;
    volatile uint32_t MU_SCRATCH;
    volatile uint32_t MU_CNTL_REG;
    volatile uint32_t MU_STAT_REG;
    volatile uint32_t MU_BAUD_REG;
} aux_regs_t;

extern volatile aux_regs_t *AUX;

void uart_init();
void uart_putc(uint8_t c);
void uart_puts(const char *s);
uint8_t uart_getc(void);
void uart_put_uint(uint32_t n);
uint16_t uart_read_line_blocking(char *buf, uint16_t maxlen, uint32_t timeout);
