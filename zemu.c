#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h> // To use strcmp().

// To implement kbhit().
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>

#include "z80_emulator.h"
#include "z80_io.h"


#define VERSION                                  "v00.01.00"

void print_help( void ) ;
int kbhit( void ) ;

int main( int argc , char * argv[] )
{
    bool setVerbose = false ;

    printf( "ZEMU - %s - github.com/francescosacco\n" , VERSION ) ;
    printf( "\n" ) ;

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
    printf( "\n" ) ;

    fclose( file_in ) ;

    if( ( argc > 2 ) && ( strcmp( argv[ 2 ] , "--verbose" ) == 0 ) )
    {
        z80_setVerbose() ;
        setVerbose = true ;
    }
    z80_io_initialization() ;
    z80_reset() ;

    for( ; /* EVER */ ; )
    {
        z80_step() ;
        if( ( z80_io_newData( 0x81 ) ) && ( setVerbose == false ) )
        {
            printf( "%c" , z80_io_getData( 0x81 ) ) ;
        }

        if( kbhit() )
        {
            char chr = getchar() ;
            // printf("%c", chr) ; //Echo
            z80_io_setData( 0x80 , 0x01 ) ;
            z80_io_setData( 0x81 , chr ) ;
            z80_interrupt() ;
        }

        if( setVerbose )
        {
            // In verbose mode, add a 10ms delay.
            usleep( 10000 ) ;
        }
    }

    return( 0 ) ;
}

void print_help( void )
{
    printf( "\n" ) ;
    printf( "\tZEMU <z80_Program.bin> [--verbose]\n" ) ;
    printf( "\n" ) ;
}

// The keyboard hit code originally came from: https://www.vivaolinux.com.br/topico/C-C++/kbhit()-no-linux
int kbhit( void )
{
  struct termios oldt, newt;
  int ch;
  int oldf;

  tcgetattr(STDIN_FILENO, &oldt);
  newt = oldt;
  newt.c_lflag &= ~(ICANON | ECHO);
  tcsetattr(STDIN_FILENO, TCSANOW, &newt);
  oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
  fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

  ch = getchar();

  tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
  fcntl(STDIN_FILENO, F_SETFL, oldf);

  if(ch != EOF)
  {
    ungetc(ch, stdin);
    return 1;
  }

  return 0;
}
