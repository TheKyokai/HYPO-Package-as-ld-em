.extern glob_A

.global glob_B

.section .text
    .word 0xFFFFFFFF
loc_B:
glob_B:
    .word loc_B
    .word glob_A
    .word glob_B

.end