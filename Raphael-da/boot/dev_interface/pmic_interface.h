#ifndef PMIC_INTERFACE_H
#define PMIC_INTERFACE_H

#include "sys/types.h"

extern void PMIC_Init();
extern status_t PMIC_Write(addr_t address, uint32 value);
extern status_t PMIC_Read(addr_t address, uint32* value);

extern void PMIC_Dump_Setting(void);

extern status_t PMIC_get_voltage(unsigned int type, unsigned int *voltage);
extern status_t PMIC_set_voltage(unsigned int type, unsigned int voltage);

extern uint32 pmic_read_interface (uint32 RegNum, uint32 *val, uint32 MASK, uint32 SHIFT);
extern uint32 pmic_config_interface (uint32 RegNum, uint32 val, uint32 MASK, uint32 SHIFT);


#endif 

