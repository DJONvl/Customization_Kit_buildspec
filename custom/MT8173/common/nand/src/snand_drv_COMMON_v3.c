/*******************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of MediaTek Inc. (C) 2005
*
*******************************************************************************/
#if defined(NAND_SPI)


#include "SW_TYPES.H"
#include "NFI.h"
#include "nand_util_func.h"
#include "snand_util_func.h"
#include "snand_drv_COMMON_v3.h"


#ifndef s8
typedef signed char         s8;
#endif
#ifndef u8
typedef unsigned char       u8;
#endif

#ifndef s16
typedef signed short        s16;
#endif
#ifndef u16
typedef unsigned short      u16;
#endif

#ifndef s32
typedef signed int          s32;
#endif
#ifndef u32
typedef unsigned int        u32;
#endif

#ifndef s64
typedef signed long long    s64;
#endif
#ifndef u64
typedef unsigned long long  u64;
#endif

#define false          0
#define true           1

#ifndef u64
typedef unsigned long long  u64;
#endif


#if defined(__MTK_SNAND_V3__)

#define DRV_WriteReg32(addr,data)     ((*(volatile u32 *)(addr)) = (u32)data)
#define DRV_Reg32(addr)               (*(volatile u32 *)(addr))
#define DRV_WriteReg16(addr,data)     ((*(volatile u16 *)(addr)) = (u16)data)
#define DRV_Reg16(addr)               (*(volatile u16 *)(addr))
#define DRV_WriteReg8(addr,data)     ((*(volatile u8 *)(addr)) = (u8)data)
#define DRV_Reg8(addr)               (*(volatile u8 *)(addr))

#define NFI_SET_REG(reg, value)   *(reg) = *(reg) | (value)
#define NFI_CLN_REG(reg, value)   *(reg) = *(reg) & (~(value))


uint32 g_snand_dbg_dev_ecc = 0;

uint32 g_snand_spare_format_1[][2] =    {
                                             {4, 8}
                                            ,{20, 8}
                                            ,{36, 8}
                                            ,{52, 8}
                                            ,{0, 0}
                                        };

#ifdef DUMP_SPEED
extern u32 gpt4_get_current_tick (void);
extern u32 g_ds_nand_block_erase_number,  g_ds_nand_block_erase_time;
extern u32 g_ds_nand_page_program_number, g_ds_nand_page_program_time;
extern u32 g_ds_nand_page_read_number,    g_ds_nand_page_read_time;
#endif

//#define _SNAND_DEBUG

#define SNAND_SPLIT_ECC_BIT_THRESHOLD                   (4)

#if defined(BB_MT6572) || defined(BB_MT8173) // should be 72, use 71 for dev (fixme)
#define SNAND_SPLIT_REGION1_ECC_BIT						(4)
#define SNAND_SPLIT_REGION2_ECC_BIT						(12)
#define SNAND_SPLIT_REGION1_SPARE_PER_SECTOR       		(16)
#define SNAND_SPLIT_REGION2_SPARE_PER_SECTOR       		(28)
#elif defined(BB_MT6571)    // should be 71, use 72 for dev (fixme)
#define SNAND_SPLIT_REGION1_ECC_BIT                 	(0)	    // the 1st region use device ECC
//#define SNAND_SPLIT_REGION2_ECC_BIT						(4)
#define SNAND_SPLIT_REGION2_ECC_BIT						(12)
#define SNAND_SPLIT_REGION1_SPARE_PER_SECTOR       		(0)	    // the 1st region use device ECC
//#define SNAND_SPLIT_REGION2_SPARE_PER_SECTOR       		(16)
#define SNAND_SPLIT_REGION2_SPARE_PER_SECTOR       		(32)
#else
#define SNAND_SPLIT_REGION1_ECC_BIT						(4)
#define SNAND_SPLIT_REGION2_ECC_BIT						(12)
#define SNAND_SPLIT_REGION1_SPARE_PER_SECTOR       		(16)
#define SNAND_SPLIT_REGION2_SPARE_PER_SECTOR       		(28)
#endif

u32 g_snand_ecc_split_boundary          = 1024 * 1024; // 1 MB region
u32 g_snand_ecc_spare_per_sector_now    = SNAND_SPLIT_REGION1_SPARE_PER_SECTOR;
u32 g_snand_ecc_bit_now                 = SNAND_SPLIT_REGION1_ECC_BIT;

#define SNAND_SPARE_BUF_INT_SIZE           (128)
#define SNAND_DATA_BUF_INT_SIZE            (4096 + SNAND_SPARE_BUF_INT_SIZE)

u32                         g_snand_data_buf_int[SNAND_DATA_BUF_INT_SIZE / sizeof(u32)];
u32                         g_snand_spare_buf_int[SNAND_SPARE_BUF_INT_SIZE / sizeof(u32)];

extern void gpt4_init (bool bStart);
extern U32 gpt4_get_current_tick (void);

static bool mtk_snand_is_invalid_page(u32 row_addr)
{
    u32 blk;

    // Gigadevice reserved block checking

    if (0xC8 == NUTL_MAKER_CODE() && 0xF4 == NUTL_DEVICE_CODE())
    {
        blk = row_addr / NUTL_PAGES_PER_BLOCK();

        if (blk >= 2045 && blk <= 2048)
        {
            return TRUE;
        }
    }

    return FALSE;
}

//---------------------------------------------------------------------------
static bool mtk_snand_status_ready(u32 u4Status)
{
    u32 timeout = 0xFFFF;

    u4Status &= ~STA_NAND_BUSY; // STA_NAND_BUSY does not work for SPI-NAND

    while ((DRV_Reg32(NFI_STA_REG32) & u4Status) != 0)
    {
        timeout--;

        if (0 == timeout)
        {
            return FALSE;
        }
    }

    return TRUE;
}

//---------------------------------------------------------------------------
static bool mtk_snand_RFIFOValidSize(u16 u2Size)
{
    u32 timeout = 0xFFFF;
    while (FIFO_RD_REMAIN(DRV_Reg16(NFI_FIFOSTA_REG16)) < u2Size)
    {
        timeout--;
        if (0 == timeout)
        {
            return FALSE;
        }
    }
    if (u2Size == 0)
    {
        while (FIFO_RD_REMAIN(DRV_Reg16(NFI_FIFOSTA_REG16)))
        {
            timeout--;
            if (0 == timeout)
            {
                return FALSE;
            }
        }
    }
    return TRUE;
}

//---------------------------------------------------------------------------
static bool mtk_snand_WFIFOValidSize(u16 u2Size)
{
    u32 timeout = 0xFFFF;
    while (FIFO_WR_REMAIN(DRV_Reg16(NFI_FIFOSTA_REG16)) > u2Size)
    {
        timeout--;
        if (0 == timeout)
        {
            return FALSE;
        }
    }
    if (u2Size == 0)
    {
        while (FIFO_WR_REMAIN(DRV_Reg16(NFI_FIFOSTA_REG16)))
        {
            timeout--;
            if (0 == timeout)
            {
                return FALSE;
            }
        }
    }
    return TRUE;
}

//---------------------------------------------------------------------------
static void mtk_snand_wait_us(u32 us)
{
    // 26MHz, 0.038 us per CPU cycle

    us = us * 100;

    while (us--);
}

static void mtk_snand_dev_mac_enable(SNAND_Mode mode)
{
    u32 mac;

    mac = DRV_Reg32(RW_SNAND_MAC_CTL);

    // SPI
    if (mode == SPI)
    {
        mac &= ~SNAND_MAC_SIO_SEL;   // Clear SIO_SEL to send command in SPI style
        mac |= SNAND_MAC_EN;         // Enable Macro Mode
    }
    // QPI
    else
    {
        /*
         * SFI V2: QPI_EN only effects direct read mode, and it is moved into DIRECT_CTL in V2
         *         There's no need to clear the bit again.
         */
        mac |= (SNAND_MAC_SIO_SEL | SNAND_MAC_EN);  // Set SIO_SEL to send command in QPI style, and enable Macro Mode
    }

    DRV_WriteReg32(RW_SNAND_MAC_CTL, mac);
}

/**
 * @brief This funciton triggers SFI to issue command to serial flash, wait SFI until ready.
 *
 * @remarks: !NOTE! This function must be used with mtk_snand_dev_mac_enable in pair!
 */
static void mtk_snand_dev_mac_trigger(void)
{
    u32 mac;
    u32 timeout;

    mac = DRV_Reg32(RW_SNAND_MAC_CTL);

    // Trigger SFI: Set TRIG and enable Macro mode
    mac |= (SNAND_TRIG | SNAND_MAC_EN);
    DRV_WriteReg32(RW_SNAND_MAC_CTL, mac);

    /*
     * Wait for SFI ready
     * Step 1. Wait for WIP_READY becoming 1 (WIP register is ready)
     */
    timeout = 0xFFFF;

    while (1)
    {
        if (0 != (DRV_Reg32(RW_SNAND_MAC_CTL) & SNAND_WIP_READY))
        {
            break;
        }
        else
        {
            timeout--;

            if (0 == timeout)
            {
                DBG_MSG("[SNAND] mtk_snand_dev_mac_trigger timeout!\n");

                break;
            }
        }
    }

    /*
     * Step 2. Wait for WIP becoming 0 (Controller finishes command write process)
     */
    timeout = 0xFFFF;

    while (1)
    {
        if (0 == ((DRV_Reg32(RW_SNAND_MAC_CTL) & SNAND_WIP)))
        {
            break;
        }
        else
        {
            timeout--;

            if (0 == timeout)
            {
                DBG_MSG("[SNAND] mtk_snand_dev_mac_trigger timeout!\n");

                break;
            }
        }
    }

}

/**
 * @brief This funciton leaves Macro mode and enters Direct Read mode
 *
 * @remarks: !NOTE! This function must be used after mtk_snand_dev_mac_trigger
 */
static void mtk_snand_dev_mac_leave(void)
{
    u32 mac;

    // clear SF_TRIG and leave mac mode
    mac = DRV_Reg32(RW_SNAND_MAC_CTL);

    /*
     * Clear following bits
     * SF_TRIG: Confirm the macro command sequence is completed
     * SNAND_MAC_EN: Leaves macro mode, and enters direct read mode
     * SNAND_MAC_SIO_SEL: Always reset quad macro control bit at the end
     */
    mac &= ~(SNAND_TRIG | SNAND_MAC_EN | SNAND_MAC_SIO_SEL);
    DRV_WriteReg32(RW_SNAND_MAC_CTL, mac);
}

static void mtk_snand_dev_mac_op(SNAND_Mode mode)
{
    mtk_snand_dev_mac_enable(mode);
    mtk_snand_dev_mac_trigger();
    mtk_snand_dev_mac_leave();
}

static void mtk_snand_dev_command_ext(SNAND_Mode mode, const u8 cmd[], u8 data[], const u32 outl, const u32 inl)
{
    u32   tmp;
    u32   i, j;
    P_U8  p_data, p_tmp;

    p_tmp = (P_U8)(&tmp);

    // Moving commands into SFI GPRAM
    for (i = 0, p_data = ((P_U8)RW_SNAND_GPRAM_DATA); i < outl; p_data += 4)
    {
        // Using 4 bytes aligned copy, by moving the data into the temp buffer and then write to GPRAM
        for (j = 0, tmp = 0; i < outl && j < 4; i++, j++)
        {
            p_tmp[j] = cmd[i];
        }

        DRV_WriteReg32(p_data, tmp);
    }

    DRV_WriteReg32(RW_SNAND_MAC_OUTL, outl);
    DRV_WriteReg32(RW_SNAND_MAC_INL, inl);
    mtk_snand_dev_mac_op(mode);

    // for NULL data, this loop will be skipped
    for (i = 0, p_data = ((P_U8)RW_SNAND_GPRAM_DATA + outl); i < inl; ++i, ++data, ++p_data)
    {
        *data = DRV_Reg8(p_data);
    }

    return;
}

static void mtk_snand_dev_command(const u32 cmd, u8 outlen)
{
    DRV_WriteReg32(RW_SNAND_GPRAM_DATA, cmd);
    DRV_WriteReg32(RW_SNAND_MAC_OUTL, outlen);
    DRV_WriteReg32(RW_SNAND_MAC_INL , 0);
    mtk_snand_dev_mac_op(SPI);

    return;
}

