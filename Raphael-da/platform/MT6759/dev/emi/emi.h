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

#include "dramc_common.h"
#include "boot/dev_interface/emi_interface.h"
#include <autoconfig.h>

#define _FLASH_TOOL_DA
 #ifdef _FLASH_TOOL_DA
  #include <sys/types.h>
#else
  #include "typedefs.h"
#endif

#define RAMSIZE_BEGIN_PATTERN	"<<<RAM_BEGIN>>>"
#define RAMSIZE_END_PATTERN		"<<<RAM_END>>>"
#define RAMRW_PATTERN           "<<<RAM_RW>>>"

#define RAM_STEP_UINT	16*1024		//16M
#define DRAM_MAX_SIZE	0xFFFFFFFF		//8G
#define DRAM_BASE DA_DRAM_BASE_ADDRESS
#define DDR_BASE DRAM_BASE
#define COMBO_MCP

#if !__ETT__ 
#if (FOR_DV_SIMULATION_USED==0)
//#define DDR_RESERVE_MODE
//#define COMBO_MCP
#define DDR_BASE DRAM_BASE
//#define DRAM_CALIB_LOG
//#define LAST_DRAMC
#endif
#endif

#if (FOR_DV_SIMULATION_USED==1 || SW_CHANGE_FOR_SIMULATION==1)
typedef unsigned long long u64;
#endif

int get_dram_rank_nr (void);
void get_dram_rank_size(u64 dram_rank_size[]);

typedef struct _AC_TIMING_EXTERNAL_T
{
    // U 00
    U32 AC_TIME_EMI_FREQUENCY      :16;
    U32 AC_TIME_EMI_TRAS           :8;
    U32 AC_TIME_EMI_TRP            :8;

    // U 01
    U32 AC_TIME_EMI_TRPAB          :8;
    U32 AC_TIME_EMI_TRC            :8;
    U32 AC_TIME_EMI_TRFC           :8;
    U32 AC_TIME_EMI_TRFCPB         :8;

    // U 02
    U32 AC_TIME_EMI_TXP            :8;
    U32 AC_TIME_EMI_TRTP           :8;
    U32 AC_TIME_EMI_TRCD           :8;
    U32 AC_TIME_EMI_TWR            :8;

    // U 03
    U32 AC_TIME_EMI_TWTR           :8;
    U32 AC_TIME_EMI_TRRD           :8;
    U32 AC_TIME_EMI_TFAW           :8;
    U32 AC_TIME_EMI_TRTW_ODT_OFF   :4;
    U32 AC_TIME_EMI_TRTW_ODT_ON    :4;

    // U 04    
    U32 AC_TIME_EMI_REFCNT         :8; //(REFFRERUN = 0)
    U32 AC_TIME_EMI_REFCNT_FR_CLK  :8; //(REFFRERUN = 1)
    U32 AC_TIME_EMI_TXREFCNT       :8;
    U32 AC_TIME_EMI_TZQCS          :8;

    // U 05 
    U32 AC_TIME_EMI_TRTPD            :8; // LP4/LP3, // Olympus new 
    U32 AC_TIME_EMI_TWTPD            :8; // LP4/LP3, // Olympus new 
    U32 AC_TIME_EMI_TMRR2W_ODT_OFF   :8; // LP4      // Olympus new 
    U32 AC_TIME_EMI_TMRR2W_ODT_ON    :8; // LP4      // Olympus new 

    // U 06
    // Byte0
    U32 AC_TIME_EMI_TRAS_05T          :2;
    U32 AC_TIME_EMI_TRP_05T           :2;
    U32 AC_TIME_EMI_TRPAB_05T         :2;
    U32 AC_TIME_EMI_TRC_05T           :2;
    // Byte1	
    U32 AC_TIME_EMI_TRFC_05T          :2;
    U32 AC_TIME_EMI_TRFCPB_05T        :2;
    U32 AC_TIME_EMI_TXP_05T           :2;
    U32 AC_TIME_EMI_TRTP_05T          :2;
    // Byte2	
    U32 AC_TIME_EMI_TRCD_05T          :2;
    U32 AC_TIME_EMI_TWR_05T           :2;
    U32 AC_TIME_EMI_TWTR_05T          :2; // Olympus modified
    U32 AC_TIME_EMI_TRRD_05T          :2;
    // Byte3	
    U32 AC_TIME_EMI_TFAW_05T          :2;
    U32 AC_TIME_EMI_TRTW_ODT_OFF_05T  :2;
    U32 AC_TIME_EMI_TRTW_ODT_ON_05T   :2;
    U32 AC_TIME_EMI_TRTPD_05T         :2; // LP4/LP3 // Olympus new 

    // U 07
    // Byte0
    U32 AC_TIME_EMI_TWTPD_05T           :2; // LP4/LP3 // Olympus new
    U32 AC_TIME_EMI_TMRR2W_ODT_OFF_05T  :2; // Useless, no 0.5T in Olympus and Elbrus
    U32 AC_TIME_EMI_TMRR2W_ODT_ON_05T   :2; // Useless, no 0.5T in Olympus and Elbrus

   
}AC_TIMING_EXTERNAL_T;

