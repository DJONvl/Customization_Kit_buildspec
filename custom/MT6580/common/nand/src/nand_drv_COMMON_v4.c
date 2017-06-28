/*******************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of MediaTek Inc. (C) 2005
*
*******************************************************************************/

#include "NFI.h"

#ifdef __MTK_NFI_V4__

#include "nand_drv_COMMON_v4.h"
#include "nand_util_func.h"

#define DRV_WriteReg(addr,data)     ((*(volatile uint32 *)(addr)) = (uint32)data)
#define DRV_Reg(addr)               (*(volatile uint32 *)(addr))
#define DRV_WriteReg32(addr,data)     ((*(volatile uint32 *)(addr)) = (uint32)data)
#define DRV_Reg32(addr)               (*(volatile uint32 *)(addr))
#define DRV_WriteReg8(addr,data)     ((*(volatile uint8 *)(addr)) = (uint8)data)
#define DRV_Reg8(addr)               (*(volatile uint8 *)(addr))
//------------------------------------------------------------------------------
// Device Spec. Reference Macro
//------------------------------------------------------------------------------
#define MTD_NAND_MAKER_CODE                 (g_pNandInfo->m_dev->m_hw_info.m_id.m_maker_code)
#define MTD_NAND_DEVICE_CODE                (g_pNandInfo->m_dev->m_hw_info.m_id.m_device_code)
#define MTD_NAND_CHIP_COUNT                 ((g_pNandInfo->m_dev->m_hw_info.m_chip_num==0)?1: g_pNandInfo->m_dev->m_hw_info.m_chip_num)
#define MTD_NAND_TOTAL_SIZE                 (((U64)(g_pNandInfo->m_dev->m_hw_info.m_total_size_in_mb))<<20)
#define MTD_NAND_SPARE_SIZE                 ((MTD_NAND_CHIP_COUNT==0)?g_pNandInfo->m_spare_size:(g_pNandInfo->m_spare_size*MTD_NAND_CHIP_COUNT))
#define MTD_NAND_BLOCK_SIZE                 ((MTD_NAND_CHIP_COUNT==0)?g_pNandInfo->m_block_size: (g_pNandInfo->m_block_size*MTD_NAND_CHIP_COUNT))
#define MTD_NAND_PAGE_SIZE                  ((MTD_NAND_CHIP_COUNT==0)?g_pNandInfo->m_dev->m_hw_info.m_page_size:(g_pNandInfo->m_dev->m_hw_info.m_page_size*MTD_NAND_CHIP_COUNT))
#define MTD_NAND_TOTAL_PAGE_COUNT           ((MTD_NAND_CHIP_COUNT==0)?g_pNandInfo->m_total_pages: (g_pNandInfo->m_total_pages/MTD_NAND_CHIP_COUNT))
#define MTD_NAND_TOTAL_BLOCK_COUNT          ((MTD_NAND_CHIP_COUNT==0)?g_pNandInfo->m_total_blocks: (g_pNandInfo->m_total_blocks/MTD_NAND_CHIP_COUNT))
#define MTD_NAND_PAGES_PER_BLOCK            (g_pNandInfo->m_dev->m_hw_info.m_pages_per_block)
#define MTD_NAND_PAGE_ADDR_SHIFT_BITS       (g_pNandInfo->m_page_addr_shift_bits)
#define MTD_NAND_BLOCK_ADDR_SHIFT_BITS      (g_pNandInfo->m_block_addr_shift_bits)
#define MTD_NAND_ADDR_CYCLE                 (g_pNandInfo->m_dev->m_hw_info.m_addr_cycle)
#define MTD_NAND_IO_INTERFACE               (g_pNandInfo->m_dev->m_hw_info.m_io_interface)


NAND_SW_CONFIG_S    g_nand_sw_config =
{
    FALSE       // use Interrupt handler, default is TRUE
    ,FALSE      // NFI initialization flag, default is FALSE
    ,FALSE       // AHB mode flag, default is TRUE
    ,TRUE       // Auto farmat enable, default is TRUE
    ,TRUE      // ECC SW correct flag, if g_nand_sw_config.bAUTO_FMT = FALSE, must is FALSE, default is FALSE
    ,FALSE      // Randomizer feature enable flag, default is TRUE
    ,FALSE      // DCM feature enable flag, default is FALSE
    ,SECTOR_SIZE_1024   // set sector size, value is SECTOR_SIZE_t, default is SECTOR_SIZE_1024
    ,ECC_24_BITS            // set ECC bits,  value is ECC_Level_t, default is ECC_24_BITS
    ,0           //Record Sector Correct err bits counter, default is 0
    ,0           //Record Page Correct err bits counter, default is 0
    ,TRUE        //default is HW_ECC method
    ,FALSE       //default is one die
    ,FALSE        //interrupt mode
};

static const uint16 _ar2Randomizerseeds[256] =
{
    // 0-127 seed
    0x576A,0x05E8,0x629D,0x45A3,0x649C,0x4BF0,0x2342,0x272E,
    0x7358,0x4FF3,0x73EC,0x5F70,0x7A60,0x1AD8,0x3472,0x3612,
    0x224F,0x0454,0x030E,0x70A5,0x7809,0x2521,0x48F4,0x5A2D,
    0x492A,0x043D,0x7F61,0x3969,0x517A,0x3B42,0x769D,0x0647,
    0x7E2A,0x1383,0x79D9,0x07B8,0x2578,0x7EEC,0x4423,0x352F,
    0x5B22,0x72B9,0x367B,0x24B6,0x7E8E,0x2318,0x6BD0,0x5519,
    0x1783,0x18A7,0x7B6E,0x7602,0x4B7F,0x3648,0x2C53,0x6B99,
    0x0C23,0x67CF,0x7E0E,0x4D8C,0x5079,0x209D,0x244A,0x747B,
    0x350B,0x0E4D,0x7004,0x6AC3,0x7F3E,0x21F5,0x7A15,0x2379,
    0x1517,0x1ABA,0x4E77,0x15A1,0x04FA,0x2D61,0x253A,0x1302,
    0x1F63,0x5AB3,0x049A,0x5AE8,0x1CD7,0x4A00,0x30C8,0x3247,
    0x729C,0x5034,0x2B0E,0x57F2,0x00E4,0x575B,0x6192,0x38F8,
    0x2F6A,0x0C14,0x45FC,0x41DF,0x38DA,0x7AE1,0x7322,0x62DF,
    0x5E39,0x0E64,0x6D85,0x5951,0x5937,0x6281,0x33A1,0x6A32,
    0x3A5A,0x2BAC,0x743A,0x5E74,0x3B2E,0x7EC7,0x4FD2,0x5D28,
    0x751F,0x3EF8,0x39B1,0x4E49,0x746B,0x6EF6,0x44BE,0x6DB7,
    //128-255 seed
    0x576A,0x05E8,0x629D,0x45A3,0x649C,0x4BF0,0x2342,0x272E,
    0x7358,0x4FF3,0x73EC,0x5F70,0x7A60,0x1AD8,0x3472,0x3612,
    0x224F,0x0454,0x030E,0x70A5,0x7809,0x2521,0x48F4,0x5A2D,
    0x492A,0x043D,0x7F61,0x3969,0x517A,0x3B42,0x769D,0x0647,
    0x7E2A,0x1383,0x79D9,0x07B8,0x2578,0x7EEC,0x4423,0x352F,
    0x5B22,0x72B9,0x367B,0x24B6,0x7E8E,0x2318,0x6BD0,0x5519,
    0x1783,0x18A7,0x7B6E,0x7602,0x4B7F,0x3648,0x2C53,0x6B99,
    0x0C23,0x67CF,0x7E0E,0x4D8C,0x5079,0x209D,0x244A,0x747B,
    0x350B,0x0E4D,0x7004,0x6AC3,0x7F3E,0x21F5,0x7A15,0x2379,
    0x1517,0x1ABA,0x4E77,0x15A1,0x04FA,0x2D61,0x253A,0x1302,
    0x1F63,0x5AB3,0x049A,0x5AE8,0x1CD7,0x4A00,0x30C8,0x3247,
    0x729C,0x5034,0x2B0E,0x57F2,0x00E4,0x575B,0x6192,0x38F8,
    0x2F6A,0x0C14,0x45FC,0x41DF,0x38DA,0x7AE1,0x7322,0x62DF,
    0x5E39,0x0E64,0x6D85,0x5951,0x5937,0x6281,0x33A1,0x6A32,
    0x3A5A,0x2BAC,0x743A,0x5E74,0x3B2E,0x7EC7,0x4FD2,0x5D28,
    0x751F,0x3EF8,0x39B1,0x4E49,0x746B,0x6EF6,0x44BE,0x6DB7
};


//------------------------------------------------------------------------------
// COMMON Callback Function Set
//------------------------------------------------------------------------------
const NAND_CMD_Callback_S   g_NAND_COMMON_CB_FUNC_SET=
{
    ID_COMMON_CB_FUNC_SET
    ,NAND_COMMON_ReadID_V4
    ,NAND_COMMON_Reset_V4
    ,NULL
    ,NAND_COMMON_ReadStatus_V4
    ,NAND_COMMON_BlockErase_V4
    ,NAND_COMMON_BadBlockSymbol_Check_V4
    ,NAND_COMMON_BadBlockSymbol_Set_V4
    ,NAND_COMMON_PageRead_V4
    ,NAND_COMMON_PageProgram_V4
    ,NAND_COMMON_SpareRead_V4
    ,NAND_COMMON_SpareProgram_V4
    ,NAND_COMMON_PageSpareProgram_V4
};

#define NFI_ISSUE_COMMAND(cmd, col_addr, row_addr, col_num, row_num) \
   do { \
      DRV_WriteReg(NFI_CMD,cmd);\
      while (DRV_Reg32(NFI_STA) & STATUS_CMD);\
      DRV_WriteReg32(NFI_COLADDR, col_addr);\
      DRV_WriteReg32(NFI_ROWADDR, row_addr);\
      DRV_WriteReg(NFI_ADDRNOB, col_num | (ROW_ADDR_NOB(row_num)));\
      while (DRV_Reg32(NFI_STA) & STATUS_ADDR);\
   }while(0);

void NFI_Reset(void)
{
    *NFI_CON = NFI_RST | FIFO_FLUSH;
    while ( *NFI_STA & (NFI_FSM_MASK|NAND_FSM_MASK));
    //while( FIFO_RD_REMAIN(*NFI_FIFOSTA) || FIFO_WR_REMAIN(*NFI_FIFOSTA) );
}

