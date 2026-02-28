// Double Pulse Generator for Raspberry Pi Zero, Zero 2, 4B, .....
// Erik Bakker 2025
// Partly used from Teensy 4.0 Signal Generator, Electronics Workshop, Robin O'Reilly

#include "../lib/general/include/config.h"
#include "../lib/boards/include/boards.h"
#include "../lib/general/include/stdlib.h"
#include "../lib/boards/soc/include/gpio.h"
#include "../lib/boards/soc/include/uart.h"
#include "../lib/boards/soc/include/irq.h"
#include "../lib/boards/soc/include/mailbox.h"
#include "../lib/boards/soc/include/timers.h"
#include "../lib/multi_core/include/core1.h"
#include "../lib/json/include/jsmn.h"
#include "../lib/general/include/serial.h"
#include "../lib/general/include/date_time.h"

// Timing Variables
//  _____________                   ____________
// | PulseWidth1 | interPulseDelay | PulseWith2 | Pulseinterval
//                _________________              _______________

static char jsonString[CHAR_BUFFER] = {0};  // Uart buffer for receiving JSON string
// Core0 is reponsible for updating parameters and communicating with the outside
// Core1 is generating the pulses based on the values in 'Intervals[]'
void core_main_0(uint32_t arg0, uint32_t arg1) {
  cpu_init();                               // get CPU information, and sest base addresses for peripherals
  date_time_t date_time;
  if (!board_init(&board)) return;          // read board information, if false we can not continue.
  uart->set(BAUDRATE);
  mu_puts("> ************** Dual Pulse Generator **************\r\n");
  mu_puts("> Usage: Send JSON string, for e.g {\"pulseWidth1\": 70, \"interPulseDelay\": 30, \"pulseWidth2\": 50, \"pulseInterval\": 500}.\r\n");
  mu_puts("> Using GPIO: ");
  mu_put_uint(OUTPUT_PIN);
  mu_puts(", default values :\r\n");
  mu_puts("> _______________                 ______________\r\n");
  mu_puts("> | pulseWidth1 | interPulseDelay | pulseWith2 | pulseInterval |\r\n");
  mu_puts(">      70       ______ 30 _________     50     ______ 500_______\r\n");
  mu_puts(">\r\n ******************* Used Board *******************\r\n");
  mu_puts("> Model:                   ");
  mu_puts(board.description);
  mu_puts(", ");
  mu_puts(board.memory_size);
  mu_puts(", made by: ");
  mu_puts(board.manufacturer);
  mu_puts(", ");
  if (board.rev_scheme == 0) {
    mu_puts("old revision scheme");
  } else if (board.rev_scheme == 1) {
    mu_puts("new revision scheme");
  } else {
    mu_puts("unknown revision scheme");
  }
  mu_puts("\r\n> SOC:                     ");
  mu_puts(soc.name);
  mu_puts("\r\n> CPU core:                ");
  mu_puts(cpu.part->name);
  mu_puts(", Architecture: ");
  mu_puts(cpu.architecture->name);
  mu_puts(", made by: ");
  mu_puts(cpu.implementer->name);
  mu_puts(", Partnumber: ");
  mu_put_hex16(cpu.part->partnum, true);
  mu_puts(", Revision: ");
  mu_puts(cpu.rNpM);
  mu_puts("\r\n> Using Addresses:         ");
  mu_put_hex32(cpu.part->mmio_base, true);
  mu_puts(", ");
  mu_put_hex32(soc.base_end, true);
  mu_puts(", ");
  mu_put_hex32(soc.gpu_io_base, true);
  mu_puts(", ");
  mu_put_hex32(soc.gpu_mem_base, true);
  mu_puts(", ");
  mu_put_hex32(soc.local_periph_base, true);
  format_firmware_date_time(board.firmware_date, &date_time);
  mu_puts("\r\n> VC firmware date:        ");
  mu_put_uint(date_time.day);
  mu_puts(" ");
  mu_put_uint(date_time.month);
  mu_puts(" ");
  mu_put_uint(date_time.year);
  mu_puts(" ");
  mu_put_uint(date_time.hour);
  mu_puts(":");
  mu_put_uint(date_time.minute);
  mu_puts(":");
  mu_put_uint(date_time.second);
  mu_puts(", board id: ");
  mu_put_hex32(board.revision_raw_value, true);
  mu_puts("\r\n> Serial nr.               ");
  mu_put_hex32(board.serial, true);
  mu_puts("\r\n> Amount of program RAM    ");
  mu_put_uint((board.arm_memory_size / (1024 * 1024)));
  mu_puts(", starting at address:  ");
  mu_put_hex32(board.arm_memory_base, true);
  mu_puts("\r\n> Amount of GPU RAM        ");
  mu_put_uint((board.gpu_memory_size / (1024 * 1024)));
  mu_puts(", starting at address:  ");
  mu_put_hex32(board.gpu_memory_base, true);
  mu_puts("\r\n> ARM Current clock rate:  ");
  mu_put_uint(board.clock_rates[ARM_id]);
  mu_puts("\r\n> CORE Current clock rate: ");
  mu_put_uint(board.clock_rates[CORE_id]);  
  mu_puts("\r\n> UART Current clock rate: ");
  mu_put_uint(board.clock_rates[UART_id]);
  mu_puts("\r\n> ARM max clock rate:      ");
  mu_put_uint(board.max_clock_rates[ARM_id]);
  mu_puts("\r\n> CORE max clock rate:     ");
  mu_put_uint(board.max_clock_rates[CORE_id]);  
  mu_puts("\r\n> UART max clock rate:     ");
  mu_put_uint(board.max_clock_rates[UART_id]);
  mu_puts("\r\n> MAC address:             ");
  for (int32_t i = 0; i < 6; i++) {
    mu_put_hex8(board.mac_address[i], false);
    if (i < 5) mu_puts(":");
  }
  mu_puts("\r\n> Chip temperature:        ");
  mu_put_uint(board.soc_temperature);
  mu_puts(" milli degrees Celsius\r\n");
  mu_puts("> ****************************************************\r\n");

#ifdef DUALCORE
  start_core1();                            // Start double pulse generator on core1
  mu_puts("> Running in dual core mode.\r\n");
#else
  mu_puts("> Running in single core mode.\r\n");
#endif
  gpio->init_pin(OUTPUT_PIN, GPIO_OUTPUT, PULL_DOWN); // Initialize output pin for doublepulse generation
  gpio->init_pin(STATUS_PIN, GPIO_OUTPUT, PULL_DOWN); // Set GPIO 21 voor hart beat indication
  timer->set(1, BLINK_TIMER);               // Initialize timer 1 for .1 second intervals
  irq->init_core0();                        // Initialize IRQs for core0

  while (1) {
#ifndef DUALCORE
    doublepulse_generator(Intervals[0], Intervals[1], Intervals[2], Intervals[3]); // In single core mode, generate the double pulse pattern in the main loop
#endif
    if (read_json(jsonString, CHAR_BUFFER, (100000))) { // If a character is in the UART buffer, try to get the whole string, or timeout (at 115200 one byte is ~87usec).
      mu_puts("> Received: ");
      mu_puts(jsonString);                  // Print the received JSON string
      mu_puts("\r\n");
      jsmn_parser p;                        // JSON parser
      static jsmntok_t t[128];              // Array of tokens for JSON parsing
      jsmn_init(&p);
      int32_t r = jsmn_parse(&p, (const char *)jsonString, strlen(jsonString), t, sizeof(t) / sizeof(t[0]));
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
        mu_puts("> ERR Parsing JSON: The top-level element is not an object.\r\n");
        continue;                           // Wait for next JSON string
      }
      // Loop through all keys in the JSON object
      bool updated = false;             // remeber if we have updated a value, so we can send a signal to core1 to update the intervals array if needed
      char key[32];                     // Buffer to hold the key string
      for (int32_t i = 1; i < r; i++) {
        if (t[i].type == JSMN_STRING && t[i].size == 1) { // Check if the token is a string and has size 1
          if (t[i].type != JSMN_STRING) {
            mu_puts("> ERR Parsing JSON: Expected a string\r\n");
            continue;
          }
          if (t[i].end - t[i].start >= sizeof(key)) {
            mu_puts(">ERR Parsing JSON: Key is too long.\r\n");
            continue;                       // Skip to the next iteration of the loop
          }
          strncpy(key, (jsonString + t[i].start), (t[i].end - t[i].start)); // Copy the key string from the JSON string
          key[t[i].end - t[i].start] = '\0'; // Null-terminate the key string

          static const char *keys[] = {"pulseWidth1", "interPulseDelay", "pulseWidth2", "pulseInterval"};
          for (int32_t j = 0; j < 4; j++) {
            if (strcmp(key, keys[j]) == 0) {
              Intervals[j] = strtoul(jsonString + t[i + 1].start, NULL, 10);
              mu_puts("> ");
              mu_puts(key);
              mu_puts(" now: ");
              mu_put_uint(Intervals[j]);
              mu_puts("\r\n");
              updated = true;
              break;
            }
          }
        } 
      }
      if (updated) {
        mailbox->write(0, 1, 0x1);      // Send a signal to core1 that the intervals have been updated
      } else {
        mu_puts("> ERR Parsing JSON: Unknown key: ");
        mu_puts(key);
        mu_puts("\r\n");
      }
    } 
  } 
}
