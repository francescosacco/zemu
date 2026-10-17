/**
 * @file z80_opcodes_logic.h
 * @author Francesco Sacco
 * @date 25 Mar 2025
 * @brief This library contains the OpCodes for logical operations.
 *
 * The functions presented here implement Z80 opcodes related to
 * logical operations and bit rotations.
 *
 * @see https://github.com/francescosacco/zemu
 */
#ifndef Z80_OPCODES_LOGIC
#define Z80_OPCODES_LOGIC

#include <stdint.h>

/**
 * @bried Performs AND, OR, or XOR logical operations between registers.
 * @param input Opcode to be executed.
 */
void z80_opcode_Logic( uint8_t opCode ) ;

/**
 * @bried Performs AND, OR, or XOR logical operations between a register and a constant.
 * @param input Opcode to be executed.
 */
void z80_opcode_LogicConst( uint8_t opCode ) ;

/**
 * @bried Performs a bit rotation of the accumulator.
 * @param input Opcode to be executed.
 */
void z80_opcode_Rotate( uint8_t opCode ) ;

#endif // Z80_OPCODES_LOGIC