//------------------------------------------------------------------------------
// Read Device ID Callback Function
//------------------------------------------------------------------------------
STATUS_E  NAND_COMMON_ReadID_V4(
    const uint32  c_timeout
    ,uint16 *p_maker_code
    ,uint16 *p_device_code
    ,uint16 *p_ext_code1
    ,uint16 *p_ext_code2
    ,uint16 *p_ext_code3 //add by jiequn,2011.11.11
)
{
    uint32      timeout = NFI_PSTA_TIMEOUT;
    uint32      id1 = 0;
    uint32      nfi_pagefmt = 0;
    uint32      reg_val;
    STATUS_E    ret=S_DA_NAND_UNKNOWN_ERR;

    DRV_WriteReg(NFI_CSEL, 0);
    /*modify by jiequnchen,2011.11.16*/
    DRV_WriteReg(NFI_ACCCON, 0x00888888/*0x33223*/);

    reg_val = DRV_Reg(NFI_CNFG);
    reg_val &= ~AHB_MODE; //It can't be AHB mode for WAIT_NFI_PIO_READY.
    DRV_WriteReg(NFI_CNFG, reg_val);

#if 1
    /* Reset NFI state machine */
    NFI_Reset();

    /* Issue NAND chip reset command for Micron's MCP */
    NFI_ISSUE_COMMAND(RESET_CMD, 0, 0, 0, 0);

    timeout = 0xFFFF;

    while (timeout)
        timeout--;
#endif

    // reset NFI
    NFI_Reset();

    DRV_WriteReg32(NFI_PAGEFMT, 0);
    /* Config single read and read enable bit in NFI_CNFG register */
    DRV_WriteReg32(NFI_CNFG, OP_READ_ID_ST);

    /* read NAND flash ID */
    DRV_WriteReg32(NFI_CMD, RD_ID_CMD);
    /* wait for the command status ready */
    while (DRV_Reg32(NFI_STA) & STATUS_CMD);

    /* Set addr registers */
    DRV_WriteReg32(NFI_COLADDR, 0);
    DRV_WriteReg32(NFI_ROWADDR, 0);
    DRV_WriteReg32(NFI_ADDRNOB, 1);
    /* wait for the address status ready */
    while (DRV_Reg32(NFI_STA) & STATUS_ADDR);

    /* Trigger read operations */
    DRV_WriteReg32(NFI_CON, SINGLE_RD|NOB_DWORD|SEC_NUM(0x4));
    while (DRV_Reg32(NFI_STA) & STATUS_DATAR);

    /* If status is ready, read back the NFI_DATAR register */
    id1 = DRV_Reg32(NFI_DATAR);
    //*p_ext_code3 = DRV_Reg8(NFI_DATAR); /removed by QQ -
    //DRV_WriteReg(NFI_CON, 0x0);

    *p_maker_code = (id1&0xFF);
    *p_device_code = ((id1>>8)&0xFF);
    *p_ext_code1 = ((id1>>16)&0xFF);
    *p_ext_code2 = ((id1>>24)&0xFF);

    ret = S_DONE;

    // restore original page format setting
    *NFI_PAGEFMT = nfi_pagefmt;

    return ret;
}


//------------------------------------------------------------------------------
// Reset Device Callback Function
//------------------------------------------------------------------------------
STATUS_E  NAND_COMMON_Reset_V4(
    const uint32  c_timeout
)
{
    uint32      timeout = NFI_PSTA_TIMEOUT;

    // reset NFI
    NFI_Reset();

    // set NFI_CNFG
    *NFI_CNFG = OP_RESET;

    // reset cmd
    *NFI_CMD = g_pNandInfo->m_dev->m_cmd_set->m_reset.m_cmd;

    // wait til CMD is completely issued
    timeout = c_timeout;
    NFI_Wait_Command(timeout);
    if (!timeout)
    {
        return S_DA_TIMEOUT;
    }

    //MT6516 FPGA, 20080418: Ray: todo for clearify NFI empty slot question
    // wait for reset finish
    timeout = c_timeout;
    NFI_Wait_Ready(timeout);
    if (!timeout)
    {
        return S_DA_TIMEOUT;
    }


    return S_DONE;
}

//------------------------------------------------------------------------------
// Read Status Callback Function
//------------------------------------------------------------------------------
STATUS_E  NAND_COMMON_ReadStatus_V4(
    const uint32  c_timeout
)
{
    uint32      timeout = NFI_PSTA_TIMEOUT;
    uint32      status = 0xFF;

    // reset NFI
    NFI_Reset();

    // set NFI_CNFG
    *NFI_CNFG = OP_READ_ID_ST;

    if (TEST_MIRCON_16bit_NAND==TRUE)
        //only for Mircon 16bit nand (NAND_MT29F4G16ABD) test
        *NFI_CSEL = CSEL(1); //choose cs1
    else
        *NFI_CSEL = CSEL(0); //choose cs0

    // read status cmd
    *NFI_CMD = g_pNandInfo->m_dev->m_cmd_set->m_status.m_cmd;
    // wait til CMD is completely issued
    timeout = c_timeout;
    NFI_Wait_Command(timeout);
    if (!timeout)
    {
        return S_DA_TIMEOUT;
    }


    // set single read by DWORD
    *NFI_CON = SINGLE_RD | NOB_DWORD;
    // wait til DATA_READ is completely issued
    while ( *NFI_STA  & STATUS_DATAR );

    // single read doesn't need to polling FIFO
    status = *NFI_DATAR;

    //~  clear NOB
    *NFI_CON = 0;

    // check READY/BUSY status first
    if ( !(STATUS_READY&status) )
    {
        return S_DA_IN_PROGRESS;
    }

    // flash is ready now, check status code
    if ( STATUS_FAIL & status )
    {
        if ( !(STATUS_WR_ALLOW&status) )
        {
            return S_DA_NAND_BLOCK_IS_LOCKED;
        }
        else
        {
            return S_DA_NAND_UNKNOWN_ERR;
        }
    }
    else
    {
        return S_DONE;
    }
}

//------------------------------------------------------------------------------
// Block Erase Related Callback Function
//------------------------------------------------------------------------------
STATUS_E  NAND_COMMON_BlockErase_V4(
    const uint32  row_addr
)
{
    uint32  page_size;
    uint32  spare_size;
    uint32  addr_cycle;
    uint32  row_addr_nob, col_addr_nob;
    uint32  c_timeout = NFI_PSTA_TIMEOUT;
    uint32  timeout = NFI_PSTA_TIMEOUT;

    page_size = NUTL_PAGE_SIZE();
    spare_size = NUTL_SPARE_SIZE();
    addr_cycle = NUTL_ADDR_CYCLE();

    // reset NFI
    NFI_Reset();

    // set NFI_CNFG
    *NFI_CNFG = OP_ERASE;

    // block erase cmd
    *NFI_CMD = g_pNandInfo->m_dev->m_cmd_set->m_erase.m_cmd;
    // wait til CMD is completely issued
    timeout = c_timeout;
    NFI_Wait_Command(timeout);
    if (!timeout)
    {
        return S_DA_TIMEOUT;
    }

    // fill 1~4 cycle addr, erase command only fill row address, so column bits shift is unnecessary
    *NFI_ROWADDR = row_addr;
    *NFI_COLADDR = 0;

    // no. of addr cycle
    col_addr_nob =  (page_size==512)?1:2;   // 1B(8 bits) is addressed 512 Bytes, 2B(16 bits) is address 64K
    row_addr_nob = addr_cycle - col_addr_nob;
    *NFI_ADDRNOB = ROW_ADDR_NOB(row_addr_nob);; // Block Erase don't need COL_ADDR_NOB


    // wait til ADDR is completely issued
    timeout = c_timeout;
    NFI_Wait_Adddress(timeout);
    if (!timeout)
    {
        return S_DA_TIMEOUT;
    }

    // block erase confirm
    *NFI_CMD = g_pNandInfo->m_dev->m_cmd_set->m_erase_cnf.m_cmd;
    // wait til CMD is completely issued
    timeout = c_timeout;
    NFI_Wait_Command(timeout);
    if (!timeout)
    {
        return S_DA_TIMEOUT;
    }
    /*wait til ready, add by jiequn,2011.11.8*/
    timeout = c_timeout;
    NFI_Wait_Ready(timeout);
    if (!timeout)
    {
        return S_DA_TIMEOUT;
    }

    return S_DONE;
}

//------------------------------------------------------------------------------
// Bad Block Symbol Identification Related Callback Function
//------------------------------------------------------------------------------
STATUS_E  NAND_COMMON_BadBlockSymbol_Check_V4(
    const uint32  *p_spare32 /* MUST be 32bits alignment addr */
)
{
    uint32  page_size;
    uint32  spare_size;
    uint32  column_addr_bits;
    uint32  addr_cycle;
    uint32  io_interface;
    const uint8     *p_spare8 = (const uint8 *)p_spare32;
    const uint16    *p_spare16 = (const uint16 *)p_spare32;

    page_size = NUTL_PAGE_SIZE();
    spare_size = NUTL_SPARE_SIZE();
    column_addr_bits = NUTL_PAGE_ADDR_SHIFT_BITS();
    addr_cycle = NUTL_ADDR_CYCLE();
    io_interface = NUTL_IO_INTERFACE();

    // check the invalid block status from spare area
    if ( 512 < page_size )
    {
        // for 2048 page size flash, the invalid block status is defined by the 1st byte/word in spare area
        if ( NAND_IO_16BITS == io_interface )
        {
            if ( 0xFFFF != p_spare16[0] )
            {
                return S_DA_NAND_BAD_BLOCK;
            }
        }
        else
        {
            if ( 0xFF != p_spare8[0] )
            {
                return S_DA_NAND_BAD_BLOCK;
            }
        }
    }
    else
    {
        // for 512 page size flash
        if ( NAND_IO_16BITS == io_interface )
        {
            // for 16 bits I/O, the invalid block status is defined by the 1st word in spare area
            if ( 0xFFFF != p_spare16[0] )
            {
                return S_DA_NAND_BAD_BLOCK;
            }
        }
        else
        {
            // for 8 bits I/O, the invalid block status is defined by the 6th byte in spare area
            if ( 0xFF != p_spare8[5] )
            {
                return S_DA_NAND_BAD_BLOCK;
            }
        }
    }

    return S_DONE;
}

