#include "z80_opcodes_logic.h"
#include "z80_registers.h"
#include "z80_memory.h"

#include "z80_verbose.h"

void z80_opcode_Logic( uint8_t opCode )
{
    uint8_t tmp ;

    /**********
     * Opcode - A0h - AND A,Reg
     *          A8h - XOR A,Reg
     *          B0h - OR  A,Reg
     * Size   - 1 Byte
     *
     *   7   6   5   4   3   2   1   0
     * +---+---+---+---+---+---+---+---+
     * | 1 | 0 | 1 | x | x | x | x | x |
     * +---+---+---+---+---+---+---+---+
     *              \__ __/ \____ ____/
     *               00-AND    000-B
     *               01-XOR    001-C
     *               10-OR     010-D
     *                         011-E
     *                         100-H
     *                         101-L
     *                         110-(HL)
     *                         111-A
     *
     * Flags -       SZ5H3PNC
     *         AND   **513P00
     *         XOR   **503P00
     *         OR    **503P00
     **********/
    z80_verbose_addOpcode( opCode ) ;

    /**********
     * Memory Read!
     **********/
    eSelectReg_t eReg = ( eSelectReg_t ) ( opCode & 0x07 ) ;

    // Read Accumulator.
    tmp = z80_Regs_GetReg( eSelectReg_regA ) ;

    /**********
     * Execute!
     **********/
    switch( ( opCode >> 3 ) & 0x03 ) // Bits 4~3.
    {
        case 0x00 : // AND
            tmp &= z80_Regs_GetReg( eReg ) ;
            z80_Flags_SetHalf() ;

            z80_verbose_addMnemonic( "AND" ) ;
            break ;
        case 0x01 : // XOR
            tmp ^= z80_Regs_GetReg( eReg ) ;
            z80_Flags_ResetHalf() ;

            z80_verbose_addMnemonic( "XOR" ) ;
            break ;
        case 0x02 : // OR
            tmp |= z80_Regs_GetReg( eReg ) ;
            z80_Flags_ResetHalf() ;

            z80_verbose_addMnemonic( "OR" ) ;
            break ;
    }

    z80_verbose_addOperatorRegister( eSelectReg_regA , DIRECT ) ;
    z80_verbose_addOperatorRegister( eReg            , DIRECT ) ;

    /**********
     * Memory Write!
     **********/

    // Write Accumulator.
    z80_Regs_SetReg( eSelectReg_regA , tmp ) ;

    z80_verbose_addComment( "Final value of A is " ) ;
    z80_verbose_addCommentByte( tmp ) ;

    /**********
     * Update Flags.
     **********/
    z80_Flags_CalculateSign( tmp ) ;
    z80_Flags_CalculateZero( tmp ) ;
    z80_Flags_CalculateParity( tmp ) ;
    z80_Flags_ResetN() ;
    z80_Flags_ResetCarry() ;
}

void z80_opcode_LogicConst( uint8_t opCode )
{
    uint8_t tmpA ;
    uint8_t tmp8 ;

    /**********
     * Opcode - E6h - AND A,XX
     *          EEh - XOR A,XX
     *          F6h - OR  A,XX
     * Size   - 2 Bytes
     *
     *   7   6   5   4   3   2   1   0
     * +---+---+---+---+---+---+---+---+
     * | 1 | 1 | 1 | x | x | 1 | 1 | 0 |
     * +---+---+---+---+---+---+---+---+
     *              \__ __/
     *               00-AND
     *               01-XOR
     *               10-OR
     *
     * Flags -       SZ5H3PNC
     *         AND   **513P00
     *         XOR   **503P00
     *         OR    **503P00
     **********/
    z80_verbose_addOpcode( opCode ) ;

    /**********
     * Memory Read!
     **********/

    tmpA = z80_Regs_GetReg( eSelectReg_regA ) ;

    tmp8 = z80_memory[ z80_Regs_GetAndIncPC() ] ;
    z80_verbose_addOpcode( tmp8 ) ;

    /**********
     * Execute!
     **********/
    switch( ( opCode >> 3 ) & 0x03 ) // Bits 4~3.
    {
        case 0x00 : // AND
            tmpA &= tmp8 ;
            z80_Flags_SetHalf() ;

            z80_verbose_addMnemonic( "AND" ) ;
            break ;
        case 0x01 : // XOR
            tmpA ^= tmp8 ;
            z80_Flags_ResetHalf() ;

            z80_verbose_addMnemonic( "XOR" ) ;
            break ;
        case 0x02 : // OR
            tmpA |= tmp8 ;
            z80_Flags_ResetHalf() ;

            z80_verbose_addMnemonic( "OR" ) ;
            break ;
    }

    /**********
     * Memory Write!
     **********/

    // Write Accumulator.
    z80_Regs_SetReg( eSelectReg_regA , tmpA ) ;

    z80_verbose_addOperatorRegister( eSelectReg_regA , DIRECT ) ;
    z80_verbose_addOperatorByte( tmp8 , DIRECT ) ;

    z80_verbose_addComment( "Final value of A is " ) ;
    z80_verbose_addCommentByte( tmpA ) ;

    /**********
     * Update Flags.
     **********/
    z80_Flags_CalculateSign( tmpA ) ;
    z80_Flags_CalculateZero( tmpA ) ;
    z80_Flags_CalculateParity( tmpA ) ;
    z80_Flags_ResetN() ;
    z80_Flags_ResetCarry() ;
}
