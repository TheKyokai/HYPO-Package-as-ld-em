.extern glob_B

.global glob_A

.section text
    ld $data, %r1
    ld data, %r2
    ld $0x33, %r3
    ld $loc_A, %r4
    ld glob_A, %r5
    ld glob_B, %r6
    ld $glob_A, %r7
    ld $glob_B, %r8

    halt



.section data
loc_A:
    .word 0x10
glob_A:
    .word loc_A
    .word glob_A
    .word glob_B



.end