STATUS_E  NAND_COMMON_BadBlockSymbol_Set_V4(
    uint32  *p_spare32 /* MUST be 32bits alignment addr */
)
{
    uint32  page_size;
    uint32  spare_size;
    uint32  io_interface;
    uint8   *p_spare8 = (uint8 *)p_spare32;
    uint16  *p_spare16 = (uint16 *)p_spare32;
    uint32  i;

    page_size = NUTL_PAGE_SIZE();
    spare_size = NUTL_SPARE_SIZE();
    io_interface = NUTL_IO_INTERFACE();

    // reset spare
    for (i=0; i<(spare_size>>2); i++)
    {
        p_spare32[i] = 0xAAAAAAAA;
    }

    // check the invalid block status from spare area
    if ( 512 < page_size )
    {
        // for 2048 page size flash, the invalid block status is defined by the 1st byte/word in spare area
        if ( NAND_IO_16BITS == io_interface )
        {
            p_spare16[0] = 0x0;
        }
        else
        {
            p_spare8[0] = 0x0;
        }
    }
    else
    {
        // for 512 page size flash
        if ( NAND_IO_16BITS == io_interface )
        {
            // for 16 bits I/O, the invalid block status is defined by the 1st word in spare area
            p_spare16[0] = 0x0;
        }
        else
        {
            // for 8 bits I/O, the invalid block status is defined by the 6th byte in spare area
            p_spare8[5] = 0x0;
        }
    }

    return S_DONE;
}



//------------------------------------------------------------------------------
// Page Read Callback Function
//------------------------------------------------------------------------------
#if defined(BB_MT8135)

//------------------------------------------------------------------------------
// ECC Error Software Correct
//------------------------------------------------------------------------------
STATUS_E NAND_COMMON_ECCErrCorrect(
    const uint32  c_timeout
    ,const uint32 u4SectIdx
    ,const uint32 *p_data32 /* MUST be 32bits alignment addr */
)
{
    uint32 timeout;
    uint32 i;
    uint32 u4ErrNum, u4ErrVal;
    uint16 u4ErrBitLoc;
    uint32 u4ErrByteLoc;
    STATUS_E ret = S_DA_NAND_UNKNOWN_ERR;

    timeout = c_timeout;

    NFI_Wait( !(*NFIECC_DECDONE & (1 << u4SectIdx)), timeout); // wait for all block decode done
    if ( 0 == timeout )
    {
        //dbg_print("NAND_COMMON_ECCErrCorrect timeout %s: %d \n", __FILE__, __LINE__);
        //dbg_print("u4SectIdx=%d, NFIECC_DECDONE=0x%x\n", u4SectIdx, *NFIECC_DECDONE);
        //dbg_print("NFI_CON=0x%x, NFI_PAGEFMT=0x%x, NFI_CNFG=0x%x\n", *NFI_CON, *NFI_PAGEFMT, *NFI_CNFG);
        //dbg_print("NFI_ADDRCNTR=0x%x, NFIECC_SYNSTA=0x%x\n", *NFI_ADDRCNTR, *NFIECC_SYNSTA);
        ret = S_TIMEOUT;
        goto end;
    }

    if (u4SectIdx < 4)
        u4ErrNum = (*NFIECC_DECENUM & (0x3F << (u4SectIdx<<3))) >> (u4SectIdx<<3);
    else if (u4SectIdx < 8)
        u4ErrNum = (*NFIECC_DECENUM2 & (0x3F << ((u4SectIdx-4)<<3))) >> ((u4SectIdx-4)<<3);
    else if (u4SectIdx < 0xc)
        u4ErrNum = (*NFIECC_DECENUM3 & (0x3F << ((u4SectIdx-8)<<3))) >> ((u4SectIdx-8)<<3);
    else
        u4ErrNum = (*NFIECC_DECENUM4 & (0x3F << ((u4SectIdx-0xc)<<3))) >> ((u4SectIdx-0xc)<<3);


    //dbg_print("NAND_COMMON_ECCErrCorrect Sector %d with u4ErrNum=%d\n", u4SectIdx, u4ErrNum);
    if ( 0x3F == u4ErrNum)
    {
        //dbg_print("NAND_COMMON_ECCErrCorrect Sector %d with ECC-uncorrect\n", u4SectIdx);
        //dbg_print("NFIECC_DECENUM=0x%x, NFIECC_DECENUM2=0x%x\n", *NFIECC_DECENUM, *NFIECC_DECENUM2);
        ret = S_DA_NAND_ECC_2BITS_ERR;
        goto end;
    }
    else if ( 0x0 == u4ErrNum)
    {
        ret = S_DONE;
        goto end;
    }
    else
    {
        //dbg_print("NAND_COMMON_ECCErrCorrect FDM=0x%x, 0x%x\n", *((uint32*)((uint32)NFI_FDM0L+16)), *((uint32*)((uint32)NFI_FDM0L+20)) );
        volatile uint32* p4ECCDECAddr = NFIECC_DECEL0;
        for (i=0; i<u4ErrNum; i++)
        {
            //dbg_print("NAND_COMMON_ECCErrCorrect u4ErrBitLoc=%d(Addr:0x%x)\n", *p4ECCDECAddr, (uint32)p4ECCDECAddr);
            if ((i&1) == 0)
            {
                u4ErrBitLoc = (*p4ECCDECAddr) & 0x0000FFFF;
            }
            else
            {
                u4ErrBitLoc = ((*p4ECCDECAddr) >>16) & 0x0000FFFF;
                p4ECCDECAddr ++;
            }

            if ( (u4ErrBitLoc >>3) < SECTOR_BYTES)
            {
                u4ErrByteLoc = ((uint32)p_data32)+(u4SectIdx*SECTOR_BYTES)+(u4ErrBitLoc>>3);
                u4ErrVal = *(uint8*)u4ErrByteLoc;
                u4ErrVal = u4ErrVal & (1 << (u4ErrBitLoc &0x7));
                if (u4ErrVal)
                    *(uint8*)u4ErrByteLoc &= (~u4ErrVal);
                else
                    *(uint8*)u4ErrByteLoc |= (1 << (u4ErrBitLoc &0x7));
            }
            else if ( (u4ErrBitLoc >>3) < (SECTOR_BYTES+FDM_ECC_BYTES))
            {

                if (u4SectIdx >= 8)
                {
                    u4ErrByteLoc = ((uint32)NFI_FDM8L) + ((u4SectIdx - 8) << 4) + ((((u4ErrBitLoc>>3) -SECTOR_BYTES) >>2) <<2);
                    u4ErrVal = *(uint32*)u4ErrByteLoc;
                    u4ErrVal = u4ErrVal & (0x1 << (u4ErrBitLoc &0x1F));
                    if (u4ErrVal)
                        *(uint32*)u4ErrByteLoc &= (~u4ErrVal);
                    else
                        *(uint32*)u4ErrByteLoc |= (0x1 << (u4ErrBitLoc &0x1F));
                }
                else
                {
                    u4ErrByteLoc = ((uint32)NFI_FDM0L)+(u4SectIdx<<4) + ((((u4ErrBitLoc>>3) -SECTOR_BYTES) >>2) <<2);
                    u4ErrVal = *(uint32*)u4ErrByteLoc;
                    u4ErrVal = u4ErrVal & (0x1 << (u4ErrBitLoc &0x1F));
                    if (u4ErrVal)
                        *(uint32*)u4ErrByteLoc &= (~u4ErrVal);
                    else
                        *(uint32*)u4ErrByteLoc |= (0x1 << (u4ErrBitLoc &0x1F));
                }
            }

        }

        //dbg_print("NAND_COMMON_ECCErrCorrect FDM=0x%x, 0x%x\n", *((uint32*)((uint32)NFI_FDM0L+16)), *((uint32*)((uint32)NFI_FDM0L+20)) );
        ret = S_DA_NAND_ECC_1BIT_CORRECT;
    }

end:
    return ret;
}
//------------------------------------------------------------------------------
// ECC Error Detect
//------------------------------------------------------------------------------
STATUS_E NAND_COMMON_ECCErrDetect(
    const NAND_DeviceInfo_S  *nand_info
    ,const uint32  c_timeout
    ,const uint32  row_addr
    ,const uint32 *p_data32 /* MUST be 32bits alignment addr */
)
{
    uint32 u4BlockNum, page_size,i, u4Tmp, timeout;
    STATUS_E ret = S_DONE;

    page_size =  nand_info->m_dev->m_hw_info.m_page_size;
    u4BlockNum =  page_size/SECTOR_BYTES;

    u4Tmp = 0;
    g_nand_sw_config.u4SectCorrErrBits=0;
    g_nand_sw_config.u4PageCorrErrBits=0;

    for ( i = 0 ; i < u4BlockNum ; i++)
        u4Tmp |= 1 << i;

    timeout = c_timeout;
    NFI_Wait( (*NFIECC_DECDONE != u4Tmp), timeout); // wait for all block decode done
    if ( 0 == timeout )
    {
        //dbg_print("ECCDetect timeout %s: %d!\n", __FILE__, __LINE__);
        return S_TIMEOUT;
    }


    if ( !*NFIECC_DECFER)
        return S_DONE;
    else
    {
        u4Tmp = *NFIECC_DECENUM; // error number
        for ( i = 0 ; i < u4BlockNum ; i++)
        {
            if ( 0x3F == (u4Tmp & 0x3F))
            {
                //dbg_print("Page 0x%x Sector %d with ECC Error-uncorrect\n", row_addr, i);
                //   dbg_print("NFIECC_DECENUM=0x%x, NFIECC_DECENUM2=0x%x\n", *NFIECC_DECENUM, *NFIECC_DECENUM2);
                return S_DA_NAND_ECC_2BITS_ERR;
            }
            else if ( u4Tmp & 0x3F)
            {
                g_nand_sw_config.u4SectCorrErrBits = (u4Tmp&0x3F);
                //dbg_print("Page 0x%x Sector %d with ECC Error-correct, Errbits=%d\n", row_addr, i, _u4ErrBits);
                g_nand_sw_config.u4PageCorrErrBits += g_nand_sw_config.u4SectCorrErrBits;

                ret = S_DA_NAND_ECC_1BIT_CORRECT;
            }

            if (i==4)
                u4Tmp = *NFIECC_DECENUM2; // error number 2

            if (i==8)
                u4Tmp = *NFIECC_DECENUM3; // error number 2

            if (i==0xc)
                u4Tmp = *NFIECC_DECENUM4; // error number 2
            else
                u4Tmp >>= 8;
        }
    }

    return ret;
}





