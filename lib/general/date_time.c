#include <stddef.h>
#include "include/date_time.h"

date_time_t date_time;

void format_firmware_time(uint32_t timestamp, date_time_t *dt) {
    dt->second = timestamp % 60;
    dt->minute = (timestamp / 60) % 60;
    dt->hour   = (timestamp / 3600) % 24;
}

void format_firmware_date(uint32_t timestamp, date_time_t *dt) {
    // 1. Haal het aantal dagen uit de timestamp
    uint32_t total_days = timestamp / 86400; // 86400 seconden in een dag

    // 2. Bereken het jaar (beginnend bij 1970)
    dt->year = 1970 + (total_days / 365);
    dt->day = total_days % 365;

    // 3. Bereken de maand en de dag
    uint8_t month_days[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    
    dt->month = 0;
    while (dt->day >= month_days[dt->month]) {
        dt->day -= month_days[dt->month];
        dt->month++;
        if (dt->month >= 12) break;
    }
    dt->month += 1; // Maanden zijn 1-12
    dt->day += 1; // Dagen zijn 1-31
}

void format_firmware_date_time(uint32_t timestamp, date_time_t *dt) {
    if (dt == NULL) return;
    format_firmware_time(timestamp, dt);
    format_firmware_date(timestamp, dt);
}