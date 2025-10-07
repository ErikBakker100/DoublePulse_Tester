#pragma once

// Select the right version in the Makefile
#if defined (raspberry_zero_2w)
    #define PERIPHERAL_BASE     0x3F000000
    #define OUTPUT_PIN 18 // GPIO 18
    #define CORE_FREQ 400000000 // 400MHz for Raspberry Pi Zero2
#elif defined (raspberry_zero_1w)
    #define PERIPHERAL_BASE   0x20000000
    #define OUTPUT_PIN 18 // GPIO 18
    #define CORE_FREQ 250000000 // 250MHz for Raspberry Pi Zero
#elif defined (raspberry_pi_4b)
    #define PERIPHERAL_BASE   0xFE000000
    #define OUTPUT_PIN 18 // GPIO 18
    #define CORE_FREQ 400000000 // 400MHz for Raspberry Pi 4B
#endif

#define GPIO_BASE       (PERIPHERAL_BASE + 0x200000)