#pragma once
#if defined (raspberry_zero_1w)
    #pragma message "using settings for raspberry_zero_1w"
    #define OUTPUT_PIN ((uint8_t)18) // GPIO 18
    #ifndef CORE_FREQ
        #define CORE_FREQ 250
    #endif
#elif defined (raspberry_zero_2w)
    #pragma message "using settings for raspberry_zero_2w"
    #define OUTPUT_PIN ((uint8_t)18) // GPIO 18
    #ifndef CORE_FREQ
        #define CORE_FREQ 500
    #endif
#elif defined (raspberry_pi_4b)
    #pragma message "using settings for raspberry_pi_4b"
    #define OUTPUT_PIN ((uint8_t)18) // GPIO 18
    #ifndef CORE_FREQ
        #define CORE_FREQ 500
    #endif
#endif

#ifndef CORE_FREQ
    #define CORE_FREQ 250
#endif

#ifndef BAUDRATE
    #define BAUDRATE 115200
#endif

#define CHAR_BUFFER 100 // Size of the UART buffer for JSON strings

#define DEFAULT_PULSE_WIDTH1 70      // in microseconds
#define DEFAULT_INTER_PULSE_DELAY 30  // in microseconds
#define DEFAULT_PULSE_WIDTH2 50      // in microseconds
#define DEFAULT_PULSE_INTERVAL 500    // in microseconds
