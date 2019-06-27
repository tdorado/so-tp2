#ifndef IOPORTS_H
#define IOPORTS_H

#include <stdint.h>

#define PIT_3_PORT 0x42
#define PIT_CONFIGURE_PORT 0x43
#define KEYBOARD_DATA_PORT 0x60
#define NOT_KEYBOARD_PORT 0x61
#define KEYBOARD_STATUS_PORT 0x64

#define RTC_REGISTER_SELECTOR 0x70
#define RTC_DATA 0x71

#define SECONDS_REGISTER 0x00
#define MINUTES_REGISTER 0x02
#define HOUR_REGISTER 0x04
#define WEEKDAY_REGISTER 0x06
#define DAY_OF_MONTH_REGISTER 0x07
#define MONTH_REGISTER 0x08
#define YEAR_REGISTER 0x09
#define CENTURY_REGISTER 0x32
#define STATUS_A_REGISTER 0x0A
#define STATUS_B_REGISTER 0x0B

extern uint8_t _ioRead(uint16_t port);
extern void _ioWrite(uint16_t port, uint8_t data);

#endif /* IOPORTS_H */