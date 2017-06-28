#ifndef EMI_INTERFACE_H
#define EMI_INTERFACE_H

#define MAX_EMI_SETTING_NUM 10

extern  uint32 Init_Dram(uint8 emi_buf[],uint32 emi_num,uint32 m_ram_cfg_ver,uint32 emi_buf_length);
extern void Release_Dram();
extern void Detect_ExtDRAM(uint32 dram_baseaddr,uint32 *dram_size);
extern status_t Diagnose_Dram(uint32 offset, uint32 test_length, uint8 *results, uint32 result_buf_size);

#endif 

