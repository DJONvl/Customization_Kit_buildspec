/*******************************************************************************
 *  Copyright Statement:
 *  --------------------
 *  This software is protected by Copyright and the information contained
 *  herein is confidential. The software may not be copied and the information
 *  contained herein may not be used or disclosed except with the written
 *  permission of MediaTek Inc. (C) 2006
 *
 ******************************************************************************/
#ifndef _MTK_STRUCT_SYNC_H_
#define _MTK_STRUCT_SYNC_H_

#define GFH_TOOL_AUTH_v1_padding    1904
#define GFH_SCTRL_CERT_v1_padding   544

/* Add for flashtoollib sync with DA about the BMT size(Block count or BMT sizes), 2013/06/20 { */
typedef enum {
    BMT_IN_BLOCK_COUT = 0,
    BMT_IN_PARTITION_SIZE = 1
}BMT_UNIT;
/* Add for flashtoollib sync with DA about the BMT size(Block count or BMT sizes), 2013/06/20 } */

/* Add for flashtoollib sync with DA about w/o battery configure, 2013/07/03 { */
typedef enum {
    DL_WO_BATTERY = 0,
    DL_WITH_BATTERY = 1,
    DL_AUTO_DETCECT_BAT
}DL_WO_BAT_CONFIGURE;
/* Add for flashtoollib sync with DA about w/o battery configure, 2013/07/03  } */

/* Add for flashtoollib sync with DA about PMIC_HW_RESET key configure, 2013/07/03 { */
typedef enum {
    LONG_PRESS_POWERKEY_ONLY = 0,
    LONG_PRESS_POWERKEY_AND_HOMEKEY = 1
}PMIC_HW_RESET_CONFIGURE;
/* Add for flashtoollib sync with DA about PMIC_HW_RESET key configure, 2013/07/03  } */

/* Add for flashtoollib sync with DA about MLC/SLC Nand configure, 2013/10/22 { */
typedef enum {
	NAND_ARCH_NONE = 0,
	NAND_ARCH_SLC = 1,
	NAND_ARCH_MLC = 2
}NAND_ARCH_TYPE;
/* Add for flashtoollib sync with DA about MLC/SLC Nand configure, 2013/10/22  } */
#endif

