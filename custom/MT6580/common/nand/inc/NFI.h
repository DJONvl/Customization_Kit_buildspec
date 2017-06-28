/*****************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of MediaTek Inc. (C) 2001
*
*****************************************************************************/
#ifndef _MTK_DVC_TEST_NFI_H
#define _MTK_DVC_TEST_NFI_H

#include "SW_TYPES.H"
#include "hw_config.h"

//------------------------------------------------------------------------------
// New Sub-System Feature                                                          
//------------------------------------------------------------------------------
#if defined(BB_MT8135)
    #define __MTK_NFI_V4__
    #define __MTK_NFI_V3_1__
    #include "nfi_v4.h"
#elif defined(BB_MT6580)
	#define __MTK_NFI_V5__
	#define __MTK_NFI_V3_1__
	#define __MTK_SNAND_V3__
	#include "nfi_v5.h"
       #include "ecc_v3.h"
	#include "snand_hw_v3.h"
#else
    #define __MTK_NFI_V3_1__
    #define __MTK_SNAND_V3__
    #include "nfi_v3_1.h"
    #include "ecc_v2.h"
    #include "snand_hw_v3.h"
#endif
#endif
