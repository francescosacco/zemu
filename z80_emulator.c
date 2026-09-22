#include <z80_emulator.h>
#include <z80_io.h>

#include <stdio.h>
#include <string.h>
#include <stdbool.h>

// Debug!
#include <stdlib.h>
// Debug! #define VERBOSE

#include "z80_io.h"
#include "z80_registers.h"

uint8_t z80_flags_getZero( uint8_t in ) ;
uint8_t z80_flags_getSign( uint8_t in ) ;
uint8_t z80_flags_getPar( uint8_t in ) ;
uint8_t z80_flags_getX5( uint8_t in ) ;
uint8_t z80_flags_getX3( uint8_t in ) ;

typedef void ( * pFunc_t )( uint8_t ) ;


struct
{
    uint8_t c  : 1; // Carry
    uint8_t n  : 1; // Add/Sub
    uint8_t pv : 1; // Parity/OverFlow
    uint8_t x3 : 1; // Not Used
    uint8_t h  : 1; // Half Carry Flag
    uint8_t x5 : 1; // Not Used
    uint8_t z  : 1; // Zero Flag
    uint8_t s  : 1; // Sign Flag
} z80_flags ;

void z80_opcode_NOP( uint8_t opCode ) ;
void z80_opcode_Logic( uint8_t opCode ) ;
void z80_opcode_DEI( uint8_t opCode ) ;
void z80_opcode_OUT( uint8_t opCode ) ;
void z80_opcode_LoadRegC( uint8_t opCode ) ;
void z80_opcode_LoadDRegC( uint8_t opCode ) ;
void z80_opcode_LoadRegReg( uint8_t opCode ) ;
void z80_opcode_Misc( uint8_t opCode ) ;
void z80_opcode_Misc_IM( uint8_t opCode ) ;
void z80_opcode_JUMP( uint8_t opCode ) ;
void z80_opcode_CALL( uint8_t opCode ) ;
void z80_opcode_RET( uint8_t opCode ) ;
void z80_opcode_RST( uint8_t opCode ) ;
void z80_opcode_LoadAInd( uint8_t opCode ) ;
void z80_opcode_LoadAMem( uint8_t opCode ) ;
void z80_opcode_CP( uint8_t opCode ) ;


void z80_opcode_CRASH( uint8_t opCode ) ;
void z80_opcode_LoadSPHL( uint8_t opCode ) ;
void z80_opcode_LoadMemHL( uint8_t opCode ) ;

