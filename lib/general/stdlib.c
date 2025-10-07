unsigned int strlen(const char *s) {
    unsigned int len = 0;
    while (*s++) len++;
    return len;
}

int strcmp(const char *s1, const char *s2) {
    while (*s1 && (*s1 == *s2)) {
        s1++; s2++;
    }
    return *(const unsigned char *)s1 - *(const unsigned char *)s2;
}

char *strncpy(char *dest, const char *src, unsigned int n) {
    unsigned int i;
    for (i = 0; i < n && src[i]; i++) {
        dest[i] = src[i];
    }
    for (; i < n; i++) {
        dest[i] = '\0';
    }
    return dest;
}

unsigned long strtoul(const char *nptr, char **endptr, int base) {
    unsigned long result = 0;
    while (*nptr >= '0' && *nptr <= '9') {
        result = result * base + (*nptr - '0');
        nptr++;
    }
    if (endptr) *endptr = (char *)nptr;
    return result;
}
