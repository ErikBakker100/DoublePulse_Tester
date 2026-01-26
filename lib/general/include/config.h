#pragma once
#include <stdint.h>

#define OUTPUT_PIN ((uint8_t)OUTPUT)        // GPIO pin used for output, set in CMakeLists.txt

#define BAUDRATE 115200

#define DEFAULT_PULSE_WIDTH1 70             // in microseconds
#define DEFAULT_INTER_PULSE_DELAY 30        // in microseconds
#define DEFAULT_PULSE_WIDTH2 50             // in microseconds
#define DEFAULT_PULSE_INTERVAL 500          // in microseconds
extern uint32_t Intervals[4]; // Array to hold the intervals

#define CPUID_REG (*(volatile uint32_t *)0xE000ED00)

