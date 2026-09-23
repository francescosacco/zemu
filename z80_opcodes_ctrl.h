#ifndef Z80_OPCODES_CTRL
#define Z80_OPCODES_CTRL

#include <stdint.h>

void z80_opcode_DEI( uint8_t opCode ) ;
void z80_opcode_OUT( uint8_t opCode ) ;
void z80_opcode_IN( uint8_t opCode ) ;
void z80_opcode_NOP( uint8_t opCode ) ;
void z80_opcode_EX( uint8_t opCode ) ;
void z80_opcode_HALT( uint8_t opCode ) ;

#endif // Z80_OPCODES_CTRL
