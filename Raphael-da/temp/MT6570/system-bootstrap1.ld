OUTPUT_ARCH(arm)
MEMORY {
	text (rwx) : ORIGIN = 0x200000 ,LENGTH = 0x20000
}
ENTRY(_start)
SECTIONS
{
	. = 0x200000;
	__text_start = .;
    .all ALIGN(16) : {*(.text .data .rodata)} > text 
	.section_end ALIGN(16) : {*(.section_end)}
	. = ALIGN(16);
	__bss_start = .;
	.bss ALIGN(16) : {*(.bss)} 			> text
	.bss_static ALIGN(16) : {.bss.*}		
	. = ALIGN(16);
	__bss_end = .;	

	
	/* heap use shared ram*/
	. = 0x100000;
	__heap_start = .;   /*0x200000;*/
	. = 0x100000 + 0x10000;
	__heap_end = .;      /*0x240000*/
	. = .;
}
