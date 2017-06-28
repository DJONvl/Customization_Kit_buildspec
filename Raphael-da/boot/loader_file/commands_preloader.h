#ifndef __COMMANDS_PRELOADER_H_
#define __COMMANDS_PRELOADER_H_

#include "sys/types.h"

status_t cmd_download_preloader(uint64 start_address, uint64 data_length, partition_info_struct_t* part_info, transfer_control_t* ctrl);

#endif