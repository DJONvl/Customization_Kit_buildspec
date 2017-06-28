#ifndef	_ERROR_CODE_H_
#define	_ERROR_CODE_H_


//
//  Values are 32 bit values laid out as follows:
//
//   3 3 2 2 2 2 2 2 2 2 2 2 1 1 1 1 1 1 1 1 1 1
//   1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0
//  +---+-+-+-----------------------+-------------------------------+
//  |Sev|C|R|     Facility          |               Code            |
//  +---+-+-+-----------------------+-------------------------------+
//
//  where
//
//      Sev - is the severity code
//
//          00 - Success
//          01 - Informational
//          10 - Warning
//          11 - Error
//
//      C - is the Customer code flag. not used now. set 0
//
//      R - is a reserved bit. not used now. set 0
//
//      Facility - is the facility code
//
//      Code - is the facility's status code
//
//
// Define the facility codes
//
#define FACILITY_COMMON				1
#define FACILITY_AUTHENTICATION		2
#define FACILITY_LIBRARY			3
#define FACILITY_DEV_HW				4
#define FACILITY_FUNCTION			5
#define FACILITY_BROM				6
#define FACILITY_DA					7
#define FACILITY_PRELOADER			8



#define SUCCESSED(code) (((status_t)(code)) >= 0)  //SUCCEEDED used by win32. 
#define FAIL(code) 		(((status_t)(code)) < 0)

#define SEV_SUCCESS 	(0 << 30)
#define SEV_INFO 		(1 << 30)
#define SEV_WARNING 	(2 << 30)
#define SEV_ERROR 		(3 << 30)

#define MAKE_CODE(sev,faci,code) ((sev) | (faci << 16) | (code))

#define STATUS_OK                           0
#define STATUS_ERR                          MAKE_CODE(SEV_ERROR, FACILITY_COMMON, 0x1) 
#define STATUS_ABORT                        MAKE_CODE(SEV_ERROR, FACILITY_COMMON, 0x2) 
#define STATUS_UNSUPPORT_CMD                MAKE_CODE(SEV_ERROR, FACILITY_COMMON, 0x3)
#define STATUS_UNSUPPORT_CTRL_CODE          MAKE_CODE(SEV_ERROR, FACILITY_COMMON, 0x4) //H&D
#define STATUS_PROTOCOL_ERR                 MAKE_CODE(SEV_ERROR, FACILITY_COMMON, 0x5) //H&D
#define STATUS_PROTOCOL_BUFFER_OVERFLOW     MAKE_CODE(SEV_ERROR, FACILITY_COMMON, 0x6)
#define STATUS_INSUFFICIENT_BUFFER          MAKE_CODE(SEV_ERROR, FACILITY_COMMON, 0x7)
#define STATUS_USB_SCAN_ERR                 MAKE_CODE(SEV_ERROR, FACILITY_COMMON, 0x8)
#define STATUS_INVALID_HSESSION             MAKE_CODE(SEV_ERROR, FACILITY_COMMON, 0x9)
#define STATUS_INVALID_SESSION              MAKE_CODE(SEV_ERROR, FACILITY_COMMON, 0xA) 
#define STATUS_INVALID_STAGE                MAKE_CODE(SEV_ERROR, FACILITY_COMMON, 0xB) 
#define STATUS_NOT_IMPLEMENTED              MAKE_CODE(SEV_ERROR, FACILITY_COMMON, 0xC) 
#define STATUS_FILE_NOT_FOUND               MAKE_CODE(SEV_ERROR, FACILITY_COMMON, 0xD)
#define STATUS_OPEN_FILE_ERR                MAKE_CODE(SEV_ERROR, FACILITY_COMMON, 0xE)
#define STATUS_WRITE_FILE_ERR               MAKE_CODE(SEV_ERROR, FACILITY_COMMON, 0xF)
#define STATUS_READ_FILE_ERR                MAKE_CODE(SEV_ERROR, FACILITY_COMMON, 0x10)
#define STATUS_CREATE_FILE_ERR            MAKE_CODE(SEV_ERROR, FACILITY_COMMON, 0x11)
#define STATUS_UNSUPPORT_VERSION   MAKE_CODE(SEV_ERROR, FACILITY_COMMON, 0x11)


