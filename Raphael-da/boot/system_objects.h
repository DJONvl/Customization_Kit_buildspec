
#ifndef __SYSTEM_OBJECTS__
#define __SYSTEM_OBJECTS__
#include "sys/types.h"
#include "interface_struct.h"
#include "device_parameters_interface.h"
#include "boot/arguments.h"

struct hw_parameters
{
   uint32 secure_version;
   uint32 bootloader_version;
   uint32 msdc_channel;        //msdc0 or msdc3
//   bool   battery_exist; 		//set from hw
};

typedef struct partition_info_struct
{
   enum storage_type storage; //emmc, slc, nand, nor, ufs.
   uint32 section;  //boot,user of emmc, LU1, LU2 for ufs.
   uint8  name[PARTITION_NAME_LEN];
   uint64 base_addr;	
   uint64 max_size;
   struct partition_extension_struct extension; // mlc will use nand_mlc_extension, etc.
}partition_info_struct_t;


extern struct chip_id_struct sysob_chip_baseband;
extern struct ram_info_struct sysob_ram;
extern struct runtime_parameters sysob_runtime_params;
extern struct hw_parameters sysob_hw_params;
extern struct bootstrap1_arg sysob_bootstrap1_arg;
extern struct bootstrap2_arg sysob_bootstrap2_arg;
extern struct mmc_info_struct sysob_mmc;
extern struct ufs_info_struct sysob_ufs;

extern struct nand_info_struct sysob_nand;
extern struct nor_info_struct sysob_nor;
//extern struct dl_info* sysob_dl_info;
//extern struct download_history* sysob_dl_history;
extern struct download_archive* sysob_dl_record;
extern uint64 sysob_record_length;
extern struct ufs_info_struct sysob_ufs;

extern struct host_info_struct* sysob_host_info;
extern struct device_charger_parameters* sysob_charger_init_param;

void init_system_objects();
#endif


