#include "include/core1.h"
#include "../multi_core/include/multi_core.h"
#include "../general/include/stdlib.h"
#include "../general/include/config.h"
#include "../boards/soc/include/gpio.h"
#include "../boards/soc/include/interrupts.h"
#include "../boards/soc/cpu/include/cpu.h"

volatile uint32_t delay1 = DEFAULT_PULSE_WIDTH1; // holds Pulse Width1 delay
volatile uint32_t delay2 = DEFAULT_INTER_PULSE_DELAY; // holds Inter Pulse width delay
volatile uint32_t delay3 = DEFAULT_PULSE_WIDTH2; // holds Pulse Width2 delay
volatile uint32_t delay4 = DEFAULT_PULSE_INTERVAL; // holds Pulse Interval delay

/*
// Start core
// voor 32 bit : asm volatile("dsb sy; sev" ::: "memory");
// omdat Core1 in WFE staat (Wait For Event) moet asm code worden uitgevoerd om te activeren. 'dsb' is een Data Synchronization Barrier 
// instructie die ervoor zorgt dat alle voorgaande geheugenoperaties zijn voltooid voordat verder wordt gegaan. 'sev' is de Set Event instructie
// die een event signaleert, waardoor een core die in WFE (Wait For Event) staat, wordt gewekt en verder kan gaan met uitvoeren.
// 'ishst' specificeert dat de DSB-operatie betrekking heeft op alle inner-shareable geheugenlocaties en dat het effect van de DSB-operatie zichtbaar moet zijn
// voor alle inner-shareable caches en buffers voordat de instructie verder gaat.
*/

#ifdef DUALCORE
void start_core(uint8_t core_nr) {
    *core_boot(board.soc.data.local_periph_base, core_nr) = (core_reg_t)(uintptr_t)&core_entry_1;
    dsb();
    sev();
}
#endif

void doublepulse_generator(uint32_t d1, uint32_t d2, uint32_t d3, uint32_t d4) {
    gpio->set(OUTPUT_PIN);
    DELAY(d1); // PulseWidth1
    gpio->clear(OUTPUT_PIN);
    DELAY(d2); // interPulseDelay
    gpio->set(OUTPUT_PIN);
    DELAY(d3); // PulseWith2
    gpio->clear(OUTPUT_PIN);
    DELAY(d4); // Pulseinterval
}

// Entry point for core1
void core_main_1() {
    interrupts->init_core1();                      // Initialize IRQs for core1
    while (1) {
    gpio->set(TRIGGER_PIN);
    gpio->set(OUTPUT_PIN);
    DELAY(delay1); // PulseWidth1
    gpio->clear(OUTPUT_PIN);
    DELAY(delay2); // interPulseDelay
    gpio->set(OUTPUT_PIN);
    DELAY(delay3); // PulseWith2
    gpio->clear(OUTPUT_PIN);
    gpio->clear(TRIGGER_PIN);
    DELAY(delay4); // Pulseinterval
    }
}

void mailbox0(uint32_t data) {
    dmb();
    delay1 = Intervals[0];
    delay2 = Intervals[1];
    delay3 = Intervals[2];
    delay4 = Intervals[3];
    dmb();
}
