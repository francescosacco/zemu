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
             * Opcode - CB.40h - BIT 0,B
             *          CB.41h - BIT 0,C
             *          CB.42h - BIT 0,D
             *          CB.43h - BIT 0,E
             *          CB.44h - BIT 0,H
             *          CB.45h - BIT 0,L
             *          CB.46h - BIT 0,(HL)
             *          CB.47h - BIT 0,A
             *          CB.48h - BIT 1,B
             *          CB.49h - BIT 1,C
             *          CB.4Ah - BIT 1,D
             *          CB.4Bh - BIT 1,E
             *          CB.4Ch - BIT 1,H
             *          CB.4Dh - BIT 1,L
             *          CB.4Eh - BIT 1,(HL)
             *          CB.4Fh - BIT 1,A
             * Size   - 2 Byte
             **********/
            z80_verbose_addOpcode( opCode ) ;
            z80_verbose_addOpcode( subOpCode ) ;
            z80_verbose_addMnemonic( "BIT" ) ;

            /**********
             *
             *     7   6   5   4   3   2   1   0
             *   +---+---+---+---+---+---+---+---+
             *   | 1 | 1 | 0 | 0 | 1 | 1 | 0 | 1 | CDh
             *   +---+---+---+---+---+---+---+---+
             *   | 0 | 1 | N. of Bit |    Reg    |
             *   +---+---+---+---+---+---+---+---+
             *
             **********/

            // Bit selection.
            uint8_t bitNum = ( subOpCode >> 3 ) & 0x07 ;
            uint8_t bitMask = 0x01 << bitNum ;

            z80_verbose_addOperatorByte( bitNum , DIRECT ) ;

            // Register selection.
            eSelectReg_t eReg = ( eSelectReg_t ) ( subOpCode & 0x07 ) ;
            uint8_t tmp = z80_Regs_GetReg( eReg ) ;

            z80_verbose_addOperatorRegister( eReg , DIRECT ) ;

            /**********
             * FLAGS.
             **********/

            // --- Carry -------------
            // No change.

            // --- Add/Sub -----------
            z80_Flags_ResetN() ;

            // --- Parity/OverFlow ---
            // Undefined.

            // --- X3 ----------------
            // No change.

            // --- Half Carry Flag ---
            z80_Flags_SetHalf() ;

            // --- X5 ----------------
            // No change.

            // --- Zero Flag ---------
            z80_flags.z = ( tmp & bitMask ) == 0x00u ;

            // --- Sign Flag ---------
            // Undefined.

            break;

        default :
            z80_verbose_addOpcode( opCode ) ;
            z80_verbose_addOpcode( subOpCode ) ;
            z80_verbose_addComment( "Not implemented!" ) ;
            break ;
    }
}
