#include <videoDriver.h>
#include <stdint.h>

static void zeroDivision(uint64_t *states);
static void invalidOpCode(uint64_t *states);
static void notImplemented(uint64_t *states);
static void printRegisters(uint64_t *states);

static const char *registers[16] = {"RSP: ", "RAX: ", "RBX: ", "RCX: ", "RDX: ", "RBP: ", "RDI: ", "RSI: ", "R8: ", "R9: ", "R10: ", "R11: ", "R12: ", "R13: ", "R14: ", "R15: "};
static void (*exceptions[])(uint64_t *states) = {
	zeroDivision,   //EX 0
	notImplemented, //EX 1
	notImplemented, //EX 2
	notImplemented, //EX 3
	notImplemented, //EX 4
	notImplemented, //EX 5
	invalidOpCode   //EX 6
};

void exceptionDispatcher(uint64_t exception, uint64_t *states)
{
	(exceptions[exception])(states);
}

static void zeroDivision(uint64_t *states)
{
	printStringError("ERROR: Division by zero exception number 0.");
	printRegisters(states);
}

static void notImplemented(uint64_t *states)
{
	printStringError("ERROR: Not implemented exception.");
	printRegisters(states);
}

static void invalidOpCode(uint64_t *states)
{
	printStringError("ERROR: Invalid opcode exception number 6.");
	printRegisters(states);
}

static void printRegisters(uint64_t *states)
{
	newLine();
	for (int i = 0; i < 16; i++)
	{
		printStringError(registers[i]);
		printDecError(states[i]);
		newLine();
	}
}