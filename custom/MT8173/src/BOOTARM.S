
.section .text,"x"

.global Start
.extern C_Main
.extern __text_start
.extern __bss_start

#include "autoconfig.h"
Start:	
    B     ResetHandler

ResetHandler:
	
    @ Disable interrupt 
    MRS   r0, cpsr 
    MOV   r1, #0xC0
    ORR   r0, r0, r1
    MSR   cpsr_cxsf, r0

    @ Backup DA arguments
    MOV   r11, r4
    MOV   r12, r5

Relocate:                 @ relocate to linking position
    ADR   r0, Start       @ r0: current position of code
    LDR   r1, =Start      @ r1: linking position of code
    CMP   r0, r1
    BEQ   InitCache
    
    LDR   r2, =__text_start
    LDR   r3, =__bss_start
    SUB   r2, r3, r2      @ r2: size of image
    ADD   r2, r0, r2      @ r2: source end address
    
CopyLoop:                 @ copy 32 bytes at a time
    LDMIA r0!, {r3 - r10} @ copy from source address [r0]
    STMIA r1!, {r3 - r10} @ copy to   target address [r1]
    CMP   r0, r2          @ until source end addreee [r2]
    BLE   CopyLoop

    MOV   r4, r11         @ Restore DA arguments
    MOV   r5, r12         @ Restore DA arguments

    LDR   r1, =Start      @ r1: linking position of code
    BX    r1              @ jump to relocated address

InitCache:
    MRC   p15, 0, r0, c1, c0, 0
    ORR   r0, r0, #0x1800
    MCR   p15, 0, r0, c1, c0, 0

@Enable access to CP10 and CP11
	MRC   p15, 0, r1, c1, c0, 2
	ORR   r1, r1, #(0xf<<20)
	MCR   p15, 0, r1, c1, c0, 2
	MOV   r1, #0
	ISB
@Enable the VFP
	MOV   r0, #0x40000000
	VMSR  FPEXC, r0

SetupStack:	
    @ Set stack pointer to 0x40001400, the stack pool is from 0x40001400~0x40000000 
	MOV		r1, #DA_INIT_STACK_ADDRESS		@defined at MTXXXX.mk
    SUB		r1, r1, #0x10
    MOV		sp, r1
	
    STMFD   sp!, {r11-r12}
    @ INT_InitRegions can initialize all variables
    @BL		INT_InitRegions
    LDMIA   sp!, {r0-r1}

	BL C_Main
