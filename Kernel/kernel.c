#include <stdint.h>
#include <lib.h>
#include <moduleLoader.h>
#include <idtLoader.h>
#include <videoDriver.h>
#include <memoryManager.h>
#include <process.h>
#include <scheduler.h>

extern uint8_t text;
extern uint8_t rodata;
extern uint8_t data;
extern uint8_t bss;
extern uint8_t endOfKernelBinary;
extern uint8_t endOfKernel;

static const uint64_t PageSize = 0x1000;

static void *const sampleCodeModuleAddress = (void *)0x400000;
static void *const sampleDataModuleAddress = (void *)0x500000;
static void *const memoryAddress = (void *)0x700000;
static void *const videoAddress = (void *)0x5C00;

typedef int (*EntryPoint)();

void clearBSS(void *bssAddress, uint64_t bssSize)
{
	memset(bssAddress, 0, bssSize);
}

void *getStackBase()
{
	return (void *)((uint64_t)&endOfKernel + PageSize * 8 - sizeof(uint64_t));
}

void *initializeKernelBinary()
{
	void *moduleAddresses[] = {
		sampleCodeModuleAddress,
		sampleDataModuleAddress};

	loadModules(&endOfKernelBinary, moduleAddresses);
	clearBSS(&bss, &endOfKernel - &bss);
	initMemoryManager(memoryAddress, 1024*1024*50); // 50MB
	initVideo(videoAddress);
	initProcesses();
	initScheduler();
	loadIdt();

	return getStackBase();
}

void init(){
	newProcess((uint64_t)sampleCodeModuleAddress, "shell", 1, 3);
}

int main()
{
	init();
	return 0;
}