pFunc_t z80_opcodesExecution[] =
{
    z80_opcode_NOP       , // 00 - 1 Byte  - NOP
    z80_opcode_LoadDRegC , // 01 - 3 Bytes - LD BC,xxxx
    z80_opcode_LoadAInd  , // 02 - 1 Byte  - LD (BC),A
    z80_opcode_CRASH     , // 03 - Crash
    z80_opcode_CRASH     , // 04 - Crash
    z80_opcode_CRASH     , // 05 - Crash
    z80_opcode_LoadRegC  , // 06 - 2 Bytes - LD B,xx
    z80_opcode_CRASH     , // 07 - Crash
    z80_opcode_CRASH     , // 08 - Crash
    z80_opcode_CRASH     , // 09 - Crash
    z80_opcode_LoadAInd  , // 0A - 1 Byte  - LD A,(BC)
    z80_opcode_CRASH     , // 0B - Crash
    z80_opcode_CRASH     , // 0C - Crash
    z80_opcode_CRASH     , // 0D - Crash
    z80_opcode_LoadRegC  , // 0E - 2 Bytes - LD C,xx
    z80_opcode_CRASH     , // 0F - Crash

    z80_opcode_CRASH     , // 10 - Crash
    z80_opcode_LoadDRegC , // 11 - 3 Bytes - LD DE,xxxx
    z80_opcode_LoadAInd  , // 12 - 1 Byte  - LD (DE),A
    z80_opcode_CRASH     , // 13 - Crash
    z80_opcode_CRASH     , // 14 - Crash
    z80_opcode_CRASH     , // 15 - Crash
    z80_opcode_LoadRegC  , // 16 - 2 Bytes - LD D,xx
    z80_opcode_CRASH     , // 17 - Crash
    z80_opcode_CRASH     , // 18 - Crash
    z80_opcode_CRASH     , // 19 - Crash
    z80_opcode_LoadAInd  , // 1A - 1 Byte  - LD A,(DE)
    z80_opcode_CRASH     , // 1B - Crash
    z80_opcode_CRASH     , // 1C - Crash
    z80_opcode_CRASH     , // 1D - Crash
    z80_opcode_LoadRegC  , // 1E - 2 Bytes - LD E,xx
    z80_opcode_CRASH     , // 1F - Crash

    z80_opcode_CRASH , // 20 - Crash
    z80_opcode_LoadDRegC , // 21 - 3 Bytes - LD HL,xxxx
    z80_opcode_LoadMemHL   , // 22 - 3 Bytes - LD (xxxx),HL
    z80_opcode_CRASH , // 23 - Crash
    z80_opcode_CRASH , // 24 - Crash
    z80_opcode_CRASH , // 25 - Crash
    z80_opcode_LoadRegC , // 26 - 2 Bytes - LD H,xx
    z80_opcode_CRASH , // 27 - Crash
    z80_opcode_CRASH , // 28 - Crash
    z80_opcode_CRASH , // 29 - Crash
    z80_opcode_CRASH , // 2A - Crash
    z80_opcode_CRASH , // 2B - Crash
    z80_opcode_CRASH , // 2C - Crash
    z80_opcode_CRASH , // 2D - Crash
    z80_opcode_LoadRegC , // 2E - 2 Bytes - LD L,xx
    z80_opcode_CRASH , // 2F - Crash

    z80_opcode_CRASH     , // 30 - Crash
    z80_opcode_LoadDRegC , // 31 - 3 Bytes - LD SP,xxxx
    z80_opcode_LoadAMem  , // 32 - 3 Bytes - LD (xxxx),A
    z80_opcode_CRASH     , // 33 - Crash
    z80_opcode_CRASH     , // 34 - Crash
    z80_opcode_CRASH     , // 35 - Crash
    z80_opcode_LoadRegC  , // 36 - 2 Bytes - LD (HL),xx
    z80_opcode_CRASH     , // 37 - Crash
    z80_opcode_CRASH     , // 38 - Crash
    z80_opcode_CRASH     , // 39 - Crash
    z80_opcode_LoadAMem  , // 3A - 3 Bytes - LD A,(xxxx)
    z80_opcode_CRASH     , // 3B - Crash
    z80_opcode_CRASH     , // 3C - Crash
    z80_opcode_CRASH     , // 3D - Crash
    z80_opcode_LoadRegC  , // 3E - 2 Bytes - LD A,xx
    z80_opcode_CRASH     , // 3F - Crash

    z80_opcode_LoadRegReg , // 40 - 1 Byte  - LD B,B
    z80_opcode_LoadRegReg , // 41 - 1 Byte  - LD B,C
    z80_opcode_LoadRegReg , // 42 - 1 Byte  - LD B,D
    z80_opcode_LoadRegReg , // 43 - 1 Byte  - LD B,E
    z80_opcode_LoadRegReg , // 44 - 1 Byte  - LD B,H
    z80_opcode_LoadRegReg , // 45 - 1 Byte  - LD B,L
    z80_opcode_LoadRegReg , // 46 - 1 Byte  - LD B,(HL)
    z80_opcode_LoadRegReg , // 47 - 1 Byte  - LD B,A
    z80_opcode_LoadRegReg , // 48 - 1 Byte  - LD C,B
    z80_opcode_LoadRegReg , // 49 - 1 Byte  - LD C,C
    z80_opcode_LoadRegReg , // 4A - 1 Byte  - LD C,D
    z80_opcode_LoadRegReg , // 4B - 1 Byte  - LD C,E
    z80_opcode_LoadRegReg , // 4C - 1 Byte  - LD C,H
    z80_opcode_LoadRegReg , // 4D - 1 Byte  - LD C,L
    z80_opcode_LoadRegReg , // 4E - 1 Byte  - LD C,(HL)
    z80_opcode_LoadRegReg , // 4F - 1 Byte  - LD C,A

    z80_opcode_LoadRegReg , // 50 - 1 Byte  - LD D,B
    z80_opcode_LoadRegReg , // 51 - 1 Byte  - LD D,C
    z80_opcode_LoadRegReg , // 52 - 1 Byte  - LD D,D
    z80_opcode_LoadRegReg , // 53 - 1 Byte  - LD D,E
    z80_opcode_LoadRegReg , // 54 - 1 Byte  - LD D,H
    z80_opcode_LoadRegReg , // 55 - 1 Byte  - LD D,L
    z80_opcode_LoadRegReg , // 56 - 1 Byte  - LD D,(HL)
    z80_opcode_LoadRegReg , // 57 - 1 Byte  - LD D,A
    z80_opcode_LoadRegReg , // 58 - 1 Byte  - LD E,B
    z80_opcode_LoadRegReg , // 59 - 1 Byte  - LD E,C
    z80_opcode_LoadRegReg , // 5A - 1 Byte  - LD E,D
    z80_opcode_LoadRegReg , // 5B - 1 Byte  - LD E,E
    z80_opcode_LoadRegReg , // 5C - 1 Byte  - LD E,H
    z80_opcode_LoadRegReg , // 5D - 1 Byte  - LD E,L
    z80_opcode_LoadRegReg , // 5E - 1 Byte  - LD E,(HL)
    z80_opcode_LoadRegReg , // 5F - 1 Byte  - LD E,A

    z80_opcode_LoadRegReg , // 60 - 1 Byte  - LD H,B
    z80_opcode_LoadRegReg , // 61 - 1 Byte  - LD H,C
    z80_opcode_LoadRegReg , // 62 - 1 Byte  - LD H,D
    z80_opcode_LoadRegReg , // 63 - 1 Byte  - LD H,E
    z80_opcode_LoadRegReg , // 64 - 1 Byte  - LD H,H
    z80_opcode_LoadRegReg , // 65 - 1 Byte  - LD H,L
    z80_opcode_LoadRegReg , // 66 - 1 Byte  - LD H,(HL)
    z80_opcode_LoadRegReg , // 67 - 1 Byte  - LD H,A
    z80_opcode_LoadRegReg , // 68 - 1 Byte  - LD L,B
    z80_opcode_LoadRegReg , // 69 - 1 Byte  - LD L,C
    z80_opcode_LoadRegReg , // 6A - 1 Byte  - LD L,D
    z80_opcode_LoadRegReg , // 6B - 1 Byte  - LD L,E
    z80_opcode_LoadRegReg , // 6C - 1 Byte  - LD L,H
    z80_opcode_LoadRegReg , // 6D - 1 Byte  - LD L,L
    z80_opcode_LoadRegReg , // 6E - 1 Byte  - LD L,(HL)
    z80_opcode_LoadRegReg , // 6F - 1 Byte  - LD L,A

    z80_opcode_LoadRegReg , // 70 - 1 Byte  - LD (HL),B
    z80_opcode_LoadRegReg , // 71 - 1 Byte  - LD (HL),C
    z80_opcode_LoadRegReg , // 72 - 1 Byte  - LD (HL),D
    z80_opcode_LoadRegReg , // 73 - 1 Byte  - LD (HL),E
    z80_opcode_LoadRegReg , // 74 - 1 Byte  - LD (HL),H
    z80_opcode_LoadRegReg , // 75 - 1 Byte  - LD (HL),L
    z80_opcode_CRASH , // 76 - Crash
    z80_opcode_LoadRegReg , // 77 - 1 Byte  - LD (HL),A
    z80_opcode_LoadRegReg , // 78 - 1 Byte  - LD A,B
    z80_opcode_LoadRegReg , // 79 - 1 Byte  - LD A,C
    z80_opcode_LoadRegReg , // 7A - 1 Byte  - LD A,D
    z80_opcode_LoadRegReg , // 7B - 1 Byte  - LD A,E
    z80_opcode_LoadRegReg , // 7C - 1 Byte  - LD A,H
    z80_opcode_LoadRegReg , // 7D - 1 Byte  - LD A,L
    z80_opcode_LoadRegReg , // 7E - 1 Byte  - LD A,(HL)
    z80_opcode_LoadRegReg , // 7F - 1 Byte  - LD A,A

    z80_opcode_CRASH , // 80 - Crash
    z80_opcode_CRASH , // 81 - Crash
    z80_opcode_CRASH , // 82 - Crash
    z80_opcode_CRASH , // 83 - Crash
    z80_opcode_CRASH , // 84 - Crash
    z80_opcode_CRASH , // 85 - Crash
    z80_opcode_CRASH , // 86 - Crash
    z80_opcode_CRASH , // 87 - Crash
    z80_opcode_CRASH , // 88 - Crash
    z80_opcode_CRASH , // 89 - Crash
    z80_opcode_CRASH , // 8A - Crash
    z80_opcode_CRASH , // 8B - Crash
    z80_opcode_CRASH , // 8C - Crash
    z80_opcode_CRASH , // 8D - Crash
    z80_opcode_CRASH , // 8E - Crash
    z80_opcode_CRASH , // 8F - Crash

    z80_opcode_CRASH , // 90 - Crash
    z80_opcode_CRASH , // 91 - Crash
    z80_opcode_CRASH , // 92 - Crash
    z80_opcode_CRASH , // 93 - Crash
    z80_opcode_CRASH , // 94 - Crash
    z80_opcode_CRASH , // 95 - Crash
    z80_opcode_CRASH , // 96 - Crash
    z80_opcode_CRASH , // 97 - Crash
    z80_opcode_CRASH , // 98 - Crash
    z80_opcode_CRASH , // 99 - Crash
    z80_opcode_CRASH , // 9A - Crash
    z80_opcode_CRASH , // 9B - Crash
    z80_opcode_CRASH , // 9C - Crash
    z80_opcode_CRASH , // 9D - Crash
    z80_opcode_CRASH , // 9E - Crash
    z80_opcode_CRASH , // 9F - Crash

    z80_opcode_Logic , // A0 - 1 Byte - AND A,B
    z80_opcode_Logic , // A1 - 1 Byte - AND A,C
    z80_opcode_Logic , // A2 - 1 Byte - AND A,D
    z80_opcode_Logic , // A3 - 1 Byte - AND A,E
    z80_opcode_Logic , // A4 - 1 Byte - AND A,H
    z80_opcode_Logic , // A5 - 1 Byte - AND A,L
    z80_opcode_Logic , // A6 - 1 Byte - AND A,(HL)
    z80_opcode_Logic , // A7 - 1 Byte - AND A,A
    z80_opcode_Logic , // A8 - 1 Byte - XOR A,B
    z80_opcode_Logic , // A9 - 1 Byte - XOR A,C
    z80_opcode_Logic , // AA - 1 Byte - XOR A,D
    z80_opcode_Logic , // AB - 1 Byte - XOR A,E
    z80_opcode_Logic , // AC - 1 Byte - XOR A,H
    z80_opcode_Logic , // AD - 1 Byte - XOR A,L
    z80_opcode_Logic , // AE - 1 Byte - XOR A,(HL)
    z80_opcode_Logic , // AF - 1 Byte - XOR A,A

    z80_opcode_Logic , // B0 - 1 Byte - OR A,B
    z80_opcode_Logic , // B1 - 1 Byte - OR A,C
    z80_opcode_Logic , // B2 - 1 Byte - OR A,D
    z80_opcode_Logic , // B3 - 1 Byte - OR A,E
    z80_opcode_Logic , // B4 - 1 Byte - OR A,H
    z80_opcode_Logic , // B5 - 1 Byte - OR A,L
    z80_opcode_Logic , // B6 - 1 Byte - OR A,(HL)
    z80_opcode_Logic , // B7 - 1 Byte - OR A,A
    z80_opcode_CRASH , // B8 - Crash
    z80_opcode_CRASH , // B9 - Crash
    z80_opcode_CRASH , // BA - Crash
    z80_opcode_CRASH , // BB - Crash
    z80_opcode_CRASH , // BC - Crash
    z80_opcode_CRASH , // BD - Crash
    z80_opcode_CRASH , // BE - Crash
    z80_opcode_CRASH , // BF - Crash

    z80_opcode_RET   , // C0 - 1 Byte  - RET NZ
    z80_opcode_CRASH , // C1 - Crash
    z80_opcode_JUMP  , // C2 - 3 Bytes - JP NZ,xxxx
    z80_opcode_JUMP  , // C3 - 3 Bytes - JP xxxx
    z80_opcode_CALL  , // C4 - 3 Bytes - CALL NZ,xxxx
    z80_opcode_CRASH , // C5 - Crash
    z80_opcode_CRASH , // C6 - Crash
    z80_opcode_RST   , // C7 - 1 Byte  - RST 00h
    z80_opcode_RET   , // C8 - 1 Byte  - RET Z
    z80_opcode_RET   , // C9 - 1 Byte  - RET
    z80_opcode_JUMP  , // CA - 3 Bytes - JP Z,xxxx
    z80_opcode_CRASH , // CB - Crash
    z80_opcode_CALL  , // CC - 3 Bytes - CALL Z,xxxx
    z80_opcode_CALL  , // CD - 3 Bytes - CALL xxxx
    z80_opcode_CRASH , // CE - Crash
    z80_opcode_RST   , // CF - 1 Byte  - RST 08h

    z80_opcode_RET   , // D0 - 1 Byte  - RET NC
    z80_opcode_CRASH , // D1 - Crash
    z80_opcode_JUMP  , // D2 - 3 Bytes - JP NC,xxxx
    z80_opcode_OUT   , // D3 - 2 Bytes - OUT (xx),A
    z80_opcode_CALL  , // D4 - 3 Bytes - CALL NC,xxxx
    z80_opcode_CRASH , // D5 - Crash
    z80_opcode_CRASH , // D6 - Crash
    z80_opcode_RST   , // D7 - 1 Byte  - RST 10h
    z80_opcode_RET   , // D8 - 1 Byte  - RET Z
    z80_opcode_CRASH , // D9 - Crash
    z80_opcode_JUMP  , // DA - 3 Bytes - JP C,xxxx
    z80_opcode_CRASH , // DB - Crash
    z80_opcode_CALL  , // DC - 3 Bytes - CALL C,xxxx
    z80_opcode_CRASH , // DD - Crash
    z80_opcode_CRASH , // DE - Crash
    z80_opcode_RST   , // DF - 1 Byte  - RST 18h

    z80_opcode_RET   , // E0 - 1 Byte  - RET NPV
    z80_opcode_CRASH , // E1 - Crash
    z80_opcode_JUMP  , // E2 - 3 Bytes - JP NPV,xxxx
    z80_opcode_CRASH , // E3 - Crash
    z80_opcode_CALL  , // E4 - 3 Bytes - CALL NPV,xxxx
    z80_opcode_CRASH , // E5 - Crash
    z80_opcode_CRASH , // E6 - Crash
    z80_opcode_RST   , // E7 - 1 Byte  - RST 20h
    z80_opcode_RET   , // E8 - 1 Byte  - RET PV
    z80_opcode_CRASH , // E9 - Crash
    z80_opcode_JUMP  , // EA - 3 Bytes - JP PV,xxxx
    z80_opcode_CRASH , // EB - Crash
    z80_opcode_CALL  , // EC - 3 Bytes - CALL PV,xxxx
    z80_opcode_Misc  , // ED - x Bytes - Misc!
    z80_opcode_CRASH , // EE - Crash
    z80_opcode_RST   , // EF - 1 Byte  - RST 28h

    z80_opcode_RET      , // F0 - 1 Byte  - RET NS
    z80_opcode_CRASH    , // F1 - Crash
    z80_opcode_JUMP     , // FA - 3 Bytes - JP NS,xxxx
    z80_opcode_DEI      , // F3 - 1 Byte  - DI
    z80_opcode_CALL     , // F4 - 3 Bytes - CALL NS,xxxx
    z80_opcode_CRASH    , // F5 - Crash
    z80_opcode_CRASH    , // F6 - Crash
    z80_opcode_RST      , // F7 - 1 Byte  - RST 30h
    z80_opcode_RET      , // F8 - 1 Byte  - RET S
    z80_opcode_LoadSPHL , // F9 - 1 Byte  - LD SP,HL
    z80_opcode_JUMP     , // FA - 3 Bytes - JP S,xxxx
    z80_opcode_DEI      , // FB - 1 Byte  - EI
    z80_opcode_CALL     , // FC - 3 Bytes - CALL S,xxxx
    z80_opcode_CRASH    , // FD - Crash
    z80_opcode_CRASH    , // FE - Crash
    z80_opcode_RST      , // FF - 1 Byte  - RST 38h
} ;

