#pragma once
#include "..\..\general\include\rpi.h"
#include "..\..\uart\include\miniuart.h"

extern volatile uint8_t core_status[4];

void irq_handler_local_timer_core0(void);

