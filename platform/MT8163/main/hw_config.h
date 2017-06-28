/*******************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of MediaTek Inc. (C) 2013
*
*  History:
*  YYYYMMDD MTK12345 Description
*  --------------------
*  20131116 MTK81353 Initial version.
*
*
*******************************************************************************/

#ifndef _HW_CONFIG_H_
#define _HW_CONFIG_H_

#include "sw_types.h"
#include "download.h"
#include "mtk_mcu.h"
#include "reg_base.h"

#define FREE_INT_SYSRAM_BEGIN_ADDR	((uint32)0x42000000)
#define FREE_EXT_RAM_BEGIN_ADDR		((uint32)0x43000000)
#define EXT_RAM_BEGIN_ADDR			((uint32)0x44000000)
#define INT_SYSRAM_END_ADDR         ((uint32)0x45000000)


#define RAMSIZE_BEGIN_PATTERN	"<<<RAM_BEGIN>>>"
#define RAMSIZE_END_PATTERN		"<<<RAM_END>>>"
#define RAMRW_PATTERN           "<<<RAM_RW>>>"

//------------------------------------------------------------------------------
// ARM System
//------------------------------------------------------------------------------
#if defined(BB_MT8163)
	#define __MTK_BB_REG_BASEADDRESS_GROUP11__
	#define __NFI_SUPPORT_TLC__
#endif

#define __MTK_RTC_V4__

#define PDN_CON0				((volatile uint16 *)(CONFIG_BASE + 0x0300))
#define PDN_CON1				((volatile uint16 *)(CONFIG_BASE + 0x0304))
#define PDN_CLR0_MT6218B		((volatile uint16 *)(CONFIG_BASE + 0x0320))
#define PDN_CLR1_MT6218B		((volatile uint16 *)(CONFIG_BASE + 0x0324))
#define PDN_CLR0_TK6516			((volatile uint16 *)(CONFIG_BASE + 0x0340))
#define PDN_CLR1_TK6516			((volatile uint16 *)(CONFIG_BASE + 0x0344))
#define MDPLL_MT6227			((volatile uint16 *)(CONFIG_BASE + 0x0100))
#define CLK_CON_MT6227			((volatile uint16 *)(CONFIG_BASE + 0x0108))
#define MCUCLK_CON_MT6227		((volatile uint16 *)(CONFIG_BASE + 0x0114))
#define MDPLL_MT6228			((volatile uint16 *)(CONFIG_BASE + 0x0100))
#define CLK_CON_MT6228			((volatile uint16 *)(CONFIG_BASE + 0x0110))
#define MCUCLK_CON_MT6228		((volatile uint16 *)(CONFIG_BASE + 0x0118))
#define MCUCLK_CON_MT6238		((volatile uint16 *)(CONFIG_BASE + 0x0118))
#define PDN_CLR3_MT6253			((volatile uint16 *)(CONFIG_BASE + 0x032C))
#define GC_CLR3_MT6268			((volatile uint16 *)(CONFIG_BASE + 0x0320))
#define MCU_CG_CLR0_MT6276		((volatile uint16 *)(CONFIG_BASE + 0x0320))
#define MCU_CG_CLR0_MT6255		((volatile uint16 *)(CONFIG_BASE + 0x0720))
#define MCU_CG_CLR1_MT6255		((volatile uint16 *)(CONFIG_BASE + 0x0724))
#define MCU_CG_CLR0_MT6573		((volatile uint16 *)(CONFIG_BASE + 0x0318))
#define PDN_CLR3_MT6251			((volatile uint16 *)(CONFIG_BASE + 0x032C))


//MT8163
#define EMI_CONA_MT8163		((volatile uint32 *)(EMI_BASE + 0x0000))
#define EMI_CONB_MT8163		((volatile uint32 *)(EMI_BASE + 0x0004))
#define EMI_CONC_MT8163		((volatile uint32 *)(EMI_BASE + 0x0008))
#define EMI_COND_MT8163		((volatile uint32 *)(EMI_BASE + 0x000C))


//------------------------------------------------------------------------------
// DMA Register                                                          
//------------------------------------------------------------------------------
#define DMA_GLBSTA				((volatile uint32 *)(DMA_BASE + 0x0000))
#define DMA1_SRC				((volatile uint32 *)(DMA_BASE + 0x0100))
#define DMA1_DST				((volatile uint32 *)(DMA_BASE + 0x0104))
#define DMA1_COUNT				((volatile uint32 *)(DMA_BASE + 0x0110))
#define DMA1_CON				((volatile uint32 *)(DMA_BASE + 0x0114))
#define DMA1_START				((volatile uint32 *)(DMA_BASE + 0x0118))
#define DMA1_LIMITER			((volatile uint32 *)(DMA_BASE + 0x0128))



