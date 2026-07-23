ASSEMBLER=./bin/assembler
LINKER=./bin/linker
EMULATOR=./bin/emulator

${ASSEMBLER} -o out/main.o tests/linker_reloc_test/main.s
${ASSEMBLER} -o out/handler.o tests/linker_reloc_test/handler.s
${ASSEMBLER} -o out/isr_terminal.o tests/linker_reloc_test/isr_terminal.s
${ASSEMBLER} -o out/isr_timer.o tests/linker_reloc_test/isr_timer.s
${LINKER} -relocatable \
  -o out/interrupt_handler.o \
  out/isr_terminal.o out/isr_timer.o out/handler.o
${LINKER} -hex \
  -place=code@0x40000000 \
  -o out/program.hex \
  out/main.o out/interrupt_handler.o
${EMULATOR} out/program.hex