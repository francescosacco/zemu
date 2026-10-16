/**
 * @file z80_io.h
 * @author Francesco Sacco
 * @date 25 Mar 2025
 * @brief This library provides functions for emulating I/O read and write operations.
 *
 * To emulate data reading and writing (Input/Output), this library provides functions
 * for such operations. It allows for checking new data on any port by performing a
 * polling process.
 *
 * @see https://github.com/francescosacco/zemu
 */
#ifndef Z80_IO
#define Z80_IO

#include <stdint.h>
#include <stdbool.h>

void z80_io_initialization( void ) ;

uint8_t z80_io_getData( uint8_t io ) ;
void    z80_io_setData( uint8_t io , uint8_t dataIn ) ;

bool z80_io_newData( uint8_t io ) ;

#endif // Z80_IO
