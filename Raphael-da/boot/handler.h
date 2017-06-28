
#ifndef __DA_HANDLER__
#define __DA_HANDLER__
#include <boot/hal.h>
typedef int (*HHANDLE)(struct com_channel_struct*);

struct dagent_cmd {
   struct dagent_cmd *next;
   unsigned int id;
   unsigned int sec_support;
   HHANDLE handle;
};

struct device_ctrl_cmd {
   struct device_ctrl_cmd *next;
   unsigned int ctrl_code;
   HHANDLE handle;
};

extern void register_major_command(unsigned int cmd_id, HHANDLE handle);
extern void register_device_ctrl(unsigned int ctrl_code, HHANDLE handle);
extern void dagent_handler();
extern int cmd_device_ctrl(struct com_channel_struct* channel);
extern void platform_early_init();
extern void platform_init();
extern void dagent_register_commands();

#endif

