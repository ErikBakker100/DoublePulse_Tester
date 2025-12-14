#include "include/miniuart.h"
#include "../gpio/include/gpio.h"

volatile ring_t uart_ring __attribute__((aligned(64)));
void ring_push_byte(uint8_t b);
bool ring_pop_byte(uint8_t *out);

// Delay loop
volatile static void delay(int count) {
    while (count--) asm volatile("nop");
}

void mu_init() {
    // Disable the UART before configuring it
    MU->ENABLES = 0;
    // Enable Mini UART (bit 0)
    MU->ENABLES |= 1;

    MU->MU_CNTL_REG = 0;           // Disable TX/RX
    MU->MU_LCR_REG = 3;            // 8-bit mode
    MU->MU_MCR_REG = 0;            // No flow control
    MU->MU_IER_REG = 0;            // Disable interrupts
    MU->MU_IIR_REG = 0xC1;         // enable FIFO, clear
    MU->MU_BAUD_REG = (((rpi_board.core_freq_mhz * 1000000) + (4 * rpi_board.baudrate)) / (8 * rpi_board.baudrate)) - 1;
    // Set GPIO 14 and 15 to ALT5 (Mini UART)
    unsigned int ra = GPIO->FSEL[1];
    ra &= ~((7 << 12) | (7 << 15));   // Clear FSEL14/15
    ra |= (2 << 12) | (2 << 15);      // ALT5
    GPIO->FSEL[1] = ra;

    // Disable pull up/down for pins 14 and 15
    GPIO->PUD = 0;
    delay(150);
    GPIO->PUDCLK[0] = (1 << 14) | (1 << 15);
    delay(150);
    GPIO->PUDCLK[0] = 0;
    MU->MU_CNTL_REG = 3; // Enable TX and RX
	
	    // clear receive buffer
    while ((MU->MU_LSR_REG) & 0x01) {
        (void)(MU->MU_IO_REG); // read and discard
    }    
}

void mu_putb(uint8_t c) {
    while (!(MU->MU_LSR_REG & 0x30));
    MU->MU_IO_REG = c;
}

void mu_putc(uint8_t c) {
    ring_push_byte(c);
}

void mu_puts(const char *s) {
    while (*s) {
        mu_putc((uint8_t)*s++);
    }
}

uint8_t mu_getc(void) {
    // Wacht tot er data beschikbaar is in de RX FIFO (bit 0 = data ready)
    while (!(MU->MU_LSR_REG & 0x01));
    // Lees de byte (laagste 8 bits)
    return (uint8_t)(MU->MU_IO_REG & 0xFF);
}

void mu_put_uint(uint32_t n) {
    uint8_t buf[10];
    int i = 0;

    if (n == 0) {
        mu_putc('0');
        return;
    }

    while (n > 0 && i < sizeof(buf) ) {
        buf[i++] = '0' + (n % 10);
        n /= 10;
    }

    // getal staat nu omgekeerd in buf[]
    while (i--) {
        mu_putc(buf[i]);
    }
}

int16_t mu_try_recv(void) {
    if (MU->MU_LSR_REG & 0x01) {
        return (uint16_t)MU->MU_IO_REG & 0xFF;
    } else {
        return -1; // Geen data beschikbaar
    }
}

uint16_t mu_read_line(char *buf, uint16_t maxlen, uint32_t timeout) {
    int i = 0, c;
    unsigned int wait = 0;
    if(!(MU->MU_LSR_REG & 0x01)) return i; // No data to process
    while (i < maxlen - 1) {
        c = mu_try_recv();
        if (c == -1) {
            if (wait++ >= timeout) break;
        } else {
            buf[i++] = (char)c;
            if (c == '\n' || c == '\r') break;
            wait = 0;
        }
    }
    buf[i] = '\0';
    return i;
}

void mu_put_hex32(uint32_t value) {
    mu_putc('0');
    mu_putc('x');
    for (int i = 28; i >= 0; i -= 4) {
        uint8_t nibble = (value >> i) & 0xF;
        char c = (nibble < 10) ? ('0' + nibble) : ('A' + nibble - 10);
        mu_putc(c);
    }
}

void mu_put_hex64(uint64_t value) {
    mu_putc('0');
    mu_putc('x');
    for (int i = 60; i >= 0; i -= 4) {
        uint8_t nibble = (value >> i) & 0xF;
        char c = (nibble < 10) ? ('0' + nibble) : ('A' + nibble - 10);
        mu_putc(c);
    }
}

void mu_put_bits32(uint32_t value) {
    for (int i = 31; i >= 0; i--) {
        mu_putc((value & (1u << i)) ? '1' : '0');
        if (i % 8 == 0 && i != 0) mu_putc(' ');  // optionele spatie per byte
    }
}

void mu_put_bits64(uint64_t value) {
    for (int i = 63; i >= 0; i--) {
        mu_putc((value & (1ul << i)) ? '1' : '0');
        if (i % 8 == 0 && i != 0) mu_putc(' ');  // optionele spatie per byte
    }
}

void ring_push_byte(uint8_t b)
{
    uint32_t head, next, tail;
    for (;;) {
        head = __atomic_load_n(&uart_ring.head, __ATOMIC_RELAXED);
        next = (head + 1) & RING_MASK;
        tail = __atomic_load_n(&uart_ring.tail, __ATOMIC_ACQUIRE);

        if (next != tail) break;
        asm volatile("nop");
    }
    uart_ring.buf[head] = b;
    __atomic_thread_fence(__ATOMIC_RELEASE);
    __atomic_store_n(&uart_ring.head, next, __ATOMIC_RELAXED);
}

bool ring_pop_byte(uint8_t *out)
{
    uint32_t tail = __atomic_load_n(&uart_ring.tail, __ATOMIC_RELAXED);
    uint32_t head = __atomic_load_n(&uart_ring.head, __ATOMIC_ACQUIRE);

    if (tail == head)
        return false;

    *out = uart_ring.buf[tail];

    __atomic_thread_fence(__ATOMIC_RELEASE);
    __atomic_store_n(&uart_ring.tail, (tail + 1) & RING_MASK, __ATOMIC_RELAXED);

    return true;
}

void uart_transmitter_loop(void) {
    uint8_t c;
    while(ring_pop_byte(&c)){
        mu_putb(c); /* laag-niveau write naar mini-UART */
    }
}
