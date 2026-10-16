/**
 * @file z80_emulator.h
 * @author Francesco Sacco
 * @date 25 Mar 2025
 * @brief This library exports the basic functions for Z80 processor emulation.
 *
 * This library exports functions to initialize and execute, step-by-step, the
 * opcodes of a Z80 processor loaded into memory. It also exports a function to
 * emulate the processor's interrupt event.
 *
 * @see https://github.com/francescosacco/zemu
 */
#ifndef Z80_EMULATOR
#define Z80_EMULATOR

#include <stdint.h>
#include <z80_memory.h>

void z80_setVerbose( void ) ;
void z80_initializeMemory( void ) ;

void z80_reset( void ) ;
void z80_step( void ) ;

void z80_interrupt( void ) ;

#endif // Z80_EMULATOR
