/* Copyright Statement:
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein
 * is confidential and proprietary to MediaTek Inc. and/or its licensors.
 * Without the prior written permission of MediaTek inc. and/or its licensors,
 * any reproduction, modification, use or disclosure of MediaTek Software,
 * and information contained herein, in whole or in part, shall be strictly prohibited.
 */
/* MediaTek Inc. (C) 2010. All rights reserved.
 *
 * BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
 * THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
 * RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO RECEIVER ON
 * AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
 * NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
 * SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
 * SUPPLIED WITH THE MEDIATEK SOFTWARE, AND RECEIVER AGREES TO LOOK ONLY TO SUCH
 * THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. RECEIVER EXPRESSLY ACKNOWLEDGES
 * THAT IT IS RECEIVER'S SOLE RESPONSIBILITY TO OBTAIN FROM ANY THIRD PARTY ALL PROPER LICENSES
 * CONTAINED IN MEDIATEK SOFTWARE. MEDIATEK SHALL ALSO NOT BE RESPONSIBLE FOR ANY MEDIATEK
 * SOFTWARE RELEASES MADE TO RECEIVER'S SPECIFICATION OR TO CONFORM TO A PARTICULAR
 * STANDARD OR OPEN FORUM. RECEIVER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND
 * CUMULATIVE LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
 * AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE,
 * OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY RECEIVER TO
 * MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
 *
 * The following software/firmware and/or related documentation ("MediaTek Software")
 * have been modified by MediaTek Inc. All revisions are subject to any receiver's
 * applicable license agreements with MediaTek Inc.
 */


#ifndef _PMT_H
#define _PMT_H

//#include "nand_types.h"
#include "nand_util_func.h"
//#include "logging.h"

#define PART_MAX_COUNT         NAND_MAX_PARTITION_COUNT

#define MAX_PARTITION_NAME_LEN 64

#define __DA_NAND__


struct nand_oobfree {
	unsigned int offset;
	unsigned int length;
};

#define MAX_OOBFREE_ENTRIES	8

struct nand_ecclayout {
	unsigned int eccbytes;
	unsigned int eccpos[64];
	struct nand_oobfree oobfree[MAX_OOBFREE_ENTRIES];
};

extern struct nand_ecclayout *nand_oob;
extern struct nand_ecclayout nand_oob_64;
extern struct nand_ecclayout nand_oob_128;


typedef struct
{
	unsigned char name[MAX_PARTITION_NAME_LEN];		/* partition name */
	unsigned long size;								/* partition size */	
	unsigned long offset;							/* partition start */
	unsigned long mask_flags;						/* partition flags */

} pt_resident_32;
typedef struct
{
	unsigned char name[MAX_PARTITION_NAME_LEN];		/* partition name */
	unsigned long long size;						/* partition size */	
	unsigned long long region;						/* partition region */ //will be used as download type on L branch. xiaolei
	unsigned long long offset;						/* partition start */
	unsigned long long mask_flags;					/* partition flags */

} pt_resident_64;

#define REGION_LOW_PAGE			0x004C4F57
#define REGION_FULL_PAGE		0x46554C4C
#define REGION_SLC_MODE			0x00534C43
#define REGION_TLC_MODE			0x00544C43

typedef struct
{
	unsigned char name[MAX_PARTITION_NAME_LEN];		/* partition name */
	unsigned long long size;						/* partition size */	
	unsigned long long region;						/* partition region */ //will be used as download type on L branch. xiaolei
	unsigned long long offset;						/* partition start */
	unsigned long long mask_flags;					/* partition flags */

} pt_resident;

#define PART_INFO_NAME_LEN  64  

struct part_info
{
	u8 name[PART_INFO_NAME_LEN];
	u64 offset;
	u64 size;
	u16 region;
};

//Sequnce number
#define S_DONE					0
#define S_DL_PAGESIZE_ERROR		1
#define S_DL_NO_SPACE_FOUND		2
#define S_DL_WRITE_PMT_ERROR	3
#define S_DL_PMT_OLD_VERSION	4

//#define PT_LOCATION          4090      // (4096-80)
//#define MPT_LOCATION        4091            // (4096-81)

#define PT_SIG					0x50547633		//"PTv3"
#define MPT_SIG					0x4D505433		//"MPT3"
#define PT_SIG_SIZE				4
#define is_valid_mpt(buf)		((*(u32 *)(buf))==MPT_SIG)
#define is_valid_pt(buf)		((*(u32 *)(buf))==PT_SIG)
#define is_valid_pt_v1(buf)		((*(u32 *)(buf))==PT_SIG_V1)
#define RETRY_TIMES				5


typedef struct _DM_PARTITION_INFO
{
	char part_name[MAX_PARTITION_NAME_LEN];			/* the name of partition */
	unsigned int start_addr;						/* the start address of partition */
	unsigned int part_len;							/* the length of partition */
	unsigned char part_visibility;					/* part_visibility is 0: this partition is hidden and CANNOT download */
													/* part_visibility is 1: this partition is visible and can download */                                            
	unsigned char dl_selected;						/* dl_selected is 0: this partition is NOT selected to download */
													/* dl_selected is 1: this partition is selected to download */
} DM_PARTITION_INFO;

typedef struct {
	unsigned int pattern;
	unsigned int part_num;							/* The actual number of partitions */
	DM_PARTITION_INFO part_info[PART_MAX_COUNT];
} DM_PARTITION_INFO_PACKET;

typedef struct {
	int sequencenumber:8;
	int tool_or_sd_update:4;
	int pt_next:4;				//have next pt
	int mirror_pt_dl:4;			//mirror download OK
	int mirror_pt_has_space:4;
	int pt_changed:4;
	int pt_has_space:4;
} pt_info;

#if defined(__NFI_SUPPORT_TLC__)
#define PT_TLCRATIO_SIZE 4
#endif


/******************************************************************************
*
 * NAND APIs
* 
******************************************************************************/
extern int load_exist_part_tab(u8 *buf, u32 size);

extern int new_part_tab(u8 *buf);
extern status_t SDMMC_PTB_NewPartTable(pt_resident *part, int table_size);
extern status_t SDMMC_PTB_Update(pt_resident *part, int table_size);
extern int load_exist_part_tab_sdmmc(u8 *buf, u32 size);

#endif
