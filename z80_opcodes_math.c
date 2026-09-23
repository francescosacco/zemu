#include "z80_opcodes_math.h"
#include "z80_registers.h"
#include "z80_memory.h"

#include "z80_verbose.h"

void z80_opcode_IncDecDReg( uint8_t opCode )
{
    /**********
     * Opcode - 03h - INC BC
     *          13h - INC DE
     *          23h - INC HL
     *          33h - INC SP
     *          0Bh - DEC BC
     *          1Bh - DEC DE
     *          2Bh - DEC HL
     *          3Bh - DEC SP
     * Size   - 1 Byte
     **********/
    z80_verbose_addOpcode( opCode ) ;

    eSelectDoubleReg_t eReg = ( eSelectDoubleReg_t ) ( ( opCode >> 4 ) & 0x03 );
    uint16_t val = z80_Regs_GetDReg( eReg ) ;

    if( opCode & 0x08 )
    {
        val-- ;
        z80_verbose_addMnemonic( "DEC" ) ;
    }
    else
    {
        val++ ;
        z80_verbose_addMnemonic( "INC" ) ;
    }

    z80_Regs_SetDReg( eReg , val ) ;
    z80_verbose_addOperatorDoubleRegister( eReg , DIRECT ) ;

    z80_verbose_addComment( "The final value is " ) ;
    z80_verbose_addCommentNumeric( val ) ;
}

void z80_opcode_IncDecReg( uint8_t opCode )
{
    uint8_t oldVal ;
    uint8_t newVal ;
    
    /**********
     * Opcode - 04h - INC B
     *          05h - DEC B
     *          0Ch - INC C
     *          0Dh - DEC C
     *          14h - INC D
     *          15h - DEC D
     *          1Ch - INC E
     *          1Dh - DEC E
     *          24h - INC H
     *          25h - DEC H
     *          2Ch - INC L
     *          2Dh - DEC L
     *          34h - INC (HL)
     *          35h - DEC (HL)
     *          3Ch - INC A
     *          3Dh - DEC A
     * Size   - 1 Byte
     *
     *     7     6     5     4     3     2     1     0
     *  +-----+-----+-----+-----+-----+-----+-----+-----+
     *  |  0  |  0  |       REG       |  1  |  0  | I/D |
     *  +-----+-----+-----+-----+-----+-----+-----+-----+
     *
     **********/
    z80_verbose_addOpcode( opCode ) ;
    
    // Load register.
    eSelectReg_t eReg = ( ( opCode >> 3 ) & 0x07 ) ;
    oldVal = z80_Regs_GetReg( eReg ) ;
    
    // Check opCode DEC or INC.
    if( opCode & 0x01 )
    {
        // Implementation for DEC.
        z80_verbose_addMnemonic( "DEC" ) ;
        
        // Execution.
        newVal = oldVal - 1 ;

        // Update flags.
        z80_Flags_SetN() ;
        z80_flags.pv = ( oldVal == 0x80 ) ;
        z80_flags.h  = ( oldVal & 0x0F ) == 0x00 ;
    }
    else
    {
        // Implementation for INC.
        z80_verbose_addMnemonic( "INC" ) ;

        // Execution.
        newVal = oldVal + 1 ;

        // Update flags.
        z80_Flags_ResetN() ;
        z80_flags.pv = ( oldVal == 0x7F ) ;
        z80_flags.h  = ( ( oldVal & 0x0F ) + 1 ) > 0x0F ;
    }

    // Save value incremented.
    z80_Regs_SetReg( eReg , newVal ) ;
    
    /**********
     * Update Flags.
     **********/
    z80_Flags_CalculateSign( newVal ) ;
    z80_Flags_CalculateZero( newVal ) ;
    // Carry flag is NOT affected by INC/DEC.

    z80_verbose_addOperatorRegister( eReg , DIRECT ) ;

    z80_verbose_addComment( "The final value is " ) ;
    z80_verbose_addCommentNumeric( ( int ) newVal ) ;
}

