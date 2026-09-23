#ifndef Z80_OPCODES_JUMP
#define Z80_OPCODES_JUMP

#include <stdint.h>

void z80_opcode_JUMP( uint8_t opCode ) ;
void z80_opcode_JR( uint8_t opCode ) ;

void z80_opcode_CALL( uint8_t opCode ) ;
void z80_opcode_RET( uint8_t opCode ) ;
void z80_opcode_RST( uint8_t opCode ) ;
void z80_opcode_DJNZ( uint8_t opCode ) ;

#endif // Z80_OPCODES_JUMP
