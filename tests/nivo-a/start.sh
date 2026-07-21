ASSEMBLER=assembler
LINKER=linker
EMULATOR=emulator

${ASSEMBLER} -o out/main.o tests/nivo-a/main.s
${ASSEMBLER} -o out/math.o tests/nivo-a/math.s
${ASSEMBLER} -o out/handler.o tests/nivo-a/handler.s
${ASSEMBLER} -o out/isr_timer.o tests/nivo-a/isr_timer.s
${ASSEMBLER} -o out/isr_terminal.o tests/nivo-a/isr_terminal.s
${ASSEMBLER} -o out/isr_software.o tests/nivo-a/isr_software.s
${LINKER} -hex \
  -place=my_code@0x40000000 -place=math@0xF0000000 \
  -o program.hex \
  out/handler.o out/math.o out/main.o out/isr_terminal.o out/isr_timer.o out/isr_software.o
${EMULATOR} program.hex