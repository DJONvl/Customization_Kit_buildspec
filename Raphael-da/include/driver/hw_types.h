#ifndef __TYPES_H__
#define __TYPES_H__

typedef enum {
   CS_0 = 0
   ,CS_1
   ,CS_2
   ,CS_3
   ,CS_4
   ,CS_5
   ,CS_6
   ,CS_7
   ,CS_WITH_DECODER
   ,MAX_CS = CS_WITH_DECODER
   ,HW_CHIP_SELECT_END
} HW_ChipSelect_E;


typedef enum {
   HW_RAM_UNKNOWN = 0
   ,HW_RAM_SRAM
   ,HW_RAM_DRAM
   ,HW_RAM_TYPE_END
} HW_RAMType_E;

typedef enum {
   HW_MEM_NOR = 0			// NOR Flash
   ,HW_MEM_NAND			// NAND Flash
   ,HW_MEM_EXT_SRAM		// External SRAM
   ,HW_MEM_EXT_DRAM		// External DRAM
   ,HW_MEM_EMMC            // EMMC Flash
   ,HW_MEM_SDMMC           // SDMMC Flash
   ,HW_MEM_TYPE_END
} HW_MemoryType_E;

typedef enum {
   HW_MEM_IO_8BIT = 0		//  8-Bits Memory I/O
   ,HW_MEM_IO_16BIT		// 16-Bits Memory I/O
   ,HW_MEM_IO_32BIT		// 32-Bits Memory I/O
   ,HW_MEM_IO_TYPE_END
} HW_MemoryIO_E;

typedef enum {
   HW_MEM_DUMP = 0		// Memory Dump (Warning: it's not a test scenario!)
   ,HW_MEM_PATTERN_TEST	// Pattern Test Scenario
   ,HW_MEM_INC_DEC_TEST	// Increment/Decrement Test Scenario
   ,HW_MEM_ADDR_BUS_TEST	// EMI Address Bus Test Scenario
   ,HW_MEM_DATA_BUS_TEST	// EMI Data Bus Test Scenario
   ,HW_MEM_IO_BUS_TEST		// NFI I/O Bus Test Scenario 
   ,HW_MEM_TEST_TYPE_END
} HW_MemoryTestMethod_E;

typedef enum {
   HW_STORAGE_NOR = 0
   ,HW_STORAGE_NAND
   ,HW_STORAGE_EMMC
   ,HW_STORAGE_SDMMC
   ,HW_STORAGE_UFS
   ,HW_STORAGE_NONE
   ,HW_STORAGE_TYPE_END
} HW_StorageType_E;


#endif