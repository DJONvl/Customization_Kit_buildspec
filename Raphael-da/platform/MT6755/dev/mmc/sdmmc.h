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

#define FEATURE_EMMC_SUPPORT_OTP
#define EMMC_OTP_START_ADDR     (0xc0000000)      /* 3G, just for debug */
#define EMMC_OTP_SIZE           (32*1024*1024)
#define OTP_EMMC_DEBUG
#define MTK_RESERVE_REGION 0xa800      /* MTK reserve 21MB */
#define OTP_RESERVE_REGION 0x15800    /* OTP reserve 43mb */
#define RESERVE_REGION  (OTP_RESERVE_REGION + MTK_RESERVE_REGION)

/* JUST SUPPORT FOR SQC TEST, NEED REMOVE FROM OFFICE RELEASE */
//#define EMMC_OTP_FOR_SQC
//#define EMMC_OTP_FOR_SQC_UNLOCK
extern status_t OTP_EMMC_LockCMD(uint64 otp_start);
extern uint32 EMMC_Get_WP_Size(void);
extern status_t EMMC_OTP_lock_status(uint64 otpStart);
#ifdef __cplusplus
}
#endif


#endif	//	SDMMC_H
