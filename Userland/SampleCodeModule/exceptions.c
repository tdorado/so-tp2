#include <exceptions.h>

typedef void (*function)();

void zeroDiv()
{
    int a = 1;
    int b = 0;
    int c = a / b;
    c++;
}

void opCode()
{
    uint64_t instruction = 0xFFFFFFFFFFFF; //invalid instruction
    uint64_t *aux = &instruction;
    ((function)aux)();
}