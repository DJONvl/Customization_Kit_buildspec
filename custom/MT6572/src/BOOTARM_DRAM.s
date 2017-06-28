

	AREA |Init|, CODE, READONLY
|x$codeseg|

DA_LOOP_STACK_BASE   EQU    0x80000000
DA_LOOP_STACK_ADD    EQU    0x200000


    IMPORT DaLoopEntry
	EXPORT StartB
	
    ENTRY
StartB

    MOV		r1, #DA_LOOP_STACK_BASE
	ADD		r1, r1, #DA_LOOP_STACK_ADD
	SUB		r1, r1, #0x04
    MOV		sp, r1
	
    LDR		a4, =DaLoopEntry
    BX		a4
    END
