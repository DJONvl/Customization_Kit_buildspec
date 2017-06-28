;*****************************************************************************
;*  Copyright Statement:
;*  --------------------
;*  This software is protected by Copyright and the information contained
;*  herein is confidential. The software may not be copied and the information
;*  contained herein may not be used or disclosed except with the written
;*  permission of MediaTek Inc. (C) 2006
;*
;*  BY OPENING THIS FILE, BUYER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
;*  THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
;*  RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO BUYER ON
;*  AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
;*  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
;*  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
;*  NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
;*  SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
;*  SUPPLIED WITH THE MEDIATEK SOFTWARE, AND BUYER AGREES TO LOOK ONLY TO SUCH
;*  THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. MEDIATEK SHALL ALSO
;*  NOT BE RESPONSIBLE FOR ANY MEDIATEK SOFTWARE RELEASES MADE TO BUYER'S
;*  SPECIFICATION OR TO CONFORM TO A PARTICULAR STANDARD OR OPEN FORUM.
;*
;*  BUYER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND CUMULATIVE
;*  LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
;*  AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE,
;*  OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY BUYER TO
;*  MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
;*
;*  THE TRANSACTION CONTEMPLATED HEREUNDER SHALL BE CONSTRUED IN ACCORDANCE
;*  WITH THE LAWS OF THE STATE OF CALIFORNIA, USA, EXCLUDING ITS CONFLICT OF
;*  LAWS PRINCIPLES.  ANY DISPUTES, CONTROVERSIES OR CLAIMS ARISING THEREOF AND
;*  RELATED THERETO SHALL BE SETTLED BY ARBITRATION IN SAN FRANCISCO, CA, UNDER
;*  THE RULES OF THE INTERNATIONAL CHAMBER OF COMMERCE (ICC).
;*
;******************************************************************************/
;
;*******************************************************************************
;* Filename:
;* ---------
;*  CACHE_CA7.S
;*
;* Project:
;* --------
;*  BootRom Library
;*
;* Description:
;* ------------
;*  ARM co-processor relevant operations
;*
;* Author:
;* -------
;*  Koshi Chiu
;*
;*==============================================================================
;*           HISTORY
;* Below this line, this part is controlled by PVCS VM. DO NOT MODIFY!!
;*------------------------------------------------------------------------------
;* $Revision$
;* $Modtime$
;* $Log$
;*
;*------------------------------------------------------------------------------
;* Upper this line, this part is controlled by PVCS VM. DO NOT MODIFY!!
;*==============================================================================
;*******************************************************************************/

    PRESERVE8
    AREA |ARM_CP15|, CODE, READONLY
    CODE32

;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
; DATA SEGMENT

C1_M_BIT        EQU     &00000001   ; MMU (0:disable, 1:enable)
C1_A_BIT        EQU     &00000002   ; Alignment fault check (0:disable, 1:enable)
C1_C_BIT        EQU     &00000004   ; D-cache (0:disable, 1:enable)
C1_R_BIT        EQU     &00000200   ; ROM protection (0:disable, 1:enable)
C1_Z_BIT        EQU     &00000800   ; Branch prediction (0:disable, 1:enable)
C1_I_BIT        EQU     &00001000   ; I-cache (0:disable, 1:enable)
C1_U_BIT        EQU     &00400000   ; Unaligned data access (0:disable, 1:enable)
C1_XP_BIT       EQU     &00800000   ; Enable ARMv6 MMU pagetable (0:disable, 1:enable)

;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
; IMPORT definition 

;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
; CODE SEGMENT 

;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
; ARM11 Cache/MMU
;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
    
;------------------------------------------------------------------------------
    EXPORT  arm_cp15_enable_cache
arm_cp15_enable_cache
    MRC p15,0,r0,c1,c0,0
    ORR r0,r0,#C1_I_BIT
    ORR r0,r0,#C1_C_BIT
    MCR p15,0,r0,c1,c0,0
    BX lr

    EXPORT  arm_cp15_disable_cache
