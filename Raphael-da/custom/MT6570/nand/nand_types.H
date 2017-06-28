/*******************************************************************************
 *  Copyright Statement:
 *  --------------------
 *  This software is protected by Copyright and the information contained
 *  herein is confidential. The software may not be copied and the information
 *  contained herein may not be used or disclosed except with the written
 *  permission of MediaTek Inc. (C) 2003
 *
 *******************************************************************************/

#ifndef NAND_TYPES_H
#define NAND_TYPES_H

#include <string.h>
#include "sys/types.h"
#include "driver/hw_types.h"


#ifndef P_U8
typedef unsigned char*  P_U8;
#endif

#ifndef P_U16
typedef unsigned short* P_U16;
#endif

#ifndef P_U32
typedef unsigned int*  P_U32;
#endif

#ifndef P_U64
typedef unsigned long long*  P_U64;
#endif

//add for NAND
#define NAND_MAX_PARTITION_COUNT 40

typedef struct 
{
   char name[16];
   unsigned int percent;
} SLC_PERCENT;

struct nand_chip
{
    int page_shift;
    unsigned int page_size;
    char ChipID;                /* Type of DiskOnChip */
    char *chips_name;
    unsigned long long chipsize;
    unsigned long erasesize;
    unsigned long mfr;          /* Flash IDs - only one type of flash per device */
    unsigned long id;
    unsigned int  page_per_block;
    char *name;
    int numchips;
    int oobblock;               /* Size of OOB blocks (e.g. 512) */
    int oobsize;                /* Amount of OOB data per block (e.g. 16) */
    int eccsize;
    int bus16;
    int nand_ecc_mode;
    u8  bmt_exist;              /* the existence of BMT, 0: NO; 1: YES*/

	SLC_PERCENT		g_SLC_percent[4];
	uint32			g_slc_percent_count;
};


//-----------------------------------------------------------------------nand-----------------------------------------------
typedef enum {
   ID_COMMON_CB_FUNC_SET = 0
   ,ID_COMMON_CB_FUNC_SET_WITHOUT_COPYBACK
   ,ID_ST_CB_FUNC_SET
   ,ID_ST_CB_FUNC_SET_WITHOUT_COPYBACK
   ,ID_superAND_CB_FUNC_SET

   ,ID_NAND_CALLBACK_UNKNOWN = 0xFFFF
} NAND_CMD_Callback_ID_E;


