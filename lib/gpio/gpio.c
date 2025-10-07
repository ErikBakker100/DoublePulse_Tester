#include "include/gpio.h"

void gpio_init(int pin, int mode) {
    if (pin < 10)      GPFSEL0 &= ~(7 << ((pin - 0) * 3));
    else if (pin < 20) GPFSEL1 &= ~(7 << ((pin - 10) * 3));
    else if (pin < 30) GPFSEL2 &= ~(7 << ((pin - 20) * 3));
    else if (pin < 40) GPFSEL3 &= ~(7 << ((pin - 30) * 3));
    else if (pin < 50) GPFSEL4 &= ~(7 << ((pin - 40) * 3));
    else if (pin < 60) GPFSEL5 &= ~(7 << ((pin - 50) * 3));

    if (pin < 10)      GPFSEL0 |= mode << ((pin - 0) * 3);
    else if (pin < 20) GPFSEL1 |= mode << ((pin - 10) * 3);
    else if (pin < 30) GPFSEL2 |= mode << ((pin - 20) * 3);
    else if (pin < 40) GPFSEL3 |= mode << ((pin - 30) * 3);
    else if (pin < 50) GPFSEL4 |= mode << ((pin - 40) * 3);
    else if (pin < 60) GPFSEL5 |= mode << ((pin - 50) * 3);
}
