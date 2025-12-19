// Double Pulse Generator for Raspberry Pi Zero, Zero 2 or 4B
// Erik Bakker 2025
// Partly used from Teensy 4.0 Signal Generator, Electronics Workshop, Robin O'Reilly

#include "../lib/general/include/config.h"
#include "../lib/general/include/rpi.h"
#include "../lib/general/include/stdlib.h" // Include standard library for string functions, TO DO check with minuart functions, extract string functions form minuart
#include "../lib/gpio/include/gpio.h"
#include "../lib/uart/include/miniuart.h"
#include "../lib/json/include/jsmn.h"
#include "../lib/multi_core/include/core1.h"
#include "../lib/general/include/BCM2835.h"
#include "../lib/general/include/BCM2836.h"

// Timing Variables
//  _____________                   ____________
// | PulseWidth1 | interPulseDelay | PulseWith2 | Pulseinterval
//                _________________              _______________

// Set default 
#define PW1 70 // Pulse Width, starts with 0,5 usec, 80nsec resolution, so 
#define IPD 30 // Inter Pulse width, starts with 0,4 usec
#define PW2 50 // Pulse Width 2, starts with 0,5 usec
#define PI 500 // Pulse Interval, starts with 1,2usec
unsigned long Intervals[4]={PW1, IPD, PW2, PI}; // Array to hold the intervals

// Core0 is reponsible for updating parameters and communicating with the outside
// Core1 is generating the pulses based on the values in 'Intervals[]'

void core_main_0(uint32_t arg0, uint32_t arg1) {
  board_init(); // detecteer bord en stel mmio_base pointers in
  gpio_init(); // Initialize GPIO
  mu_init(); // Initialize UART
  start_core1();
  mu_puts("**************Dual Pulse Generator**************\r\n");
  mu_puts("> Usage: Send JSON string, for e.g {\"pulseWidth1\": 70, \"interPulseDelay\": 30, \"pulseWidth2\": 50, \"pulseInterval\": 500}.\r\n");
  mu_puts("> Using GPIO: ");
  mu_put_uint(OUTPUT_PIN);
  mu_puts(", default values :\r\n");
  mu_puts("> _____________                   ____________\r\n");
  mu_puts(">| pulseWidth1 | interPulseDelay | pulseWith2 | pulseInterval |\r\n");
  mu_puts(">      70       _____ 30 ________      50      _____ 500______\r\n");
  uart_transmitter_loop();
  // Tell core1 the default values via IRQ's
  INT_ARM_LOCAL_REGS->MBOX_SET04_REG = Intervals[0];
  INT_ARM_LOCAL_REGS->MBOX_SET05_REG = Intervals[1];
  INT_ARM_LOCAL_REGS->MBOX_SET06_REG = Intervals[2];
  INT_ARM_LOCAL_REGS->MBOX_SET07_REG = Intervals[3];

  while (1) {
    uart_transmitter_loop();
    char jsonString[CHAR_BUFFER]; // Uart buffer for receiving JSON string
    if (mu_read_line(jsonString, CHAR_BUFFER, 100000)) { // If a character is in the UART buffer, try to get the whole string, or timeout.
      mu_puts("> Parsing JSON string... : ");
      mu_puts(jsonString); // Print the received JSON string
      mu_puts("\r\n");
      jsmn_parser p; // JSON parser
      jsmntok_t t[CHAR_BUFFER]; // Array of tokens for JSON parsing
      jsmn_init(&p);
      int r = jsmn_parse(&p, (const char *)jsonString, strlen(jsonString), t, sizeof(t) / sizeof(t[0]));
      mu_put_uint(r);
      mu_puts(" tokens found.\r\n");
      if (r < 0) {
        mu_puts(">ERR Parsing JSON: ");
        switch (r) {
          case JSMN_ERROR_NOMEM:
            mu_puts("Not enough tokens were provided.\r\n");
            continue;
          case JSMN_ERROR_INVAL:
            mu_puts("Invalid character in JSON string.\r\n");
            continue;
          case JSMN_ERROR_PART:
            mu_puts("The string is not a full JSON packet.\r\n");
            continue;
          default:
            mu_puts("Unknown error.\r\n");
            continue;
        }
        continue; // Skip to the next iteration of the loop
      }
      // Assume the top-level element is an object
      if (t[0].type != JSMN_OBJECT) {
        mu_puts(">ERR Parsing JSON: The top-level element is not an object.\r\n");
        continue; // Wait for next JSON string
      }
      // Loop through all keys in the JSON object
      for (int i = 1; i < r; i+=2) {
        mu_puts("> Processing token ");;
        mu_put_uint(i);
        mu_puts(": ");
        mu_puts(jsonString + t[i].start);
        mu_puts(" (size ");
        mu_put_uint(t[i].size);
        my_puts(") type: ");
        mu_put_uint(t[i].type);
        mu_puts("\r\n");
        if (t[i].type == JSMN_STRING && t[i].size == 1) { // Check if the token is a string and has size 1
          char key[30]; // Buffer to hold the key string
          int len = t[i].end - t[i].start;
          if (t[i].type != JSMN_STRING) {
            mu_puts(">ERR Parsing JSON: Expected a string\r\n");
            break;
          }
          if (len >= sizeof(key)) {
            mu_puts(">ERR Parsing JSON: Key is too long.\r\n");
            continue; // Skip to the next iteration of the loop
          }
          strncpy(key, jsonString + t[i].start, len); // Copy the key string from the JSON string
          key[len] = '\0';
          if (strcmp(key, "pulseWidth1") == 0) {
            Intervals[0] = strtoul(jsonString + t[i + 1].start, NULL, 10); // Convert the value to unsigned long and store it in the array
            INT_ARM_LOCAL_REGS->MBOX_SET04_REG = Intervals[0];
          } else if (strcmp(key, "interPulseDelay") == 0) {
            Intervals[1] = strtoul(jsonString + t[i + 1].start, NULL, 10);
            INT_ARM_LOCAL_REGS->MBOX_SET05_REG = Intervals[1];
          } else if (strcmp(key, "pulseWidth2") == 0) {
            Intervals[2] = strtoul(jsonString + t[i + 1].start, NULL, 10);
            INT_ARM_LOCAL_REGS->MBOX_SET06_REG = Intervals[2];
          } else if (strcmp(key, "pulseInterval") == 0) {
            Intervals[3] = strtoul(jsonString + t[i + 1].start, NULL, 10);
            INT_ARM_LOCAL_REGS->MBOX_SET07_REG = Intervals[3];
          } else {
            mu_puts(">ERR Parsing JSON: ");
            mu_puts(key);
            mu_puts(" not in use.\r\n");
          }
        }
      }
      mu_puts("> Intervals set to: \r\n");
      mu_puts("> pulseWidth1: ");
      mu_put_uint(Intervals[0]);
      mu_puts("\r\n> interPulseDelay: ");
      mu_put_uint(Intervals[1]);
      mu_puts("\r\n> pulseWidth2: ");
      mu_put_uint(Intervals[2]);
      mu_puts("\r\n> pulseInterval: ");
      mu_put_uint(Intervals[3]);
      mu_puts("\r\n> Starting new pulse generation...\r\n");
    }
  }
}

void irq_handler_core0(void){} // IRQ handler for core0