// NAND flash device id 
typedef enum {
   NAND_K9F5608Q0C = 0
   ,NAND_K9F5608X0C
   ,NAND_K9F5616Q0C
   ,NAND_K9F5616X0C
   ,NAND_K9K1208X0C
   ,NAND_K9K1G08X0A
   ,NAND_K9F1G08Q0M
   ,NAND_K9F1G08X0M
   ,NAND_K9F1G16Q0M
   ,NAND_K9F1G16X0M
   ,NAND_K9F2G08X0M
   ,NAND_K9F2G16X0M
   ,NAND_K9F2G08R0A
   ,NAND_K9K4G08Q0M
   ,NAND_K9K4G08U0M
   ,NAND_K9K4G16Q0M
   ,NAND_K9K4G16U0M
   ,NAND_KTN0403CS_TCR1
   ,NAND_KSLCGBL2GA2H2A
   ,NAND_S34ML04G200TFI00
   ,NAND_KSLCBBL1FB4G3A
   ,NAND_TC58DVM82A1FT
   ,NAND_TC58DVM92A1FT
   ,NAND_TC58DVG02A1FT
   ,NAND_TC58NVG0S3AFT
   ,NAND_TH58NVG1S3AFT
   ,NAND_HY27XS08561M
   ,NAND_HY27XS08121M
   ,NAND_HY27XA081G1M
   ,NAND_HY27XA161G1M
   ,NAND_ST128W3A
   ,NAND_ST128W4A
   ,NAND_ST256W3A
   ,NAND_ST256W4A
   ,NAND_ST512W3A
   ,NAND_ST512W4A
   ,NAND_ST01GW3A
   ,NAND_ST01GW4A
   ,NAND_ST512R3B
   ,NAND_ST512W3B
   ,NAND_ST512R4B
   ,NAND_ST512W4B
   ,NAND_ST01GR3B
   ,NAND_ST01GW3B
   ,NAND_ST01GR4B
   ,NAND_ST01GW4B
   ,NAND_ST02GR3B
   ,NAND_ST02GW3B
   ,NAND_ST02GR4B
   ,NAND_ST02GW4B
   ,NAND_ST04GR3B
   ,NAND_ST04GW3B
   ,NAND_ST04GR4B
   ,NAND_ST04GW4B
   ,NAND_ST08GR3B
   ,NAND_ST08GW3B
   ,NAND_ST08GR4B
   ,NAND_ST08GW4B
   ,NAND_K9F2808U0C
   ,NAND_K9K8G08U0M
   ,NAND_TH58NVG2D4BFT
   ,NAND_HY27UF081G2M
   ,NAND_TH58NVG1int8BFT
   ,NAND_MT29F2G08AAC
   ,NAND_MT29F2G08ABD
   ,NAND_MT29F4G08BAB
   ,NAND_MT29F4G16ABD
   ,NAND_MT29F64G08CBABB
   ,NAND_MT29F32G08CBADB
   ,NAND_MT29F128G08CFAAA
   ,NAND_SDTNRGAME_008G
   ,NAND_SDTNRGAMA_008G
   ,NAND_TC58TEG5DCJTA00
   ,NAND_SDTNRGAMA_008GK
   ,NAND_SDTNRFAMA_004GK
   ,NAND_SDTNQGAMA_008G
   ,NAND_TC58TEG5DCKTA00
   ,NAND_TC58TEG6DDKTA00
   ,NAND_H27UCG8T2ATR
   ,NAND_SUPERAND51208
   ,NAND_SUPERAND51216
   ,NAND_HYF33DS51280
   ,NAND_HYF33DS1G80
   ,NAND_HYF33DS1G16
   ,NAND_NCSPN4N2A
   ,NAND_TC58NWM9S3B
   ,NAND_HY27UF082G2M
   ,NAND_M6MGA157F2LCWG08
   ,NAND_M6MGA157F2LCWG16
   ,NAND_TC58NWM9int8C
   ,NAND_TC58NWG0int8C
   ,NAND_HY27US16561M
   ,NAND_HY27UF162G5A
   ,NAND_HY27UT088G2A
   ,NAND_TC58NVG4D1DTG
   ,NAND_TY58NYG3S5F
   ,NAND_K5E1257ACM
   ,NAND_K5E1G12ACF
   ,NAND_MT29F2G16ABD
   ,NAND_HYD0SFG0MF1P
   ,NAND_HYD0SQG0MF1P
   ,NAND_H9DA4VH4JJAMCR
   ,NAND_H9DA4GH4JJAMCR
   ,NAND_H27UBG8T2CTR
   ,NAND_H27UCG8T2ETR
   ,NAND_H27UCG4T2ETR
   ,NAND_H8BCS0CG0MBR
   ,NAND_H8BCS0PG0MBP
   ,NAND_TC58NYG0int8C
   ,NAND_TC58NYG1int8C
   ,NAND_ST512R3A
   ,NAND_ST512R4A
   ,NAND_MT29F1G08ABB
   ,NAND_MT29F1G16ABB
   ,NAND_ASU1GA30GT_G30CA
   ,NAND_K522H1GACE
   ,NAND_H8BCS0SI0MBR
   ,NAND_MT29F4G08ABC
   ,NAND_MT29C4G48MAZAPAKD
   ,NAND_H8BCS0UN0MCR
   ,NAND_TYBC0A111236KC10
   ,NAND_TOSHIBA_mLBA
   ,NAND_H9DA4GH4JJAMCR_4EM
   ,NAND_H9DA4VH2GJAMCR_4EM
   ,NAND_KA100O015B_BJTT
   ,NAND_KA100O015E_BJTT
   ,NAND_KF94G16Q4V
   ,NAND_KF94G16Q4X
   ,NAND_MT29C4G96MAZAPCJA_5IT
   ,NAND_MT29C4G48MAAHBAAKS_5WT
   ,NAND_MT29C8G96MAZBADKD_5WT
   ,NAND_KF98G16Q4X
   ,NAND_TY58NYG2int8E
   ,NAND_K9K8G16U0K
   ,NAND_TYBC0A111557KC
   ,NAND_TH58NVG7DDJTA20
   ,NAND_H27uint8G8T2B
   ,NAND_H27UAG8T2BTR
   ,NAND_ST08GW3C
   ,NAND_TC58NVG6D2GTA00
   ,NAND_H27UBG8T2B
   ,NAND_K9GBG08U0A
   ,NAND_H27UAG8T2A
   ,NAND_MT29F8G08ABA
   ,NAND_H27U4G8F2DTR
   ,NAND_MT29F1G08ABA
   ,NAND_H27US08561A
   ,NAND_H9TA4GH2GDMCPR
   ,NAND_H9DA4VH2GJMMCR_4EM
   ,NAND_A5R4GA41ATS
   ,NAND_KEM51200_NADGSBXM2C32
   ,NAND_F592G81A//bayi
   //------ SPI-NAND begin ------
   ,SNAND_GD5F4GQ4UAYIG
   //------ SPI-NAND end ------
   //------ TLC-NAND begin ------
   ,TNAND_TC58TEG6TCKTA00
   ,TNAND_SDTNRIAMA008GK
   ,TNAND_SDTNRCAMA008GK
   ,TNAND_H27UDG8M2MTR
   ,TNAND_TC58TEG6TGLTA00
   //------ TLC-NAND end ------
   ,NAND_LAST
   ,SNAND_LAST = NAND_LAST
   ,NAND_UNKNOWN = 0xFFFF		    // Unknown Device
   ,SNAND_UNKNOWN = NAND_UNKNOWN
} NAND_DeviceID_E;

