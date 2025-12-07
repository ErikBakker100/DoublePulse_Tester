#include <stdint.h>
#include "include/rpi.h"
#include "include/stdlib.h"
#include "../jtag/include/jtag.h" // for getGpioFunctionName
#include "../uart/include/miniuart.h" // for mu_puts, mu_put_hex32, mu_put_bits32

void get_board_info(void);
void get_cpu_info(void);

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
    get_cpu_info();
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

cpu_state_t rpi_cpu = {0};

void get_cpu_info(void) {
#if defined(__aarch64__)
    rpi_cpu.arch = 64;

    uint64_t daif, el;
    asm volatile ("mrs %0, daif" : "=r"(daif));
    asm volatile ("mrs %0, CurrentEL" : "=r"(el));

    rpi_cpu.level = (el >> 2) & 0x3;
    rpi_cpu.irq_masked = (daif >> 7) & 1;
    rpi_cpu.fiq_masked = (daif >> 6) & 1;
    strncpy(rpi_cpu.mode, "ARM\0", sizeof(rpi_cpu.mode));
#elif defined(__arm__)
    rpi_cpu.arch = 32;
    uint32_t cpsr;
    asm volatile ("mrs %0, cpsr" : "=r"(cpsr));
    rpi_cpu.level = cpsr & 0x1F;             // mode bits
    rpi_cpu.irq_masked = (cpsr >> 7) & 1; // I-bit
    rpi_cpu.fiq_masked = (cpsr >> 6) & 1; // F-bit
    if (cpsr & (1 << 5)) {
    strncpy(rpi_cpu.mode, "Thumb\0", sizeof(rpi_cpu.mode));
    } else {
    strncpy(rpi_cpu.mode, "ARM\0", sizeof(rpi_cpu.mode));
    }
#else
#   error "Unsupported architecture"
#endif
    if (rpi_cpu.arch == 64) {
        switch (rpi_cpu.level) {
            case 0: strncpy(rpi_cpu.level_str, "EL0\0", sizeof(rpi_cpu.level_str)); break;
            case 1: strncpy(rpi_cpu.level_str, "EL1\0", sizeof(rpi_cpu.level_str)); break;
            case 2: strncpy(rpi_cpu.level_str, "EL2\0", sizeof(rpi_cpu.level_str)); break;
            case 3: strncpy(rpi_cpu.level_str, "EL3\0", sizeof(rpi_cpu.level_str)); break;
            default: strncpy(rpi_cpu.level_str, "??\0", sizeof(rpi_cpu.level_str)); break;
        }
    } else {
        switch (rpi_cpu.level) {
            case 0b10000: strncpy(rpi_cpu.level_str, "USR\0", sizeof(rpi_cpu.level_str)); break;
            case 0b10001: strncpy(rpi_cpu.level_str, "FIQ\0", sizeof(rpi_cpu.level_str)); break;
            case 0b10010: strncpy(rpi_cpu.level_str, "IRQ\0", sizeof(rpi_cpu.level_str)); break;
            case 0b10011: strncpy(rpi_cpu.level_str, "SVC\0", sizeof(rpi_cpu.level_str)); break;
            case 0b10111: strncpy(rpi_cpu.level_str, "ABT\0", sizeof(rpi_cpu.level_str)); break;
            case 0b11011: strncpy(rpi_cpu.level_str, "UND\0", sizeof(rpi_cpu.level_str)); break;
            case 0b11111: strncpy(rpi_cpu.level_str, "SYS\0", sizeof(rpi_cpu.level_str)); break;
            default:       strncpy(rpi_cpu.level_str, "??\0", sizeof(rpi_cpu.level_str)); break;
        }
    }
}

void print_board_info() {
    mu_puts("Detected board: ");
    mu_puts(rpi_board.name);
    mu_puts("\n");
    mu_puts("Core frequency (MHz): ");
    mu_put_uint(rpi_board.core_freq_mhz);
    mu_puts("\n");
    mu_puts("Baudrate: ");
    mu_put_uint(rpi_board.baudrate);
    mu_puts("\n");
    mu_puts("SoC Type: ");
    mu_put_hex32(rpi_board.soc_type);
    mu_puts("\n");
    mu_puts("MMIO base address: ");
    // Simple hex print (assuming 32-bit address)
    mu_put_hex32((uintptr_t)rpi_board.mmio_base);
    mu_puts("\n");
    uint8_t pins[] = {22, 23, 24, 25, 26, 27};
    for (uint8_t i = 0; i < sizeof(pins)/sizeof(pins[0]); i++) {
        const char* func_name = getGpioFunctionName(pins[i]);
        mu_puts("GPIO Pin ");
        // Print pin number
        if (pins[i] >= 10) {
            mu_puts((const char[]){'0' + (char)(pins[i] / 10), '\0'});
        }
        mu_puts((const char[]){'0' + (char)(pins[i] % 10), '\0'});
        mu_puts(": Function = ");
        mu_puts(func_name);
        mu_puts(")\n");
  } 
}

void print_cpu_info() {
    get_cpu_info();
    mu_puts(rpi_cpu.arch == 64 ? "AArch64\n" : "AArch32\n");
    mu_puts("  Level: ");
    mu_puts(rpi_cpu.level_str);
    mu_puts("\n");
    mu_puts("  IRQ: ");
    mu_puts(rpi_cpu.irq_masked ? "disabled\n" : "enabled\n");
    mu_puts("  FIQ: ");
    mu_puts(rpi_cpu.fiq_masked ? "disabled\n" : "enabled\n");
    mu_puts("  Mode: ");
    mu_puts(rpi_cpu.mode);
    mu_puts("\n");
}
