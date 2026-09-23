#include "z80_opcodes_extended_misc.h"

#include "z80_registers.h"
#include "z80_memory.h"

#include "z80_verbose.h"

void z80_opcode_Extended_Misc( uint8_t opCode )
{
    uint8_t subOpCode ;

    subOpCode  = z80_memory[ z80_Regs_GetAndIncPC() ] ;

    switch( subOpCode )
    {
        case 0x46 : // IM 0
            /**********
             * Opcode - ED.46h - IM 0
             * Size   - 2 Byte
             **********/
            z80_verbose_addOpcode( opCode ) ;
            z80_verbose_addOpcode( subOpCode ) ;
            z80_verbose_addMnemonic( "IM" ) ;

            z80_Regs_SetIM( eInterruptMode_0 ) ;
            z80_verbose_addOperator( "0" , DIRECT ) ;
            break ;

        case 0x56 : // IM 1
            /**********
             * Opcode - ED.56h - IM 1
             * Size   - 2 Byte
             **********/
            z80_verbose_addOpcode( opCode ) ;
            z80_verbose_addOpcode( subOpCode ) ;
            z80_verbose_addMnemonic( "IM" ) ;

            z80_Regs_SetIM( eInterruptMode_1 ) ;
            z80_verbose_addOperator( "1" , DIRECT ) ;
            break ;

        case 0x5E : // IM 2
            /**********
             * Opcode - ED.5Eh - IM 2
             * Size   - 2 Byte
             **********/
            z80_verbose_addOpcode( opCode ) ;
            z80_verbose_addOpcode( subOpCode ) ;
            z80_verbose_addMnemonic( "IM" ) ;

            z80_Regs_SetIM( eInterruptMode_2 ) ;
            z80_verbose_addOperator( "2" , DIRECT ) ;
            break ;

        case 0x4D : // RETI
            /**********
             * Opcode - ED.4Dh - RETI
             * Size   - 2 Byte
             **********/
            z80_verbose_addOpcode( opCode ) ;
            z80_verbose_addOpcode( subOpCode ) ;
            z80_verbose_addMnemonic( "RETI" ) ;

            uint16_t tmp16 ;

            tmp16  =   ( uint16_t ) z80_memory[ z80_SP++ ]        ; // LSB
            tmp16 |= ( ( uint16_t ) z80_memory[ z80_SP++ ] ) << 8 ; // MSB

            z80_Regs_SetPC( tmp16 ) ;
            break ;

        default:
            z80_verbose_addOpcode( opCode ) ;
            z80_verbose_addOpcode( subOpCode ) ;
            z80_verbose_addComment( "Not implemented!" ) ;
            break ;
    }
}
