#pragma once
#include <stdint.h>

#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))
uint32_t strlen(const char *);
int32_t strcmp(const char *, const char *);
char *strncpy(char *, const char *, unsigned int);
uint32_t strtoul(const char *, char **, int);
void u32_to_str(uint32_t value, char *buf);
void *memset(void *, int, uint32_t);
    
// Delay loop
//#define DELAY(count) do { volatile uint32_t _i = (count); while (_i--) asm volatile("nop"); } while(0)
#define DELAY(count) do {               \
    uint32_t __temp = (count);          \
    __asm__ volatile (                  \
        "   cbz %0, 2f          \n"     \
        "1: subs %0, %0, #1     \n"     \
        "   bne 1b              \n"     \
        "2:                     \n"     \
        : "+r" (__temp)                 \
        :                               \
        : "cc"                          \
    );                                  \
} while(0)

/* Data Memory Barrier */
void dmb(void);