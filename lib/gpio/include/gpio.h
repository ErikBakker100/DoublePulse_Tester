// File: gpio.h
#pragma once
#include <stdint.h>
#include "..\..\general\include\rpi.h"

typedef struct {
    /* 0x00–0x14: Function Select Registers */
    volatile uint32_t FSEL[6];     // 0x00, 0x04, 0x08, 0x0C, 0x10, 0x14
    uint32_t reserved0;           // 0x18 (onbenut)
    /* 0x1C–0x20: Pin Output Set */
    volatile uint32_t SET[2];      // 0x1C, 0x20
    uint32_t reserved1;            // 0x24 (onbenut)
    /* 0x28–0x2C: Pin Output Clear */
    volatile uint32_t CLR[2];      // 0x28, 0x2C
    uint32_t reserved2;            // 0x30 (onbenut)
    /* 0x34–0x38: Pin Level */
    volatile uint32_t LEV[2];      // 0x34, 0x38
    uint32_t reserved3;            // 0x3C (onbenut)
    /* 0x40–0x44: Event Detect Status */
    volatile uint32_t EDS[2];      // 0x40, 0x44
    uint32_t reserved4;            // 0x48 (onbenut)
    /* 0x4C–0x50: Rising Edge Detect Enable */
    volatile uint32_t REN[2];      // 0x4C, 0x50
    uint32_t reserved5;            // 0x54 (onbenut)
    /* 0x58–0x5C: Falling Edge Detect Enable */
    volatile uint32_t FEN[2];      // 0x58, 0x5C
    uint32_t reserved6;            // 0x60 (onbenut)
    /* 0x64–0x68: High Detect Enable */
    volatile uint32_t HEN[2];      // 0x64, 0x68
    uint32_t reserved7;            // 0x6C (onbenut)
    /* 0x70–0x74: Low Detect Enable */
    volatile uint32_t LEN[2];      // 0x70, 0x74
    uint32_t reserved8;            // 0x78 (onbenut)
    /* 0x7C–0x80: Async Rising Edge Detect */
    volatile uint32_t AREN[2];     // 0x7C, 0x80
    uint32_t reserved9;            // 0x84 (onbenut)
     /* 0x88–0x8C: Async Falling Edge Detect */
    volatile uint32_t AFEN[2];     // 0x88, 0x8C
    uint32_t reserved10;           // 0x90 (onbenut)
    /* 0x94–0x9C: Pull-up/down + Clocks */
    volatile uint32_t PUD;         // 0x94
    volatile uint32_t PUDCLK[2];   // 0x98, 0x9C
} gpio_regs_t;

extern volatile gpio_regs_t *GPIO;

#define GPIO_IN	    0
#define GPIO_OUT	1

void gpio_init();

void gpio_init_pin(uint8_t pin, uint8_t mode);

void gpio_set(uint32_t pin) ;

void gpio_clear(uint32_t pin);