#ifndef Z80_REGISTERS_H
#define Z80_REGISTERS_H

#include <stdint.h>
#include <stdbool.h>

typedef enum
{
    eSelectReg_regB  = 0x00 , // xxxx.x000
    eSelectReg_regC  = 0x01 , // xxxx.x001
    eSelectReg_regD  = 0x02 , // xxxx.x010
    eSelectReg_regE  = 0x03 , // xxxx.x011
    eSelectReg_regH  = 0x04 , // xxxx.x100
    eSelectReg_regL  = 0x05 , // xxxx.x101
    eSelectReg_memHL = 0x06 , // xxxx.x110
    eSelectReg_regA  = 0x07   // xxxx.x111
} eSelectReg_t ;

typedef enum
{
    eSelectReg_regBC  = 0x00 , // xxxx.xx00
    eSelectReg_regDE  = 0x01 , // xxxx.xx01
    eSelectReg_regHL  = 0x02 , // xxxx.xx10
    eSelectReg_regSP  = 0x03 , // xxxx.xx11
} eSelectDoubleReg_t ;

typedef enum
{
    eInterruptMode_0 = 0x00 ,
    eInterruptMode_1 = 0x01 ,
    eInterruptMode_2 = 0x02 ,
} eInterruptMode_t ;

typedef struct
{
    uint8_t c  : 1; // Carry
    uint8_t n  : 1; // Add/Sub
    uint8_t pv : 1; // Parity/OverFlow
    uint8_t x3 : 1; // Not Used
    uint8_t h  : 1; // Half Carry Flag
    uint8_t x5 : 1; // Not Used
    uint8_t z  : 1; // Zero Flag
    uint8_t s  : 1; // Sign Flag
} z80_flags_t ;

extern uint16_t z80_SP ;
extern z80_flags_t z80_flags ;

uint8_t z80_Regs_GetReg( eSelectReg_t eReg ) ;
void    z80_Regs_SetReg( eSelectReg_t eReg , uint8_t value ) ;

uint16_t z80_Regs_GetDReg( eSelectDoubleReg_t eReg ) ;
void     z80_Regs_SetDReg( eSelectDoubleReg_t eReg , uint16_t value ) ;

// Program Counter.
uint16_t z80_Regs_GetPC( void ) ;
uint16_t z80_Regs_GetAndIncPC( void ) ;
void     z80_Regs_SetPC( uint16_t value ) ;

// Index Register.
uint16_t z80_Regs_GetIX( void ) ;
void     z80_Regs_SetIX( uint16_t value ) ;

// Interrupt Mode.
eInterruptMode_t z80_Regs_GetIM( void ) ;
void             z80_Regs_SetIM( eInterruptMode_t im ) ;

// Interrupt Register.
void z80_Regs_EnableInterrupt( void ) ;
void z80_Regs_DisableInterrupt( void ) ;
bool z80_Regs_GetInterrupt( void ) ;

// Flags.
void z80_Flags_SetCarry( void ) ;
void z80_Flags_ResetCarry( void ) ;
bool z80_Flags_GetCarry( void ) ;

void z80_Flags_SetN( void ) ;
void z80_Flags_ResetN( void ) ;
bool z80_Flags_GetN( void ) ;

void z80_Flags_SetPV( void ) ;
void z80_Flags_ResetPV( void ) ;
bool z80_Flags_GetPV( void ) ;
void z80_Flags_CalculateParity( uint8_t val ) ;

void z80_Flags_SetZero( void ) ;
void z80_Flags_ResetZero( void ) ;
bool z80_Flags_GetZero( void ) ;

void z80_Flags_SetSign( void ) ;
void z80_Flags_ResetSign( void ) ;
bool z80_Flags_GetSign( void ) ;
void z80_Flags_CalculateSign( uint8_t val ) ;

void z80_Flags_SetHalf( void ) ;
void z80_Flags_ResetHalf( void ) ;
bool z80_Flags_GetHalf( void ) ;
void z80_Flags_CalculateHalf_sub( uint8_t a , uint8_t b ) ;
void z80_Flags_CalculateHalf_adc( uint8_t a , uint8_t b , bool c ) ;

void z80_Flags_CalculateOverflowSub( uint8_t a , uint8_t b , uint8_t result ) ;
void z80_Flags_CalculateOverflowAdc( uint8_t a , uint8_t b , bool c , uint8_t result ) ;

#endif // Z80_REGISTERS_H
