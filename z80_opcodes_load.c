#include "z80_opcodes_load.h"
#include "z80_registers.h"
#include "z80_memory.h"

#include "z80_verbose.h"

void z80_opcode_LoadRegC( uint8_t opCode )
{
    uint8_t data ;

    /**********
     * Opcode - 06h - LD B,xx
     *          0Eh - LD C,xx
     *          16h - LD D,xx
     *          1Eh - LD E,xx
     *          26h - LD H,xx
     *          2Eh - LD L,xx
     *          36h - LD (HL),xx
     *          3Eh - LD A,xx
     * Size   - 2 Byte
     **********/
    z80_verbose_addOpcode( opCode ) ;
    z80_verbose_addMnemonic( "LD" ) ;

    /**********
     * Memory Read!
     **********/
    data = z80_memory[ z80_Regs_GetAndIncPC() ] ;

    /**********
     * Execute!
     **********/
    eSelectReg_t eReg = ( eSelectReg_t ) ( ( opCode >> 3 ) & 0x07 ) ; // Bits 5~3.

    z80_verbose_addOperatorRegister( eReg , DIRECT ) ;
    z80_verbose_addOperatorByte( data , DIRECT ) ;

    /**********
     * Memory Write!
     **********/
    z80_Regs_SetReg( eReg , data ) ;
}

void z80_opcode_LoadRegReg( uint8_t opCode )
{
    uint8_t tmp ;

    /**********
     * Opcode - 40h - LD B,Reg
     *          48h - LD C,Reg
     *          50h - LD D,Reg
     *          58h - LD E,Reg
     *          60h - LD H,Reg
     *          68h - LD L,Reg
     *          70h - LD (HL),Reg
     *          78h - LD A,Reg
     * Size   - 1 Byte
     **********/
    z80_verbose_addOpcode( opCode ) ;
    z80_verbose_addMnemonic( "LD" ) ;

    /**********
     * Memory Read!
     *
     *   7   6   5   4   3   2   1   0
     * +---+---+---+---+---+---+---+---+
     * | 0 | 1 | x | x | x | x | x | x |
     * +---+---+---+---+---+---+---+---+
     *          \____ ____/ \____ ____/
     *           000 - B     000 - B
     *           001 - C     001 - C
     *           010 - D     010 - D
     *           011 - E     011 - E
     *           100 - H     100 - H
     *           101 - L     101 - L
     *           110 - (HL)  110 - (HL)
     *           111 - A     111 - A
     *
     **********/
    eSelectReg_t regOrigin = (   opCode        & 0x07 ) ; // Bits 2~0.
    eSelectReg_t regDestin = ( ( opCode >> 3 ) & 0x07 ) ; // Bits 5~3.

    z80_verbose_addOperatorRegister( regDestin , DIRECT ) ;
    z80_verbose_addOperatorRegister( regOrigin , DIRECT ) ;

    tmp = z80_Regs_GetReg( regOrigin ) ;

    /**********
     * Memory Write!
     **********/
    z80_Regs_SetReg( regDestin , tmp ) ;
}

void z80_opcode_LoadDRegC( uint8_t opCode )
{
    eSelectDoubleReg_t eReg ;
    uint16_t tmp ;

    /**********
     * Opcode - 01h - LD BC,xxxx
     *          11h - LD DE,xxxx
     *          21h - LD HL,xxxx
     *          31h - LD SP,xxxx
     * Size   - 3 Byte
     **********/
    z80_verbose_addOpcode( opCode ) ;
    z80_verbose_addMnemonic( "LD" ) ;

    /**********
     * Memory Read!
     **********/
    tmp  =   ( uint16_t ) z80_memory[ z80_Regs_GetAndIncPC() ] ;
    tmp |= ( ( uint16_t ) z80_memory[ z80_Regs_GetAndIncPC() ] ) << 8 ;

    /**********
     * Execute!
     **********/
    eReg = ( eSelectDoubleReg_t ) ( ( opCode >> 4 ) & 0x03 ) ;
    z80_verbose_addOperatorDoubleRegister( eReg , DIRECT ) ;
    z80_verbose_addOperatorWord( tmp , DIRECT ) ;

    /**********
     * Memory Write!
     **********/
    z80_Regs_SetDReg( eReg , tmp ) ;
}