//Host command 
#define STATUS_DEVICE_CTRL_EXCEPTION        MAKE_CODE(SEV_ERROR, FACILITY_FUNCTION, 0x1)
#define STATUS_SHUTDOWN_CMD_EXCEPTION       MAKE_CODE(SEV_ERROR, FACILITY_FUNCTION, 0x2)
#define STATUS_DOWNLOAD_EXCEPTION           MAKE_CODE(SEV_ERROR, FACILITY_FUNCTION, 0x3)
#define STATUS_UPLOAD_EXCEPTION             MAKE_CODE(SEV_ERROR, FACILITY_FUNCTION, 0x4)
#define STATUS_EXT_RAM_EXCEPTION            MAKE_CODE(SEV_ERROR, FACILITY_FUNCTION, 0x5)
#define STATUS_NOTIFY_SWITCH_USB_SPEED_EXCEPTION MAKE_CODE(SEV_ERROR, FACILITY_FUNCTION, 0x6)
#define STATUS_READ_DATA_EXCEPTION          MAKE_CODE(SEV_ERROR, FACILITY_FUNCTION, 0x7)
#define STATUS_WRITE_DATA_EXCEPTION         MAKE_CODE(SEV_ERROR, FACILITY_FUNCTION, 0x8)
#define STATUS_FORMAT_EXCEPTION             MAKE_CODE(SEV_ERROR, FACILITY_FUNCTION, 0x9)
#define STATUS_OTP_OPERATION_EXCEPTION      MAKE_CODE(SEV_ERROR, FACILITY_FUNCTION, 0xA)
#define STATUS_SWITCH_USB_EXCEPTION         MAKE_CODE(SEV_ERROR, FACILITY_FUNCTION, 0xB)
#define STATUS_WRITE_EFUSE_EXCEPTION        MAKE_CODE(SEV_ERROR, FACILITY_FUNCTION, 0xC)
#define STATUS_READ_EFUSE_EXCEPTION          MAKE_CODE(SEV_ERROR, FACILITY_FUNCTION, 0xD)


//brom
#define STATUS_BROM_CMD_STARTCMD_FAIL        MAKE_CODE(SEV_ERROR, FACILITY_BROM, 0x1)
#define STATUS_BROM_GET_BBCHIP_HW_VER_FAIL   MAKE_CODE(SEV_ERROR, FACILITY_BROM, 0x2)
#define STATUS_BROM_CMD_SEND_DA_FAIL         MAKE_CODE(SEV_ERROR, FACILITY_BROM, 0x3)
#define STATUS_BROM_CMD_JUMP_DA_FAIL         MAKE_CODE(SEV_ERROR, FACILITY_BROM, 0x4)
#define STATUS_BROM_CMD_FAIL                 MAKE_CODE(SEV_ERROR, FACILITY_BROM, 0x5)
#define STATUS_CALLBACK_BROM_STAGE_FAIL      MAKE_CODE(SEV_ERROR, FACILITY_BROM, 0x6)

//brom info
#define STATUS_BROM_CONNECT_NOT_PL			MAKE_CODE(SEV_INFO, FACILITY_BROM, 0x1)

//da
#define STATUS_DA_VERSION_INCORRECT         MAKE_CODE(SEV_ERROR, FACILITY_DA, 0x1)
#define STATUS_DA_NOT_FOUND                 MAKE_CODE(SEV_ERROR, FACILITY_DA, 0x2)
#define STATUS_DA_SECTION_NOT_FOUND         MAKE_CODE(SEV_ERROR, FACILITY_DA, 0x3)
#define STATUS_DA_HASH_MISMATCH             MAKE_CODE(SEV_ERROR, FACILITY_DA, 0x4)  //D.
#define STATUS_DA_EXCEED_MAX_NUM    MAKE_CODE(SEV_ERROR, FACILITY_DA, 0x5)

