/********************************************************************************************
 *     LEGAL DISCLAIMER
 *
 *     (Header of MediaTek Software/Firmware Release or Documentation)
 *
 *     BY OPENING OR USING THIS FILE, BUYER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
 *     THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE") RECEIVED
 *     FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO BUYER ON AN "AS-IS" BASIS
 *     ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES, EXPRESS OR IMPLIED,
 *     INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR
 *     A PARTICULAR PURPOSE OR NONINFRINGEMENT. NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY
 *     WHATSOEVER WITH RESPECT TO THE SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY,
 *     INCORPORATED IN, OR SUPPLIED WITH THE MEDIATEK SOFTWARE, AND BUYER AGREES TO LOOK
 *     ONLY TO SUCH THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. MEDIATEK SHALL ALSO
 *     NOT BE RESPONSIBLE FOR ANY MEDIATEK SOFTWARE RELEASES MADE TO BUYER'S SPECIFICATION
 *     OR TO CONFORM TO A PARTICULAR STANDARD OR OPEN FORUM.
 *
 *     BUYER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND CUMULATIVE LIABILITY WITH
 *     RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE, AT MEDIATEK'S OPTION,
 *     TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE, OR REFUND ANY SOFTWARE LICENSE
 *     FEES OR SERVICE CHARGE PAID BY BUYER TO MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
 *
 *     THE TRANSACTION CONTEMPLATED HEREUNDER SHALL BE CONSTRUED IN ACCORDANCE WITH THE LAWS
 *     OF THE STATE OF CALIFORNIA, USA, EXCLUDING ITS CONFLICT OF LAWS PRINCIPLES.
 ************************************************************************************************/

/*-----------------------------------------------------------------------------
 *
 * $Author: qingqing.li $
 * $Date: 2014/03/25 $
 * $RCSfile: serialflash_hw.h,v $
 * $Revision: #5 $
 *
 *---------------------------------------------------------------------------*/

#ifndef SERIAL_FLASH_HW_H
#define SERIAL_FLASH_HW_H

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
#include "flash_dev_tbl.h"

#define SFLASH_WREG8(offset, value)       ((*(volatile unsigned int *)(SFLASH_REG_BASE + offset)) = (value & 0xFF))
#define SFLASH_RREG8(offset)              ((*(volatile unsigned int *)(SFLASH_REG_BASE + offset)) & 0xFF)

#define SFLASH_WREG32(offset, value)      (*(volatile unsigned int *)(SFLASH_REG_BASE + offset)) = (value)
#define SFLASH_RREG32(offset)             (*(volatile unsigned int *)(SFLASH_REG_BASE + offset))

#define SFLASH_WRITE32(offset, value)     (*(volatile unsigned int *)(offset)) = (value)
#define SFLASH_READ32(offset)             (*(volatile unsigned int *)(offset))
#define IO_READ8(base, offset)            ((*(volatile unsigned int *)(base + offset)) & 0xFF)
#define IO_WRITE8(offset, value)          (*(volatile unsigned int *)(offset)) = (value)

//---------------------------------------------------------------------------
// Type definitions
//---------------------------------------------------------------------------
#define MAX_FLASHCOUNT  1
#define SFLASHNAME_LEN  48

#define NORPART_DYNAMIC_TYPE   (0)  // with wear leveling algorithm protection.
#define NORPART_STATIC_TYPE       (1) // raw data access device without wear leveling.
#define NORPART_UNUSED_TYPE      (0xFF) // unused partition.

#ifndef NORPART_PARTIAL_PROTECTION_ID
   #define NORPART_PARTIAL_PROTECTION_ID (2)
#endif
#define NORPART_PROTECTION_ENABLE TRUE
#define NORPART_PROTECTION_DISENABLE FALSE

typedef struct
{
    U8   u1MenuID;
    U8   u1DevID1;
    U8   u1DevID2;
    uint32  u4ChipSize;
    uint32  u4SecSize;
    uint32  u4SecCount;

    U8    pcFlashStr[SFLASHNAME_LEN];
} SFLASH_CHIPINFO_T;

typedef struct
{
    U8   u1FlashCount;
    SFLASH_CHIPINFO_T arFlashInfo[MAX_FLASHCOUNT];
} SFLASH_INFO_T;
//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------
#define ENABLE_DUALREAD
#define SFLASH_MEM_BASE         ((uint32)0x30000000)//mmu on
#define SFLASH_REG_BASE         ((uint32)(0x11000000 + 0x14000))


