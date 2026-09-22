#ifndef Z80_IO_H
#define Z80_IO_H

#include <stdint.h>
#include <stdbool.h>

void z80_io_initialization( void ) ;

uint8_t z80_io_getData( uint8_t io ) ;
void    z80_io_setData( uint8_t io , uint8_t dataIn ) ;

bool z80_io_newData( uint8_t io ) ;

#endif // Z80_IO_H
