#ifndef MUTEX_H_
#define MUTEX_H_

#include <stdint.h>

void lockMutex(void * lock);
void unlockMutex(void * lock);

extern void _mutexLock(uint64_t lock);
extern void _mutexUnlock(uint64_t lock);

#endif