bool mtk_snand_reset_con(void)
{
    int timeout = 0xFFFF;

    // part 1. SNF

    *RW_SNAND_MISC_CTL = *RW_SNAND_MISC_CTL | SNAND_SW_RST;
    *RW_SNAND_MISC_CTL = *RW_SNAND_MISC_CTL &= ~SNAND_SW_RST;

    // part 2. NFI

    if (DRV_Reg16(NFI_MASTERSTA_REG16)) // master is busy
    {
        *NFI_CON = CON_FIFO_FLUSH | CON_NFI_RST;

	while (1)
	{
		if (DRV_Reg16(NFI_MASTERSTA_REG16))
		{
			--timeout;

			if (timeout <= 0)
			{
				break;
			}
		}
	}
        //while (DRV_Reg16(NFI_MASTERSTA_REG16));
    }

    /* issue reset operation */
    *NFI_CON = CON_FIFO_FLUSH | CON_NFI_RST;

    //return mtk_snand_status_ready(STA_NFI_FSM_MASK | STA_NAND_BUSY) && mtk_snand_RFIFOValidSize(0) && mtk_snand_WFIFOValidSize(0);
    return mtk_snand_status_ready(STA_NFI_FSM_MASK | STA_NAND_FSM_MASK) && mtk_snand_RFIFOValidSize(0) && mtk_snand_WFIFOValidSize(0);
}

static void mtk_snand_reset_dev()
{
    u8 cmd = g_pNandInfo->m_dev->m_cmd_set->m_reset.m_cmd;
    u32 timeout = 0xFFFF;

    // issue SW RESET command to device
    mtk_snand_dev_command_ext(SPI, &cmd, NULL, 1, 0);

    // wait for awhile, then polling status register (required by spec)
    mtk_snand_wait_us(SNAND_DEV_RESET_LATENCY_US);

    *RW_SNAND_GPRAM_DATA = (g_pNandInfo->m_dev->m_cmd_set->m_get_feature.m_cmd | (SNAND_CMD_FEATURES_STATUS << 8));
    *RW_SNAND_MAC_OUTL = 2;
    *RW_SNAND_MAC_INL = 1;

    // polling status register

    for (;;)
    {
        mtk_snand_dev_mac_op(SPI);

        cmd = DRV_Reg8(((P_U8)RW_SNAND_GPRAM_DATA + 2));

        if (0 == (cmd & SNAND_STATUS_OIP))
        {
            break;
        }
	else
	{
		timeout--;

		if (timeout == 0)
		{
			break;
		}
	}
    }
}

static u32 mtk_snand_reverse_byte_order(u32 num)
{
   u32 ret = 0;

   ret |= ((num >> 24) & 0x000000FF);
   ret |= ((num >> 8)  & 0x0000FF00);
   ret |= ((num << 8)  & 0x00FF0000);
   ret |= ((num << 24) & 0xFF000000);

   return ret;
}

static u32 mtk_snand_gen_c1a3(const u32 cmd, const u32 address)
{
    return ((mtk_snand_reverse_byte_order(address) & 0xFFFFFF00) | (cmd & 0xFF));
}

static void mtk_snand_dev_enable_spiq(bool enable)
{
    u8   regval;
    u32  cmd;

    // read QE in status register
    cmd = g_pNandInfo->m_dev->m_cmd_set->m_get_feature.m_cmd | (SNAND_CMD_FEATURES_OTP << 8);
    DRV_WriteReg32(RW_SNAND_GPRAM_DATA, cmd);
    DRV_WriteReg32(RW_SNAND_MAC_OUTL, 2);
    DRV_WriteReg32(RW_SNAND_MAC_INL , 1);

    mtk_snand_dev_mac_op(SPI);

    regval = DRV_Reg8(((volatile u8 *)RW_SNAND_GPRAM_DATA + 2));

    if (FALSE == enable)    // disable x4
    {
        if ((regval & SNAND_OTP_QE) == 0)
        {
            return;
        }
        else
        {
            regval = regval & ~SNAND_OTP_QE;
        }
    }
    else    // enable x4
    {
        if ((regval & SNAND_OTP_QE) == 1)
        {
            return;
        }
        else
        {
            regval = regval | SNAND_OTP_QE;
        }
    }

    // if goes here, it means QE needs to be set as new different value

    // write status register
    cmd = g_pNandInfo->m_dev->m_cmd_set->m_set_feature.m_cmd | (SNAND_CMD_FEATURES_OTP << 8) | (regval << 16);
    DRV_WriteReg32(RW_SNAND_GPRAM_DATA, cmd);
    DRV_WriteReg32(RW_SNAND_MAC_OUTL, 3);
    DRV_WriteReg32(RW_SNAND_MAC_INL , 0);

    mtk_snand_dev_mac_op(SPI);
}

static void mtk_snand_configure_fdm(u16 u2FDMSize)
{
    NFI_CLN_REG(NFI_PAGEFMT_REG16, PAGEFMT_FDM_MASK | PAGEFMT_FDM_ECC_MASK);
    NFI_SET_REG(NFI_PAGEFMT_REG16, u2FDMSize << PAGEFMT_FDM_SHIFT);
    NFI_SET_REG(NFI_PAGEFMT_REG16, u2FDMSize << PAGEFMT_FDM_ECC_SHIFT);
}

//---------------------------------------------------------------------------
static void mtk_snand_ecc_config(u32 ecc_bit)
{
    u32 u4ENCODESize;
    u32 u4DECODESize;
    u32 timeout;

    u32 ecc_bit_cfg = ECC_CNFG_ECC4;

    switch (ecc_bit)
    {
      case 4:
          ecc_bit_cfg = ECC_CNFG_ECC4;
          break;
      case 8:
          ecc_bit_cfg = ECC_CNFG_ECC8;
          break;
      case 10:
          ecc_bit_cfg = ECC_CNFG_ECC10;
          break;
      case 12:
          ecc_bit_cfg = ECC_CNFG_ECC12;
          break;
      default:
          break;
    }

    DRV_WriteReg16(ECC_DECCON_REG16, DEC_DE);

    timeout = 0xFFFF;

    do
    {
        timeout--;

        if (0 == timeout)
        {
            DBG_MSG("[SNAND] mtk_snand_ecc_config timeout: ECC_DECIDLE_REG16\n");

            break;
        }
    }
    while (!DRV_Reg16(ECC_DECIDLE_REG16));

    DRV_WriteReg16(ECC_ENCCON_REG16, ENC_DE);

    timeout = 0xFFFF;

    do
    {
        timeout--;

        if (0 == timeout)
        {
            DBG_MSG("[SNAND] mtk_snand_ecc_config timeout: ECC_ENCIDLE_REG32\n");

            break;
        }
    }
    while (!DRV_Reg32(ECC_ENCIDLE_REG32));

    /* setup FDM register base */
    //DRV_WriteReg32(ECC_FDMADDR_REG32, NFI_FDM0L_REG32);

    u4ENCODESize = (NAND_SECTOR_SIZE + 8) << 3;

    #if defined(BB_MT6571)
    u4DECODESize = ((NAND_SECTOR_SIZE + 8) << 3) + ecc_bit * 14;    // MT6571 use 14 bit ECC parity for each byte data
    #elif (defined(BB_MT6572) || defined(BB_MT8173))
    u4DECODESize = ((NAND_SECTOR_SIZE + 8) << 3) + ecc_bit * 13;    // MT6572 use 13 bit ECC parity for each byte data
    #else
    #error "Please confirm ecoding size for MTK ECC in this chip!"
    #endif

    /* configure ECC decoder && encoder */
    DRV_WriteReg32(ECC_DECCNFG_REG32, ecc_bit_cfg | DEC_CNFG_NFI | DEC_CNFG_EMPTY_EN | (u4DECODESize << DEC_CNFG_CODE_SHIFT));

    DRV_WriteReg32(ECC_ENCCNFG_REG32, ecc_bit_cfg | ENC_CNFG_NFI | (u4ENCODESize << ENC_CNFG_MSG_SHIFT));   // FIXME: Need confirm ENC_CNFG_NFI will be clear when DMA is used

#ifndef MANUAL_CORRECT
    NFI_SET_REG(ECC_DECCNFG_REG32, DEC_CNFG_CORRECT);
#else
    NFI_SET_REG(ECC_DECCNFG_REG32, DEC_CNFG_EL);
#endif

}

void mtk_snand_ecc_config_dynamic(u32 row_addr)
{
    u32     page_size;
    u16     spare_bit = 0;

    if (0 == (NUTL_ADDR_CYCLE() & SNAND_ADV_ECC_SPLIT))
    {
        return;
    }

    page_size = NUTL_PAGE_SIZE();

    if (row_addr * page_size < g_snand_ecc_split_boundary)
    {
        g_snand_ecc_spare_per_sector_now = SNAND_SPLIT_REGION1_SPARE_PER_SECTOR;
    }
    else
    {
        g_snand_ecc_spare_per_sector_now = SNAND_SPLIT_REGION2_SPARE_PER_SECTOR;
    }

	if (0 == g_snand_ecc_spare_per_sector_now)
	{
	    g_snand_ecc_bit_now = 0;

	    mtk_snand_dev_ecc_control(1);
    }
    else
    {
        mtk_snand_dev_ecc_control(0);
    }

    if (g_snand_ecc_spare_per_sector_now >= 32)
    {
        spare_bit = PAGEFMT_SPARE_32;
        g_snand_ecc_bit_now = 12;
        g_snand_ecc_spare_per_sector_now = 32;
    }
    else if (g_snand_ecc_spare_per_sector_now >= 28)
    {
        spare_bit = PAGEFMT_SPARE_28;
        g_snand_ecc_bit_now = 12;
        g_snand_ecc_spare_per_sector_now = 28;
    }
    else if (g_snand_ecc_spare_per_sector_now >= 27)
    {
        spare_bit = PAGEFMT_SPARE_27;
        g_snand_ecc_bit_now = 8;
        g_snand_ecc_spare_per_sector_now = 27;
    }
    else if (g_snand_ecc_spare_per_sector_now >= 26)
    {
        spare_bit = PAGEFMT_SPARE_26;
        g_snand_ecc_bit_now = 8;
        g_snand_ecc_spare_per_sector_now = 26;
    }
    else if (g_snand_ecc_spare_per_sector_now >= 16)
    {
        spare_bit = PAGEFMT_SPARE_16;
        g_snand_ecc_bit_now = 4;
        g_snand_ecc_spare_per_sector_now = 16;
    }
    else if (g_snand_ecc_spare_per_sector_now == 0) // g_snand_ecc_bit_now should be 0 as well here
    {
        spare_bit = NUTL_SPARE_PER_SEC_SIZE();

        if (16 == spare_bit)
        {
            spare_bit = PAGEFMT_SPARE_16;
        }
        else if (26 == spare_bit)
        {
            spare_bit = PAGEFMT_SPARE_26;
        }
        else if (27 == spare_bit)
        {
            spare_bit = PAGEFMT_SPARE_27;
        }
        else if (28 == spare_bit)
        {
            spare_bit = PAGEFMT_SPARE_28;
        }
        else if (32 == spare_bit)
        {
            spare_bit = PAGEFMT_SPARE_32;
        }
    }

    DRV_WriteReg16(NFI_CNFG_REG16, 0);
    DRV_WriteReg16(NFI_PAGEFMT_REG16, 0);

    if (page_size == 4096)
    {
        NFI_SET_REG(NFI_PAGEFMT_REG16, (spare_bit << PAGEFMT_SPARE_SHIFT) | PAGEFMT_4K);
    }
    else if (page_size == 2048)
    {
        NFI_SET_REG(NFI_PAGEFMT_REG16, (spare_bit << PAGEFMT_SPARE_SHIFT) | PAGEFMT_2K);
    }
    else if (page_size == 512)
    {
        NFI_SET_REG(NFI_PAGEFMT_REG16, (spare_bit << PAGEFMT_SPARE_SHIFT) | PAGEFMT_512);
    }

    #if defined(BB_MT6572) || defined(BB_MT8173)
    // do nothing
    #elif defined(BB_MT6571)
    NFI_SET_REG(NFI_PAGEFMT_REG16, PAGEFMT_SEC_SEL_512);
    #else
    #error "Please confirm page size configuration in this chip!"
    #endif

    // initialize HW ECC related registers

    if (0 != g_snand_ecc_bit_now)
    {
        NFI_SET_REG(NFI_CNFG_REG16, CNFG_HW_ECC_EN);
        mtk_snand_ecc_config(g_snand_ecc_bit_now);
    }

    mtk_snand_configure_fdm(8);
}

