#include <stdint.h>
#include <timeDriver.h>
#include <keyboardDriver.h>

static void int20();
static void int21();

static void (*irqs[])() = {
	int20,
	int21
};

void irqDispatcher(uint64_t irq)
{
	(*irqs[irq])();
}

static void int20()
{
	timerHandler();
}

static void int21()
{
	keyboardHandler();
}