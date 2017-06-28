#ifndef __COMMAND_DEFINE__
#define __COMMAND_DEFINE__

#define MAKE_COMMAND_ID(catagory, function) ((catagory << 16) | function) 

#define CATA_MAIN                1
#define CATA_DEVICE_SET_PARAM    2
#define CATA_DEVICE_GET_PARAM    4
#define CATA_DEVICE_ACTION       8
#define CATA_DEVICE_CTRL         (CATA_DEVICE_SET_PARAM|CATA_DEVICE_GET_PARAM|CATA_DEVICE_ACTION)

#define IS_DEVICE_SET_PARAM_CATA(id) ((id & MAKE_COMMAND_ID(CATA_DEVICE_SET_PARAM,0)) != 0)
#define IS_DEVICE_GET_PARAM_CATA(id) ((id & MAKE_COMMAND_ID(CATA_DEVICE_GET_PARAM,0)) != 0)
#define IS_DEVICE_ACTION_CATA(id)    (id & MAKE_COMMAND_ID(CATA_DEVICE_ACTION,0) != 0)

//main
#define CMD_UNKNOWN        MAKE_COMMAND_ID(CATA_MAIN, 0)
#define CMD_DOWNLOAD       MAKE_COMMAND_ID(CATA_MAIN, 1)
#define CMD_UPLOAD         MAKE_COMMAND_ID(CATA_MAIN, 2)
#define CMD_FORMAT         MAKE_COMMAND_ID(CATA_MAIN, 3)
#define CMD_WRITE_DATA     MAKE_COMMAND_ID(CATA_MAIN, 4)
#define CMD_READ_DATA      MAKE_COMMAND_ID(CATA_MAIN, 5)
#define CMD_FORMAT_PARTITION        MAKE_COMMAND_ID(CATA_MAIN, 6)
#define CMD_SHUTDOWN                MAKE_COMMAND_ID(CATA_MAIN, 7)
#define CMD_BOOT_TO                 MAKE_COMMAND_ID(CATA_MAIN, 8)
#define CMD_DEVICE_CTRL             MAKE_COMMAND_ID(CATA_MAIN, 9)
#define CMD_INIT_EXT_RAM            MAKE_COMMAND_ID(CATA_MAIN, 0xA)
#define CMD_SWITCH_USB_SPEED        MAKE_COMMAND_ID(CATA_MAIN, 0xB)
#define CMD_READ_OTP_ZONE           MAKE_COMMAND_ID(CATA_MAIN, 0xC)
#define CMD_WRITE_OTP_ZONE          MAKE_COMMAND_ID(CATA_MAIN, 0xD)
#define CMD_WRITE_EFUSE       MAKE_COMMAND_ID(CATA_MAIN, 0xE)
#define CMD_READ_EFUSE        MAKE_COMMAND_ID(CATA_MAIN, 0xF)
#define CMD_NAND_BMT_REMARK        MAKE_COMMAND_ID(CATA_MAIN, 0x10)

#define SPECIAL_CMD_SETUP_ENVIRONMENT    MAKE_COMMAND_ID(CATA_MAIN, 0x100)
#define SPECIAL_CMD_SETUP_HW_INIT_PARAMS    MAKE_COMMAND_ID(CATA_MAIN, 0x101)

#define SYNC_SIGNAL 0x434E5953  //"SYNC"