//------------------------------------------------------------------------------
// Read From NFI FIFO
//------------------------------------------------------------------------------
STATUS_E  NAND_COMMON_FIFO_Read(
    const uint32  c_timeout
    ,const bool bUsingDMA
    ,uint32 *p_data32 /* MUST be 32bits alignment addr */
    ,const uint32 data_len
    ,const uint32 spare_size
    ,bool  fgByteRW
)
{
    uint32  timeout = c_timeout;
    uint32  i ;

    if (bUsingDMA)
    {
        // read page data with DMA
        timeout = c_timeout;
        // wait for DMA transmission complet
        {
            //--------------
            /* Trigger DMA action */
            //dbg_print("timeout before:NFI_CON0= %x\n",*NFI_CON);
            NFI_Wait(( AHB_DONE != (*NFI_INTR&AHB_DONE)), timeout);
            //dbg_print("NFI_CON= %x\n",*NFI_CON);
            //dbg_print("NFI_CNFG= %x\n",*NFI_CNFG);
            //dbg_print("NFI_PAGEFMT= %x\n",*NFI_PAGEFMT);
            *NFI_INTR_EN &= ~AHB_DONE_EN; // disable INT first
            //dbg_print("timeout after disable INT:NFI_CON2 = %x\n",*NFI_CON);
            i = *NFI_INTR;//read clear again
            *NFI_INTR = i;

            if ( 0 == timeout)
            {
                //dbg_print("polling: NAND_COMMON_FIFO_Read wait AHB_DONE timeout %s: %d \n", __FILE__, __LINE__);
                return S_TIMEOUT;
            }
        }
    }
    else
    {
        // read page data
        timeout = 0x7FFFF;
        if (g_nand_sw_config.bAUTO_FMT)
        {
            uint32 u4SecIdx = data_len/SECTOR_BYTES;
            uint32 j;
            STATUS_E ret = S_DA_NAND_UNKNOWN_ERR;
            uint32* p_TmpData = (uint32*)p_data32;

            for (j=0; j<u4SecIdx; j++)
            {
                for (i=0; i<SECTOR_BYTES; i+=4)
                {
                    // wait for data ready
                    // when RD_EMPTY_MASK flag is poll-down, it means data is ready in FIFO at least 4 bytes.
                    //dbg_print("1.i=%d, NFI_ADDRCNTR=0x%x, NFIECC_SYNSTA=0x%x\n", i, *NFI_ADDRCNTR, *NFIECC_SYNSTA);
                    NFI_Wait( (*NFI_FIFOSTA & RD_EMPTY_MASK), timeout);
                    if ( 0 == timeout )
                    {
                        //dbg_print("NAND_COMMON_FIFO_Read wait FIFO empty(i=%d,0x%x) timeout %s: %d \n", i, i, __FILE__, __LINE__);
                        return S_TIMEOUT;
                    }

                   if(fgByteRW)
                   {
                      *(p_TmpData++) = ((uint8)(*NFI_DATAR))<<0 | ((uint8)(*NFI_DATAR))<<8 | ((uint8)(*NFI_DATAR))<<16 |((uint8)(*NFI_DATAR))<<24;
                   }
                   else
                   {
                      *(p_TmpData++) = *NFI_DATAR;
                   }

                }
		  /*
		  for (i=0; i<spare_size/u4SecIdx; )//spare data TX by HW in auto_fmt mode
                {
                    if(fgByteRW)
                    {
                         NFI_Wait( (2 == (*NFI_FIFOSTA & RD_REMAIN_MASK)), timeout);
                         if ( 0 ==  timeout)
                         {
                              return S_TIMEOUT;
                         }
                         *NFI_DATAR;	
                         *NFI_DATAR;
                         i+= 2;
                    }
                    else
                    {
                         NFI_Wait( (*NFI_FIFOSTA & RD_EMPTY_MASK), timeout);
                         if ( 0 == timeout )
                         {
                              return S_TIMEOUT;
                         }
                         *NFI_DATAR;	
                         i+= 4;
                    }
                }*/

                if (g_nand_sw_config.bECCSWCorrect)
                {
                    ret = NAND_COMMON_ECCErrCorrect(c_timeout, j, p_data32); // AUTO_FMT must be enabled
                    if ( (S_DONE != ret) && (S_DA_NAND_ECC_1BIT_CORRECT != ret))
                    {
                        return ret;
                    }
                }

            }
        }
        else
        {
            for (i=0; i<data_len + spare_size;)
            {
                if(fgByteRW)
                {
                    NFI_Wait( (2 == (*NFI_FIFOSTA & RD_REMAIN_MASK)), timeout);
                    if ( 0 == timeout )
                    {
                        return S_TIMEOUT;
                    }
                    *(uint8*)((uint8)p_data32 + i) = *NFI_DATAR;	
                    *(uint8*)((uint8)p_data32 + i) = *NFI_DATAR;
                    i+= 2;
                }
                else
                {
                    timeout = c_timeout;
                    NFI_Wait( (*NFI_FIFOSTA & RD_EMPTY_MASK), timeout);
                    if ( 0 == timeout )
                    {
                        //dbg_print("NAND_COMMON_FIFO_Read wait FIFO empty(i=%d,0x%x) timeout %s: %d \n", i, i, __FILE__, __LINE__);
                        return S_TIMEOUT;
                    }
                    *(uint32*)((uint32)p_data32 + i) = *NFI_DATAR;
                    i+=4;
                }
            }
        }
    }
    return S_DONE;
}



//------------------------------------------------------------------------------
// Write To NFI FIFO
//------------------------------------------------------------------------------
STATUS_E  NAND_COMMON_FIFO_Write(
    const uint32  c_timeout
    ,const bool bUsingDMA
    ,const uint32 *p_data32 /* MUST be 32bits alignment addr */
    ,const uint32 data_len
)
{
    uint32  timeout = c_timeout;
    uint32  i;

    if (bUsingDMA)
    {
        timeout = c_timeout;
        {
            NFI_Wait( (AHB_DONE != (*NFI_INTR&AHB_DONE)), timeout); //clear INT status
            *NFI_INTR_EN &= ~AHB_DONE; // disable INT
            i = *NFI_INTR;//read clear again
            *NFI_INTR = i;
            if ( 0 == timeout)
            {
                //dbg_print("NAND_COMMON_FIFO_Write wait AHB_DONE timeout %s: %d \n", __FILE__, __LINE__);
                return S_TIMEOUT;
            }
        }
    }
    else
    {
        // program page data
        for (i=0; i<data_len; i+=4, p_data32++)
        {
            timeout = c_timeout;
            NFI_Wait( (*NFI_FIFOSTA & WR_FULL_MASK), timeout);
            if ( 0 == timeout )
            {
                //dbg_print("NAND_COMMON_FIFO_Write wait WR_FULL_MASK timeout, i=%d %s: %d \n", i, __FILE__, __LINE__);
                return S_TIMEOUT;
            }
            *NFI_DATAW = *p_data32;
        }
    }

    return S_DONE;
}

