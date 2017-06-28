/*
* Copyright (c) 2016 MediaTek
*
*/
//Custom GPIO For Enable External LDO
#include "gpio_cus.h"
#include "gpio_pin.h"
unsigned int get_efuse_gpio_num(){
    return GPIO_EFUSE_LDO;
}