arm_cp15_disable_cache    
    MRC p15,0,r0,c1,c0,0
    ORR r0,r0,#C1_I_BIT
    ORR r0,r0,#C1_C_BIT
    MCR p15,0,r0,c1,c0,0
    BX lr
    
    EXPORT  arm_cp15_tlb_invalidate_all
arm_cp15_tlb_invalidate_all
    ;   Uses unified TLB, so we do not have to specify the I/D/Unified TLB.
    ;      Their results are the same.
    MCR p15,0,r0,c8,c3,0
    DSB
    ISB
    BX lr
    
;------------------------------------------------------------------------------
    EXPORT  arm_cp15_icache_ctrl
arm_cp15_icache_ctrl
    MRC     p15, 0, a4, c1, c0, 0
    CMPS    a1, #0
    ORRNE   a4, a4, #C1_I_BIT               ; 1: Enable I-Cache
    BICEQ   a4, a4, #C1_I_BIT               ; 0: Disable I-Cache
    MCR     p15, 0, a4, c1, c0, 0
    BX      lr                              ; Return to caller

;------------------------------------------------------------------------------
    EXPORT  arm_cp15_icache_is_enabled
arm_cp15_icache_is_enabled
    MRC     p15, 0, a4, c1, c0, 0
    ANDS    a4, a4, #C1_I_BIT               ; check if I-Cache is enabled 
    MOVEQ   a1, #0
    MOVNE   a1, #1
    BX      lr                              ; Return to caller

;------------------------------------------------------------------------------
; ======================================================
; 20100925 : copied from SS2's MT6575 FPGA MCU function
; ======================================================

    EXPORT  arm_cp15_icache_invalidate
arm_cp15_icache_invalidate
    MOV     a4, #0
    MCR     p15, 0, a4, c7, c1, 0           ; Invalidate I-Cache
    BX      lr                              ; Return to caller

;------------------------------------------------------------------------------
    EXPORT  arm_cp15_dcache_ctrl
arm_cp15_dcache_ctrl
    MRC     p15, 0, a4, c1, c0, 0
    CMPS    a1, #0
    ORRNE   a4, a4, #C1_C_BIT               ; 1: Enable D-Cache
    BICEQ   a4, a4, #C1_C_BIT               ; 0: Disable D-Cache
    MCR     p15, 0, a4, c1, c0, 0
    BX      lr                              ; Return to caller

;------------------------------------------------------------------------------
; ======================================================
; 20100925 : copied from SS2's MT6575 FPGA MCU function
; ======================================================
    EXPORT  arm_cp15_dcache_invalidate
arm_cp15_dcache_invalidate    
    push    {r4,r5,r7,r9,r10,r11}
    dmb                                     ; ensure ordering with previous memory accesses
    mrc     p15, 1, r0, c0, c0, 1           ; read clidr
    ands    r3, r0, #0x7000000              ; extract loc from clidr
    mov     r3, r3, lsr #23                 ; left align loc bit field
    beq     finished                        ; if loc is 0, then no need to clean
    mov     r10, #0                         ; start clean at cache level 0
loop1
    add     r2, r10, r10, lsr #1            ; work out 3x current cache level
    mov     r1, r0, lsr r2                  ; extract cache type bits from clidr
    and     r1, r1, #7                      ; mask of the bits for current cache only
    cmp     r1, #2                          ; see what cache we have at this level
    blt     skip                            ; skip if no cache, or just i-cache
    mcr     p15, 2, r10, c0, c0, 0          ; select current cache level in cssr
    isb                                     ; isb to sych the new cssr&csidr
    mrc     p15, 1, r1, c0, c0, 0           ; read the new csidr
    and     r2, r1, #7                      ; extract the length of the cache lines
    add     r2, r2, #4                      ; add 4 (line length offset)
    ldr     r4, =0x3ff
    ands    r4, r4, r1, lsr #3              ; find maximum number on the way size
    clz     r5, r4                          ; find bit position of way size increment
    ldr     r7, =0x7fff
    ands    r7, r7, r1, lsr #13             ; extract max number of the index size
