
#ifndef __HAL__
#define __HAL__

#include "sys/types.h"
#include "interface_struct.h"
#include "boot/system_objects.h"
#include "boot/protocol_callbacks.h"
#include <malloc.h>
#include "lib/string.h"

typedef enum {
   EXT_13M = 1,
   EXT_26M = 2,
   EXT_39M = 3,
   EXT_52M = 4,
   EXT_CLOCK_END,
   AUTO_DETECT_EXT_CLOCK = 254,
   UNKNOWN_EXT_CLOCK = 255
} EXT_CLOCK;

struct com_channel_struct
{
   status_t (*read)(uint8* buffer, uint32* length);
   status_t (*write)(uint8* buffer, uint32 length);  
   status_t (*log_to_pc)(const uint8* buffer, uint32 length);
   status_t (*log_to_uart)(const uint8* buffer, uint32 length);
};

extern struct com_channel_struct com_channel  ;


struct com_interface_struct
{
   status_t (*init)();
   bool (*is_usb_download)();
   status_t (*read)(uint8* buffer, uint32 length);
   status_t (*write)(uint8* buffer, uint32 length);
   status_t (*log_to_pc)(uint8* buffer, uint32 length);
   status_t (*log_to_uart)(uint8* buffer, uint32 length);
};

struct storage_interface_struct
{
   status_t (*read)(uint64 at_address, uint8* data, uint64 length, struct partition_info_struct* part_info);
   status_t (*write)(uint64 at_address, uint8* data, uint64 length, struct partition_info_struct* part_info);
   status_t (*erase)(struct partition_info_struct* part_info, const struct progress_cb* cb);
   status_t (*diagnose)(struct partition_info_struct* part_info);
   status_t (*device_ctrl)(enum storage_type storage, uint32 ctrl_code, void* in, uint32 in_len, void* out, uint32 out_len, uint32* ret_len);
};

struct device_usb_struct
{ 
   status_t (*init)(bool is_new, uint32 pid, uint32 vid);
   status_t (*device_ctrl)(uint32 ctrl_code, uint32* value);
   status_t (*read)(uint8* buffer, uint32 length);
   status_t (*write)(uint8* buffer, uint32 length); 
};

struct device_uart_struct
{ 
   void (*init)(int32 port);
   status_t (*read)(uint8* buffer, uint32 length);
   status_t (*write)(uint8* buffer, uint32 length);
};

struct device_clock_struct
{
   void (*init_pll)(); 
   uint32 (*get_uart_clock)();
   uint32 (*get_external_clock)();  //g_HW_DevCfg.m_clk_cfg.m_ext_clock = EXT_52M
};

struct device_timer_struct
{ 
   void 	(*gpt_timer_init)();
   uint32 (*get_timer_current_time_us) ();
   void 	(*udelay)(unsigned int usec);
   uint32 	(*gpt_timer_test)();
};

struct device_pmic_struct
{ 
   void (*init)();
   status_t (*set_voltage)(uint32 type, uint32 value);
   status_t (*get_voltage)(uint32 type, uint32 *value);
   status_t (*write)(addr_t address, uint32 value);
   status_t (*read)(addr_t address, uint32* value);
   void (*dump_setting)();
};

struct device_dram_struct
{ 
   uint32 (*init_dram)(uint8 emi_buf[],uint32 emi_num,uint32 m_ram_cfg_ver,uint32 emi_buf_length);
   void (*release_dram)();
   void (*detect_dram_size)(uint32 dram_baseaddr,uint32 *dram_size);
   status_t (*diagnose_dram)(uint32 offset, uint32 test_length, uint8 *results, uint32 result_buf_size);
};


