#include <scheduler.h>
#include <memoryManager.h>
#include <videoDriver.h>
#include <interrupts.h>
#include <lib.h>

typedef struct nodeADT *node_t;

typedef struct nodeADT
{
    process_t element;
    node_t prev;
    node_t next;
    int quantum;
} nodeADT;

static void setNextProcess();

//LISTA DE NODOS CIRCULAR PARA LOS PROCESOS
//TODO PROCESO SE AGREGA AL FINAL, SIGUIENDO EL ALGORITMO ROUND ROBIN
static node_t currentProcess;
static node_t firstProcess;
static node_t lastProcess;
static uint64_t numberOfProcesses;

void initScheduler()
{
    currentProcess = NULL;
    lastProcess = NULL;
    numberOfProcesses = 0;
}

void runProcess(process_t process)
{
    numberOfProcesses++;
    if (lastProcess == NULL)
    {
        lastProcess = malloc(sizeof(nodeADT));
        lastProcess->element = process;
        lastProcess->next = lastProcess;
        lastProcess->prev = lastProcess;
        lastProcess->quantum = 1;
        currentProcess = lastProcess;
        firstProcess = currentProcess;
        setState(currentProcess->element, P_RUNNING);
        _changeProcess(getStackPointer(process));
    }
    else
    {
        node_t aux = malloc(sizeof(nodeADT));
        aux->element = process;
        aux->next = lastProcess->next;
        aux->prev = lastProcess;
        aux->quantum = 1;
        lastProcess->next->prev = aux;
        lastProcess->next = aux;
        lastProcess = aux;
    }
}

void killCurrentProcess()
{

    if (getCurrentGroundProcess() == FOREGROUND)
    {
        setForegroundProcesses(getForegroundProcesses() - 1);
    }

    numberOfProcesses--;

    node_t aux = currentProcess;
    currentProcess->prev->next = currentProcess->next;
    currentProcess->next->prev = currentProcess->prev;

    if (lastProcess == aux)
    {
        lastProcess = aux->prev;
    }

    setNextProcess();

    deleteProcess(aux->element);
    free(aux);

    _changeProcess(getStackPointer(currentProcess->element));
}

static void setNextProcess()
{
    if ((getPriority(currentProcess->element) % currentProcess->quantum) == 0)
    {
        currentProcess->quantum = 1;
        currentProcess = currentProcess->next;
    }
    else
    {
        currentProcess->quantum++;
        return;
    }

    pstate_t pstate = getState(currentProcess->element);

    if (pstate == P_TERMINATE)
    {
        killCurrentProcess();
    }
    else if (pstate == P_WAITING)
    {
        setNextProcess();
    }
    else if (pstate == P_READY)
    {
        setState(currentProcess->element, P_RUNNING);
    }
}

//Esto se llama desde el iqr handler que seria mas rapido que el timer creo, asi nos acercamos
// lo mas posible a un quantum como pide la consigna
uint64_t switchProcess(uint64_t stackPointer)
{
    if (numberOfProcesses > 1)
    {

        setStackPointer(currentProcess->element, stackPointer);

        setNextProcess();

        return getStackPointer(currentProcess->element);
    }
    else
    {
        return stackPointer;
    }
}

void printCurrentProcesses()
{
    node_t aux = firstProcess;
    for (int i = 0; i < numberOfProcesses; i++)
    {
        printProcess(aux->element);
        aux = aux->next;
    }
}

process_t getCurrentProcess()
{
    return (process_t)currentProcess->element;
}

uint64_t newProcess(uint64_t processStart, char *processName, ppriority_t priority, pground_t ground)
{
    process_t newProcess = createProcess(processStart, processName, priority, ground);
    runProcess(newProcess);
    return getPid(newProcess);
}