OUTPUT_FORMAT("elf32-littlearm", "elf32-littlearm", "elf32-littlearm")
OUTPUT_ARCH(arm)

ENTRY(_start)
SECTIONS
{	
	. = DA_DRAM_BASE_ADDRESS;
	__memory_base = .;

	/* text/read-only data */
	.text.boot : { *(.text.boot) }
	.text :	{ *(.text .text.*) } =0x9090

	.rodata : { 
		*(.rodata .rodata.*)
		. = ALIGN(16);
		__rodata_end = . ;
	}

	/* writable data  */
	__data_start_rom = .;	/* in one segment binaries, the rom data address is on top of the ram data address */
	__data_start = .;
	.data : SUBALIGN(16) { *(.data .data.*) }

	__data_end = .;

	/* unintialized data (in same segment as writable data) */
	. = ALIGN(16);
	__bss_start = .;
	.bss : { *(.bss .bss.*) }

	. = ALIGN(16); 
	_end = .;

      . = ALIGN(16); 
	__heap_start = .;

	. = DA_DRAM_BASE_ADDRESS + DA_DRAM_SIZE;     /*like 256M*/
	__heap_end = .;
	_end_of_ram = .;
	__memory_end = .;
}