void z80_initializeMemory( void )
{
    memset( z80_memory , 0x00 , MEMORY_SIZE ) ;
}

void z80_reset( void )
{
    z80_PC = 0x0000 ; // Initialize Program Counter.
    z80_SP = 0xFFFF ;
    z80_EI = false ;
    z80_IM = 0x00 ;
}

void z80_step( void )
{
    uint8_t opcode ;
    
#ifdef VERBOSE
printf( "\t PC[%04X] ",z80_PC);
#endif
    opcode = z80_memory[ z80_PC++ ] ;

#ifdef VERBOSE
printf( "OP[%02X] ",opcode);
#endif

    z80_opcodesExecution[ opcode ]( opcode ) ;
}

void z80_opcode_CRASH( uint8_t opCode )
{
    printf( "Program Counter %04Xh - OpCode %02Xh\n" , ( z80_PC - 1 ) , opCode ) ;
    exit( 0 ) ;
}

void z80_opcode_NOP( uint8_t opCode )
{
    /**********
     * Opcode - 00h - NOP
     * Size   - 1 Byte
     **********/
    
    /**********
     * Memory Read!
     **********/
    // Nothing to do.

    /**********
     * Execute!
     **********/
    // Nothing to do.

    /**********
     * Memory Write!
     **********/
    // Nothing to do.

    /**********
     * Update Flags.
     **********/
    // Nothing to do.
}

