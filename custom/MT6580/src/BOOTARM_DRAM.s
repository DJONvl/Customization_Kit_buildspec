
.section .text,"x"

.global StartDram
.extern DaLoopEntry

StartDram:

    MOV		r1, #0x80000000
    ADD		r1, r1, #0x200000
    SUB		r1, r1, #0x10
    MOV		sp, r1
    BL      DaLoopEntry