#define SFLASH_CMD_REG          ((uint32)0x00)
#define SFLASH_CNT_REG          ((uint32)0x04)
#define SFLASH_RDSR_REG         ((uint32)0x08)
#define SFLASH_RDATA_REG        ((uint32)0x0C)
#define SFLASH_RADR0_REG        ((uint32)0x10)
#define SFLASH_RADR1_REG        ((uint32)0x14)
#define SFLASH_RADR2_REG        ((uint32)0x18)
#define SFLASH_WDATA_REG        ((uint32)0x1C)
#define SFLASH_PRGDATA0_REG     ((uint32)0x20)
#define SFLASH_PRGDATA1_REG     ((uint32)0x24)
#define SFLASH_PRGDATA2_REG     ((uint32)0x28)
#define SFLASH_PRGDATA3_REG     ((uint32)0x2C)
#define SFLASH_PRGDATA4_REG     ((uint32)0x30)
#define SFLASH_PRGDATA5_REG     ((uint32)0x34)
#define SFLASH_SHREG0_REG       ((uint32)0x38)
#define SFLASH_SHREG1_REG       ((uint32)0x3C)
#define SFLASH_SHREG2_REG       ((uint32)0x40)
#define SFLASH_SHREG3_REG       ((uint32)0x44)
#define SFLASH_SHREG4_REG       ((uint32)0x48)
#define SFLASH_SHREG5_REG       ((uint32)0x4C)
#define SFLASH_SHREG6_REG       ((uint32)0x50)
#define SFLASH_SHREG7_REG       ((uint32)0x54)
#define SFLASH_SHREG8_REG       ((uint32)0x58)
#define SFLASH_SHREG9_REG       ((uint32)0x5C)
#define SFLASH_FLHCFG_REG      	((uint32)0x84)
#define SFLASH_PP_DATA_REG      ((uint32)0x98)
#define SFLASH_PREBUF_STUS_REG  ((uint32)0x9C)
#define SFLASH_SF_INTRSTUS_REG  ((uint32)0xA8)
#define SFLASH_SF_INTREN_REG    ((uint32)0xAC)
#define SFLASH_SF_TIME_REG      ((uint32)0x94)
#define SFLASH_CHKSUM_CTL_REG   ((uint32)0xB8)
#define SFLASH_CHECKSUM_REG     ((uint32)0xBC)
#define SFLASH_CMD2_REG     	((uint32)0xC0)
#define SFLASH_WRPROT_REG       ((uint32)0xC4)
#define SFLASH_RADR3_REG        ((uint32)0xC8)
#define SFLASH_READ_DUAL_REG    ((uint32)0xCC)
#define SFLASH_DELSEL0_REG    	((uint32)0xA0)
#define SFLASH_DELSEL1_REG    	((uint32)0xA4)
#define SFLASH_DELSEL2_REG    	((uint32)0xD0)
#define SFLASH_DELSEL3_REG    	((uint32)0xD4)
#define SFLASH_DELSEL4_REG    	((uint32)0xD8)
#define SFLASH_CFG1_REG         ((uint32)0x60)
#define SFLASH_CFG2_REG         ((uint32)0x64)
#define SFLASH_CFG3_REG         ((uint32)0x68)
#define SFLASH_STATUS0_REG      ((uint32)0x70)
#define SFLASH_STATUS1_REG      ((uint32)0x74)
#define SFLASH_STATUS2_REG      ((uint32)0x78)
#define SFLASH_STATUS3_REG      ((uint32)0x7C)

//-----------------------------------------------------------------------------
#define SFLASH_WRBUF_SIZE       128

//-----------------------------------------------------------------------------
#define SFLASHHWNAME_LEN    48


typedef struct
{
    U8   u1MenuID;
    U8   u1DevID1;
    U8   u1DevID2;
    U8   u1PPType;
    uint32  u4ChipSize;
    uint32  u4SecSize;
    uint32  u4CEraseTimeoutMSec;

    U8   u1WRENCmd;
    U8   u1WRDICmd;
    U8   u1RDSRCmd;
    U8   u1WRSRCmd;
    U8   u1READCmd;
    U8   u1FASTREADCmd;
    U8   u1READIDCmd;
    U8   u1SECERASECmd;
    U8   u1CHIPERASECmd;
    U8   u1PAGEPROGRAMCmd;
    U8   u1AAIPROGRAMCmd;
    U8   u1DualREADCmd;
    U8   u1Protection;
 //   U8   u1SupBitMode;
    U8    pcFlashStr[SFLASHHWNAME_LEN];
} SFLASHHW_VENDOR_T;

extern bool COMMON_CheckDevID(volatile uint16 *die1_addr, volatile uint16 *die2_addr) ;
extern bool Common_CheckDevIdle(const uint32 addr);
extern void Common_Erase_CMD(const uint32 blockaddr);
extern STATUS_E Common_Erase_CheckDone_By_Polling(const uint32 blockaddr);
extern void Common_PreProcess(void) ;
extern void Common_Program_PostProcess(void);
extern void Common_UnlockBypass_Enter_CMD(const uint32 blockaddr);
extern void Common_UnlockBypass_Exit_CMD(const uint32 blockaddr);
extern void Common_UnlockBypass_Program_CMD(const uint32 blockaddr, const uint32 prog_addr, const uint16 data);
extern STATUS_E Common_Program_CheckDone_By_Toggle(const uint32 prog_addr);
extern void Common_Nor_Buf_Program(const uint32 blockaddr, const uint32 prog_addr, const uint16 *data, const uint32 length_in_word);
extern STATUS_E Common_Nor_Buf_Program_Check_Done(const uint32 prog_addr);

#endif