//------------------------------------------------------------------------------

typedef struct {
	BBCHIP_TYPE		m_bbchip_type;
	uint32			m_hw_code;
	uint32			m_hw_ver;
} HW_BB_CHIP_ID_S;

typedef struct {
	uint64	m_baseaddr;
	volatile uint32 	*m_reg_emi_con;
} HW_BB_EMI_Bank_S;

typedef struct {
	HW_BB_EMI_Bank_S	m_bank[MAX_CS];
	bool				m_is_dram_supported;
	uint64              m_max_dram_size;
	uint32              m_dram_size_det_step;
	volatile uint32 	*m_reg_emi_gen_a;
	volatile uint32 	*m_reg_emi_gen_b;
	volatile uint32 	*m_reg_emi_gen_c;
	volatile uint32 	*m_reg_emi_gen_d;
	volatile uint32 	*m_reg_emi_con_i;
	volatile uint32 	*m_reg_emi_con_j;
	volatile uint32 	*m_reg_emi_con_k;
	volatile uint32 	*m_reg_emi_con_l;
	volatile uint32 	*m_reg_emi_con_m;
	volatile uint32 	*m_reg_emi_con_n;
	volatile uint32 	*m_reg_emi_del_a;
	volatile uint32 	*m_reg_emi_del_b;
	volatile uint32 	*m_reg_emi_del_c;
	volatile uint32 	*m_reg_emi_del_d;
	volatile uint32 	*m_reg_emi_del_e;
	volatile uint32 	*m_reg_emi_del_f;
	volatile uint32 	*m_reg_emi_del_g;
	volatile uint32 	*m_reg_emi_del_h;
	volatile uint32 	*m_reg_emi_del_i;
	volatile uint32 	*m_reg_emi_del_j;
	volatile uint32 	*m_reg_emi_arb_a;
	volatile uint32 	*m_reg_emi_arb_b;
	volatile uint32 	*m_reg_emi_arb_c;
	volatile uint32 	*m_reg_emi_arb_d;
	volatile uint32 	*m_reg_emi_arb_e;
	volatile uint32 	*m_reg_emi_arb_f;
	volatile uint32 	*m_reg_emi_arb_g;
	volatile uint32 	*m_reg_emi_arb_h;
	volatile uint32 	*m_reg_emi_arb_i;
	volatile uint32     *m_reg_emi_drv_a;
	volatile uint32     *m_reg_emi_drv_b;
	volatile uint32     *m_reg_emi_iocl;
	volatile uint32     *m_reg_emi_drct;
	volatile uint32     *m_reg_emi_ppct;
	volatile uint32     *m_reg_emi_slct;
	volatile uint32     *m_reg_emi_abct;
	volatile uint32     *m_reg_emi_dutb;
} HW_BB_EMI_Config_S;

typedef struct {
	volatile uint16 	*m_reg_highspeed_uart;
	volatile uint16 	*m_reg_sample_count;
	volatile uint16 	*m_reg_sample_point;
} HW_BB_UART_Config_S;

typedef struct {
	uint32	m_dummy;
} HW_BB_DMA_Config_S;

typedef struct {
	uint32	m_dummy;
} HW_BB_INT_Config_S;

typedef struct {
	uint16	m_timeout_shift_bits;
} HW_BB_RGU_Config_S;

typedef struct {
	volatile uint16 	*m_reg_gpio_mod_ctrl_4;
	uint32	m_nfi_mask;
	uint32	m_nfi_cs0;
	uint32	m_nfi_cs1;
} HW_BB_GPIO_Config_S;

typedef struct {
	volatile uint16 	*m_reg_power_down_clear_0;
	volatile uint16 	*m_reg_power_down_clear_1;
} HW_BB_PDN_Config_S;

typedef struct {
	bool	m_nfi_v2;
} HW_BB_NFI_Config_S;

struct _HW_Device_Config_S;
typedef struct _HW_Device_Config_S	HW_Device_Config_S;
typedef STATUS_E  (*CB_SETUP_PLL)(HW_Device_Config_S  *p_hw_dev_cfg);

typedef struct {
	volatile uint16 	*m_reg_mdpll;
	volatile uint16 	*m_reg_clk_con;
	volatile uint16 	*m_reg_mcuclk_con;
	CB_SETUP_PLL		m_cb_setup_pll;
} HW_BB_CLK_Config_S;

