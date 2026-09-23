#include "z80_opcodes_extended_ix.h"

#include "z80_registers.h"
#include "z80_memory.h"

#include "z80_verbose.h"

void z80_opcode_Extended_IX( uint8_t opCode )
{
    uint8_t subOpCode = z80_memory[ z80_Regs_GetAndIncPC() ] ;
    uint16_t tmp16;
    uint8_t tmp8 ;

    switch(subOpCode)
    {
        case 0x21 : // LD IX,xxxx
            /**********
             * Opcode - DD.21h - LD IX,xxxx
             * Size   - 4 Byte
             **********/
            z80_verbose_addOpcode( opCode ) ;
            z80_verbose_addOpcode( subOpCode ) ;
            z80_verbose_addMnemonic( "LD" ) ;

            tmp16  = z80_memory[ z80_Regs_GetAndIncPC() ] ;
            tmp16 |= ( ( uint16_t ) z80_memory[ z80_Regs_GetAndIncPC() ] ) << 8 ;

            z80_verbose_addOpcode( ( uint8_t ) ( tmp16 &  0x00FF ) ) ;
            z80_verbose_addOpcode( ( uint8_t ) ( tmp16 >>      8 ) ) ;

            z80_verbose_addOperator( "IX" , DIRECT ) ;
            z80_verbose_addOperatorWord( tmp16 , DIRECT ) ;

            z80_Regs_SetIX( tmp16 ) ;

            z80_verbose_addComment( "IX = " ) ;
            z80_verbose_addCommentWord( tmp16 ) ;
            break;
        case 0xF9 : // LD SP,IX
            /**********
             * Opcode - DD.F9h - LD SP,IX
             * Size   - 2 Byte
             **********/
            z80_verbose_addOpcode( opCode ) ;
            z80_verbose_addOpcode( subOpCode ) ;
            z80_verbose_addMnemonic( "LD" ) ;

            z80_verbose_addOperator( "SP" , DIRECT ) ;
            z80_verbose_addOperator( "IX" , DIRECT ) ;

            z80_SP = z80_Regs_GetIX() ;
            break;

        case 0xE5 : // PUSH IX
            /**********
             * Opcode - DD.E5h - PUSH IX
             * Size   - 2 Byte
             **********/
            z80_verbose_addOpcode( opCode ) ;
            z80_verbose_addOpcode( subOpCode ) ;
            z80_verbose_addMnemonic( "PUSH" ) ;

            tmp16 = z80_Regs_GetIX() ;
            z80_verbose_addOperator( "IX" , DIRECT ) ;

            z80_memory[ --z80_SP ] = ( uint8_t ) z80_Regs_GetReg( ( uint8_t ) ( tmp16 >>      8 ) ) ;
            z80_memory[ --z80_SP ] = ( uint8_t ) z80_Regs_GetReg( ( uint8_t ) ( tmp16 &  0x00FF ) ) ;
            break;

        case 0xE1 : // POP IX
            /**********
             * Opcode - DD.E1h - POP IX
             * Size   - 2 Byte
             **********/
            z80_verbose_addOpcode( opCode ) ;
            z80_verbose_addOpcode( subOpCode ) ;
            z80_verbose_addMnemonic( "POP" ) ;

            tmp16 =   ( uint16_t ) z80_memory[ z80_SP++ ]        ; // LSB
            tmp16 = ( ( uint16_t ) z80_memory[ z80_SP++ ] ) << 8 ; // MSB

            z80_Regs_SetIX( tmp16 ) ;
            z80_verbose_addOperator( "IX" , DIRECT ) ;
            break;

        case 0x39 : // ADD IX,SP
            /**********
             * Opcode - DD.E5h - ADD IX,SP
             * Size   - 2 Byte
             **********/
            z80_verbose_addOpcode( opCode ) ;
            z80_verbose_addOpcode( subOpCode ) ;
            z80_verbose_addMnemonic( "ADD" ) ;

            tmp16 = z80_Regs_GetIX() ;
            z80_verbose_addOperator( "IX" , DIRECT ) ;

            tmp16 += z80_SP ;
            z80_verbose_addOperator( "SP" , DIRECT ) ;

            z80_flags.c = ( tmp16 & 0x8000 ) ? ( 1 ) : ( 0 ) ;
            // TODO! z80_flags.h
            z80_Flags_ResetN() ;
            break;
        case 0x7E : // LD A,(IX + x)
            /**********
             * Opcode - DD.7Eh - LD A,(IX + x)
             * Size   - 3 Byte
             **********/
            z80_verbose_addOpcode( opCode ) ;
            z80_verbose_addOpcode( subOpCode ) ;
            z80_verbose_addMnemonic( "LD" ) ;

            tmp8 = z80_memory[ z80_Regs_GetAndIncPC() ] ;
            z80_verbose_addOpcode( tmp8 ) ;

            z80_verbose_addOperatorRegister( eSelectReg_regA , DIRECT ) ;
            z80_verbose_addOperator( "IX + D" , INDIRECT ) ;

            tmp16 = z80_Regs_GetIX() ;
            tmp16 = ( uint16_t ) ( ( int16_t ) tmp16 ) + ( ( int16_t ) tmp8 ) ;

            z80_Regs_SetReg( eSelectReg_regA , z80_memory[ tmp16 ] ) ;

            z80_verbose_addComment( "A = " ) ;
            z80_verbose_addCommentByte( z80_memory[ tmp16 ] ) ;
            break ;
        default :
            z80_verbose_addOpcode( opCode ) ;
            z80_verbose_addOpcode( subOpCode ) ;
            z80_verbose_addComment( "Not implemented!" ) ;
            break ;
    }

}
