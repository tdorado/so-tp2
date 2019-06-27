#ifndef VIDEO_DRIVER_H
#define VIDEO_DRIVER_H

#include <stdint.h>

#define MAX_NUMBER_LENGHT 256

void loadVideo(void * vbeDir);
int setActualPixel(unsigned int x, unsigned int y);
int printPixel(unsigned int x, unsigned int y, unsigned char R, unsigned char G, unsigned char B);
void printChar(unsigned char c);
void newLine();
void printBackGround();
void setBackGroundColor(unsigned char R, unsigned char G, unsigned char B);
void setCharColor(unsigned char R, unsigned char G, unsigned char B);
int paintPixelBackGroundColor(unsigned int x, unsigned int y);
void printString(const char *str);
void printDec(uint64_t value);
void printHex(uint64_t value);
void printBin(uint64_t value);
void printBase(uint64_t value, uint32_t base);
void printStringError(const char *str);
void printDecError(uint64_t value);
void printHexError(uint64_t value);
void printBinError(uint64_t value);
void printBaseError(uint64_t value, uint32_t base);

#endif /* VIDEO_DRIVER_H */