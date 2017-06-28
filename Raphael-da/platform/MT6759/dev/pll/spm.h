#ifndef _SPM_
#define _SPM_

/* 
//XXX: only in kernel
#include <linux/kernel.h>
#include <linux/io.h>

#include <mach/mt_reg_base.h>
#include <mach/mt_irq.h>
#include <mach/sync_write.h>
*/
#include "inc/register_base.h"
//#include <typedefs.h>
//#include <sync_write.h>

/**************************************
* Config and Parameter
**************************************/
//#define SPM_BASE		SLEEP_BASE
#define SPM_IRQ0_ID		SLEEP_IRQ_BIT0_ID
#define SPM_IRQ1_ID		SLEEP_IRQ_BIT1_ID
#define SPM_IRQ2_ID		SLEEP_IRQ_BIT2_ID
#define SPM_IRQ3_ID		SLEEP_IRQ_BIT3_ID
#define SPM_IRQ4_ID		SLEEP_IRQ_BIT4_ID
#define SPM_IRQ5_ID		SLEEP_IRQ_BIT5_ID
#define SPM_IRQ6_ID		SLEEP_IRQ_BIT6_ID
#define SPM_IRQ7_ID		SLEEP_IRQ_BIT7_ID
typedef unsigned int    u32;


/**************************************
 * Define and Declare
 **************************************/
