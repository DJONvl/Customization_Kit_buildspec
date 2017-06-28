
.section .text,"x"

.global StartDram
.extern DaLoopEntry

StartDram:

    MOV		r1, #0x40000000
    ADD		r1, r1, #0x200000
    SUB		r1, r1, #0x100
    MOV		sp, r1
	
	BL      DaLoopEntry