//Lib
#define STATUS_SCATTER_FILE_INVALID         MAKE_CODE(SEV_ERROR, FACILITY_LIBRARY, 0x1)
#define STATUS_DA_FILE_INVALID              MAKE_CODE(SEV_ERROR, FACILITY_LIBRARY, 0x2)
#define STATUS_DA_SELECTION_ERR             MAKE_CODE(SEV_ERROR, FACILITY_LIBRARY, 0x3)
#define STATUS_PRELOADER_INVALID            MAKE_CODE(SEV_ERROR, FACILITY_LIBRARY, 0x4)
#define STATUS_EMI_HDR_INVALID              MAKE_CODE(SEV_ERROR, FACILITY_LIBRARY, 0x5)
#define STATUS_STORAGE_MISMATCH             MAKE_CODE(SEV_ERROR, FACILITY_LIBRARY, 0x6) //H&D.
#define STATUS_INVALID_PARAMETERS           MAKE_CODE(SEV_ERROR, FACILITY_LIBRARY, 0x7)
#define STATUS_INVALID_GPT                  MAKE_CODE(SEV_ERROR, FACILITY_LIBRARY, 0x8) //H&D.
#define STATUS_INVALID_PMT                  MAKE_CODE(SEV_ERROR, FACILITY_LIBRARY, 0x9) //H&D.
#define STATUS_LAYOUT_CHANGED               MAKE_CODE(SEV_ERROR, FACILITY_LIBRARY, 0xA)
#define STATUS_INVALID_FORMAT_PARAMETER     MAKE_CODE(SEV_ERROR, FACILITY_LIBRARY, 0xB)
#define STATUS_UNKNOWN_STORAGE_SECTION_TYPE MAKE_CODE(SEV_ERROR, FACILITY_LIBRARY, 0xC)
#define STATUS_UNKNOWN_SCATTER_FIELD 		MAKE_CODE(SEV_ERROR, FACILITY_LIBRARY, 0xD)
#define STATUS_PARTITION_TBL_NOT_EXIST      MAKE_CODE(SEV_ERROR, FACILITY_LIBRARY, 0xE)
#define STATUS_SCATTER_HW_CHIP_ID_MISMATCH  MAKE_CODE(SEV_ERROR, FACILITY_LIBRARY, 0xF)
#define STATUS_SEC_CERT_FILE_NOT_FOUND      MAKE_CODE(SEV_ERROR, FACILITY_LIBRARY, 0x10)
#define STATUS_SEC_AUTH_FILE_NOT_FOUND      MAKE_CODE(SEV_ERROR, FACILITY_LIBRARY, 0x11)
#define STATUS_SEC_AUTH_FILE_NEEDED         MAKE_CODE(SEV_ERROR, FACILITY_LIBRARY, 0x12)
#define STATUS_EMI_CONTAINER_FILE_NOT_FOUND MAKE_CODE(SEV_ERROR, FACILITY_LIBRARY, 0x13)
#define STATUS_SCATTER_FILE_NOT_FOUND       MAKE_CODE(SEV_ERROR, FACILITY_LIBRARY, 0x14)
#define STATUS_XML_FILE_OP_ERR         MAKE_CODE(SEV_ERROR, FACILITY_LIBRARY,0x15)
#define STATUS_UNSUPPORTED_PAGE_SIZE      MAKE_CODE(SEV_ERROR, FACILITY_LIBRARY, 0x16)
#define STATUS_EMI_INFO_LEN_OFFSET_INVALID	MAKE_CODE(SEV_ERROR, FACILITY_LIBRARY, 0x17)
#define STATUS_EMI_INFO_LEN_INVALID			MAKE_CODE(SEV_ERROR, FACILITY_LIBRARY, 0x18)


//device info
#define STATUS_SPARSE_INCOMPLETE            MAKE_CODE(SEV_INFO, FACILITY_DEV_HW, 0x1)
#define STATUS_OTP_LOCKED                   MAKE_CODE(SEV_INFO, FACILITY_DEV_HW, 0x2)
#define STATUS_OTP_UNLOCKED                 MAKE_CODE(SEV_INFO, FACILITY_DEV_HW, 0x3)
#define STATUS_CONTINUE                    MAKE_CODE(SEV_INFO, FACILITY_DEV_HW, 0x4)
#define STATUS_COMPLETE                    MAKE_CODE(SEV_INFO, FACILITY_DEV_HW, 0x5)

