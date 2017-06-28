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
*  20131116 MTK81353 Delete unused code.
*
*
*******************************************************************************/
#ifndef	SDMMC_H
#define	SDMMC_H

#ifdef __cplusplus
extern "C" {
#endif

#define SDMMC_BLK_BITS              (9)
#define SDMMC_BLK_SIZE              (1 << SDMMC_BLK_BITS)
#define SDMMC_BLK_NUM(size)         ((unsigned long long)(size) / SDMMC_BLK_SIZE)

#define SDMMC_SECTOR_SIZE           (SDMMC_BLK_SIZE)

#ifndef BB_MT6573 //avoid 6573 load region INT_SYSRAM size  exceeds limit, skip emmc_support_otp feature temply
#define FEATURE_EMMC_SUPPORT_OTP
#endif
//#if defined(FEATURE_EMMC_SUPPORT) && defined(FEATURE_EMMC_SUPPORT_OTP)

#define EMMC_OTP_START_ADDR     (0xc0000000)      /* 3G, just for debug */
#define EMMC_OTP_SIZE           (32*1024*1024)
#define OTP_EMMC_DEBUG
#define MTK_RESERVE_REGION 0xa800      /* MTK reserve 21MB */
#define OTP_RESERVE_REGION 0x15800    /* OTP reserve 43mb */
#define RESERVE_REGION  (OTP_RESERVE_REGION + MTK_RESERVE_REGION)

/* JUST SUPPORT FOR SQC TEST, NEED REMOVE FROM OFFICE RELEASE */
#define OTP_EMMC_DEBUG
/* #define EMMC_OTP_FOR_SQC */
/* #define EMMC_OTP_FOR_SQC_UNLOCK */

//#endif

extern status_t OTP_EMMC_LockCMD(uint64 otp_start);


typedef enum {
    SDMMC_SUCCESS = 0,
    SDMMC_FAIL,
    SDMMC_CRC_ERR,    
    SDMMC_TIMEOUT,
    SDMMC_INVALID,
    SDMMC_UNKNOWN_ERR
} SDMMC_STATUS;

typedef enum {
    SDMMC_PHYS_ADDR_MODE,
    SDMMC_VIRT_ADDR_MODE
} SDMMC_ADDR_MODE;

#define SDMMC_CAP_BOOTMODE           (0x00000001)
#define SDMMC_CAP_PARTITION          (0x00000002)
#define SDMMC_CAP_ENH_PARTITION      (0x00000004)

#define SDMMC_FLAG_HW_RESET_EN       (0x00000001)

typedef struct {
    uint32 cap;
    uint32 flags;
    uint32 cid[4];
	#if !defined(BB_MT6573) && !defined(BB_MT6516)
	unsigned char  fwid[6];
	#endif
} SDMMC_INFO;

#define SDMMC_COMMON_BUF_SZ          8192
#define SDMMC_LEGACY_COMMON_BUFFER_SIZE (SDMMC_COMMON_BUF_SZ)

extern uint8 g_sdmmc_common_buf[];
extern unsigned long long g_sdmmc_size;
extern unsigned long long g_sdmmc_user_size;

// Exported Interface to DA
extern void SDMMC_PowerCycle(void);
extern SDMMC_STATUS SDMMC_Init(int host_id);
extern SDMMC_STATUS SDMMC_Read(uint32 blockAddr, uint32 *bufferPtr, uint64 size);
extern SDMMC_STATUS SDMMC_Write(uint32 blockAddr, uint32 *bufferPtr, uint64 size);
#ifndef FEATURE_EMMC_ADDR_TRANS
extern SDMMC_STATUS SDMMC_Read_Part(uint32 blockAddr, uint32 *bufferPtr, uint64 size, emmc_section_e part_id);
extern SDMMC_STATUS SDMMC_Write_Part(uint32 blockAddr, uint32 *bufferPtr, uint64 size, emmc_section_e part_id);
#endif
extern SDMMC_STATUS SDMMC_GetInfo(SDMMC_INFO *ptr);
extern SDMMC_STATUS SDMMC_SetEXTCSD(uint8 addr, uint8 value);
extern SDMMC_STATUS SDMMC_CompletePartition(void);
extern SDMMC_STATUS SDMMC_ConfigurePartition(emmc_section_e partition, uint64 startAddr, uint64 size, bool isEnhanced);
extern emmc_section_e  SDMMC_GetCurrentPart(void);
extern SDMMC_STATUS SDMMC_SetCurrentPart(emmc_section_e partition);
extern SDMMC_STATUS SDMMC_GetAllPartitionSize(uint64 *p_boot1_size,
                                            uint64 *p_boot2_size,
                                            uint64 *p_rpmb_size,
                                            uint64 *p_gp1_size,
                                            uint64 *p_gp2_size,
                                            uint64 *p_gp3_size,
                                            uint64 *p_gp4_size,
                                            uint64 *p_ua_size);
extern SDMMC_STATUS SDMMC_SetAddrMode(SDMMC_ADDR_MODE mode);
extern SDMMC_ADDR_MODE SDMMC_GetAddrMode(void);
extern uint32 EMMC_Get_WP_Size(void);
extern status_t EMMC_OTP_lock_status(uint64 otpStart);

extern SDMMC_STATUS SDMMC_UnitTest(void);
extern int SDMMC_set_boot_host_id(int host_id);
extern int SDMMC_get_boot_host_id(void); 

#define DEFAULT_SECTOR_SIZE    512
#define PART_TBL_OFFSET        0x1be
#define PART_MAGIC_OFFSET      0x1fe

typedef struct partition_table
{
    unsigned char boot_ind;
    unsigned char head;
    unsigned char sector;
    unsigned char cyl;
    unsigned char sys_ind;
    unsigned char end_head;
    unsigned char end_sector;
    unsigned char end_cyl;
    unsigned char start[4];
    unsigned char size[4];
} partition_t;
#ifdef __cplusplus
}
#endif


#endif	//	SDMMC_H
