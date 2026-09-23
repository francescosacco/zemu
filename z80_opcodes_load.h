#ifndef Z80_OPCODES_LOAD
#define Z80_OPCODES_LOAD

#include <stdint.h>

void z80_opcode_LoadRegC( uint8_t opCode ) ;
void z80_opcode_LoadRegReg( uint8_t opCode ) ;
void z80_opcode_LoadDRegC( uint8_t opCode ) ;
void z80_opcode_LoadAInd( uint8_t opCode ) ;
void z80_opcode_LoadAMem( uint8_t opCode ) ;
void z80_opcode_LoadMemHL( uint8_t opCode ) ;
void z80_opcode_LoadSPHL( uint8_t opCode ) ;

void z80_opcode_PUSH( uint8_t opCode ) ;
void z80_opcode_POP( uint8_t opCode ) ;

#endif // Z80_OPCODES_LOAD
