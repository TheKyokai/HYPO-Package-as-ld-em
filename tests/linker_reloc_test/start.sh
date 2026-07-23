#!/bin/bash

ASSEMBLER=assembler
LINKER=linker
EMULATOR=emulator

${ASSEMBLER} -o out/A.o tests/linker_reloc_test/A.s
${ASSEMBLER} -o out/B.o tests/linker_reloc_test/B.s
${ASSEMBLER} -o out/C.o tests/linker_reloc_test/C.s
${LINKER} -hex \
  -o tests/linker_reloc_test/program.hex \
  -place=text@0x40000000 out/A.o out/B.o out/C.o
${EMULATOR} tests/linker_reloc_test/program.hex 