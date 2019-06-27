#include <soundDriver.h>
#include <stdint.h>
#include <ioports.h>
#include <timeDriver.h>

void activateSound(int frequence_div) {
 int freq;
 uint8_t aux;
 freq = 1193180 / frequence_div;
 _ioWrite(PIT_CONFIGURE_PORT, 0xb6);
 _ioWrite(PIT_3_PORT, (uint8_t) (freq) );
 _ioWrite(PIT_3_PORT, (uint8_t) (freq >> 8));

 aux = _ioRead(NOT_KEYBOARD_PORT);
   if (aux != (aux | 3)) {
   _ioWrite(NOT_KEYBOARD_PORT, aux | 3);
 }
}


void deactivateSound() {
 uint8_t aux = _ioRead(NOT_KEYBOARD_PORT) & 0xFC;
 _ioWrite(NOT_KEYBOARD_PORT, aux);
}

void beep() {
  activateSound(100);
  ticksDelay(2);
  deactivateSound();
}