int mtk_snand_init(void)
{
    u32     page_size;
    u16     spare_bit = 0;
    u16     spare_per_sector = 16;
    u32     ecc_bit = 4;

    page_size = NUTL_PAGE_SIZE();

    DRV_WriteReg16(NFI_CSEL_REG16, NFI_DEFAULT_CS);

    /* Set default NFI access timing control */
    DRV_WriteReg32(NFI_ACCCON_REG32, NFI_DEFAULT_ACCESS_TIMING);

    DRV_WriteReg16(NFI_CNFG_REG16, 0);
    DRV_WriteReg16(NFI_PAGEFMT_REG16, 0);

    /* Reset NFI HW internal state machine and flush NFI in/out FIFO */
    mtk_snand_reset_con();

    // raise SNF to 104 MHz

    #if defined(BB_MT6571) || defined(BB_MT6572) || defined(BB_MT8173)
    #if 1
    // configure SNF timing
    *RW_SNAND_DLY_CTL1 = 0x00000000;
    *RW_SNAND_DLY_CTL2 = 0x09090909;
    *RW_SNAND_DLY_CTL3 = 0x20000020;
    *RW_SNAND_DLY_CTL4 = 0x00000000;
    *RW_SNAND_MISC_CTL = 0x0542000A;

    // set inverse clk & latch latency
    *RW_SNAND_MISC_CTL &= ~SNAND_CLK_INVERSE;       // disable inverse clock and 1 T delay
    *RW_SNAND_MISC_CTL &= ~SNAND_LATCH_LAT_MASK;    // set latency to 0T delay
    *RW_SNAND_MISC_CTL |= SNAND_SAMPLE_CLK_INVERSE; // enable sample clock inverse
    *RW_SNAND_MISC_CTL |= SNAND_4FIFO_EN;

    // raise SNF to 104 MHz
    *((P_U32)(0x10000000)) = (*((P_U32)(0x10000000)) & 0xBFFE3FFF) | 0x40010000;    // (DA does not have such API, thus modify register directly)
    #endif
    #endif

    spare_per_sector = NUTL_SPARE_PER_SEC_SIZE();

    if (spare_per_sector >= 32)
    {
        spare_bit = PAGEFMT_SPARE_32;
        ecc_bit = 12;
        spare_per_sector = 32;
    } else if (spare_per_sector >= 28)
    {
        spare_bit = PAGEFMT_SPARE_28;
        ecc_bit = 12;
        spare_per_sector = 28;
    } else if (spare_per_sector >= 27)
    {
        spare_bit = PAGEFMT_SPARE_27;
        ecc_bit = 8;
        spare_per_sector = 27;
    } else if (spare_per_sector >= 26)
    {
        spare_bit = PAGEFMT_SPARE_26;
        ecc_bit = 8;
        spare_per_sector = 26;
    } else if (spare_per_sector >= 16)
    {
        spare_bit = PAGEFMT_SPARE_16;
        ecc_bit = 4;
        spare_per_sector = 16;
    } else
    {
        //ASSERT(0);
    }

    if (page_size == 4096)
    {
        NFI_SET_REG(NFI_PAGEFMT_REG16, (spare_bit << PAGEFMT_SPARE_SHIFT) | PAGEFMT_4K);
    }
    else if (page_size == 2048)
    {
        NFI_SET_REG(NFI_PAGEFMT_REG16, (spare_bit << PAGEFMT_SPARE_SHIFT) | PAGEFMT_2K);
    }
    else if (page_size == 512)
    {
        NFI_SET_REG(NFI_PAGEFMT_REG16, (spare_bit << PAGEFMT_SPARE_SHIFT) | PAGEFMT_512);
    }

    #if defined(BB_MT6572) || defined(BB_MT8173)
    // do nothing
    #elif defined(BB_MT6571)
    NFI_SET_REG(NFI_PAGEFMT_REG16, PAGEFMT_SEC_SEL_512);
    #else
    #error "Please confirm page size configuration in this chip!"
    #endif

    // initialize HW ECC related registers
    NFI_SET_REG(NFI_CNFG_REG16, CNFG_HW_ECC_EN);
    mtk_snand_ecc_config(ecc_bit);
    mtk_snand_configure_fdm(8);

    /* Initilize interrupt. Clear interrupt, read clear. */
    DRV_Reg16(NFI_INTR_REG16);

    /* Interrupt arise when read data or program data to/from AHB is done. */
    DRV_WriteReg16(NFI_INTR_EN_REG16, 0);

    return 0;
}

//---------------------------------------------------------------------------
static void mtk_snand_set_mode(u16 u2OpMode)
{
    u16 u2Mode = DRV_Reg16(NFI_CNFG_REG16);
    u2Mode &= ~CNFG_OP_MODE_MASK;
    u2Mode |= u2OpMode;
    DRV_WriteReg16(NFI_CNFG_REG16, u2Mode);
}

//---------------------------------------------------------------------------
static void mtk_snand_set_autoformat(bool bEnable)
{
    if (bEnable)
    {
        NFI_SET_REG(NFI_CNFG_REG16, CNFG_AUTO_FMT_EN);
    } else
    {
        NFI_CLN_REG(NFI_CNFG_REG16, CNFG_AUTO_FMT_EN);
    }
}

//---------------------------------------------------------------------------
static void mtk_snand_ecc_decode_start(void)
{
    /* wait for device returning idle */
    while (!(DRV_Reg16(ECC_DECIDLE_REG16) & DEC_IDLE)) ;
    DRV_WriteReg16(ECC_DECCON_REG16, DEC_EN);
}

//---------------------------------------------------------------------------
static void mtk_snand_ecc_decode_end(void)
{
    /* wait for device returning idle */
    while (!(DRV_Reg16(ECC_DECIDLE_REG16) & DEC_IDLE)) ;
    DRV_WriteReg16(ECC_DECCON_REG16, DEC_DE);
}

//---------------------------------------------------------------------------
static void mtk_snand_ecc_encode_start(void)
{
    /* wait for device returning idle */
    while (!(DRV_Reg32(ECC_ENCIDLE_REG32) & ENC_IDLE)) ;
    DRV_WriteReg16(ECC_ENCCON_REG16, ENC_EN);
}

//---------------------------------------------------------------------------
static void mtk_snand_ecc_encode_end(void)
{
    /* wait for device returning idle */
    while (!(DRV_Reg32(ECC_ENCIDLE_REG32) & ENC_IDLE)) ;
    DRV_WriteReg16(ECC_ENCCON_REG16, ENC_DE);
}

//-----------------------------------------------------------------------------
static bool mtk_snand_check_dececc_done(u32 u4SecNum)
{
    u32 timeout, dec_mask;
    timeout = 0xffff;
    dec_mask = (1 << u4SecNum) - 1;

    while ((dec_mask != DRV_Reg16(ECC_DECDONE_REG16)) && timeout > 0) // all sectors are decoded done!
    {
        timeout--;

        if (timeout == 0)
        {
            return FALSE;
        }
    }

    return TRUE;
}

//-----------------------------------------------------------------------------
static void mtk_snand_stop_read_devecc(void)
{
    //------ NFI Part

    NFI_CLN_REG(NFI_CON, CON_NFI_BRD);

    //------ SNF Part

    // set 1 then set 0 to clear done flag
    DRV_WriteReg32(RW_SNAND_STA_CTL1, SNAND_CUSTOM_READ);
    DRV_WriteReg32(RW_SNAND_STA_CTL1, 0);

    // clear essential SNF setting
    NFI_CLN_REG(RW_SNAND_MISC_CTL, SNAND_DATARD_CUSTOM_EN);

    //mtk_snand_dev_enable_spiq(FALSE);
}

//-----------------------------------------------------------------------------
static void mtk_snand_stop_read(void)
{
    //------ NFI Part

    NFI_CLN_REG(NFI_CON, CON_NFI_BRD);

    //------ SNF Part

    // set 1 then set 0 to clear done flag
    DRV_WriteReg32(RW_SNAND_STA_CTL1, SNAND_CUSTOM_READ);
    DRV_WriteReg32(RW_SNAND_STA_CTL1, 0);

    // clear essential SNF setting
    NFI_CLN_REG(RW_SNAND_MISC_CTL, SNAND_DATARD_CUSTOM_EN);

    //mtk_snand_dev_enable_spiq(FALSE);

    mtk_snand_ecc_decode_end();
}

static bool mtk_snand_split_read_part2(u32 row_addr, u32 num_sec, u8 nfi_auto_fmt, u8 * buf)
{
    bool    bRet = FALSE;
    u32     reg;
    SNAND_Mode mode = SPIQ;
    u32     col_part2, i, len;
    P_U8    buf_part2;
    u32     timeout = 0xFFFF;

    for (i = 0; i < 2 ; i++)
    {
        mtk_snand_reset_con();

        if (0 == i)
        {
            //mtk_snand_dev_enable_spiq(TRUE);

            col_part2 = (NAND_SECTOR_SIZE + g_snand_ecc_spare_per_sector_now) * (num_sec - 1);

            if (0 == nfi_auto_fmt)
            {
                buf_part2 = buf + col_part2;
            }
            else
            {
                buf_part2 = buf + (NAND_SECTOR_SIZE * (num_sec - 1));
            }

            len = 2112 - col_part2;
        }
        else
        {
            col_part2 = 2112;

            buf_part2 += len;   // append to first round

            len = (num_sec * (NAND_SECTOR_SIZE + g_snand_ecc_spare_per_sector_now)) - 2112;
        }

        //------ SNF Part ------

        // set DATA READ address
        DRV_WriteReg32(RW_SNAND_RD_CTL3, (col_part2 & SNAND_DATA_READ_ADDRESS_MASK));

        // set RW_SNAND_MISC_CTL
        reg = DRV_Reg32(RW_SNAND_MISC_CTL);

        reg |= SNAND_DATARD_CUSTOM_EN;

        reg &= ~SNAND_DATA_READ_MODE_MASK;
        reg |= ((SNAND_DATA_READ_MODE_X4 << SNAND_DATA_READ_MODE_OFFSET) & SNAND_DATA_READ_MODE_MASK);

        DRV_WriteReg32(RW_SNAND_MISC_CTL, reg);

        // set SNF data length
        reg = len | (len << SNAND_PROGRAM_LOAD_BYTE_LEN_OFFSET);

        DRV_WriteReg32(RW_SNAND_MISC_CTL2, reg);

        //------ NFI Part ------

        mtk_snand_set_mode(CNFG_OP_CUST);
        NFI_SET_REG(NFI_CNFG_REG16, CNFG_READ_EN);
        NFI_SET_REG(NFI_CNFG_REG16, CNFG_AHB);
        NFI_CLN_REG(NFI_CNFG_REG16, CNFG_HW_ECC_EN);
        mtk_snand_set_autoformat(FALSE);

        *NFI_CON = 1 << CON_NFI_SEC_SHIFT;  // fixed to sector number 1

        DRV_WriteReg32(NFI_STRADDR_REG32, buf_part2);

        DRV_WriteReg32(NFI_SPIDMA_REG32, SPIDMA_SEC_EN | (len & SPIDMA_SEC_SIZE_MASK));


        NFI_CLN_REG(NFI_CNFG_REG16, CNFG_BYTE_RW);

        // set dummy command to trigger NFI enter custom mode
        DRV_WriteReg16(NFI_CMD_REG16, NAND_CMD_DUMMYREAD);

        DRV_Reg16(NFI_INTR_REG16);  // read clear
        DRV_WriteReg16(NFI_INTR_EN_REG16, INTR_AHB_DONE_EN);

        NFI_SET_REG(NFI_CON, CON_NFI_BRD);

        timeout = 0xFFFF;

        while (!(DRV_Reg16(NFI_INTR_REG16) & INTR_AHB_DONE))    // for custom read, wait NFI's INTR_AHB_DONE done to ensure all data are transferred to buffer
        {
            timeout--;

            if (0 == timeout)
            {
                return FALSE;
            }
        }

        timeout = 0xFFFF;

        while (((*NFI_BYTELEN & 0xf000) >> 12) != 1)
        {
            timeout--;

            if (0 == timeout)
            {
                return FALSE;
            }
        }

        //------ NFI Part

        NFI_CLN_REG(NFI_CON, CON_NFI_BRD);

        //------ SNF Part

        // set 1 then set 0 to clear done flag
        DRV_WriteReg32(RW_SNAND_STA_CTL1, SNAND_CUSTOM_READ);
        DRV_WriteReg32(RW_SNAND_STA_CTL1, 0);

        // clear essential SNF setting
        NFI_CLN_REG(RW_SNAND_MISC_CTL, SNAND_DATARD_CUSTOM_EN);
    }

    /* configure ECC decoder && encoder */
    reg = DRV_Reg32(ECC_DECCNFG_REG32);
    reg &= ~DEC_CNFG_DEC_MODE_MASK;
    reg |= DEC_CNFG_AHB;
    DRV_WriteReg32(ECC_DECCNFG_REG32, reg);

    if (0 == nfi_auto_fmt)
    {
        DRV_WriteReg32(ECC_DECDIADDR, (u32)(buf + ((NAND_SECTOR_SIZE + g_snand_ecc_spare_per_sector_now) * (num_sec - 1))));
    }
    else
    {
        DRV_WriteReg32(ECC_DECDIADDR, (u32)(buf + (NAND_SECTOR_SIZE * (num_sec - 1))));
    }

    DRV_WriteReg16(ECC_DECCON, 0);
    DRV_WriteReg16(ECC_DECCON, DEC_EN);

    while(!((DRV_Reg32(ECC_DECDONE)) & (1 << 0)));

  cleanup:
    return bRet;
}

