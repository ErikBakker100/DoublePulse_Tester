#include "include/gpio_BCM2711.h"
#include "../../include/gpio.h"

void bcm2711_init_pin(uint8_t pin, gpio_mode_t mode, gpio_pud_t pud) { /* ... */ }
void bcm2711_set(uint8_t pin) { /* ... */ }
void bcm2711_clear(uint8_t pin) { /* ... */ }
uint32_t bcm2711_read(uint8_t pin) { /* ... */ return 0; }

const gpio_ops_t bcm2711_gpio_ops = {
    .init_pin = bcm2711_init_pin,
    .set      = bcm2711_set,
    .clear    = bcm2711_clear,
    .read     = bcm2711_read,
};

void bcm2711_gpio_Init(void)
{
    gpio = &bcm2711_gpio_ops;
}
