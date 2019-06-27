#include <stdint.h>
#include <keyboardDriver.h>
#include <videoDriver.h>
#include <soundDriver.h>
#include <timeDriver.h>
#include <memoryManager.h>
#include <lib.h>

static uint64_t getTime(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9);
static uint64_t readChar(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9);
static uint64_t writeChar(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9);
static uint64_t beepSound(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9);
static uint64_t memalloc(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9);
static uint64_t memfree(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9);
static uint64_t clearBackGround(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9);
static uint64_t setBackGround(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9);
static uint64_t writePixel(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9);
static uint64_t setPixel(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9);
static uint64_t paintPixelBackGround(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9);
static uint64_t setLetterColor(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9);
static uint64_t showMemoryNodes(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9);
static uint64_t sleep(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9);

static uint64_t (*systemCall[])(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9) = {
	getTime,			  //0
	readChar,			  //1
	writeChar,			  //2
	beepSound,			  //3
	memalloc,			  //4
	memfree,			  //5
	clearBackGround,	  //6
	setBackGround,		  //7
	writePixel,			  //8
	setPixel,			  //9
	paintPixelBackGround, //10
	setLetterColor,		  //11
	showMemoryNodes,	  //12
	sleep				  //13
};

uint64_t systemCallDispatcher(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9)
{
	return (*systemCall[rdi])(rsi, rdx, rcx, r8, r9);
}

static uint64_t getTime(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9)
{
	return getTimeRTC(rsi);
}

static uint64_t readChar(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9)
{
	return getChar();
}

static uint64_t writeChar(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9)
{
	printChar((unsigned char)rsi);
	return 1;
}

static uint64_t beepSound(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9)
{
	beep();
	return 1;
}

static uint64_t memalloc(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9)
{
	return (uint64_t)malloc(rsi);
}

static uint64_t memfree(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9)
{
	free((void *)rsi);
	return 1;
}

static uint64_t clearBackGround(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9)
{
	printBackGround();
	return 1;
}

static uint64_t setBackGround(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9)
{
	setBackGroundColor((unsigned char)rsi, (unsigned char)rdx, (unsigned char)rcx);
	return 1;
}

static uint64_t writePixel(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9)
{
	return printPixel((unsigned int)rsi, (unsigned int)rdx, (unsigned char)rcx, (unsigned char)r8, (unsigned char)r9);
}

static uint64_t setPixel(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9)
{
	return setActualPixel((unsigned int)rsi, (unsigned int)rdx);
}

static uint64_t paintPixelBackGround(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9)
{
	return paintPixelBackGroundColor((unsigned int)rsi, (unsigned int)rdx);
}

static uint64_t setLetterColor(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9)
{
	setCharColor((unsigned char)rsi, (unsigned char)rdx, (unsigned char)rcx);
	return 1;
}

static uint64_t showMemoryNodes(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9)
{
	printMemoryNodes();
	return 1;
}

static uint64_t sleep(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9)
{
	ticksDelay(rsi);
	return 1;
}