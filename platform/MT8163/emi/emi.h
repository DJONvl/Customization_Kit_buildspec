
#ifndef _EMI_H
#define _EMI_H

#include "hw_config.h"
#include "drv_comm.h"

extern U32* uiLPDDR_PHY_Mapping_POP_CHA;

typedef volatile unsigned int *V_UINT32P;

typedef  EMI_SETTINGS_v16 EMI_SETTINGS;


#define READ_REG(REG)           DRV_Reg32(REG)
#define WRITE_REG(VAL, REG)     DRV_WriteReg32(REG,VAL)


uint32 mt_set_emis(uint8* emi, uint32 len, bool use_default); //array of emi setting.

#endif
