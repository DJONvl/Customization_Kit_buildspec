
#ifndef USB_INTERFACE_H
#define USB_INTERFACE_H

#include "sys/types.h"

typedef enum {
   USB_FULL_SPEED = 1,
   USB_HIGH_SPEED = 2,
   USB_ULTRA_HIGH_SPEED = 3,
   USB_STATUS_UNKNOWN = 0xFFFF
}USB_SPEED_STATUS_E;


typedef enum {
   USB_CRCODE_GET_SPEED = 1,
   USB_CRCODE_ENABLE_DMA = 2,
   USB_CRCODE_DISCONNECT_WITH_PC =3,
   USB_CRCODE_QMU_MODE = 4
}USB_CRTL_CODE_E;


extern status_t USB_Startup(bool is_new, uint32 pid, uint32 vid);
extern status_t USB_DeviceCtrl(uint32 ctrl_code, uint32* value);
extern status_t USB_Write(uint8* buffer, uint32 length);
extern status_t USB_Read(uint8* buffer, uint32 length);
extern bool USB_IsUsbDownload();


#endif /* USB_INTERFACE_H */

