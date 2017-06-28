#ifndef __CONFIG_H__
#define __CONFIG_H__

#include "sys/types.h"

typedef enum cfg_uart_key
{
   CFG_UART_DOWNLOAD_PORT = 1,
   CFG_USB_DOWNLOAD_LOG_PORT,
   CFG_UART_DOWNLOAD_LOG_PORT
}cfg_uart_key_e;

BOOL has_uart_download_log_port();
int get_uart_port_of(cfg_uart_key_e key);

#endif