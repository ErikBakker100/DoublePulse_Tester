#pragma once
#include "../../general/include/config.h"
#include "../../general/include/rpi.h"
#include "../../uart/include/miniuart.h"
#include "../../multi_core/include/multi_core.h"
#include "../../general/include/stdlib.h"
#include "../../general/include/BCM2835.h"
#include "../../general/include/config.h"
#include "../../gpio/include/gpio.h"
#include "../../irq/include/irq.h"

void core_main_1(void); // Function to be executed by core1 after start
void start_core1(void); // Set entry point for core1
extern void core_entry_1(void); // Entry point defined in startxx.s
void mailbox0_core1(uint32_t);
void mailbox1_core1(uint32_t);
void mailbox2_core1(uint32_t);
void mailbox3_core1(uint32_t);