typedef struct {
	const HW_BB_CHIP_ID_S			m_ID;
	const HW_BB_EMI_Config_S		*m_EMI;
	const HW_BB_UART_Config_S		*m_UART;
	const HW_BB_DMA_Config_S		*m_DMA;
	const HW_BB_INT_Config_S		*m_INT;
	const HW_BB_RGU_Config_S		*m_RGU;
	const HW_BB_GPIO_Config_S		*m_GPIO;
	const HW_BB_PDN_Config_S		*m_PDN;
	const HW_BB_NFI_Config_S		*m_NFI;
	const HW_BB_CLK_Config_S		*m_CLK;
	const bool						m_SB_Support;
	const uint32					m_INT_SYSRAM_SIZE;
} HW_BB_Config_S;

typedef struct {
	// flash chip-select 
	HW_ChipSelect_E		m_chip_select[MAX_DIE_IN_MCP];
	// NOR flash base address 
	uint32				m_nor_baseaddr[MAX_DIE_IN_MCP];
	// NOR flash setting 
	uint32				m_nor_emi_con;
} HW_Storage_NOR_Config_S;

typedef struct {
	// flash chip-select 
	HW_ChipSelect_E		m_chip_select;
	// NAND flash setting 
	uint32				m_nand_acccon;
} HW_Storage_NAND_Config_S;


#define MCP_MASK_DEV  0xFF00
#define MCP_MASK_RAM  0x00FF
#define MCP_NAND_DEV  0x0100
#define MCP_EMMC_DEV  0x0200
#define MCP_DDR1_RAM  0x0001
#define MCP_DDR2_RAM  0x0002
#define MCP_DDR3_RAM  0x0003
#define MCP_DISC_DDR1 MCP_DDR1_RAM
#define MCP_DISC_DDR2 MCP_DDR2_RAM
#define MCP_DISC_DDR3 MCP_DDR3_RAM
#define MCP_NAND_DDR1 (MCP_NAND_DEV | MCP_DDR1_RAM)
#define MCP_NAND_DDR2 (MCP_NAND_DEV | MCP_DDR2_RAM)
#define MCP_NAND_DDR3 (MCP_NAND_DEV | MCP_DDR3_RAM)
#define MCP_EMMC_DDR1 (MCP_EMMC_DEV | MCP_DDR1_RAM)
#define MCP_EMMC_DDR2 (MCP_EMMC_DEV | MCP_DDR2_RAM)
#define MCP_EMMC_DDR3 (MCP_EMMC_DEV | MCP_DDR3_RAM)
#define MCP_NAND_ID_LEN 4
#define MCP_EMMC_ID_LEN 9


typedef struct
{
    int   sub_version;            // sub_version: 0x1 for new version
    int  type;             /* 0x0000 : Invalid
                                 0x0001 : Discrete DDR1
                                 0x0002 : Discrete LPDDR2
                                 0x0003 : Discrete LPDDR3
                                 0x0004 : Discrete PCDDR3
                                 0x0101 : MCP(NAND+DDR1)
                                 0x0102 : MCP(NAND+LPDDR2)
                                 0x0103 : MCP(NAND+LPDDR3)
                                 0x0104 : MCP(NAND+PCDDR3)
                                 0x0201 : MCP(eMMC+DDR1)
                                 0x0202 : MCP(eMMC+LPDDR2)
                                 0x0203 : MCP(eMMC+LPDDR3)
                                 0x0204 : MCP(eMMC+PCDDR3)
                              */
    int   id_length;              // EMMC and NAND ID checking length
    int   fw_id_length;              // FW ID checking length
    char  ID[16];
    char  fw_id[8];               // To save fw id
    int   EMI_CONA_VAL;           //@0x3000
    int   DRAMC_DRVCTL0_VAL;      //@0x40B8               -> customized TX I/O driving
    int   DRAMC_DRVCTL1_VAL;      //@0x40BC               -> customized TX I/O driving
    int   DRAMC_ACTIM_VAL;        //@0x4000
    int   DRAMC_GDDR3CTL1_VAL;    //@0x40F4
    int   DRAMC_CONF1_VAL;        //@0x4004
    int   DRAMC_DDR2CTL_VAL;      //@0x407C 
    int   DRAMC_TEST2_3_VAL;      //@0x4044
    int   DRAMC_CONF2_VAL;        //@0x4008
    int   DRAMC_PD_CTRL_VAL;      //@0x41DC
    int   DRAMC_PADCTL3_VAL;      //@0x4014               -> customized TX DQS delay
    int   DRAMC_DQODLY_VAL;       //@0x4200~0x420C        -> customized TX DQ delay
    int   DRAMC_ADDR_OUTPUT_DLY;  // for E1 DDR2 only
    int   DRAMC_CLK_OUTPUT_DLY;   // for E1 DDR2 only
    int   DRAMC_ACTIM1_VAL;       //@0x41E8
    int   DRAMC_MISCTL0_VAL;      //@0x40FC
    int   DRAMC_ACTIM05T_VAL;     //@0x41F8
    int   DRAM_RANK_SIZE[4];
    int   reserved[10];

    union
    {
        struct
        {
            int   LPDDR2_MODE_REG_1;
            int   LPDDR2_MODE_REG_2;
            int   LPDDR2_MODE_REG_3;
            int   LPDDR2_MODE_REG_5;
            int   LPDDR2_MODE_REG_10;
            int   LPDDR2_MODE_REG_63;
        };
        struct
        {
            int   DDR1_MODE_REG;
            int   DDR1_EXT_MODE_REG;
        };
        struct
        {
            int   PCDDR3_MODE_REG0;
            int   PCDDR3_MODE_REG1;
            int   PCDDR3_MODE_REG2;
            int   PCDDR3_MODE_REG3;
            int   PCDDR3_MODE_REG4;
            int   PCDDR3_MODE_REG5;			
        };
        struct
        {
            int   LPDDR3_MODE_REG_1;
            int   LPDDR3_MODE_REG_2;
            int   LPDDR3_MODE_REG_3;
            int   LPDDR3_MODE_REG_5;
            int   LPDDR3_MODE_REG_10;
            int   LPDDR3_MODE_REG_63;
        };
    };
} EMI_SETTINGS_v16;


