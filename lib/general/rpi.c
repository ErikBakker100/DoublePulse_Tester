#include <stdint.h>
#include "include/rpi.h"

rpi_board_info_t rpi_board = {
    .baudrate = BAUDRATE,
    .core_freq_mhz = CORE_FREQ
};

// gedeeld statusblok, in .bss of .data
volatile uint32_t core_status[4] = {0}; 
// core_status[n] == 1 → core is gestart

void findmmio_base() {
    // The base address of the MMIO registers depends on the Raspberry Pi version, lets find out which board we are running on
    uint32_t reg;

    /* read the system register */
#if defined(__aarch64__) || defined(__AARCH64__)
    asm volatile ("mrs %x0, midr_el1" : "=r" (reg));
#else
    asm volatile ("mrc p15,0,%0,c0,c0,0" : "=r" (reg));
#endif

    /* get the PartNum, detect board and MMIO base address */
    switch ((reg >> 4) & 0xFFF) {
        case 0xB76: rpi_board.name = "Rpi1"; rpi_board.mmio_base = 0x20000000UL; break;
        case 0xC07: rpi_board.name = "Rpi2"; rpi_board.mmio_base = 0x3F000000UL; break;
        case 0xD03: rpi_board.name = "Rpi3"; rpi_board.mmio_base = 0x3F000000UL; break;
        case 0xD08: rpi_board.name = "Rpi4"; rpi_board.mmio_base = 0xFE000000UL; break;
        default:    rpi_board.name = "????"; rpi_board.mmio_base = 0x20000000UL; break;
    }


}

// Delay loop
void delay(int count) {
    while (count--) asm volatile("nop");
}