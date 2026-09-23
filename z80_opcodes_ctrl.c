#include "z80_opcodes_ctrl.h"
#include "z80_registers.h"
#include "z80_memory.h"
#include"z80_io.h"

#include "z80_verbose.h"

#include <stdlib.h>

void z80_opcode_DEI( uint8_t opCode )
{
    /**********
     * Opcode - F3h - DI
     *          FBh - EI
     * Size   - 1 Byte
     **********/
    z80_verbose_addOpcode( opCode ) ;

    /**********
     * Execute!
     **********/
    if( opCode & 0x08 ) // Check bit 3.
    {
        // OpCode FBh.
        z80_verbose_addMnemonic( "EI" ) ;
        z80_verbose_addComment( "Enable Interrupts" ) ;

        z80_Regs_EnableInterrupt() ;
    }
    else
    {
        // OpCode F3h.
        z80_verbose_addMnemonic( "DI" ) ;
        z80_verbose_addComment( "Disable Interrupts" ) ;

        z80_Regs_DisableInterrupt() ;
    }
}

void z80_opcode_OUT( uint8_t opCode )
{
    uint8_t tmp ;
    uint8_t port ;

    /**********
     * Opcode - D3h - OUT (xx),A
     * Size   - 1 Byte
     **********/
    z80_verbose_addOpcode( opCode ) ;
    z80_verbose_addMnemonic( "OUT" ) ;

    /**********
     * Memory Read!
     **********/
    port = z80_memory[ z80_Regs_GetAndIncPC() ] ;
    z80_verbose_addOperatorByte( port , INDIRECT ) ;

    // Read Accumulator.
    tmp = z80_Regs_GetReg( eSelectReg_regA ) ;
    z80_verbose_addOperatorRegister( eSelectReg_regA , DIRECT ) ;

    /**********
     * Execute!
     **********/
    z80_io_setData( port , tmp ) ;

    z80_verbose_addComment( "Data output is " ) ;
    z80_verbose_addCommentByte( tmp ) ;
}

void z80_opcode_IN( uint8_t opCode )
{
    uint8_t tmp ;
    uint8_t port ;

    /**********
     * Opcode - D3h - IN A,(xx)
     * Size   - 1 Byte
     **********/
    z80_verbose_addOpcode( opCode ) ;
    z80_verbose_addMnemonic( "IN" ) ;

    /**********
     * Memory Read!
     **********/
    port = z80_memory[ z80_Regs_GetAndIncPC() ] ;

    z80_verbose_addOperatorRegister( eSelectReg_regA , DIRECT ) ;
    z80_verbose_addOperatorByte( port , INDIRECT ) ;

    /**********
     * Execute!
     **********/
    tmp = z80_io_getData( port ) ;

    // Read Accumulator.
    z80_Regs_SetReg( eSelectReg_regA , tmp ) ;
}

void z80_opcode_NOP( uint8_t opCode )
{
    /**********
     * Opcode - 00h - NOP
     * Size   - 1 Byte
     **********/
    z80_verbose_addOpcode( opCode ) ;
    z80_verbose_addMnemonic( "NOP" ) ;
}

void z80_opcode_EX( uint8_t opCode )
{
    uint16_t tmpDE ;
    uint16_t tmpHL ;

    /**********
     * Opcode - EBh - EX DE,HL
     * Size   - 1 Byte
     **********/
    z80_verbose_addOpcode( opCode ) ;
    z80_verbose_addMnemonic( "EX" ) ;

    /**********
     * Memory Read!
     **********/

    tmpHL = z80_Regs_GetDReg( eSelectReg_regHL ) ;
    tmpDE = z80_Regs_GetDReg( eSelectReg_regDE ) ;

    z80_Regs_SetDReg( eSelectReg_regHL , tmpDE ) ;
    z80_Regs_SetDReg( eSelectReg_regDE , tmpHL ) ;

    z80_verbose_addOperatorDoubleRegister( eSelectReg_regDE , DIRECT ) ;
    z80_verbose_addOperatorDoubleRegister( eSelectReg_regHL , DIRECT ) ;
}

void z80_opcode_HALT( uint8_t opCode )
{
    /**********
     * Opcode - 76h - HALT
     * Size   - 1 Byte
     **********/
    z80_verbose_addOpcode( opCode ) ;
    z80_verbose_addMnemonic( "HALT" ) ;

    z80_verbose_addComment( "Processor suspended.\n\n" ) ;

    // This opcode terminates with the program.
    z80_verbose_print() ;
    exit( 0 ) ;
}
