#include <z80_registers.h>
#include <z80_memory.h>

static uint8_t z80_A ;

static uint8_t z80_B ;
static uint8_t z80_C ;
static uint8_t z80_D ;
static uint8_t z80_E ;

static uint16_t z80_PC ;
static uint16_t z80_IX ;
uint16_t z80_HL ;
uint16_t z80_SP ;

static bool z80_EI ;
static eInterruptMode_t z80_IM ;

z80_flags_t z80_flags ;

uint8_t z80_Regs_GetReg( eSelectReg_t eReg )
{
    uint8_t reg = 0x00 ;

    switch( eReg )
    {
        case eSelectReg_regB  :
            reg = z80_B ;
            break ;
        case eSelectReg_regC  :
            reg = z80_C ;
            break ;
        case eSelectReg_regD  :
            reg = z80_D ;
            break ;
        case eSelectReg_regE  :
            reg = z80_E ;
            break ;
        case eSelectReg_regH  :
            reg = ( ( uint8_t ) ( z80_HL >> 8 ) ) ;
            break ;
        case eSelectReg_regL  :
            reg = ( ( uint8_t ) z80_HL ) ;
            break ;
        case eSelectReg_memHL :
            reg = z80_memory[ z80_HL ] ;
            break ;
        case eSelectReg_regA  :
            reg = z80_A ;
            break ;
    }

    return( reg ) ;
}

void z80_Regs_SetReg( eSelectReg_t eReg , uint8_t value )
{
    switch( eReg )
    {
        case eSelectReg_regB  :
            z80_B = value ;
            break ;
        case eSelectReg_regC  :
            z80_C = value ;
            break ;
        case eSelectReg_regD  :
            z80_D = value ;
            break ;
        case eSelectReg_regE  :
            z80_E = value ;
            break ;
        case eSelectReg_regH  :
            {
                uint16_t tmp = ( uint16_t ) value ;

                tmp <<= 8 ;
                tmp  &= 0xFF00 ;

                z80_HL &= 0x00FF ;
                z80_HL |= tmp ;
            }
            break ;
        case eSelectReg_regL  :
            {
                z80_HL &= 0xFF00 ;
                z80_HL |= ( uint16_t ) value ;
            }
            break ;
        case eSelectReg_memHL :
            z80_memory[ z80_HL ] = value ;
            break ;
        case eSelectReg_regA  :
            z80_A = value ;
            break ;
    }
}

uint16_t z80_Regs_GetDReg( eSelectDoubleReg_t eReg )
{
    uint16_t value = 0x0000 ;

    switch( eReg )
    {
        case eSelectReg_regBC :
            value  =   ( uint16_t ) z80_C ;
            value |= ( ( uint16_t ) z80_B ) << 8 ;
            break ;
        case eSelectReg_regDE :
            value  =   ( uint16_t ) z80_E ;
            value |= ( ( uint16_t ) z80_D ) << 8 ;
            break ;
        case eSelectReg_regHL :
            value = z80_HL ;
            break ;
        case eSelectReg_regSP :
            value = z80_SP ;
            break ;
    }

    return( value ) ;
}

void z80_Regs_SetDReg( eSelectDoubleReg_t eReg , uint16_t value )
{
    switch( eReg )
    {
        case eSelectReg_regBC :
            z80_C = ( uint8_t ) ( value ) ;
            z80_B = ( uint8_t ) ( value >> 8 ) ;
            break ;
        case eSelectReg_regDE :
            z80_E = ( uint8_t ) ( value ) ;
            z80_D = ( uint8_t ) ( value >> 8 ) ;
            break ;
        case eSelectReg_regHL :
            z80_HL = value ;
            break ;
        case eSelectReg_regSP :
            z80_SP = value ;
            break ;
    }
}

uint16_t z80_Regs_GetPC( void )
{
    uint16_t tmp16 ;

    tmp16 = z80_PC ;

    return( tmp16 ) ;
}

uint16_t z80_Regs_GetAndIncPC( void )
{
    uint16_t tmp16 ;

    tmp16 = z80_PC ;
    z80_PC++ ;

    return( tmp16 ) ;
}

void z80_Regs_SetPC( uint16_t value )
{
    z80_PC = value ;
}

uint16_t z80_Regs_GetIX( void )
{
    return( z80_IX ) ;
}

void z80_Regs_SetIX( uint16_t value )
{
    z80_IX = value ;
}