#define DRAM_CFG_ENABLE_CALIBRATION 0x00000001
#define DRAM_CFG_ENABLE_DUMP        0x80000000
typedef struct {
	// external ram type 
	HW_RAMType_E		m_type;
	// external ram chip-select 
	HW_ChipSelect_E		m_chip_select;
	// external ram base address 
	uint64				m_baseaddr;
	// external ram size 
	uint64				m_size;
	// external sram setting 
	uint32				m_sram_emi_con;
	// external dram setting 
	uint32				m_emi_gen_a;
	uint32				m_emi_gen_b;
	uint32				m_emi_gen_c;
	uint32				m_emi_gen_d;
	uint32				m_emi_con_i;
	uint32				m_emi_con_i_ext;
	uint32				m_emi_con_j;
	uint32				m_emi_con_k;
	uint32				m_emi_con_l;
	uint32				m_emi_con_m;
	uint32				m_emi_con_n;
	uint32				m_emi_del_a;
	uint32				m_emi_del_b;
	uint32				m_emi_del_c;
	uint32				m_emi_del_d;
	uint32				m_emi_del_e;
	uint32				m_emi_del_f;
	uint32				m_emi_del_g;
	uint32				m_emi_del_h;
	uint32				m_emi_del_i;
	uint32				m_emi_del_j;
	uint32				m_emi_arb_a;
	uint32				m_emi_arb_b;
	uint32				m_emi_arb_c;
	uint32				m_emi_arb_d;
	uint32				m_emi_arb_e;
	uint32				m_emi_arb_f;
	uint32				m_emi_arb_g;
	uint32				m_emi_arb_h;
	uint32				m_emi_arb_i;
	uint32              m_emi_drv_a;
	uint32              m_emi_drv_b;
	uint32              m_emi_iocl;
	uint32              m_emi_drct;
	uint32              m_emi_ppct;
	uint32              m_emi_slct;
	uint32              m_emi_abct;
	uint32              m_emi_dutb;
    
    uint32              m_ver; 
    uint32              m_cfg;
    union
    {
        EMI_SETTINGS_v16 ev16;
        
    } m_emi;     
	// internal sram size 
	uint32				m_int_sram_size;
} HW_RAM_Config_S;

typedef struct {
	// external clock 
	EXT_CLOCK			m_ext_clock;
} HW_CLK_Config_S;

struct _HW_Device_Config_S {
	// storage config 
	HW_Storage_NOR_Config_S		m_nor_cfg;
	HW_Storage_NAND_Config_S	m_nand_cfg;
	// ram config 
	HW_RAM_Config_S				m_ram_cfg;
	// clock config 
	HW_CLK_Config_S				m_clk_cfg;
	// baseband config 
	const HW_BB_Config_S	*	m_hw_bbcfg;
};

