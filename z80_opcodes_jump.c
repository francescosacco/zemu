#include "z80_opcodes_jump.h"
#include "z80_registers.h"
#include "z80_memory.h"

#include "z80_verbose.h"

void z80_opcode_JUMP( uint8_t opCode )
{
    uint16_t addr ;
    bool shouldJump = false ;

    /**********
     * Opcode - C2h - JP NZ,xxxx
     *          C3h - JP xxxx
     *          CAh - JP Z,xxxx
     *          D2h - JP NC,xxxx
     *          DAh - JP C,xxxx
     *          E2h - JP NPV,xxxx
     *          EAh - JP PV,xxxx
     *          F2h - JP NS,xxxx
     *          FAh - JP S,xxxx
     * Size   - 3 Byte
     **********/
    z80_verbose_addOpcode( opCode ) ;
    z80_verbose_addMnemonic( "JP" ) ;

    /**********
     * Memory Read!
     *
     *      7     6     5     4     3     2     1     0
     *   +-----+-----+-----+-----+-----+-----+-----+-----+
     *   |  1  |  1  |  x  |  x  |  x  |  0  |  1  |  x  |
     *   +-----+-----+-----+-----+-----+-----+-----+-----+
     *                \____ ____/ \_ _/             \_ _/
     *                     |        |                 |
     *                     |        |                 +--> 0 - Conditional
     *                     |        |                      1 - Inconditional
     *                     |        |
     *                     |        +--> 0 Not Flag
     *                     |             1 Flag
     *                     |
     *                     +--> 00 - Zero Flag
     *                          01 - Carry Flag
     *                          10 - Parity/OverFlow Flag
     *                          11 - Sign Flag
     *
     **********/
    addr  =   ( uint16_t ) z80_memory[ z80_Regs_GetAndIncPC() ] ;
    addr |= ( ( uint16_t ) z80_memory[ z80_Regs_GetAndIncPC() ] ) << 8 ;

    /**********
     * Execute!
     **********/
    // Is the jump unconditional?
    if( opCode & 0x01 ) // Bit 0.
    {
        // Yes.
        shouldJump = true ;
        z80_verbose_addOperatorWord( addr , DIRECT ) ;
    }
    else
    {
        switch( opCode & 0x38 ) // Check bits 5~3.
        {
            case 0x00 : // xx00.0xxx
                shouldJump = !z80_Flags_GetZero() ;
                z80_verbose_addOperator( "NZ" , DIRECT ) ;
                break ;
            case 0x08 : // xx00.1xxx
                shouldJump = z80_Flags_GetZero() ;
                z80_verbose_addOperator( "Z" , DIRECT ) ;
                break ;
            case 0x10 : // xx01.0xxx
                shouldJump = !z80_Flags_GetCarry() ;
                z80_verbose_addOperator( "NC" , DIRECT ) ;
                break ;
            case 0x18 : // xx01.1xxx
                shouldJump = z80_Flags_GetCarry() ;
                z80_verbose_addOperator( "C" , DIRECT ) ;
                break ;
            case 0x20 : // xx10.0xxx
                shouldJump = !z80_Flags_GetPV() ;
                z80_verbose_addOperator( "NPV" , DIRECT ) ;
                break ;
            case 0x28 : // xx10.1xxx
                shouldJump = z80_Flags_GetPV() ;
                z80_verbose_addOperator( "PV" , DIRECT ) ;
                break ;
            case 0x30 : // xx11.0xxx
                shouldJump = !z80_Flags_GetSign() ;
                z80_verbose_addOperator( "NS" , DIRECT ) ;
                break ;
            case 0x38 : // xx11.1xxx
                shouldJump = z80_Flags_GetSign() ;
                z80_verbose_addOperator( "S" , DIRECT ) ;
                break ;
        }

        z80_verbose_addOperatorWord( addr , DIRECT ) ;
    }

    /**********
     * Memory Write!
     **********/
    if( shouldJump )
    {
        z80_Regs_SetPC( addr ) ;
        z80_verbose_addComment( "It jumped." ) ;
    }
    else
    {
        z80_verbose_addComment( "It didn't jump." ) ;
    }
}