eInterruptMode_t z80_Regs_GetIM( void )
{
    return( z80_IM ) ;
}

void z80_Regs_SetIM( eInterruptMode_t im )
{
    z80_IM = im ;
}

void z80_Regs_EnableInterrupt( void )
{
    z80_EI = true ;
}

void z80_Regs_DisableInterrupt( void )
{
    z80_EI = false ;
}

bool z80_Regs_GetInterrupt( void )
{
    return( z80_EI ) ;
}

void z80_Flags_SetCarry( void )
{
    z80_flags.c = 1 ;
}

void z80_Flags_ResetCarry( void )
{
    z80_flags.c = 0 ;
}

bool z80_Flags_GetCarry( void )
{
    return( z80_flags.c == 1 ) ;
}

void z80_Flags_SetN( void )
{
    z80_flags.n = 1 ;
}

void z80_Flags_ResetN( void )
{
    z80_flags.n = 0 ;
}

bool z80_Flags_GetN( void )
{
    return( z80_flags.n == 1 ) ;
}

void z80_Flags_SetPV( void )
{
    z80_flags.pv = 1 ;
}

void z80_Flags_ResetPV( void )
{
    z80_flags.pv = 0 ;
}

bool z80_Flags_GetPV( void )
{
    return( z80_flags.pv == 1 ) ;
}

void z80_Flags_CalculateParity( uint8_t val )
{
    uint8_t parity = 0x00 ;
	uint8_t i ;

	for( i = 0 ; i < 8 ; i++ )
	{
		if( val & ( 1 << i ) )
		{
			parity++ ;
		}
	}

    z80_flags.pv = !( parity & 0x01 ) ;
}

void z80_Flags_CalculateOverflow( int16_t val )
{
    if( ( val > 127 ) || ( val < -128 ) )
    {
        z80_flags.pv = 1 ;
    }
    else
    {
        z80_flags.pv = 0 ;
    }
}

void z80_Flags_CalculateOverflowSub( uint8_t a , uint8_t b , uint8_t result )
{
    z80_flags.pv = ( ( ( a ^ b ) & ( a ^ result ) & 0x80 ) != 0 ) ;
}

void z80_Flags_CalculateOverflowAdc( uint8_t a , uint8_t b , bool c , uint8_t result )
{
    uint8_t tmpB = b ;
    
    tmpB += ( c ) ? ( 0x01 ) : ( 0x00 ) ;
    z80_flags.pv = ( ( ~( a ^ tmpB ) & ( a ^ result ) & 0x80 ) != 0 ) ;
}

void z80_Flags_SetZero( void )
{
    z80_flags.z = 1 ;
}

void z80_Flags_ResetZero( void )
{
    z80_flags.z = 0 ;
}

bool z80_Flags_GetZero( void )
{
    return( z80_flags.z == 1 ) ;
}

void z80_Flags_CalculateZero( uint8_t val )
{
    if( 0x00 == val )
    {
        z80_flags.z = 1 ;
    }
    else
    {
        z80_flags.z = 0 ;
    }
}

void z80_Flags_SetSign( void )
{
    z80_flags.s = 1 ;
}

void z80_Flags_ResetSign( void )
{
    z80_flags.s = 0 ;
}

bool z80_Flags_GetSign( void )
{
    return( z80_flags.s == 1 ) ;
}

void z80_Flags_CalculateSign( uint8_t val )
{
    if( val & 0x80 )
    {
        z80_flags.s = 1 ;
    }
    else
    {
        z80_flags.s = 0 ;
    }
}

void z80_Flags_SetHalf( void )
{
    z80_flags.h = 1 ;
}

void z80_Flags_ResetHalf( void )
{
    z80_flags.h = 0 ;
}

bool z80_Flags_GetHalf( void )
{
    return( z80_flags.h == 1 ) ;
}

void z80_Flags_CalculateHalf_sub( uint8_t a , uint8_t b )
{
    z80_flags.h = ( ( a & 0x0F ) < ( b & 0x0F ) ) ;
}

void z80_Flags_CalculateHalf_adc( uint8_t a , uint8_t b , bool c )
{
    uint8_t carry = ( c ) ? ( 0x01 ) : ( 0x00 ) ;
    z80_flags.h = ( ( a & 0x0F ) + ( b & 0x0F ) + carry ) > 0x0F ;
}
