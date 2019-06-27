#include <timeDriver.h>
#include <interrupts.h>

static unsigned long ticks = 0;

void timerHandler()
{
	ticks++;
}

int ticksElapsed()
{
	return ticks;
}

int secondsElapsed()
{
	return ticks / 18;
}

void secondsDelay(unsigned long secondsParam)
{
	_cli();
	_sti();
	unsigned long finalSecond = (ticks / 18) + secondsParam;
	while ((ticks / 18) < finalSecond);
}

void ticksDelay(unsigned long ticksParam)
{
	_cli();
	_sti();
	unsigned long finalTick = ticks + ticksParam;
	while (ticks < finalTick);
}