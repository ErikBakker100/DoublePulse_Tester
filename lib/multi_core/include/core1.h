#pragma once

void core_main_1(void); // Function to be executed by core1 after start
void start_core1(void); // Set entry point for core1
extern void core_entry_1(void); // Entry point defined in startxx.s