void z80_opcode_Logic( uint8_t opCode )
{
    uint8_t tmp ;
    uint8_t flag_h = 0;
    
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
    
    /**********
     * Memory Read!
     **********/
    eSelectReg_t reg = ( eSelectReg_t ) ( opCode & 0x07 ) ;

    // Read Accumulator.
    tmp = z80_Regs_GetReg( eSelectReg_regA ) ;

    /**********
     * Execute!
     **********/
    switch( ( opCode >> 3 ) & 0x03 ) // Bits 4~3.
    {
        case 0x00 : // AND
            tmp &= z80_Regs_GetReg( reg ) ;
            flag_h = 1 ;
            break ;
        case 0x01 : // XOR
            tmp ^= z80_Regs_GetReg( reg ) ;
            flag_h = 0 ;
            break ;
        case 0x02 : // OR
            tmp |= z80_Regs_GetReg( reg ) ;
            flag_h = 0 ;
            break ;
    }

    /**********
     * Memory Write!
     **********/
    
    // Write Accumulator.
    z80_Regs_SetReg( eSelectReg_regA , tmp ) ;

    /**********
     * Update Flags.
     **********/

    // S is set if result is negative; otherwise, it is reset.
    z80_flags.s = z80_flags_getSign( tmp ) ;
    // Z is set if result is 0; otherwise, it is reset.
    z80_flags.z = z80_flags_getZero( tmp ) ;
    // X5 is the copy of bit 5.
    z80_flags.x5 = z80_flags_getX5( tmp ) ;
    // H is set for AND, and reset for XOR/OR.
    z80_flags.h = flag_h ;
    // X3 is the copy of bit 3.
    z80_flags.x3 = z80_flags_getX3( tmp ) ;
    // P is the parity flag.
    z80_flags.pv = z80_flags_getPar( tmp ) ;
    // N is reset.
    z80_flags.n = 0 ;
    // C is reset.
    z80_flags.c = 0 ;
}

void z80_opcode_DEI( uint8_t opCode )
{
    /**********
     * Opcode - F3h - DI
     *          FBh - EI
     * Size   - 1 Byte
     **********/
    
    /**********
     * Memory Read!
     **********/
    // Nothing to do.

    /**********
     * Execute!
     **********/
    if( opCode & 0x08 ) // Check bit 3.
    {
        // It's F3h, so Disable.
        z80_EI = false ;
#ifdef VERBOSE
printf("DI\n");
#endif
    }
    else
    {
        // It's FBh, so Enable.
        z80_EI = true ;
#ifdef VERBOSE
printf("EI\n");
#endif
    }

    /**********
     * Memory Write!
     **********/
    // Nothing to do.

    /**********
     * Update Flags.
     **********/
    // Nothing to do.
}

void z80_opcode_OUT( uint8_t opCode )
{
    uint8_t tmp ;
    uint8_t port ;

    /**********
     * Opcode - D3h - OUT (xx),A
     * Size   - 1 Byte
     **********/
    
    /**********
     * Memory Read!
     **********/
    port = z80_memory[ z80_PC++ ] ;
#ifdef VERBOSE
printf("OUT (%02X),A\n",port);
#endif
    
    // Read Accumulator.
    tmp = z80_Regs_GetReg( eSelectReg_regA ) ;

    /**********
     * Execute!
     **********/
    z80_io_setData( port , tmp ) ;

    /**********
     * Memory Write!
     **********/
    // Nothing to do.

    /**********
     * Update Flags.
     **********/
    // Nothing to do.
}

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
    
    /**********
     * Memory Read!
     **********/
    data = z80_memory[ z80_PC++ ] ;
