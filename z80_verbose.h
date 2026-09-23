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