struct device_mmc_struct
{ 
   status_t (*init)(uint32 msdc_channel);
   status_t (*get_info)(/*IN OUT*/struct mmc_info_struct* info);
   status_t (*device_ctrl)(uint32 ctrl_code, void* in, uint32 in_len, void* out, uint32 out_len, uint32* ret_len);
   status_t (*switch_section)(uint32 section);
   status_t (*read)(uint64 address, uint8* buffer, uint64 length);
   status_t (*write)(uint64 address, uint8* buffer, uint64 length);
   status_t (*erase)(uint64 address, uint64 length);
   status_t (*diagnose)(uint64 address, uint64 length, uint8 *results, uint32 result_buf_size);
};
struct device_ufs_struct
{ 
   status_t (*init)(uint32 msdc_channel);
   status_t (*get_info)(/*IN OUT*/struct ufs_info_struct* info);
   status_t (*device_ctrl)(uint32 ctrl_code, void* in, uint32 in_len, void* out, uint32 out_len, uint32* ret_len);
   status_t (*switch_section)(uint32 section);
   status_t (*read)(uint64 address, uint8* buffer, uint64 length);
   status_t (*write)(uint64 address, uint8* buffer, uint64 length);
   status_t (*erase)(uint64 address, uint64 length);
   status_t (*diagnose)(uint64 address, uint64 length, uint8 *results, uint32 result_buf_size);
};

struct device_nand_struct
{ 
   status_t (*init)(void);
   status_t (*get_info)(/*IN OUT*/struct nand_info_struct* info);
   status_t (*read)(uint64 address, uint8* buffer, uint64 length, struct nand_extension* extension);
   status_t (*write)(uint64 address, uint8* buffer, uint64 length, struct nand_extension* extension);
   status_t (*erase)(uint64 address, uint64 length,struct nand_extension* extension, const struct progress_cb* cb);
   status_t (*create_nfb_hdr)(uint8* data, uint32 size);
   status_t (*device_ctrl)(uint32 ctrl_code, void* in, uint32 in_len, void* out, uint32 out_len, uint32* ret_len);
   status_t (*diagnose)(uint64 address, uint64 length, struct nand_extension* extension, uint8 *results, uint32 result_buf_size);   
};


struct device_power_function_struct
{ 
   bool (*is_battery_exist)();
   uint32 (*get_battery_voltage)();
   void (*latch_power)();
   void (*release_power)();
   void (*set_reset_key)(uint32 config);
   int (*get_power_via_usb)();
   void (*enable_wdt)(uint16 interval_ms,uint32 flags) ;
   void (*disable_wdt)(void);
   void (*disable_Charger)(void);
   void (*Charger_VBUS_limit)(void);
   void (*clear_power)(void);
};

struct device_low_power_function_struct
{ 
   void (*change_charge_voltage)(uint32 voltage);
   void (*close_useless_cpu)();
   void (*close_useless_gpu)();
   void (*close_modem_1)(void);
   void (*close_modem_2)(void);
   void (*change_vcore)();
   void (*change_settings)();
};

struct efuse_interface_struct
{
   status_t (*write_efuse)(efuse_argument_t *efuse_arg, yield_argument_t *yield_arg);
   status_t (*read_efuse)(yield_argument_t *yield_arg, /*out*/efuse_argument_t *efuse_arg);
};

extern struct com_interface_struct com_interface;
extern struct storage_interface_struct storage_interface;
extern struct device_usb_struct device_usb0;
extern struct device_usb_struct device_usb_log;
//protocol
extern struct device_uart_struct device_uart0;
//for log
extern struct device_uart_struct device_uart_log;

extern struct device_timer_struct device_timer;
extern struct device_clock_struct device_clock;
extern struct device_pmic_struct device_pmic;

extern struct device_power_function_struct device_power;
extern struct device_low_power_function_struct device_low_power;

extern struct device_mmc_struct device_mmc;
extern struct device_ufs_struct device_ufs;
extern struct device_nand_struct device_nand;

extern struct device_dram_struct device_dram;

extern struct efuse_interface_struct efuse_interface;

#endif


