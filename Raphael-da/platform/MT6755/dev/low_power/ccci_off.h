
//#define sync_write32(v, a)			mt_reg_sync_writel(v, a)
#define sync_write16(v, a)			mt_reg_sync_writew(v, a)
#define sync_write8(v, a)			mt_reg_sync_writeb(v, a)

/*  */
#define MD_P_TOPSM_BASE			(0x200D0000)
#define REG_MD_P_TOPSM_RM_PWR0_CON(base)	((base)+0x0800)
#define REG_MD_P_TOPSM_RM_PWR1_CON(base)	((base)+0x0804)
#define REG_MD_P_TOPSM_RM_PWR2_CON(base)	((base)+0x0808)
#define REG_MD_P_TOPSM_RM_PWR3_CON(base)	((base)+0x080C)
#define REG_MD_P_TOPSM_RM_PWR4_CON(base)	((base)+0x0810)
#define REG_MD_P_TOPSM_RM_TMR_PWR0(base)	((base)+0x0018)
#define REG_MD_P_TOPSM_RM_TMR_PWR1(base)	((base)+0x001C)

#define MD_L1_TOPSM_BASE			(0x26070000)
#define REG_MD_L1_TOPSM_SM_TMR_PWR0(base)	((base)+0x0140)
#define REG_MD_L1_TOPSM_SM_TMR_PWR1(base)	((base)+0x0144)
#define REG_MD_L1_TOPSM_SM_TMR_PWR2(base)	((base)+0x0148)
#define REG_MD_L1_TOPSM_SM_TMR_PWR3(base)	((base)+0x014C)
#define REG_MD_L1_TOPSM_SM_TMR_PWR4(base)	((base)+0x0150)
#define pr_debug LOGD
typedef unsigned int    UINT32;
void internal_md_power_down(void);