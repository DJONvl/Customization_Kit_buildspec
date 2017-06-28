
#ifndef __API_INTERFACE__
#define __API_INTERFACE__

#include "common_struct.h"
#include "efuse_struct.h"

#define PARTITION_NAME_LEN 64


#define DA_LOG_PATH_NONE (0)
#define DA_LOG_PATH_UART (1<<0)
#define DA_LOG_PATH_USB (1<<1)
#define DA_LOG_PATH_UART_USB (DA_LOG_PATH_UART | DA_LOG_PATH_USB)

typedef enum da_log_level_e
{
   dtrace = 0,
   ddebug,
   dinfo,
   dwarning,
   derror,
   dfatal
}da_log_level_e;

typedef enum checksum_algorithm{
   CHK_ALGO_PLAIN = 0,
   CHK_ALGO_CRC32,
   CHK_ALGO_MD5,
} checksum_algorithm_e;

typedef enum ft_system_os_e
{
   FT_SYSTEM_OS_WIN = 0,
   FT_SYSTEM_OS_LINUX
}ft_system_os_e;

typedef struct device_environment
{
   uint32 log_level;  //6 level, string
   uint32 log_channel; //usb & uart
   uint32 system_os; 
   uint32 reserved2;
   uint32 reserved3;
}device_environment;

typedef struct wr_packet_length
{
   uint32 write_packet_length;
   uint32 read_packet_length;
}wr_packet_length;

/********************/
/*Internal structure.*/
/********************/
typedef struct partition_parameter_struct
{
   enum storage_type storage; //emmc, slc, nand, nor, ufs.
   uint32 section;  //boot,user of emmc, LU1, LU2 for ufs.
   uint64 address;	
   uint64 size;
   struct partition_extension_struct extension; // mlc will use nand_mlc_extension, etc.
}partition_parameter_struct_t;

typedef struct boot_to_param_struct
{     
   uint64 at_address;	
   uint64 code_length;
}boot_to_param_struct_t;


#define TIMESTAMP_LENGTH 16
#define HOST_INFO_LENGTH 384
typedef struct host_info_struct
{     
   char8  time_stamp[TIMESTAMP_LENGTH];	//20160303&110159
   char8  info[HOST_INFO_LENGTH];
}host_info_struct_t;

//EMI related.
#define BLOADER_INFO_ID_BEGIN	"MTK_BLOADER_INFO_v"
#define BLOADER_INFO_ID_LEN	24
#define BLOADER_INFO_NAME_LEN	64

typedef struct 
{
   int8     id[BLOADER_INFO_ID_LEN];
   int8     filename[BLOADER_INFO_NAME_LEN];
   int8     version[4];
   uint32   chksum_seed;
   uint32   start_address;
   int8     bin_id[8];
   uint32   emi_count;
}BLOADER_INFO_V05_T;

struct mtk_boot_mode_flag
{
    unsigned char  boot_mode;  //0:normal, 1: meta,  бн
    unsigned char  com_type;    //0:unknow com,1:uart, 2:usb
    unsigned char  com_id;     //0:single interface device, 1: composite device 
} ;

#endif


