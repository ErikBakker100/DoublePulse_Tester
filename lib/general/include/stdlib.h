#pragma once
#include <stdint.h>

#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))
unsigned int strlen(const char *);
int strcmp(const char *, const char *);
char *strncpy(char *, const char *, unsigned int);
unsigned long strtoul(const char *, char **, int);
void u32_to_str(uint32_t value, char *buf);
void *memset(void *, int, unsigned long);

// Delay loop
#define DELAY(count) do { volatile uint32_t _i = (count); while (_i--) asm volatile("nop"); } while(0)