void z80_opcode_JR( uint8_t opCode )
{
    int8_t tmp ;
    bool shouldJump    = false ;

    /**********
     * Opcode - 18h - JR xx
     *          20h - JR NZ,xx
     *          28h - JR Z,xx
     *          30h - JR NC,xx
     *          38h - JR C,xx
     * Size   - 2 Byte
     **********/
    z80_verbose_addOpcode( opCode ) ;
    z80_verbose_addMnemonic( "JR" ) ;

    /**********
     * Memory Read!
     **********/
    tmp  = ( int8_t ) z80_memory[ z80_Regs_GetAndIncPC() ] ;
    z80_verbose_addOpcode( ( uint8_t ) tmp ) ;

    /**********
     * Execute!
     **********/
    // Is the jump unconditional?
    if( opCode == 0x18 )
    {
        // Yes.
        shouldJump = true ;
    }
    else
    {
        switch( opCode )
        {
            case 0x20 : // JR NZ,xx
                z80_verbose_addOperator( "NZ" , DIRECT ) ;
                shouldJump = !z80_Flags_GetZero() ;
                break ;
            case 0x28 : // JR Z,xx
                z80_verbose_addOperator( "Z" , DIRECT ) ;
                shouldJump = z80_Flags_GetZero() ;
                break ;
            case 0x30 : // JR NC,xx
                z80_verbose_addOperator( "NC" , DIRECT ) ;
                shouldJump = !z80_Flags_GetCarry() ;
                break ;
            case 0x38 : // JR C,xx
                z80_verbose_addOperator( "C" , DIRECT ) ;
                shouldJump = z80_Flags_GetCarry() ;
                break ;
        }

    }

    z80_verbose_addOperatorByte( ( uint8_t ) tmp , DIRECT ) ;


    /**********
     * Memory Write!
     **********/
    if( shouldJump )
    {
        int16_t read_pc = ( int16_t ) z80_Regs_GetPC() ;
        read_pc += ( int16_t ) tmp ;
        z80_Regs_SetPC( ( uint16_t ) read_pc ) ;

        z80_verbose_addComment( "It jumped." ) ;
    }
    else
    {
        z80_verbose_addComment( "It didn't jump." ) ;
    }
}

void z80_opcode_CALL( uint8_t opCode )
{
    uint16_t addr ;

    bool shouldJump    = false ;

    /**********
     * Opcode - C4h - CALL NZ,xxxx
     *          CCh - CALL Z,xxxx
     *          CDh - CALL xxxx
     *          D4h - CALL NC,xxxx
     *          DCh - CALL C,xxxx
     *          E4h - CALL NPV,xxxx
     *          ECh - CALL PV,xxxx
     *          F4h - CALL NS,xxxx
     *          FCh - CALL S,xxxx
     * Size   - 3 Byte
     **********/
    z80_verbose_addOpcode( opCode ) ;
    z80_verbose_addMnemonic( "CALL" ) ;

    /**********
     * Memory Read!
     *
     *      7     6     5     4     3     2     1     0
     *   +-----+-----+-----+-----+-----+-----+-----+-----+
     *   |  1  |  1  |  x  |  x  |  x  |  1  |  0  |  x  |
     *   +-----+-----+-----+-----+-----+-----+-----+-----+
     *                \____ ____/ \_ _/             \_ _/
     *                     |        |                 |
     *                     |        |                 +--> 0 - Conditional
     *                     |        |                      1 - Inconditional
     *                     |        |
     *                     |        +--> 0 Not Flag
     *                     |             1 Flag
     *                     |
     *                     +--> 00 - Zero Flag
     *                          01 - Carry Flag
     *                          10 - Parity/OverFlow Flag
     *                          11 - Sign Flag
     *
     **********/
    addr  =   ( uint16_t ) z80_memory[ z80_Regs_GetAndIncPC() ] ;
    addr |= ( ( uint16_t ) z80_memory[ z80_Regs_GetAndIncPC() ] ) << 8 ;

    /**********
     * Execute!
     **********/
    // Is the jump unconditional?
    if( opCode & 0x01 ) // Bit 0.
    {
        // Yes.
        shouldJump = true ;
    }
    else
    {
        switch( opCode & 0x38 ) // Check bits 5~3.
        {
            case 0x00 : // xx00.0xxx
                shouldJump = !z80_Flags_GetZero() ;
                z80_verbose_addOperator( "NZ" , DIRECT ) ;
                break ;
            case 0x08 : // xx00.1xxx
                shouldJump = z80_Flags_GetZero() ;
                z80_verbose_addOperator( "Z" , DIRECT ) ;
                break ;
            case 0x10 : // xx01.0xxx
                shouldJump = !z80_Flags_GetCarry() ;
                z80_verbose_addOperator( "NC" , DIRECT ) ;
                break ;
            case 0x18 : // xx01.1xxx
                shouldJump = z80_Flags_GetCarry() ;
                z80_verbose_addOperator( "C" , DIRECT ) ;
                break ;
            case 0x20 : // xx10.0xxx
                shouldJump = !z80_Flags_GetPV() ;
                z80_verbose_addOperator( "NPV" , DIRECT ) ;
                break ;
            case 0x28 : // xx10.1xxx
                shouldJump = z80_Flags_GetPV() ;
                z80_verbose_addOperator( "PV" , DIRECT ) ;
                break ;
            case 0x30 : // xx11.0xxx
                shouldJump = !z80_Flags_GetSign() ;
                z80_verbose_addOperator( "NS" , DIRECT ) ;
                break ;
            case 0x38 : // xx11.1xxx
                shouldJump = z80_Flags_GetSign() ;
                z80_verbose_addOperator( "S" , DIRECT ) ;
                break ;
        }
    }

    z80_verbose_addOperatorWord( addr , DIRECT ) ;

    /**********
     * Memory Write!
     **********/
    if( shouldJump )
    {
        z80_memory[ --z80_SP ] = ( uint8_t ) z80_Regs_GetPC() >> 8 ;
        z80_memory[ --z80_SP ] = ( uint8_t ) z80_Regs_GetPC()      ;

        z80_Regs_SetPC( addr ) ;

        z80_verbose_addComment( "It jumped." ) ;
    }
    else
    {
        z80_verbose_addComment( "It didn't jump." ) ;
    }
}

