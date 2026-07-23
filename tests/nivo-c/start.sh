ASSEMBLER=./bin/assembler
LINKER=./bin/linker
EMULATOR=./bin/emulator

${ASSEMBLER} -o out/main.o tests/nivo-c/main.s
${ASSEMBLER} -o out/handler.o tests/nivo-c/handler.s
${ASSEMBLER} -o out/isr_terminal.o tests/nivo-c/isr_terminal.s
${ASSEMBLER} -o out/isr_timer.o tests/nivo-c/isr_timer.s
${LINKER} -hex \
  -place=code@0x40000000 \
  -o tests/nivo-c/program.hex \
  out/main.o out/isr_terminal.o out/isr_timer.o out/handler.o
${EMULATOR} tests/nivo-c/program.hex