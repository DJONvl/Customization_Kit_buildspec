#ifndef _AUTHENTICATION_H
#define _AUTHENTICATION_H

#include "cust_sec.h"

/**************************************************************************
 * EXPORT FUNCTION
**************************************************************************/
extern void lib_init_key(CUST_SEC_INTER  input_cus_inter);
extern int lib_hash(unsigned char* data_buf,  unsigned int data_len, unsigned char* hash_buf, unsigned int hash_len);
extern int lib_verify(unsigned char* data_buf,  unsigned int data_len, unsigned char* sig_buf, unsigned int sig_len);

#endif /* _AUTHENTICATION_H */                   

