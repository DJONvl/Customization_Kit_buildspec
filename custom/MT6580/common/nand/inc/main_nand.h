/*******************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of MediaTek Inc. (C) 2005
*
*******************************************************************************/
#ifndef _NAND_MAIN_H_
#define _NAND_MAIN_H_

//#include "nand.h"
#include "DOWNLOAD.h"

#define MTK_ILLEGAL_VAUE	0xFFFFFFFF

#define SECUREROM_PREIMAGE_NUM		2	// SEC_INFO_head, SEC_INFO_tail, 
#define SECUREROM_POSTIMAGE_NUM		4	// MAC, SEC_RO, FlashTool_Cfg, CUST_PARA
#define SECUREROM_TOTALIMAGE_NUM	(SECUREROM_PREIMAGE_NUM + SECUREROM_POSTIMAGE_NUM)

#define	ENFB_NAME		(0x42464E45)
#define	FOTA_NAME		(0x41544F46)
#define RESB_NAME		(0x42534552)

// Dual BL
#define MTK_ILB_BUFFER_SIZE		6	// number of blocks

// Image List Tail
#define MTK_ILB_TAG_SIZE		7	// number of words

#define MTK_ILB_MAIN_TAG {0xf83f03e0, 0xf83f03e0, 0x5f4b544d, 0x5f424c49, 0x4c494154, 0xf83f03e0, 0xf83f03e0}  /* MTK_ILB_MAIN */
#define MTK_ILB_TEMP_TAG {0x0000ffff, 0x0000ffff, 0x5f4b544d, 0x5f424c49, 0x504d4554, 0x0000ffff, 0x0000ffff}  /* MTK_ILB_TEMP */
#define MTK_ILB_BAK_TAG  {0xffc0fc00, 0xffc0fc00, 0x5f4b544d, 0x5f424c49, 0x004b4142, 0xffc0fc00, 0xffc0fc00}  /* MTK_ILB_BAK  */
#define MTK_ILB_FOTA_TAG {0x07ff001f, 0x07ff001f, 0x5f4b544d, 0x5f424c49, 0x41544f46, 0x07ff001f, 0x07ff001f}  /* MTK_ILB_FOTA */

#define SLC_MODE 1
#define MLC_MODE 0

typedef enum {
	MTK_ILB_MAIN = 1,
	MTK_ILB_TEMP,
	MTK_ILB_BAK,
	MTK_ILB_FOTA,
//	
	MTK_ILB_NONE	= 254,
	MTK_ILB_UNKNOW = 255
} MTK_ILB_E;

//------------------------------------------------------------------------------
// NFB Information (Boot Loader Header and Image List)
//------------------------------------------------------------------------------

// BOOTLOADER
typedef struct NFIType {
   uint16	IOInterface;     /* IO_8BITS or IO_16BITS */
   uint16	pageSize;        /* 512, 2048 */
   uint16	addressCycle;
   uint16	pageShift;
} NFI_MENU_v1;

typedef struct _BOOTLHeader_ {
   int8			ID1[12];
   int8			version[4];
   uint32		length;
   uint32		startAddr;
   uint32		checksum;
   int8			ID2[8];
   NFI_MENU_v1	NFIinfo;
   uint16		pagesPerBlock;   
   uint16		totalBlocks;
   uint16		blockShift;
   uint16		linkAddr[6];
   uint16		lastBlock;
} BOOTL_HEADER;

typedef struct _BOOTLHeader2_ {
	int8	ID3[16];
	int8	customer_name[32];
	uint8	mac_of_customer[16];
	uint8	mac_of_bootloader[16];
	uint8	reserved[48];
} BOOTL_HEADER2;			//128 bytes

typedef struct _BOOTLHeader2_v2_ {
	int8	ID3[16];
	int8	customer_name[32];
	uint8	mac_of_customer[24];	// SHA1(20B)+PADDING(4B) for 3DES 
	uint8	mac_of_bootloader[24];	// SHA1(20B)+PADDING(4B) for 3DES 
	uint8	reserved[32];
} BOOTL_HEADER2_v2_ST;		//128 bytes


