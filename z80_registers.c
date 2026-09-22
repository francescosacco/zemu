#include <z80_registers.h>
#include <z80_memory.h>

uint8_t z80_A ;

uint8_t z80_B ;
uint8_t z80_C ;
uint8_t z80_D ;
uint8_t z80_E ;

uint16_t z80_PC ;
uint16_t z80_HL ;
uint16_t z80_SP ;

bool z80_EI ;
uint8_t z80_IM ;

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
