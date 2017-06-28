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

#ifndef __BMT_H__
#define __BMT_H__

#define __DA_NAND__


#include "nand_types.h"
#include "nand_util_func.h"
#include "debug.h"
#include "boot/dev_interface/nand_interface.h"

#define PMT_POOL_SIZE (2)

#define MAX_BMT_SIZE        (0x200) //0x80
#define BMT_VERSION         (1)         // initial version

#define MAIN_SIGNATURE_OFFSET   (0)
#if defined(__NFI_SUPPORT_TLC__)
#define OOB_SIGNATURE_OFFSET    ((g_pNandInfo->m_dev->m_nand_type == NAND_FLASH_TLC)?0:1)
#define OOB_INDEX_OFFSET        ((g_pNandInfo->m_dev->m_nand_type == NAND_FLASH_TLC)?24:29)
extern u32  _u4TLC2SLCRatio;
#else
#define OOB_SIGNATURE_OFFSET    (1)
#define OOB_INDEX_OFFSET        (29)
#endif
#define OOB_INDEX_SIZE          (2)
#define FAKE_INDEX              (0xAAAA)

#define BLOCK_SIZE          (g_nand_chip.erasesize)
#define PAGE_SIZE           (g_nand_chip.page_size)
#if defined(__NFI_SUPPORT_TLC__)
#define PAGE_PER_BLOCK      (NUTL_PAGES_PER_BLOCK())
#else
#define PAGE_PER_BLOCK      (g_nand_chip.page_per_block)
#endif

typedef struct _bmt_entry_
{
    u16 bad_index;      // bad block index
    u16 mapped_index;  // mapping block index in the replace pool
} bmt_entry;

typedef enum
{
    UPDATE_ERASE_FAIL,
    UPDATE_WRITE_FAIL,
    UPDATE_UNMAPPED_BLOCK,
    // add for remove mapping entry
    UPDATE_REMOVE_ENTRY,
    UPDATE_INIT_WRITE,
    UPDATE_REASON_COUNT,
    INIT_BAD_BLK,
} update_reason_t;


#define FTL_BAD_BLK		0xffffff

#define FTL_MAX_BMT_COUNT		0x400
#define FTL_BMT_VERSION				(1)

struct ftl_bmt_entry {
	u16 bad_index;	/* bad block index */
	u8 flag;		/* mapping block index in the replace pool */
};

struct ftl_bmt_struct{
	struct ftl_bmt_entry entry[FTL_MAX_BMT_COUNT];
	unsigned int  version;
	unsigned int bad_count;		/* bad block count */
	unsigned int  start_block;	/* data partition start block addr */
	unsigned int  end_block;	/* data partition start block addr */
	unsigned int checksum;	
};


typedef struct {
    bmt_entry table[MAX_BMT_SIZE];
    u8 version;
    u8 mapped_count;                // mapped block count in pool
    u8 bad_count;                   // bad block count in pool. Not used in V1
    struct ftl_bmt_struct ftl_bmt;
}bmt_struct;


/***************************************************************
*                                                              *
* Interface BMT need to use                                    *
*                                                              *
***************************************************************/


/***************************************************************
*                                                              *
* Different function interface for preloader/uboot/kernel      *
*                                                              *
***************************************************************/
void set_bad_index_to_oob(u8 *oob, u16 index);


/* Note: DA download related functions */
bmt_struct *init_bmt(struct nand_chip* nand, int size);
bool update_bmt(u64 offset, update_reason_t reason, u8 *dat, u8 *oob, NUTL_ProgramFlag_E flag);
status_t init_build_bmt(void* user_arg, CB_OP_PROGRESS cb);
int load_bmt_data(int start, int pool_size);

//void init_build_bmt_temp(void);
unsigned int get_mapping_block_index(int index);

//
// for NUTL use
//
void NUTL_INIT_PARM();
void NUTL_NFI_InitBMT(uint32 m_nand_bmt_num, u8* p_bmt_exist);
void NUTL_AVAIL_SIZE(u64* pAvailSize);
int is_block_bad(int index);
int is_bmt_pool_block(int index);
int is_ftl_block(int index);
int create_vendor_bmt(void);
int is_bmt_block(int index);

#endif // #ifndef __BMT_H__
