/**
 * @file z80_verbose.h
 * @author Francesco Sacco
 * @date 25 Mar 2025
 * @brief This library provides functions for verbose output of opcode execution.
 *
 * This library aims to provide functions dedicated to information about opcodes,
 * arguments, and values. This information will be presented via standard output
 * to allow for the monitoring of the execution of each opcode.
 *
 * @see https://github.com/francescosacco/zemu
 */
#ifndef Z80_VERBOSE
#define Z80_VERBOSE

#include <stdint.h>
#include <stdbool.h>
#include "z80_registers.h"

#define INDIRECT           true
#define DIRECT             false

void z80_verbose_start( void ) ;
void z80_verbose_clear( void ) ;
void z80_verbose_print( void ) ;

void z80_verbose_addAddr( uint16_t addr ) ;
void z80_verbose_addOpcode( uint8_t opCode ) ;
void z80_verbose_addMnemonic( char * mnemonic ) ;
void z80_verbose_addFlags( uint8_t flags ) ;

void z80_verbose_addOperator( char * operator , bool indirect ) ;
void z80_verbose_addOperatorByte( uint8_t operator , bool indirect ) ;
void z80_verbose_addOperatorWord( uint16_t operator , bool indirect ) ;
void z80_verbose_addOperatorRegister( eSelectReg_t eReg , bool indirect ) ;
void z80_verbose_addOperatorDoubleRegister( eSelectDoubleReg_t eReg , bool indirect ) ;

void z80_verbose_addComment( char * comment ) ;
void z80_verbose_addCommentNumeric( int val ) ;
void z80_verbose_addCommentByte( uint8_t val ) ;
void z80_verbose_addCommentWord( uint16_t val ) ;

#endif // Z80_VERBOSE
