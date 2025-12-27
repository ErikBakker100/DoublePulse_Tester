#include "include/core1.h"

unsigned long delay1 = DEFAULT_PULSE_WIDTH1; // holds Pulse Width1 delay
unsigned long delay2 = DEFAULT_INTER_PULSE_DELAY; // holds Inter Pulse width delay
unsigned long delay3 = DEFAULT_PULSE_WIDTH2; // holds Pulse Width2 delay
unsigned long delay4 = DEFAULT_PULSE_INTERVAL; // holds Pulse Interval delay

/*
// Start core1
// voor 32 bit : asm volatile("dsb sy; sev" ::: "memory");
// omdat Core1 in WFE staat (Wait For Event) moet asm code worden uitgevoerd om te activeren. 'dsb' is een Data Synchronization Barrier 
// instructie die ervoor zorgt dat alle voorgaande geheugenoperaties zijn voltooid voordat verder wordt gegaan. 'sev' is de Set Event instructie
// die een event signaleert, waardoor een core die in WFE (Wait For Event) staat, wordt gewekt en verder kan gaan met uitvoeren.
// 'ishst' specificeert dat de DSB-operatie betrekking heeft op alle inner-shareable geheugenlocaties en dat het effect van de DSB-operatie zichtbaar moet zijn
// voor alle inner-shareable caches en buffers voordat de instructie verder gaat.
*/

// Delay loop
#define DELAY(count) do { volatile uint32_t _i = (count); while (_i--) asm volatile("nop"); } while(0)

void start_core1(void) {
    *core_boot(1) = (core_reg_t)(uintptr_t)&core_entry_1;
#if defined(__aarch64__) || defined(__AARCH64__)
    asm volatile ("dsb ishst");
#else
    asm volatile ("dsb");
#endif
    asm volatile ("sev"); // stuur event om core wakker te maken
}

// Entry point for core1
void core_main_1() {
    uint32_t bank = OUTPUT_PIN<32?0:1;
    volatile uint32_t *gpio_on = &GPIO->SET[bank];
    volatile uint32_t *gpio_off = &GPIO->CLR[bank];
    uint32_t mask = OUTPUT_PIN<32?(1u << OUTPUT_PIN):(1u << (OUTPUT_PIN - 32));
    gpio_init_pin(OUTPUT_PIN, GPIO_OUT);
    gpio_clear(OUTPUT_PIN); // Initial state low (inactive)    
    irq_init_core1();

    while (1) {
        *gpio_on = mask;
        DELAY(delay1); // PulseWidth1
        *gpio_off = mask;
        DELAY(delay2); // interPulseDelay
        *gpio_on = mask;
        DELAY(delay3); // PulseWith2
        *gpio_off = mask;
        DELAY(delay4); // Pulseinterval
   }
/*  Alternatief voor instabiele DELAY() functie, gebruik PWM in ns modus
    pwm_ns_init();
    irq_init_core1();

    while (1) {
        pwm_ns_pulse(delay1); // PulseWidth1
        delay_us(delay2 / 1000); // interPulseDelay
        pwm_ns_pulse(delay3); // PulseWith2
        delay_us(delay4 / 1000); // Pulseinterval
   }
   #define PWM_BASE    0x2020C000
    #define PWM_CTL     ((volatile uint32_t *)(PWM_BASE + 0x0))
    #define PWM_RNG1    ((volatile uint32_t *)(PWM_BASE + 0x10))
    #define PWM_DAT1    ((volatile uint32_t *)(PWM_BASE + 0x14))

    #define CM_PWMCTL   ((volatile uint32_t *)(0x20101000 + 0xA0)) // Clock manager
    #define CM_PWMDIV   ((volatile uint32_t *)(0x20101000 + 0xA4))
    #define CM_PWMCLK_CNTL  ((volatile uint32_t *)(0x20101000 + 0xA0))
    #define CM_PWMCLK_DIV   ((volatile uint32_t *)(0x20101000 + 0xA4))

    void pwm_ns_init() {
        // 1. Stop PWM
        *PWM_CTL = 0;
        delay_us(10);

        // 2. Stop clock
        *CM_PWMCTL = 0x5A000000 | (1 << 5); // kill clock
        delay_us(10);

        // 3. Stel PWM klok in: 1 GHz / 1000 = 1 MHz tick (1 tick = 1 ns, voorbeeld)
        *CM_PWMDIV = 0x5A000000 | 1000<<12; // divider
        *CM_PWMCTL = 0x5A000000 | (1 << 4) | 1; // source oscillator + enable

        // 4. Stel PWM range en modus
        *PWM_RNG1 = 1000; // 1000 ticks = 1 µs, 1 tick = 1 ns
        *PWM_DAT1 = 0;     // begin low
        *PWM_CTL = (1 << 0) | (1 << 7); // PWM1 enable, MS mode
    }

    void pwm_ns_pulse(uint32_t width_ns) {
        if (width_ns > *PWM_RNG1) width_ns = *PWM_RNG1;
        *PWM_DAT1 = width_ns;     // pulse high for width_ns ticks
        while (*PWM_DAT1 != 0);   // wacht tot pulse klaar is
    }
*/
}

void mailbox0_core1(uint32_t delay) {
    delay1 = delay;
}

void mailbox1_core1(uint32_t delay) {
    delay2 = delay;
}

void mailbox2_core1(uint32_t delay) {
    delay3 = delay;
}

void mailbox3_core1(uint32_t delay) {
    delay4 = delay;
}