loop2
    mov     r9, r4                          ; create working copy of max way size
loop3
    orr     r11, r10, r9, lsl r5            ; factor way and cache number into r11
    orr     r11, r11, r7, lsl r2            ; factor index number into r11
    mcr     p15, 0,  r11, c7, c6, 2         ; invalidate by set/way    
    subs    r9, r9, #1                      ; decrement the way
    bge     loop3                           ;
    subs    r7, r7, #1                      ; decrement the index
    bge     loop2
skip
    add     r10, r10, #2                    ; increment cache number
    cmp     r3, r10
    bgt     loop1
finished
    mov     r10, #0                         ; swith back to cache level 0
    mcr     p15, 2, r10, c0, c0, 0          ; select current cache level in cssr
    dsb
    isb
    pop     {r4,r5,r7,r9,r10,r11}
    bx      lr

;------------------------------------------------------------------------------
; ======================================================
; 20100925 : copied from SS2's MT6575 FPGA MCU function
; ======================================================
    EXPORT  arm_cp15_dcache_clean
arm_cp15_dcache_clean
    push    {r4,r5,r7,r9,r10,r11}
    dmb                                     ; ensure ordering with previous memory accesses
    mrc     p15, 1, r0, c0, c0, 1           ; read clidr
    ands    r3, r0, #0x7000000              ; extract loc from clidr
    mov     r3, r3, lsr #23                 ; left align loc bit field
    beq     c_finished                      ; if loc is 0, then no need to clean
    mov     r10, #0
c_loop1
    add     r2, r10, r10, lsr #1            ; work out 3x current cache level
    mov     r1, r0, lsr r2                  ; extract cache type bits from clidr
    and     r1, r1, #7                      ; mask of the bits for current cache only
    cmp     r1, #2                          ; see what cache we have at this level
    blt     c_skip                          ; skip if no cache, or just i-cache
    mcr     p15, 2, r10, c0, c0, 0          ; select current cache level in cssr
    isb                                     ; isb to sych the new cssr&csidr
    mrc     p15, 1, r1, c0, c0, 0           ; read the new csidr
    and     r2, r1, #7                      ; extract the length of the cache lines
    add     r2, r2, #4                      ; add 4 (line length offset)
    ldr     r4, =0x3ff
    ands    r4, r4, r1, lsr #3              ; find maximum number on the way size
    clz     r5, r4                          ; find bit position of way size increment
    ldr     r7, =0x7fff
    ands    r7, r7, r1, lsr #13             ; extract max number of the index size
c_loop2
    mov     r9, r4                          ; create working copy of max way size
c_loop3
    orr     r11, r10, r9, lsl r5            ; factor way and cache number into r11
    orr     r11, r11, r7, lsl r2            ; factor index number into r11
    mcr     p15, 0, r11, c7, c10, 2         ; clean by set/way
    subs    r9, r9, #1                      ; decrement the way
    bge     c_loop3
    subs    r7, r7, #1                      ; decrement the index
    bge     c_loop2
c_skip
    add     r10, r10, #2                    ; increment cache number
    cmp     r3, r10
    bgt     c_loop1
c_finished
    mov     r10, #0                         ; swith back to cache level 0
    mcr     p15, 2, r10, c0, c0, 0          ; select current cache level in cssr
    dsb
    isb
    pop     {r4,r5,r7,r9,r10,r11}
    bx      lr

;------------------------------------------------------------------------------
; ======================================================
; 20100925 : copied from SS2's MT6575 FPGA MCU function
; ======================================================
    EXPORT  arm_cp15_dcache_clean_invalidate
arm_cp15_dcache_clean_invalidate
    push    {r4,r5,r7,r9,r10,r11}
    dmb                                     ; ensure ordering with previous memory accesses
    mrc     p15, 1, r0, c0, c0, 1           ; read clidr
    ands    r3, r0, #0x7000000              ; extract loc from clidr
    mov     r3, r3, lsr #23                 ; left align loc bit field
    beq     ci_finished                     ; if loc is 0, then no need to clean
    mov     r10, #0
