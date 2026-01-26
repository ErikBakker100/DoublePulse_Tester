#include "include/timers_BCM2837.h"
#include "../../include/timers.h"

bool bcm2837_timer_set(uint8_t timer, uint32_t time) {
    // Implementation for setting the timer on BCM2837
    if (timer > 3) return false; // BCM2835 has 4 timers: 0-3
    SYS_TMR_2837->C[timer] = SYS_TMR_2837->CLO + time;
    return true;
}

bool bcm2837_timer_expired(uint8_t timer) {
     // Implementation for checking if the timer has expired
    if (timer > 3) return false; // BCM2835 has 4 timers: 0-3
        return (SYS_TMR_2837->CS & (1 << timer));
}

void bcm2837_timer_clear(uint8_t timer) {
    // Implementation for clearing the timer after being expired
    if (timer > 3) return; // BCM2835 has 4 timers: 0-3
    SYS_TMR_2837->CS = (1 << timer); // Clear the expiration flag
}

const timers_ops_t bcm2837_timers_ops = {
    .set        = bcm2837_timer_set,
    .expired    = bcm2837_timer_expired,
    .clear      = bcm2837_timer_clear,
};

void bcm2837_timer_init(void)
{
    timer = &bcm2837_timers_ops;
}
