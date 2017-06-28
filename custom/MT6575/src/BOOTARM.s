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
;*  BOOTARM_MT6255.S
;*
;* Project:
;* --------
;*  BootRom Library
;*
;* Description:
;* ------------
;*  This Module defines the boot sequence of asm level for MT6225.
;*
;* Author:
;* -------
;*  Amos Hsu
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
	AREA |Init|, CODE, READONLY
|x$codeseg|

MODE_USR	EQU		0X10
MODE_FIQ	EQU		0X11
MODE_IRQ	EQU		0X12
MODE_SVC	EQU		0X13
MODE_ABT	EQU		0X17
MODE_UNDEF	EQU		0X1B
MODE_SYS	EQU		0X1F
I_BIT		EQU		0X80
F_BIT		EQU		0X40

;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
;2010-10-29 Koshi
;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
INT_SRAM_BASE	EQU		0xF0000000
;INT_SRAM_BASE	EQU		0x00020000
INT_SRAM_OFFSET	EQU		0x3000
STACK_PTR		EQU		0x1000

    IMPORT	C_Main
    IMPORT	INT_InitRegions
    IMPORT  ||Image$$INT_SYSRAM$$Base||
    IMPORT  ||Image$$FREE_INT_SYSRAM_BEGIN$$Base||      

    ENTRY
Start	
    B     ResetHandler

;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

ResetHandler
	
    ; Disable interrupt 
    MRS   r0, cpsr 
    MOV   r1, #(I_BIT|F_BIT)
    ORR   r0, r0, r1
    MSR   cpsr_cxsf, r0

    ; Backup DA arguments
    MOV   r11, r4         ; DA argument: BL_Parm_v2 address from BROM or DownloadArg from BL
    MOV   r12, r5         ; DA argument size
	
Relocate                  ; relocate to linking position
    ADR   r0, Start       ; r0: current position of code
    LDR   r1, =Start      ; r1: linking position of code
    CMP   r0, r1
    BEQ   InitCache
    
    LDR   r2, =||Image$$INT_SYSRAM$$Base||
    LDR   r3, =||Image$$FREE_INT_SYSRAM_BEGIN$$Base||
    SUB   r2, r3, r2      ; r2: size of image
    ADD   r2, r0, r2      ; r2: source end address
    
CopyLoop                  ; copy 32 bytes at a time
    LDMIA r0!, {r3 - r10} ; copy from source address [r0]
    STMIA r1!, {r3 - r10} ; copy to   target address [r1]
    CMP   r0, r2          ; until source end addreee [r2]
    BLE   CopyLoop

    MOV   r4, r11         ; Restore DA arguments
    MOV   r5, r12         ; Restore DA arguments

    LDR   r1, =Start      ; r1: linking position of code
    BX    r1              ; jump to relocated address

InitCache
    MRC   p15, 0, r0, c1, c0, 0
    ORR   r0, r0, #0x1800 ; enable Icache + branch prediction */
    MCR   p15, 0, r0, c1, c0, 0

SetupStack	
    ; Set stack pointer to 0x40001400, the stack pool is from 0x40001400~0x40000000 
    MOV		r1, #INT_SRAM_BASE
    ADD		r1, r1, #INT_SRAM_OFFSET
    ADD		r1, r1, #STACK_PTR
    SUB		r1, r1, #0x04
    MOV		sp, r1

    STMFD   sp!, {r11-r12} ; Backup DA argument
    ; INT_InitRegions can initialize all variables
    BL		INT_InitRegions
    LDMIA   sp!, {r0-r1}   ; Restore DA argument

    ; C_Main may be compiled by either ARM code or THUMB code, use BX to jump to C_Main to avoid undef instruction 
    LDR		a4, =C_Main
    BX		a4

    END
