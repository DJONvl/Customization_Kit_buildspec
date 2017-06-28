/* Copyright Statement:
*
* This software/firmware and related documentation ("MediaTek Software") are
* protected under relevant copyright laws. The information contained herein is
* confidential and proprietary to MediaTek Inc. and/or its licensors. Without
* the prior written permission of MediaTek inc. and/or its licensors, any
* reproduction, modification, use or disclosure of MediaTek Software, and
* information contained herein, in whole or in part, shall be strictly
* prohibited.
* 
* MediaTek Inc. (C) 2010. All rights reserved.
* 
* BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
* THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
* RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO RECEIVER
* ON AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL
* WARRANTIES, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED
* WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR
* NONINFRINGEMENT. NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH
* RESPECT TO THE SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY,
* INCORPORATED IN, OR SUPPLIED WITH THE MEDIATEK SOFTWARE, AND RECEIVER AGREES
* TO LOOK ONLY TO SUCH THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO.
* RECEIVER EXPRESSLY ACKNOWLEDGES THAT IT IS RECEIVER'S SOLE RESPONSIBILITY TO
* OBTAIN FROM ANY THIRD PARTY ALL PROPER LICENSES CONTAINED IN MEDIATEK
* SOFTWARE. MEDIATEK SHALL ALSO NOT BE RESPONSIBLE FOR ANY MEDIATEK SOFTWARE
* RELEASES MADE TO RECEIVER'S SPECIFICATION OR TO CONFORM TO A PARTICULAR
* STANDARD OR OPEN FORUM. RECEIVER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S
* ENTIRE AND CUMULATIVE LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE
* RELEASED HEREUNDER WILL BE, AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE
* MEDIATEK SOFTWARE AT ISSUE, OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE
* CHARGE PAID BY RECEIVER TO MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
*
* The following software/firmware and/or related documentation ("MediaTek
* Software") have been modified by MediaTek Inc. All revisions are subject to
* any receiver's applicable license agreements with MediaTek Inc.
*/

#ifndef MT_EMI_H
#define MT_EMI_H

//#define DDR_RESERVE_MODE
#include "sys/types.h"
#include "driver/hw_types.h"

#define DRAM_CFG_ENABLE_CALIBRATION 0x00000001
#define DRAM_CFG_ENABLE_DUMP        0x80000000

#define RAMSIZE_BEGIN_PATTERN	"<<<RAM_BEGIN>>>"
#define RAMSIZE_END_PATTERN		"<<<RAM_END>>>"
#define RAMRW_PATTERN           "<<<RAM_RW>>>"

#define RAM_STEP_UINT	16*1024		//16M
#define DRAM_MAX_SIZE	0xFFFFFFFF		//8G

typedef struct {
   uint32	m_baseaddr;
   volatile uint32 	*m_reg_emi_con;
} HW_BB_EMI_Bank_S;

