#include <process.h>
#include <scheduler.h>
#include <memoryManager.h>
#include <videoDriver.h>
#include <interrupts.h>

typedef struct processADT {
	char * name[MAX_PROCESS_NAME];
    uint64_t pid;
	uint64_t ppid;
    pstate_t state;
	uint64_t stackStart;
    uint64_t stackPointer;
	int priority;
} processADT;

typedef struct {
	//Registers restore context
	uint64_t gs;
	uint64_t fs;
	uint64_t r15;
	uint64_t r14;
	uint64_t r13;
	uint64_t r12;
	uint64_t r11;
	uint64_t r10;
	uint64_t r9;
	uint64_t r8;
	uint64_t rsi;
	uint64_t rdi;
	uint64_t rbp;
	uint64_t rdx;
	uint64_t rcx;
	uint64_t rbx;
	uint64_t rax;

	//iretq hook
	uint64_t rip;
	uint64_t cs;
	uint64_t eflags;
	uint64_t rsp;
	uint64_t ss;
	uint64_t base;
}stack_t;

static uint64_t initStack(process_t process, uint64_t processStart, uint64_t stackPointer); 
static void processCaller(process_t process, uint64_t processStart);

static uint64_t global_pid;
static process_t allProcesses[MAX_PROCESSES];
static process_t foregroundProcess;

void initProcesses(){
	global_pid = 0;
	foregroundProcess = NULL;
	for(int i = 0; i < MAX_PROCESSES; i++){
		allProcesses[i] = NULL;
	}
}

process_t createProcess(uint64_t processStart, char * processName, int priority){

    process_t newProcess = (process_t)malloc(sizeof(*newProcess));
	strcpy(processName, (char*)(newProcess->name));
    newProcess->pid = global_pid;
    newProcess->state = P_READY;
	newProcess->stackStart = (uint64_t)malloc(STACK_SIZE);
    newProcess->stackPointer = initStack(newProcess, processStart, newProcess->stackStart);
	newProcess->priority = priority;
	if(global_pid != 0){
		newProcess->ppid = getCurrentProcess()->pid;
	}
	else{
		foregroundProcess = newProcess;
		newProcess->ppid = 0;
	}

	allProcesses[global_pid++] = newProcess;

    return newProcess;
}

void setForegroundProcess(uint64_t pid){
	if(allProcesses[pid] != NULL){
		foregroundProcess = allProcesses[pid];
	}
}

int isCurrentProcessForeground(){
	return foregroundProcess == getCurrentProcess();
}

void deleteProcess(process_t process){
	allProcesses[process->pid] = NULL;
    free((void *)process->stackStart);
    free(process);
}

void setState(process_t process, pstate_t state){
    process->state = state;
	if(state == P_TERMINATE && process == foregroundProcess){
		foregroundProcess = allProcesses[process->ppid];
		_contextSwitchProcess();
	}
}

void setCurrentProcessTerminate(){
	setState(getCurrentProcess(), P_TERMINATE);
}

pstate_t getState(process_t process){
    return process->state;
}

void setStackPointer(process_t process, uint64_t stackPointer){
    process->stackPointer=stackPointer;
}

void setStateId(uint64_t pid, pstate_t state){
	allProcesses[pid]->state = state;
}

uint64_t getPid(process_t process){
	return process->pid;
}

uint64_t getStackPointer(process_t process){
	return process->stackPointer;
}

static uint64_t initStack(process_t process, uint64_t processStart, uint64_t stackPointer) {
    stack_t * frame = (stack_t *)(stackPointer + STACK_SIZE - sizeof(stack_t) - 1);

	frame->gs = 0x000;
	frame->fs =	0x000;
	frame->r15 = 0x000;
	frame->r14 = 0x000;
	frame->r13 = 0x000;
	frame->r12 = 0x000;
	frame->r11 = 0x000;
	frame->r10 = 0x000;
	frame->r9 =	0x000;
	frame->r8 =	0x000;
	frame->rsi = processStart;
	frame->rdi = (uint64_t)process;
	frame->rbp = 0x000;
	frame->rdx = 0x000;
	frame->rcx = 0x000;
	frame->rbx = 0x000;
	frame->rax = 0x000;

	frame->rip = (uint64_t)processCaller;
	frame->cs =	0x008;
	frame->eflags = 0x202;
	frame->rsp = (uint64_t)&(frame->base);
	frame->ss = 0x000;
	frame->base = 0x000;

	return (uint64_t)frame;
}

void printProcess(process_t process){
	printString("PID: ");
	printDec(process->pid);
	printString(" PROCESS NAME: ");
	printString((char *)process->name);
	printString(" STATE: ");
	if(process->state == P_READY){
		printString("P_READY");
	}
	else if(process->state == P_RUNNING){
		printString("P_RUNNING");
	}
	else if(process->state == P_WAITING){
		printString("P_WAITING");
	}
	else{
		printString("P_TERMINATE");
	}
	printString(" PPID: ");
	printDec(process->ppid);
	printString(" STACK: ");
	printDec((int)process->stackStart);
	if(foregroundProcess == process){
		printString(" PROCESS IN: FOREGROUND");
	}
	else{
		printString(" PROCESS IN: BACKGROUND");
	}
	printString(" PRIORITY: ");
	printDec(process->priority);

	printString("/n");
}

static void processCaller(process_t process, uint64_t processStart){
	void (*processCall)(void) = (void (*)(void))processStart;
	(*processCall)();
	if(foregroundProcess == process){
		setForegroundProcess(process->ppid);
	}
	killCurrentProcess();
}

int getPriority(process_t process){
	return process->priority;
}

pstate_t getStateId(uint64_t pid){
	return allProcesses[pid]->state;
}