#pragma pack(push, 1)

typedef struct
{
    unsigned int  sub_version;            // sub_version: 0x1 for new version
    unsigned int  type;                /* 0x0000 : Invalid
                             0x0001 : Discrete DDR1
                             0x0002 : Discrete LPDDR2
                             0x0003 : Discrete LPDDR3
                             0x0004 : Discrete PCDDR3
                                 0x0005 : Discrete LPDDR4
                                 0x0006 : Discrete LPDR4X
                             0x0101 : MCP(NAND+DDR1)
                             0x0102 : MCP(NAND+LPDDR2)
                             0x0103 : MCP(NAND+LPDDR3)
                             0x0104 : MCP(NAND+PCDDR3)
                             0x0201 : MCP(eMMC+DDR1)
                             0x0202 : MCP(eMMC+LPDDR2)
                             0x0203 : MCP(eMMC+LPDDR3)
                             0x0204 : MCP(eMMC+PCDDR3)
                                 0x0205 : MCP(eMMC+LPDDR4)
                                 0x0206 : MCP(eMMC+LPDR4X)
                             */
    unsigned int   id_length;              // EMMC and NAND ID checking length
    unsigned int   fw_id_length;              // FW ID checking length
    unsigned char  ID[16];
    unsigned char  fw_id[8];               // To save fw id
    unsigned int   EMI_CONA_VAL;           //@0x3000
    unsigned int   EMI_CONH_VAL;
    
    union   {        
        unsigned int DRAMC_ACTIME_UNION[8];
    AC_TIMING_EXTERNAL_T AcTimeEMI;
    };       

    u64   DRAM_RANK_SIZE[4];
    unsigned int   EMI_CONF_VAL;
    unsigned int   EMI_CONJ_VAL;
    unsigned int   CHN0_EMI_CONA_VAL;
    unsigned int   CHN1_EMI_CONA_VAL;
    /* Single field to store LP4 dram type (normal, byte, mixed) */
    unsigned int   dram_cbt_mode_extern;
    unsigned int   reserved[6];

#if 0
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
#else
    unsigned int   iLPDDR3_MODE_REG_5;
#endif    
    unsigned int   PIN_MUX_TYPE;   
} EMI_SETTINGS;
#pragma pack(pop)

//typedef  EMI_SETTINGS_v15 EMI_SETTINGS;
#if (FOR_DV_SIMULATION_USED==0)
void setup_dramc_voltage_by_pmic(void);
void switch_dramc_voltage_to_auto_mode(void);
#if ! __ETT__
uint32 mt_set_emis(uint8* emi, uint32 len, bool use_default); //array of emi setting.
#endif
#endif

extern int num_of_emi_records;
extern int emi_setting_index;
extern EMI_SETTINGS emi_settings[MAX_EMI_SETTING_NUM];
extern EMI_SETTINGS default_emi_setting;
extern EMI_SETTINGS emi_setting_default_lpddr3;  
extern EMI_SETTINGS emi_setting_default_lpddr4;  

int mt_get_dram_type (void); 

#ifdef LAST_DRAMC
#include "x_hal_io.h"
#define LAST_DRAMC_MAGIC_PATTERN 0x19870611
static void update_last_dramc_info(void);
void init_ta2_all_channel(void);
void init_ta2_single_channel(unsigned int);
typedef struct {
    unsigned int ta2_result_magic;
    unsigned int ta2_result_last;
    unsigned int ta2_result_past;
    unsigned int ta2_result_checksum;
    unsigned int reboot_count;
    volatile unsigned int last_fatal_err_flag;
    volatile unsigned int fatal_err_flag;
    volatile unsigned int storage_api_err_flag;
    volatile unsigned int last_gating_err[2][2]; // [channel][rank]
    volatile unsigned int gating_err[2][2]; // [channel][rank]
} LAST_DRAMC_INFO_T;
#define DEF_LAST_DRAMC LAST_DRAMC_INFO_T

