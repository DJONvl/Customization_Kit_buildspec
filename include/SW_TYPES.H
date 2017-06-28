/*******************************************************************************
 *  Copyright Statement:
 *  --------------------
 *  This software is protected by Copyright and the information contained
 *  herein is confidential. The software may not be copied and the information
 *  contained herein may not be used or disclosed except with the written
 *  permission of MediaTek Inc. (C) 2003
 *
 *******************************************************************************/

/*******************************************************************************
 * Filename:
 * ---------
 *   sw_types.h
 *
 * Project:
 * --------
 *   Download Agent
 *
 * Description:
 * ------------
 *   Common type and structure definition
 *
 * Author:
 * -------
 *   Amos Hsu
 *
 *------------------------------------------------------------------------------
 * $Revision:   1.1  $
 * $Modtime:   Nov 06 2003 15:32:26  $
 * $Log:   //mtkvs01/vmdata/flash_tool/archives/DA/INC/SW_TYPES.H-arc  $
 * 
 *    Rev 1.1   Nov 12 2003 14:14:44   mtk00539
 * Add SOC feature
 * Resolution for 35: [BootRom v2.1.1005][New] Add SOC feature.
 *
 *******************************************************************************/

#ifndef SW_TYPES_H
#define SW_TYPES_H

#include <string.h>

/*
 * general definitions
 */

typedef signed char    int8;
typedef signed char    INT8;
typedef signed short   int16;
typedef signed short   INT16;
typedef signed int     int32;
typedef signed int     INT32;
typedef signed int     intx;
typedef unsigned char  uint8;
typedef unsigned char  UINT8;
typedef unsigned short uint16;
typedef unsigned short UINT16;
typedef unsigned int   uint32;
typedef unsigned int   UINT32;

typedef unsigned long long uint64;
typedef unsigned int   uintx;
typedef signed char    S8;
typedef unsigned char  U8;
typedef signed short   S16;
typedef unsigned short U16;
typedef unsigned short u16;

typedef unsigned int   U32;
typedef unsigned long long U64;
typedef int32          S32;
typedef unsigned int   Ux;
typedef unsigned char*  P_U8;
typedef unsigned short* P_U16;
typedef unsigned int*  P_U32;
typedef unsigned int*   P_Ux;

#ifndef __cplusplus
typedef unsigned char  bool;
typedef unsigned char  BOOL;
#endif

/*
 * Definitions for BOOLEAN
 */

#define FALSE          0
#define TRUE           1




/*
 * Definitions for NULL
 */
#ifndef NULL
#ifndef __cplusplus
#define NULL          ((void*)0)
#else
#define NULL          0
#endif
#endif

#endif  /* SW_TYPES_H */