#if 0
typedef enum {
   NUTL_ERASE = 0
   ,NUTL_FORCE_ERASE
   ,NUTL_MARK_BAD_BLOCK
   ,NUTL_ERASE_FLAG_END
} NUTL_EraseFlag_E;
#endif

typedef enum {
   NUTL_PROGRAM_PAGE_SPARE = 0
   ,NUTL_PROGRAM_PAGE_ONLY
   ,NUTL_PROGRAM_SPARE_ONLY
   ,NUTL_PROGRAM_PAGE_FDM
   ,NUTL_PROGRAM_PAGE_SPARE_WITH_ECCENCODE
   ,NUTL_PROGRAM_FLAG_END
} NUTL_ProgramFlag_E;

typedef enum {
   NUTL_ADDR_LOGICAL= 0
   ,NUTL_ADDR_PHYSICAL
   ,NUTL_ADDR_FLAG_END
} NUTL_AddrTypeFlag_E;

typedef enum {
   NUTL_READ_PAGE_SPARE = 0
   ,NUTL_READ_PAGE_ONLY
   ,NUTL_READ_SPARE_ONLY
   ,NUTL_READ_PAGE_WITH_ECC
   ,NUTL_READ_PAGE_SPARE_WITH_ECCDECODE
   ,NUTL_VERIFY_AFTER_PROGRAM
   ,NUTL_READ_PAGE_SPARE_NORANDOM
   ,NUTL_READ_FLAG_END
} NUTL_ReadFlag_E;
//-----------------------------------------------------------nand------------------------------------------------------------



#endif  /* NAND_TYPES_H */