void z80_opcode_LoadAInd( uint8_t opCode )
{
    eSelectDoubleReg_t eReg ;
    uint16_t addr ;
    uint8_t tmp ;

    /**********
     * Opcode - 02h - LD (BC),A
     *          0Ah - LD A,(BC)
     *          12h - LD (DE),A
     *          1Ah - LD A,(DE)
     * Size   - 1 Byte
     **********/
    z80_verbose_addOpcode( opCode ) ;
    z80_verbose_addMnemonic( "LD" ) ;

    /**********
     * Memory Read!
     *
     *    7   6   5   4   3   2   1   0
     *  +---+---+---+---+---+---+---+---+
     *  | 0 | 0 | x | x | x | 0 | 1 | 0 |
     *  +---+---+---+---+---+---+---+---+
     *           \__ __/ \ /
     *              |     |
     *              |     +--> 0 = (ind)<-A / 1 = A<-(ind)
     *              +--> 00 - BC
     *                   01 - DE
     *
     **********/
    eReg = ( eSelectDoubleReg_t ) ( ( opCode & 0x30 ) >> 4 ) ; // Bits 5~4.
    addr = z80_Regs_GetDReg( eReg ) ;

    /**********
     * Execute!
     **********/
    // Nothing to do.

    /**********
     * Memory Write!
     **********/
    // Check direction.
    if( opCode & 0x08 )
    {
        // A <- (ind)
        tmp = z80_memory[ addr ] ;
        z80_Regs_SetReg( eSelectReg_regA , tmp ) ;

        z80_verbose_addOperatorRegister( eSelectReg_regA , DIRECT ) ;
        z80_verbose_addOperatorDoubleRegister( eReg , DIRECT ) ;
    }
    else
    {
        // (ind) <- A
        tmp = z80_Regs_GetReg( eSelectReg_regA ) ;
        z80_memory[ addr ] = tmp ;

        z80_verbose_addOperatorDoubleRegister( eReg , DIRECT ) ;
        z80_verbose_addOperatorRegister( eSelectReg_regA , DIRECT ) ;
    }
}

void z80_opcode_LoadAMem( uint8_t opCode )
{
    uint16_t addr ;
    uint8_t  tmp ;

    /**********
     * Opcode - 32h - LD (xxxx),A
     *          3Ah - LD A,(xxxx)
     * Size   - 3 Byte
     **********/
    z80_verbose_addOpcode( opCode ) ;
    z80_verbose_addMnemonic( "LD" ) ;

    /**********
     * Memory Read!
     **********/
    addr  =   ( uint16_t ) z80_memory[ z80_Regs_GetAndIncPC() ] ;
    addr |= ( ( uint16_t ) z80_memory[ z80_Regs_GetAndIncPC() ] ) << 8 ;

    /**********
     * Memory Write!
     *
     *    7   6   5   4   3   2   1   0
     *  +---+---+---+---+---+---+---+---+
     *  | 0 | 0 | 1 | 1 | x | 0 | 1 | 0 |
     *  +---+---+---+---+---+---+---+---+
     *                   \ /
     *                    |
     *                    +--> 0 = (ind)<-A / 1 = A<-(ind)
     *
     **********/
    // Check direction.
    if( opCode & 0x08 )
    {
        // A <- (ind)
        tmp = z80_memory[ addr ] ;
        z80_Regs_SetReg( eSelectReg_regA , tmp ) ;

        z80_verbose_addOperatorRegister( eSelectReg_regA , DIRECT ) ;
        z80_verbose_addOperatorWord( addr , INDIRECT ) ;
    }
    else
    {
        // (ind) <- A
        tmp = z80_Regs_GetReg( eSelectReg_regA ) ;
        z80_memory[ addr ] = tmp ;

        z80_verbose_addOperatorWord( addr , INDIRECT ) ;
        z80_verbose_addOperatorRegister( eSelectReg_regA , DIRECT ) ;
    }
}

void z80_opcode_LoadMemHL( uint8_t opCode )
{
    uint16_t addr ;

    addr  =   ( uint16_t ) z80_memory[ z80_Regs_GetAndIncPC() ] ;
    addr |= ( ( uint16_t ) z80_memory[ z80_Regs_GetAndIncPC() ] ) << 8 ;

    /**********
     * Opcode - 22h - LD (xxxx),HL
     *          2Ah - LD HL,(xxxx)
     * Size   - 3 Byte
     **********/
    z80_verbose_addOpcode( opCode ) ;
    z80_verbose_addOpcode( addr & 0x00FF ) ;
    z80_verbose_addOpcode( addr >> 8 ) ;

    z80_verbose_addMnemonic( "LD" ) ;

    if( opCode & 0x08 ) // Bit 3, LD HL,(xxxx)
    {
        z80_Regs_SetReg( eSelectReg_regL , z80_memory[ addr     ] ) ;
        z80_Regs_SetReg( eSelectReg_regH , z80_memory[ addr + 1 ] ) ;

        z80_verbose_addOperatorRegister( eSelectReg_regHL , DIRECT ) ;
        z80_verbose_addOperatorWord( addr , INDIRECT ) ;
    }
    else // LD (xxxx),HL
    {
        z80_memory[ addr     ] = z80_Regs_GetReg( eSelectReg_regL ) ;
        z80_memory[ addr + 1 ] = z80_Regs_GetReg( eSelectReg_regH ) ;

        z80_verbose_addOperatorWord( addr , INDIRECT ) ;
        z80_verbose_addOperatorRegister( eSelectReg_regHL , DIRECT ) ;
    }
}

