OUTPUT_ARCH(arm)
MEMORY {
	text (rwx) : ORIGIN = SRAM_START ,LENGTH = SRAM_LEN
}
ENTRY(_start)
SECTIONS
{
	. = SRAM_START;
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
	. = HEAP_BASE_INIT;
	__heap_start = .;   /*0x200000;*/
	. = HEAP_BASE_INIT + HEAP_SIZE_INIT;
	__heap_end = .;      /*0x240000*/
	. = .;
}
