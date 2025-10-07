// File: gpio.h
#pragma once

#include "..\..\general\rpi.h"

// GPIO Function Select Registers
#define GPFSEL0          (*(volatile unsigned int*)(GPIO_BASE + 0x00))
#define GPFSEL1          (*(volatile unsigned int*)(GPIO_BASE + 0x04))
#define GPFSEL2          (*(volatile unsigned int*)(GPIO_BASE + 0x08))
#define GPFSEL3          (*(volatile unsigned int*)(GPIO_BASE + 0x0C))
#define GPFSEL4          (*(volatile unsigned int*)(GPIO_BASE + 0x10))
#define GPFSEL5          (*(volatile unsigned int*)(GPIO_BASE + 0x14))

// GPIO Pin Output Set Registers
#define GPSET0           (*(volatile unsigned int*)(GPIO_BASE + 0x1C))
#define GPSET1           (*(volatile unsigned int*)(GPIO_BASE + 0x20))

// GPIO Pin Output Clear Registers
#define GPCLR0           (*(volatile unsigned int*)(GPIO_BASE + 0x28))
#define GPCLR1           (*(volatile unsigned int*)(GPIO_BASE + 0x2C))

// GPIO Pin Level Registers
#define GPLEV0           (*(volatile unsigned int*)(GPIO_BASE + 0x34))
#define GPLEV1           (*(volatile unsigned int*)(GPIO_BASE + 0x38))

// GPIO Pull-up/down Registers
#define GPPUD            (*(volatile unsigned int*)(GPIO_BASE + 0x94))
#define GPPUDCLK0        (*(volatile unsigned int*)(GPIO_BASE + 0x98))

#define GPIO_IN	    0
#define GPIO_OUT	1

void gpio_init(int pin, int mode);

#define gpio_set(pin)   ((pin) < 32) ? (GPSET0 = (1 << (pin))) : (GPSET1 = (1 << ((pin) - 32)))
#define gpio_clear(pin) ((pin) < 32) ? (GPCLR0 = (1 << (pin))) : (GPCLR1 = (1 << ((pin) - 32)))