void z80_opcode_LoadSPHL( uint8_t opCode )
{
    /**********
     * Opcode - F9h - LD SP,HL
     * Size   - 1 Byte
     **********/
    z80_verbose_addOpcode( opCode ) ;
    z80_verbose_addMnemonic( "LD" ) ;

    z80_SP = z80_Regs_GetDReg( eSelectReg_regHL ) ;

    z80_verbose_addOperator( "SP" , DIRECT ) ;
    z80_verbose_addOperatorRegister( eSelectReg_regHL , DIRECT ) ;
}

void z80_opcode_PUSH( uint8_t opCode )
{
    /**********
     * Opcode - C5h - PUSH BC
     *          D5h - PUSH DE
     *          E5h - PUSH HL
     *          F5h - PUSH AF
     * Size   - 1 Byte
     **********/
    z80_verbose_addOpcode( opCode ) ;
    z80_verbose_addMnemonic( "PUSH" ) ;

    /**********
     * Execute!
     **********/
    switch( opCode )
    {
        case 0xC5: // PUSH BC
            z80_memory[ --z80_SP ] = ( uint8_t ) z80_Regs_GetReg( eSelectReg_regB ) ;
            z80_memory[ --z80_SP ] = ( uint8_t ) z80_Regs_GetReg( eSelectReg_regC ) ;

            z80_verbose_addOperatorDoubleRegister( eSelectReg_regBC , DIRECT ) ;
            break ;
        case 0xD5: // PUSH DE
            z80_memory[ --z80_SP ] = ( uint8_t ) z80_Regs_GetReg( eSelectReg_regD ) ;
            z80_memory[ --z80_SP ] = ( uint8_t ) z80_Regs_GetReg( eSelectReg_regE ) ;

            z80_verbose_addOperatorDoubleRegister( eSelectReg_regDE , DIRECT ) ;
            break ;
        case 0xE5: // PUSH HL
            z80_memory[ --z80_SP ] = ( uint8_t ) z80_Regs_GetReg( eSelectReg_regH ) ;
            z80_memory[ --z80_SP ] = ( uint8_t ) z80_Regs_GetReg( eSelectReg_regL ) ;

            z80_verbose_addOperatorDoubleRegister( eSelectReg_regHL , DIRECT ) ;
            break ;
        case 0xF5: // PUSH AF
            z80_memory[ --z80_SP ] = ( uint8_t ) z80_Regs_GetReg( eSelectReg_regA ) ;
            z80_memory[ --z80_SP ] = *( ( uint8_t * ) &z80_flags ) ;

            z80_verbose_addOperator( "AF" , DIRECT ) ;
            break ;
    }
}

void z80_opcode_POP( uint8_t opCode )
{
    /**********
     * Opcode - C1h - POP BC
     *          D1h - POP DE
     *          E1h - POP HL
     *          F1h - POP AF
     * Size   - 1 Byte
     **********/
    z80_verbose_addOpcode( opCode ) ;
    z80_verbose_addMnemonic( "POP" ) ;

    /**********
     * Execute!
     **********/
    switch( opCode )
    {
        case 0xC1: // POP BC
            z80_Regs_SetReg( eSelectReg_regC , z80_memory[ z80_SP++ ] ) ;
            z80_Regs_SetReg( eSelectReg_regB , z80_memory[ z80_SP++ ] ) ;

            z80_verbose_addOperatorDoubleRegister( eSelectReg_regBC , DIRECT ) ;
            break ;
        case 0xD1: // POP DE
            z80_Regs_SetReg( eSelectReg_regE , z80_memory[ z80_SP++ ] ) ;
            z80_Regs_SetReg( eSelectReg_regD , z80_memory[ z80_SP++ ] ) ;

            z80_verbose_addOperatorDoubleRegister( eSelectReg_regDE , DIRECT ) ;
            break ;
        case 0xE1: // POP HL
            z80_Regs_SetReg( eSelectReg_regL , z80_memory[ z80_SP++ ] ) ;
            z80_Regs_SetReg( eSelectReg_regH , z80_memory[ z80_SP++ ] ) ;

            z80_verbose_addOperatorDoubleRegister( eSelectReg_regHL , DIRECT ) ;
            break ;
        case 0xF1: // POP AF
            *( ( uint8_t * ) &z80_flags ) = z80_memory[ z80_SP++ ] ;
            z80_Regs_SetReg( eSelectReg_regA , z80_memory[ z80_SP++ ] ) ;

            z80_verbose_addOperator( "AF" , DIRECT ) ;
            break ;
    }
}
