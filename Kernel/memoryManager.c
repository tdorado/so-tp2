#include <memoryManager.h>

static int8_t *memoryAddress;

void loadMemoryManager(void *memoryAddressLocation){
    memoryAddress = (int8_t *)memoryAddressLocation;
}

void *malloc(uint64_t length)
{
	uint64_t i;
	for (i = 0; i < length; i++)
	{
		memoryAddress[i] = 0;
	}
	void *result = memoryAddress;
	memoryAddress += length + 1;
	return result;
}