typedef struct {
	// bbchip 
	BBCHIP_TYPE			m_bbchip;
	uint16				m_bbchip_hw_code;
	uint16				m_bbchip_hw_ver;
	uint16				m_bbchip_sw_ver;
	// NOR storagte report 
	STATUS_E			m_nor_ret;
	HW_ChipSelect_E		m_nor_chip_select[2];
	uint16				m_nor_flash_id;
	uint32				m_nor_flash_size;
	uint16				m_nor_flash_dev_code_1;
	uint16				m_nor_flash_dev_code_2;
	uint16				m_nor_flash_dev_code_3;
	uint16				m_nor_flash_dev_code_4;
	STATUS_E			m_nor_flash_otp_status;
	uint32				m_nor_flash_otp_size;
	// NAND storagte report 
	STATUS_E			m_nand_ret;
	HW_ChipSelect_E		m_nand_chip_select;
	uint16				m_nand_flash_id;
	uint64				m_nand_flash_size;
	uint16				m_nand_flash_dev_code_1;
	uint16				m_nand_flash_dev_code_2;
	uint16				m_nand_flash_dev_code_3;
	uint16				m_nand_flash_dev_code_4;
	uint16				m_nand_flash_dev_code_5;
	uint16				m_nand_flash_dev_code_6;
	uint16				m_nand_flash_dev_code_7;
	uint16				m_nand_pagesize;
	uint16				m_nand_sparesize;
	uint16				m_nand_pages_per_block;
	uint8				m_nand_io_interface;
	uint8				m_nand_addr_cycle;
    	uint8				m_nand_bmt_exist;
	// internal sram report 
	STATUS_E			m_int_sram_ret;
	uint32				m_int_sram_size;
	// external ram report 
	STATUS_E			m_ext_ram_ret;
	HW_RAMType_E		m_ext_ram_type;
	HW_ChipSelect_E		m_ext_ram_chip_select;
	uint64				m_ext_ram_size;
	// random id
	uint8				m_random_id[16];
	// eMMC storage report
	STATUS_E			m_emmc_ret;
	uint64				m_emmc_boot1_size;
	uint64				m_emmc_boot2_size;
	uint64				m_emmc_rpmb_size;
	uint64				m_emmc_gp1_size;
	uint64				m_emmc_gp2_size;
	uint64				m_emmc_gp3_size;
	uint64				m_emmc_gp4_size;
	uint64				m_emmc_ua_size;
	uint32				m_emmc_cid[4];
	uint8				m_emmc_fwver[8];
	
	// SD/MMC storage report
	STATUS_E			m_sdmmc_ret;
	uint64				m_sdmmc_ua_size;
	uint32				m_sdmmc_cid[4];
} HW_DetectionResult_S;

#ifdef __cplusplus
extern "C" {
#endif

extern HW_Device_Config_S		g_HW_DevCfg;
extern HW_DetectionResult_S  	g_HW_DetectionResult;
extern uint32 g_FreeIntSysRAMBeginAddr;
extern uint32 g_FreeIntTCMBeginAddr;
extern uint32 g_FreeExtRAMBeginAddr;
extern uint32 g_ExtRAMBeginAddr;

#define GetEMIConfig() (g_HW_DevCfg.m_hw_bbcfg->m_EMI)
#define GetRAMConfig() (g_HW_DevCfg.m_ram_cfg)

#define EMI_BANK_BASEADDR(p_hw_bbcfg, ecs)			((CS_WITH_DECODER==ecs)?CS_WITH_DECODER:p_hw_bbcfg->m_EMI->m_bank[ecs].m_baseaddr)
#define EMI_BANK_REG(p_hw_bbcfg, ecs)				(p_hw_bbcfg->m_EMI->m_bank[ecs].m_reg_emi_con)

extern int32		mem_overlap_copy(void *dest, void *src, uint32 len);
extern void			mem_set(void *dest, uint8 default_value, uint32 len);
	
extern BBCHIP_TYPE  DetectBasebandChip(void);
extern STATUS_E  	SetupEMI(HW_ChipSelect_E  cs, uint32  emi_setting);
extern STATUS_E  	DetectIntSRAM(uint32 *p_size);
extern STATUS_E  	DetectExtSRAM(HW_RAMType_E *p_type, HW_ChipSelect_E *p_chip_select, uint32 *p_baseaddr, uint32 *p_size);
extern STATUS_E  	DetectExtDRAM(HW_RAMType_E *p_type, HW_ChipSelect_E *p_chip_select, uint64 *p_baseaddr, uint64 *p_size);
extern void			SchematicSetup(HW_DetectionResult_S  *p_result);
	
#ifdef __cplusplus
}
#endif

#endif