void z80_opcode_RET( uint8_t opCode )
{
    uint16_t addr ;

    bool shouldJump    = false ;

    /**********
     * Opcode - C0h - RET NZ,xxxx
     *          C8h - RET Z,xxxx
     *          C9h - RET xxxx
     *          D0h - RET NC,xxxx
     *          D8h - RET C,xxxx
     *          E0h - RET NPV,xxxx
     *          E8h - RET PV,xxxx
     *          F0h - RET NS,xxxx
     *          F8h - RET S,xxxx
     * Size   - 1 Byte
     **********/
    z80_verbose_addOpcode( opCode ) ;
    z80_verbose_addMnemonic( "RET" ) ;

    /**********
     * Memory Read!
     *
     *      7     6     5     4     3     2     1     0
     *   +-----+-----+-----+-----+-----+-----+-----+-----+
     *   |  1  |  1  |  x  |  x  |  x  |  0  |  0  |  x  |
     *   +-----+-----+-----+-----+-----+-----+-----+-----+
     *                \____ ____/ \_ _/             \_ _/
     *                     |        |                 |
     *                     |        |                 +--> 0 - Conditional
     *                     |        |                      1 - Inconditional
     *                     |        |
     *                     |        +--> 0 Not Flag
     *                     |             1 Flag
     *                     |
     *                     +--> 00 - Zero Flag
     *                          01 - Carry Flag
     *                          10 - Parity/OverFlow Flag
     *                          11 - Sign Flag
     *
     **********/

    /**********
     * Execute!
     **********/
    // Is the jump unconditional?
    if( opCode & 0x01 ) // Bit 0.
    {
        // Yes.
        shouldJump = true ;
    }
    else
    {
        switch( opCode & 0x38 ) // Check bits 5~3.
        {
            case 0x00 : // xx00.0xxx
                shouldJump = !z80_Flags_GetZero() ;
                z80_verbose_addOperator( "NZ" , DIRECT ) ;
                break ;
            case 0x08 : // xx00.1xxx
                shouldJump = z80_Flags_GetZero() ;
                z80_verbose_addOperator( "Z" , DIRECT ) ;
                break ;
            case 0x10 : // xx01.0xxx
                shouldJump = !z80_Flags_GetCarry() ;
                z80_verbose_addOperator( "NC" , DIRECT ) ;
                break ;
            case 0x18 : // xx01.1xxx
                shouldJump = z80_Flags_GetCarry() ;
                z80_verbose_addOperator( "C" , DIRECT ) ;
                break ;
            case 0x20 : // xx10.0xxx
                shouldJump = !z80_Flags_GetPV() ;
                z80_verbose_addOperator( "NPV" , DIRECT ) ;
                break ;
            case 0x28 : // xx10.1xxx
                shouldJump = z80_Flags_GetPV() ;
                z80_verbose_addOperator( "PV" , DIRECT ) ;
                break ;
            case 0x30 : // xx11.0xxx
                shouldJump = !z80_Flags_GetSign() ;
                z80_verbose_addOperator( "NS" , DIRECT ) ;
                break ;
            case 0x38 : // xx11.1xxx
                shouldJump = z80_Flags_GetSign() ;
                z80_verbose_addOperator( "S" , DIRECT ) ;
                break ;
        }
    }

    z80_verbose_addOperatorWord( addr , DIRECT ) ;

    /**********
     * Memory Write!
     **********/
    if( shouldJump )
    {
        addr  =   ( uint16_t ) z80_memory[ z80_SP++ ]        ; // LSB
        addr |= ( ( uint16_t ) z80_memory[ z80_SP++ ] ) << 8 ; // MSB

        z80_Regs_SetPC( addr ) ;

        z80_verbose_addComment( "It jumped." ) ;
    }
    else
    {
        z80_verbose_addComment( "It didn't jump." ) ;
    }
}

