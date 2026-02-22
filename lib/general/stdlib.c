#include "include/stdlib.h"

uint32_t strlen(const char *s) {
    uint32_t len = 0;
    while (*s++) len++;
    return len;
}

int32_t strcmp(const char *s1, const char *s2) {
    while (*s1 && (*s1 == *s2)) {
        s1++; s2++;
    }
    return *(const unsigned char *)s1 - *(const unsigned char *)s2;
}

char *strncpy(char *dest, const char *src, uint32_t n) {
    uint32_t i;
    for (i = 0; i < n && src[i]; i++) {
        dest[i] = src[i];
    }
    for (; i < n; i++) {
        dest[i] = '\0';
    }
    return dest;
}

uint32_t strtoul(const char *nptr, char **endptr, int base) {
    uint32_t result = 0;
    while (*nptr >= '0' && *nptr <= '9') {
        result = result * base + (*nptr - '0');
        nptr++;
    }
    if (endptr) *endptr = (char *)nptr;
    return result;
}

void u32_to_str(uint32_t value, char *buf) {
    char tmp[10];
    int i = 0;
    // Speciale case: 0
    if (value == 0) {
        buf[0] = '0';
        buf[1] = '\0';
        return;
    }
    // Cijfer voor cijfer in omgekeerde volgorde
    while (value > 0 && i <10) {
        tmp[i++] = '0' + (value % 10);
        value /= 10;
    }
    // Draai de volgorde om in buf[]
    int j = 0;
    while (i > 0) {
        buf[j++] = tmp[--i];
    }
    buf[j] = '\0';  // nulterminator
}

void *memset(void *dst, int v, uint32_t n)
{
    unsigned char *p = dst;
    while (n--) *p++ = (unsigned char)v;
    return dst;
}

void delay(unsigned long count) {
    while (count--) {
        asm volatile("nop");
    }
}

/* Data Memory Barrier */
void dmb(void) {
    #if defined(__arm__) && (__ARM_ARCH <= 6)
        // ARMv6 (Pi 1 / Zero)
        __asm__ __volatile__ ("mcr p15, 0, %0, c7, c10, 5" : : "r" (0) : "memory");
    #else
        // ARMv7 en ARMv8 (Pi 2, 3, 4, 5 in zowel 32-bit als 64-bit)
        __asm__ __volatile__ ("dmb sy" : : : "memory");
    #endif
}