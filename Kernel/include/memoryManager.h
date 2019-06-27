#ifndef MEMORY_MANAGER_H
#define MEMORY_MANAGER_H

#include <stdint.h>

void loadMemoryManager(void *memoryAddressLocation);
void *malloc(uint64_t length);

#endif /* MODULE_LOADER_H */