#ifdef VERBOSE
printf("LD Reg,%02X\n",data);
#endif

    /**********
     * Execute!
     **********/
    eSelectReg_t eReg = ( eSelectReg_t ) ( ( opCode >> 3 ) & 0x07 ) ; // Bits 5~3.

    /**********
     * Memory Write!
     **********/
    z80_Regs_SetReg( eReg , data ) ;

    /**********
     * Update Flags.
     **********/
    // Nothing to do.
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
#ifdef VERBOSE
printf("LD Reg,Reg\n");
#endif
    
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

    tmp = z80_Regs_GetReg( regOrigin ) ;

    /**********
     * Execute!
     **********/
    // Nothing to do.

    /**********
     * Memory Write!
     **********/
    z80_Regs_SetReg( regDestin , tmp ) ;

    /**********
     * Update Flags.
     **********/
    // Nothing to do.
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
    
    /**********
     * Memory Read!
     **********/
    tmp  =   ( uint16_t ) z80_memory[ z80_PC++ ] ;
    tmp |= ( ( uint16_t ) z80_memory[ z80_PC++ ] ) << 8 ;
#ifdef VERBOSE
printf("LD DReg,%04X\n",tmp);
#endif

    /**********
     * Execute!
     **********/
    eReg = ( eSelectDoubleReg_t ) ( ( opCode >> 4 ) & 0x03 ) ;

    /**********
     * Memory Write!
     **********/
    z80_Regs_SetDReg( eReg , tmp ) ;

    /**********
     * Update Flags.
     **********/
    // Nothing to do.


}

void z80_opcode_JUMP( uint8_t opCode )
{
    uint16_t tmp ;

    bool shouldJump    = false ;
    bool expectedValue = false ;
    bool flagToTest    = false ;

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
    tmp  =   ( uint16_t ) z80_memory[ z80_PC++ ] ;
    tmp |= ( ( uint16_t ) z80_memory[ z80_PC++ ] ) << 8 ;
#ifdef VERBOSE
printf("JP ?,%04X\n",tmp);
#endif

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
        expectedValue = ( opCode & 0x08 ) == 0x08 ; // Bit 3.
        switch( opCode & 0x30 ) // Bits 5~4.
        {
            case 0x00 : // xx00.xxxx - Zero Flag.
                flagToTest = z80_flags.z ;
                break ;
            case 0x10 : // xx01.xxxx - Carry Flag.
                flagToTest = z80_flags.c ;
                break ;
            case 0x20 : // xx10.xxxx - Parity/Overflow Flag.
                flagToTest = z80_flags.pv ;
                break ;
            case 0x30 : // xx11.xxxx - Sign Flag.
                flagToTest = z80_flags.s ;
                break ;
        }

        shouldJump = ( flagToTest == expectedValue ) ;
    }

    /**********
     * Memory Write!
     **********/
    if( shouldJump )
    {
        z80_PC = tmp ;
    }

    /**********
     * Update Flags.
     **********/
    // Nothing to do.
}

void z80_opcode_CALL( uint8_t opCode )
{
    uint16_t tmp ;

    bool shouldJump    = false ;
    bool expectedValue = false ;
    bool flagToTest    = false ;

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
    tmp  =   ( uint16_t ) z80_memory[ z80_PC++ ] ;
    tmp |= ( ( uint16_t ) z80_memory[ z80_PC++ ] ) << 8 ;
#ifdef VERBOSE
printf("CALL ?,%04X\n",tmp);
#endif

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
        expectedValue = ( opCode & 0x08 ) == 0x08 ; // Bit 3.
        switch( opCode & 0x30 ) // Bits 5~4.
        {
            case 0x00 : // xx00.xxxx - Zero Flag.
                flagToTest = z80_flags.z ;
                break ;
            case 0x10 : // xx01.xxxx - Carry Flag.
                flagToTest = z80_flags.c ;
                break ;
            case 0x20 : // xx10.xxxx - Parity/Overflow Flag.
                flagToTest = z80_flags.pv ;
                break ;
            case 0x30 : // xx11.xxxx - Sign Flag.
                flagToTest = z80_flags.s ;
                break ;
        }

        shouldJump = ( flagToTest == expectedValue ) ;
    }

    /**********
     * Memory Write!
     **********/
    if( shouldJump )
    {
        z80_memory[ --z80_SP ] = ( uint8_t ) z80_PC >> 8 ;
        z80_memory[ --z80_SP ] = ( uint8_t ) z80_PC      ;

        z80_PC = tmp ;
    }

    /**********
     * Update Flags.
     **********/
    // Nothing to do.
}

void z80_opcode_RET( uint8_t opCode )
{
    uint16_t tmp ;

    bool shouldJump    = false ;
    bool expectedValue = false ;
    bool flagToTest    = false ;

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
#ifdef VERBOSE
printf("RET ?\n");
#endif

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
        expectedValue = ( opCode & 0x08 ) == 0x08 ; // Bit 3.
        switch( opCode & 0x30 ) // Bits 5~4.
        {
            case 0x00 : // xx00.xxxx - Zero Flag.
                flagToTest = z80_flags.z ;
                break ;
            case 0x10 : // xx01.xxxx - Carry Flag.
                flagToTest = z80_flags.c ;
                break ;
            case 0x20 : // xx10.xxxx - Parity/Overflow Flag.
                flagToTest = z80_flags.pv ;
                break ;
            case 0x30 : // xx11.xxxx - Sign Flag.
                flagToTest = z80_flags.s ;
                break ;
        }

        shouldJump = ( flagToTest == expectedValue ) ;
    }

    /**********
     * Memory Write!
     **********/
    if( shouldJump )
    {
        tmp  =   ( uint16_t ) z80_memory[ z80_SP++ ]        ; // LSB
        tmp |= ( ( uint16_t ) z80_memory[ z80_SP++ ] ) << 8 ; // MSB

        z80_PC = tmp ;
    }

    /**********
     * Update Flags.
     **********/
    // Nothing to do.
}

void z80_opcode_RST( uint8_t opCode )
{
    uint16_t tmp ;

    /**********
     * Opcode - C7h - RET 00h
     *          CFh - RET 08h
     *          D7h - RET 10h
     *          DFh - RET 18h
     *          E7h - RET 20h
     *          EFh - RET 28h
     *          F7h - RET 30h
     *          FFh - RET 38h
     * Size   - 1 Byte
     **********/
    
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
    tmp  = opCode & 0x38 ; // Bits 5~3.

    /**********
     * Execute!
     **********/

    /**********
     * Memory Write!
     **********/
    z80_memory[ --z80_SP ] = ( uint8_t ) z80_PC >> 8 ;
    z80_memory[ --z80_SP ] = ( uint8_t ) z80_PC      ;

    z80_PC = tmp ;

    /**********
     * Update Flags.
     **********/
    // Nothing to do.
}

