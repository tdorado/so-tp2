#ifndef PROCESS_H
#define PROCESS_H

#include <stdint.h>
#include <lib.h>

#define STACK_SIZE 4096

#define MAX_PROCESS_NAME 256
#define MAX_PROCESSES 256

typedef enum {P_READY, P_RUNNING, P_WAITING, P_TERMINATE} pstate_t;
typedef enum {BACKGROUND, FOREGROUND} pground_t;
typedef enum {PRIORITY_1=1, PRIORITY_2, PRIORITY_3, PRIORITY_4} ppriority_t;

typedef struct processADT * process_t;

void initProcesses();
process_t createProcess(uint64_t processStart, char * name, ppriority_t priority, pground_t ground);
void deleteProcess(process_t process);
void setState(process_t process, pstate_t state);
void setStateId(uint64_t pid, pstate_t state);
pstate_t getState(process_t process);
void setStackPointer(process_t process, uint64_t stackPointer);
void printProcess(process_t process);
uint64_t getPid(process_t process);
uint64_t getStackPointer(process_t process);
pstate_t getStateId(uint64_t pid);
void setGroundProcess(uint64_t pid, pground_t ground);
pground_t getCurrentGroundProcess();
int getForegroundProcesses();
void setCurrentProcessTerminate();
ppriority_t getPriority(process_t process);
void setForegroundProcesses(int newForegroundProcesses);

#endif