#define OFFSET_DRAM_FATAL_ERR		(31)
#define OFFSET_DRAM_TA2_ERR		(23)
#define OFFSET_DRAM_GATING_ERR		(7)
#define OFFSET_CPU_RW_ERR		(5)
#define OFFSET_DDR_RSV_MODE_FLOW	(4)
#define OFFSET_DDR_RSV_MODE_ERR		(3)
#define OFFSET_EMI_DCS_ERR		(2)
#define OFFSET_DVFSRC_ERR		(1)
#define OFFSET_DRS_ERR			(0)

#define ERR_DRAM_TA2_RK0		(1 << 0)
#define ERR_DRAM_TA2_RK1		(1 << 1)

#define ERR_DRAM_GATING_RK0_R		(1 << 0)
#define ERR_DRAM_GATING_RK0_F		(1 << 1)
#define ERR_DRAM_GATING_RK1_R		(1 << 2)
#define ERR_DRAM_GATING_RK1_F		(1 << 3)

#define ERR_CPU_RW_RK0			(1 << 0)
#define ERR_CPU_RW_RK1			(1 << 1)

/* 0x1f -> bit[4:0] is for DDR reserve mode */
#define DDR_RSV_MODE_ERR_MASK		(0x1f)

unsigned int check_last_dram_fatal_exception(void);
unsigned int check_dram_fatal_exception(void);
void set_err_code_for_storage_api(void);
void dram_fatal_set_ta2_err(unsigned int chn, unsigned int err_code);
void dram_fatal_set_gating_err(unsigned int chn, unsigned int err_code);
void dram_fatal_set_cpu_rw_err(unsigned int err_code);
void dram_fatal_set_stberr(unsigned int chn, unsigned int rk, unsigned int err_code);

void dram_fatal_backup_stberr(void);
void dram_fatal_init_stberr(void);

#define dram_fatal_set_cpu_rw_err(err_code)\
	do {\
		dram_fatal_set_err(err_code, 0x3, OFFSET_CPU_RW_ERR);\
	} while(0)

#define dram_fatal_set_ddr_rsv_mode_err()\
	do {\
		dram_fatal_set_err(0x1, 0x1, OFFSET_DDR_RSV_MODE_ERR);\
	} while(0)

#define dram_fatal_set_emi_dcs_err()\
	do {\
		dram_fatal_set_err(0x1, 0x1, OFFSET_EMI_DCS_ERR);\
	} while(0)

#define dram_fatal_set_dvfsrc_err()\
	do {\
		dram_fatal_set_err(0x1, 0x1, OFFSET_DVFSRC_ERR);\
	} while(0)

#define dram_fatal_set_drs_err()\
	do {\
		dram_fatal_set_err(0x1, 0x1, OFFSET_DRS_ERR);\
	} while(0)

#define dram_fatal_set_ddr_rsv_mode_flow()\
	do {\
		dram_fatal_set_err(0x1, 0x1, OFFSET_DDR_RSV_MODE_FLOW);\
	} while(0)

#endif //LAST_DRAMC

//================================================
//=============pmic related api for ETT HQA test ==============
//================================================
#define STD_VCORE	750000
#define STD_VDRAM	1100000
#define STD_VDDQ	600000

#define MAX_VSRAM	1309375
#define MAX_VCORE	1196875
#define MAX_VDRAM	1309375
#define MAX_VDDQ	1190625

#define UNIT_VCORE	6250
#define UNIT_VDRAM	6250
#define UNIT_VDDQ	6250

//#define DRAM_HQA
#if defined(DRAM_HQA) || __ETT__

//#define HVCORE_HVDRAM
#define NVCORE_NVDRAM
//#define LVCORE_LVDRAM
//#define HVCORE_LVDRAM
//#define LVCORE_HVDRAM

#define HQA_VCORE_LV		750000 // uV
#define HQA_VCORE_NV		750000
#define HQA_VCORE_HV		750000

