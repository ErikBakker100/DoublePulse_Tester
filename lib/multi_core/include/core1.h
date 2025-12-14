#pragma once
#include "..\..\general\include\config.h"
#include "..\..\general\include\rpi.h"
#include "..\..\uart\include\miniuart.h"
#include "..\..\multi_core\include\multi_core.h"
#include "..\..\general\include\stdlib.h"
#include "..\..\general\include\BCM2835.h"

void core_main_1(void); // Function to be executed by core1 after start
void start_core1(void); // Set entry point for core1
extern void core_entry_1(void); // Entry point defined in startxx.s
void irq_handler_core1(void); // IRQ handler for core1
void irq_init_core1(void); // Initialize IRQs for core1