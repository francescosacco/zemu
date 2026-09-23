CC=gcc
CFLAGS=-I.

zemu: zemu.o z80_registers.o z80_memory.o z80_io.o z80_emulator.o z80_opcodes_extended_misc.o z80_opcodes_ctrl.o z80_opcodes_jump.o z80_opcodes_load.o z80_opcodes_logic.o z80_opcodes_math.o z80_opcodes_extended_bits.o z80_opcodes_extended_ix.o z80_verbose.o
	$(CC) -o zemu zemu.o z80_registers.o z80_memory.o z80_io.o z80_emulator.o z80_opcodes_extended_misc.o z80_opcodes_ctrl.o z80_opcodes_jump.o z80_opcodes_load.o z80_opcodes_logic.o z80_opcodes_math.o z80_opcodes_extended_bits.o z80_opcodes_extended_ix.o z80_verbose.o
