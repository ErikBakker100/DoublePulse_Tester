#include <stdint.h>
#include "include/rpi.h"
#include "include/stdlib.h"
#include "../uart/include/miniuart.h" // for mu_puts, mu_put_hex32, mu_put_bits32

void get_board_info(void);

void irq_disable(void) {
#ifdef __aarch64__
    asm volatile("msr daifset, #2" ::: "memory");
#else
    asm volatile("cpsid i" ::: "memory");
#endif
}

void fiq_disable(void) {
#ifdef __aarch64__
    asm volatile("msr daifset, #1" ::: "memory");
#else
    asm volatile("cpsid f" ::: "memory");
#endif
}

void irq_enable(void) {
#ifdef __aarch64__
    asm volatile("msr daifclr, #2" ::: "memory");
#else
    asm volatile("cpsie i" ::: "memory");
#endif
}

void fiq_enable(void) {
#ifdef __aarch64__
    asm volatile("msr daifclr, #1" ::: "memory");
#else
    asm volatile("cpsie f" ::: "memory");
#endif
}

rpi_board_t rpi_board = {
    .baudrate = BAUDRATE,
    .core_freq_mhz = CORE_FREQ
};

void board_init(void) {
    get_board_info();
    BCM2835_Init(rpi_board.mmio_base);
    BCM2836_Init(rpi_board.mmio_base);
}

void get_board_info() {
    // The base address of the MMIO registers depends on the Raspberry Pi version, lets find out which board we are running on
    /* read the system register */
#if defined(__aarch64__) || defined(__AARCH64__)
    uint64_t reg;
    asm volatile ("mrs %x0, midr_el1" : "=r" (reg));
    rpi_board.soc_type = (reg >> 4) & 0xFFF;
#else
    uint32_t reg;
    asm volatile ("mrc p15,0,%0,c0,c0,0" : "=r" (reg));
    rpi_board.soc_type = (reg >> 4) & 0xFFF;
#endif
    /* get the PartNum, detect board and MMIO base address */
    switch (rpi_board.soc_type) {
        case 0xB76:
            rpi_board.name = "Rpi1";
            rpi_board.mmio_base = 0x20000000UL;
            rpi_board.mmio_base_end = 0x20FFFFFFUL;
            rpi_board.gpu_io_base = 0x7E000000UL;
            // If GPU L2 Cache Disabled, else gpu_mem_base = 0x40000000UL;
            rpi_board.gpu_mem_base = 0xC0000000UL;
            break;
        case 0xC07:
            rpi_board.name = "Rpi2";
            rpi_board.mmio_base = 0x3F000000UL;
            rpi_board.mmio_base_end = 0x300FFFFFUL;
            rpi_board.gpu_io_base = 0x7E000000UL;
            rpi_board.gpu_mem_base = 0xC0000000UL;
            break;
        case 0xD03:
            rpi_board.name = "Rpi3";
            rpi_board.mmio_base = 0x3F000000UL;
            rpi_board.mmio_base_end = 0x300FFFFFUL;
            rpi_board.gpu_io_base = 0x7E000000UL;
            rpi_board.gpu_mem_base = 0xC0000000UL;
            break;
        case 0xD08:
            rpi_board.name = "Rpi4"; 
            rpi_board.mmio_base = 0xFE000000UL;
            rpi_board.mmio_base_end = 0xFE3FFFFFUL;
            rpi_board.gpu_io_base = 0x7E000000UL;
            rpi_board.gpu_mem_base = 0xC0000000UL;
            break;
        default:    
            rpi_board.name = "????";
            rpi_board.mmio_base = 0x20000000UL;
            break;
    }
}