STATUS_E  NAND_COMMON_PageRead_V4(
    const uint32  c_timeout
    ,const uint32  row_addr
    ,uint32 *p_data32 /* MUST be 32bits alignment addr */
    ,uint32  ecc_parity_from_reg[8]
)
{
    uint32    page_size;
    uint32    spare_size;
    uint32    pages_per_block;
    uint32    column_addr_bytes, row_addr_bytes;
    uint32    addr_cycle;
    STATUS_E  ret=S_DA_NAND_UNKNOWN_ERR;
    uint32  timeout = c_timeout;
    uint32  i, j,k;
    volatile uint8* p1TmpSpare;// = p_spare32;
    uint32 fdm_parity;
    uint32 chip_count = 2;
    bool  fgByteRW = FALSE;

    //ASSERT(!((uint32)p_data32%0x10)); //assert
    page_size =  g_pNandInfo->m_dev->m_hw_info.m_page_size;
    spare_size = g_pNandInfo->m_spare_size;
    pages_per_block = g_pNandInfo->m_dev->m_hw_info.m_pages_per_block;

    p1TmpSpare = (volatile uint8*)((uint32)p_data32 + MTD_NAND_PAGE_SIZE);


    //*NFI_ACCCON2 = 0x000F000a;
     g_nand_sw_config.eSecSize =  (page_size == SECTOR_SIZE_512) ? 512 : 1024;

    if (g_nand_sw_config.bRandomizerEn)
    {
        *NFI_RANDOM_CFG = 0x0;
        *NFI_FIFOSTA      = NFI_Empty_Tolerate << 24;
        //if(_u4Randomizer_seed )
        //{
        // *NFI_RANDOM_CFG = RANDOM_DECODE_EN | RANDOM_DECODE_SEED(_ar2Randomizerseeds[(row_addr-1) &(pages_per_block-1)]);
        //}
        // else
        {
            *NFI_RANDOM_CFG = RANDOM_DECODE_EN | RANDOM_DECODE_SEED(_ar2Randomizerseeds[(row_addr) &(pages_per_block-1)]);
        }
    }
    else
    {
        *NFI_RANDOM_CFG = 0x0;
    }

    switch (*NFI_PAGEFMT & 0x003C)
    {
    case SPARE_16:
        spare_size = ( spare_size/(page_size/SECTOR_BYTES)) >= 16?(16*page_size/SECTOR_BYTES):spare_size;
        break;
    case SPARE_26:
        spare_size = 26*page_size/SECTOR_BYTES;
        break;
    case SPARE_27:
        spare_size = 27*page_size/SECTOR_BYTES;
        break;
    case SPARE_28:
        spare_size = 28*page_size/SECTOR_BYTES;
        break;
    case SPARE_32:
        spare_size = 32*page_size/SECTOR_BYTES;
        break;
    case SPARE_36:
        spare_size = 36*page_size/SECTOR_BYTES;
        break;
    case SPARE_40:
        spare_size = 40*page_size/SECTOR_BYTES;
        break;
    case SPARE_44:
        spare_size = 44*page_size/SECTOR_BYTES;
        break;
    case SPARE_48:
        spare_size = 48*page_size/SECTOR_BYTES;
        break;
    case SPARE_50:
        spare_size = 50*page_size/SECTOR_BYTES;
        break;
    case SPARE_52:
        spare_size = 52*page_size/SECTOR_BYTES;
        break;
    case SPARE_54:
        spare_size = 54*page_size/SECTOR_BYTES;
        break;
    case SPARE_56:
        spare_size = 56*page_size/SECTOR_BYTES;
        break;
    case SPARE_62:
        spare_size = 62*page_size/SECTOR_BYTES;
        break;
    case SPARE_63:
        spare_size = 63*page_size/SECTOR_BYTES;
        break;
    case SPARE_64:
        spare_size = 64*page_size/SECTOR_BYTES;
        break;
    default:
        break;
    }
    if (g_nand_sw_config.eSecSize == SECTOR_SIZE_1024)
        spare_size <<= 1;

    //  if (g_nand_sw_config.bUsingDMA) {
    //  if (g_nand_sw_config.bAUTO_FMT) {
    // __cache_invalidate_buf((void*)p_data32, (page_size));
    // }else {
    //  __cache_invalidate_buf((void*)p_data32, (page_size+spare_size));
    // }
    // }

    addr_cycle = g_pNandInfo->m_dev->m_hw_info.m_addr_cycle;
    column_addr_bytes = g_pNandInfo->m_page_addr_shift_bits >> 3;
    row_addr_bytes = addr_cycle - column_addr_bytes;

    // reset the NFI core state machine, data FIFO and flushing FIFO
    *NFI_CON = NFI_RST | FIFO_FLUSH;
    *NFI_CNFG = (OP_READ|READ_MODE);
    if (g_nand_sw_config.eSecSize == SECTOR_SIZE_512)
        *NFI_CNFG |= SEL_SEC_512BYTE;
    else
        *NFI_CNFG &= (~SEL_SEC_512BYTE);

    if(  ((*NFI_PAGEFMT & 0x003C)%4 > 0)  &&  (FALSE == g_nand_sw_config.bUsingDMA) )
    {
        *NFI_CNFG |= BYTE_RW;
        fgByteRW = TRUE;
    }

    if (g_nand_sw_config.bAUTO_FMT)
    {
        *NFI_CNFG |= AUTO_FMT_EN;
        //if(TEST_EMPTY_PAGE == TRUE )
        //{
        //   *NFI_CNFG &= ~HW_ECC_EN;
        //}
        //else
        {
            *NFI_CNFG |= HW_ECC_EN;
        }
    }

    // setup NFI page format and I/O interface
    if ( NAND_IO_16BITS == MTD_NAND_IO_INTERFACE )
        *NFI_CNFG |= DBYTE_EN_16BITS;


    if (TEST_MIRCON_16bit_NAND==TRUE)
    {
        //only for Mircon 16bit nand (NAND_MT29F4G16ABD) test
        *NFI_CSEL = CSEL(1); //choose cs1
        //*NFI_CSEL |= RB_CS1;
    }
    else
    {
        *NFI_CSEL = CSEL(0); //choose cs0
        //*NFI_CSEL &= (~RB_CS1);
    }


    *NFI_CON = SEC_NUM(page_size/SECTOR_BYTES);

    if (g_nand_sw_config.bECCSWCorrect)
        *NFIECC_DECCNFG = DEC_EMPTY_EN |DEC_CON(ECC_DEC_LOCATE) | DEC_NFI_MODE;
    else
        *NFIECC_DECCNFG = DEC_EMPTY_EN |DEC_CON(ECC_DEC_CORRECT) | DEC_NFI_MODE;

    // FDM_ECC_BYTES + 24*14/8 = 12+42
    fdm_parity = (spare_size/(page_size/SECTOR_BYTES)) - FDM_ECC_BYTES;
    fdm_parity <<= 3;
    fdm_parity /= ECC_PARITY_NUM;

    if (fdm_parity >=ECC_60_BITS)       g_nand_sw_config.eECC_BIT = ECC_60_BITS;
    else if (fdm_parity >=ECC_56_BITS)  g_nand_sw_config.eECC_BIT = ECC_56_BITS;
    else if (fdm_parity >=ECC_52_BITS)  g_nand_sw_config.eECC_BIT = ECC_52_BITS;
    else if (fdm_parity >=ECC_48_BITS)  g_nand_sw_config.eECC_BIT = ECC_48_BITS;
    else if (fdm_parity >=ECC_44_BITS)  g_nand_sw_config.eECC_BIT = ECC_44_BITS;
    else if (fdm_parity >=ECC_40_BITS)  g_nand_sw_config.eECC_BIT = ECC_40_BITS;
    else if (fdm_parity >=ECC_36_BITS)  g_nand_sw_config.eECC_BIT = ECC_36_BITS;
    else if (fdm_parity >=ECC_32_BITS)  g_nand_sw_config.eECC_BIT = ECC_32_BITS;
    else if (fdm_parity >=ECC_28_BITS)  g_nand_sw_config.eECC_BIT = ECC_28_BITS;
    else if (fdm_parity >=ECC_24_BITS)  g_nand_sw_config.eECC_BIT = ECC_24_BITS;
    else if (fdm_parity >=ECC_22_BITS)  g_nand_sw_config.eECC_BIT = ECC_22_BITS;
    else if (fdm_parity >=ECC_20_BITS)  g_nand_sw_config.eECC_BIT = ECC_20_BITS;
    else if (fdm_parity >=ECC_18_BITS)  g_nand_sw_config.eECC_BIT = ECC_18_BITS;
    else if (fdm_parity >=ECC_16_BITS)  g_nand_sw_config.eECC_BIT = ECC_16_BITS;
    else if (fdm_parity >=ECC_14_BITS)  g_nand_sw_config.eECC_BIT = ECC_14_BITS;
    else if (fdm_parity >=ECC_12_BITS)  g_nand_sw_config.eECC_BIT = ECC_12_BITS;
    else if (fdm_parity >=ECC_10_BITS)  g_nand_sw_config.eECC_BIT = ECC_10_BITS;
    else if (fdm_parity >=ECC_8_BITS)   g_nand_sw_config.eECC_BIT = ECC_8_BITS;
    else if (fdm_parity >=ECC_6_BITS)   g_nand_sw_config.eECC_BIT = ECC_6_BITS;
    else                                g_nand_sw_config.eECC_BIT = ECC_4_BITS;

    //12bit ECC ,  protected bit:  (1024+8)*8 +12*14 =8424
    *NFIECC_DECCNFG |= DEC_TNUM(g_nand_sw_config.eECC_BIT) | DEC_CS( ((SECTOR_BYTES + FDM_ECC_BYTES) << 3) + g_nand_sw_config.eECC_BIT*ECC_PARITY_NUM ) | DEC_NFI_MODE;

    *NFIECC_DECCON = 0; // reset
    *NFIECC_DECCON = DEC_EN;

    if ( AUTO_FMT_EN & (*NFI_CNFG))
        *NFIECC_FDMADDR = (uint32)0x110010A0;  //NFI_FDM0L, need write phy address

    if (g_nand_sw_config.bUsingDMA)
    {
        *NFI_CNFG |= AHB_MODE;
        *NFI_STRADDR = ((uint32)p_data32 | 0x00000000);
    }


    //dbg_print("Send read command\n");
    // read cmd
    *NFI_CMD = g_pNandInfo->m_dev->m_cmd_set->m_read.m_cmd;
    // wait til CMD is completely issued
    while ( *NFI_STA  & STATUS_CMD );

    // fill 1~4 cycle addr
    *NFI_COLADDR = 0;
    *NFI_ROWADDR = row_addr;
    // no. of addr cycle
    *NFI_ADDRNOB = COL_ADDR_NOB(column_addr_bytes) | ROW_ADDR_NOB(row_addr_bytes);
    // wait til ADDR is completely issued
    while ( *NFI_STA  & STATUS_ADDR );

    if (g_pNandInfo->m_dev->m_cmd_set->m_read_cnf.m_enable)
    {

        //dbg_print("Send read confirm start: 0x%x \n",nand_info->m_dev->m_cmd_set->m_read_cnf.m_cmd);
        *NFI_CMD = g_pNandInfo->m_dev->m_cmd_set->m_read_cnf.m_cmd;
        while ( *NFI_STA  & STATUS_BUSY);
    }

    //dbg_print("Send read confirm command\n");

    if (g_nand_sw_config.bUsingDMA)
    {
        if (g_nand_sw_config.nfi_interrupt == TRUE)
        {
            *NFI_INTR_EN |= AHB_DONE_EN;
        }
        else
        {
            *NFI_INTR_EN |= AHB_DONE_EN;
        }
    }

    i = *NFI_INTR;
    //#ifdef INT_WR_CLR
    *NFI_INTR = i;
    //#endif

    //#if(NAND_AUTORD_DONE == 1)
    // set burst read by DWORD
    //          *NFI_CON |= BURST_RD | NAND_NFI_CON_BRD_HW_EN |NOB_DWORD;
    // #else
    *NFI_CON |= BURST_RD | NOB_DWORD;
    //#endif


    //#if (NFI_CHECK_NAND_RB == 1)
    // For FPGA emulation, IC Verify please setting default value.
    //  *NFI_CNRNB = 0xF1;
    //#endif

    // wait til DATA_READ is completely issued
    if (g_nand_sw_config.bAUTO_FMT)
    {
        //dbg_print("AUTOFMT READ FIFO\n");
        if ( S_DONE != (ret=NAND_COMMON_FIFO_Read(c_timeout, g_nand_sw_config.bUsingDMA, p_data32, page_size, spare_size, fgByteRW)) )
        {
            goto end;
        }
    }
    else
    {
        if ( S_DONE != (ret=NAND_COMMON_FIFO_Read(c_timeout, g_nand_sw_config.bUsingDMA, p_data32, page_size, spare_size, fgByteRW)) )
        //if ( S_DONE != (ret=NAND_COMMON_FIFO_Read(c_timeout, g_nand_sw_config.bUsingDMA, p_data32, page_size)) )//no need spare data
        {
            goto end;
        }
    }

    // read spare data
    if ( AUTO_FMT_EN & (*NFI_CNFG)) // g_nand_sw_config.bAUTO_FMT == TRUE
    {
        volatile uint32* pFDMAddr = NFI_FDM0L;
        uint32 u4TmpFDMData = 0;

        timeout = c_timeout;
        //NFI_Wait( ((*NFI_ADDRCNTR & 0xF800) >> 11) != (page_size/SECTOR_BYTES) , timeout);
        NFI_Wait( ((*NFIECC_DECDONE & (0x1<<((page_size/SECTOR_BYTES)-1))) != (0x1<<((page_size/SECTOR_BYTES)-1))) , timeout);
        if ( 0 == timeout )
        {
            //dbg_print("Read Page timeout %s: %d, NFIECC_DECDONE=%x \n", __FILE__, __LINE__, *NFIECC_DECDONE);
            ret = S_TIMEOUT;
            goto end;
        }
        //dbg_print("NFIECC_DECENUM=0x%x, NFIECC_DECEL0=0x%x.\n", (*NFIECC_DECENUM), (*NFIECC_DECEL0));

        if ((page_size/SECTOR_BYTES)>8)
        {
            // read spare data
            for (i = 0; i < (page_size/SECTOR_BYTES) ; i++)
            {
                if (i>=8)
                {
                    k=i-8;
                    pFDMAddr = NFI_FDM8L + (k<<2); // i*16/4

                    for (j=0; j<(FDM_BYTES>>2); j++)
                    {
                        u4TmpFDMData = *pFDMAddr++;
                        *p1TmpSpare++ = (u4TmpFDMData ) & 0xFF;
                        *p1TmpSpare++ = (u4TmpFDMData >> 8) & 0xFF;
                        *p1TmpSpare++ = (u4TmpFDMData >>16) & 0xFF;
                        *p1TmpSpare++ = (u4TmpFDMData >>24) & 0xFF;
                    }
                    if ((FDM_BYTES - (j<<2)) == 1)
                    {
                        u4TmpFDMData = *pFDMAddr;
                        *p1TmpSpare++ = (u4TmpFDMData ) & 0xFF;
                    }
                    else if ((FDM_BYTES - (j<<2)) == 2)
                    {
                        u4TmpFDMData = *pFDMAddr;
                        *p1TmpSpare++ = (u4TmpFDMData ) & 0xFF;
                        *p1TmpSpare++ = (u4TmpFDMData >> 8) & 0xFF;
                    }
                    else if ((FDM_BYTES - (j<<2)) == 3)
                    {
                        u4TmpFDMData = *pFDMAddr;
                        *p1TmpSpare++ = (u4TmpFDMData ) & 0xFF;
                        *p1TmpSpare++ = (u4TmpFDMData >> 8) & 0xFF;
                        *p1TmpSpare++ = (u4TmpFDMData >>16) & 0xFF;
                    }

                }
                else
                {
                    pFDMAddr = NFI_FDM0L + (i<<2); // i*16/4

                    for (j=0; j<(FDM_BYTES>>2); j++)
                    {
                        u4TmpFDMData = *pFDMAddr++;
                        *p1TmpSpare++ = (u4TmpFDMData ) & 0xFF;
                        *p1TmpSpare++ = (u4TmpFDMData >> 8) & 0xFF;
                        *p1TmpSpare++ = (u4TmpFDMData >>16) & 0xFF;
                        *p1TmpSpare++ = (u4TmpFDMData >>24) & 0xFF;
                    }
                    if ((FDM_BYTES - (j<<2)) == 1)
                    {
                        u4TmpFDMData = *pFDMAddr;
                        *p1TmpSpare++ = (u4TmpFDMData ) & 0xFF;
                    }
                    else if ((FDM_BYTES - (j<<2)) == 2)
                    {
                        u4TmpFDMData = *pFDMAddr;
                        *p1TmpSpare++ = (u4TmpFDMData ) & 0xFF;
                        *p1TmpSpare++ = (u4TmpFDMData >> 8) & 0xFF;
                    }
                    else if ((FDM_BYTES - (j<<2)) == 3)
                    {
                        u4TmpFDMData = *pFDMAddr;
                        *p1TmpSpare++ = (u4TmpFDMData ) & 0xFF;
                        *p1TmpSpare++ = (u4TmpFDMData >> 8) & 0xFF;
                        *p1TmpSpare++ = (u4TmpFDMData >>16) & 0xFF;
                    }

                }

            }

        }
        else
        {
            // read spare data
            for (i = 0; i < (page_size/SECTOR_BYTES) ; i++)
            {
                pFDMAddr = NFI_FDM0L + (i<<2); // i*16/4

                for (j=0; j<(FDM_BYTES>>2); j++)
                {
                    u4TmpFDMData = *pFDMAddr++;
                    *p1TmpSpare++ = (u4TmpFDMData ) & 0xFF;
                    *p1TmpSpare++ = (u4TmpFDMData >> 8) & 0xFF;
                    *p1TmpSpare++ = (u4TmpFDMData >>16) & 0xFF;
                    *p1TmpSpare++ = (u4TmpFDMData >>24) & 0xFF;
                }
                if ((FDM_BYTES - (j<<2)) == 1)
                {
                    u4TmpFDMData = *pFDMAddr;
                    *p1TmpSpare++ = (u4TmpFDMData ) & 0xFF;
                }
                else if ((FDM_BYTES - (j<<2)) == 2)
                {
                    u4TmpFDMData = *pFDMAddr;
                    *p1TmpSpare++ = (u4TmpFDMData ) & 0xFF;
                    *p1TmpSpare++ = (u4TmpFDMData >> 8) & 0xFF;
                }
                else if ((FDM_BYTES - (j<<2)) == 3)
                {
                    u4TmpFDMData = *pFDMAddr;
                    *p1TmpSpare++ = (u4TmpFDMData ) & 0xFF;
                    *p1TmpSpare++ = (u4TmpFDMData >> 8) & 0xFF;
                    *p1TmpSpare++ = (u4TmpFDMData >>16) & 0xFF;
                }
            }

        }

    }

    if ( *NFI_CNFG & HW_ECC_EN)
        ret = NAND_COMMON_ECCErrDetect(g_pNandInfo, c_timeout, row_addr, p_data32);
    if ( S_DA_NAND_ECC_1BIT_CORRECT == ret)
        ret = S_DONE;

end:

    if (g_nand_sw_config.bRandomizerEn)
    {
        if( NFI_Empty_Page() )
        {
              if (g_nand_sw_config.bAUTO_FMT)
                     memset((void*)p_data32, 0xFF, page_size + spare_size);
              else
                     memset((void*)p_data32, 0xFF, page_size);	
               ret = S_DONE;
        }
        *NFI_RANDOM_CFG = 0x0;

    }
    // disable burst read
    *NFI_CON = 0x0;
    *NFIECC_DECCON = 0x0;
    //*NFIECC_DECCNFG = 0;
   	//test rom code
    return ret;
}
//------------------------------------------------------------------------------
// Page Program Callback Function
//------------------------------------------------------------------------------
STATUS_E  NAND_COMMON_PageProgram_V4(
    const uint32  c_timeout
    ,const uint32  row_addr
    ,const uint32 *p_data32 /* MUST be 32bits alignment addr */
    ,uint32  ecc_parity_from_reg[8]
)
{
    uint32    page_size, spare_size, backup_sparesize, backup_pageFMT;
    uint32    column_addr_bytes,row_addr_bytes;
    uint32    addr_cycle;
    uint32     timeout;
    uint32    pages_per_block;
    //  uint32    io_interface;
    STATUS_E  ret=S_DA_NAND_UNKNOWN_ERR;
    uint32 i, j,k;
    uint32 fdm_parity;
    uint32* p_spare32;
    NAND_DeviceInfo_S  *nand_info = g_pNandInfo;

    // ASSERT(!((uint32)p_data32%0x10)); //assert
    g_nand_sw_config.eSecSize =
       (nand_info->m_dev->m_hw_info.m_page_size == SECTOR_SIZE_512) ? 512 : 1024;

    page_size =  nand_info->m_dev->m_hw_info.m_page_size;
    /*
    if ((0 == row_addr)&&(1024 ==  g_nand_sw_config.eSecSize)) // header block first page
    {
          backup_sparesize = nand_info->m_spare_size;
          nand_info->m_spare_size = 128*page_size/SECTOR_BYTES ;
          backup_pageFMT = *NFI_PAGEFMT ;
          *NFI_PAGEFMT  &= 0x0000FFC3;
          *NFI_PAGEFMT |= SPARE_64; 
          *NFI_PAGEFMT |= ((SECTOR_BYTES +  (g_pNandInfo->m_spare_size))<<16);
    }*/ 
    //marked by qq - due to PIO mode added

    spare_size = nand_info->m_spare_size;
    addr_cycle = nand_info->m_dev->m_hw_info.m_addr_cycle;
    column_addr_bytes = nand_info->m_page_addr_shift_bits >> 3;
    row_addr_bytes = addr_cycle - column_addr_bytes;
    pages_per_block = nand_info->m_dev->m_hw_info.m_pages_per_block;

    p_spare32 = (uint32*)((uint32)p_data32 + MTD_NAND_PAGE_SIZE);

    //-----------------------------------------------------------------------
    //Randomizer Encode Enable NFI_60BIT_RANDOM_CFG[0]= 1
    //-----------------------------------------------------------------------
    if (g_nand_sw_config.bRandomizerEn)
    {
        *NFI_RANDOM_CFG = 0x0;
        *NFI_RANDOM_CFG = RANDOM_ENCODE_EN | RANDOM_ENCODE_SEED(_ar2Randomizerseeds[row_addr &(pages_per_block-1)]);
    }
    else
    {
        *NFI_RANDOM_CFG = 0x0;
    }
    //-----------------------------------------------------------------------

    // reset the NFI core state machine, data FIFO and flushing FIFO
    *NFI_CON = NFI_RST | FIFO_FLUSH;
    *NFI_CNFG = OP_PROGRAM ;

    if (g_nand_sw_config.eSecSize == SECTOR_SIZE_512)
        *NFI_CNFG |= SEL_SEC_512BYTE;
    else
        *NFI_CNFG &= (~SEL_SEC_512BYTE);
    if (g_nand_sw_config.bAUTO_FMT) //this flag is for ECC or no ecc
    {
        *NFI_CNFG |= AUTO_FMT_EN;// automatic fill ecc and fdm data to spare area by ECC hw module
        *NFI_CNFG |= HW_ECC_EN;  // enable or disable HW ECC engine
    }


    // setup NFI page format and I/O interface
    if ( NAND_IO_16BITS == MTD_NAND_IO_INTERFACE )
    {
        *NFI_CNFG |= DBYTE_EN_16BITS;
    }
	
    if(  ((*NFI_PAGEFMT & 0x003C)%4 > 0)  &&  (FALSE == g_nand_sw_config.bUsingDMA) )
    {
        //*NFI_CNFG |= BYTE_RW;
    }

    if (TEST_MIRCON_16bit_NAND==TRUE)
        //only for Mircon 16bit nand (NAND_MT29F4G16ABD) test
        *NFI_CSEL = CSEL(1); //choose cs1
    else
        *NFI_CSEL = CSEL(0); //choose cs0



    switch (*NFI_PAGEFMT & 0x003C)
    {
    case SPARE_16:
        spare_size = ( spare_size/(page_size/SECTOR_BYTES)) >= 16?(16*page_size/SECTOR_BYTES):spare_size;
        break;
    case SPARE_26:
        spare_size = 26*page_size/SECTOR_BYTES;
        break;
    case SPARE_27:
        spare_size = 27*page_size/SECTOR_BYTES;
        break;
    case SPARE_28:
        spare_size = 28*page_size/SECTOR_BYTES;
        break;
    case SPARE_32:
        spare_size = 32*page_size/SECTOR_BYTES;
        break;
    case SPARE_36:
        spare_size = 36*page_size/SECTOR_BYTES;
        break;
    case SPARE_40:
        spare_size = 40*page_size/SECTOR_BYTES;
        break;
    case SPARE_44:
        spare_size = 44*page_size/SECTOR_BYTES;
        break;
    case SPARE_48:
        spare_size = 48*page_size/SECTOR_BYTES;
        break;
    case SPARE_50:
        spare_size = 50*page_size/SECTOR_BYTES;
        break;
    case SPARE_52:
        spare_size = 52*page_size/SECTOR_BYTES;
        break;
    case SPARE_54:
        spare_size = 54*page_size/SECTOR_BYTES;
        break;
    case SPARE_56:
        spare_size = 56*page_size/SECTOR_BYTES;
        break;
    case SPARE_62:
        spare_size = 62*page_size/SECTOR_BYTES;
        break;
    case SPARE_63:
        spare_size = 63*page_size/SECTOR_BYTES;
        break;
    case SPARE_64:
        spare_size = 64*page_size/SECTOR_BYTES;
        break;
    default:
        break;
    }


    if (g_nand_sw_config.eSecSize == SECTOR_SIZE_1024)
        spare_size <<= 1;

    // FDM_ECC_BYTES + 24*14/8 = 12+42
    fdm_parity = (spare_size/(page_size/SECTOR_BYTES)) - FDM_ECC_BYTES;
    fdm_parity <<= 3;
    fdm_parity /= ECC_PARITY_NUM;

    if (fdm_parity >=ECC_60_BITS)       g_nand_sw_config.eECC_BIT = ECC_60_BITS;
    else if (fdm_parity >=ECC_56_BITS)  g_nand_sw_config.eECC_BIT = ECC_56_BITS;
    else if (fdm_parity >=ECC_52_BITS)  g_nand_sw_config.eECC_BIT = ECC_52_BITS;
    else if (fdm_parity >=ECC_48_BITS)  g_nand_sw_config.eECC_BIT = ECC_48_BITS;
    else if (fdm_parity >=ECC_44_BITS)  g_nand_sw_config.eECC_BIT = ECC_44_BITS;
    else if (fdm_parity >=ECC_40_BITS)  g_nand_sw_config.eECC_BIT = ECC_40_BITS;
    else if (fdm_parity >=ECC_36_BITS)  g_nand_sw_config.eECC_BIT = ECC_36_BITS;
    else if (fdm_parity >=ECC_32_BITS)  g_nand_sw_config.eECC_BIT = ECC_32_BITS;
    else if (fdm_parity >=ECC_28_BITS)  g_nand_sw_config.eECC_BIT = ECC_28_BITS;
    else if (fdm_parity >=ECC_24_BITS)  g_nand_sw_config.eECC_BIT = ECC_24_BITS;
    else if (fdm_parity >=ECC_22_BITS)  g_nand_sw_config.eECC_BIT = ECC_22_BITS;
    else if (fdm_parity >=ECC_20_BITS)  g_nand_sw_config.eECC_BIT = ECC_20_BITS;
    else if (fdm_parity >=ECC_18_BITS)  g_nand_sw_config.eECC_BIT = ECC_18_BITS;
    else if (fdm_parity >=ECC_16_BITS)  g_nand_sw_config.eECC_BIT = ECC_16_BITS;
    else if (fdm_parity >=ECC_14_BITS)  g_nand_sw_config.eECC_BIT = ECC_14_BITS;
    else if (fdm_parity >=ECC_12_BITS)  g_nand_sw_config.eECC_BIT = ECC_12_BITS;
    else if (fdm_parity >=ECC_10_BITS)  g_nand_sw_config.eECC_BIT = ECC_10_BITS;
    else if (fdm_parity >=ECC_8_BITS)   g_nand_sw_config.eECC_BIT = ECC_8_BITS;
    else if (fdm_parity >=ECC_6_BITS)   g_nand_sw_config.eECC_BIT = ECC_6_BITS;
    else                                g_nand_sw_config.eECC_BIT = ECC_4_BITS;

    //fdm_parity = 32 - 8;
    //fdm_parity <<= 3; ==192
    //fdm_parity /= ECC_PARITY_NUM; ==192/8 = 13.7
    //12bit ECC, protect data number = (1024+8)*8 = 8256
    *NFIECC_ENCCNFG = ENC_TNUM(g_nand_sw_config.eECC_BIT) | ENC_MS( (SECTOR_BYTES + FDM_ECC_BYTES) << 3 ) | ENC_NFI_MODE;

    *NFIECC_ENCCON = 0;
    *NFIECC_ENCCON = ENC_EN;
    if (g_nand_sw_config.bUsingDMA)
    {
        *NFI_CNFG |= AHB_MODE ;
        *NFI_STRADDR = ((uint32)p_data32 | 0x00000000);
    }

    // in most 512 page size NAND flash, you have to setup destination pointer to 1st half area
    if (nand_info->m_dev->m_cmd_set->m_program_1st_half_page.m_enable)
    {
        *NFI_CMD = nand_info->m_dev->m_cmd_set->m_program_1st_half_page.m_cmd;
        // wait til CMD is completely issued
        while ( *NFI_STA  & STATUS_CMD );
        *NFI_CON = NFI_RST;
        while ( *NFI_STA  & STATUS_CMD );
    }

    *NFI_CON = SEC_NUM(page_size/SECTOR_BYTES);

    // program cmd
    *NFI_CMD = nand_info->m_dev->m_cmd_set->m_program.m_cmd;
    // wait til CMD is completely issued
    while ( *NFI_STA  & STATUS_CMD );

    // fill 1~4 cycle addr
    *NFI_COLADDR = 0;
    *NFI_ROWADDR = row_addr;
    // no. of addr cycle
    //*NFI_ADDRNOB = addr_cycle;
    *NFI_ADDRNOB = COL_ADDR_NOB(column_addr_bytes) | ROW_ADDR_NOB(row_addr_bytes);
    // wait til ADDR is completely issued
    while ( *NFI_STA  & STATUS_ADDR );

    // prepare FDM data
    if ( AUTO_FMT_EN & (*NFI_CNFG))
    {
        if ((page_size/SECTOR_BYTES)>8)
        {
            volatile uint32* pFDMAddr = NFI_FDM0L;
            uint32 u4TmpFDMData = 0;
            uint8 *p1TmpSpare = (uint8*)p_spare32;

            for (i = 0; i < (page_size/SECTOR_BYTES); i++)
            {
                if (i>=8)
                {
                    k= i-8;
                    pFDMAddr = NFI_FDM8L + (k<<2); // i*16/4
                    for (j=0; j<(FDM_BYTES>>2); j++)
                    {
                        u4TmpFDMData = (uint32)(*p1TmpSpare++);
                        u4TmpFDMData |= (uint32)((*p1TmpSpare++) <<8);
                        u4TmpFDMData |= (uint32)((*p1TmpSpare++) <<16);
                        u4TmpFDMData |= (uint32)((*p1TmpSpare++) <<24);
                        *pFDMAddr++ = u4TmpFDMData;
                    }
                    if ((FDM_BYTES - (j<<2)) == 1)
                    {
                        u4TmpFDMData = (uint32)(*p1TmpSpare++);
                        u4TmpFDMData |= 0xFFFFFF00;
                        *pFDMAddr = u4TmpFDMData;
                    }
                    else if ((FDM_BYTES - (j<<2)) == 2)
                    {
                        u4TmpFDMData = (uint32)(*p1TmpSpare++);
                        u4TmpFDMData |= (uint32)((*p1TmpSpare++) <<8);
                        u4TmpFDMData |= 0xFFFF0000;
                        *pFDMAddr = u4TmpFDMData;
                    }
                    else if ((FDM_BYTES - (j<<2)) == 3)
                    {
                        u4TmpFDMData = (uint32)(*p1TmpSpare++);
                        u4TmpFDMData |= (uint32)((*p1TmpSpare++) <<8);
                        u4TmpFDMData |= (uint32)((*p1TmpSpare++) <<16);
                        u4TmpFDMData |= 0xFF000000;
                        *pFDMAddr = u4TmpFDMData;
                    }
                }
                else
                {
                    pFDMAddr = NFI_FDM0L + (i<<2); // i*16/4
                    for (j=0; j<(FDM_BYTES>>2); j++)
                    {
                        u4TmpFDMData = (uint32)(*p1TmpSpare++);
                        u4TmpFDMData |= (uint32)((*p1TmpSpare++) <<8);
                        u4TmpFDMData |= (uint32)((*p1TmpSpare++) <<16);
                        u4TmpFDMData |= (uint32)((*p1TmpSpare++) <<24);
                        *pFDMAddr++ = u4TmpFDMData;
                    }
                    if ((FDM_BYTES - (j<<2)) == 1)
                    {
                        u4TmpFDMData = (uint32)(*p1TmpSpare++);
                        u4TmpFDMData |= 0xFFFFFF00;
                        *pFDMAddr = u4TmpFDMData;
                    }
                    else if ((FDM_BYTES - (j<<2)) == 2)
                    {
                        u4TmpFDMData = (uint32)(*p1TmpSpare++);
                        u4TmpFDMData |= (uint32)((*p1TmpSpare++) <<8);
                        u4TmpFDMData |= 0xFFFF0000;
                        *pFDMAddr = u4TmpFDMData;
                    }
                    else if ((FDM_BYTES - (j<<2)) == 3)
                    {
                        u4TmpFDMData = (uint32)(*p1TmpSpare++);
                        u4TmpFDMData |= (uint32)((*p1TmpSpare++) <<8);
                        u4TmpFDMData |= (uint32)((*p1TmpSpare++) <<16);
                        u4TmpFDMData |= 0xFF000000;
                        *pFDMAddr = u4TmpFDMData;
                    }

                }

            }
        }
        else
        {
            volatile uint32* pFDMAddr = NFI_FDM0L;
            uint32 u4TmpFDMData = 0;
            uint8 *p1TmpSpare = (uint8*)p_spare32;

            for (i = 0; i < (page_size/SECTOR_BYTES) ; i++)
            {
                pFDMAddr = NFI_FDM0L + (i<<2); // i*16/4

                for (j=0; j<(FDM_BYTES>>2); j++)
                {
                    u4TmpFDMData = (uint32)(*p1TmpSpare++);
                    u4TmpFDMData |= (uint32)((*p1TmpSpare++) <<8);
                    u4TmpFDMData |= (uint32)((*p1TmpSpare++) <<16);
                    u4TmpFDMData |= (uint32)((*p1TmpSpare++) <<24);
                    *pFDMAddr++ = u4TmpFDMData;
                }
                if ((FDM_BYTES - (j<<2)) == 1)
                {
                    u4TmpFDMData = (uint32)(*p1TmpSpare++);
                    u4TmpFDMData |= 0xFFFFFF00;
                    *pFDMAddr = u4TmpFDMData;
                }
                else if ((FDM_BYTES - (j<<2)) == 2)
                {
                    u4TmpFDMData = (uint32)(*p1TmpSpare++);
                    u4TmpFDMData |= (uint32)((*p1TmpSpare++) <<8);
                    u4TmpFDMData |= 0xFFFF0000;
                    *pFDMAddr = u4TmpFDMData;
                }
                else if ((FDM_BYTES - (j<<2)) == 3)
                {
                    u4TmpFDMData = (uint32)(*p1TmpSpare++);
                    u4TmpFDMData |= (uint32)((*p1TmpSpare++) <<8);
                    u4TmpFDMData |= (uint32)((*p1TmpSpare++) <<16);
                    u4TmpFDMData |= 0xFF000000;
                    *pFDMAddr = u4TmpFDMData;
                }
            }

        }
    }


    if (g_nand_sw_config.bUsingDMA)
    {
        if (g_nand_sw_config.nfi_interrupt == TRUE)
        {
            /* set up NFI_INT_EN */
            *NFI_INTR_EN |= AHB_DONE_EN ;
        }
        else
        {
            *NFI_INTR_EN |= AHB_DONE_EN ;
        }
    }

    i = *NFI_INTR;  // read clear
    *NFI_INTR = i;

    *NFI_CON |= BURST_WR | NOB_DWORD ;
    //*NFI_CON |= BURST_WR;

    // program page data
    if (g_nand_sw_config.bAUTO_FMT)
    {
        if ( S_DONE != (ret=NAND_COMMON_FIFO_Write(c_timeout, g_nand_sw_config.bUsingDMA, p_data32, page_size)) )
        {
            goto end;
        }
    }
    else
    {
        if ( S_DONE != (ret=NAND_COMMON_FIFO_Write(c_timeout, g_nand_sw_config.bUsingDMA, p_data32, (page_size+spare_size))) )
        {
            goto end;
        }
    }

    timeout = c_timeout;
    NFI_Wait( ((*NFI_ADDRCNTR & 0xF800) >> 11) != (page_size/SECTOR_BYTES) , timeout);
    if ( 0 == timeout )
    {
        //dbg_print("NAND_COMMON_PageProgram wait NFI_ADDRCNTR(0x%x) timeout %s: %d \n", (*NFI_ADDRCNTR), __FILE__, __LINE__);
        ret = S_TIMEOUT;
        goto end;
    }

    //*NFI_INTR_EN |= WR_DONE_EN;

    if (g_nand_sw_config.nfi_interrupt == TRUE)
    {
        *NFI_INTR_EN |= WR_DONE_EN ;
    }
    else
    {
        *NFI_INTR_EN |= WR_DONE_EN ;
    }

    // program confirm
    *NFI_CMD = nand_info->m_dev->m_cmd_set->m_program_cnf.m_cmd;
    // wait til CMD is completely issued
    while ( *NFI_STA  & STATUS_CMD );

    timeout = c_timeout;
    {
        NFI_Wait( (WR_DONE != (*NFI_INTR&WR_DONE)), timeout); //clear INT status
        i =  *NFI_INTR; // read clear
        *NFI_INTR = i;
        *NFI_INTR_EN &= ~WR_DONE; // disable INT first
        if ( 0 == timeout)
        {
            //dbg_print("NAND_COMMON_PageProgram wait WR_DONE timeout %s: %d \n", __FILE__, __LINE__);
            ret = S_TIMEOUT;
            goto end;
        }
    }

    timeout = c_timeout;
    NFI_Wait( (S_DA_IN_PROGRESS==(ret=NAND_COMMON_ReadStatus_V4(c_timeout))), timeout);
    if ( 0 == timeout )
    {
        //dbg_print("NAND_COMMON_PageProgram wait ReadStatus timeout %s: %d \n", __FILE__, __LINE__);
        ret = S_TIMEOUT;
        goto end;
    }
    else if ( S_DA_NAND_UNKNOWN_ERR == ret )
    {
        ret = S_DA_NAND_PAGE_PROGRAM_FAILED;
        goto end;
    }
    else if ( S_DONE != ret )
    {
        goto end;
    }

end:
    *NFI_CON = 0;
    *NFIECC_ENCCON = 0x0;
    /*
    if ((0 == row_addr)&&(1024 ==  g_nand_sw_config.eSecSize)) // header block first page
    {
          nand_info->m_spare_size = backup_sparesize;
          *NFI_PAGEFMT = backup_pageFMT ;
    }*/
    //marked by qq - due to PIO mode added
    //*NFIECC_ENCCNFG = 0;
    //----------------------------------------------------------
    if (g_nand_sw_config.bRandomizerEn)
    {
        *NFI_RANDOM_CFG = 0x0;
    }
    //----------------------------------------------------------

    return ret;
}