void z80_opcode_RST( uint8_t opCode )
{
    uint16_t addr ;

    /**********
     * Opcode - C7h - RST 00h
     *          CFh - RST 08h
     *          D7h - RST 10h
     *          DFh - RST 18h
     *          E7h - RST 20h
     *          EFh - RST 28h
     *          F7h - RST 30h
     *          FFh - RST 38h
     * Size   - 1 Byte
     **********/
    z80_verbose_addOpcode( opCode ) ;
    z80_verbose_addMnemonic( "RST" ) ;

    /**********
     * Memory Read!
     *
     *      7     6     5     4     3     2     1     0
     *   +-----+-----+-----+-----+-----+-----+-----+-----+
     *   |  1  |  1  |  x  |  x  |  x  |  1  |  1  |  1  |
     *   +-----+-----+-----+-----+-----+-----+-----+-----+
     *                \_______ _______/
     *                        |
     *                        +--> 000 - 00h
     *                             001 - 08h
     *                             010 - 10h
     *                             011 - 18h
     *                             100 - 20h
     *                             101 - 28h
     *                             110 - 30h
     *                             111 - 38h
     *
     **********/
    addr  = opCode & 0x38 ; // Bits 5~3.

    /**********
     * Memory Write!
     **********/
    z80_memory[ --z80_SP ] = ( uint8_t ) z80_Regs_GetPC() >> 8 ;
    z80_memory[ --z80_SP ] = ( uint8_t ) z80_Regs_GetPC()      ;

    z80_Regs_SetPC( addr ) ;
    z80_verbose_addOperatorWord( addr , DIRECT ) ;
}

void z80_opcode_DJNZ( uint8_t opCode )
{
    int8_t relativeAddr ;
    uint8_t data ;

    /**********
     * Opcode - 10h - DJNZ xx
     * Size   - 2 Byte
     **********/
    z80_verbose_addOpcode( opCode ) ;
    z80_verbose_addMnemonic( "DJNZ" ) ;

    /**********
     * Memory Read!
     **********/
    relativeAddr  = ( int8_t ) z80_memory[ z80_Regs_GetAndIncPC() ] ;
    z80_verbose_addOpcode( ( uint8_t ) relativeAddr ) ;

    /**********
     * Execute!
     **********/

    data = z80_Regs_GetReg( eSelectReg_regB ) ;
    data-- ;
    z80_Regs_SetReg( eSelectReg_regB , data ) ;

    z80_verbose_addOperatorByte( ( uint8_t ) relativeAddr , DIRECT ) ;

    /**********
     * Memory Write!
     **********/
    if( data != 0 )
    {
        int16_t read_pc = ( int16_t ) z80_Regs_GetPC() ;
        read_pc += ( int16_t ) relativeAddr ;
        z80_Regs_SetPC( ( uint16_t ) read_pc ) ;

        z80_verbose_addComment( "B is " ) ;
        z80_verbose_addCommentByte( data ) ;
        z80_verbose_addComment( ". It jumped." ) ;
    }
    else
    {
        z80_verbose_addComment( "B is " ) ;
        z80_verbose_addCommentByte( data ) ;
        z80_verbose_addComment( ". It didn't jump." ) ;
    }
}
