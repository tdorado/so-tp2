#ifndef LIB_H
#define LIB_H

#define EOF -1
#define NULL 0

#include <stdint.h>

// C functions
void *memset(void *destination, int32_t character, uint64_t length);
void *memcpy(void *destination, const void *source, uint64_t length);
char *strcpy(char *destination, const char *source);

//ASM functions
extern char *cpuVendor(char *result);
extern uint64_t getTimeRTC(uint64_t value);

#endif /* LIB_H */