/**
 * @file z80_memory.h
 * @author Francesco Sacco
 * @date 25 Mar 2025
 * @brief This library only provides access to the emulation of the Z80 processor's memory.
 *
 * The Z80 processor is capable of addressing 64kBytes of memory.
 * This library provides access to a global memory variable.
 *
 * @see https://github.com/francescosacco/zemu
 */
#ifndef Z80_MEMORY
#define Z80_MEMORY

#include <stdint.h>

#define MEMORY_SIZE              ( 64 * 1024 ) // 64KB

extern uint8_t z80_memory[ MEMORY_SIZE ] ;

#endif // Z80_MEMORY