// EXT BOOTLOADER

typedef struct _EXT_BOOTL_LINKADDR_ {
	uint32	cs;
	uint32	blockNo;
} EXT_BOOTL_LINKADDR;

typedef struct _EXT_BOOTL_HEADER_ {
	int8		ID[16];
	int8		version[4];
	uint32		length;
	uint32		checksum;
	
	EXT_BOOTL_LINKADDR	linkAddr[10];
	uint32		lastLogicalBlock;
	
	uint8		reserved[16];
} EXT_BOOTL_HEADER;			//128 bytes

typedef struct {
	uint32	version;
	uint8	mac_of_ext_bootloader[24];
	uint8	reserved[100];
} EXT_BOOTL_HEADER2;		//128 bytes


// IMAGE LIST

typedef struct {
	uint32		m_load_addr;
	uint32		m_length;
	uint32		m_start_block;
	uint32		m_reserved;
} Nand_ImageInfo_S;

typedef struct {
	Nand_ImageInfo_S	m_image[10];
	uint32				m_image_count;
	Nand_ImageInfo_S	m_image_ext[10];
} NMAIN_ImageList_Phy_S;

typedef struct {
	Nand_ImageInfo_S	m_image[20];
	uint32				m_image_count;
} NMAIN_ImageList_S;

typedef struct {
	NAND_PageBuffer_U	m_PageBuf;
	uint32				m_ecc_parity_from_reg[8];
	uint32				m_ecc_parity_from_spare[8];
} NUTL_InternalTempBuf_S;

typedef struct {
	uint32				ra_erase_blkaddr;			// row address of next erase block
	uint32				ca_cur_addr;
	uint32				ra_cur_addr;				// row address of current page
	uint32				ra_1st_blkaddr;				// row address of 1st block address
	uint32				bloader_linkblocks;
	BOOTL_HEADER		bloader_header;
	BOOTL_HEADER2		bloader_header2;			// Secure NFB
	EXT_BOOTL_HEADER	ext_bloader_header;
	EXT_BOOTL_HEADER2	ext_bloader_header2;		// Secure NFB
	uint32				ecc_parity_from_reg[8];
	uint32				ecc_parity_from_spare[8];
	uint32				block_replacement;
	uint32				ra_image_startaddr;			// row address of start address of each image file
	uint32				ra_header_addr;				// row address of image list
	bool				bBootLoaderExist;
	bool				bExtBootLoaderExist;
	uint32				sec_ro_row_addr;
	uint32				ilb_main_row_addr;
	uint32				ilb_bak_row_addr;
	uint32				ilb_temp_row_addr;
} NMAIN_DownloadStruct;

//--------------------------------------------------------------
// MBA Related INFO
//--------------------------------------------------------------

#define	MBA_NO_RESERVED_MAX_LENGTH		0xFFFFFFFF

typedef struct {
	char	m_verno[32];	// verno: Project Name + SW version
	uint32	m_type;
	uint32	m_start_addr;
	uint32	m_table_size;
	uint32	m_content_size;
	uint32	m_magic;
} NMAIN_Resource_Header_S;

typedef struct {
	char	m_verno[32];	// verno: Project Name + SW version
} NMAIN_MBA_Parameter_S;


typedef struct {
	uint32 m_index;			// index in image list
	uint32 m_row_addr;		// phy addr in image list
	uint32 m_res_type;		// resource type in resource header
} NMAIN_Resource_Info_S;
	
typedef struct {
	uint32 					m_res_count;
	NMAIN_Resource_Info_S	m_res[10];
	uint32					m_enfb_index;
	uint32					m_enfb_row_addr;
} NMAIN_Resource_List_S;

