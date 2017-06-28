#ifndef __DRIVER_CUSTOM_H__
#define __DRIVER_CUSTOM_H__

/*
0: rtc will not reset when first boot.
1: rtc reset when first boot by rtc_first_boot_init.
*/
#define RTC_FIRST_BOOT_INIT 1

unsigned int first_boot();

#endif
