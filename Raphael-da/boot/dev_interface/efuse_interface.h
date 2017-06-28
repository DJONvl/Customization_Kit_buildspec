#include "common_struct.h"
#include "efuse_struct.h"

extern status_t EFuse_Write(efuse_argument_t *efuse_arg, yield_argument_t *yield_arg);
extern status_t EFuse_Read(yield_argument_t *yield_arg, efuse_argument_t *efuse_arg);
