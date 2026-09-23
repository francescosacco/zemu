#include <z80_emulator.h>

#include <stdio.h>
#include <string.h>
#include <stdbool.h>

// Debug!
#include <stdlib.h>
#include "z80_verbose.h"

#include "z80_io.h"
#include "z80_registers.h"

#include "z80_opcodes_ctrl.h"
#include "z80_opcodes_jump.h"
#include "z80_opcodes_load.h"
#include "z80_opcodes_logic.h"
#include "z80_opcodes_math.h"
#include "z80_opcodes_extended_ix.h"
#include "z80_opcodes_extended_bits.h"
#include "z80_opcodes_extended_misc.h"

typedef void ( * pFunc_t )( uint8_t ) ;
bool z80_verbose_mode = false ;


void z80_opcode_CRASH( uint8_t opCode ) ;

pFunc_t z80_opcodesExecution[] =
{
    z80_opcode_NOP        , // 00 - 1 Byte  - NOP
    z80_opcode_LoadDRegC  , // 01 - 3 Bytes - LD BC,xxxx
    z80_opcode_LoadAInd   , // 02 - 1 Byte  - LD (BC),A
    z80_opcode_IncDecDReg , // 03 - 1 Byte  - INC BC
    z80_opcode_IncDecReg  , // 04 - 1 Byte  - INC B
    z80_opcode_IncDecReg  , // 05 - 1 Byte  - DEC B
    z80_opcode_LoadRegC   , // 06 - 2 Bytes - LD B,xx
    z80_opcode_CRASH      , // 07 - Crash
    z80_opcode_CRASH      , // 08 - Crash
    z80_opcode_CRASH      , // 09 - Crash
    z80_opcode_LoadAInd   , // 0A - 1 Byte  - LD A,(BC)
    z80_opcode_IncDecDReg , // 0B - 1 Byte  - DEC BC
    z80_opcode_IncDecReg  , // 0C - 1 Byte  - INC C
    z80_opcode_IncDecReg  , // 0D - 1 Byte  - DEC C
    z80_opcode_LoadRegC   , // 0E - 2 Bytes - LD C,xx
    z80_opcode_CRASH      , // 0F - Crash

    z80_opcode_DJNZ       , // 10 - 2 Bytes - DJNZ xx
    z80_opcode_LoadDRegC  , // 11 - 3 Bytes - LD DE,xxxx
    z80_opcode_LoadAInd   , // 12 - 1 Byte  - LD (DE),A
    z80_opcode_IncDecDReg , // 13 - 1 Byte  - INC DE
    z80_opcode_IncDecReg  , // 14 - 1 Byte  - INC D
    z80_opcode_IncDecReg  , // 15 - 1 Byte  - DEC D
    z80_opcode_LoadRegC   , // 16 - 2 Bytes - LD D,xx
    z80_opcode_CRASH      , // 17 - Crash
    z80_opcode_JR         , // 18 - 2 Bytes - JR xx
    z80_opcode_CRASH      , // 19 - Crash
    z80_opcode_LoadAInd   , // 1A - 1 Byte  - LD A,(DE)
    z80_opcode_IncDecDReg , // 1B - 1 Byte  - DEC DE
    z80_opcode_IncDecReg  , // 1C - 1 Byte  - INC E
    z80_opcode_IncDecReg  , // 1D - 1 Byte  - DEC E
    z80_opcode_LoadRegC   , // 1E - 2 Bytes - LD E,xx
    z80_opcode_CRASH      , // 1F - Crash

    z80_opcode_JR         , // 20 - 2 Bytes - JR NZ,xx
    z80_opcode_LoadDRegC  , // 21 - 3 Bytes - LD HL,xxxx
    z80_opcode_LoadMemHL  , // 22 - 3 Bytes - LD (xxxx),HL
    z80_opcode_IncDecDReg , // 23 - 1 Byte  - INC HL
    z80_opcode_IncDecReg  , // 24 - 1 Byte  - INC H
    z80_opcode_IncDecReg  , // 25 - 1 Byte  - DEC H
    z80_opcode_LoadRegC   , // 26 - 2 Bytes - LD H,xx
    z80_opcode_CRASH      , // 27 - Crash
    z80_opcode_JR         , // 28 - 2 Bytes - JR Z,xx
    z80_opcode_CRASH      , // 29 - Crash
    z80_opcode_LoadMemHL  , // 2A - 3 Bytes - LD HL,(xxxx)
    z80_opcode_IncDecDReg , // 2B - 1 Byte  - DEC HL
    z80_opcode_IncDecReg  , // 2C - 1 Byte  - INC L
    z80_opcode_IncDecReg  , // 2D - 1 Byte  - DEC L
    z80_opcode_LoadRegC   , // 2E - 2 Bytes - LD L,xx
    z80_opcode_CRASH      , // 2F - Crash

    z80_opcode_JR         , // 30 - 2 Bytes - JR NC,xx
    z80_opcode_LoadDRegC  , // 31 - 3 Bytes - LD SP,xxxx
    z80_opcode_LoadAMem   , // 32 - 3 Bytes - LD (xxxx),A
    z80_opcode_IncDecDReg , // 33 - 1 Byte  - INC SP
    z80_opcode_IncDecReg  , // 34 - 1 Byte  - INC (HL)
    z80_opcode_IncDecReg  , // 35 - 1 Byte  - DEC (HL)
    z80_opcode_LoadRegC   , // 36 - 2 Bytes - LD (HL),xx
    z80_opcode_CRASH      , // 37 - Crash
    z80_opcode_JR         , // 38 - 2 Bytes - JR C,xx
    z80_opcode_CRASH      , // 39 - Crash
    z80_opcode_LoadAMem   , // 3A - 3 Bytes - LD A,(xxxx)
    z80_opcode_IncDecDReg , // 3B - 1 Byte  - DEC SP
    z80_opcode_IncDecReg  , // 3C - 1 Byte  - INC A
    z80_opcode_IncDecReg  , // 3D - 1 Byte  - DEC A
    z80_opcode_LoadRegC   , // 3E - 2 Bytes - LD A,xx
    z80_opcode_CRASH      , // 3F - Crash

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
    z80_opcode_HALT       , // 76 - 1 Byte  - HALT
    z80_opcode_LoadRegReg , // 77 - 1 Byte  - LD (HL),A
    z80_opcode_LoadRegReg , // 78 - 1 Byte  - LD A,B
    z80_opcode_LoadRegReg , // 79 - 1 Byte  - LD A,C
    z80_opcode_LoadRegReg , // 7A - 1 Byte  - LD A,D
    z80_opcode_LoadRegReg , // 7B - 1 Byte  - LD A,E
    z80_opcode_LoadRegReg , // 7C - 1 Byte  - LD A,H
    z80_opcode_LoadRegReg , // 7D - 1 Byte  - LD A,L
    z80_opcode_LoadRegReg , // 7E - 1 Byte  - LD A,(HL)
    z80_opcode_LoadRegReg , // 7F - 1 Byte  - LD A,A

    z80_opcode_ADD , // 80 - 1 Byte  - ADD A,B
    z80_opcode_ADD , // 81 - 1 Byte  - ADD A,C
    z80_opcode_ADD , // 82 - 1 Byte  - ADD A,D
    z80_opcode_ADD , // 83 - 1 Byte  - ADD A,E
    z80_opcode_ADD , // 84 - 1 Byte  - ADD A,H
    z80_opcode_ADD , // 85 - 1 Byte  - ADD A,L
    z80_opcode_ADD , // 86 - 1 Byte  - ADD A,(HL)
    z80_opcode_ADD , // 87 - 1 Byte  - ADD A,A
    z80_opcode_ADD , // 88 - 1 Byte  - ADC A,B
    z80_opcode_ADD , // 89 - 1 Byte  - ADC A,C
    z80_opcode_ADD , // 8A - 1 Byte  - ADC A,D
    z80_opcode_ADD , // 8B - 1 Byte  - ADC A,E
    z80_opcode_ADD , // 8C - 1 Byte  - ADC A,H
    z80_opcode_ADD , // 8D - 1 Byte  - ADC A,L
    z80_opcode_ADD , // 8E - 1 Byte  - ADC A,(HL)
    z80_opcode_ADD , // 8F - 1 Byte  - ADC A,A

    z80_opcode_SUB , // 90 - 1 Byte  - SUB A,B
    z80_opcode_SUB , // 91 - 1 Byte  - SUB A,C
    z80_opcode_SUB , // 92 - 1 Byte  - SUB A,D
    z80_opcode_SUB , // 93 - 1 Byte  - SUB A,E
    z80_opcode_SUB , // 94 - 1 Byte  - SUB A,H
    z80_opcode_SUB , // 95 - 1 Byte  - SUB A,L
    z80_opcode_SUB , // 96 - 1 Byte  - SUB A,(HL)
    z80_opcode_SUB , // 97 - 1 Byte  - SUB A,A
    z80_opcode_SUB , // 98 - 1 Byte  - SBC A,B
    z80_opcode_SUB , // 99 - 1 Byte  - SBC A,C
    z80_opcode_SUB , // 9A - 1 Byte  - SBC A,D
    z80_opcode_SUB , // 9B - 1 Byte  - SBC A,E
    z80_opcode_SUB , // 9C - 1 Byte  - SBC A,H
    z80_opcode_SUB , // 9D - 1 Byte  - SBC A,L
    z80_opcode_SUB , // 9E - 1 Byte  - SBC A,(HL)
    z80_opcode_SUB , // 9F - 1 Byte  - SBC A,A

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
    z80_opcode_CP    , // B8 - 1 Byte - CP B
    z80_opcode_CP    , // B9 - 1 Byte - CP C
    z80_opcode_CP    , // BA - 1 Byte - CP D
    z80_opcode_CP    , // BB - 1 Byte - CP E
    z80_opcode_CP    , // BC - 1 Byte - CP H
    z80_opcode_CP    , // BD - 1 Byte - CP L
    z80_opcode_CP    , // BE - 1 Byte - CP (HL)
    z80_opcode_CP    , // BF - 1 Byte - CP A

    z80_opcode_RET           , // C0 - 1 Byte  - RET NZ
    z80_opcode_POP           , // C1 - 1 Byte  - POP BC
    z80_opcode_JUMP          , // C2 - 3 Bytes - JP NZ,xxxx
    z80_opcode_JUMP          , // C3 - 3 Bytes - JP xxxx
    z80_opcode_CALL          , // C4 - 3 Bytes - CALL NZ,xxxx
    z80_opcode_PUSH          , // C5 - 1 Byte  - PUSH BC
    z80_opcode_ADDConst      , // C6 - 2 Bytes - ADD A,xx
    z80_opcode_RST           , // C7 - 1 Byte  - RST 00h
    z80_opcode_RET           , // C8 - 1 Byte  - RET Z
    z80_opcode_RET           , // C9 - 1 Byte  - RET
    z80_opcode_JUMP          , // CA - 3 Bytes - JP Z,xxxx
    z80_opcode_Extended_Bits , // CB - Extended
    z80_opcode_CALL          , // CC - 3 Bytes - CALL Z,xxxx
    z80_opcode_CALL          , // CD - 3 Bytes - CALL xxxx
    z80_opcode_CRASH         , // CE - Crash
    z80_opcode_RST           , // CF - 1 Byte  - RST 08h

    z80_opcode_RET         , // D0 - 1 Byte  - RET NC
    z80_opcode_POP         , // D1 - 1 Byte  - POP DE
    z80_opcode_JUMP        , // D2 - 3 Bytes - JP NC,xxxx
    z80_opcode_OUT         , // D3 - 2 Bytes - OUT (xx),A
    z80_opcode_CALL        , // D4 - 3 Bytes - CALL NC,xxxx
    z80_opcode_PUSH        , // D5 - 1 Byte  - PUSH DE
    z80_opcode_SUBConst    , // D6 - 2 Bytes - SUB A,xx
    z80_opcode_RST         , // D7 - 1 Byte  - RST 10h
    z80_opcode_RET         , // D8 - 1 Byte  - RET Z
    z80_opcode_CRASH       , // D9 - Crash
    z80_opcode_JUMP        , // DA - 3 Bytes - JP C,xxxx
    z80_opcode_IN          , // DB - 2 Bytes - IN A,(xx)
    z80_opcode_CALL        , // DC - 3 Bytes - CALL C,xxxx
    z80_opcode_Extended_IX , // DD - Extended IX
    z80_opcode_CRASH       , // DE - Crash
    z80_opcode_RST         , // DF - 1 Byte  - RST 18h

    z80_opcode_RET           , // E0 - 1 Byte  - RET NPV
    z80_opcode_POP           , // E1 - 1 Byte  - POP HL
    z80_opcode_JUMP          , // E2 - 3 Bytes - JP NPV,xxxx
    z80_opcode_CRASH         , // E3 - Crash
    z80_opcode_CALL          , // E4 - 3 Bytes - CALL NPV,xxxx
    z80_opcode_PUSH          , // E5 - 1 Byte  - PUSH HL
    z80_opcode_LogicConst    , // E6 - 2 Bytes - AND A,xx
    z80_opcode_RST           , // E7 - 1 Byte  - RST 20h
    z80_opcode_RET           , // E8 - 1 Byte  - RET PV
    z80_opcode_CRASH         , // E9 - Crash
    z80_opcode_JUMP          , // EA - 3 Bytes - JP PV,xxxx
    z80_opcode_EX            , // EB - 1 Byte  - EX DE,HL
    z80_opcode_CALL          , // EC - 3 Bytes - CALL PV,xxxx
    z80_opcode_Extended_Misc , // ED - x Bytes - Misc!
    z80_opcode_LogicConst    , // EE - 2 Bytes - XOR A,xx
    z80_opcode_RST           , // EF - 1 Byte  - RST 28h

    z80_opcode_RET        , // F0 - 1 Byte  - RET NS
    z80_opcode_POP        , // F1 - 1 Byte  - POP AF
    z80_opcode_JUMP       , // FA - 3 Bytes - JP NS,xxxx
    z80_opcode_DEI        , // F3 - 1 Byte  - DI
    z80_opcode_CALL       , // F4 - 3 Bytes - CALL NS,xxxx
    z80_opcode_PUSH       , // F5 - 1 Byte  - PUSH AF
    z80_opcode_LogicConst , // F6 - 2 Bytes - OR A,xx
    z80_opcode_RST        , // F7 - 1 Byte  - RST 30h
    z80_opcode_RET        , // F8 - 1 Byte  - RET S
    z80_opcode_LoadSPHL   , // F9 - 1 Byte  - LD SP,HL
    z80_opcode_JUMP       , // FA - 3 Bytes - JP S,xxxx
    z80_opcode_DEI        , // FB - 1 Byte  - EI
    z80_opcode_CALL       , // FC - 3 Bytes - CALL S,xxxx
    z80_opcode_CRASH      , // FD - Crash
    z80_opcode_CP         , // FE - 2 Bytes - CP XX
    z80_opcode_RST        , // FF - 1 Byte  - RST 38h
} ;

