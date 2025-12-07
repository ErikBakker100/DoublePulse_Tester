#pragma once
#include <stdint.h>

#ifndef CORE_FREQ
    #define CORE_FREQ 250
#endif

#ifndef BAUDRATE
    #define BAUDRATE 115200
#endif

typedef struct {
    const char *name;
    uintptr_t mmio_base;
    int32_t baudrate;
    int16_t core_freq_mhz;
}rpi_board_info_t;

extern rpi_board_info_t rpi_board;
extern volatile uint32_t core_status[4];

void findmmio_base();

void delay(int count);