//device err
#define STATUS_UNSUPPORT_OP                 MAKE_CODE(SEV_ERROR, FACILITY_DEV_HW, 0x1) 
#define STATUS_THREAD                       MAKE_CODE(SEV_ERROR, FACILITY_DEV_HW, 0x2) 
#define STATUS_CHECKSUM_ERR                 MAKE_CODE(SEV_ERROR, FACILITY_DEV_HW, 0x3) 
#define STATUS_TOO_LARGE                    MAKE_CODE(SEV_ERROR, FACILITY_DEV_HW, 0x4)
#define STATUS_UNKNOWN_SPARSE_CHUNK_TYPE    MAKE_CODE(SEV_ERROR, FACILITY_DEV_HW, 0x5)
#define STATUS_PARTITON_NOT_FOUND           MAKE_CODE(SEV_ERROR, FACILITY_DEV_HW, 0x6)
#define STATUS_READ_PT_FAIL                 MAKE_CODE(SEV_ERROR, FACILITY_DEV_HW, 0x7)
#define STATUS_EXCEED_MAX_PART_NUM          MAKE_CODE(SEV_ERROR, FACILITY_DEV_HW, 0x8) 
#define STATUS_UNKNOWN_STORAGE_TYPE         MAKE_CODE(SEV_ERROR, FACILITY_DEV_HW, 0x9)
#define STATUS_DRAM_TEST_FAILED             MAKE_CODE(SEV_ERROR, FACILITY_DEV_HW, 0xA)
#define STATUS_EXCEED_AVALIABLE_RANGE       MAKE_CODE(SEV_ERROR, FACILITY_DEV_HW, 0xB)
#define STATUS_WRITE_SPARSE_IMAGE_FAIL      MAKE_CODE(SEV_ERROR, FACILITY_DEV_HW, 0xC)

//emmc err
#define STATUS_MMC_ERR                      MAKE_CODE(SEV_ERROR, FACILITY_DEV_HW, 0x30)

//nand err
#define STATUS_NAND_ERR                     MAKE_CODE(SEV_ERROR, FACILITY_DEV_HW, 0x40)
#define STATUS_NAND_IN_PROGRESS             MAKE_CODE(SEV_ERROR, FACILITY_DEV_HW, 0x41)
#define STATUS_NAND_TIMEOUT                 MAKE_CODE(SEV_ERROR, FACILITY_DEV_HW, 0x42)
#define STATUS_NAND_BAD_BLOCK               MAKE_CODE(SEV_ERROR, FACILITY_DEV_HW, 0x43)
#define STATUS_NAND_ERASE_FAILED            MAKE_CODE(SEV_ERROR, FACILITY_DEV_HW, 0x44)
#define STATUS_NAND_PAGE_PROGRAM_FAILED     MAKE_CODE(SEV_ERROR, FACILITY_DEV_HW, 0x45)
#define STATUS_NAND_FTL_OFFSET_JUMP_NEXT    MAKE_CODE(SEV_INFO, FACILITY_DEV_HW, 0x46)

//DRAM init error
#define STATUS_EMI_SETTING_VERSION_ERROR    MAKE_CODE(SEV_ERROR, FACILITY_DEV_HW, 0x50)

//ufs err
#define STATUS_UFS_ERR                      MAKE_CODE(SEV_ERROR, FACILITY_DEV_HW, 0x60)

//otp
#define STATUS_DA_OTP_NOT_SUPPORT           MAKE_CODE(SEV_ERROR, FACILITY_DEV_HW, 0x100)
#define STATUS_DA_OTP_LOCK_FAIL             MAKE_CODE(SEV_ERROR, FACILITY_DEV_HW, 0x102)

//emmc otp
#define STATUS_DA_EMMC_OTP_NOT_SUPPORT            STATUS_DA_OTP_NOT_SUPPORT
#define STATUS_DA_EMMC_OTP_LOCKED 			      STATUS_OTP_LOCKED  
#define STATUS_DA_EMMC_OTP_LOCK_FAIL 		      STATUS_DA_OTP_LOCK_FAIL  
#define STATUS_DA_EMMC_OTP_UNLOCKED   		      STATUS_OTP_UNLOCKED  

