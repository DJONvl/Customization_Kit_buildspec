
.section .text,"x"

.global Start
.extern C_Main
.extern __text_start
.extern __bss_start

Start:	
    B     ResetHandler

CloseCPU1_7:
    @key 
    mov r0, #0x0B100000
    add r0, r0, #0x60000
    add r0, r0, #0x1
    
    mov r2, #0x10000000
    add r2, r2, #0x6000
    str r0, [r2]  

    mov r2, #0x10000000
    add r2, r2, #0x6200

    @cluster 1 L1 cache
    mov r0, #0xF0
    mov r1, #0xFF
    add r3, r2, #0xB4
    str r0, [r3]
    str r1, [r3]  
    
    @val
    mov r0, #0x7F
    mov r1, #0x32
    mov r4, #0x100
    add r4, r4, #0x1
    
    @cpu7
    add r3, r2, #0xAC
    str r0, [r3]
    str r1, [r3]   
   

    @cpu6
    add r3, r2, #0xA8
    str r0, [r3]
    str r1, [r3]

    @cpu5
    add r3, r2, #0xA4 
    str r0, [r3]
    str r1, [r3]

    @cpu4
    add r3, r2, #0xA0 
    str r0, [r3]
    str r1, [r3]

    @cluster1
    add r3, r2, #0xB0    
    str r1, [r3]

    @cpu3
    add r5, r2, #0x20  
    add r6, r2, #0x74 
    
    
    mov r7, #0x4F
    str r7, [r5]    
    str r1, [r5]
    str r4, [r6]   

    @cpu2
    add r5, r2, #0x1C 
    add r6, r2, #0x6C 
    
    mov r7, #0x4F
    str r7, [r5]    
    str r1, [r5]
    str r4, [r6] 
 
    @cpu1
    add r5, r2, #0x18  
    add r6, r2, #0x64 
    
    mov r7, #0x4F
    str r7, [r5]    
    str r1, [r5]
    str r4, [r6] 
    
    BX 		lr

ResetHandler:
	@BL CloseCPU1_7
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
    MOV		r1,     #0x130000
    SUB		r1, r1, #0x04
    MOV		sp, r1
	
    STMFD   sp!, {r11-r12}
    @ INT_InitRegions can initialize all variables
    @BL		INT_InitRegions
    LDMIA   sp!, {r0-r1}

	BL C_Main
