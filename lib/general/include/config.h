#pragma once
#include <stdint.h>

#define OUTPUT_PIN 18                       // GPIO pin used for output.
#define TRIGGER_PIN 19                      // GPIO pin used for triggering scope.
#define STATUS_PIN 21                       // GPIO pin used for status LED, used for heart beat indication

#define BAUDRATE 115200

#define DEFAULT_PULSE_WIDTH1 70             // in microseconds
#define DEFAULT_INTER_PULSE_DELAY 30        // in microseconds
#define DEFAULT_PULSE_WIDTH2 50             // in microseconds
#define DEFAULT_PULSE_INTERVAL 500          // in microseconds
extern volatile uint32_t Intervals[4]; // Array to hold the intervals
#define MAX_INTERVAL 255                    // Maximum allowed value for pulse widths and intervals, based on the fact that we are using uint8_t to store these values in the bitstream for the PWM signal.

#define CPUID_REG (*(volatile uint32_t *)0xE000ED00)

#define BLINK_TIMER 100000

#define JSON_TIMEOUT_US 50000              // Timeout for receiving a full JSON string, in microseconds (at 115200 baud, one byte is ~87 microseconds)