//---------------------------------------------------------------------------
static bool mtk_snand_ready_for_read_devecc(u32 row_addr, u32 sec_num, u8 * buf)
//bool mtk_snand_ready_for_read(u32 row_addr, u32 sec_num, u8 * buf)
{
    u32     colnob = 2;
    bool    bRet = FALSE;
    u32     cmd;
    u32     reg;
    u32     page_size;
    u32     spare_size;
    SNAND_Mode mode = SPIQ;

    page_size = NUTL_PAGE_SIZE();
	spare_size = NUTL_SPARE_SIZE();

    if (!mtk_snand_reset_con())
    {
        goto cleanup;
    }

    // 1. Read page to cache

    cmd = mtk_snand_gen_c1a3(g_pNandInfo->m_dev->m_cmd_set->m_read.m_cmd, row_addr); // PAGE_READ command + 3-byte address

    DRV_WriteReg32(RW_SNAND_GPRAM_DATA, cmd);
    DRV_WriteReg32(RW_SNAND_MAC_OUTL, 1 + 3);
    DRV_WriteReg32(RW_SNAND_MAC_INL , 0);

    mtk_snand_dev_mac_op(SPI);

    // 2. Get features (status polling)

    cmd = g_pNandInfo->m_dev->m_cmd_set->m_get_feature.m_cmd | (SNAND_CMD_FEATURES_STATUS << 8);

    DRV_WriteReg32(RW_SNAND_GPRAM_DATA, cmd);
    DRV_WriteReg32(RW_SNAND_MAC_OUTL, 2);
    DRV_WriteReg32(RW_SNAND_MAC_INL , 1);

    for (;;)
    {
        mtk_snand_dev_mac_op(SPI);

        cmd = DRV_Reg8(((P_U8)RW_SNAND_GPRAM_DATA + 2));

        if ((cmd & SNAND_STATUS_OIP) == 0)
        {
            if (SNAND_STATUS_TOO_MANY_ERROR_BITS == (cmd & SNAND_STATUS_ECC_STATUS_MASK) )
            {
                bRet = FALSE;
            }

            break;
        }
    }

    //------ SNF Part ------

    // set PAGE READ command & address
    reg = (g_pNandInfo->m_dev->m_cmd_set->m_read.m_cmd << SNAND_PAGE_READ_CMD_OFFSET) | (row_addr & SNAND_PAGE_READ_ADDRESS_MASK);
    DRV_WriteReg32(RW_SNAND_RD_CTL1, reg);

    // set DATA READ dummy cycle and command (use default value, ignored)
    if (mode == SPI)
    {
        reg = DRV_Reg32(RW_SNAND_RD_CTL2);
        reg &= ~SNAND_DATA_READ_CMD_MASK;
        reg |= g_pNandInfo->m_dev->m_cmd_set->m_read_cnf.m_cmd & SNAND_DATA_READ_CMD_MASK;
        DRV_WriteReg32(RW_SNAND_RD_CTL2, reg);

    }
    else if (mode == SPIQ)
    {
        mtk_snand_dev_enable_spiq(TRUE);

        reg = DRV_Reg32(RW_SNAND_RD_CTL2);
        reg &= ~SNAND_DATA_READ_CMD_MASK;
        reg |= g_pNandInfo->m_dev->m_cmd_set->m_read_cnf.m_cmd & SNAND_DATA_READ_CMD_MASK;
        DRV_WriteReg32(RW_SNAND_RD_CTL2, reg);
    }

    // set DATA READ address
    DRV_WriteReg32(RW_SNAND_RD_CTL3, (0 & SNAND_DATA_READ_ADDRESS_MASK));

    // set SNF data length
    reg = (page_size + spare_size) |
          ((page_size + spare_size) << SNAND_PROGRAM_LOAD_BYTE_LEN_OFFSET);

    DRV_WriteReg32(RW_SNAND_MISC_CTL2, reg);

    // set SNF timing
    reg = DRV_Reg32(RW_SNAND_MISC_CTL);

    reg |= SNAND_DATARD_CUSTOM_EN;

    if (mode == SPI)
    {
        reg &= ~SNAND_DATA_READ_MODE_MASK;
    }
    else if (mode == SPIQ)
    {
        reg &= ~SNAND_DATA_READ_MODE_MASK;
        reg |= ((SNAND_DATA_READ_MODE_X4 << SNAND_DATA_READ_MODE_OFFSET) & SNAND_DATA_READ_MODE_MASK);
    }

    DRV_WriteReg32(RW_SNAND_MISC_CTL, reg);

    //------ NFI Part ------

    mtk_snand_set_mode(CNFG_OP_CUST);
    NFI_SET_REG(NFI_CNFG_REG16, CNFG_READ_EN);
    *NFI_CON = sec_num << CON_NFI_SEC_SHIFT;

    DRV_WriteReg32(NFI_SPIDMA_REG32, 0);

#if 0
#ifndef DISABLE_NFI_DMA
    NFI_SET_REG(NFI_CNFG_REG16, CNFG_AHB);
   // NFI_SET_REG(NFI_CNFG_REG16, CNFG_AHB_BURST_EN);
#else
    NFI_CLN_REG(NFI_CNFG_REG16, CNFG_AHB);
#endif
#endif

    // use DMA anyway
    NFI_SET_REG(NFI_CNFG_REG16, CNFG_AHB);

    DRV_WriteReg32(NFI_STRADDR_REG32, buf);

    bRet = TRUE;

  cleanup:
    return bRet;
}


//---------------------------------------------------------------------------
static bool mtk_snand_ready_for_read(u32 row_addr, u32 sec_num, u8 * buf)
{
    u32     colnob = 2;
    bool    bRet = FALSE;
    u32     cmd;
    u32     reg;
    u32     page_size;
    u32     spare_size;
    SNAND_Mode mode = SPIQ;

    page_size = NUTL_PAGE_SIZE();
	spare_size = NUTL_SPARE_SIZE();

    if (!mtk_snand_reset_con())
    {
        goto cleanup;
    }

    // 1. Read page to cache

    cmd = mtk_snand_gen_c1a3(g_pNandInfo->m_dev->m_cmd_set->m_read.m_cmd, row_addr); // PAGE_READ command + 3-byte address

    DRV_WriteReg32(RW_SNAND_GPRAM_DATA, cmd);
    DRV_WriteReg32(RW_SNAND_MAC_OUTL, 1 + 3);
    DRV_WriteReg32(RW_SNAND_MAC_INL , 0);

    mtk_snand_dev_mac_op(SPI);

    // 2. Get features (status polling)

    cmd = g_pNandInfo->m_dev->m_cmd_set->m_get_feature.m_cmd | (SNAND_CMD_FEATURES_STATUS << 8);

    DRV_WriteReg32(RW_SNAND_GPRAM_DATA, cmd);
    DRV_WriteReg32(RW_SNAND_MAC_OUTL, 2);
    DRV_WriteReg32(RW_SNAND_MAC_INL , 1);

    for (;;)
    {
        mtk_snand_dev_mac_op(SPI);

        cmd = DRV_Reg8(((P_U8)RW_SNAND_GPRAM_DATA + 2));

        if ((cmd & SNAND_STATUS_OIP) == 0)
        {
            if (SNAND_STATUS_TOO_MANY_ERROR_BITS == (cmd & SNAND_STATUS_ECC_STATUS_MASK) )
            {
                bRet = FALSE;
            }
            break;
        }
    }

    //------ SNF Part ------

    // set PAGE READ command & address
    reg = (g_pNandInfo->m_dev->m_cmd_set->m_read.m_cmd << SNAND_PAGE_READ_CMD_OFFSET) | (row_addr & SNAND_PAGE_READ_ADDRESS_MASK);
    DRV_WriteReg32(RW_SNAND_RD_CTL1, reg);

    // set DATA READ dummy cycle and command (use default value, ignored)
    if (mode == SPI)
    {
        reg = DRV_Reg32(RW_SNAND_RD_CTL2);
        reg &= ~SNAND_DATA_READ_CMD_MASK;
        reg |= g_pNandInfo->m_dev->m_cmd_set->m_read_cnf.m_cmd & SNAND_DATA_READ_CMD_MASK;
        DRV_WriteReg32(RW_SNAND_RD_CTL2, reg);

    }
    else if (mode == SPIQ)
    {
        mtk_snand_dev_enable_spiq(TRUE);

        reg = DRV_Reg32(RW_SNAND_RD_CTL2);
        reg &= ~SNAND_DATA_READ_CMD_MASK;
        reg |= g_pNandInfo->m_dev->m_cmd_set->m_read_cnf.m_cmd & SNAND_DATA_READ_CMD_MASK;
        DRV_WriteReg32(RW_SNAND_RD_CTL2, reg);
    }

    // set DATA READ address
    DRV_WriteReg32(RW_SNAND_RD_CTL3, (0 & SNAND_DATA_READ_ADDRESS_MASK));

    // set SNF data length
    if (((NUTL_ADDR_CYCLE() & SNAND_ADV_ECC_SPLIT) == 0) ||
        (((NUTL_ADDR_CYCLE() & SNAND_ADV_ECC_SPLIT) != 0) && (SNAND_SPLIT_REGION1_ECC_BIT == g_snand_ecc_bit_now)) ||
        (((NUTL_ADDR_CYCLE() & SNAND_ADV_ECC_SPLIT) != 0) && (SNAND_SPLIT_REGION1_ECC_BIT != g_snand_ecc_bit_now) && (g_snand_ecc_bit_now <= SNAND_SPLIT_ECC_BIT_THRESHOLD)))  // ECC split && 4-bit
    {
        reg = (page_size + spare_size) |
              ((page_size + spare_size) << SNAND_PROGRAM_LOAD_BYTE_LEN_OFFSET);
    }
    else    // enable 3+1 read, this is the first part
    {
        reg = (((page_size / NAND_SECTOR_SIZE) - 1) * (NAND_SECTOR_SIZE + g_snand_ecc_spare_per_sector_now)) |
              ((((page_size / NAND_SECTOR_SIZE) - 1) * (NAND_SECTOR_SIZE + g_snand_ecc_spare_per_sector_now)) << SNAND_PROGRAM_LOAD_BYTE_LEN_OFFSET);
    }

    DRV_WriteReg32(RW_SNAND_MISC_CTL2, reg);

    // set SNF timing
    reg = DRV_Reg32(RW_SNAND_MISC_CTL);

    reg |= SNAND_DATARD_CUSTOM_EN;

    if (mode == SPI)
    {
        reg &= ~SNAND_DATA_READ_MODE_MASK;
    }
    else if (mode == SPIQ)
    {
        reg &= ~SNAND_DATA_READ_MODE_MASK;
        reg |= ((SNAND_DATA_READ_MODE_X4 << SNAND_DATA_READ_MODE_OFFSET) & SNAND_DATA_READ_MODE_MASK);
    }

    DRV_WriteReg32(RW_SNAND_MISC_CTL, reg);

    //------ NFI Part ------

    mtk_snand_set_mode(CNFG_OP_CUST);
    NFI_SET_REG(NFI_CNFG_REG16, CNFG_READ_EN);
    *NFI_CON = sec_num << CON_NFI_SEC_SHIFT;

    DRV_WriteReg32(NFI_SPIDMA_REG32, 0);

#if 0
#ifndef DISABLE_NFI_DMA
    NFI_SET_REG(NFI_CNFG_REG16, CNFG_AHB);
   // NFI_SET_REG(NFI_CNFG_REG16, CNFG_AHB_BURST_EN);
#else
    NFI_CLN_REG(NFI_CNFG_REG16, CNFG_AHB);
#endif
#endif

    // use DMA anyway
    NFI_SET_REG(NFI_CNFG_REG16, CNFG_AHB);

    DRV_WriteReg32(NFI_STRADDR_REG32, buf);

    NFI_SET_REG(NFI_CNFG_REG16, CNFG_HW_ECC_EN);

    mtk_snand_set_autoformat(FALSE);    // read raw data

    mtk_snand_ecc_decode_start();

    bRet = TRUE;

  cleanup:
    return bRet;
}

