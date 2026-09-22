#ifndef Z80_EMULATOR_H
#define Z80_EMULATOR_H

#include <stdint.h>
#include <z80_memory.h>

void z80_initializeMemory( void ) ;

void z80_reset( void ) ;
void z80_step( void ) ;


#endif // Z80_EMULATOR_H
