#ifndef INTERRUPS_H
#define INTERRUPS_H

#include <stdint.h>

extern void _exception0Handler(void);
extern void _exception1Handler(void);

extern void _irq00Handler(void);
extern void _irq01Handler(void);

extern void _systemCallHandler(void);

extern void _cli(void);
extern void _sti(void);
extern void _hlt(void);

extern void _picMasterMask(uint8_t mask);
extern void _picSlaveMask(uint8_t mask);

//Termina la ejecución de la cpu.
extern void _haltCpu(void);

#endif /* INTERRUPS_H */