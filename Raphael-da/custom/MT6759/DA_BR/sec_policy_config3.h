#ifndef _SEC_POLICY_CONFIG3_H_
#define _SEC_POLICY_CONFIG3_H_

/* sboot: enable, lock_state: lock */

/* download policy */
/* 0: permit download, 1: forbid download */
#define DL_DEFAULT_POLICY_3 		0
#define DL_PL_POLICY_3 				0
#define DL_LK_POLICY_3 				0	
#define DL_LOGO_POLICY_3 			0
#define DL_BOOT_POLICY_3 			0
#define DL_SYSTEM_POLICY_3 			0
#define DL_TEE1_POLICY_3 			0
#define DL_TEE2_POLICY_3 			0
#define DL_CACHE_POLICY_3 			0
#define DL_SECRO_POLICY_3 			0
#define DL_OEMKEYSTORE_POLICY_3 	0
#define DL_KEYSTORE_POLICY_3 		0
#define DL_USERDATA_POLICY_3 		0
#define DL_MD1IMG_POLICY_3 			0
#define DL_MD1DSP_POLICY_3 			0
#define DL_MD1ARM7_POLICY_3 		0
#define DL_MD3IMG_POLICY_3 			0
#define DL_SGPT_POLICY_3 			0
#define DL_PGPT_POLICY_3 			0

/* verify policy */
/*0: skip verify, 1: need verify */
#define VERIFY_DEFAULT_POLICY_3 	1
#define VERIFY_PL_POLICY_3 			1
#define VERIFY_LK_POLICY_3 			1
#define VERIFY_LOGO_POLICY_3 		1
#define VERIFY_BOOT_POLICY_3 		1
#define VERIFY_SYSTEM_POLICY_3      0
#define VERIFY_TEE1_POLICY_3 		1
#define VERIFY_TEE2_POLICY_3 		1
#define VERIFY_CACHE_POLICY_3 		0
#define VERIFY_SECRO_POLICY_3 		0
#define VERIFY_OEMKEYSTORE_POLICY_3 1
#define VERIFY_KEYSTORE_POLICY_3 	1
#define VERIFY_USERDATA_POLICY_3 	0
#define VERIFY_MD1IMG_POLICY_3 		1
#define VERIFY_MD1DSP_POLICY_3 		1
#define VERIFY_MD1ARM7_POLICY_3 	1
#define VERIFY_MD3IMG_POLICY_3 		1
#define VERIFY_SGPT_POLICY_3 		0
#define VERIFY_PGPT_POLICY_3 		0

/* bind to hash policy */
/* 0: skip bind, 1: bind to hash */
#define BIND_DEFAULT_POLICY_3  		0
#define BIND_PL_POLICY_3   			0
#define BIND_LK_POLICY_3   			0		 
#define BIND_LOGO_POLICY_3   		0
#define BIND_BOOT_POLICY_3 			0
#define BIND_SYSTEM_POLICY_3 		0
#define BIND_TEE1_POLICY_3 			0
#define BIND_TEE2_POLICY_3 			0
#define BIND_CACHE_POLICY_3 		0
#define BIND_SECRO_POLICY_3 		0
#define BIND_OEMKEYSTORE_POLICY_3 	0
#define BIND_KEYSTORE_POLICY_3 		0
#define BIND_USERDATA_POLICY_3 		0
#define BIND_MD1IMG_POLICY_3 		0
#define BIND_MD1DSP_POLICY_3 		0
#define BIND_MD1ARM7_POLICY_3 		0
#define BIND_MD3IMG_POLICY_3 		0
#define BIND_SGPT_POLICY_3 		    0
#define BIND_PGPT_POLICY_3 		    0

