// Dual Pulse Generator for Raspberry Pi Zero, Zero 2 or 4B
// Erik Bakker 2025
// Partly used from Teensy 4.0 Signal Generator, Electronics Workshop, Robin O'Reilly

#include "..\lib\general\rpi.h"
#include "../lib/general/include/stdlib.h" // Include standard library for string functions>
#include "../lib/gpio/include/gpio.h"
#include "../lib/uart/include/uartmini.h"
#include "../lib/json/include/jsmn.h"

#if defined (raspberry_zero_2w)
    #warning "using settings for raspberry_zero_2w"
#elif defined (raspberry_zero_1w)
    #warning "using settings for raspberry_zero_1w"
#elif defined (raspberry_pi_4b)
    #warning "using settings for raspberry_pi_4b"
#endif


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

// put function declarations here:
void DoublePulseControl(void); // Function to control the double pulse generation

void delay(unsigned int count) {
    for (volatile unsigned int i = 0; i < count; i++) {
        asm volatile("nop");
    }
}

void main() {
  char jsonString[CHAR_BUFFER]; // Uart buffer for receiving JSON string
  jsmn_parser p; // JSON parser
  jsmntok_t t[CHAR_BUFFER]; // Array of tokens for JSON parsing
  jsmn_init(&p);
  uart_init(); // Initialize UART
  gpio_init(OUTPUT_PIN, GPIO_OUT);
  gpio_clear(OUTPUT_PIN); // Initial state low (inactive)
  delay(300); // Give serial port time to open
  uart_puts("**************Dual Pulse Generator**************\r\n");
  uart_puts("> Usage: Send JSON string, for e.g {\"pulseWidth1\": 70, \"interPulseDelay\": 30, \"pulseWidth2\": 50, \"pulseInterval\": 500}.\r\n");
  uart_puts("> Using output port: ");
  uart_put_uint(OUTPUT_PIN);
  uart_puts(", default values :\r\n");
  uart_puts("> _____________                   ____________\r\n");
  uart_puts(">| pulseWidth1 | interPulseDelay | pulseWith2 | pulseInterval |\r\n");
  uart_puts(">      70       _____ 30 ________      50      _____ 500______\r\n");
  // clear receive buffer
  while ((AUX_MU_LSR_REG) & 0x01) {
    (void)(AUX_MU_IO_REG); // read and discard
  }

  while (1) {
    if (uart_read_line_blocking(jsonString, CHAR_BUFFER, 10000)) { // If a character is in the UART buffer, try to get the whole string until recieving /n, or timeout.
      uart_puts("> Parsing JSON string... : ");
      uart_puts(jsonString); // Print the received JSON string
      uart_puts("\r\n");
      jsmn_init(&p);
      int r = jsmn_parse(&p, jsonString, strlen(jsonString), t, sizeof(t) / sizeof(t[0]));
      if (r < 0) {
        uart_puts(">ERR Parsing JSON: ");
        switch (r) {
          case JSMN_ERROR_NOMEM:
            uart_puts("Not enough tokens were provided.\r\n");
            continue;
          case JSMN_ERROR_INVAL:
            uart_puts("Invalid character in JSON string.\r\n");
            continue;
          case JSMN_ERROR_PART:
            uart_puts("The string is not a full JSON packet.\r\n");
            continue;
          default:
            uart_puts("Unknown error.\r\n");
            continue;
        }
        continue; // Skip to the next iteration of the loop
      }
      // Assume the top-level element is an object
      if (t[0].type != JSMN_OBJECT) {
        uart_puts(">ERR Parsing JSON: The top-level element is not an object.\r\n");
        continue; // Wait for next JSON string
      }
      // Loop through all keys in the JSON object
      for (int i = 1; i < r; i++) {
        if (t[i].type == JSMN_STRING && t[i].size == 1) { // Check if the token is a string and has size 1
          char key[30]; // Buffer to hold the key string
          if (t[i].end - t[i].start >= sizeof(key)) {
            uart_puts(">ERR Parsing JSON: Key is too long.\r\n");
            continue; // Skip to the next iteration of the loop
          }
          strncpy(key, jsonString + t[i].start, t[i].end - t[i].start); // Copy the key string from the JSON string
          key[t[i].end - t[i].start] = '\0'; // Null-terminate the string
          if (strcmp(key, "pulseWidth1") == 0) {
            Intervals[0] = strtoul(jsonString + t[i + 1].start, NULL, 10); // Convert the value to unsigned long and store it in the array
          } else if (strcmp(key, "interPulseDelay") == 0) {
            Intervals[1] = strtoul(jsonString + t[i + 1].start, NULL, 10);
          } else if (strcmp(key, "pulseWidth2") == 0) {
            Intervals[2] = strtoul(jsonString + t[i + 1].start, NULL, 10);
          } else if (strcmp(key, "pulseInterval") == 0) {
            Intervals[3] = strtoul(jsonString + t[i + 1].start, NULL, 10);
          } else {
            uart_puts(">ERR Parsing JSON: ");
            uart_puts(key);
            uart_puts(" not in use.\r\n");
          }
        }
      }
      uart_puts("> Intervals set to: \r\n");
      uart_puts("> pulseWidth1: ");
      uart_put_uint(Intervals[0]);
      uart_puts("\r\n> interPulseDelay: ");
      uart_put_uint(Intervals[1]);
      uart_puts("\r\n> pulseWidth2: ");
      uart_put_uint(Intervals[2]);
      uart_puts("\r\n> pulseInterval: ");
      uart_put_uint(Intervals[3]);
      uart_puts("\r\n> Starting new pulse generation...\r\n");
    }
    DoublePulseControl(); // Call the function to generate double pulse
  }
  uart_puts("Program ended\r\n");
}

void DoublePulseControl() {
    gpio_set(OUTPUT_PIN);
//    gpio_write(OUTPUT_PIN, HIGH); 
    delay(Intervals[0]); // PulseWidth1
    gpio_clear(OUTPUT_PIN);
//    gpio_write(OUTPUT_PIN, LOW);
    delay(Intervals[1]); // interPulseDelay
    gpio_set(OUTPUT_PIN);
//    gpio_write(OUTPUT_PIN, HIGH);
    delay(Intervals[2]); // PulseWith2
    gpio_clear(OUTPUT_PIN);
//    gpio_write(OUTPUT_PIN, LOW);
    delay(Intervals[3]); // Pulseinterval
}
