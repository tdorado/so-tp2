#ifndef VIDEO_DRIVER_H
#define VIDEO_DRIVER_H

#include <stdint.h>

#define MAX_NUMBER_LENGHT 256

void loadVideo(void * vbeDir);
int setActualPixel(unsigned int x, unsigned int y);
int printPixel(unsigned int x, unsigned int y, unsigned char R, unsigned char G, unsigned char B);
void printChar(unsigned char c, unsigned char R, unsigned char G, unsigned char B);
void newLine();
void printBackGround();
void setBackGroundColor(unsigned char R, unsigned char G, unsigned char B);
int paintPixelBackGroundColor(unsigned int x, unsigned int y);
void printString(const char *str, unsigned char R, unsigned char G, unsigned char B);
void printDec(uint64_t value);
void printHex(uint64_t value);
void printBin(uint64_t value);
void printBase(uint64_t value, uint32_t base);

#endif /* VIDEO_DRIVER_H */