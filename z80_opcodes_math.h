#ifndef Z80_OPCODES_MATH
#define Z80_OPCODES_MATH

#include <stdint.h>

void z80_opcode_IncDecDReg( uint8_t opCode ) ;
void z80_opcode_IncDecReg( uint8_t opCode ) ;

void z80_opcode_ADD( uint8_t opCode ) ;
void z80_opcode_ADDConst( uint8_t opCode ) ;

void z80_opcode_SUB( uint8_t opCode ) ;
void z80_opcode_SUBConst( uint8_t opCode ) ;

void z80_opcode_CP( uint8_t opCode ) ;

#endif // Z80_OPCODES_MATH