void z80_setVerbose( void )
{
    z80_verbose_mode = true ;
    z80_verbose_start() ;
    z80_verbose_clear() ;
}

void z80_interrupt( void )
{
    if( z80_Regs_GetInterrupt() == false )
    {
        return ;
    }

    switch( z80_Regs_GetIM() )
    {
        case eInterruptMode_0:
            // TODO!
            break;
        case eInterruptMode_1:
            z80_memory[ --z80_SP ] = ( uint8_t ) z80_Regs_GetPC() >> 8 ;
            z80_memory[ --z80_SP ] = ( uint8_t ) z80_Regs_GetPC()      ;

            z80_Regs_SetPC( 0x0038 ) ;
            break;
        case eInterruptMode_2:
            // TODO!
            break;
    }
}

void z80_initializeMemory( void )
{
    memset( z80_memory , 0x00 , MEMORY_SIZE ) ;
}

void z80_reset( void )
{
    z80_verbose_clear() ;
    z80_Regs_SetPC( 0x0000 ) ; // Initialize Program Counter.
    z80_SP = 0xFFFF ;
    z80_Regs_DisableInterrupt() ;
    z80_Regs_SetIM( eInterruptMode_0 ) ;
}

void z80_step( void )
{
    uint8_t opcode ;

    z80_verbose_clear() ;

    z80_verbose_addAddr( z80_Regs_GetPC() ) ;
    opcode = z80_memory[ z80_Regs_GetAndIncPC() ] ;

    z80_opcodesExecution[ opcode ]( opcode ) ;

    z80_verbose_print() ;
}

void z80_opcode_CRASH( uint8_t opCode )
{
    printf( "Not Implemented!\n" ) ;
    printf( "Program Counter %04Xh - OpCode %02Xh\n" , ( z80_Regs_GetPC() - 1 ) , opCode ) ;
    printf( "\n" ) ;

    exit( 0 ) ;
}