//------------------------------------------------------------------------------
// Page+Spare Program Callback Function
//------------------------------------------------------------------------------
STATUS_E NAND_COMMON_PageSpareProgram_V4(uint32 row_addr,
        const uint32 *p_page_spare)
{//only used by write head block bad block mark, no need ECC
     STATUS_E  ret=S_DA_NAND_UNKNOWN_ERR;
     g_nand_sw_config.bAUTO_FMT = FALSE; // NO ECC
     ret =  NAND_COMMON_PageProgram_V4(NUTL_DEFAULT_TIMEOUT, row_addr,  p_page_spare, NULL);
     g_nand_sw_config.bAUTO_FMT = TRUE; //enable ECC
     return ret;
}


//------------------------------------------------------------------------------
// Spare Read Callback Function
//------------------------------------------------------------------------------
STATUS_E  NAND_COMMON_SpareRead_V4(
    const uint32  c_timeout
    ,const uint32  row_addr
    ,uint32 *p_spare32 /* MUST be 32bits alignment addr */
)
{
    uint32      page_size;
//  uint32      spare_addr;
    uint32      spare_size;
    uint32      column_addr_bits;
    uint32      addr_cycle;
    uint32      io_interface;
//  uint32      timeout = NFI_PSTA_TIMEOUT;
//  STATUS_E    ret=S_DA_NAND_UNKNOWN_ERR;
//  bool        bUsingDMA=TRUE;

    page_size = NUTL_PAGE_SIZE();
    spare_size = NUTL_SPARE_SIZE();
    column_addr_bits = NUTL_PAGE_ADDR_SHIFT_BITS();
    addr_cycle = NUTL_ADDR_CYCLE();
    io_interface = NUTL_IO_INTERFACE();

    // reset NFI
    NFI_Reset();

    return S_DONE;
}

