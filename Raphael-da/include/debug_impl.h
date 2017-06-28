#ifndef __DEBUG_IMPL_H
#define __DEBUG_IMPL_H
#include <sys/types.h>

int _dputs(const uint8 *str);


void set_log_level(uint32 level);
uint32 get_log_level();

#endif