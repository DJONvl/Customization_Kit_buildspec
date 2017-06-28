/*******************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of MediaTek Inc. (C) 2016
*
*******************************************************************************/

#ifndef _NAND_IRQ_H_
#define _NAND_IRQ_H_
#include "boot/system_objects.h"

extern void nfi_irq_handler(unsigned int irq);
extern void nfi_irq_init(void);
extern int nfi_irq_wait(uint32 tmo);

#endif