//------------------------------------------------------------------------------
// Spare Program Callback Function
//------------------------------------------------------------------------------
STATUS_E  NAND_COMMON_SpareProgram_V4(
    const uint32  c_timeout
    ,const uint32  row_addr
    ,const uint32 *p_spare32 /* MUST be 32bits alignment addr */
)
{
    uint32      page_size;
//  uint32      spare_addr;
    uint32      spare_size;
    uint32      column_addr_bits;
    uint32      addr_cycle;
    uint32      io_interface;
//  uint32      timeout = NFI_PSTA_TIMEOUT;
//  STATUS_E    ret=S_DA_NAND_UNKNOWN_ERR;
//  bool        bUsingDMA=TRUE;

    page_size = NUTL_PAGE_SIZE();
    spare_size = NUTL_SPARE_SIZE();
    column_addr_bits = NUTL_PAGE_ADDR_SHIFT_BITS();
    addr_cycle = NUTL_ADDR_CYCLE();
    io_interface = NUTL_IO_INTERFACE();

    // reset NFI
    NFI_Reset();

    return S_DONE;
}

//------------------------------------------------------------------------------
// CopyBack Callback Function
//------------------------------------------------------------------------------
STATUS_E  NAND_COMMON_CopyBack_V4(
    const uint32  c_timeout
    ,const uint32  src_row_addr
    ,const uint32  dest_row_addr
)
{
    uint32  page_size;
    uint32  spare_size;
    uint32  column_addr_bits;
    uint32  addr_cycle;
    uint32  io_interface;
//  uint32  timeout = NFI_PSTA_TIMEOUT;
    STATUS_E    ret=S_DA_NAND_UNKNOWN_ERR;

    page_size = NUTL_PAGE_SIZE();
    spare_size = NUTL_SPARE_SIZE();
    column_addr_bits = NUTL_PAGE_ADDR_SHIFT_BITS();
    addr_cycle = NUTL_ADDR_CYCLE();
    io_interface = NUTL_IO_INTERFACE();

// !!
// !! We don't maintain CopyBack operation after MT6235/MT6238/MT6516
// !!

    // reset NFI
    NFI_Reset();

    return ret;
}

#endif

#endif