//device ctrl code, need send parameter and result data return.
#define CC_UNKNOWN_CTRL_CODE   MAKE_COMMAND_ID(CATA_DEVICE_CTRL, 0)
#define CC_CTRL_STORAGE_TEST  MAKE_COMMAND_ID(CATA_DEVICE_CTRL, 1)
#define CC_CTRL_RAM_TEST   MAKE_COMMAND_ID(CATA_DEVICE_CTRL, 2)
//get parameter, need result data
#define CC_GET_EMMC_INFO            MAKE_COMMAND_ID(CATA_DEVICE_GET_PARAM, 1)
#define CC_GET_NAND_INFO            MAKE_COMMAND_ID(CATA_DEVICE_GET_PARAM, 2)
#define CC_GET_NOR_INFO             MAKE_COMMAND_ID(CATA_DEVICE_GET_PARAM, 3)
#define CC_GET_UFS_INFO             MAKE_COMMAND_ID(CATA_DEVICE_GET_PARAM, 4)
#define CC_GET_VERSION              MAKE_COMMAND_ID(CATA_DEVICE_GET_PARAM, 5)
#define CC_GET_EXPIRE_DATA          MAKE_COMMAND_ID(CATA_DEVICE_GET_PARAM, 6)
#define CC_GET_PACKET_LENGTH        MAKE_COMMAND_ID(CATA_DEVICE_GET_PARAM, 7)
#define CC_GET_RAMDOM_ID            MAKE_COMMAND_ID(CATA_DEVICE_GET_PARAM, 8)
#define CC_GET_PARTITION_TBL_CATA   MAKE_COMMAND_ID(CATA_DEVICE_GET_PARAM, 9)
#define CC_GET_CONNECTION_AGENT     MAKE_COMMAND_ID(CATA_DEVICE_GET_PARAM, 0xA)
#define CC_GET_USB_SPEED            MAKE_COMMAND_ID(CATA_DEVICE_GET_PARAM, 0xB)
#define CC_GET_RAM_INFO             MAKE_COMMAND_ID(CATA_DEVICE_GET_PARAM, 0xC)
#define CC_GET_CHIP_ID              MAKE_COMMAND_ID(CATA_DEVICE_GET_PARAM, 0xD)
#define CC_GET_OTP_LOCK_STATUS      MAKE_COMMAND_ID(CATA_DEVICE_GET_PARAM, 0xE)
#define CC_GET_BATTERY_VOLTAGE      MAKE_COMMAND_ID(CATA_DEVICE_GET_PARAM, 0xF)
#define CC_GET_RPMB_STATUS			MAKE_COMMAND_ID(CATA_DEVICE_GET_PARAM, 0x10)
#define CC_GET_EXPIRE_DATE			MAKE_COMMAND_ID(CATA_DEVICE_GET_PARAM, 0x11)

/**************************************/
//set parameter, no result data return.

#define CC_SET_BMT_PERCENTAGE   MAKE_COMMAND_ID(CATA_DEVICE_SET_PARAM, 1)
#define CC_SET_BATTERY_OPT      MAKE_COMMAND_ID(CATA_DEVICE_SET_PARAM, 2)
#define CC_SET_CHECKSUM_LEVEL   MAKE_COMMAND_ID(CATA_DEVICE_SET_PARAM, 3)
#define CC_SET_RESET_KEY  	 	  MAKE_COMMAND_ID(CATA_DEVICE_SET_PARAM, 4)
#define CC_SET_HOST_INFO  	 	  MAKE_COMMAND_ID(CATA_DEVICE_SET_PARAM, 5)
#define CC_SET_META_BOOT_MODE  MAKE_COMMAND_ID(CATA_DEVICE_SET_PARAM, 6)
#define CC_SET_EMMC_HWRESET_PIN  MAKE_COMMAND_ID(CATA_DEVICE_SET_PARAM, 7)
#define CC_SET_GENERATE_GPX    MAKE_COMMAND_ID(CATA_DEVICE_SET_PARAM, 8)

//device action, no parameter and result data needed.
#define CC_START_DL_INFO         MAKE_COMMAND_ID(CATA_DEVICE_ACTION, 1)
#define CC_END_DL_INFO           MAKE_COMMAND_ID(CATA_DEVICE_ACTION, 2)
#define CC_ACT_LOCK_OTP_ZONE     MAKE_COMMAND_ID(CATA_DEVICE_ACTION, 3)
#define CC_DISABLE_EMMC_HWRESET_PIN MAKE_COMMAND_ID(CATA_DEVICE_ACTION, 4)

#endif
