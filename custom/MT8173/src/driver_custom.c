#include "driver_custom.h"

/*
0: rtc will not reset when first boot.
1: rtc reset when first boot by rtc_first_boot_init.
*/
unsigned int first_boot()
{
  return RTC_FIRST_BOOT_INIT;
}