ci_loop1
    add     r2, r10, r10, lsr #1            ; work out 3x current cache level
    mov     r1, r0, lsr r2                  ; extract cache type bits from clidr
    and     r1, r1, #7                      ; mask of the bits for current cache only
    cmp     r1, #2                          ; see what cache we have at this level
    blt     ci_skip                         ;  @ skip if no cache, or just i-cache
    mcr     p15, 2, r10, c0, c0, 0          ; select current cache level in cssr
    isb                                     ; isb to sych the new cssr&csidr
    mrc     p15, 1, r1, c0, c0, 0           ; read the new csidr
    and     r2, r1, #7                      ; extract the length of the cache lines
    add     r2, r2, #4                      ; add 4 (line length offset)
    ldr     r4, =0x3ff
    ands    r4, r4, r1, lsr #3              ; find maximum number on the way size
    clz     r5, r4                          ; find bit position of way size increment
    ldr     r7, =0x7fff
    ands    r7, r7, r1, lsr #13             ; extract max number of the index size
ci_loop2
    mov     r9, r4                          ; create working copy of max way size
ci_loop3
    orr     r11, r10, r9, lsl r5            ; factor way and cache number into r11
    orr     r11, r11, r7, lsl r2            ; factor index number into r11
    mcr     p15, 0, r11, c7, c14, 2         ; clean & invalidate by set/way
    subs    r9, r9, #1                      ; decrement the way
    bge     ci_loop3
    subs    r7, r7, #1                      ; decrement the index
    bge     ci_loop2
ci_skip
    add     r10, r10, #2                    ; increment cache number
    cmp     r3, r10
    bgt     ci_loop1
ci_finished
    mov     r10, #0                         ; swith back to cache level 0
    mcr     p15, 2, r10, c0, c0, 0          ; select current cache level in cssr
    dsb
    isb
    pop     {r4,r5,r7,r9,r10,r11}
    bx      lr
    
;------------------------------------------------------------------------------
    EXPORT  arm_cp15_DSB
arm_cp15_DSB
    ;MOV     a4, #0
    ;MCR     p15, 0, a4, c7, c10, 4          ; Flush write buffer
    DSB
    BX      lr                              ; Return to caller

;------------------------------------------------------------------------------
    EXPORT  arm_cp15_ISB
arm_cp15_ISB
    ;MOV     a4, #0
    ;MCR     p15, 0, a4, c7, c5, 4           ; Flush write buffer
    ISB
    BX      lr                              ; Return to caller

;------------------------------------------------------------------------------
    EXPORT  arm_cp15_mmu_ctrl
arm_cp15_mmu_ctrl
    MOV     a4, #0
    MCR     p15, 0, a4, c8, c6, 0           ; Invalidate TLB
    MRC     p15, 0, a4, c1, c0, 0
    CMPS    a1, #0
    ORRNE   a4, a4, #C1_M_BIT               ; 1: Enable MMU
    BICEQ   a4, a4, #C1_M_BIT               ; 0: Disable MMU
    MCR     p15, 0, a4, c1, c0, 0
    NOP 
    NOP 
    NOP 
    NOP 
    NOP 
    NOP 
    NOP 
    NOP 
    BX      lr                              ; Return to caller

;------------------------------------------------------------------------------
    EXPORT  arm_cp15_mmu_ttbr0_set
arm_cp15_mmu_ttbr0_set
    MCR     p15, 0, a1, c2, c0, 0           ; Set page table 0
    BX      lr                              ; Return to caller

;------------------------------------------------------------------------------
    EXPORT  arm_cp15_mmu_domain_access_set
arm_cp15_mmu_domain_access_set
    MOV     a1, a1, LSL #1                  ; Each domainAccess contains 2bits
    MOV     a3, #3                          ; a3 is the mask
    MOV     a3, a3, LSL a1                  ; Adjust mask to correct position
    MOV     a2, a2, LSL a1                  ; Adjust domainAccess to correct position