void z80_opcode_CP( uint8_t opCode )
{
    uint8_t result ;
    uint8_t tmp8 ;
    uint8_t regA ;

    /**********
     * Opcode - B8h - CP B
     *          B9h - CP C
     *          BAh - CP D
     *          BBh - CP E
     *          BCh - CP H
     *          BDh - CP L
     *          BEh - CP (HL)
     *          BFh - CP A
     * Size   - 1 Byte
     *
     *          FEh - CP xx
     * Size   - 2 Bytes
     **********/
    z80_verbose_addOpcode( opCode ) ;
    z80_verbose_addMnemonic( "CP" ) ;

    if( opCode == 0xFE )
    {
        tmp8 = z80_memory[ z80_Regs_GetAndIncPC() ] ;
        z80_verbose_addOperatorByte( tmp8 , DIRECT ) ;
    }
    else
    {
        eSelectReg_t eReg ;

        /**********
         * Memory Read!
         *
         *    7   6   5   4   3   2   1   0
         *  +---+---+---+---+---+---+---+---+
         *  | 1 | 0 | 1 | 1 | 1 | x | x | x |
         *  +---+---+---+---+---+---+---+---+
         *                       \____ ____/
         *                         000-B
         *                         001-C
         *                         010-D
         *                         011-E
         *                         100-H
         *                         101-L
         *                         110-(HL)
         *                         111-A
         **********/
        eReg = ( eSelectReg_t ) ( opCode & 0x07 ) ; // Bits 2~0.
        tmp8 = z80_Regs_GetReg( eReg ) ;

        z80_verbose_addOperatorRegister( eReg , DIRECT ) ;
    }

    /**********
     * Execute!
     **********/
    regA = z80_Regs_GetReg( eSelectReg_regA ) ;
    result = regA - tmp8 ;

    /**********
     * Update Flags.
     **********/
    z80_flags.c = ( regA < tmp8 ) ;
    z80_Flags_SetN() ;
    z80_Flags_CalculateOverflowSub( regA , tmp8 , result ) ;
    z80_Flags_CalculateHalf_sub( regA , tmp8 ) ;
    z80_Flags_CalculateZero( result ) ;
    z80_Flags_CalculateSign( result ) ;
}

void z80_opcode_ADD( uint8_t opCode )
{
    /**********
     * Opcode - 80h - ADD A,B
     *          81h - ADD A,C
     *          82h - ADD A,D
     *          83h - ADD A,E
     *          84h - ADD A,H
     *          85h - ADD A,L
     *          86h - ADD A,(HL)
     *          87h - ADD A,A
     *          88h - ADC A,B
     *          89h - ADC A,C
     *          8Ah - ADC A,D
     *          8Bh - ADC A,E
     *          8Ch - ADC A,H
     *          8Dh - ADC A,L
     *          8Eh - ADC A,(HL)
     *          8Fh - ADC A,A
     * Size   - 1 Byte
     **********/
    z80_verbose_addOpcode( opCode ) ;

    /**********
     * Memory Read!
     *
     *    7   6   5   4   3   2   1   0
     *  +---+---+---+---+---+---+---+---+
     *  | 1 | 0 | 0 | 0 | C | x | x | x |
     *  +---+---+---+---+---+---+---+---+
     *                   \ / \____ ____/
     *                    |    000-B
     *     1 - With C <---+    001-C
     *                         010-D
     *                         011-E
     *                         100-H
     *                         101-L
     *                         110-(HL)
     *                         111-A
     **********/

    eSelectReg_t eReg = ( eSelectReg_t ) ( opCode & 0x07 ) ;
    uint8_t tmp8 = z80_Regs_GetReg( eReg ) ;
    uint8_t tmpA = z80_Regs_GetReg( eSelectReg_regA ) ;
    bool tmpCarry = false ;

    uint16_t result16 = ( ( uint16_t ) tmpA ) + ( ( uint16_t ) tmp8 ) ;
    uint8_t result8 ;

    if( opCode & 0x08 )
    {
        // Implementation for ADC.
        
        tmpCarry = z80_Flags_GetCarry() ;
        result16 += ( uint16_t ) tmpCarry ;

        z80_verbose_addMnemonic( "ADC" ) ;
    }
    else
    {
        // Implementation for ADD.

        z80_verbose_addMnemonic( "ADD" ) ;
    }

    result8 = ( uint8_t ) result16 ;

    z80_Regs_SetReg( eSelectReg_regA , result8 ) ;

    z80_verbose_addOperatorRegister( eSelectReg_regA , DIRECT ) ;
    z80_verbose_addOperatorRegister( eReg            , DIRECT ) ;

    // Update flags.
    z80_Flags_CalculateHalf_adc( tmpA , tmp8 , tmpCarry ) ;
    z80_Flags_CalculateOverflowAdc( tmpA , tmp8 , tmpCarry , result8 ) ;
    z80_Flags_CalculateSign( result8 ) ;
    z80_Flags_CalculateZero( result8 ) ;
    z80_Flags_ResetN() ;
    z80_flags.c = ( ( result16 >> 8 ) != 0 ) ;
}

void z80_opcode_ADDConst( uint8_t opCode )
{
    /**********
     * Opcode - C6h - ADD A,xx
     * Size   - 2 Byte
     **********/
    z80_verbose_addOpcode( opCode ) ;
    z80_verbose_addMnemonic( "ADD" ) ;

    /**********
     * Memory Read!
     **********/

    int8_t tmp8 = ( int8_t ) z80_memory[ z80_Regs_GetAndIncPC() ] ;
    int8_t tmpA = ( int8_t ) z80_Regs_GetReg( eSelectReg_regA ) ;

    z80_verbose_addOpcode( ( uint8_t ) tmp8 ) ;
    z80_verbose_addOperatorRegister( eSelectReg_regA , DIRECT ) ;
    z80_verbose_addOperatorByte( ( uint8_t ) tmp8 , DIRECT ) ;

    int16_t tmp16 = ( ( int16_t ) tmpA ) + ( ( int16_t ) tmp8 ) ;

    z80_Regs_SetReg( eSelectReg_regA , ( uint8_t ) tmp16 ) ;

    z80_Flags_CalculateSign( ( uint8_t ) tmp16 ) ;
    z80_Flags_CalculateZero( ( uint8_t ) tmp16 ) ;
    z80_Flags_ResetN() ;
    z80_flags.c = ( tmp16 & 0xFF00 ) ? ( 1 ) : ( 0 ) ;
    z80_Flags_CalculateHalf_adc( ( uint8_t ) tmpA , ( uint8_t ) tmp8 , false ) ;
    z80_Flags_CalculateOverflow( tmp16 ) ;
}

