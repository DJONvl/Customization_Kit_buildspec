/*****************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of MediaTek Inc. (C) 2010
*
*  BY OPENING THIS FILE, BUYER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
*  THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
*  RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO BUYER ON
*  AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
*  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
*  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
*  NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
*  SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
*  SUPPLIED WITH THE MEDIATEK SOFTWARE, AND BUYER AGREES TO LOOK ONLY TO SUCH
*  THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. MEDIATEK SHALL ALSO
*  NOT BE RESPONSIBLE FOR ANY MEDIATEK SOFTWARE RELEASES MADE TO BUYER'S
*  SPECIFICATION OR TO CONFORM TO A PARTICULAR STANDARD OR OPEN FORUM.
*
*  BUYER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND CUMULATIVE
*  LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
*  AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE,
*  OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY BUYER TO
*  MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
*
*  THE TRANSACTION CONTEMPLATED HEREUNDER SHALL BE CONSTRUED IN ACCORDANCE
*  WITH THE LAWS OF THE STATE OF CALIFORNIA, USA, EXCLUDING ITS CONFLICT OF
*  LAWS PRINCIPLES.  ANY DISPUTES, CONTROVERSIES OR CLAIMS ARISING THEREOF AND
*  RELATED THERETO SHALL BE SETTLED BY ARBITRATION IN SAN FRANCISCO, CA, UNDER
*  THE RULES OF THE INTERNATIONAL CHAMBER OF COMMERCE (ICC).
*
*****************************************************************************/

#ifndef _MSDC_CFG_H_
#define _MSDC_CFG_H_
#include <autoconfig.h>
#include "inc/register_base.h"
/*--------------------------------------------------------------------------*/
/* Common Definition							    */
/*--------------------------------------------------------------------------*/

#define MMC_DEBUG		(1)
#define MSDC_DEBUG		(1)
#define MSDC_TUNE_LOG			(1)

#define MSDC_USE_REG_OPS_DUMP	(0)
#if (ENABLE_EMMC_DMA)
#define MSDC_USE_DMA_MODE		1
#else
#define MSDC_USE_DMA_MODE		0
#endif
#define MSDC_USE_EMMC45_POWER	(0)

#define MSDC0_BASE		(APB_MSDC0_BASE) //Sheet PerisysAPB, line22
#define MSDC1_BASE		(APB_MSDC1_BASE) //Sheet PerisysAPB, line23
#define MSDC2_BASE		(APB_MSDC2_BASE) //Sheet PerisysAPB, line24
#define MSDC3_BASE		(APB_MSDC3_BASE) //Sheet PerisysAPB, line24

//#define FEATURE_MMC_SDIO
//#define FEATURE_MMC_UHS1
//#define FEATURE_MMC_BOOT_MODE
#define FEATURE_MMC_RDWR_TUNING

#if MSDC_DEBUG
#define MSG_DEBUG
#endif

#endif /* end of _MSDC_CFG_H_ */