#define POWERON_CONFIG_EN              (SPM_BASE + 0x0000)
#define SPM_POWER_ON_VAL0              (SPM_BASE + 0x0004)
#define SPM_POWER_ON_VAL1              (SPM_BASE + 0x0008)
#define SPM_CLK_CON                    (SPM_BASE + 0x000C)
#define SPM_CLK_SETTLE                 (SPM_BASE + 0x0010)
#define PCM_CON0                       (SPM_BASE + 0x0018)
#define PCM_CON1                       (SPM_BASE + 0x001C)
#define PCM_IM_PTR                     (SPM_BASE + 0x0020)
#define PCM_IM_LEN                     (SPM_BASE + 0x0024)
#define PCM_REG_DATA_INI               (SPM_BASE + 0x0028)
#define PCM_PWR_IO_EN                  (SPM_BASE + 0x002C)
#define PCM_TIMER_VAL                  (SPM_BASE + 0x0030)
#define PCM_WDT_VAL                    (SPM_BASE + 0x0034)
#define PCM_IM_HOST_RW_PTR             (SPM_BASE + 0x0038)
#define PCM_IM_HOST_RW_DAT             (SPM_BASE + 0x003C)
#define PCM_EVENT_VECTOR0              (SPM_BASE + 0x0040)
#define PCM_EVENT_VECTOR1              (SPM_BASE + 0x0044)
#define PCM_EVENT_VECTOR2              (SPM_BASE + 0x0048)
#define PCM_EVENT_VECTOR3              (SPM_BASE + 0x004C)
#define PCM_EVENT_VECTOR4              (SPM_BASE + 0x0050)
#define PCM_EVENT_VECTOR5              (SPM_BASE + 0x0054)
#define PCM_EVENT_VECTOR6              (SPM_BASE + 0x0058)
#define PCM_EVENT_VECTOR7              (SPM_BASE + 0x005C)
#define PCM_EVENT_VECTOR8              (SPM_BASE + 0x0060)
#define PCM_EVENT_VECTOR9              (SPM_BASE + 0x0064)
#define PCM_EVENT_VECTOR10             (SPM_BASE + 0x0068)
#define PCM_EVENT_VECTOR11             (SPM_BASE + 0x006C)
#define PCM_EVENT_VECTOR12             (SPM_BASE + 0x0070)
#define PCM_EVENT_VECTOR13             (SPM_BASE + 0x0074)
#define PCM_EVENT_VECTOR14             (SPM_BASE + 0x0078)
#define PCM_EVENT_VECTOR15             (SPM_BASE + 0x007C)
#define PCM_EVENT_VECTOR_EN            (SPM_BASE + 0x0080)
#define SPM_MD32_IRQ                   (SPM_BASE + 0x0084)
#define SPM_SCP_IRQ                    (SPM_BASE + 0x008C)
#define SPM_TWAM_CON                   (SPM_BASE + 0x0090)
#define SPM_TWAM_WINDOW_LEN            (SPM_BASE + 0x0094)
#define SPM_TWAM_IDLE_SEL              (SPM_BASE + 0x0098)
#define SPM_CPU_WAKEUP_EVENT           (SPM_BASE + 0x009C)
#define SPM_IRQ_MASK                   (SPM_BASE + 0x00A0)
#define SPM_SRC_REQ                    (SPM_BASE + 0x00B0)
#define SPM_SRC_MASK                   (SPM_BASE + 0x00B4)
#define SPM_SRC2_MASK                  (SPM_BASE + 0x00B8)
#define SPM_SRC3_MASK                  (SPM_BASE + 0x00BC)
#define SPM_SRC4_MASK                  (SPM_BASE + 0x00C0)
#define SPM_WAKEUP_EVENT_MASK          (SPM_BASE + 0x00C4)
#define SPM_EXT_WAKEUP_EVENT_MASK      (SPM_BASE + 0x00C8)
#define MEM_CLK_CON                    (SPM_BASE + 0x00CC)
#define SCP_CLK_CON                    (SPM_BASE + 0x00D0)
#define PCM_DEBUG_CON                  (SPM_BASE + 0x00D4)
#define DDR_EN_DBC_LEN                 (SPM_BASE + 0x00D8)
#define SPM_ACK_CHK_CON                (SPM_BASE + 0x00E0)
#define SPM_ACK_CHK_TIMER_VAL          (SPM_BASE + 0x00E4)
#define SRCCLKENI2PWRAP_MASK_B         (SPM_BASE + 0x00E8)
#define SPM_DRS_CON                    (SPM_BASE + 0x00EC)
#define PCM_REG0_DATA                  (SPM_BASE + 0x0100)
#define PCM_REG1_DATA                  (SPM_BASE + 0x0104)
#define PCM_REG2_DATA                  (SPM_BASE + 0x0108)
#define PCM_REG3_DATA                  (SPM_BASE + 0x010C)
#define PCM_REG4_DATA                  (SPM_BASE + 0x0110)
#define PCM_REG5_DATA                  (SPM_BASE + 0x0114)
#define PCM_REG6_DATA                  (SPM_BASE + 0x0118)
#define PCM_REG7_DATA                  (SPM_BASE + 0x011C)
#define PCM_REG8_DATA                  (SPM_BASE + 0x0120)
#define PCM_REG9_DATA                  (SPM_BASE + 0x0124)
#define PCM_REG10_DATA                 (SPM_BASE + 0x0128)
#define PCM_REG11_DATA                 (SPM_BASE + 0x012C)
#define PCM_REG12_DATA                 (SPM_BASE + 0x0130)
#define PCM_REG13_DATA                 (SPM_BASE + 0x0134)
#define PCM_REG14_DATA                 (SPM_BASE + 0x0138)
#define PCM_REG15_DATA                 (SPM_BASE + 0x013C)
#define PCM_REG12_MASK_B_STA           (SPM_BASE + 0x0140)
#define PCM_REG12_EXT_DATA             (SPM_BASE + 0x0144)
#define PCM_REG12_EXT_MASK_B_STA       (SPM_BASE + 0x0148)
#define PCM_EVENT_REG_STA              (SPM_BASE + 0x014C)
#define PCM_TIMER_OUT                  (SPM_BASE + 0x0150)
#define PCM_WDT_OUT                    (SPM_BASE + 0x0154)
#define SPM_IRQ_STA                    (SPM_BASE + 0x0158)
#define SPM_WAKEUP_STA                 (SPM_BASE + 0x015C)
#define SPM_WAKEUP_EXT_STA             (SPM_BASE + 0x0160)
#define SPM_WAKEUP_MISC                (SPM_BASE + 0x0164)
#define SPM_WAKEUP_MISC2               (SPM_BASE + 0x0168)
#define BUS_PROTECT_RDY                (SPM_BASE + 0x016C)
#define BUS_PROTECT2_RDY               (SPM_BASE + 0x0170)
#define BUS_PROTECT3_RDY               (SPM_BASE + 0x0174)
#define BUS_PROTECT4_RDY               (SPM_BASE + 0x0178)
#define BUS_PROTECT5_RDY               (SPM_BASE + 0x017C)
#define SUBSYS_IDLE_STA                (SPM_BASE + 0x0180)
#define CPU_IDLE_STA                   (SPM_BASE + 0x0184)
#define CPU_IDLE_STA_AFT_SEL           (SPM_BASE + 0x0188)
#define PCM_FSM_STA                    (SPM_BASE + 0x018C)
#define PWR_STATUS                     (SPM_BASE + 0x0190)
#define PWR_STATUS_2ND                 (SPM_BASE + 0x0194)
#define CPU_PWR_STATUS                 (SPM_BASE + 0x0198)
#define CPU_PWR_STATUS_2ND             (SPM_BASE + 0x019C)
#define PCM_WDT_LATCH_0                (SPM_BASE + 0x01A0)
#define PCM_WDT_LATCH_1                (SPM_BASE + 0x01A4)
#define PCM_WDT_LATCH_2                (SPM_BASE + 0x01A8)
#define PCM_WDT_LATCH_3                (SPM_BASE + 0x01AC)
#define PCM_WDT_LATCH_4                (SPM_BASE + 0x01B0)
#define PCM_WDT_LATCH_5                (SPM_BASE + 0x01B4)
#define PCM_WDT_LATCH_6                (SPM_BASE + 0x01B8)
#define PCM_WDT_LATCH_7                (SPM_BASE + 0x01BC)
#define PCM_WDT_LATCH_8                (SPM_BASE + 0x01C0)
#define PCM_WDT_LATCH_9                (SPM_BASE + 0x01C4)
#define SPM_TWAM_LAST_STA0             (SPM_BASE + 0x01C8)
#define SPM_TWAM_LAST_STA1             (SPM_BASE + 0x01CC)
#define SPM_TWAM_LAST_STA2             (SPM_BASE + 0x01D0)
#define SPM_TWAM_LAST_STA3             (SPM_BASE + 0x01D4)
#define SPM_TWAM_CURR_STA0             (SPM_BASE + 0x01D8)
#define SPM_TWAM_CURR_STA1             (SPM_BASE + 0x01DC)
#define SPM_TWAM_CURR_STA2             (SPM_BASE + 0x01E0)
#define SPM_TWAM_CURR_STA3             (SPM_BASE + 0x01E4)
#define SPM_TWAM_TIMER_OUT             (SPM_BASE + 0x01E8)
#define SPM_SRC_RDY_STA                (SPM_BASE + 0x01EC)
#define CPU_RET_STATUS                 (SPM_BASE + 0x01F0)
#define SPM_ACK_CHK_TIMER              (SPM_BASE + 0x01F4)
#define SPM_ACK_CHK_STA                (SPM_BASE + 0x01F8)
#define MCU0_PWR_CON                   (SPM_BASE + 0x0200)
#define MCU1_PWR_CON                   (SPM_BASE + 0x0204)
#define MCU2_PWR_CON                   (SPM_BASE + 0x0208)
#define MCU3_PWR_CON                   (SPM_BASE + 0x020C)
#define MCU4_PWR_CON                   (SPM_BASE + 0x0210)
#define MCU5_PWR_CON                   (SPM_BASE + 0x0214)
#define MCU6_PWR_CON                   (SPM_BASE + 0x0218)
#define MCU7_PWR_CON                   (SPM_BASE + 0x021C)
#define MCU8_PWR_CON                   (SPM_BASE + 0x0220)
#define MCU9_PWR_CON                   (SPM_BASE + 0x0224)
#define MCU10_PWR_CON                  (SPM_BASE + 0x0228)
#define MCU11_PWR_CON                  (SPM_BASE + 0x022C)
#define MCU12_PWR_CON                  (SPM_BASE + 0x0230)
#define MCU13_PWR_CON                  (SPM_BASE + 0x0234)
#define MCU14_PWR_CON                  (SPM_BASE + 0x0238)
#define MCU15_PWR_CON                  (SPM_BASE + 0x023C)
#define MCU16_PWR_CON                  (SPM_BASE + 0x0240)
#define MCU17_PWR_CON                  (SPM_BASE + 0x0244)
#define CPU_EXT_BUCK_ISO               (SPM_BASE + 0x0260)
#define ARMPLL_CLK_CON                 (SPM_BASE + 0x0264)
#define SPMC_DORMANT_ENABLE            (SPM_BASE + 0x0268)
#define BYPASS_SPMC                    (SPM_BASE + 0x026C)
#define CPU_SRAM_DREQ_CON              (SPM_BASE + 0x0270)
#define CPU_SRAM_DREQ_CON_SET          (SPM_BASE + 0x0274)
#define CPU_SRAM_DREQ_CON_CLR          (SPM_BASE + 0x0278)
#define CPU_SRAM_DREQ_ACK              (SPM_BASE + 0x027C)
#define SPMC_MISC                      (SPM_BASE + 0x0280)
#define SPMC_MISC2                     (SPM_BASE + 0x0284)
#define COMMON_CPU_PWR_ADDR            (SPM_BASE + 0x0288)
#define COMMON_TOP_PWR_ADDR            (SPM_BASE + 0x028C)
#define MCU2_L2_FLUSH                  (SPM_BASE + 0x0290)
#define MCU7_L2_FLUSH                  (SPM_BASE + 0x0294)
#define MCU12_L2_FLUSH                 (SPM_BASE + 0x0298)
#define MFG0_PWR_CON                   (SPM_BASE + 0x0300)
#define MFG1_PWR_CON                   (SPM_BASE + 0x0304)
#define MFG2_PWR_CON                   (SPM_BASE + 0x0308)
#define MFG3_PWR_CON                   (SPM_BASE + 0x030C)
#define MFG4_PWR_CON                   (SPM_BASE + 0x0310)
#define C2K_PWR_CON                    (SPM_BASE + 0x0314)
#define MD1_PWR_CON                    (SPM_BASE + 0x0318)
#define DPY_CH0_PWR_CON                (SPM_BASE + 0x031C)
#define DPY_CH1_PWR_CON                (SPM_BASE + 0x0320)
#define CONN_PWR_CON                   (SPM_BASE + 0x0324)
#define INFRA_PWR_CON                  (SPM_BASE + 0x0328)
#define AUD_PWR_CON                    (SPM_BASE + 0x032C)
#define MJC_PWR_CON                    (SPM_BASE + 0x0330)
#define MM0_PWR_CON                    (SPM_BASE + 0x0334)
#define CAM_PWR_CON                    (SPM_BASE + 0x0338)
#define IPU_PWR_CON                    (SPM_BASE + 0x033C)
#define ISP_PWR_CON                    (SPM_BASE + 0x0340)
#define VEN_PWR_CON                    (SPM_BASE + 0x0344)
#define VDE_PWR_CON                    (SPM_BASE + 0x0348)
#define DUMMY0_PWR_CON                 (SPM_BASE + 0x034C)
#define DUMMY1_PWR_CON                 (SPM_BASE + 0x0350)
#define DUMMY2_PWR_CON                 (SPM_BASE + 0x0354)
#define DUMMY3_PWR_CON                 (SPM_BASE + 0x0358)
#define PERI_SRAM_CON                  (SPM_BASE + 0x0360)
#define SYSRAM_0_CON                   (SPM_BASE + 0x0370)
#define SYSRAM_1_CON                   (SPM_BASE + 0x0374)
#define SYSROM_CON                     (SPM_BASE + 0x0378)
#define SCP_SRAM_CON                   (SPM_BASE + 0x037C)
#define PMCU_SRAM_CON                  (SPM_BASE + 0x0380)
#define BSI_TOP_SRAM_CON               (SPM_BASE + 0x0384)
#define DBGSYS_SRAM_CON                (SPM_BASE + 0x0388)
#define AUD_DSP_SRAM_CON               (SPM_BASE + 0x038C)
#define AUD_ANC_SRAM_CON               (SPM_BASE + 0x0390)
#define EXT_BUCK_ISO                   (SPM_BASE + 0x0394)
#define UFS_SRAM_CON                   (SPM_BASE + 0x0398)
#define DXCC_SRAM_CON                  (SPM_BASE + 0x039C)
#define IPU_SRAM_CON                   (SPM_BASE + 0x03A0)
#define SPARE_SRAM_CON                 (SPM_BASE + 0x03A4)
#define SPM_DVFS_CON                   (SPM_BASE + 0x0400)
#define SPM_MDBSI_CON                  (SPM_BASE + 0x0404)
#define SPM_MAS_PAUSE_MASK_B           (SPM_BASE + 0x0408)
#define SPM_MAS_PAUSE2_MASK_B          (SPM_BASE + 0x040C)
#define SPM_MAS_PAUSE3_MASK_B          (SPM_BASE + 0x0410)
#define SPM_MAS_PAUSE4_MASK_B          (SPM_BASE + 0x0414)
#define SPM_MAS_PAUSE5_MASK_B          (SPM_BASE + 0x0418)
#define SPM_BSI_GEN                    (SPM_BASE + 0x041C)
#define SPM_BSI_EN_SR                  (SPM_BASE + 0x0420)
#define SPM_BSI_CLK_SR                 (SPM_BASE + 0x0424)
#define SPM_BSI_D0_SR                  (SPM_BASE + 0x0428)
#define SPM_BSI_D1_SR                  (SPM_BASE + 0x042C)
#define SPM_BSI_D2_SR                  (SPM_BASE + 0x0430)
#define AP2MD_CROSS_TRIGGER            (SPM_BASE + 0x0434)
#define AP_MDSRC_REQ                   (SPM_BASE + 0x0438)
#define SPM2MD_DVFS_CON                (SPM_BASE + 0x043C)
#define MD2SPM_DVFS_CON                (SPM_BASE + 0x0440)
#define SPM_PLL_CON                    (SPM_BASE + 0x044C)
#define AP2MD_PEER_WAKEUP              (SPM_BASE + 0x0454)
#define ULPOSC_CON                     (SPM_BASE + 0x0458)
#define DRAMC_DPY_CLK_SW_CON_SEL       (SPM_BASE + 0x0460)
#define DRAMC_DPY_CLK_SW_CON           (SPM_BASE + 0x0464)
#define DRAMC_DPY_CLK_SW_CON_SEL2      (SPM_BASE + 0x0468)
#define DRAMC_DPY_CLK_SW_CON2          (SPM_BASE + 0x046C)
#define DRAMC_DPY_CLK_SW_CON_SEL3      (SPM_BASE + 0x0470)
#define DRAMC_DPY_CLK_SW_CON3          (SPM_BASE + 0x0474)
#define DVFS_LEVEL                     (SPM_BASE + 0x0478)
#define MSDC_DVFS_HALT                 (SPM_BASE + 0x047C)
#define SRAM_DREQ_CON                  (SPM_BASE + 0x0480)
#define SRAM_DREQ_CON_SET              (SPM_BASE + 0x0484)
#define SRAM_DREQ_CON_CLR              (SPM_BASE + 0x0488)
#define SRAM_DREQ_ACK                  (SPM_BASE + 0x048C)
#define DRAMC_DPY_CLK_SW_CON_SEL4      (SPM_BASE + 0x0490)
#define DRAMC_DPY_CLK_SW_CON4          (SPM_BASE + 0x0494)
#define DRAMC_DPY_CLK_SPM_CON          (SPM_BASE + 0x0498)
#define DRAMC_DPY_CLK_SW_CON5          (SPM_BASE + 0x049C)
#define DRAMC_GATING_ERR               (SPM_BASE + 0x04A0)
#define MCU0_IRQ_MASK                  (SPM_BASE + 0x0500)
#define MCU1_IRQ_MASK                  (SPM_BASE + 0x0504)
#define MCU2_IRQ_MASK                  (SPM_BASE + 0x0508)
#define MCU3_IRQ_MASK                  (SPM_BASE + 0x050C)
#define MCU4_IRQ_MASK                  (SPM_BASE + 0x0510)
#define MCU5_IRQ_MASK                  (SPM_BASE + 0x0514)
#define MCU6_IRQ_MASK                  (SPM_BASE + 0x0518)
#define MCU7_IRQ_MASK                  (SPM_BASE + 0x051C)
#define MCU8_IRQ_MASK                  (SPM_BASE + 0x0520)
#define MCU9_IRQ_MASK                  (SPM_BASE + 0x0524)
#define MCU10_IRQ_MASK                 (SPM_BASE + 0x0528)
#define MCU11_IRQ_MASK                 (SPM_BASE + 0x052C)
#define MCU12_IRQ_MASK                 (SPM_BASE + 0x0530)
#define MCU13_IRQ_MASK                 (SPM_BASE + 0x0534)
#define MCU14_IRQ_MASK                 (SPM_BASE + 0x0538)
#define MCU15_IRQ_MASK                 (SPM_BASE + 0x053C)
#define MCU16_IRQ_MASK                 (SPM_BASE + 0x0540)
#define MCU17_IRQ_MASK                 (SPM_BASE + 0x0544)
#define MCU0_WFI_EN                    (SPM_BASE + 0x0548)
#define MCU1_WFI_EN                    (SPM_BASE + 0x054C)
#define MCU2_WFI_EN                    (SPM_BASE + 0x0550)
#define MCU3_WFI_EN                    (SPM_BASE + 0x0554)
#define MCU4_WFI_EN                    (SPM_BASE + 0x0558)
#define MCU5_WFI_EN                    (SPM_BASE + 0x055C)
#define MCU6_WFI_EN                    (SPM_BASE + 0x0560)
#define MCU7_WFI_EN                    (SPM_BASE + 0x0564)
#define MCU8_WFI_EN                    (SPM_BASE + 0x0568)
#define MCU9_WFI_EN                    (SPM_BASE + 0x056C)
#define MCU10_WFI_EN                   (SPM_BASE + 0x0570)
#define MCU11_WFI_EN                   (SPM_BASE + 0x0574)
#define MCU12_WFI_EN                   (SPM_BASE + 0x0578)
#define MCU13_WFI_EN                   (SPM_BASE + 0x057C)
#define MCU14_WFI_EN                   (SPM_BASE + 0x0580)
#define MCU15_WFI_EN                   (SPM_BASE + 0x0584)
#define MCU16_WFI_EN                   (SPM_BASE + 0x0588)
#define MCU17_WFI_EN                   (SPM_BASE + 0x058C)
#define SPM_SW_FLAG                    (SPM_BASE + 0x0600)
#define SPM_SW_DEBUG                   (SPM_BASE + 0x0604)
#define SPM_SW_RSV_0                   (SPM_BASE + 0x0608)
#define SPM_SW_RSV_1                   (SPM_BASE + 0x060C)
#define SPM_SW_RSV_2                   (SPM_BASE + 0x0610)
#define SPM_SW_RSV_3                   (SPM_BASE + 0x0614)
#define SPM_SW_RSV_4                   (SPM_BASE + 0x0618)
#define SPM_SW_RSV_5                   (SPM_BASE + 0x061C)
#define SPM_RSV_CON3                   (SPM_BASE + 0x0620)
#define SPM_RSV_CON2                   (SPM_BASE + 0x0624)
#define SPM_RSV_STA                    (SPM_BASE + 0x0628)
#define SPM_RSV_STA2                   (SPM_BASE + 0x062C)
#define SPM_PASR_DPD_0                 (SPM_BASE + 0x0630)
#define SPM_PASR_DPD_1                 (SPM_BASE + 0x0634)
#define SPM_PASR_DPD_2                 (SPM_BASE + 0x0638)
#define SPM_PASR_DPD_3                 (SPM_BASE + 0x063C)
#define SPM_SPARE_CON                  (SPM_BASE + 0x0640)
#define SPM_SPARE_CON_SET              (SPM_BASE + 0x0644)
#define SPM_SPARE_CON_CLR              (SPM_BASE + 0x0648)
#define SPM_C2K_DEBUG                  (SPM_BASE + 0x064C)
#define SPM_DEBUG_OUT_STA              (SPM_BASE + 0x0650)
#define SPM_CCIF_STA                   (SPM_BASE + 0x0654)
#define SPM_PC_TRACE_G0                (SPM_BASE + 0x0658)
#define SPM_PC_TRACE_G1                (SPM_BASE + 0x065C)
#define SPM_PC_TRACE_G2                (SPM_BASE + 0x0660)
#define SPM_PC_TRACE_G3                (SPM_BASE + 0x0664)
#define SPM_PC_TRACE_G4                (SPM_BASE + 0x0668)
#define SPM_PC_TRACE_G5                (SPM_BASE + 0x066C)
#define SPM_PC_TRACE_G6                (SPM_BASE + 0x0670)
#define SPM_PC_TRACE_G7                (SPM_BASE + 0x0674)
#define SPM_PC_TRACE_CON               (SPM_BASE + 0x0678)
#define PCM_WDT_LATCH_10               (SPM_BASE + 0x067C)
#define PCM_WDT_LATCH_11               (SPM_BASE + 0x0680)
#define DCHA_GATING_LATCH_0            (SPM_BASE + 0x0684)
#define DCHA_GATING_LATCH_1            (SPM_BASE + 0x0688)
#define DCHA_GATING_LATCH_2            (SPM_BASE + 0x068C)
#define DCHA_GATING_LATCH_3            (SPM_BASE + 0x0690)
#define DCHA_GATING_LATCH_4            (SPM_BASE + 0x0694)
#define DCHA_GATING_LATCH_5            (SPM_BASE + 0x0698)
#define DCHA_GATING_LATCH_6            (SPM_BASE + 0x069C)
#define DCHA_GATING_LATCH_7            (SPM_BASE + 0x06A0)
#define DCHB_GATING_LATCH_0            (SPM_BASE + 0x06A4)
#define DCHB_GATING_LATCH_1            (SPM_BASE + 0x06A8)
#define DCHB_GATING_LATCH_2            (SPM_BASE + 0x06AC)
#define DCHB_GATING_LATCH_3            (SPM_BASE + 0x06B0)
#define DCHB_GATING_LATCH_4            (SPM_BASE + 0x06B4)
#define DCHB_GATING_LATCH_5            (SPM_BASE + 0x06B8)
#define DCHB_GATING_LATCH_6            (SPM_BASE + 0x06BC)
#define DCHB_GATING_LATCH_7            (SPM_BASE + 0x06C0)
#define SPM_SW_RSV_6                   (SPM_BASE + 0x06C4)
#define SPM_SW_RSV_7                   (SPM_BASE + 0x06C8)
#define SPM_SW_RSV_8                   (SPM_BASE + 0x06CC)
#define PCM_WDT_LATCH_12               (SPM_BASE + 0x06D0)
#define PCM_WDT_LATCH_13               (SPM_BASE + 0x06D4)
#define PCM_WDT_LATCH_14               (SPM_BASE + 0x06D8)
#define SPM_DVFS_INITIAL_MASK          (SPM_BASE + 0x0700)
#define SPM_DVFS_INITIAL_MASK_SET      (SPM_BASE + 0x0704)
#define SPM_DVFS_INITIAL_MASK_CLR      (SPM_BASE + 0x0708)
#define SPM_DVFS_FLOOR_MASK            (SPM_BASE + 0x070C)
#define SPM_DVFS_FLOOR_MASK_SET        (SPM_BASE + 0x0710)
#define SPM_DVFS_FLOOR_MASK_CLR        (SPM_BASE + 0x0714)
#define SPM_DVFS_FLOOR_MASK2           (SPM_BASE + 0x0718)
#define SPM_DVFS_FLOOR_MASK2_SET       (SPM_BASE + 0x071C)
#define SPM_DVFS_FLOOR_MASK2_CLR       (SPM_BASE + 0x0720)
#define SPM_DVFS_FLOOR_MASK3           (SPM_BASE + 0x0724)
#define SPM_DVFS_FLOOR_MASK3_SET       (SPM_BASE + 0x0728)
#define SPM_DVFS_FLOOR_MASK3_CLR       (SPM_BASE + 0x072C)
#define SPM_DVFS_FLOOR_MASK4           (SPM_BASE + 0x0730)
#define SPM_DVFS_FLOOR_MASK4_SET       (SPM_BASE + 0x0734)
#define SPM_DVFS_FLOOR_MASK4_CLR       (SPM_BASE + 0x0738)
#define SPM_DVFS_FORCE_MASK0           (SPM_BASE + 0x073C)
#define SPM_DVFS_FORCE_MASK0_SET       (SPM_BASE + 0x0740)
#define SPM_DVFS_FORCE_MASK0_CLR       (SPM_BASE + 0x0744)
#define SPM_DVFS_CNFG                  (SPM_BASE + 0x0748)
#define SPM_AP_SCENARIO                (SPM_BASE + 0x074C)
#define SPM_MD_DVFS_INITIAL            (SPM_BASE + 0x0750)
#define SPM_MD_DVFS_INITIAL2           (SPM_BASE + 0x0754)
#define SPM_DVFS_FORCE                 (SPM_BASE + 0x0758)
#define SPM_DVFS_FORCE_SET             (SPM_BASE + 0x075C)
#define SPM_DVFS_FORCE_CLR             (SPM_BASE + 0x0760)
#define SPM_DISP_CAM_BW_LEVEL          (SPM_BASE + 0x0764)
#define SPM_DISP_CAM_BW                (SPM_BASE + 0x0768)
#define SPM_MM_LARB1_BW_LEVEL          (SPM_BASE + 0x076C)
#define SPM_MM_LARB2_BW_LEVEL          (SPM_BASE + 0x0770)
#define SPM_DVFS_INITIAL_SETTING       (SPM_BASE + 0x0774)
#define SPM_DVFS_SW_FLOOR0             (SPM_BASE + 0x0778)
#define SPM_DVFS_SW_FLOOR1             (SPM_BASE + 0x077C)
#define SPM_DVFS_SW_FLOOR2             (SPM_BASE + 0x0780)
#define SPM_DVFS_SW_FLOOR3             (SPM_BASE + 0x0784)
#define SPM_DVFS_SW_FLOOR4             (SPM_BASE + 0x0788)
#define SPM_DVFS_SW_FLOOR5             (SPM_BASE + 0x078C)
#define SPM_DVFS_SW_FLOOR6             (SPM_BASE + 0x0790)
#define SPM_DVFS_SW_FLOOR7             (SPM_BASE + 0x0794)
#define SPM_MD_DVFS_FLOOR              (SPM_BASE + 0x0798)
#define SPM_MD_DVFS_FLOOR2             (SPM_BASE + 0x079C)
#define SPM_MD_DVFS_FLOOR3             (SPM_BASE + 0x07A0)
#define SPM_MD_DVFS_FLOOR4             (SPM_BASE + 0x07A4)
#define SPM_CONN_DVFS_FLOOR            (SPM_BASE + 0x07A8)
#define SPM_C2K_DVFS_FLOOR             (SPM_BASE + 0x07AC)
#define SPM_DVFS_DEBOUNCE0             (SPM_BASE + 0x07B0)
#define SPM_DVFS_DEBOUNCE1             (SPM_BASE + 0x07B4)
#define SPM_DVFS_DEBOUNCE2             (SPM_BASE + 0x07B8)
#define SPM_DVFS_DEBOUNCE3             (SPM_BASE + 0x07BC)
#define SPM_DVFS_DEBUG                 (SPM_BASE + 0x07C0)
#define SPM_DVFS_DEBUG1                (SPM_BASE + 0x07C4)
#define SPM_DVFS_DEBUG2                (SPM_BASE + 0x07C8)
#define SPM_DVFS_DEBUG3                (SPM_BASE + 0x07CC)
#define SPM_DVFS_DEBUG4                (SPM_BASE + 0x07D0)
#define SPM_DVFS_DEBUG5                (SPM_BASE + 0x07D4)
#define SPM_DVFS_RSV0                  (SPM_BASE + 0x07D8)
#define SPM_DVFS_RSV1                  (SPM_BASE + 0x07DC)
#define SPM_DVFS_RSV2                  (SPM_BASE + 0x07E0)
#define SPM_DVFS_RSV3                  (SPM_BASE + 0x07E4)
#define SPM_DVFS_RSV_STA0              (SPM_BASE + 0x07E8)
#define SPM_DVFS_RSV_STA1              (SPM_BASE + 0x07EC)
#define SPM2PMCU_INT_SET               (SPM_BASE + 0x10C00)
#define SPM2PMCU_INT_CLR               (SPM_BASE + 0x10C04)
#define SPM2PMCU_INT                   (SPM_BASE + 0x10C08)
#define PMCU2SPM_MAILBOX_0             (SPM_BASE + 0x10C20)
#define PMCU2SPM_MAILBOX_1             (SPM_BASE + 0x10C24)
#define PMCU2SPM_MAILBOX_2             (SPM_BASE + 0x10C28)
#define PMCU2SPM_MAILBOX_3             (SPM_BASE + 0x10C2C)
#define SPM2PMCU_MAILBOX_0             (SPM_BASE + 0x10C30)
#define SPM2PMCU_MAILBOX_1             (SPM_BASE + 0x10C34)
#define SPM2PMCU_MAILBOX_2             (SPM_BASE + 0x10C38)
#define SPM2PMCU_MAILBOX_3             (SPM_BASE + 0x10C3C)
#define PMCU2SPM_INT_SET               (SPM_BASE + 0x10C40)
#define PMCU2SPM_INT_CLR               (SPM_BASE + 0x10C44)
#define PMCU2SPM_INT                   (SPM_BASE + 0x10C48)
#define AHB_HADDR_EXT                  (SPM_BASE + 0x10C4C)
#define SPM_SWINT_SET                  (SPM_BASE + 0x20E00)
#define SPM_SWINT_CLR                  (SPM_BASE + 0x20E04)
#define SPM_SWINT                      (SPM_BASE + 0x20E08)
#define SPM_SW_NONSERSV_0              (SPM_BASE + 0x20E10)
#define SPM_SW_NONSERSV_1              (SPM_BASE + 0x20E14)
#define SPM_SW_NONSERSV_2              (SPM_BASE + 0x20E18)
#define SPM_SW_NONSERSV_3              (SPM_BASE + 0x20E1C)
#define SW2SPM_MAILBOX_0               (SPM_BASE + 0x20E20)
#define SW2SPM_MAILBOX_1               (SPM_BASE + 0x20E24)
#define SW2SPM_MAILBOX_2               (SPM_BASE + 0x20E28)
#define SW2SPM_MAILBOX_3               (SPM_BASE + 0x20E2C)
#define SPM2SW_MAILBOX_0               (SPM_BASE + 0x20E30)
#define SPM2SW_MAILBOX_1               (SPM_BASE + 0x20E34)
#define SPM2SW_MAILBOX_2               (SPM_BASE + 0x20E38)
#define SPM2SW_MAILBOX_3               (SPM_BASE + 0x20E3C)
#define SW2SPM_INT_SET                 (SPM_BASE + 0x20E40)
#define SW2SPM_INT_CLR                 (SPM_BASE + 0x20E44)
#define SW2SPM_INT                     (SPM_BASE + 0x20E48)
#define SPM_SEMA_M0                    (SPM_BASE + 0x20E50)
#define SPM_SEMA_M1                    (SPM_BASE + 0x20E54)
#define SPM_SEMA_M2                    (SPM_BASE + 0x20E58)
#define SPM_SEMA_M3                    (SPM_BASE + 0x20E5C)
#define CPU_HOTPLUG_SET                (SPM_BASE + 0x20E60)
#define CPU_HOTPLUG_CLR                (SPM_BASE + 0x20E64)
#define CPU_HOTPLUG                    (SPM_BASE + 0x20E68)
#define BYPASS_MSDC_DVFS_HALT          (SPM_BASE + 0x20E6C)