//-----------------------------------------------------------------------------
static bool mtk_snand_ready_for_write_devecc(u32 page_addr, u32 sec_num, u8 * buf)
{
    bool bRet = FALSE;
    bool dma_mode;
    u32 reg;
    u32 page_size;
    u32 spare_size;
    SNAND_Mode mode = SPIQ;

    page_size = NUTL_PAGE_SIZE();
	spare_size = NUTL_SPARE_SIZE();

	#ifdef DISABLE_NFI_DMA
	dma_mode = TRUE;
	#else
	dma_mode = FALSE;
	#endif

	dma_mode = TRUE;    // Stanley Chu (use DMA mode anyway)

    if (!mtk_snand_reset_con())
    {
        return FALSE;
    }

    // 1. Write Enable
    mtk_snand_dev_command(g_pNandInfo->m_dev->m_cmd_set->m_write_enable.m_cmd, 1);

    //------ SNF Part ------

    // set SPI-NAND command
    if (SPI == mode)
    {
        reg = g_pNandInfo->m_dev->m_cmd_set->m_write_enable.m_cmd | (g_pNandInfo->m_dev->m_cmd_set->m_program.m_cmd << SNAND_PG_LOAD_CMD_OFFSET) | (g_pNandInfo->m_dev->m_cmd_set->m_program_cnf.m_cmd << SNAND_PG_EXE_CMD_OFFSET);
        DRV_WriteReg32(RW_SNAND_PG_CTL1, reg);
    }
    else if (SPIQ == mode)
    {
        reg = g_pNandInfo->m_dev->m_cmd_set->m_write_enable.m_cmd | (g_pNandInfo->m_dev->m_cmd_set->m_program.m_cmd << SNAND_PG_LOAD_CMD_OFFSET) | (g_pNandInfo->m_dev->m_cmd_set->m_program_cnf.m_cmd << SNAND_PG_EXE_CMD_OFFSET);
        DRV_WriteReg32(RW_SNAND_PG_CTL1, reg);
        mtk_snand_dev_enable_spiq(TRUE);
    }

    // set program load address
    DRV_WriteReg32(RW_SNAND_PG_CTL2, 0 & SNAND_PG_LOAD_ADDR_MASK);  // col_addr = 0

    // set program execution address
    DRV_WriteReg32(RW_SNAND_PG_CTL3, page_addr);

    // set SNF data length
    reg = (page_size + spare_size) |
          ((page_size + spare_size) << SNAND_PROGRAM_LOAD_BYTE_LEN_OFFSET);

    DRV_WriteReg32(RW_SNAND_MISC_CTL2, reg);

    // set SNF timing
    reg = DRV_Reg32(RW_SNAND_MISC_CTL);

    reg |= SNAND_PG_LOAD_CUSTOM_EN;

    if (SPI == mode)
    {
        reg &= ~SNAND_DATA_READ_MODE_MASK;
        reg |= ((SNAND_DATA_READ_MODE_X1 << SNAND_DATA_READ_MODE_OFFSET) & SNAND_DATA_READ_MODE_MASK);
        reg &=~ SNAND_PG_LOAD_X4_EN;
    }
    else if (SPIQ == mode)
    {
        reg &= ~SNAND_DATA_READ_MODE_MASK;
        reg |= ((SNAND_DATA_READ_MODE_X4 << SNAND_DATA_READ_MODE_OFFSET) & SNAND_DATA_READ_MODE_MASK);
        reg |= SNAND_PG_LOAD_X4_EN;
    }

    DRV_WriteReg32(RW_SNAND_MISC_CTL, reg);

    //------ NFI Part ------

    // reset NFI
    mtk_snand_reset_con();

    mtk_snand_set_mode(CNFG_OP_PRGM);

    NFI_CLN_REG(NFI_CNFG_REG16, CNFG_READ_EN);

    *NFI_CON = sec_num << CON_NFI_SEC_SHIFT;

    DRV_WriteReg32(NFI_SPIDMA_REG32, 0);

    if (TRUE == dma_mode)
    {
        NFI_SET_REG(NFI_CNFG_REG16, CNFG_AHB);
       // NFI_SET_REG(NFI_CNFG_REG16, CNFG_AHB_BURST_EN);
        DRV_WriteReg32(NFI_STRADDR_REG32, buf);
    }
    else
    {
        NFI_CLN_REG(NFI_CNFG_REG16, CNFG_AHB);
    }

    NFI_CLN_REG(NFI_CNFG_REG16, CNFG_HW_ECC_EN);
    mtk_snand_set_autoformat(FALSE);

    bRet = TRUE;
  cleanup:

    return bRet;
}

//-----------------------------------------------------------------------------
static bool mtk_snand_ready_for_write(u32 page_addr, u32 sec_num, u8 * buf, u8 raw)
{
    bool bRet = FALSE;
    bool dma_mode;
    u32 reg;
    u32 page_size;
    u32 spare_size;
    SNAND_Mode mode = SPIQ;

    page_size = NUTL_PAGE_SIZE();
	spare_size = NUTL_SPARE_SIZE();

	#ifdef DISABLE_NFI_DMA
	dma_mode = TRUE;
	#else
	dma_mode = FALSE;
	#endif

	dma_mode = TRUE;    // Stanley Chu (use DMA mode anyway)

    if (!mtk_snand_reset_con())
    {
        return FALSE;
    }

    // 1. Write Enable
    mtk_snand_dev_command(g_pNandInfo->m_dev->m_cmd_set->m_write_enable.m_cmd, 1);

    //------ SNF Part ------

    // set SPI-NAND command
    if (SPI == mode)
    {
        reg = g_pNandInfo->m_dev->m_cmd_set->m_write_enable.m_cmd | (g_pNandInfo->m_dev->m_cmd_set->m_program.m_cmd << SNAND_PG_LOAD_CMD_OFFSET) | (g_pNandInfo->m_dev->m_cmd_set->m_program_cnf.m_cmd << SNAND_PG_EXE_CMD_OFFSET);
        DRV_WriteReg32(RW_SNAND_PG_CTL1, reg);
    }
    else if (SPIQ == mode)
    {
        reg = g_pNandInfo->m_dev->m_cmd_set->m_write_enable.m_cmd | (g_pNandInfo->m_dev->m_cmd_set->m_program.m_cmd << SNAND_PG_LOAD_CMD_OFFSET) | (g_pNandInfo->m_dev->m_cmd_set->m_program_cnf.m_cmd << SNAND_PG_EXE_CMD_OFFSET);
        DRV_WriteReg32(RW_SNAND_PG_CTL1, reg);
        mtk_snand_dev_enable_spiq(TRUE);
    }

    // set program load address
    DRV_WriteReg32(RW_SNAND_PG_CTL2, 0 & SNAND_PG_LOAD_ADDR_MASK);  // col_addr = 0

    // set program execution address
    DRV_WriteReg32(RW_SNAND_PG_CTL3, page_addr);

    // set SNF data length
    if ((NUTL_ADDR_CYCLE() & SNAND_ADV_ECC_SPLIT) == 0)
    {
        reg = (page_size + spare_size) |
              ((page_size + spare_size) << SNAND_PROGRAM_LOAD_BYTE_LEN_OFFSET);
    }
    else
    {
        reg = (page_size + g_snand_ecc_spare_per_sector_now * page_size / NAND_SECTOR_SIZE) |
              ((page_size + g_snand_ecc_spare_per_sector_now * page_size / NAND_SECTOR_SIZE) << SNAND_PROGRAM_LOAD_BYTE_LEN_OFFSET);
    }

    DRV_WriteReg32(RW_SNAND_MISC_CTL2, reg);

    // set SNF timing
    reg = DRV_Reg32(RW_SNAND_MISC_CTL);

    reg |= SNAND_PG_LOAD_CUSTOM_EN;

    if (SPI == mode)
    {
        reg &= ~SNAND_DATA_READ_MODE_MASK;
        reg |= ((SNAND_DATA_READ_MODE_X1 << SNAND_DATA_READ_MODE_OFFSET) & SNAND_DATA_READ_MODE_MASK);
        reg &=~ SNAND_PG_LOAD_X4_EN;
    }
    else if (SPIQ == mode)
    {
        reg &= ~SNAND_DATA_READ_MODE_MASK;
        reg |= ((SNAND_DATA_READ_MODE_X4 << SNAND_DATA_READ_MODE_OFFSET) & SNAND_DATA_READ_MODE_MASK);
        reg |= SNAND_PG_LOAD_X4_EN;
    }

    DRV_WriteReg32(RW_SNAND_MISC_CTL, reg);

    //------ NFI Part ------

    // reset NFI
    mtk_snand_reset_con();

    mtk_snand_set_mode(CNFG_OP_PRGM);

    NFI_CLN_REG(NFI_CNFG_REG16, CNFG_READ_EN);

    *NFI_CON = sec_num << CON_NFI_SEC_SHIFT;

    DRV_WriteReg32(NFI_SPIDMA_REG32, 0);

    if (TRUE == dma_mode)
    {
        NFI_SET_REG(NFI_CNFG_REG16, CNFG_AHB);
       // NFI_SET_REG(NFI_CNFG_REG16, CNFG_AHB_BURST_EN);
        DRV_WriteReg32(NFI_STRADDR_REG32, buf);
    }
    else
    {
        NFI_CLN_REG(NFI_CNFG_REG16, CNFG_AHB);
    }

    if (TRUE == raw)    // raw data (2048|64)
    {
        NFI_CLN_REG(NFI_CNFG_REG16, CNFG_HW_ECC_EN);
        mtk_snand_set_autoformat(FALSE);
    }
    else
    {
        NFI_SET_REG(NFI_CNFG_REG16, CNFG_HW_ECC_EN);
        mtk_snand_set_autoformat(TRUE);
        mtk_snand_ecc_encode_start();
    }

    if (!mtk_snand_status_ready(STA_NAND_BUSY))
    {
        goto cleanup;
    }

    bRet = TRUE;
  cleanup:

    return bRet;
}

//-----------------------------------------------------------------------------
static bool mtk_snand_read_page_data_devecc(u32 num_sec, u32 * buf)
{
    u32 timeout;
    u32 u4Size = (num_sec * NAND_SECTOR_SIZE) + NUTL_SPARE_SIZE();  // read whole page
    u32 i;
    u32 *pBuf32;

    pBuf32 = (u32 *) buf;
    NFI_CLN_REG(NFI_CNFG_REG16, CNFG_BYTE_RW);

    // set dummy command to trigger NFI enter custom mode
    DRV_WriteReg16(NFI_CMD_REG16, NAND_CMD_DUMMYREAD);

    DRV_Reg16(NFI_INTR_REG16);  // read clear
    DRV_WriteReg16(NFI_INTR_EN_REG16, INTR_AHB_DONE_EN);

    NFI_SET_REG(NFI_CON, CON_NFI_BRD);

    timeout = 0xFFFF;

    while (!(DRV_Reg16(NFI_INTR_REG16) & INTR_AHB_DONE))    // for custom read, wait NFI's INTR_AHB_DONE done to ensure all data are transferred to buffer
    {
        timeout--;

        if (0 == timeout)
        {
            return FALSE;
        }
    }

    timeout = 0xFFFF;

    while ((u4Size >> 9) > ((*NFI_BYTELEN & 0xf000) >> 12))
    {
        timeout--;

        if (0 == timeout)
        {
            return FALSE;
        }
    }

    return TRUE;
}

//-----------------------------------------------------------------------------
static bool mtk_snand_read_page_data(u32 num_sec, u32 * buf)
{
    u32 timeout = 0xFFFF;
    u32 u4Size = (num_sec * NAND_SECTOR_SIZE) + NUTL_SPARE_SIZE();  // read whole page
    u32 i;
    u32 *pBuf32;

    pBuf32 = (u32 *) buf;
    NFI_CLN_REG(NFI_CNFG_REG16, CNFG_BYTE_RW);

    // set dummy command to trigger NFI enter custom mode
    DRV_WriteReg16(NFI_CMD_REG16, NAND_CMD_DUMMYREAD);

    DRV_Reg16(NFI_INTR_REG16);  // read clear
    DRV_WriteReg16(NFI_INTR_EN_REG16, INTR_AHB_DONE_EN);

    NFI_SET_REG(NFI_CON, CON_NFI_BRD);

    while (!(DRV_Reg16(NFI_INTR_REG16) & INTR_AHB_DONE))    // for custom read, wait NFI's INTR_AHB_DONE done to ensure all data are transferred to buffer
    {
        timeout--;

        if (0 == timeout)
        {
            #if defined(_SNAND_DEBUG)
            DBG_MSG("ERROR: mtk_snand_read_page_data timeout for INTR_AHB_DONE! u4Size: %d\n", u4Size);
            while (1);
            #endif

            return FALSE;
        }
    }

    timeout = 0xFFFF;

    while ((u4Size >> 9) > ((*NFI_BYTELEN & 0xf000) >> 12))
    {
        timeout--;

        if (0 == timeout)
        {
            #if defined(_SNAND_DEBUG)
            DBG_MSG("ERROR: mtk_snand_read_page_data timeout for NFI_BYTELEN! u4Size: %d\n", u4Size);
            while (1);
            #endif

            return FALSE;
        }
    }

    return TRUE;
}

