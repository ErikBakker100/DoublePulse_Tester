#include "include/BCM2835.h"

volatile sys_timer_regs_t *SYS_TMR;
volatile int_regs_t *INT;
volatile arm_timer_regs_t *ARM_TMR;
volatile gp_clks_regs_t *GPIO_CLKS;
volatile gpio_regs_t *GPIO;
volatile uart_regs_t *UART;
volatile sdhost_regs_t *SDHOST;
volatile pcm_regs_t *PCM;
volatile spi_regs_t *SPI0;
volatile i2c1_regs_t *I2C1;
volatile pwm_regs_t *PWM;
volatile i2c_spi_slave_regs_t *I2C_SPI_SLAVE;
volatile aux_regs_t *AUX;
volatile mu_regs_t *MU;
volatile spi12_regs_t *SPI1;
volatile spi12_regs_t *SPI2;
volatile emmc_regs_t *EMMC;
volatile i2c2_regs_t *I2C2;
volatile i2c3_regs_t *I2C3;

void BCM2835_Init(uintptr_t mmio_base) {
    SYS_TMR = (volatile sys_timer_regs_t *)(mmio_base + 0x3000);
    INT = (volatile int_regs_t *)(mmio_base + 0xB200);
    ARM_TMR = (volatile arm_timer_regs_t *)(mmio_base + 0xB400);
    GPIO_CLKS = (volatile gp_clks_regs_t*)(mmio_base + 0x101070);
    GPIO = (volatile gpio_regs_t *)(mmio_base + 0x200000);
    UART = (volatile uart_regs_t *)(mmio_base + 0x201000);
    SDHOST = (volatile sdhost_regs_t *)(mmio_base + 0x202000);
    PCM = (volatile pcm_regs_t *)(mmio_base + 0x203000);
    SPI0 = (volatile spi_regs_t *)(mmio_base + 0x204000);
    I2C1 = (volatile i2c1_regs_t *)(mmio_base + 0x205000);
    PWM = (volatile pwm_regs_t *)(mmio_base + 0x20C000);
    I2C_SPI_SLAVE = (volatile i2c_spi_slave_regs_t *)(mmio_base + 0x214000);
    AUX = (volatile aux_regs_t *)(mmio_base + 0x215000);
    MU = (volatile mu_regs_t *)(mmio_base + 0x215000 + 0x40);
    SPI1 = (volatile spi12_regs_t *)(mmio_base + 0x215000 + 0x80);
    SPI2 = (volatile spi12_regs_t *)(mmio_base + 0x215000 + 0xC0);
    EMMC = (volatile emmc_regs_t *)(mmio_base + 0x300000);
    I2C2 = (volatile i2c2_regs_t *)(mmio_base + 0x804000);
    I2C3 = (volatile i2c3_regs_t *)(mmio_base + 0x805000);

    SYS_TMR->CS = 0; // Clear timer control/status register
    SYS_TMR->C[0] = 0;
    SYS_TMR->C[1] = 0;
    SYS_TMR->C[2] = 0;
    SYS_TMR->C[3] = 0; // Clear timer counter registers
}

