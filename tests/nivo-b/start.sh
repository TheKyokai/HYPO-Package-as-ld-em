ASSEMBLER=./bin/assembler
LINKER=./bin/linker
EMULATOR=./bin/emulator

${ASSEMBLER} -o out/main.o tests/nivo-b/main.s
${ASSEMBLER} -o out/handler.o tests/nivo-b/handler.s
${ASSEMBLER} -o out/isr_terminal.o tests/nivo-b/isr_terminal.s
${ASSEMBLER} -o out/isr_timer.o tests/nivo-b/isr_timer.s
${LINKER} -hex \
  -place=my_code@0x40000000 \
  -o tests/nivo-b/program.hex \
  out/main.o out/isr_terminal.o out/isr_timer.o out/handler.o
${EMULATOR} tests/nivo-b/program.hex