//--------------------------------------------------------------
// FOAT Related INFO
//--------------------------------------------------------------
typedef struct {
	bool								m_enable;
	uint32								m_bin_type;
	uint32								m_resb_type;
	uint32								m_load_addr;
	uint32								m_length;
    U64                              m_part_len;
	uint32								m_max_length;
	uint32								m_start_block;	
	uint32								m_last_block;
	uint32								m_end_block;
	uint8                               m_rom_name[DL_IMG_NAME_LENGTH];
	uint32								m_download_type;	
	uint32								m_reserved;	
} NMAIN_Image_Info_S;

typedef struct {
	bool								m_FOTA_enable;
	bool								m_MBA_enable;
	bool								m_MBA_Partial_enable;
	NMAIN_Image_Info_S					m_ua;
	uint32								m_image_count;
	uint32								m_enable_count;
	NMAIN_Image_Info_S					m_image[NAND_MAX_PARTITION_COUNT];
	uint32								m_package_addr;
	uint32								m_package_length;
	uint32								m_backup_addr;
	uint32								m_backup_length;
	uint32								m_secure_block_num;
} NMAIN_FOTA_ImageList_S;

// Special Block

typedef enum {
	MTK_MAC_BLOCK = 0x4249F100,
	MTK_SECURE_RO_BLOCK,
	MTK_FLASHTOOL_CFG,
	MTK_CUSTOM_PARA,
	MTK_FOTA_UA_MAP_BLOCK,
	MTK_FOTA_MAUI_MAP_BLOCK,
	MTK_FOTA_MAUI_IMG_LIST_BLOCK,
	MTK_FOTA_MAUI_THIRD_MAP_BLOCK,
	MTK_FOTA_UPDATE_RECORD_BLOCK,
	MTK_FOTA_PACKAGE_BLOCK,
	MTK_FOTA_BLOCK_TYPE_NUM
} MTK_FOTA_BLOCK_TYPE;

#define FOTA_SUPER_BLOCK_VERSION    	(0xA01)
#define FOTA_SUPER_INITIAL_KEY      	(0x53555052)

#define FOTA_INFO_END_VERSION			(0xE01)
#define FOTA_INFO_END_INITIAL_KEY		(0x4D444E45)

//--
#define MAUI_MAC_BLK_NAME				(0x0043414D)
#define FOTA_MAC_BLOCK_VERSION			(0xD101)

#define MAUI_SEC_BLK_NAME				(0x004F5253)
#define FOTA_SEC_RO_BLOCK_VERSION		(0xD201)

#define MAUI_CFG_BLK_NAME				(0x00474643)
#define FOTA_CFG_BLOCK_VERSION			(0xD301)

#define MAUI_PAR_BLK_NAME				(0x00524150)
#define FOTA_PAR_BLOCK_VERSION			(0xD401)

#define MAUI_LST_BLK_NAME              	(0x0054534C)
#define FOTA_LST_BLOCK_VERSION         	(0xE01)

//--
#define MAUI_THIRD_MAP_BLK_NAME			(0x00445233)
#define FOTA_THIRD_MAP_BLOCK_VERSION	(0x1101)


//const char FOTA_FUE_SUPER_BLOCK_PATTERN[] = "MTK_FOTA_SUPER_BLOCK_HEADER_V01";

typedef struct {
	char								m_super_block_header[64];
	MTK_FOTA_BLOCK_TYPE					m_super_block_type;
	uint32								m_super_version;
	uint32								m_header_id_chksum;
} MTK_FOTA_SUPER_BLOCK_HEADER_v1_ST;

typedef struct {
	MTK_FOTA_SUPER_BLOCK_HEADER_v1_ST	m_super;
	uint32								m_name;
	uint32								m_version;
	uint32								m_replace;
	uint32								m_session;
} MTK_FOTA_BLOCK_v1_ST;

typedef struct {
	char								m_info_end_mark[64];
	uint32								m_info_end_version;
  	uint32								m_info_end_name;
  	uint32								m_info_end_block;
  	uint32								m_info_end_chksum;
} MTK_FOTA_INFO_END_v1_ST;