typedef struct {
   HW_BB_EMI_Bank_S	m_bank[MAX_CS];
   bool				m_is_dram_supported;
   uint32              m_max_dram_size;
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

/* DRAM rank info */
typedef struct {
   u64 start;
   u64 size;
} mem_desc_t;

/* mblock is used by CPU */
typedef struct {
   u64 start;
   u64 size;
   u32 rank;	/* rank the mblock belongs to */
} mblock_t;

typedef struct {
   u32 mblock_num;
   mblock_t mblock[4];
} mblock_info_t;

typedef struct {
   u32 rank_num;
   mem_desc_t rank_info[4];
} dram_info_t;
enum reserve_rank {
   RANK0,	/* first rank */
   RANKMAX,	/* as high rank as possible */
};
typedef volatile unsigned int *V_UINT32P;
int get_dram_rank_nr (void);
void get_dram_rank_size(unsigned int dram_rank_size[]);
#define DDR1  1
#define LPDDR2  2
#define LPDDR3  3
#define PCDDR3  4
typedef struct
{
   int   sub_version;            // sub_version: 0x1 for new version
   int  type;                /* 0x0000 : Invalid
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
   int   EMI_CONH_VAL;
   //int   DRAMC_DRVCTL0_VAL;      //@0x40B8               -> customized TX I/O driving
   //int   DRAMC_DRVCTL1_VAL;      //@0x40BC               -> customized TX I/O driving
   int   DRAMC_ACTIM_VAL;        //@0x4000
   int   DRAMC_GDDR3CTL1_VAL;    //@0x40F4
   int   DRAMC_CONF1_VAL;        //@0x4004
   int   DRAMC_DDR2CTL_VAL;      //@0x407C 
   int   DRAMC_TEST2_3_VAL;      //@0x4044
   int   DRAMC_CONF2_VAL;        //@0x4008
   int   DRAMC_PD_CTRL_VAL;      //@0x41DC
   //int   DRAMC_PADCTL3_VAL;      //@0x4014               -> customized TX DQS delay
   //int   DRAMC_DQODLY_VAL;       //@0x4200~0x420C        -> customized TX DQ delay
   //int   DRAMC_ADDR_OUTPUT_DLY;  // for E1 DDR2 only
   //int   DRAMC_CLK_OUTPUT_DLY;   // for E1 DDR2 only
   int   DRAMC_ACTIM1_VAL;       //@0x41E8
   int   DRAMC_MISCTL0_VAL;      //@0x40FC
   int   DRAMC_ACTIM05T_VAL;     //@0x41F8
   int   DRAMC_RKCFG_VAL;
   int   DRAMC_TEST2_4_VAL;    
   int   DRAM_RANK_SIZE[4];
   int   reserved[10];
   union
   {
      struct
      {
         int   iLPDDR2_MODE_REG_1;
         int   iLPDDR2_MODE_REG_2;
         int   iLPDDR2_MODE_REG_3;
         int   iLPDDR2_MODE_REG_5;
         int   iLPDDR2_MODE_REG_10;
         int   iLPDDR2_MODE_REG_63;
      };
      struct
      {
         int   iDDR1_MODE_REG;
         int   iDDR1_EXT_MODE_REG;
      };
      struct
      {
         int   iPCDDR3_MODE_REG0;
         int   iPCDDR3_MODE_REG1;
         int   iPCDDR3_MODE_REG2;
         int   iPCDDR3_MODE_REG3;
      };
      struct
      {
         int   iLPDDR3_MODE_REG_1;
         int   iLPDDR3_MODE_REG_2;
         int   iLPDDR3_MODE_REG_3;
         int   iLPDDR3_MODE_REG_5;
         int   iLPDDR3_MODE_REG_10;
         int   iLPDDR3_MODE_REG_63;
      };
   };
} EMI_SETTINGS;


typedef struct {
   // external ram type 
   HW_RAMType_E		m_type;
   // external ram chip-select 
   HW_ChipSelect_E		m_chip_select;
   // external ram base address 
   uint32				m_baseaddr;
   // external ram size 
   uint32				m_size;
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
      EMI_SETTINGS ev22;

   } m_emi;     
   // internal sram size 
   uint32				m_int_sram_size;
} HW_RAM_Config_S;

uint32 mt_set_emis(uint8* emi, uint32 len, bool use_default); //array of emi setting.
extern int num_of_emi_records;
extern int emi_setting_index;
extern EMI_SETTINGS emi_settings[];
extern EMI_SETTINGS *default_emi_setting;
extern EMI_SETTINGS emi_setting_default_lpddr2;
extern EMI_SETTINGS emi_setting_default_lpddr3;  

int mt_get_dram_type (void); 
void release_dram(void);
unsigned int is_TB_setting(void);
unsigned long long  get_dram_size();

/* 0: invalid */
/* 1: mDDR1 */
/* 2: LPDDR2 */
/* 3: LPDDR3 */
/* 4: PCDDR3 */

#endif
