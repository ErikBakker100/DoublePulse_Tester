#include "include/gpio_BCM2836.h"
#include "../../include/gpio.h"
#include "../../../../general/include/stdlib.h" // For DELAY function

void bcm2836_gpio_init_pin(uint8_t pin, gpio_mode_t mode, gpio_pud_t pud) {
    uint32_t reg_index = pin / 10;          // Every GPFSEL register handles 10 pins
    uint32_t shift = (pin % 10) * 3;          // 3 bits per pin
    // Mask the 3 bits for this pin, and put them to 'mode' (gpio_mode_t)
    uint32_t val = GPIO_2836->FSEL[reg_index];
    val &= ~(7u << shift);
    val |=  ((uint32_t)mode << shift);
    GPIO_2836->FSEL[reg_index] = val;
    // Set pull up/down for pin
    GPIO_2836->PUD = (uint32_t)pud;
    DELAY(150);
    reg_index = pin / 32;
    uint32_t clk_bit = 1u << (pin % 32);
    GPIO_2836->PUDCLK[reg_index] = (1 << clk_bit);
    DELAY(150);
    GPIO_2836->PUD = 0;
    GPIO_2836->PUDCLK[reg_index] = 0;
}

void bcm2836_gpio_set(uint8_t pin) {
    uint8_t reg_index = pin / 32;          // Elk register behandelt 32 pins    
    uint32_t mask = 1u << (pin % 32);
    GPIO_2836->SET[reg_index] = mask;
    }

void bcm2836_gpio_clear(uint8_t pin) {
    uint8_t reg_index = pin / 32;          // Elk register behandelt 32 pins    
    uint32_t mask = 1u << (pin % 32);
    GPIO_2836->CLR[reg_index] = mask;
}   

void bcm2836_gpio_toggle(uint8_t pin) {
    uint8_t reg_index = pin / 32;          // Elk register behandelt 32 pins    
    uint32_t mask = 1u << (pin % 32);
    if (GPIO_2836->LEV[reg_index] & mask) GPIO_2836->CLR[reg_index] = mask;
    else GPIO_2836->SET[reg_index] = mask;
}

gpio_mode_t bcm2836_gpio_read(uint8_t pin) {
    uint32_t reg_index = pin / 10;          // Every GPFSEL register handles 10 pins
    uint32_t shift = (pin % 10) * 3;        // 3 bits per pin

    // Mask the 3 bits for this pin, and read 'mode'
    uint32_t val = GPIO_2836->FSEL[reg_index];
    val >>= shift;
    val &= 0x7;                             // isolate 3 bits
    return (gpio_mode_t)val;
}

const gpio_ops_t bcm2836_gpio_ops = {
    .init_pin   = bcm2836_gpio_init_pin,
    .set        = bcm2836_gpio_set,
    .clear      = bcm2836_gpio_clear,
    .toggle     = bcm2836_gpio_toggle,
    .read       = bcm2836_gpio_read
};

void bcm2836_gpio_init(void)
{
    gpio = &bcm2836_gpio_ops;
}

