
#ifndef __ARGUMENTS__
#define __ARGUMENTS__

#include "sys/types.h"


#define DA_ARG_MAGIC               0x58885168
#define DA_ARG_VER                 1

#define DA_FLAG_PLL_ALREADY_INIT      0x00000001
#define DA_FLAG_EMI_ALREADY_INIT      0x00000002

#define NEED_PLL_SETTING !(sysob_bootstrap1_arg.flags & DA_FLAG_PLL_ALREADY_INIT)
#define NEED_EMI_SETTING !(sysob_bootstrap1_arg.flags & DA_FLAG_EMI_ALREADY_INIT)

struct runtime_parameters
{
   uint32 checksum_level;        //usb.emmc.both
   uint32 da_log_level;        //log level 1-5
   uint32 da_log_path_flag;        //uart | usb
   uint32 system_os;             //windows | linux
   uint32 write_packet_length;   //protocol write data packet length
   uint32 read_packet_length;    //protocol read data packet length
   uint32 max_sec_img_length;    /* max size of authenticated image */
   uint32 flag;
   uint32 power_source_config;   //from battery or USB, auto,set form pc
   uint32 reset_key_setting;   //one key or two key to reset, none
};

struct bootstrap1_arg
{
   uint32 magic;
   uint32 ver;
   uint32 flags;
};

//typedef struct ram_item_struct ram_item_struct;
#define BOOT2_ARG_MAGIC               0xFE4A4D42   //0xFE,BMJ

struct bootstrap2_arg
{
   uint32 magic;
   BOOL preloader_exist; /*brom mode:false; preloader mode: true*/	
   struct ram_item_struct dram_info;
   struct runtime_parameters runtime_params;

   //maybe add some later.
};

#endif