//-----------------------------------------------------------------------------
static void mtk_snand_read_fdm_data(u32 u4SecNum, u8 * spare_buf)
{
    u32 i;
    u32 *pBuf32 = (u32 *) spare_buf;

    for (i = 0; i < u4SecNum; ++i)
    {
        //*pBuf32++ = DRV_Reg32(NFI_FDM0L_REG32 + (i << 3));
        //*pBuf32++ = DRV_Reg32(NFI_FDM0M_REG32 + (i << 3));

        *pBuf32++ = DRV_Reg32(NFI_FDM0L_REG32 + (i << 1));
        *pBuf32++ = DRV_Reg32(NFI_FDM0M_REG32 + (i << 1));
    }
}


//-----------------------------------------------------------------------------
// Option:
//          0: Source buffer has all continuous spare data for all sectors (16|16|16|16)
//          1: Source buffer has page data and spare data with format as (512|16|512|16)
//-----------------------------------------------------------------------------
static bool mtk_snand_write_fdm_data_devecc(u32 u4SecNum, u8 * dest, u8 * src, u32 option)
{
    u32 i, j, src_idx, src_len;
    u32 spare_size, spare_size_per_sec;

    spare_size = NUTL_SPARE_SIZE();
    spare_size_per_sec = NUTL_SPARE_PER_SEC_SIZE();

    if ((NUTL_ADDR_CYCLE() & SNAND_SPARE_FORMAT_1) != 0)
    {
        if (1 == option)    // convert spare format from (512|16|512|16) to 64 (continuous spare data)
        {
            memset((void *)g_snand_spare_buf_int, 0xFF, spare_size);

            src_idx = 0;

            for (i = 0; i < u4SecNum; i++)
            {
                //src_idx = i * spare_size_per_sec;

                for (j = 0; j < 8; i++)
                {
                    ((u8 *)g_snand_spare_buf_int)[src_idx] = src[((NAND_SECTOR_SIZE + spare_size_per_sec) * i) + j];

                    src_idx++;
                }
            }

            src = (u8 *)g_snand_spare_buf_int;
        }

        // convert from continuous spare data to DEVICE ECC's spare format

        memset(dest, 0xFF, spare_size);

        src_len = u4SecNum * 8;
        src_idx = 0;

        for (i = 0; g_snand_spare_format_1[i][0] != 0; i++)
        {
            for (j = 0; j < g_snand_spare_format_1[i][1]; j++)
            {

                dest[g_snand_spare_format_1[i][0] + j] = src[src_idx];

                src_idx++;

                if (src_idx == src_len)
                {
                    return TRUE;
                }
            }
        }
    }
    else
    {
        return FALSE;
    }
}

/*-----------------------------------------------------------------------------
 * SNAND_COMMON_PageRead_V3 only needs to return RAW data format (with AUTO_FMT off)
 *
 * Target format: SECTOR DATA | SPARE DATA OF THIS SECTOR | SECTOR DATA | SPARE DATA OF THIS SECTOR | ...
 * Example:       512|16|512|16 ...
 *
 * This API will transform the spare format from DEVICE ECC's format to required format
 *
 * Parameters:
 *  - dest: Destination buffer (starting from the beginning of target page)
 *  - src:  Source buffer (starting from the beginning of the source page)
 *-----------------------------------------------------------------------------*/
static bool mtk_snand_read_fdm_data_devecc(u32 u4SecNum, u8 * dest, u8 * src)
{
    u32 i, j, dest_idx, dest_len, cur_dest_sec;
    u32 page_size, spare_size, spare_size_per_sec;

    page_size = NUTL_PAGE_SIZE();
    spare_size = NUTL_SPARE_SIZE();
    spare_size_per_sec = NUTL_SPARE_PER_SEC_SIZE();

    if ((NUTL_ADDR_CYCLE() & SNAND_SPARE_FORMAT_1) != 0)
    {
        dest_len = u4SecNum * 8;
        dest_idx = 0;
        cur_dest_sec = 0;

        memset(dest + NAND_SECTOR_SIZE, 0xFF, spare_size_per_sec);  // reset the 1st sector's spare area

        for (i = 0; g_snand_spare_format_1[i][0] != 0; i++)
        {
            for (j = 0; j < g_snand_spare_format_1[i][1]; j++)
            {
                dest[((cur_dest_sec + 1) * NAND_SECTOR_SIZE + cur_dest_sec * spare_size_per_sec) + dest_idx] = src[page_size + (g_snand_spare_format_1[i][0] + j)];

                dest_idx++;

                if (8 == dest_idx)  // all spare bytes in this sector are copied, go to next sector
                {
                    cur_dest_sec++;

                    if (u4SecNum == cur_dest_sec)
                    {
                        return TRUE;
                    }
                    else
                    {
                        dest_idx = 0;

                        // reset new sector's spare area
                        memset(dest + ((cur_dest_sec + 1) * NAND_SECTOR_SIZE + cur_dest_sec * spare_size_per_sec), 0xFF, spare_size_per_sec);
                    }
                }
            }
        }
    }
    else
    {
        return FALSE;
    }
}

//-----------------------------------------------------------------------------
static void mtk_snand_write_fdm_data(u32 u4SecNum, u8 * oob)
{
    u32 i;
    u32 *pBuf32 = (u32 *) oob;

    for (i = 0; i < u4SecNum; ++i)
    {
        //DRV_WriteReg32(NFI_FDM0L_REG32 + (i << 3), *pBuf32++);
        //DRV_WriteReg32(NFI_FDM0M_REG32 + (i << 3), *pBuf32++);

        DRV_WriteReg32(NFI_FDM0L_REG32 + (i << 1), *pBuf32++);
        DRV_WriteReg32(NFI_FDM0M_REG32 + (i << 1), *pBuf32++);
    }
}

//-----------------------------------------------------------------------------
static bool mtk_snand_write_page_data(u32 * buf)
{
    u32 timeout = 0xFFFF;
    u32 u4Size = NUTL_PAGE_SIZE();

    NFI_CLN_REG(NFI_CNFG_REG16, CNFG_BYTE_RW);

    // set dummy command to trigger NFI enter custom mode
    DRV_WriteReg16(NFI_CMD_REG16, NAND_CMD_DUMMYPROG);

    DRV_Reg16(NFI_INTR_REG16);  // read clear
    DRV_WriteReg16(NFI_INTR_EN_REG16, INTR_CUSTOM_PROG_DONE_INTR_EN);

    NFI_SET_REG(NFI_CON, CON_NFI_BWR);

    while (!(DRV_Reg32(RW_SNAND_STA_CTL1) & SNAND_CUSTOM_PROGRAM))  // for custom program, wait RW_SNAND_STA_CTL1's SNAND_CUSTOM_PROGRAM done to ensure all data are loaded to device buffer
    {
        timeout--;

        if (0 == timeout)
        {
            return FALSE;
        }
    }

    return TRUE;
}

//---------------------------------------------------------------------------
static bool mtk_snand_check_RW_count(u16 u2WriteSize)
{
    u32 timeout = 0xFFFF;
    u16 u2SecNum = u2WriteSize >> 9;

    while (ADDRCNTR_CNTR(*NFI_ADDRCNTR) < u2SecNum)
    {
        timeout--;
        if (0 == timeout)
        {
            return FALSE;
        }
    }

    return TRUE;
}

//-----------------------------------------------------------------------------
static void mtk_snand_stop_write(void)
{
    //------ NFI Part

    NFI_CLN_REG(NFI_CON, CON_NFI_BWR);

    //------ SNF Part

    // set 1 then set 0 to clear done flag
    DRV_WriteReg32(RW_SNAND_STA_CTL1, SNAND_CUSTOM_PROGRAM);
    DRV_WriteReg32(RW_SNAND_STA_CTL1, 0);

    // clear essential SNF setting
    NFI_CLN_REG(RW_SNAND_MISC_CTL, SNAND_PG_LOAD_CUSTOM_EN);

    mtk_snand_dev_enable_spiq(FALSE);

    mtk_snand_ecc_encode_end(); // for raw data, it is OK to call this
}

static bool mtk_snand_dev_program_execute(u32 page)
{
    u32 cmd;
    bool bRet = TRUE;

    // 3. Program Execute
    cmd = mtk_snand_gen_c1a3(g_pNandInfo->m_dev->m_cmd_set->m_program_cnf.m_cmd, page);

    mtk_snand_dev_command(cmd, 4);

    // 4. Status Polling
    cmd = g_pNandInfo->m_dev->m_cmd_set->m_get_feature.m_cmd | (SNAND_CMD_FEATURES_STATUS << 8);

    DRV_WriteReg32(RW_SNAND_GPRAM_DATA, cmd);
    DRV_WriteReg32(RW_SNAND_MAC_OUTL, 2);
    DRV_WriteReg32(RW_SNAND_MAC_INL , 1);

    while (1)
    {
        mtk_snand_dev_mac_op(SPI);

        cmd = DRV_Reg8(((P_U8)RW_SNAND_GPRAM_DATA + 2));

        if ((cmd & SNAND_STATUS_PROGRAM_FAIL) != 0)
        {
            bRet = FALSE;
        }

        if ((cmd & SNAND_STATUS_OIP) == 0)
        {
            break;
        }
    }

    return bRet;
}

void mtk_snand_dev_ecc_control(uint8 enable)
{
    u32 cmd;
    u8  otp;
    u8  otp_new;

    // read original otp settings

    cmd = SNAND_CMD_GET_FEATURES | (SNAND_CMD_FEATURES_OTP << 8);
    DRV_WriteReg32(RW_SNAND_GPRAM_DATA, cmd);
    DRV_WriteReg32(RW_SNAND_MAC_OUTL, 2);
    DRV_WriteReg32(RW_SNAND_MAC_INL , 1);

    mtk_snand_dev_mac_op(SPI);

    otp = DRV_Reg8(((P_U8)RW_SNAND_GPRAM_DATA + 2));

    if (enable == TRUE)
    {
        otp_new = otp | SNAND_OTP_ECC_ENABLE;
    }
    else
    {
        otp_new = otp & ~SNAND_OTP_ECC_ENABLE;
    }

    if (otp != otp_new)
    {
        // write enable

        mtk_snand_dev_command(SNAND_CMD_WRITE_ENABLE, 1);


        // set features
        cmd = SNAND_CMD_SET_FEATURES | (SNAND_CMD_FEATURES_OTP << 8) | (otp_new << 16);

        mtk_snand_dev_command(cmd, 3);
    }

    if (enable == TRUE) g_snand_dbg_dev_ecc = 1;
    else g_snand_dbg_dev_ecc = 0;
}

void mtk_snand_dev_turn_off_bbi()
{
    u32 cmd;
    u8  reg;
    u8  reg_new;

    // read original block lock settings
    cmd = SNAND_CMD_GET_FEATURES | (SNAND_CMD_FEATURES_OTP << 8);
    DRV_WriteReg32(RW_SNAND_GPRAM_DATA, cmd);
    DRV_WriteReg32(RW_SNAND_MAC_OUTL, 2);
    DRV_WriteReg32(RW_SNAND_MAC_INL , 1);

    mtk_snand_dev_mac_op(SPI);

    reg = DRV_Reg8(((P_U8)RW_SNAND_GPRAM_DATA + 2));

    reg_new = reg & ~SNAND_OTP_BBI;

    if (reg != reg_new)
    {
        // write enable
        mtk_snand_dev_command(SNAND_CMD_WRITE_ENABLE, 1);

        // set features
        cmd = SNAND_CMD_SET_FEATURES | (SNAND_CMD_FEATURES_OTP << 8) | (reg_new << 16);
        mtk_snand_dev_command(cmd, 3);
    }
}

void mtk_snand_dev_unlock_all_blocks()
{
    u32 cmd;
    u8  lock;
    u8  lock_new;

    // read original block lock settings
    cmd = SNAND_CMD_GET_FEATURES | (SNAND_CMD_FEATURES_BLOCK_LOCK << 8);
    DRV_WriteReg32(RW_SNAND_GPRAM_DATA, cmd);
    DRV_WriteReg32(RW_SNAND_MAC_OUTL, 2);
    DRV_WriteReg32(RW_SNAND_MAC_INL , 1);

    mtk_snand_dev_mac_op(SPI);

    lock = DRV_Reg8(((P_U8)RW_SNAND_GPRAM_DATA + 2));

    lock_new = lock & ~SNAND_BLOCK_LOCK_BITS;

    if (lock != lock_new)
    {
        // write enable
        mtk_snand_dev_command(SNAND_CMD_WRITE_ENABLE, 1);

        // set features
        cmd = SNAND_CMD_SET_FEATURES | (SNAND_CMD_FEATURES_BLOCK_LOCK << 8) | (lock_new << 16);
        mtk_snand_dev_command(cmd, 3);
    }
}

