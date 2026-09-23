#include <z80_io.h>

#define Z80_IO_MAX                               256u

static struct
{
    uint8_t data ;
    bool    new  ;
} z80_io[ Z80_IO_MAX ] ;

void z80_io_initialization( void )
{
    for( uint32_t i = 0 ; i < Z80_IO_MAX ; i++ )
    {
        z80_io[ i ].data = 0x00 ;
        z80_io[ i ].new  = false ;
    }
}

uint8_t z80_io_getData( uint8_t io )
{
    uint8_t ret ;

    ret = z80_io[ io ].data ;
    z80_io[ io ].new = false ;

    return( ret ) ;
}

void z80_io_setData( uint8_t io , uint8_t dataIn )
{
    z80_io[ io ].data = dataIn ;
    z80_io[ io ].new  = true ;
}

bool z80_io_newData( uint8_t io )
{
    return( z80_io[ io ].new ) ;
}
