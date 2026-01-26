#pragma once
#include <stdint.h>

// GPIO
typedef enum {
    GPIO_INPUT  = 0b000,  // 000
    GPIO_OUTPUT = 0b001,  // 001
    GPIO_ALT5   = 0b010,  // 010
    GPIO_ALT4   = 0b011,  // 011
    GPIO_ALT0   = 0b100,  // 100
    GPIO_ALT1   = 0b101,  // 101
    GPIO_ALT2   = 0b110,  // 110
    GPIO_ALT3   = 0b111,  // 111
} gpio_mode_t;

typedef enum {
    PULL_NONE = 0,
    PULL_DOWN = 1,
    PULL_UP = 2,
} gpio_pud_t;

typedef struct {
    void (*init_pin)(uint8_t pin, gpio_mode_t mode, gpio_pud_t pud);
    void (*set)(uint8_t pin);
    void (*clear)(uint8_t pin);
    void (*toggle)(uint8_t pin);
    uint8_t (*read)(uint8_t pin);
} gpio_ops_t;
extern const gpio_ops_t *gpio;
