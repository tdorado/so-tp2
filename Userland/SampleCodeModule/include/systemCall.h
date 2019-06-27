#ifndef SYSTEM_CALL_H
#define SYSTEM_CALL_H

#include <stdint.h>

#define GET_TIME 0
#define READ_CHAR 1
#define WRITE_CHAR 2
#define BEEP_SOUND 3
#define MEM_ALLOC 4
#define MEM_FREE 5
#define CLEAR_BACKGROUND 6
#define SET_BACKGROUND 7
#define WRITE_PIXEL 8
#define SET_PIXEL 9
#define PAINT_PIXEL_BACKGROUND 10
#define SET_CHAR_COLOR 11
#define SHOW_MEMORY_NODES 12

uint64_t systemCall(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9);

#endif