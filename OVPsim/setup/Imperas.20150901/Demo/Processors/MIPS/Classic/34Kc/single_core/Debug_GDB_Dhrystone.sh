#!/bin/sh
# Debug_GDB_Dhrystone.sh

${IMPERAS_ISS} --verbose --program ../../../Applications/dhrystone/dhrystone.CS_MIPS32LE-O1-g.elf \
    --processorvendor mips.ovpworld.org --processorname mips32 --variant 34Kc \
    --gdbconsole --numprocessors 1     --parameter endian=little --semihostname mips32Newlib --semihostvendor mips.ovpworld.org  --gdbinit dhrystone.gdb $@ 

