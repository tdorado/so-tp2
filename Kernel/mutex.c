#include <mutex.h>

void lockMutex(void * lock){
    _mutexLock((uint64_t)lock);
}

void unlockMutex(void * lock){
    _mutexUnlock((uint64_t)lock);
}