//Note: need modify nand driver with error code "S_..."
#define STATUS_DA_NAND_OTP_NOT_SUPPORT 	       STATUS_DA_OTP_NOT_SUPPORT  
#define STATUS_DA_NAND_OTP_LOCKED 			   STATUS_OTP_LOCKED  
#define STATUS_DA_NAND_OTP_LOCK_FAIL 		   STATUS_DA_OTP_LOCK_FAIL  
#define STATUS_DA_NAND_OTP_UNLOCKED 		   STATUS_OTP_UNLOCKED  


/* efuse */
#define STATUS_EFUSE_UNKNOWN_ERR                      MAKE_CODE(SEV_ERROR, FACILITY_DEV_HW, 0x200) 
#define STATUS_EFUSE_WRITE_TIMEOUT_WITHOUT_VERIFY     MAKE_CODE(SEV_ERROR, FACILITY_DEV_HW, 0x201)
#define STATUS_EFUSE_BLOWN_DONE                       MAKE_CODE(SEV_ERROR, FACILITY_DEV_HW, 0x202)
#define STATUS_EFUSE_REVERT_BIT                       MAKE_CODE(SEV_ERROR, FACILITY_DEV_HW, 0x203)
#define STATUS_EFUSE_BLOWN_PARTLY                     MAKE_CODE(SEV_ERROR, FACILITY_DEV_HW, 0x204) //need to reblow
#define STATUS_EFUSE_INVALID_ARGUMENT                 MAKE_CODE(SEV_ERROR, FACILITY_DEV_HW, 0x205)
#define STATUS_EFUSE_VALUE_IS_NOT_ZERO                MAKE_CODE(SEV_ERROR, FACILITY_DEV_HW, 0x206)
#define STATUS_EFUSE_BLOWN_INCORRECT_DATA             MAKE_CODE(SEV_ERROR, FACILITY_DEV_HW, 0x207)
#define STATUS_EFUSE_BROKEN                           MAKE_CODE(SEV_ERROR, FACILITY_DEV_HW, 0x208)
#define STATUS_EFUSE_BLOW_ERR                         MAKE_CODE(SEV_ERROR, FACILITY_DEV_HW, 0x209)
#define STATUS_EFUSE_DATA_PROCESS_ERR                 MAKE_CODE(SEV_ERROR, FACILITY_DEV_HW, 0x20A)
#define STATUS_EFUSE_UNLOCK_BPKEY_ERR                 MAKE_CODE(SEV_ERROR, FACILITY_DEV_HW, 0x20B)
#define STATUS_EFUSE_CREATE_LIST_ERR                   MAKE_CODE(SEV_ERROR, FACILITY_DEV_HW, 0x20C)
#define STATUS_EFUSE_WRITE_REGISTER_ERR          MAKE_CODE(SEV_ERROR, FACILITY_DEV_HW, 0x20D)
#define STATUS_EFUSE_PADDING_TYPE_MISMATCH  MAKE_CODE(SEV_ERROR, FACILITY_DEV_HW, 0x20E)

/* security */
#define STATUS_SEC_ROM_INFO_NOT_FOUND       MAKE_CODE(SEV_ERROR, FACILITY_AUTHENTICATION, 0x1)
#define STATUS_SEC_CUST_NAME_NOT_FOUND      MAKE_CODE(SEV_ERROR, FACILITY_AUTHENTICATION, 0x2)
#define STATUS_SEC_ROM_INFO_DEVICE_NOT_SUPPORTED       MAKE_CODE(SEV_ERROR, FACILITY_AUTHENTICATION, 0x3)

