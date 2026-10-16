#include "z80_opcodes_extended_iy.h"

#include "z80_registers.h"
#include "z80_memory.h"

#include "z80_verbose.h"

#include <stdlib.h> // for exit().

void z80_opcode_Extended_IY( uint8_t opCode )
{
    uint8_t subOpCode = z80_memory[ z80_Regs_GetAndIncPC() ] ;
    uint16_t tmp16;
    uint8_t tmp8 ;

    switch(subOpCode)
    {
        default :
            z80_verbose_addOpcode( opCode ) ;
            z80_verbose_addOpcode( subOpCode ) ;
            z80_verbose_addComment( "Not implemented!" ) ;
            exit( 0 ) ;
            break ;
    }

}
