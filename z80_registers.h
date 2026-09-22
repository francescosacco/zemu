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

extern uint16_t z80_PC ;
extern uint16_t z80_HL ;
extern uint16_t z80_SP ;
extern bool z80_EI ;
extern uint8_t z80_IM ;

uint8_t z80_Regs_GetReg( eSelectReg_t eReg ) ;
void    z80_Regs_SetReg( eSelectReg_t eReg , uint8_t value ) ;

uint16_t z80_Regs_GetDReg( eSelectDoubleReg_t eReg ) ;
void     z80_Regs_SetDReg( eSelectDoubleReg_t eReg , uint16_t value ) ;

#endif // Z80_REGISTERS_H