#define STATUS_SEC_DL_FORBIDDEN             MAKE_CODE(SEV_ERROR, FACILITY_AUTHENTICATION, 0x4)
#define STATUS_SEC_IMG_TOO_LARGE            MAKE_CODE(SEV_ERROR, FACILITY_AUTHENTICATION, 0x5)
#define STATUS_SEC_PL_VFY_FAIL              MAKE_CODE(SEV_ERROR, FACILITY_AUTHENTICATION, 0x6)
#define STATUS_SEC_IMG_VFY_FAIL             MAKE_CODE(SEV_ERROR, FACILITY_AUTHENTICATION, 0x7)
#define STATUS_SEC_HASH_OP_FAIL             MAKE_CODE(SEV_ERROR, FACILITY_AUTHENTICATION, 0x8)
#define STATUS_SEC_HASH_BINDING_CHK_FAIL    MAKE_CODE(SEV_ERROR, FACILITY_AUTHENTICATION, 0x9)
#define STATUS_SEC_INVALID_BUF              MAKE_CODE(SEV_ERROR, FACILITY_AUTHENTICATION, 0xA)
#define STATUS_SEC_BINDING_HASH_NOT_AVAIL   MAKE_CODE(SEV_ERROR, FACILITY_AUTHENTICATION, 0xB)
#define STATUS_SEC_WRITE_DATA_NOT_ALLOWED   MAKE_CODE(SEV_ERROR, FACILITY_AUTHENTICATION, 0xC)
#define STATUS_SEC_FORMAT_NOT_ALLOWED       MAKE_CODE(SEV_ERROR, FACILITY_AUTHENTICATION, 0xD)
#define STATUS_SEC_SV5_PUBK_AUTH_FAIL       MAKE_CODE(SEV_ERROR, FACILITY_AUTHENTICATION, 0xE)
#define STATUS_SEC_SV5_HASH_VFY_FAIL        MAKE_CODE(SEV_ERROR, FACILITY_AUTHENTICATION, 0xF)
#define STATUS_SEC_SV5_RSA_OP_FAIL          MAKE_CODE(SEV_ERROR, FACILITY_AUTHENTICATION, 0x10)
#define STATUS_SEC_SV5_RSA_VFY_FAIL         MAKE_CODE(SEV_ERROR, FACILITY_AUTHENTICATION, 0x11)
#define STATUS_SEC_SV5_GFH_NOT_FOUND        MAKE_CODE(SEV_ERROR, FACILITY_AUTHENTICATION, 0x12)
#define STATUS_SEC_NOT_VALID_CERT1          MAKE_CODE(SEV_ERROR, FACILITY_AUTHENTICATION, 0x13)
#define STATUS_SEC_NOT_VALID_CERT2          MAKE_CODE(SEV_ERROR, FACILITY_AUTHENTICATION, 0x14)
#define STATUS_SEC_NOT_VALID_IMGHDR         MAKE_CODE(SEV_ERROR, FACILITY_AUTHENTICATION, 0x15)
#define STATUS_SEC_SIG_SZ_NOT_VALID         MAKE_CODE(SEV_ERROR, FACILITY_AUTHENTICATION, 0x16)
#define STATUS_SEC_PSS_OP_FAIL              MAKE_CODE(SEV_ERROR, FACILITY_AUTHENTICATION, 0x17)
#define STATUS_SEC_CERT_AUTH_FAIL           MAKE_CODE(SEV_ERROR, FACILITY_AUTHENTICATION, 0x18)
#define STATUS_SEC_PUBK_AUTH_MISMATCH_N_SIZE  MAKE_CODE(SEV_ERROR, FACILITY_AUTHENTICATION, 0x19)
#define STATUS_SEC_PUBK_AUTH_MISMATCH_E_SIZE  MAKE_CODE(SEV_ERROR, FACILITY_AUTHENTICATION, 0x1A)
#define STATUS_SEC_PUBK_AUTH_MISMATCH_N       MAKE_CODE(SEV_ERROR, FACILITY_AUTHENTICATION, 0x1B)
#define STATUS_SEC_PUBK_AUTH_MISMATCH_E       MAKE_CODE(SEV_ERROR, FACILITY_AUTHENTICATION, 0x1C)
#define STATUS_SEC_PUBK_AUTH_MISMATCH_HASH    MAKE_CODE(SEV_ERROR, FACILITY_AUTHENTICATION, 0x1D)
#define STATUS_SEC_CERT_OBJ_NOT_FOUND         MAKE_CODE(SEV_ERROR, FACILITY_AUTHENTICATION, 0x1E)
#define STATUS_SEC_CERT_OID_NOT_FOUND         MAKE_CODE(SEV_ERROR, FACILITY_AUTHENTICATION, 0x1F)
#define STATUS_SEC_CERT_OUT_OF_RANGE          MAKE_CODE(SEV_ERROR, FACILITY_AUTHENTICATION, 0x20)
#define STATUS_SEC_OID_NOT_MATCH              MAKE_CODE(SEV_ERROR, FACILITY_AUTHENTICATION, 0x21)
#define STATUS_SEC_LEN_NOT_MATCH              MAKE_CODE(SEV_ERROR, FACILITY_AUTHENTICATION, 0x22)
#define STATUS_SEC_ASN1_UNKNOWN_OP            MAKE_CODE(SEV_ERROR, FACILITY_AUTHENTICATION, 0x23)
#define STATUS_SEC_OID_IDX_OUT_OF_RANGE       MAKE_CODE(SEV_ERROR, FACILITY_AUTHENTICATION, 0x24)
#define STATUS_SEC_OID_TOO_LARGE              MAKE_CODE(SEV_ERROR, FACILITY_AUTHENTICATION, 0x25)
#define STATUS_SEC_PUBK_SZ_MISMATCH           MAKE_CODE(SEV_ERROR, FACILITY_AUTHENTICATION, 0x26)
#define STATUS_SEC_SWID_MISMATCH              MAKE_CODE(SEV_ERROR, FACILITY_AUTHENTICATION, 0x27)
#define STATUS_SEC_HASH_SZ_MISMATCH           MAKE_CODE(SEV_ERROR, FACILITY_AUTHENTICATION, 0x28)
#define STATUS_SEC_IMGHDR_TYPE_MISMATCH       MAKE_CODE(SEV_ERROR, FACILITY_AUTHENTICATION, 0x29)
#define STATUS_SEC_IMG_TYPE_MISMATCH          MAKE_CODE(SEV_ERROR, FACILITY_AUTHENTICATION, 0x2A)
#define STATUS_SEC_IMGHDR_HASH_VFY_FAIL       MAKE_CODE(SEV_ERROR, FACILITY_AUTHENTICATION, 0x2B)
#define STATUS_SEC_IMG_HASH_VFY_FAIL          MAKE_CODE(SEV_ERROR, FACILITY_AUTHENTICATION, 0x2C)
#define STATUS_SEC_VIOLATE_ANTI_ROLLBACK      MAKE_CODE(SEV_ERROR, FACILITY_AUTHENTICATION, 0x2D)
#define STATUS_SEC_SECCFG_NOT_FOUND           MAKE_CODE(SEV_ERROR, FACILITY_AUTHENTICATION, 0x2E)
#define STATUS_SEC_SECCFG_MAGIC_INCORRECT     MAKE_CODE(SEV_ERROR, FACILITY_AUTHENTICATION, 0x2F)
#define STATUS_SEC_SECCFG_NOT_VALID           MAKE_CODE(SEV_ERROR, FACILITY_AUTHENTICATION, 0x30)
#define STATUS_SEC_CIPHER_MODE_INVALID        MAKE_CODE(SEV_ERROR, FACILITY_AUTHENTICATION, 0x31)
#define STATUS_SEC_CIPHER_KEY_INVALID         MAKE_CODE(SEV_ERROR, FACILITY_AUTHENTICATION, 0x32)
#define STATUS_SEC_CIPHER_DATA_UNALIGNED      MAKE_CODE(SEV_ERROR, FACILITY_AUTHENTICATION, 0x33)
#define STATUS_SEC_GFH_FILE_INFO_NOT_FOUND    MAKE_CODE(SEV_ERROR, FACILITY_AUTHENTICATION, 0x34)
#define STATUS_SEC_GFH_ANTI_CLONE_NOT_FOUND   MAKE_CODE(SEV_ERROR, FACILITY_AUTHENTICATION, 0x35)
#define STATUS_SEC_GFH_SEC_CFG_NOT_FOUND      MAKE_CODE(SEV_ERROR, FACILITY_AUTHENTICATION, 0x36)
#define STATUS_SEC_UNSUPPORTED_SOURCE_TYPE    MAKE_CODE(SEV_ERROR, FACILITY_AUTHENTICATION, 0x37)
#define STATUS_SEC_CUST_NAME_MISMATCH         MAKE_CODE(SEV_ERROR, FACILITY_AUTHENTICATION, 0x38)

#endif
