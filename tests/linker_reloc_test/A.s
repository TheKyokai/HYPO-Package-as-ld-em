.extern glob_B

.global glob_A

.section .text
loc_A:
    .word 10
glob_A:
    .word loc_A
    .word glob_A
    .word glob_B

.end