//------------------------------------------------------------------------------
// Read Device ID Callback Function
//------------------------------------------------------------------------------
STATUS_E  SNAND_COMMON_ReadID_V3(
                const u32  c_timeout
                ,u16 *p_maker_code
                ,u16 *p_device_code
                ,u16 *p_ext_code1
                ,u16 *p_ext_code2
				,u16 *p_ext_code3 //add by jiequn,2011.11.11
) {
    u16         reg_val;
    //u8          cmd = g_pNandInfo->m_dev->m_cmd_set->m_read_id.m_cmd;	// FIXME: Stanley Chu
    u8          cmd = SNAND_CMD_READ_ID;
    u8          id[SNAND_MAX_ID + 1];
    STATUS_E    ret = S_DONE;


    /* Reset NFI state machine */
    mtk_snand_reset_con();
    mtk_snand_reset_dev();

    mtk_snand_dev_command_ext(SPI, &cmd, id, 1, SNAND_MAX_ID + 1);

    // SPI-NAND only has 2 byte ID
    *p_maker_code = id[1];
    *p_device_code = id[2];
    *p_ext_code1 = 0;
    *p_ext_code2 = 0;
	*p_ext_code3 = 0;

    ret = S_DONE;

    return ret;
}

//------------------------------------------------------------------------------
// Reset Device Callback Function
//------------------------------------------------------------------------------
STATUS_E  SNAND_COMMON_Reset_V3(
				const u32  c_timeout
) {
	// reset NFI
	mtk_snand_reset_con();

	// reset device
    mtk_snand_reset_dev();

	return S_DONE;
}

//------------------------------------------------------------------------------
// Read Status Callback Function
//------------------------------------------------------------------------------
STATUS_E  SNAND_COMMON_ReadStatus_V3(
				const u32  c_timeout
) {
    if (DRV_Reg16(NFI_MASTERSTA_REG16)) // master is busy
    {
        return S_DA_IN_PROGRESS;
    }
    else
    {
        return S_DONE;
    }
}

//------------------------------------------------------------------------------
// Block Erase Related Callback Function
//------------------------------------------------------------------------------
STATUS_E  SNAND_COMMON_BlockErase_V3(
				const u32  row_addr
) {
    u32 reg;
    STATUS_E ret = S_DONE;

#ifdef DUMP_SPEED
    u32 tick_start = gpt4_get_current_tick(), tick_end;
#endif

    if (row_addr > NUTL_TOTAL_PAGE_COUNT())
    {
        return S_DA_INVALID_RANGE;
    }

    if (TRUE == mtk_snand_is_invalid_page(row_addr))
    {
        return S_DA_NAND_ERASE_FAILED;
    }

	// reset NFI
	mtk_snand_reset_con();

    // erase address
    DRV_WriteReg32(RW_SNAND_ER_CTL2, row_addr);

    // set loop limit and polling cycles
    reg = (SNAND_LOOP_LIMIT_NO_LIMIT << SNAND_LOOP_LIMIT_OFFSET) | 0x20;
    DRV_WriteReg32(RW_SNAND_GF_CTL3, reg);

    // set latch latency & CS de-select latency (ignored)

    // set erase command
    reg = g_pNandInfo->m_dev->m_cmd_set->m_erase.m_cmd << SNAND_ER_CMD_OFFSET;
    DRV_WriteReg32(RW_SNAND_ER_CTL, reg);

    // trigger interrupt waiting
    reg = DRV_Reg16(NFI_INTR_EN_REG16);
    reg = INTR_AUTO_BLKER_INTR_EN;
    DRV_WriteReg16(NFI_INTR_EN_REG16, reg);

    // trigger auto erase
    reg = DRV_Reg32(RW_SNAND_ER_CTL);
    reg |= SNAND_AUTO_ERASE_TRIGGER;
    DRV_WriteReg32(RW_SNAND_ER_CTL, reg);

    // wait for auto erase finish
    for (;;)
    {
        reg = DRV_Reg32(RW_SNAND_STA_CTL1);

        if ((reg & SNAND_AUTO_BLKER) == 0)
        {
            reg = DRV_Reg32(RW_SNAND_GF_CTL1);
            reg &= SNAND_GF_STATUS_MASK;

            continue;
        }
        else
        {
            // set 1 then set 0 to clear done flag
            DRV_WriteReg32(RW_SNAND_STA_CTL1, reg);
            reg = reg & ~SNAND_AUTO_BLKER;
            DRV_WriteReg32(RW_SNAND_STA_CTL1, reg);

            // clear trigger bit
            reg = DRV_Reg32(RW_SNAND_ER_CTL);
            reg = reg & ~SNAND_AUTO_ERASE_TRIGGER;
            DRV_WriteReg32(RW_SNAND_ER_CTL, reg);

            reg = DRV_Reg32(RW_SNAND_GF_CTL1);
            reg &= SNAND_GF_STATUS_MASK;

            break;
        }
    }

    // check get feature status
    reg = *RW_SNAND_GF_CTL1 & SNAND_GF_STATUS_MASK;

    if (0 != (reg & SNAND_STATUS_ERASE_FAIL))
    {
        ret = S_DA_NAND_ERASE_FAILED;
    }

#ifdef DUMP_SPEED
    g_ds_nand_block_erase_number++;
    tick_end = gpt4_get_current_tick();
    g_ds_nand_block_erase_time += (tick_end - tick_start);
#endif

	return ret;
}

//------------------------------------------------------------------------------
// Bad Block Symbol Identification Related Callback Function
//------------------------------------------------------------------------------
STATUS_E  SNAND_COMMON_BadBlockSymbol_Check_V3(
				const u32  *p_spare32 /* MUST be 32bits alignment addr */
) {
	u32	page_size;
	u32	spare_size;
	const u8    *p_spare8 = (const u8 *)p_spare32;
	const u16   *p_spare16 = (const u16 *)p_spare32;

	page_size = NUTL_PAGE_SIZE();
	spare_size = NUTL_SPARE_SIZE();

	// check the invalid block status from spare area
	if( 512 < page_size ) {
		// for 2048 page size flash, the invalid block status is defined by the 1st byte/word in spare area
		if( 0xFF != p_spare8[0] ) {
			return S_DA_NAND_BAD_BLOCK;
		}
	}
	else {
		// for 512 page size flash
		// for 8 bits I/O, the invalid block status is defined by the 6th byte in spare area
		if( 0xFF != p_spare8[5] ) {
			return S_DA_NAND_BAD_BLOCK;
        }
	}

	return S_DONE;
}

STATUS_E  SNAND_COMMON_BadBlockSymbol_Set_V3(
				u32  *p_spare32 /* MUST be 32bits alignment addr */
) {
	u32	page_size;
	u32	spare_size;
	u32	io_interface;
	u8	*p_spare8 = (u8 *)p_spare32;
	u16	*p_spare16 = (u16 *)p_spare32;
	u32	i;

	page_size = NUTL_PAGE_SIZE();
	spare_size = NUTL_SPARE_SIZE();
	io_interface = NUTL_IO_INTERFACE();

	// reset spare
	for(i=0; i<(spare_size>>2); i++) {
		p_spare32[i] = 0xAAAAAAAA;
	}

	// check the invalid block status from spare area
	if( 512 < page_size ) {
		// for 2048 page size flash, the invalid block status is defined by the 1st byte/word in spare area
		if( NAND_IO_16BITS == io_interface ) {
			p_spare16[0] = 0x0;
		}
		else {
			p_spare8[0] = 0x0;
		}
	}
	else {
		// for 512 page size flash
		if( NAND_IO_16BITS == io_interface ) {
			// for 16 bits I/O, the invalid block status is defined by the 1st word in spare area
			p_spare16[0] = 0x0;
		}
		else {
			// for 8 bits I/O, the invalid block status is defined by the 6th byte in spare area
			p_spare8[5] = 0x0;
		}
	}

	return S_DONE;
}

//------------------------------------------------------------------------------
// Page Read Callback Function
//------------------------------------------------------------------------------
STATUS_E  SNAND_COMMON_PageRead_V3(
				const u32  c_timeout
				,const u32  row_addr
				,u32 *p_data32 /* MUST be 32bits alignment addr */
				,u32  ecc_parity_from_reg[8]
) {
    u32         cmd;
    u32         i, j;
	u32		    sectors;
	u32         spare_per_sector;
	u32         page_size;
	u32       * p_data32_int = p_data32;
	u32         retry;
	STATUS_E	ret = S_DONE;
	SNAND_Mode  mode = SPIQ;
#ifdef DUMP_SPEED
    u32 tick_start = gpt4_get_current_tick(), tick_end;
#endif

    if (row_addr > NUTL_TOTAL_PAGE_COUNT())
    {
        return S_DA_INVALID_RANGE;
    }

    /*if (TRUE == mtk_snand_is_invalid_page(row_addr))	// reserve block is allowed to be read
    {
        return S_DA_NAND_UNKNOWN_ERR;
    }
    */

    spare_per_sector = NUTL_SPARE_PER_SEC_SIZE();

    mtk_snand_ecc_config_dynamic(row_addr);

	sectors = NUTL_PAGE_SIZE() / NAND_SECTOR_SIZE;

	mtk_snand_reset_con();

    if (0 == g_snand_ecc_bit_now)
    {
        p_data32_int = &g_snand_data_buf_int[0];    // use internal buffer for device ECC region
    }
    else if ((NUTL_ADDR_CYCLE() & SNAND_ADV_ECC_SPLIT) != 0)    // 3+1, use internal buffer to read a whole page
    {
        if ((SNAND_SPLIT_REGION1_ECC_BIT != g_snand_ecc_bit_now) &&
            (g_snand_ecc_bit_now > SNAND_SPLIT_ECC_BIT_THRESHOLD)) // only 12-bit region needs 3+1
        {
            sectors--;

            p_data32_int = &g_snand_data_buf_int[0];    // both 4-bit and 12-bit need internal buffer (4 bit: for format transfrom, 12-bit: for 3+1 read)
        }
    }

    #if defined(_SNAND_DEBUG)
    DBG_MSG("R  (r: %d, b: %d, ecc: %d)\n", row_addr, row_addr / 64, g_snand_ecc_bit_now);
    #endif

    retry = 0;

snand_common_pageread_retry:

    if (0 != g_snand_ecc_bit_now)
    {
        if (mtk_snand_ready_for_read(row_addr, sectors, (u8 *)p_data32_int))    // read with MTK ECC enabled
        {
            if (!mtk_snand_read_page_data(sectors, p_data32_int))
            {
                ret = S_DA_NAND_UNKNOWN_ERR;

                #if defined(_SNAND_DEBUG)
                DBG_MSG("ERROR: read failed! Pos: 1, sectors: %d\n", sectors);
                while (1);
                #endif
            }

            if (!mtk_snand_check_dececc_done(sectors))
            {
                ret = S_DA_NAND_UNKNOWN_ERR;

                #if defined(_SNAND_DEBUG)
                DBG_MSG("ERROR: read failed! Pos: 2, sectors: %d\n", sectors);
                while (1);
                #endif
            }

            // !REMARKS! SNAND_COMMON_PageRead_V3 only needs to return RAW data format (with AUTO_FMT off)

            //mtk_snand_read_fdm_data(sectors, (u8 *)(p_data32 + (page_size / sizeof(32))));

            mtk_snand_stop_read();
        }
    }
    else    // region with device ECC used
    {
        if (mtk_snand_ready_for_read_devecc(row_addr, sectors, (u8 *)p_data32_int))    // read with device ECC enabled
        {
            if (!mtk_snand_read_page_data_devecc(sectors, p_data32_int))
            {
                ret = S_DA_NAND_UNKNOWN_ERR;

                #if defined(_SNAND_DEBUG)
                DBG_MSG("ERROR: read failed! Pos: 3, sectors: %d\n", sectors);
                while (1);
                #endif
            }

            mtk_snand_stop_read_devecc();
        }
    }

    if (S_DONE != ret)
    {
        if (retry <= 3)
        {
            retry++;

            DBG_MSG("[snand] read retrying the %d time ... row:%d\n", retry, row_addr);

            while (1);  // [Stanley Chu] debug

            mtk_snand_reset_dev();

            goto snand_common_pageread_retry;
        }
    }

    if ((NUTL_ADDR_CYCLE() & SNAND_ADV_ECC_SPLIT) != 0)
    {
        if ((SNAND_SPLIT_REGION1_ECC_BIT != g_snand_ecc_bit_now) &&
            (g_snand_ecc_bit_now > SNAND_SPLIT_ECC_BIT_THRESHOLD)) // for 12 bit, read part 2
        {
            sectors++;

            mtk_snand_split_read_part2(row_addr, sectors, 0, (u8 *)p_data32_int);
        }
    }

    if (p_data32_int != p_data32)
    {
        if (0 != g_snand_ecc_bit_now)
        {
            memset(p_data32, 0xFF, NUTL_PAGE_SIZE() + NUTL_SPARE_SIZE()); // dummy, need to remove

            for (i = 0; i < sectors; i++)
            {
                for (j = 0; j < (NAND_SECTOR_SIZE + spare_per_sector) / sizeof(u32); j++)
                {
                    p_data32[(i * (NAND_SECTOR_SIZE + spare_per_sector) / sizeof(u32)) + j] = p_data32_int[(i * (NAND_SECTOR_SIZE + g_snand_ecc_spare_per_sector_now) / sizeof(u32)) + j];
                }
            }
        }
        else    // device ECC is used, need to re-org data and spare formt
        {
            /*
             * SNAND_COMMON_PageRead_V3 only needs to return RAW data format (with AUTO_FMT off)
             *
             * Target format: SECTOR DATA | SPARE DATA OF THIS SECTOR | SECTOR DATA | SPARE DATA OF THIS SECTOR | ...
             * Example:       512|16|512|16 ...
             */

            page_size = NUTL_PAGE_SIZE();

            // copy data
            for (i = 0; i < sectors; i++)
            {
                memcpy((void *)(((u8 *)p_data32) + i * (NAND_SECTOR_SIZE + spare_per_sector)) , (void *)(((u8 *)p_data32_int) + i * NAND_SECTOR_SIZE), NAND_SECTOR_SIZE);
            }

            // copy spare
            mtk_snand_read_fdm_data_devecc(sectors, (u8 *)p_data32, (u8 *)p_data32_int);
        }
    }

    mtk_snand_dev_enable_spiq(FALSE);

#ifdef DUMP_SPEED
    g_ds_nand_page_read_number++;
    tick_end = gpt4_get_current_tick();
    g_ds_nand_page_read_time += (tick_end - tick_start);
#endif

	return ret;
}

