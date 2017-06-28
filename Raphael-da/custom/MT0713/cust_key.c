#include <debug.h>
#include "oemkey.h"
#include "boot/security_export/sha1.h"
int cust_pubk_init(void)
{
   int ret = 0;

   unsigned char oemkey[OEM_PUBK_SZ] = {OEM_PUBK};
   seclib_set_oemkey(oemkey, OEM_PUBK_SZ);

   return ret;
}

