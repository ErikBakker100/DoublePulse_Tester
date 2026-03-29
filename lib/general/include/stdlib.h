#pragma once
#include <stdint.h>

#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))
uint32_t strlen(const char *);
int32_t strcmp(const char *, const char *);
char *strncpy(char *, const char *, uint32_t);
uint32_t strtoul(const char *, char **, int);
void u32_to_str(uint32_t value, char *buf);
void *memset(void *, int, uint32_t);
    
// Delay loop
#define DELAY(count) do { volatile uint32_t _i = (count); while (_i--) asm volatile("nop"); } while(0)
/*ifdef __aarch64__
    #define DELAY(count) do { \
        uint64_t __temp = (uint64_t)(count); \
        __asm__ volatile ( \
            "1: cbz %[cnt], 2f     \n" \
            "   sub %[cnt], %[cnt], #1 \n" \
            "   b 1b               \n" \
            "2:                    \n" \
            : [cnt] "+r" (__temp) \
            : \
            : "cc" \
        ); \
    } while(0)
#else
    #define DELAY(count) do { \
    uint32_t __temp = (uint32_t)(count); \
    __asm__ volatile ( \
        "   cmp %0, #0      \n" \
        "   beq 2f          \n" \
        "1: subs %0, %0, #1 \n" \
        "   bne 1b          \n" \
        "2:                 \n" \
        : "+r" (__temp) \
        : \
        : "cc" \
    ); \
} while(0)
#endif
*/
/* Data Memory Barrier */
void dmb(void);
/* Data Synchronization Barrier */
void dsb(void);
/* Instruction Synchronization Barrier */
void isb(void);
/* Cache Clean */
void clean_cache(const volatile void *, uint32_t);
void invalidate_cache(const volatile void *, uint32_t);
/* Send Event */
void sev(void);
/* Wait for Event */
void wfe(void);