//------------------------------------------------------------------------------
// Page Program Callback Function
//------------------------------------------------------------------------------
STATUS_E  SNAND_COMMON_PageProgram_V3(
				const u32  c_timeout
				,const u32  row_addr
				,const u32 *p_data32 /* MUST be 32bits alignment addr */
				,u32  ecc_parity_from_reg[8]
) {
	u32		    page_size;
	u32		    spare_size;
	u32		    sectors;
	bool        ret_bool;
	STATUS_E	ret = S_DONE;
	u32       * p_data32_int = (u32 *)p_data32;
#ifdef DUMP_SPEED
    u32 tick_start = gpt4_get_current_tick(), tick_end;
#endif

    if (row_addr > NUTL_TOTAL_PAGE_COUNT())
    {
        return S_DA_INVALID_RANGE;
    }

    if (TRUE == mtk_snand_is_invalid_page(row_addr))
    {
        return S_DA_NAND_PAGE_PROGRAM_FAILED;
    }

    mtk_snand_ecc_config_dynamic(row_addr);

    // Given data format is 2048|64 (spare data is continuos!). Here we use AUTO_FMT to write data to SPI-NAND as 512|16|512|16|512|16|512|16

	page_size = NUTL_PAGE_SIZE();
	spare_size = NUTL_SPARE_SIZE();
	sectors = NUTL_PAGE_SIZE()/NAND_SECTOR_SIZE;

	mtk_snand_reset_con();

    if (0 != g_snand_ecc_bit_now)
    {
        if (mtk_snand_ready_for_write(row_addr, sectors, (u8 *)p_data32, FALSE))
        {
            mtk_snand_write_fdm_data(sectors, (u8 *)(p_data32 + (page_size / sizeof(32))));

            if (!mtk_snand_write_page_data((u32 *)p_data32))
            {
                ret = S_DA_TIMEOUT;
            }

            if (!mtk_snand_check_RW_count(page_size))
            {
                ret = S_DA_TIMEOUT;
            }

            mtk_snand_stop_write();

            ret_bool = mtk_snand_dev_program_execute(row_addr);

            if (FALSE == ret_bool)
            {
                ret = S_DA_NAND_PAGE_PROGRAM_FAILED;
            }
        }
    }
    else
    {
        while (1)
        {
            p_data32_int = &g_snand_data_buf_int[0];    // use internal buffer for device ECC region

            ret_bool = mtk_snand_write_fdm_data_devecc(sectors, (u8 *)(p_data32_int + (page_size / sizeof(32))), (u8 *)(p_data32 + (page_size / sizeof(32))), 0);

            if (FALSE == ret_bool)
            {
                ret = S_DA_NAND_PAGE_PROGRAM_FAILED;
                break;
            }

            memcpy((void *)p_data32_int, (void *)p_data32, page_size);

            if (mtk_snand_ready_for_write_devecc(row_addr, sectors, (u8 *)p_data32_int))
            {
                if (!mtk_snand_write_page_data((u32 *)p_data32_int))
                {
                    ret = S_DA_TIMEOUT;
                }

                if (!mtk_snand_check_RW_count(page_size))
                {
                    ret = S_DA_TIMEOUT;
                }

                mtk_snand_stop_write();

                ret_bool = mtk_snand_dev_program_execute(row_addr);

                if (FALSE == ret_bool)
                {
                    ret = S_DA_NAND_PAGE_PROGRAM_FAILED;
                }
            }

            break;
        }
    }

#ifdef DUMP_SPEED
    g_ds_nand_page_program_number++;
    tick_end = gpt4_get_current_tick();
    g_ds_nand_page_program_time += (tick_end - tick_start);
#endif

	return ret;
}

//------------------------------------------------------------------------------
// Page+Spare Program Callback Function
//------------------------------------------------------------------------------
STATUS_E SNAND_COMMON_PageSpareProgram_V3(u32 row_addr,
                                         const u32 *p_page_spare)
{
	u32		    page_size;
	u32		    i, j, spare_per_sector;
	u32		    sectors;
	u32       * p_page_spare_int = (u32 *)p_page_spare;
	bool        ret_bool;
	STATUS_E	ret = S_DONE;
#ifdef DUMP_SPEED
    u32 tick_start = gpt4_get_current_tick(), tick_end;
#endif

    if (row_addr > NUTL_TOTAL_PAGE_COUNT())
    {
        return S_DA_INVALID_RANGE;
    }

    if (TRUE == mtk_snand_is_invalid_page(row_addr))
    {
        return S_DA_NAND_PAGE_PROGRAM_FAILED;
    }

    mtk_snand_ecc_config_dynamic(row_addr);

    // Given data format is already 512|spare0|512|spare1|512|spare2|512|spare3. Keep the same format (turn-off AUTO_FMT) and write them into SPI-NAND

	page_size = NUTL_PAGE_SIZE();
	spare_per_sector = NUTL_SPARE_PER_SEC_SIZE();
	sectors = NUTL_PAGE_SIZE() / NAND_SECTOR_SIZE;

	if (((NUTL_ADDR_CYCLE() & SNAND_ADV_ECC_SPLIT) != 0) &&
	    (SNAND_SPLIT_REGION1_ECC_BIT == g_snand_ecc_bit_now))   // for ECC 4-bit region program, transform buffer contents from ECC12-bit to ECC4-bit
	{
	    if (0 != g_snand_ecc_bit_now)   // MTK ECC is used (for Device ECC case, it will be processed below)
	    {
    	    p_page_spare_int = &g_snand_data_buf_int[0];

    	    memset(p_page_spare_int, 0xFF, SNAND_DATA_BUF_INT_SIZE);

    	    for (i = 0; i < sectors; i++)
            {
                for (j = 0; j < (NAND_SECTOR_SIZE + spare_per_sector) / sizeof(u32); j++)
                {
                    p_page_spare_int[(i * (NAND_SECTOR_SIZE + g_snand_ecc_spare_per_sector_now) / sizeof(u32)) + j] = p_page_spare[(i * (NAND_SECTOR_SIZE + spare_per_sector) / sizeof(u32)) + j];
                }
            }
        }
	}

	mtk_snand_reset_con();

    if (0 != g_snand_ecc_bit_now)
    {
        if (mtk_snand_ready_for_write(row_addr, sectors, (u8 *)p_page_spare_int, TRUE)) // set raw = TRUE
        {
            if (!mtk_snand_write_page_data((u32 *)p_page_spare_int))
            {
                ret = S_DA_TIMEOUT;
            }

            if (!mtk_snand_check_RW_count(page_size))
            {
                ret = S_DA_TIMEOUT;
            }

            mtk_snand_stop_write();

            ret_bool =  mtk_snand_dev_program_execute(row_addr);

            if (FALSE == ret_bool)
            {
                ret = S_DA_NAND_PAGE_PROGRAM_FAILED;
            }
        }
    }
    else
    {
        while (1)
        {
            p_page_spare_int = &g_snand_data_buf_int[0];

            memset(p_page_spare_int, 0xFF, SNAND_DATA_BUF_INT_SIZE);

            ret_bool = mtk_snand_write_fdm_data_devecc(sectors, (u8 *)(p_page_spare_int + (page_size / sizeof(32))), (u8 *)(p_page_spare + (page_size / sizeof(32))), 1);

            if (FALSE == ret_bool)
            {
                ret = S_DA_NAND_PAGE_PROGRAM_FAILED;

                break;
            }

            for (i = 0; i < sectors; i++)
            {
                memcpy((void *)(p_page_spare_int + ((i * NAND_SECTOR_SIZE) / sizeof(u32))), (void *)(p_page_spare + (i * (NAND_SECTOR_SIZE + spare_per_sector) / sizeof(u32))), NAND_SECTOR_SIZE);
            }

            if (mtk_snand_ready_for_write_devecc(row_addr, sectors, (u8 *)p_page_spare_int))
            {
                if (!mtk_snand_write_page_data((u32 *)p_page_spare_int))
                {
                    ret = S_DA_TIMEOUT;
                }

                if (!mtk_snand_check_RW_count(page_size))
                {
                    ret = S_DA_TIMEOUT;
                }

                mtk_snand_stop_write();

                ret_bool = mtk_snand_dev_program_execute(row_addr);

                if (FALSE == ret_bool)
                {
                    ret = S_DA_NAND_PAGE_PROGRAM_FAILED;
                }
            }

            break;
        }
    }

#ifdef DUMP_SPEED
    g_ds_nand_page_program_number++;
    tick_end = gpt4_get_current_tick();
    g_ds_nand_page_program_time += (tick_end - tick_start);
#endif

	return ret;
}

//------------------------------------------------------------------------------
// Spare Read Callback Function
//------------------------------------------------------------------------------
STATUS_E  SNAND_COMMON_SpareRead_V3(
				const u32  c_timeout
				,const u32  row_addr
				,u32 *p_spare32 /* MUST be 32bits alignment addr */
) {
	u32		page_size;
	u32		spare_size;
	u32		column_addr_bits;

    if (row_addr > NUTL_TOTAL_PAGE_COUNT())
    {
        return S_DA_INVALID_RANGE;
    }

    if (TRUE == mtk_snand_is_invalid_page(row_addr))
    {
        return S_DA_NAND_UNKNOWN_ERR;
    }

	page_size = NUTL_PAGE_SIZE();
	spare_size = NUTL_SPARE_SIZE();

	// reset NFI
	mtk_snand_reset_con();

	return S_DONE;
}

//------------------------------------------------------------------------------
// Spare Program Callback Function
//------------------------------------------------------------------------------
STATUS_E  SNAND_COMMON_SpareProgram_V3(
				const u32  c_timeout
				,const u32  row_addr
				,const u32 *p_spare32 /* MUST be 32bits alignment addr */
) {
	u32		page_size;
//	u32		spare_addr;
	u32		spare_size;
	u32		column_addr_bits;

    if (row_addr > NUTL_TOTAL_PAGE_COUNT())
    {
        return S_DA_INVALID_RANGE;
    }

    if (TRUE == mtk_snand_is_invalid_page(row_addr))
    {
        return S_DA_NAND_PAGE_PROGRAM_FAILED;
    }

	page_size = NUTL_PAGE_SIZE();
	spare_size = NUTL_SPARE_SIZE();

	// reset NFI
	mtk_snand_reset_con();

	return S_DONE;
}

//------------------------------------------------------------------------------
// COMMON Callback Function Set
//------------------------------------------------------------------------------
const NAND_CMD_Callback_S	g_SNAND_COMMON_CB_FUNC_SET={
	ID_COMMON_CB_FUNC_SET
	,SNAND_COMMON_ReadID_V3
	,SNAND_COMMON_Reset_V3
	,NULL
	,SNAND_COMMON_ReadStatus_V3
	,SNAND_COMMON_BlockErase_V3
	,SNAND_COMMON_BadBlockSymbol_Check_V3
	,SNAND_COMMON_BadBlockSymbol_Set_V3
	,SNAND_COMMON_PageRead_V3
	,SNAND_COMMON_PageProgram_V3
	,SNAND_COMMON_SpareRead_V3
	,SNAND_COMMON_SpareProgram_V3
	,SNAND_COMMON_PageSpareProgram_V3
};

#endif
#endif

