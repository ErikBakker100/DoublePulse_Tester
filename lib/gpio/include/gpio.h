// File: gpio.h
#pragma once
#include <stdint.h>
#include "..\..\general\include\rpi.h"
#include "..\..\general\include\BCM2835.h"

#define GPIO_IN	    0
#define GPIO_OUT	1

void gpio_init();

void gpio_init_pin(uint8_t pin, uint8_t mode);

void gpio_set(uint32_t pin) ;

void gpio_clear(uint32_t pin);