#define HQA_VCORE_KOPP0_LV	750000 //uV
#define HQA_VCORE_KOPP1_LV	625000
#define HQA_VCORE_KOPP2_LV	625000
#define HQA_VCORE_KOPP3_LV	625000

#define HQA_VCORE_KOPP0_NV	750000
#define HQA_VCORE_KOPP1_NV	625000
#define HQA_VCORE_KOPP2_NV	625000
#define HQA_VCORE_KOPP3_NV	625000

#define HQA_VCORE_KOPP0_HV	750000
#define HQA_VCORE_KOPP1_HV	625000
#define HQA_VCORE_KOPP2_HV	625000
#define HQA_VCORE_KOPP3_HV	625000

#define HQA_VDRAM_LV		1100000 //uV
#define HQA_VDRAM_NV		1100000
#define HQA_VDRAM_HV		1100000

#define HQA_VDDQ_LV		600000 //uV
#define HQA_VDDQ_NV		600000
#define HQA_VDDQ_HV		600000

#if defined(HVCORE_HVDRAM)
#define HQA_VCORE	HQA_VCORE_HV
#define HQA_VDRAM	HQA_VDRAM_HV
#define HQA_VDDQ	HQA_VDDQ_HV
#define HQA_VCORE_KOPP0	HQA_VCORE_KOPP0_HV
#define HQA_VCORE_KOPP1	HQA_VCORE_KOPP1_HV
#define HQA_VCORE_KOPP2	HQA_VCORE_KOPP2_HV
#define HQA_VCORE_KOPP3	HQA_VCORE_KOPP3_HV
#elif defined(NVCORE_NVDRAM)
#define HQA_VCORE	HQA_VCORE_NV
#define HQA_VDRAM	HQA_VDRAM_NV
#define HQA_VDDQ	HQA_VDDQ_NV
#define HQA_VCORE_KOPP0	HQA_VCORE_KOPP0_NV
#define HQA_VCORE_KOPP1	HQA_VCORE_KOPP1_NV
#define HQA_VCORE_KOPP2	HQA_VCORE_KOPP2_NV
#define HQA_VCORE_KOPP3	HQA_VCORE_KOPP3_NV
#elif defined(LVCORE_LVDRAM)
#define HQA_VCORE	HQA_VCORE_LV
#define HQA_VDRAM	HQA_VDRAM_LV
#define HQA_VDDQ	HQA_VDDQ_LV
#define HQA_VCORE_KOPP0	HQA_VCORE_KOPP0_LV
#define HQA_VCORE_KOPP1	HQA_VCORE_KOPP1_LV
#define HQA_VCORE_KOPP2	HQA_VCORE_KOPP2_LV
#define HQA_VCORE_KOPP3	HQA_VCORE_KOPP3_LV
#elif defined(HVCORE_LVDRAM)
#define HQA_VCORE	HQA_VCORE_HV
#define HQA_VDRAM	HQA_VDRAM_LV
#define HQA_VDDQ	HQA_VDDQ_LV
#define HQA_VCORE_KOPP0	HQA_VCORE_KOPP0_HV
#define HQA_VCORE_KOPP1	HQA_VCORE_KOPP1_HV
#define HQA_VCORE_KOPP2	HQA_VCORE_KOPP2_HV
#define HQA_VCORE_KOPP3	HQA_VCORE_KOPP3_HV
#elif defined(LVCORE_HVDRAM)
#define HQA_VCORE	HQA_VCORE_LV
#define HQA_VDRAM	HQA_VDRAM_HV
#define HQA_VDDQ	HQA_VDDQ_HV
#define HQA_VCORE_KOPP0	HQA_VCORE_KOPP0_LV
#define HQA_VCORE_KOPP1	HQA_VCORE_KOPP1_LV
#define HQA_VCORE_KOPP2	HQA_VCORE_KOPP2_LV
#define HQA_VCORE_KOPP3	HQA_VCORE_KOPP3_LV
#endif
#endif //end #if defined(DRAM_HQA) || __ETT__

#ifdef DRAM_HQA
#define SEL_PREFIX_VCORE	HQA_VCORE
#define SEL_PREFIX_VDRAM	HQA_VDRAM
#define SEL_PREFIX_VDDQ		HQA_VDDQ
#else
#define SEL_PREFIX_VCORE	STD_VCORE
#define SEL_PREFIX_VDRAM	STD_VDRAM
#define SEL_PREFIX_VDDQ		STD_VDDQ
#endif

