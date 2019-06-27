#ifndef IOPORTS_H
#define IOPORTS_H

#include <stdint.h>

#define PIT_3_PORT 0x42
#define PIT_CONFIGURE_PORT 0x43
#define KEYBOARD_DATA_PORT 0x60
#define NOT_KEYBOARD_PORT 0x61
#define KEYBOARD_STATUS_PORT 0x64

extern uint8_t _ioRead(uint16_t port);
extern void _ioWrite(uint16_t port, uint8_t data);

#endif /* IOPORTS_H */