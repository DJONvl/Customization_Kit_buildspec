#define STA_POWER_DOWN	0
#define STA_POWER_ON	1


#define SPM_REG(offset)			 (0x10006000 + offset)
//#define POWERON_CONFIG_EN			SPM_REG(0x0000)
//#define PWR_STATUS						SPM_REG(0x0180) 
//#define PWR_STATUS_2ND				SPM_REG(0x0184) 
//#define MD1_PWR_CON						SPM_REG(0x0320) 
//#define MDSYS_INTF_INFRA_PWR_CON       SPM_REG(0x0360)
//#define MD_EXT_BUCK_ISO 			SPM_REG(0x0390)

#define INFRACFG_REG(offset) (0x10001000 + offset)
#define INFRA_TOPAXI_PROTECTSTA1_1	INFRACFG_REG(0x0258)
#define INFRA_TOPAXI_PROTECTEN_1	INFRACFG_REG(0x0250)


#define SPM_PROJECT_CODE		0xb16

#define MD1_PROT_MASK       ((0x1<<16) | (0x1<<17) | (0x1<<18) | (0x1<<19) | (0x1<<20) | (0x1<<21) | (0x1<<28))
#define MD1_PROT_CHECK_MASK       ((0x1<<16) | (0x1<<17) | (0x1<<18) | (0x1<<19) | (0x1<<20) | (0x1<<21))
#define clk_infracfg_ao_base    (0x10001000)
#define INFRA_TOPAXI_PROTECTEN   (clk_infracfg_ao_base + 0x0220)
#define INFRA_TOPAXI_PROTECTSTA0 (clk_infracfg_ao_base + 0x0224)
#define INFRA_TOPAXI_PROTECTSTA1 (clk_infracfg_ao_base + 0x0228)

#define MD1_SRAM_PDN                     (0x1 << 8)
#define MD1_SRAM_PDN_ACK                 (0x0 << 12)
#define PWR_CLK_DIS             (1U << 4)
#define PWR_ON_2ND              (1U << 3)
#define PWR_ON                  (1U << 2)
#define PWR_ISO                 (1U << 1)
#define PWR_RST_B               (1U << 0)
#define MD1_PWR_STA_MASK		(0x1 << 0)
#define MDSYS_INTF_INFRA_PWR_STA_MASK    (0x1 << 29)
#define MDSYS_INTF_INFRA_PROT_MASK       ((0x1 << 3) \
					  |(0x1 << 4))
void MD1_sys_enable_op();
void MD1_sys_disable_op();