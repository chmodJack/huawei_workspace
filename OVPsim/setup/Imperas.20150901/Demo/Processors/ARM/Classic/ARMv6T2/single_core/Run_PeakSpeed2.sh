#!/bin/sh
# Run_PeakSpeed2.sh

${IMPERAS_ISS} --verbose --program ../../../Applications/peakSpeed2/peakSpeed2.ARM7-O0-g.elf \
    --processorvendor arm.ovpworld.org --processorname arm --variant ARMv6T2 \
     --numprocessors 1     --parameter compatibility=nopSVC --parameter UAL=1 --parameter endian=little   $@     -argv 500000000