void z80_opcode_SUB( uint8_t opCode )
{
    /**********
     * Opcode - 90h - SUB A,B
     *          91h - SUB A,C
     *          92h - SUB A,D
     *          93h - SUB A,E
     *          94h - SUB A,H
     *          95h - SUB A,L
     *          96h - SUB A,(HL)
     *          97h - SUB A,A
     *          98h - SBC A,B
     *          99h - SBC A,C
     *          9Ah - SBC A,D
     *          9Bh - SBC A,E
     *          9Ch - SBC A,H
     *          9Dh - SBC A,L
     *          9Eh - SBC A,(HL)
     *          9Fh - SBC A,A
     * Size   - 1 Byte
     **********/
    z80_verbose_addOpcode( opCode ) ;

    /**********
     * Memory Read!
     *
     *    7   6   5   4   3   2   1   0
     *  +---+---+---+---+---+---+---+---+
     *  | 1 | 0 | 0 | 1 | C | x | x | x |
     *  +---+---+---+---+---+---+---+---+
     *                   \ / \____ ____/
     *                    |    000-B
     *     1 - With C <---+    001-C
     *                         010-D
     *                         011-E
     *                         100-H
     *                         101-L
     *                         110-(HL)
     *                         111-A
     **********/

    eSelectReg_t eReg = ( eSelectReg_t ) ( opCode & 0x07 ) ;
    int8_t tmp8 = ( int8_t ) z80_Regs_GetReg( eReg ) ;
    int8_t tmpA = ( int8_t ) z80_Regs_GetReg( eSelectReg_regA ) ;

    int16_t tmp16 = ( ( int16_t ) tmpA ) - ( ( int16_t ) tmp8 ) ;
    if( opCode & 0x08 )
    {
        tmp16 -= ( int16_t ) z80_Flags_GetCarry() ;
        z80_verbose_addMnemonic( "SBC" ) ;
    }
    else
    {
        z80_verbose_addMnemonic( "SUB" ) ;
    }

    z80_Regs_SetReg( eSelectReg_regA , (uint8_t)tmp16) ;

    z80_verbose_addOperatorRegister( eSelectReg_regA , DIRECT ) ;
    z80_verbose_addOperatorRegister( eReg            , DIRECT ) ;

    z80_Flags_CalculateSign( ( uint8_t ) tmp16 ) ;
    z80_Flags_CalculateZero( ( uint8_t ) tmp16 ) ;
    z80_Flags_SetN() ;
    z80_flags.c = ( tmp16 & 0xFF00 ) ? ( 1 ) : ( 0 ) ;
    z80_Flags_CalculateHalf_sub( ( uint8_t ) tmpA , ( uint8_t ) tmp8 ) ;
    z80_Flags_CalculateOverflow( tmp16 ) ;
}

void z80_opcode_SUBConst( uint8_t opCode )
{
    /**********
     * Opcode - D6h - SUB A,xx
     * Size   - 2 Byte
     **********/
    z80_verbose_addOpcode( opCode ) ;
    z80_verbose_addMnemonic( "SUB" ) ;

    /**********
     * Memory Read!
     **********/

    int8_t tmp8 = ( int8_t ) z80_memory[ z80_Regs_GetAndIncPC() ] ;
    int8_t tmpA = ( int8_t ) z80_Regs_GetReg( eSelectReg_regA ) ;

    z80_verbose_addOperatorRegister( eSelectReg_regA , DIRECT ) ;
    z80_verbose_addOperatorByte( ( uint8_t ) tmp8 , DIRECT ) ;

    int16_t tmp16 = ( ( int16_t ) tmpA ) - ( ( int16_t ) tmp8 ) ;

    z80_Regs_SetReg( eSelectReg_regA , (uint8_t)tmp16) ;

    z80_Flags_CalculateSign( ( uint8_t ) tmp16 ) ;
    z80_Flags_CalculateZero( ( uint8_t ) tmp16 ) ;
    z80_Flags_SetN() ;
    z80_flags.c = ( tmp16 & 0xFF00 ) ? ( 1 ) : ( 0 ) ;
    z80_Flags_CalculateHalf_sub( ( uint8_t ) tmpA , ( uint8_t ) tmp8 ) ;
    z80_Flags_CalculateOverflow( tmp16 ) ;
}
