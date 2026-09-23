#include "z80_opcodes_extended_bits.h"

#include "z80_registers.h"
#include "z80_memory.h"

#include "z80_verbose.h"

void z80_opcode_Extended_Bits( uint8_t opCode )
{
    uint8_t subOpCode = z80_memory[ z80_Regs_GetAndIncPC() ] ;

    switch( subOpCode & 0xC0 ) // Check bits 7~6.
    {
        case 0x40 : // 01xx.xxxx - BIT
            /**********
             * Opcode - CD.40h - BIT
             * Size   - 2 Byte
             **********/
            z80_verbose_addOpcode( opCode ) ;
            z80_verbose_addOpcode( subOpCode ) ;
            z80_verbose_addMnemonic( "BIT" ) ;

            /**********
             *
             *      7     6     5     4     3     2     1     0
             *   +-----+-----+-----+-----+-----+-----+-----+-----+
             *   |  1  |  1  |  0  |  0  |  1  |  1  |  0  |  1  | CDh
             *   +-----+-----+-----+-----+-----+-----+-----+-----+
             *   |  0  |  1  |  x  |  x  |  x  |  x  |  x  |  x  |
             *   +-----+-----+-----+-----+-----+-----+-----+-----+
             *                \_______ _______/ \_______ _______/
             *                        |                 |
             *                        |                 +--> Register
             *                        |
             *                        +--> Bit number
             *
             **********/

            uint8_t bitNum = ( subOpCode >> 3 ) & 0x07 ;
            uint8_t bitMask = 0x01 << bitNum ;
            z80_verbose_addOperatorByte( bitNum , DIRECT ) ;

            eSelectReg_t eReg = ( eSelectReg_t ) ( subOpCode & 0x07 ) ;
            uint8_t tmp = z80_Regs_GetReg( eReg ) ;
            z80_verbose_addOperatorRegister( eReg , DIRECT ) ;

            /**********
             * Update Flags.
             **********/
            z80_Flags_CalculateZero( tmp & bitMask ) ;
            // z80_flags.s - Unknown.
            // z80_flags.pv - Unknown.
            z80_Flags_SetHalf() ;
            z80_Flags_ResetN() ;
            // z80_flags.c - Not affected.
            break;

        default :
            z80_verbose_addOpcode( opCode ) ;
            z80_verbose_addOpcode( subOpCode ) ;
            z80_verbose_addComment( "Not implemented!" ) ;
            break ;
    }
}
