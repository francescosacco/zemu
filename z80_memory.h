#ifndef Z80_MEMORY_H
#define Z80_MEMORY_H

#include <stdint.h>

#define MEMORY_SIZE              ( 64 * 1024 ) // 64KB

extern uint8_t z80_memory[ MEMORY_SIZE ] ;

#endif // Z80_MEMORY_H
