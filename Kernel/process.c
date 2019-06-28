#include <process.h>
#include <scheduler.h>
#include <memoryManager.h>
#include <videoDriver.h>
#include <interrupts.h>

typedef struct processADT
{
	char *name[MAX_PROCESS_NAME];
	uint64_t pid;
	uint64_t ppid;
	pstate_t state;
	uint64_t stackStart;
	uint64_t stackPointer;
	pground_t ground;
	ppriority_t priority;
} processADT;

typedef struct
{
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
} stack_t;

static uint64_t initStack(process_t process, uint64_t processStart, uint64_t stackPointer, int argc, void ** argv);
static void processCaller(process_t process, uint64_t processStart, int argc, void ** argv);

static uint64_t global_pid;
static process_t allProcesses[MAX_PROCESSES];
static int foregroundProcesses;

void initProcesses()
{
	global_pid = 0;
	foregroundProcesses = 0;
	for (int i = 0; i < MAX_PROCESSES; i++)
	{
		allProcesses[i] = NULL;
	}
}

process_t createProcess(uint64_t processStart, char *processName, int argc, void ** argv, ppriority_t priority, pground_t ground)
{

	process_t newProcess = (process_t)malloc(sizeof(*newProcess));
	strcpy(processName, (char *)(newProcess->name));
	newProcess->pid = global_pid;
	newProcess->state = P_READY;
	newProcess->stackStart = (uint64_t)malloc(STACK_SIZE);
	newProcess->stackPointer = initStack(newProcess, processStart, newProcess->stackStart, argc, argv);
	newProcess->priority = priority;
	newProcess->ground = ground;
	if (ground == FOREGROUND)
	{
		foregroundProcesses++;
	}
	if (global_pid != 0)
	{
		newProcess->ppid = getCurrentProcess()->pid;
	}
	else
	{
		newProcess->ppid = 0;
	}

	allProcesses[global_pid++] = newProcess;

	return newProcess;
}

void setGroundProcess(uint64_t pid, pground_t ground)
{
	if (allProcesses[pid] != NULL)
	{
		allProcesses[pid]->ground = ground;
	}
}

pground_t getCurrentGroundProcess()
{
	return getCurrentProcess()->ground;
}

int getForegroundProcesses()
{
	return foregroundProcesses;
}

void setForegroundProcesses(int newForegroundProcesses)
{
	foregroundProcesses = newForegroundProcesses;
}

void deleteProcess(process_t process)
{
	allProcesses[process->pid] = NULL;
	free((void *)process->stackStart);
	free(process);
}

void setState(process_t process, pstate_t state)
{
	process->state = state;
	if (state == P_TERMINATE)
	{
		_contextSwitchProcess();
	}
}

void setCurrentProcessTerminate()
{
	setState(getCurrentProcess(), P_TERMINATE);
}

pstate_t getState(process_t process)
{
	return process->state;
}

void setStackPointer(process_t process, uint64_t stackPointer)
{
	process->stackPointer = stackPointer;
}

void setStateId(uint64_t pid, pstate_t state)
{
	allProcesses[pid]->state = state;
}

uint64_t getPid(process_t process)
{
	return process->pid;
}

uint64_t getStackPointer(process_t process)
{
	return process->stackPointer;
}

static uint64_t initStack(process_t process, uint64_t processStart, uint64_t stackPointer, int argc, void ** argv)
{
	stack_t *frame = (stack_t *)(stackPointer + STACK_SIZE - sizeof(stack_t) - 1);

	frame->gs = 0x000;
	frame->fs = 0x000;
	frame->r15 = 0x000;
	frame->r14 = 0x000;
	frame->r13 = 0x000;
	frame->r12 = 0x000;
	frame->r11 = 0x000;
	frame->r10 = 0x000;
	frame->r9 = 0x000;
	frame->r8 = 0x000;
	frame->rsi = processStart;
	frame->rdi = (uint64_t)process;
	frame->rbp = 0x000;
	frame->rdx = (uint64_t)argc;
	frame->rcx = (uint64_t)argv;
	frame->rbx = 0x000;
	frame->rax = 0x000;

	frame->rip = (uint64_t)processCaller;
	frame->cs = 0x008;
	frame->eflags = 0x202;
	frame->rsp = (uint64_t) & (frame->base);
	frame->ss = 0x000;
	frame->base = 0x000;

	return (uint64_t)frame;
}

void printProcess(process_t process)
{
	printString("PID: ");
	printDec(process->pid);
	printString(" PROCESS NAME: ");
	printString((char *)process->name);
	printString(" STATE: ");
	if (process->state == P_READY)
	{
		printString("P_READY");
	}
	else if (process->state == P_RUNNING)
	{
		printString("P_RUNNING");
	}
	else if (process->state == P_WAITING)
	{
		printString("P_WAITING");
	}
	else
	{
		printString("P_TERMINATE");
	}
	printString(" PPID: ");
	printDec(process->ppid);
	printString(" STACK: ");
	printDec((int)process->stackStart);
	if (process->ground == FOREGROUND)
	{
		printString(" PROCESS IN: FOREGROUND");
	}
	else
	{
		printString(" PROCESS IN: BACKGROUND");
	}
	printString(" PRIORITY: ");
	printDec(process->priority);

	printString("/n");
}

static void processCaller(process_t process, uint64_t processStart, int argc, void ** argv)
{
	((void (*)(int , void **))processStart)(argc, argv);
	killCurrentProcess();
}

ppriority_t getPriority(process_t process)
{
	return process->priority;
}

pstate_t getStateId(uint64_t pid)
{
	return allProcesses[pid]->state;
}