void z80_opcode_LoadAInd( uint8_t opCode )
{
    eSelectDoubleReg_t reg ;
    uint16_t addr ;
    uint8_t tmp ;
    
    /**********
     * Opcode - 02h - LD (BC),A
     *          0Ah - LD A,(BC)
     *          12h - LD (DE),A
     *          1Ah - LD A,(DE)
     * Size   - 1 Byte
     **********/

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
    reg = ( eSelectDoubleReg_t ) ( ( opCode & 0x30 ) >> 4 ) ; // Bits 5~4.
    addr = z80_Regs_GetDReg( reg ) ;

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
    }
    else
    {
        // (ind) <- A
        tmp = z80_Regs_GetReg( eSelectReg_regA ) ;
        z80_memory[ addr ] = tmp ;
    }

    /**********
     * Update Flags.
     **********/
    // Nothing to do.
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
#ifdef VERBOSE
printf("LD A?(xxxx)\n");
#endif
    
    /**********
     * Memory Read!
     **********/
    addr  =   ( uint16_t ) z80_memory[ z80_PC++ ] ;
    addr |= ( ( uint16_t ) z80_memory[ z80_PC++ ] ) << 8 ;

    /**********
     * Execute!
     **********/
    // Nothing to do.

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
    }
    else
    {
        // (ind) <- A
        tmp = z80_Regs_GetReg( eSelectReg_regA ) ;
        z80_memory[ addr ] = tmp ;
    }

    /**********
     * Update Flags.
     **********/
    // Nothing to do.

}

void z80_opcode_CP( uint8_t opCode )
{
#ifdef VERBOSE
printf("CP A,Reg\n");
#endif

    eSelectReg_t reg ;
    uint8_t tmp ;

    /**********
     * Opcode - B8h - CP A,Reg
     * Size   - 1 Byte
     **********/

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
    reg = ( eSelectReg_t ) ( opCode & 0x07 ) ; // Bits 2~0.

    /**********
     * Execute!
     **********/
    tmp  = z80_Regs_GetReg( eSelectReg_regA ) ;
    tmp -= z80_Regs_GetReg( reg ) ;

    /**********
     * Memory Write!
     **********/
    // Nothing to do.

    /**********
     * Update Flags.
     **********/
    z80_flags.c = 0 ;
    // N is set.
    z80_flags.n = 1 ;
    z80_flags.pv = z80_flags_getPar( tmp ) ; // Calculate the Parity of the result.
    z80_flags.h = 0 ;
    // Z is set if result is 0; otherwise, it is reset.
    z80_flags.z = z80_flags_getZero( tmp ) ; // Check if the result is zero.
    // S is set if result is negative; otherwise, it is reset.
    z80_flags.s = z80_flags_getSign( tmp ) ; // Set if the input is negative.
}

uint8_t z80_flags_getZero( uint8_t in )
{
    uint8_t ret = 0x00 ;
    
    if( in == 0 )
    {
        ret = 0x01 ;
    }
    
    return( ret ) ;
}

uint8_t z80_flags_getX5( uint8_t in )
{
    uint8_t ret = 0x00 ;
    
    if( in & 0x20 ) // Bit 5 (0010.0000b).
    {
        ret = 0x01 ;
    }
    
    return( ret ) ;
}

uint8_t z80_flags_getX3( uint8_t in )
{
    uint8_t ret = 0x00 ;
    
    if( in & 0x08 ) // Bit 3 (0000.1000b).
    {
        ret = 0x01 ;
    }
    
    return( ret ) ;
}

uint8_t z80_flags_getSign( uint8_t in )
{
    uint8_t ret = 0x00 ;
    
    if( in & 0x80 )
    {
        ret = 0x01 ;
    }
    
    return( ret ) ;
}

uint8_t z80_flags_getPar( uint8_t in )
{ 
    uint8_t parity = 0x00 ;
	uint8_t i ;

	for( i = 0 ; i < 8 ; i++ )
	{
		if( in & ( 1 << i ) )
		{
			parity++ ;
		}
	}

    return( !( parity & 0x01 )) ;
}


















void z80_opcode_LoadSPHL( uint8_t opCode )
{
    z80_SP = z80_HL ;
}

void z80_opcode_LoadMemHL( uint8_t opCode )
{
    uint16_t tmp ;

    tmp  =   ( uint16_t ) z80_memory[ z80_PC++ ] ;
    tmp |= ( ( uint16_t ) z80_memory[ z80_PC++ ] ) << 8 ;

    z80_memory[ tmp     ] = z80_HL ;
    z80_memory[ tmp + 1 ] = z80_HL >> 8 ;
}