;    MRC     p15, 0, a4, c3, c0, 0
    MOV     a4, #0                          ; since unknown value will be read from CP15, set the value instead of reading from CP15
    BIC     a4, a4, a3
    ORR     a4, a4, a2
    MCR     p15, 0, a4, c3, c0, 0
    BX      lr                              ; Return to caller

;------------------------------------------------------------------------------
    EXPORT  arm_cp15_rom_protection_ctrl
arm_cp15_rom_protection_ctrl
    MRC     p15, 0, a4, c1, c0, 0
    CMPS    a1, #0
    ORRNE   a4, a4, #C1_R_BIT               ; 1: Enable ROM protection 
    BICEQ   a4, a4, #C1_R_BIT               ; 0: Disable ROM protection 
    MCR     p15, 0, a4, c1, c0, 0
    BX      lr                              ; Return to caller

;------------------------------------------------------------------------------
    EXPORT  arm_cp15_branch_prediction_ctrl
arm_cp15_branch_prediction_ctrl
    MOV     a4, #0
    MCR     p15, 0, a4, c7, c5, 6           ; Flush entire branch target cache 
    MRC     p15, 0, a4, c1, c0, 0
    CMPS    a1, #0
    ORRNE   a4, a4, #C1_Z_BIT               ; 1: Enable branch prediction
    BICEQ   a4, a4, #C1_Z_BIT               ; 0: Disable branch prediction
    MCR     p15, 0, a4, c1, c0, 0           ; Enable branch prediction 
    BX      lr 

;------------------------------------------------------------------------------
    EXPORT  arm_cp15_unalignment_access_ctrl
arm_cp15_unalignment_access_ctrl
    MRC     p15, 0, a4, c1, c0, 0 
    CMPS    a1, #0
    ORRNE   a4, a4, #C1_U_BIT               ; 1: Enable un-aligned address access
    BICEQ   a4, a4, #C1_U_BIT               ; 0: Disable un-aligned address access
    MCR     p15, 0, a4, c1, c0, 0 
    BX      lr                              ; Return to caller

;------------------------------------------------------------------------------
    EXPORT  arm_cp15_alignment_fault_ctrl
arm_cp15_alignment_fault_ctrl
    MRC     p15, 0, a4, c1, c0, 0 
    CMPS    a1, #0
    ORRNE   a4, a4, #C1_A_BIT               ; 1: Enable alignment fault check 
    BICEQ   a4, a4, #C1_A_BIT               ; 0: Disable alignment fault check 
    MCR     p15, 0, a4, c1, c0, 0 
    BX      lr                              ; Return to caller

;------------------------------------------------------------------------------
    EXPORT  arm_cp15_arm_v6_pagetable_ctrl
arm_cp15_arm_v6_pagetable_ctrl
    MRC     p15, 0, a4, c1, c0, 0 
    CMPS    a1, #0
    ORRNE   a4, a4, #C1_XP_BIT              ; 1: Enable ARMv6 MMU (Main Core subpage disabled) 
    BICEQ   a4, a4, #C1_XP_BIT              ; 0: Disable ARMv6 MMU (Main Core subpage enabled, ARMv5 MMU compatible) 
    MCR     p15, 0, a4, c1, c0, 0 
    BX      lr                              ; Return to caller
;------------------------------------------------------------------------------
    EXPORT  arm_cp15_enable_smp
arm_cp15_enable_smp
    MRC     p15,0,r0,c1,c0,1
    ORR     r0, r0, #0x040
    MCR     p15,0,r0,c1,c0,1
    BX      lr                              ; Return to caller
;------------------------------------------------------------------------------
    EXPORT  arm_cp15_disable_smp
arm_cp15_disable_smp
    MRC     p15,0,r0,c1,c0,1
    BIC     r0, r0, #0x040
    MCR     p15,0,r0,c1,c0,1
    BX      lr                              ; Return to caller

    END