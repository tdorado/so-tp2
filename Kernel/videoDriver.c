#include <videoDriver.h>
#include <font.h>
#include <lib.h>

typedef struct vbe_mode_info_structure
{
	uint16_t attributes;				// deprecated, only bit 7 should be of interest to you, and it indicates the mode supports a linear frame buffer.
	uint8_t window_a;					// deprecated
	uint8_t window_b;					// deprecated
	uint16_t granularity;				// deprecated; used while calculating bank numbers
	uint16_t window_size;
	uint16_t segment_a;
	uint16_t segment_b;
	uint32_t win_func_ptr;				// deprecated; used to switch banks from protected mode without returning to real mode
	uint16_t pitch;						// number of bytes per horizontal line
	uint16_t width;						// width in pixels
	uint16_t height;					// height in pixels
	uint8_t w_char;						// unused...
	uint8_t y_char;						// ...
	uint8_t planes;
	uint8_t bpp;						// bits per pixel in this mode
	uint8_t banks;						// deprecated; total number of banks in this mode
	uint8_t memory_model;
	uint8_t bank_size;					// deprecated; size of a bank, almost always 64 KB but may be 16 KB...
	uint8_t image_pages;
	uint8_t reserved0;

	uint8_t red_mask;
	uint8_t red_position;
	uint8_t green_mask;
	uint8_t green_position;
	uint8_t blue_mask;
	uint8_t blue_position;
	uint8_t reserved_mask;
	uint8_t reserved_position;
	uint8_t direct_color_attributes;

	uint32_t framebuffer;				// physical address of the linear frame buffer; write here to draw to the screen
	uint32_t off_screen_mem_off;
	uint16_t off_screen_mem_size;		// size of memory in the framebuffer but not being displayed on the screen
	uint8_t reserved1[206];
} __attribute__((packed)) vbe;

void shiftScreen();
void backSpace();
uint32_t uintToBase(uint64_t value, char * buffer, uint32_t base);

static vbe *vbeStruct; //Sacado de sysvar.asm en Bootloader/Pure64/src
static unsigned int actualX = 0;
static unsigned int actualY = 0;
static unsigned char backgroundR = 0;
static unsigned char backgroundG = 0;
static unsigned char backgroundB = 0;
static char buffer[MAX_NUMBER_LENGHT] = {0};

void loadVideo(void * vbeDir){
	vbeStruct = (vbe *)vbeDir;
	printBackGround();
}

int setActualPixel(unsigned int x, unsigned int y)
{
	if (x >= 0 && x <= vbeStruct->width && y >= 0 && y <= vbeStruct->height)
	{
		actualX = x;
		actualY = y;
		return 1;
	}
	return 0;
}

int printPixel(unsigned int x, unsigned int y, unsigned char R, unsigned char G, unsigned char B)
{
	if (x >= 0 && x <= vbeStruct->width && y >= 0 && y <= vbeStruct->height)
	{
		unsigned char *pixel = 0;
		pixel += vbeStruct->framebuffer + 3 * (x + y * vbeStruct->width);
		*pixel = B;
		*(pixel + 1) = G;
		*(pixel + 2) = R;
		return 1;
	}
	return 0;
}

void printChar(unsigned char c, unsigned char R, unsigned char G, unsigned char B)
{
	if (c == 0)
	{
		return;
	}
	else if (c == '\n')
	{
		newLine();
	}
	else if (c == '\b')
	{
		backSpace();
	}
	else if (c > 31)
	{
		if (actualX >= vbeStruct->width)
		{
			newLine();
		}
		unsigned char *charPixelMap = fontPixelMap(c);
		unsigned char charPixel;

		for (int y = 0; y < FONT_HEIGHT; y++)
		{
			for (int x = 0; x < FONT_WIDTH; x++)
			{
				charPixel = charPixelMap[y];
				charPixel >>= FONT_WIDTH - x;

				if (charPixel % 2 == 1)
				{
					printPixel(actualX + x, actualY + y, R, G, B);
				}
				else
				{
					printPixel(actualX + x, actualY + y, backgroundR, backgroundG, backgroundB);
				}
			}
		}
		actualX += FONT_WIDTH;
	}
}

void newLine()
{
	actualX = 0;
	actualY += FONT_HEIGHT;
	if (actualY >= vbeStruct->height)
	{
		actualY -= FONT_HEIGHT;
		shiftScreen();
	}
}

void shiftScreen()
{
	unsigned char *frameBuffer = 0;
	frameBuffer += vbeStruct->framebuffer;
	memcpy(frameBuffer, frameBuffer + 3 * vbeStruct->width * FONT_HEIGHT, 3 * vbeStruct->width * (vbeStruct->height - FONT_HEIGHT));
	for (int y = actualY; y < vbeStruct->height; y++)
	{
		for (int x = 0; x < vbeStruct->width; x++)
		{
			printPixel(x, y, backgroundR, backgroundG, backgroundB);
		}
	}
}

void backSpace()
{
	if (actualX >= FONT_WIDTH)
	{
		actualX -= FONT_WIDTH;
		for (int y = 0; y < FONT_HEIGHT; y++)
		{
			for (int x = 0; x < FONT_WIDTH; x++)
			{
				printPixel(actualX + x, actualY + y, backgroundR, backgroundG, backgroundB);
			}
		}
	}
	else if (actualX == 0 && actualY >= FONT_HEIGHT)
	{
		actualY -= FONT_HEIGHT;
		actualX = vbeStruct->width;
		backSpace();
	}
}

void printBackGround()
{
	for (int y = 0; y < vbeStruct->height; y++)
	{
		for (int x = 0; x < vbeStruct->width; x++)
		{
			printPixel(x, y, backgroundR, backgroundG, backgroundB);
		}
	}
	actualX = 0;
	actualY = 0;
}

void setBackGroundColor(unsigned char R, unsigned char G, unsigned char B)
{
	backgroundR = R;
	backgroundG = G;
	backgroundB = B;
}

int paintPixelBackGroundColor(unsigned int x, unsigned int y)
{
	return printPixel(x, y, backgroundR, backgroundG, backgroundB);
}

void printString(const char *str, unsigned char R, unsigned char G, unsigned char B)
{
	int i = 0;
	while (str[i] != 0)
	{
		printChar(str[i++], R, G, B);
	}
}

void printDec(uint64_t value)
{
	printBase(value, 10);
}

void printHex(uint64_t value)
{
	printBase(value, 16);
}

void printBin(uint64_t value)
{
	printBase(value, 2);
}

void printBase(uint64_t value, uint32_t base)
{
	uintToBase(value, buffer, base);
	printString(buffer, 255, 255, 255);
}

uint32_t uintToBase(uint64_t value, char *buffer, uint32_t base)
{
	char *p = buffer;
	char *p1, *p2;
	uint32_t digits = 0;

	//Calculate characters for each digit
	do
	{
		uint32_t remainder = value % base;
		*p++ = (remainder < 10) ? remainder + '0' : remainder + 'A' - 10;
		digits++;
	} while (value /= base);

	// Terminate string in buffer.
	*p = 0;

	//Reverse string in buffer.
	p1 = buffer;
	p2 = p - 1;
	while (p1 < p2)
	{
		char tmp = *p1;
		*p1 = *p2;
		*p2 = tmp;
		p1++;
		p2--;
	}

	return digits;
}