pFunc_t z80_opcodesMisc[] =
{
    z80_opcode_CRASH , // 00 - Crash
    z80_opcode_CRASH , // 01 - Crash
    z80_opcode_CRASH , // 02 - Crash
    z80_opcode_CRASH , // 03 - Crash
    z80_opcode_CRASH , // 04 - Crash
    z80_opcode_CRASH , // 05 - Crash
    z80_opcode_CRASH , // 06 - Crash
    z80_opcode_CRASH , // 07 - Crash
    z80_opcode_CRASH , // 08 - Crash
    z80_opcode_CRASH , // 09 - Crash
    z80_opcode_CRASH , // 0A - Crash
    z80_opcode_CRASH , // 0B - Crash
    z80_opcode_CRASH , // 0C - Crash
    z80_opcode_CRASH , // 0D - Crash
    z80_opcode_CRASH , // 0E - Crash
    z80_opcode_CRASH , // 0F - Crash

    z80_opcode_CRASH , // 10 - Crash
    z80_opcode_CRASH , // 11 - Crash
    z80_opcode_CRASH , // 12 - Crash
    z80_opcode_CRASH , // 13 - Crash
    z80_opcode_CRASH , // 14 - Crash
    z80_opcode_CRASH , // 15 - Crash
    z80_opcode_CRASH , // 16 - Crash
    z80_opcode_CRASH , // 17 - Crash
    z80_opcode_CRASH , // 18 - Crash
    z80_opcode_CRASH , // 19 - Crash
    z80_opcode_CRASH , // 1A - Crash
    z80_opcode_CRASH , // 1B - Crash
    z80_opcode_CRASH , // 1C - Crash
    z80_opcode_CRASH , // 1D - Crash
    z80_opcode_CRASH , // 1E - Crash
    z80_opcode_CRASH , // 1F - Crash

    z80_opcode_CRASH , // 20 - Crash
    z80_opcode_CRASH , // 21 - Crash
    z80_opcode_CRASH , // 22 - Crash
    z80_opcode_CRASH , // 23 - Crash
    z80_opcode_CRASH , // 24 - Crash
    z80_opcode_CRASH , // 25 - Crash
    z80_opcode_CRASH , // 26 - Crash
    z80_opcode_CRASH , // 27 - Crash
    z80_opcode_CRASH , // 28 - Crash
    z80_opcode_CRASH , // 29 - Crash
    z80_opcode_CRASH , // 2A - Crash
    z80_opcode_CRASH , // 2B - Crash
    z80_opcode_CRASH , // 2C - Crash
    z80_opcode_CRASH , // 2D - Crash
    z80_opcode_CRASH , // 2E - Crash
    z80_opcode_CRASH , // 2F - Crash

    z80_opcode_CRASH , // 30 - Crash
    z80_opcode_CRASH , // 31 - Crash
    z80_opcode_CRASH , // 32 - Crash
    z80_opcode_CRASH , // 33 - Crash
    z80_opcode_CRASH , // 34 - Crash
    z80_opcode_CRASH , // 35 - Crash
    z80_opcode_CRASH , // 36 - Crash
    z80_opcode_CRASH , // 37 - Crash
    z80_opcode_CRASH , // 38 - Crash
    z80_opcode_CRASH , // 39 - Crash
    z80_opcode_CRASH , // 3A - Crash
    z80_opcode_CRASH , // 3B - Crash
    z80_opcode_CRASH , // 3C - Crash
    z80_opcode_CRASH , // 3D - Crash
    z80_opcode_CRASH , // 3E - Crash
    z80_opcode_CRASH , // 3F - Crash

    z80_opcode_CRASH , // 40 - Crash
    z80_opcode_CRASH , // 41 - Crash
    z80_opcode_CRASH , // 42 - Crash
    z80_opcode_CRASH , // 43 - Crash
    z80_opcode_CRASH , // 44 - Crash
    z80_opcode_CRASH , // 45 - Crash
    z80_opcode_Misc_IM , // 46 - 2 Bytes - IM 0
    z80_opcode_CRASH , // 47 - Crash
    z80_opcode_CRASH , // 48 - Crash
    z80_opcode_CRASH , // 49 - Crash
    z80_opcode_CRASH , // 4A - Crash
    z80_opcode_CRASH , // 4B - Crash
    z80_opcode_CRASH , // 4C - Crash
    z80_opcode_CRASH , // 4D - Crash
    z80_opcode_CRASH , // 4E - Crash
    z80_opcode_CRASH , // 4F - Crash

    z80_opcode_CRASH , // 50 - Crash
    z80_opcode_CRASH , // 51 - Crash
    z80_opcode_CRASH , // 52 - Crash
    z80_opcode_CRASH , // 53 - Crash
    z80_opcode_CRASH , // 54 - Crash
    z80_opcode_CRASH , // 55 - Crash
    z80_opcode_Misc_IM , // 56 - 2 Bytes - IM 1
    z80_opcode_CRASH , // 57 - Crash
    z80_opcode_CRASH , // 58 - Crash
    z80_opcode_CRASH , // 59 - Crash
    z80_opcode_CRASH , // 5A - Crash
    z80_opcode_CRASH , // 5B - Crash
    z80_opcode_CRASH , // 5C - Crash
    z80_opcode_CRASH , // 5D - Crash
    z80_opcode_Misc_IM , // 5E - 2 Bytes - IM 2
    z80_opcode_CRASH , // 5F - Crash

    z80_opcode_CRASH , // 60 - Crash
    z80_opcode_CRASH , // 61 - Crash
    z80_opcode_CRASH , // 62 - Crash
    z80_opcode_CRASH , // 63 - Crash
    z80_opcode_CRASH , // 64 - Crash
    z80_opcode_CRASH , // 65 - Crash
    z80_opcode_CRASH , // 66 - Crash
    z80_opcode_CRASH , // 67 - Crash
    z80_opcode_CRASH , // 68 - Crash
    z80_opcode_CRASH , // 69 - Crash
    z80_opcode_CRASH , // 6A - Crash
    z80_opcode_CRASH , // 6B - Crash
    z80_opcode_CRASH , // 6C - Crash
    z80_opcode_CRASH , // 6D - Crash
    z80_opcode_CRASH , // 6E - Crash
    z80_opcode_CRASH , // 6F - Crash

    z80_opcode_CRASH , // 70 - Crash
    z80_opcode_CRASH , // 71 - Crash
    z80_opcode_CRASH , // 72 - Crash
    z80_opcode_CRASH , // 73 - Crash
    z80_opcode_CRASH , // 74 - Crash
    z80_opcode_CRASH , // 75 - Crash
    z80_opcode_CRASH , // 76 - Crash
    z80_opcode_CRASH , // 77 - Crash
    z80_opcode_CRASH , // 78 - Crash
    z80_opcode_CRASH , // 79 - Crash
    z80_opcode_CRASH , // 7A - Crash
    z80_opcode_CRASH , // 7B - Crash
    z80_opcode_CRASH , // 7C - Crash
    z80_opcode_CRASH , // 7D - Crash
    z80_opcode_CRASH , // 7E - Crash
    z80_opcode_CRASH , // 7F - Crash

    z80_opcode_CRASH , // 80 - Crash
    z80_opcode_CRASH , // 81 - Crash
    z80_opcode_CRASH , // 82 - Crash
    z80_opcode_CRASH , // 83 - Crash
    z80_opcode_CRASH , // 84 - Crash
    z80_opcode_CRASH , // 85 - Crash
    z80_opcode_CRASH , // 86 - Crash
    z80_opcode_CRASH , // 87 - Crash
    z80_opcode_CRASH , // 88 - Crash
    z80_opcode_CRASH , // 89 - Crash
    z80_opcode_CRASH , // 8A - Crash
    z80_opcode_CRASH , // 8B - Crash
    z80_opcode_CRASH , // 8C - Crash
    z80_opcode_CRASH , // 8D - Crash
    z80_opcode_CRASH , // 8E - Crash
    z80_opcode_CRASH , // 8F - Crash

    z80_opcode_CRASH , // 90 - Crash
    z80_opcode_CRASH , // 91 - Crash
    z80_opcode_CRASH , // 92 - Crash
    z80_opcode_CRASH , // 93 - Crash
    z80_opcode_CRASH , // 94 - Crash
    z80_opcode_CRASH , // 95 - Crash
    z80_opcode_CRASH , // 96 - Crash
    z80_opcode_CRASH , // 97 - Crash
    z80_opcode_CRASH , // 98 - Crash
    z80_opcode_CRASH , // 99 - Crash
    z80_opcode_CRASH , // 9A - Crash
    z80_opcode_CRASH , // 9B - Crash
    z80_opcode_CRASH , // 9C - Crash
    z80_opcode_CRASH , // 9D - Crash
    z80_opcode_CRASH , // 9E - Crash
    z80_opcode_CRASH , // 9F - Crash

    z80_opcode_CRASH , // A0 - Crash
    z80_opcode_CRASH , // A1 - Crash
    z80_opcode_CRASH , // A2 - Crash
    z80_opcode_CRASH , // A3 - Crash
    z80_opcode_CRASH , // A4 - Crash
    z80_opcode_CRASH , // A5 - Crash
    z80_opcode_CRASH , // A6 - Crash
    z80_opcode_CRASH , // A7 - Crash
    z80_opcode_CRASH , // A8 - Crash
    z80_opcode_CRASH , // A9 - Crash
    z80_opcode_CRASH , // AA - Crash
    z80_opcode_CRASH , // AB - Crash
    z80_opcode_CRASH , // AC - Crash
    z80_opcode_CRASH , // AD - Crash
    z80_opcode_CRASH , // AE - Crash
    z80_opcode_CRASH , // AF - Crash

    z80_opcode_CRASH , // B0 - Crash
    z80_opcode_CRASH , // B1 - Crash
    z80_opcode_CRASH , // B2 - Crash
    z80_opcode_CRASH , // B3 - Crash
    z80_opcode_CRASH , // B4 - Crash
    z80_opcode_CRASH , // B5 - Crash
    z80_opcode_CRASH , // B6 - Crash
    z80_opcode_CRASH , // B7 - Crash
    z80_opcode_CRASH , // B8 - Crash
    z80_opcode_CRASH , // B9 - Crash
    z80_opcode_CRASH , // BA - Crash
    z80_opcode_CRASH , // BB - Crash
    z80_opcode_CRASH , // BC - Crash
    z80_opcode_CRASH , // BD - Crash
    z80_opcode_CRASH , // BE - Crash
    z80_opcode_CRASH , // BF - Crash

    z80_opcode_CRASH , // C0 - Crash
    z80_opcode_CRASH , // C1 - Crash
    z80_opcode_CRASH , // C2 - Crash
    z80_opcode_CRASH , // C3 - Crash
    z80_opcode_CRASH , // C4 - Crash
    z80_opcode_CRASH , // C5 - Crash
    z80_opcode_CRASH , // C6 - Crash
    z80_opcode_CRASH , // C7 - Crash
    z80_opcode_CRASH , // C8 - Crash
    z80_opcode_CRASH , // C9 - Crash
    z80_opcode_CRASH , // CA - Crash
    z80_opcode_CRASH , // CB - Crash
    z80_opcode_CRASH , // CC - Crash
    z80_opcode_CRASH , // CD - Crash
    z80_opcode_CRASH , // CE - Crash
    z80_opcode_CRASH , // CF - Crash

    z80_opcode_CRASH , // D0 - Crash
    z80_opcode_CRASH , // D1 - Crash
    z80_opcode_CRASH , // D2 - Crash
    z80_opcode_CRASH , // D3 - Crash
    z80_opcode_CRASH , // D4 - Crash
    z80_opcode_CRASH , // D5 - Crash
    z80_opcode_CRASH , // D6 - Crash
    z80_opcode_CRASH , // D7 - Crash
    z80_opcode_CRASH , // D8 - Crash
    z80_opcode_CRASH , // D9 - Crash
    z80_opcode_CRASH , // DA - Crash
    z80_opcode_CRASH , // DB - Crash
    z80_opcode_CRASH , // DC - Crash
    z80_opcode_CRASH , // DD - Crash
    z80_opcode_CRASH , // DE - Crash
    z80_opcode_CRASH , // DF - Crash

    z80_opcode_CRASH , // E0 - Crash    
    z80_opcode_CRASH , // E1 - Crash    
    z80_opcode_CRASH , // E2 - Crash    
    z80_opcode_CRASH , // E3 - Crash    
    z80_opcode_CRASH , // E4 - Crash    
    z80_opcode_CRASH , // E5 - Crash    
    z80_opcode_CRASH , // E6 - Crash    
    z80_opcode_CRASH , // E7 - Crash    
    z80_opcode_CRASH , // E8 - Crash    
    z80_opcode_CRASH , // E9 - Crash    
    z80_opcode_CRASH , // EA - Crash    
    z80_opcode_CRASH , // EB - Crash    
    z80_opcode_CRASH , // EC - Crash    
    z80_opcode_CRASH , // ED - Crash    
    z80_opcode_CRASH , // EE - Crash    
    z80_opcode_CRASH , // EF - Crash    

    z80_opcode_CRASH , // F0 - Crash
    z80_opcode_CRASH , // F1 - Crash
    z80_opcode_CRASH , // F2 - Crash
    z80_opcode_CRASH , // F3 - Crash
    z80_opcode_CRASH , // F4 - Crash
    z80_opcode_CRASH , // F5 - Crash
    z80_opcode_CRASH , // F6 - Crash
    z80_opcode_CRASH , // F7 - Crash
    z80_opcode_CRASH , // F8 - Crash
    z80_opcode_CRASH , // F9 - Crash
    z80_opcode_CRASH , // FA - Crash
    z80_opcode_CRASH , // FB - Crash
    z80_opcode_CRASH , // FC - Crash
    z80_opcode_CRASH , // FD - Crash
    z80_opcode_CRASH , // FE - Crash
    z80_opcode_CRASH   // FF - Crash
} ;

void z80_opcode_Misc( uint8_t opCode )
{
    uint8_t subOpCode ;

    subOpCode  = z80_memory[ z80_PC++ ] ;

    z80_opcodesMisc[ subOpCode ]( subOpCode ) ;
}

void z80_opcode_Misc_IM( uint8_t opCode )
{
    /**********
     * Opcode - ED.46h - IM 0
     *          ED.56h - IM 1
     *          ED.5Eh - IM 2
     * Size   - 2 Byte
     **********/

    /**********
     * Memory Read!
     **********/
    // Nothing to do.

    /**********
     * Execute!
     **********/
    switch( opCode )
    {
        case 0x46 : // IM 0
            z80_IM = 0 ;
            break ;
        case 0x56 : // IM 1
            z80_IM = 1 ;
            break ;
        case 0x5E : // IM 2
            z80_IM = 2 ;
            break ;
    }

    /**********
     * Memory Write!
     **********/
    // Nothing to do.

    /**********
     * Update Flags.
     **********/
    // Nothing to do.
}
