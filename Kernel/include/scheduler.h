#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <process.h>

void initScheduler();
void runProcess(process_t process);
void killCurrentProcess();
uint64_t switchProcess(uint64_t stackPointer);
void printCurrentProcesses();
process_t getCurrentProcess();
uint64_t newProcess(uint64_t processStart, char * processName, int foreground, int priority);

#endif