#ifdef DRAM_CALIB_LOG
#include <mt_rtc_hw.h>
#include <rtc.h>
#include <pmic_wrap_init.h>
#define CALIB_LOG_BASE 0x0011E000
#define CALIB_LOG_SIZE 8192
int i4WriteSramLog(unsigned int u4Offset, unsigned int *pu4Src, unsigned int u4WordCnt);
typedef struct {
    unsigned int rtc_yea_mth;
    unsigned int rtc_dom_hou;
    unsigned int rtc_min_sec;
    unsigned int data_count;
} DRAM_KLOG_HEAD;
typedef struct {
    unsigned int check;
    unsigned int guard;
} DRAM_KLOG_TAIL;
#endif //end #ifdef DRAM_CALIB_LOG

#if SUPPORT_SAVE_TIME_FOR_CALIBRATION

#define PART_DRAM_DATA_ADDR	0x8000
#define PART_DRAM_DATA_SIZE	0x100000

#if (CFG_BOOT_DEV == BOOTDEV_UFS)
#define	PART_ID_DRAM_DATA	UFS_LU_USER
#elif (CFG_BOOT_DEV == BOOTDEV_SDMMC)
#define	PART_ID_DRAM_DATA	EMMC_PART_USER
#endif

#define	DRAM_CALIBRATION_DATA_MAGIC	0x9502

typedef struct _DRAM_CALIBRATION_DATA_T
{
	u32	pl_version;
	u16	magic_number;
	u16	checksum;
	u32	calib_err_code;
	SAVE_TIME_FOR_CALIBRATION_T	calibration_data;
} DRAM_CALIBRATION_DATA_T;

/*
 * g_dram_storage_api_err_code:
 * 	bit[0:3] -> read api
 * 	bit[4:7] -> write api
 * 	bit[8:11] -> clean api
 * 	bit[12:12] -> data formatted due to fatal exception
 */
#define ERR_NULL_POINTER	(0x1)
#define ERR_MAGIC_NUMBER	(0x2)
#define ERR_CHECKSUM		(0x3)
#define ERR_PL_UPDATED		(0x4)
#define ERR_BLKDEV_NOT_FOUND	(0x5)
#define ERR_BLKDEV_READ_FAIL	(0x6)
#define ERR_BLKDEV_WRITE_FAIL	(0x7)

#define ERR_DATA_FORMATTED_OFFSET	(12)

typedef enum {
	DRAM_STORAGE_API_READ = 0,
	DRAM_STORAGE_API_WRITE,
	DRAM_STORAGE_API_CLEAN,
} DRAM_STORAGE_API_TPYE;

extern u32 g_dram_storage_api_err_code;
#define SET_DRAM_STORAGE_API_ERR(err_type, api_type) \
do {\
	g_dram_storage_api_err_code |= (err_type << (api_type * 4));\
} while(0)

#define SET_DATA_FORMATTED_STORAGE_API_ERR() \
do {\
	g_dram_storage_api_err_code |= (1 << ERR_DATA_FORMATTED_OFFSET);\
} while(0)

int read_offline_dram_calibration_data(SAVE_TIME_FOR_CALIBRATION_T *offLine_SaveData);
int write_offline_dram_calibration_data(SAVE_TIME_FOR_CALIBRATION_T *offLine_SaveData);
int clean_dram_calibration_data(void);

void dram_fatal_exception_detection_start(void);
void dram_fatal_exception_detection_end(void);

#define CBT_VREF_OFFSET			2
#define WRITE_LEVELING_OFFSET		5
#define GATING_START_OFFSET		0
#define GATING_PASS_WIN_OFFSET		3
#define RX_WIN_PERBIT_OFFSET		5
#define RX_WIN_PERBIT_VREF_OFFSET	4
#define TX_WIN_PERBIT_OFFSET		5
#define TX_WIN_PERBIT_VREF_OFFSET	4
#define RX_DATLAT_OFFSET		1
#define RX_WIN_HIGH_SPEED_TH		10
#define RX_WIN_LOW_SPEED_TH		100
#define TX_WIN_TH			12

#endif

void release_dram(void);
void print_DBG_info(DRAMC_CTX_T *p);
void Dump_EMIRegisters(DRAMC_CTX_T *p);

#endif
