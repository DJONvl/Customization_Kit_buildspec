
#ifndef __DA_COMMANDS__
#define __DA_COMMANDS__

#include "hal.h"
#include "command_define.h"

//COMMAND IMPLEMENT
extern int unknown_command_guard(struct com_channel_struct* channel);
extern int cmd_download(struct com_channel_struct* channel);
extern int cmd_upload(struct com_channel_struct* channel);
extern int cmd_format(struct com_channel_struct* channel);
extern int cmd_write_data(struct com_channel_struct* channel);
extern int cmd_read_data(struct com_channel_struct* channel);
extern int cmd_format_partition(struct com_channel_struct* channel);
extern int cmd_boot_to(struct com_channel_struct* channel);
extern int cmd_shutdown(struct com_channel_struct* channel);
extern int cmd_init_dram(struct com_channel_struct* channel);

extern int cmd_switch_usb_speed(struct com_channel_struct* channel);

extern int cmd_read_otp_zone(struct com_channel_struct* channel);
extern int cmd_write_otp_zone(struct com_channel_struct* channel);

extern int cmd_write_efuse(struct com_channel_struct* channel);
extern int cmd_read_efuse(struct com_channel_struct* channel);

extern int cmd_nand_bmt_remark(struct com_channel_struct* channel);

extern int devc_set_battery_opt(struct com_channel_struct* channel);
extern int devc_set_reset_key(struct com_channel_struct* channel);
extern int devc_set_checksum_level(struct com_channel_struct* channel);
extern int devc_set_meta_boot_flag(struct com_channel_struct* channel);

extern int devc_get_partition_tbl_catagory(struct com_channel_struct* channel);
extern int devc_get_version(struct com_channel_struct* channel);
extern int devc_get_packet_length(struct com_channel_struct* channel);
extern int devc_get_emmc_info(struct com_channel_struct* channel);
extern int devc_get_nand_info(struct com_channel_struct* channel);
extern int devc_get_nor_info(struct com_channel_struct* channel);
extern int devc_get_ufs_info(struct com_channel_struct* channel);
extern int devc_get_ram_info(struct com_channel_struct* channel);
extern int devc_get_connection_agent(struct com_channel_struct* channel);
extern int devc_get_usb_speed(struct com_channel_struct* channel);
extern int devc_get_chip_baseband(struct com_channel_struct * channel);
extern int devc_get_random_id(struct com_channel_struct* channel);
extern int devc_check_rpmb_key_status(struct com_channel_struct* channel);


extern int devc_start_dl_info();
extern int devc_end_dl_info();
extern int devc_lock_otp_zone(struct com_channel_struct* channel);

extern int devc_ctrl_ram_test(struct com_channel_struct* channel);
extern int devc_ctrl_storage_test(struct com_channel_struct* channel);
extern int devc_check_status_otp_zone(struct com_channel_struct* channel);
extern int devc_check_battery_voltage( struct com_channel_struct* channel);

extern int devc_disable_emmc_reset_pin(struct com_channel_struct* channel);

extern int devc_set_host_info( struct com_channel_struct* channel);
extern int devc_set_emmc_hwreset_pin( struct com_channel_struct* channel);

extern int devc_get_expire_date( struct com_channel_struct* channel);
#endif


