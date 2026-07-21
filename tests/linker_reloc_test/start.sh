#!/bin/bash

ASSEMBLER=assembler
LINKER=linker

${ASSEMBLER} -o out/A.o tests/linker_reloc_test/A.s
${ASSEMBLER} -o out/B.o tests/linker_reloc_test/B.s
${ASSEMBLER} -o out/C.o tests/linker_reloc_test/C.s
${LINKER} -hex \
  -o program.hex \
  out/A.o out/B.o out/C.o &> tests/linker_reloc_test/out