#define SPM_PROJECT_CODE	0xb16

#define SPM_REGWR_EN		(1U << 0)
#define SPM_REGWR_CFG_KEY	(SPM_PROJECT_CODE << 16)

#define SPM_CPU_PDN_DIS		(1U << 0)
#define SPM_INFRA_PDN_DIS	(1U << 1)
#define SPM_DDRPHY_PDN_DIS	(1U << 2)
#define SPM_DUALVCORE_PDN_DIS	(1U << 3)
#define SPM_PASR_DIS		(1U << 4)
#define SPM_DPD_DIS		(1U << 5)
#define SPM_SODI_DIS		(1U << 6)
#define SPM_MEMPLL_RESET	(1U << 7)
#define SPM_MAINPLL_PDN_DIS	(1U << 8)
#define SPM_CPU_DVS_DIS		(1U << 9)
#define SPM_CPU_DORMANT		(1U << 10)

#define SPM_WAKE_SRC_LIST	{	\
	SPM_WAKE_SRC(0, SPM_MERGE),	/* PCM timer, TWAM or CPU */	\
	SPM_WAKE_SRC(1, NO_USE),	\
	SPM_WAKE_SRC(2, KP),		\
	SPM_WAKE_SRC(3, WDT),		\
	SPM_WAKE_SRC(4, GPT),		\
	SPM_WAKE_SRC(5, GPT_MD32),	\
	SPM_WAKE_SRC(6, EINT),		\
	SPM_WAKE_SRC(7, EINT_MD32),	\
	SPM_WAKE_SRC(8, CCIF_MD),	\
	SPM_WAKE_SRC(9, LOW_BAT),	\
	SPM_WAKE_SRC(10, MD32),		\
	SPM_WAKE_SRC(11, F26M_WAKE),	\
	SPM_WAKE_SRC(12, F26M_SLEEP),	\
	SPM_WAKE_SRC(13, PCM_WDT),	\
	SPM_WAKE_SRC(14, USB_CD),	\
	SPM_WAKE_SRC(15, USB_PDN),	\
	SPM_WAKE_SRC(16, PMIC_EINT_0),	\
	SPM_WAKE_SRC(17, PMIC_EINT_1),	\
	SPM_WAKE_SRC(18, PMIC_MD32),	\
	SPM_WAKE_SRC(19, UART0),	\
	SPM_WAKE_SRC(20, AFE),		\
	SPM_WAKE_SRC(21, THERM),	\
	SPM_WAKE_SRC(22, CIRQ),		\
	SPM_WAKE_SRC(23, AUD_MD32),	\
	SPM_WAKE_SRC(24, SYSPWREQ),	\
	SPM_WAKE_SRC(25, MD_WDT),	\
	SPM_WAKE_SRC(26, CLDMA_MD),	\
	SPM_WAKE_SRC(27, SEJ),		\
	SPM_WAKE_SRC(28, ALL_MD32),	\
	SPM_WAKE_SRC(29, CPU_IRQ),	\
	SPM_WAKE_SRC(30, APSRC_WAKE),	\
	SPM_WAKE_SRC(31, APSRC_SLEEP)	\
}

