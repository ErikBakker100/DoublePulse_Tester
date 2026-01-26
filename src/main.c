// Double Pulse Generator for Raspberry Pi Zero, Zero 2 or 4B
// Erik Bakker 2025
// Partly used from Teensy 4.0 Signal Generator, Electronics Workshop, Robin O'Reilly

#include "../lib/general/include/config.h"
#include "../lib/boards/include/boards.h"
#include "../lib/general/include/stdlib.h"  // Include standard library for string functions, TO DO check with minuart functions, extract string functions form minuart
#include "../lib/boards/soc/include/gpio.h"
#include "../lib/boards/soc/include/uart.h"
#include "../lib/boards/soc/include/irq.h"
#include "../lib/boards/soc/include/mailbox.h"
#include "../lib/boards/soc/include/timers.h"
#include "../lib/multi_core/include/core1.h"
#include "../lib/json/include/jsmn.h"
#include "../lib/general/include/serial.h"

// Timing Variables
//  _____________                   ____________
// | PulseWidth1 | interPulseDelay | PulseWith2 | Pulseinterval
//                _________________              _______________

static char jsonString[CHAR_BUFFER] = {0};  // Uart buffer for receiving JSON string
// Core0 is reponsible for updating parameters and communicating with the outside
// Core1 is generating the pulses based on the values in 'Intervals[]'

void core_main_0(uint32_t arg0, uint32_t arg1) {
  board->init();                            // check board and set mmio_base pointers.
  board->info();                            // Get board information via mailbox
  start_core1();                            // Start double pulse generator on core1
//  mu_puts("> **************Dual Pulse Generator**************\r\n");
  mu_puts("> Usage: Send JSON string, for e.g {\"pulseWidth1\": 70, \"interPulseDelay\": 30, \"pulseWidth2\": 50, \"pulseInterval\": 500}.\r\n");
  mu_puts("> Using GPIO: ");
  mu_put_uint(OUTPUT_PIN);
  mu_puts(", default values :\r\n");
  mu_puts("> _______________                 ______________\r\n");
  mu_puts("> | pulseWidth1 | interPulseDelay | pulseWith2 | pulseInterval |\r\n");
  mu_puts(">      70       ______ 30 _________     50     ______ 500_______\r\n");

  gpio->init_pin(21, GPIO_OUTPUT, PULL_DOWN);// Set GPIO 21 voor hart beat indication
  timer->set(1, 1000000);                    // Initialize timer 1 for 1 second intervals
  irq->init_core0();                         // Initialize IRQs for core0

  while (1) {
    if (read_json(jsonString, CHAR_BUFFER, (100000))) { // If a character is in the UART buffer, try to get the whole string, or timeout (at 115200 one byte is ~87usec).
      mu_puts("> Received: ");
      mu_puts(jsonString);                  // Print the received JSON string
      mu_puts("\r\n");
      jsmn_parser p;                        // JSON parser
      jsmntok_t t[10];                      // Array of tokens for JSON parsing
      jsmn_init(&p);
      int r = jsmn_parse(&p, (const char *)jsonString, strlen(jsonString), t, sizeof(t) / sizeof(t[0]));
      if (r < 0) {
        mu_puts("> ERR Parsing JSON: ");
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
        continue;                           // Skip to the next iteration of the loop
      }
      // Assume the top-level element is an object
      if (t[0].type != JSMN_OBJECT) {
        mu_puts(">ERR Parsing JSON: The top-level element is not an object.\r\n");
        continue;                           // Wait for next JSON string
      }
      // Loop through all keys in the JSON object
      for (int i = 1; i < r; i++) {
        if (t[i].type == JSMN_STRING && t[i].size == 1) { // Check if the token is a string and has size 1
          char key[30];                     // Buffer to hold the key string
          if (t[i].type != JSMN_STRING) {
            mu_puts(">ERR Parsing JSON: Expected a string\r\n");
            continue;
          }
          if (t[i].end - t[i].start >= sizeof(key)) {
            mu_puts(">ERR Parsing JSON: Key is too long.\r\n");
            continue;                       // Skip to the next iteration of the loop
          }
          strncpy(key, (jsonString + t[i].start), (t[i].end - t[i].start)); // Copy the key string from the JSON string
          key[t[i].end - t[i].start] = '\0'; // Null-terminate the key string
          if (strcmp(key, "pulseWidth1") == 0) {
            Intervals[0] = strtoul(jsonString + t[i + 1].start, NULL, 10); // Convert the value to unsigned long and store it in the array
            mailbox->write(4, 0, Intervals[0]); // Send new value to core1 via mailbox
            mu_puts("> PulseWidth1 now: ");
            mu_put_uint(Intervals[0]);
            mu_puts("\r\n");
          } else if (strcmp(key, "interPulseDelay") == 0) {
            Intervals[1] = strtoul(jsonString + t[i + 1].start, NULL, 10);
            mailbox->write(5, 0, Intervals[1]);
            mu_puts("> InterPulseDelay now: ");
            mu_put_uint(Intervals[1]);
            mu_puts("\r\n");
          } else if (strcmp(key, "pulseWidth2") == 0) {
            Intervals[2] = strtoul(jsonString + t[i + 1].start, NULL, 10);
            mailbox->write(6, 0, Intervals[2]);
            mu_puts("> PulseWidth2 now: ");
            mu_put_uint(Intervals[2]);
            mu_puts("\r\n");
          } else if (strcmp(key, "pulseInterval") == 0) {
            Intervals[3] = strtoul(jsonString + t[i + 1].start, NULL, 10);
            mailbox->write(7, 0, Intervals[3]);
            mu_puts("> PulseInterval now: ");
            mu_put_uint(Intervals[3]);
            mu_puts("\r\n");
          } else {
            mu_puts("> ERR Parsing JSON: Unknown key: ");
            mu_puts(key);
            mu_puts("\r\n");
          }
        }
      }
    }
  }
}
