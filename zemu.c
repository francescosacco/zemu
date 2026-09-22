#include <stdio.h>
#include <stdint.h>

#include "z80_emulator.h"
#include "z80_io.h"

#define VERSION                                  "v00.01.01"

void print_help( void ) ;



int main( int argc , char * argv[] )
{
    printf( "ZEMU - %s\n" , VERSION ) ;
    
    z80_initializeMemory() ;
    
    if( argc < 2 )
    {
        print_help() ;
        return( -1 ) ;
    }
    
    FILE * file_in = fopen( argv[ 1 ] , "rb" ) ;
    if( file_in == ( FILE * ) NULL )
    {
        printf( "\tIt's not possible to open \"%s\"\n" , argv[ 1 ] ) ;
        return( -1 ) ;
    }
    
    uint32_t i = 0 ;
    for( i = 0 ; i < MEMORY_SIZE ; i++ )
    {
        int data = getc( file_in ) ;

        if( data == EOF )
        {
            break ;
        }
        
        z80_memory[ i ] = ( uint8_t ) data ;
    }
    
    printf( "\t%u bytes loaded.\n" , i ) ;
    
    fclose( file_in ) ;
    
    z80_io_initialization() ;
    z80_reset() ;
    
    for( ; /* EVER */ ; )
    {
        z80_step() ;
        if( z80_io_newData( 0x81 ) )
        {
            printf( "%c" , z80_io_getData( 0x81 ) ) ;
        }
    }
    
    return( 0 ) ;
}

void print_help( void )
{
    printf( "\n" ) ;
    printf( "\tZEMU <z80_Program.bin> [out=80]\n" ) ;
    printf( "\n" ) ;
}
