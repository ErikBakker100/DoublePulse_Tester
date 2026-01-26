#include "include/timers_BCM2835.h"
#include "../../include/timers.h"

bool bcm2835_timer_set(uint8_t timer, uint32_t time) {
    // Implementation for setting the timer on BCM2835
    if (timer > 3) return false; // BCM2835 has 4 timers: 0-3
    SYS_TMR_2835->C[timer] = SYS_TMR_2835->CLO + time;
    return true;
}

bool bcm2835_timer_expired(uint8_t timer) {
     // Implementation for checking if the timer has expired
    if (timer > 3) return false; // BCM2835 has 4 timers: 0-3
        return (SYS_TMR_2835->CS & (1 << timer));
}

void bcm2835_timer_clear(uint8_t timer) {
    // Implementation for clearing the timer after being expired
    if (timer > 3) return; // BCM2835 has 4 timers: 0-3
    SYS_TMR_2835->CS = (1 << timer); // Clear the expiration flag
}

const timers_ops_t bcm2835_timers_ops = {
    .set        = bcm2835_timer_set,
    .expired    = bcm2835_timer_expired,
    .clear      = bcm2835_timer_clear,
};

void bcm2835_timer_init(void)
{
    timer = &bcm2835_timers_ops;
}
