#include "include/gpio.h"

volatile gpio_regs_t *GPIO;

void gpio_init() {
    GPIO = (volatile gpio_regs_t *)(rpi_board.mmio_base + 0x200000);
}

void gpio_init_pin(uint8_t pin, uint8_t mode) {
    uint32_t reg_index = pin / 10;          // Elk GPFSEL register behandelt 10 pins
    uint32_t bit = (pin % 10) * 3;          // 3 bits per pin

    // Mask de 3 bits voor deze pin, en zet ze op 'mode' (GPIO_IN of GPIO_OUT)
    GPIO->FSEL[reg_index] &= ~(7u << bit);
    GPIO->FSEL[reg_index] |=  (mode << bit);
}

void gpio_set(uint32_t pin) {
    if (pin < 32)
        GPIO->SET[0] = (1u << pin);
    else
        GPIO->SET[1] = (1u << (pin - 32));
}

void gpio_clear(uint32_t pin) {
    if (pin < 32)
        GPIO->CLR[0] = (1u << pin);
    else
        GPIO->CLR[1] = (1u << (pin - 32));
}