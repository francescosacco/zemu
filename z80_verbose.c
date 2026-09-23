#include "z80_verbose.h"
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

static bool z80_verbose_enabled = false ;

static struct
{
    uint16_t addr ;

    uint8_t opCode[ 4 ] ;
    uint8_t opCodeCount ;

    char mnemonic[ 8 ] ;
    char operators[ 16 ] ;
    char comment[ 32 ] ;

    bool operatorEmpty ;
    bool commentEmpty ;
} z80_verbose_instruction ;

char * z80_verbose_returnRegName( eSelectReg_t eReg ) ;
char * z80_verbose_returnDRegName( eSelectDoubleReg_t eReg ) ;

void z80_verbose_start( void )
{
    z80_verbose_enabled = true ;
    z80_verbose_clear() ;
}

void z80_verbose_clear( void )
{
    z80_verbose_instruction.addr = 0x0000 ;
    z80_verbose_instruction.opCodeCount = 0 ;
    z80_verbose_instruction.operatorEmpty = true ;
    z80_verbose_instruction.commentEmpty  = true ;

    memset( z80_verbose_instruction.mnemonic  , '\0' , sizeof( z80_verbose_instruction.mnemonic  ) ) ;
    memset( z80_verbose_instruction.operators , '\0' , sizeof( z80_verbose_instruction.operators ) ) ;
    memset( z80_verbose_instruction.comment   , '\0' , sizeof( z80_verbose_instruction.comment   ) ) ;
}

void z80_verbose_addAddr( uint16_t addr )
{
    z80_verbose_instruction.addr = addr ;
}

void z80_verbose_addOpcode( uint8_t opCode )
{
    if( z80_verbose_instruction.opCodeCount < 4 )
    {
        z80_verbose_instruction.opCode[ z80_verbose_instruction.opCodeCount++ ] = opCode ;
    }
}

void z80_verbose_addMnemonic( char * mnemonic )
{
    strncpy( z80_verbose_instruction.mnemonic , mnemonic , sizeof( z80_verbose_instruction.mnemonic ) - 1 ) ;
}

void z80_verbose_addOperator( char * operator , bool indirect )
{
    if( z80_verbose_instruction.operatorEmpty == false )
    {
        strcat( z80_verbose_instruction.operators , "," ) ;
    }

    if( indirect )
    {
        strcat( z80_verbose_instruction.operators , "(" ) ;
    }

    strcat( z80_verbose_instruction.operators , operator ) ;

    if( indirect )
    {
        strcat( z80_verbose_instruction.operators , ")" ) ;
    }

    z80_verbose_instruction.operatorEmpty = false ;
}

void z80_verbose_addOperatorByte( uint8_t operator , bool indirect )
{
    char numOperator[ 8 ] ;

    memset( numOperator , '\0' , sizeof( numOperator ) ) ;
    sprintf( numOperator , "%02Xh" , operator ) ;

    z80_verbose_addOperator( numOperator , indirect ) ;
}

void z80_verbose_addOperatorWord( uint16_t operator , bool indirect )
{
    char numOperator[ 8 ] ;

    memset( numOperator , '\0' , sizeof( numOperator ) ) ;
    sprintf( numOperator , "%04Xh" , operator ) ;

    z80_verbose_addOperator( numOperator , indirect ) ;
}

void z80_verbose_addOperatorRegister( eSelectReg_t eReg , bool indirect )
{
    z80_verbose_addOperator( z80_verbose_returnRegName( eReg ) , indirect ) ;
}

void z80_verbose_addOperatorDoubleRegister( eSelectDoubleReg_t eReg , bool indirect )
{
    z80_verbose_addOperator( z80_verbose_returnDRegName( eReg ) , indirect ) ;
}

void z80_verbose_addComment( char * comment )
{
    strcat( z80_verbose_instruction.comment , comment ) ;
    z80_verbose_instruction.commentEmpty = false ;
}

void z80_verbose_addCommentNumeric( int val )
{
    char str[ 32 ] ;

    sprintf( str , "%d" , val ) ;
    z80_verbose_addComment( str ) ;
}

void z80_verbose_addCommentByte( uint8_t val )
{
    char str[ 8 ] ;

    sprintf( str , "%02Xh" , val ) ;
    z80_verbose_addComment( str ) ;
}

void z80_verbose_addCommentWord( uint16_t val )
{
    char str[ 8 ] ;

    sprintf( str , "%04Xh" , val ) ;
    z80_verbose_addComment( str ) ;
}

void z80_verbose_print( void )
{
    int size ;

    if( z80_verbose_enabled == false )
    {
        return ;
    }

    // Print the Address.
    printf( "[%04Xh] " , z80_verbose_instruction.addr ) ;


    // Print the OpCodes.
    size = 0 ;
    for( int i = 0 ; i < z80_verbose_instruction.opCodeCount ; i++ )
    {
        size += printf( "%02X" , z80_verbose_instruction.opCode[ i ] ) ;
        if( i < ( z80_verbose_instruction.opCodeCount - 1 ) )
        {
            size += printf( "." ) ;
        }
    }
    for( ; size < 12 ; size++ )
    {
        printf( " " ) ;
    }
    printf( "- " ) ;

    // Print the mnemonic.
    size = printf( "%s" , z80_verbose_instruction.mnemonic ) ;
    for( ; size < sizeof( z80_verbose_instruction.mnemonic ) ; size++ )
    {
        printf( " " ) ;
    }

    // Print the operators.
    size = printf( "%s" , z80_verbose_instruction.operators ) ;
    for( ; size < sizeof( z80_verbose_instruction.operators ) ; size++ )
    {
        printf( " " ) ;
    }

    // Print the comments.

    if( z80_verbose_instruction.commentEmpty == false )
    {
        printf( " ; " ) ;
        size = printf( "%s" , z80_verbose_instruction.comment ) ;
        for( ; size < sizeof( z80_verbose_instruction.comment ) ; size++ )
        {
            printf( " " ) ;
        }
    }

    printf( "\n" ) ;
}

char * z80_verbose_returnRegName( eSelectReg_t eReg )
{
    switch( eReg )
    {
    case eSelectReg_regB :
        return( "B" ) ;
        break ;
    case eSelectReg_regC :
        return( "C" ) ;
        break ;
    case eSelectReg_regD :
        return( "D" ) ;
        break ;
    case eSelectReg_regE :
        return( "E" ) ;
        break ;
    case eSelectReg_regH :
        return( "H" ) ;
        break ;
    case eSelectReg_regL :
        return( "L" ) ;
        break ;
    case eSelectReg_memHL :
        return( "(HL)" ) ;
        break ;
    case eSelectReg_regA :
        return( "A" ) ;
        break ;
    }

    return( "Unknown" ) ;
}

char * z80_verbose_returnDRegName( eSelectDoubleReg_t eReg )
{
    switch( eReg )
    {
    case eSelectReg_regBC :
        return( "BC" ) ;
        break ;
    case eSelectReg_regDE :
        return( "DE" ) ;
        break ;
    case eSelectReg_regHL :
        return( "HL" ) ;
        break ;
    case eSelectReg_regSP :
        return( "SP" ) ;
        break ;
    }

    return( "Unknown" ) ;
}