/* define WAKE_SRC_XXX */
#undef SPM_WAKE_SRC
#define SPM_WAKE_SRC(id, name)	\
	WAKE_SRC_##name = (1U << (id))
enum SPM_WAKE_SRC_LIST;

typedef enum {
	WR_NONE			= 0,
	WR_UART_BUSY		= 1,
	WR_PCM_ASSERT		= 2,
	WR_PCM_TIMER		= 3,
	WR_WAKE_SRC		= 4,
	WR_UNKNOWN		= 5,
} wake_reason_t;

struct twam_sig {
	u32 sig0;		/* signal 0: config or status */
	u32 sig1;		/* signal 1: config or status */
	u32 sig2;		/* signal 2: config or status */
	u32 sig3;		/* signal 3: config or status */
};

typedef void (*twam_handler_t)(struct twam_sig *twamsig);

/* for power management init */
extern int spm_module_init(void);

/* for ANC in talking */
extern void spm_mainpll_on_request(const char *drv_name);
extern void spm_mainpll_on_unrequest(const char *drv_name);

/* for TWAM in MET tool */
extern void spm_twam_register_handler(twam_handler_t handler);
extern void spm_twam_enable_monitor(struct twam_sig *twamsig, bool speed_mode);
extern void spm_twam_disable_monitor(void);


/**************************************
* Macro and Inline
**************************************/
/* 
//XXX: only in kernel
#define spm_read(addr)			__raw_readl(IOMEM(addr))
*/
 #define spm_read(addr)          DRV_Reg32(addr)
#define spm_write(addr, val)    DRV_WriteReg32(addr, val)

#define is_cpu_pdn(flags)		(!((flags) & SPM_CPU_PDN_DIS))
#define is_infra_pdn(flags)		(!((flags) & SPM_INFRA_PDN_DIS))
#define is_ddrphy_pdn(flags)		(!((flags) & SPM_DDRPHY_PDN_DIS))
#define is_dualvcore_pdn(flags)		(!((flags) & SPM_DUALVCORE_PDN_DIS))

#define get_high_cnt(sigsta)		((sigsta) & 0x3ff)
#define get_high_percent(sigsta)	(get_high_cnt(sigsta) * 100 / 1023)

#endif
