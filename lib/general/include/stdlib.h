#pragma once
#include <stdint.h>

unsigned int strlen(const char *);
int strcmp(const char *, const char *);
char *strncpy(char *, const char *, unsigned int);
unsigned long strtoul(const char *, char **, int);
void u32_to_str(uint32_t value, char *buf);
void *memset(void *, int, unsigned long);
void delay(unsigned long);
