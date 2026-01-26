#pragma once
#include <stdint.h>

// UART
typedef struct {
    void (*init)(void);
    void (*putc)(const char c);
    void (*flush_rx)(void);
    const char (*getc)(void);
} uart_ops_t;
extern const uart_ops_t *uart;