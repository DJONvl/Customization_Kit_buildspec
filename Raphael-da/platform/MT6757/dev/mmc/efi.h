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


#ifndef _EFI_H
#define _EFI_H

#ifndef s8
typedef signed char s8;
#endif
#ifndef u8
typedef unsigned char u8;
#endif

#ifndef s16
typedef signed short s16;
#endif
#ifndef u16
typedef unsigned short u16;
#endif

#ifndef s32
typedef signed int s32;
#endif
#ifndef u32
typedef unsigned int u32;
#endif

#ifndef s64
typedef signed long long s64;
#endif
#ifndef u64
typedef unsigned long long u64;
#endif

#define MAX_PARTITION_NAME_LEN 64

#define MAX_GPT_PARTITION_NUM      (128)

#if 0
//Sequnce number
#define S_DONE                  0
#define S_DL_PAGESIZE_ERROR     1
#define S_DL_NO_SPACE_FOUND     2
#define S_DL_WRITE_PMT_ERROR    3
#endif

/* GuidPT */

typedef struct {
   u8 boot_ind;
   u8 head;
   u8 sector;
   u8 cyl;
   u8 sys_ind;
   u8 end_head;
   u8 end_sector;
   u8 end_cyl;
   u32 start_sector;
   u32 nr_sects;
} __attribute__((packed)) partition;

typedef struct {
   u8 boot_code[440];
   u32 unique_mbr_signature;
   u16 unknown;
   partition partition_record[4];
   u16 signature;
} __attribute__((packed)) pmbr;

typedef struct {
   u8 b[16];
} __attribute__((packed)) efi_guid_t;

#define GPT_ENTRY_NAME_LEN  (72 / sizeof(u16))

typedef struct {
   efi_guid_t partition_type_guid;
   efi_guid_t unique_partition_guid;
   u64 starting_lba;
   u64 ending_lba;
   u64 attributes;
   u16 partition_name[GPT_ENTRY_NAME_LEN];
} __attribute__((packed)) gpt_entry;

typedef struct {
   u64 signature;
   u32 revision;
   u32 header_size;
   u32 header_crc32;
   u32 reserved;
   u64 my_lba;
   u64 alternate_lba;
   u64 first_usable_lba;
   u64 last_usable_lba;
   efi_guid_t disk_guid;
   u64 partition_entry_lba;
   u32 num_partition_entries;
   u32 sizeof_partition_entry;
   u32 partition_entry_array_crc32;
} __attribute__((packed)) gpt_header;


typedef struct {
   u32 a;
   u16 b;
   u16 c;
   u8  d0;
   u8  d1;
   u8  d2;
   u8  d3;
   u8  d4;
   u8  d5;
   u8  d6;
   u8  d7;
} __attribute__((packed)) efi_guid_raw_data;

#define EFI_GUID(a,b,c,d0,d1,d2,d3,d4,d5,d6,d7) \
   ((efi_guid_t)   \
{(a) & 0xff, ((a) >> 8) & 0xff, ((a) >> 16) & 0xff, ((a) >> 24) & 0xff, \
   (b) & 0xff, ((b) >> 8) & 0xff, \
   (c) & 0xff, ((c) >> 8) & 0xff, \
   (d0), (d1), (d2), (d3), (d4), (d5), (d6), (d7)})

#define PARTITION_BASIC_DATA_GUID   \
   EFI_GUID(0xEBD0A0A2, 0xB9E5, 0x4433, 0x87, 0xC0, 0x68, 0xB6, 0xB7, 0x26, 0x99, 0xC7)


#endif
