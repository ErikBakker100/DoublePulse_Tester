#pragma once
#include <stdint.h>
#include <stdbool.h>

typedef struct {
    bool (*set)(uint8_t timer, uint32_t time); // Set timer to expire after 'time'. The free running counter is driven by the timer. Returns true on success
                                            // clock and stopped whenever the processor is stopped in debug mode.
    bool (*expired)(uint8_t timer);         // Check if timer has expired, returns true if expired
    void (*clear)(uint8_t timer);           // After timer expiration, clear the flag. clear timer channel comparator matches.
                                            // The system timer match bits are routed to the interrupt controller where they can generate an interrupt.
                                            // If using with interrrupts, the interrupts should be routed seperately
} timers_ops_t;
extern const timers_ops_t *timer;