/* create ap image policy */
#define DEFAULT_SEC_POLICY_3 	CREATE_POLICY_ENTRY(BIND_DEFAULT_POLICY_3, VERIFY_DEFAULT_POLICY_3, DL_DEFAULT_POLICY_3)
#define PL_SEC_POLICY_3     	CREATE_POLICY_ENTRY(BIND_PL_POLICY_3, VERIFY_PL_POLICY_3, DL_PL_POLICY_3)
#define LK_SEC_POLICY_3    		CREATE_POLICY_ENTRY(BIND_LK_POLICY_3, VERIFY_LK_POLICY_3, DL_LK_POLICY_3)
#define LOGO_SEC_POLICY_3    	CREATE_POLICY_ENTRY(BIND_LOGO_POLICY_3, VERIFY_LOGO_POLICY_3, DL_LOGO_POLICY_3)
#define BOOT_SEC_POLICY_3   	CREATE_POLICY_ENTRY(BIND_BOOT_POLICY_3, VERIFY_BOOT_POLICY_3, DL_BOOT_POLICY_3)
#define SYSTEM_SEC_POLICY_3  	CREATE_POLICY_ENTRY(BIND_SYSTEM_POLICY_3, VERIFY_SYSTEM_POLICY_3, DL_SYSTEM_POLICY_3)
#define TEE1_SEC_POLICY_3  		CREATE_POLICY_ENTRY(BIND_TEE1_POLICY_3, VERIFY_TEE1_POLICY_3, DL_TEE1_POLICY_3)
#define TEE2_SEC_POLICY_3  		CREATE_POLICY_ENTRY(BIND_TEE2_POLICY_3, VERIFY_TEE2_POLICY_3, DL_TEE2_POLICY_3)
#define CACHE_SEC_POLICY_3  	CREATE_POLICY_ENTRY(BIND_CACHE_POLICY_3, VERIFY_CACHE_POLICY_3, DL_CACHE_POLICY_3)
#define SECRO_SEC_POLICY_3  	CREATE_POLICY_ENTRY(BIND_SECRO_POLICY_3, VERIFY_SECRO_POLICY_3, DL_SECRO_POLICY_3)
#define OEMKEYSTORE_SEC_POLICY_3 	CREATE_POLICY_ENTRY(BIND_OEMKEYSTORE_POLICY_3, VERIFY_OEMKEYSTORE_POLICY_3, DL_OEMKEYSTORE_POLICY_3)
#define KEYSTORE_SEC_POLICY_3  	CREATE_POLICY_ENTRY(BIND_KEYSTORE_POLICY_3, VERIFY_KEYSTORE_POLICY_3, DL_KEYSTORE_POLICY_3)
#define USERDATA_SEC_POLICY_3  	CREATE_POLICY_ENTRY(BIND_USERDATA_POLICY_3, VERIFY_USERDATA_POLICY_3, DL_USERDATA_POLICY_3)

#define SGPT_SEC_POLICY_3       CREATE_POLICY_ENTRY(BIND_SGPT_POLICY_3, VERIFY_SGPT_POLICY_3, DL_SGPT_POLICY_3)
#define PGPT_SEC_POLICY_3       CREATE_POLICY_ENTRY(BIND_PGPT_POLICY_3, VERIFY_PGPT_POLICY_3, DL_PGPT_POLICY_3)

/* create md image policy */
#define MD1IMG_SEC_POLICY_3		CREATE_POLICY_ENTRY(BIND_MD1IMG_POLICY_3, VERIFY_MD1IMG_POLICY_3, DL_MD1IMG_POLICY_3)
#define MD1DSP_SEC_POLICY_3		CREATE_POLICY_ENTRY(BIND_MD1DSP_POLICY_3, VERIFY_MD1DSP_POLICY_3, DL_MD1DSP_POLICY_3)
#define MD1ARM7_SEC_POLICY_3	CREATE_POLICY_ENTRY(BIND_MD1ARM7_POLICY_3, VERIFY_MD1ARM7_POLICY_3, DL_MD1ARM7_POLICY_3)
#define MD3IMG_SEC_POLICY_3		CREATE_POLICY_ENTRY(BIND_MD3IMG_POLICY_3, VERIFY_MD3IMG_POLICY_3, DL_MD3IMG_POLICY_3)

/* custom image security policy */
#define  DL_CUST1_POLICY_3       	0
#define  VERIFY_CUST1_POLICY_3   	0
#define  BIND_CUST1_POLICY_3     	0

#define  DL_CUST2_POLICY_3       	0
#define  VERIFY_CUST2_POLICY_3   	0
#define  BIND_CUST2_POLICY_3    	0

/* create custom image policy */
#define  CUST1_SEC_POLICY_3  CREATE_POLICY_ENTRY(BIND_CUST1_POLICY_3, VERIFY_CUST1_POLICY_3, DL_CUST1_POLICY_3)
#define  CUST2_SEC_POLICY_3  CREATE_POLICY_ENTRY(BIND_CUST2_POLICY_3, VERIFY_CUST2_POLICY_3, DL_CUST2_POLICY_3)

#endif
