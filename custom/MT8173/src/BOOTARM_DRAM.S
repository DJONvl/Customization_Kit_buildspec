
.section .text,"x"

.global StartDram
.extern DaLoopEntry
#include "autoconfig.h"
StartDram:

    MOV		r1, #DA_DRAM_BASE_ADDRESS		@defined at MTXXXX.mk
    ADD		r1, r1, #0x200000
    SUB		r1, r1, #0x10
    MOV		sp, r1
	
	BL      DaLoopEntry