typedef struct {
	MTK_FOTA_INFO_END_v1_ST				m_mars_footer;
	uint32								m_mars_last_block;
	uint32								m_mars_session;
	uint32								m_mars_end_chksum;
} MTK_FOTA_MARS_BLOCK_END_V1_ST;


// Special Block - Image Area Info
typedef struct {
	uint32								m_start_block;
	uint32								m_last_block;
	uint32								m_end_block;
} Nand_Update_Area_ST;

typedef struct {
	uint32								m_image_end_id;
	uint32								m_image_count;
	Nand_Update_Area_ST					m_image_space[10];
	uint32								m_package_start_block;
	uint32								m_package_last_block;
	uint32								m_backup_start_block;
	uint32								m_backup_last_block;
	Nand_Update_Area_ST  		 		m_image_ext_space[10];
	uint32								m_reserved_last;
	uint32								m_image_end_chksum;
} Nand_ImageSpace_ST;

//--------------------------------------------------------------
//--------------------------------------------------------------


#ifdef __cplusplus
extern "C" {
#endif

extern NAND_PageBuffer_U		g_NMAIN_TmpPage;
//extern g_NMAIN_ENFB_MappingTable ==> *g_NMAIN_ENFB_MappingTable
//extern NAND_PageBuffer_U		g_NMAIN_ENFB_MappingTable;
extern NAND_BlockBuffer_S*		g_NMAIN_ENFB_MappingTable;	//only for NMAIN_DownloadImage
extern NMAIN_DownloadStruct	g_NMAIN_DL;
extern NMAIN_ImageList_S		g_NMAIN_ImageList;
extern NMAIN_FOTA_ImageList_S	g_NMAIN_FOTA_ImageList;
extern Nand_ImageSpace_ST		g_Main_ImageSpaceInfo;
extern NMAIN_Resource_List_S	g_NMAIN_ResourceList;

extern NAND_BlockBuffer_S	*g_NMAIN_p1stBlock;
extern uint32 g_NMAIN_lastBlock;

extern void NMAIN_SetupImageList(void);
extern void NMAIN_DownloadImage(void);
extern void NMAIN_ReadCmd(void);

typedef void(*CALLBACK_BMPCHECK_PROGRESS)(char percent);
extern void NMAIN_Format(NUTL_EraseFlag_E  flag, uint64 start_addr, uint64 length, bool bValidation, NUTL_AddrTypeFlag_E addr_type);
extern int NMAIN_BmtRemark(CALLBACK_BMPCHECK_PROGRESS prog_update);

extern STATUS_E  NMAIN_ReadPageCmd(uint32  page_index, uint32  page_size, uint32  read_len, uint32 *p_page32, uint32 *p_spare32, NUTL_ReadFlag_E  flag, uint16  *p_check_sum, NUTL_AddrTypeFlag_E addr_flag);
extern void NMAIN_ReadBlockCmd(uint32 block_ra_addr, uint32  page_size, uint32 read_len, uint32 *p_page32, uint32 *p_spare32, NUTL_ReadFlag_E  flag);

extern STATUS_E  NMAIN_GetPageCmd(uint32 * p_write_len, uint32 *p_page32, uint32 *p_spare32);
extern void NMAIN_WriteBlockCmd(uint32 * p_block_ra_addr, uint32 * p_write_len, uint32 *p_page32, uint32 *p_spare32);

extern uint32 fue_calculate_chksum(uint32* start_ptr, uint32 data_len, uint32 initial_key);
extern void	NFB_FOTA_Super_Block_Header_Gen(MTK_FOTA_BLOCK_TYPE super_type, uint32 block_name, uint32 block_ver, void *p_page8 );
extern bool NFB_FOTA_Super_Block_Header_Verify(MTK_FOTA_BLOCK_TYPE super_type, uint32 block_name, uint32 block_ver, void *p_page8 );
extern void NFB_FOTA_Super_Block_EndMark_Gen(uint32 block_name, void *p_page8 );

extern void NMAIN_Read_DL_info(void);
extern void NMAIN_Write_DL_info(char *download_status);
#ifdef __cplusplus
}
#endif

#endif
