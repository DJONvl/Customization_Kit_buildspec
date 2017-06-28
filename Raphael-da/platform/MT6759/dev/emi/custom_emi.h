#ifndef __CUSTOM_EMI__
#define __CUSTOM_EMI__
#include "dramc_pi_api.h"
#include "emi.h"

int num_of_emi_records = 1;

#define H9HP52AECMMDBR	// Hynix LP4X 6GB eMCP, mix mode
//#define H9HP52ACPMMDAR  // Hynix LP4X 4GB eMCP, normal mode
//#define KM3H6001CMB515 // Samsung LP4X 6GB eMCP, mix mode

#ifdef H9HP52AECMMDBR
EMI_SETTINGS default_emi_setting =
{
	.sub_version = 0x1,		/* sub_version */
	.type = 0x0206,			/* TYPE */
	.id_length = 9,			/* EMMC ID/FW ID checking length */
	.fw_id_length = 0,		/* FW length */
	.ID = {0x90,0x01,0x4A,0x48,0x43,0x47,0x38,0x61,0x34,0x0,0x0,0x0,0x0,0x0,0x0,0x0},	/* NAND_EMMC_ID */
	.fw_id = {0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0},	/* FW_ID */
	.EMI_CONA_VAL =
#if CHANNEL_NUMBER==1 // signle channel
    #if DUAL_RANK_ENABLE
		0x32533054,             /* EMI_CONA_VAL */  // single channel, dual rank
    #else
		0x32503054,		/* EMI_CONA_VAL */  // single channel, single rank
    #endif
#elif CHANNEL_NUMBER==2 // 2 channel
    #if DUAL_RANK_ENABLE
		0x32533154,		/* EMI_CONA_VAL */  // dual channel, dual rank
    #else
		0x32503154,		/* EMI_CONA_VAL */  // dual channel, single rank
    #endif
#endif
	.EMI_CONH_VAL = 0x00000020,	/* EMI_CONH_VAL */
	.DRAMC_ACTIME_UNION = {
		0x05080640,		/* U 00 */
		0x2C641001,		/* U 01 */
		0x0C060201,		/* U 02 */
		0xA7070207,		/* U 03 */
		0x22776561,		/* U 04 */
		0x0C0A0C0D,		/* U 05 */
		0x00054045,		/* U 06 */
		0x00000001,		/* U 07 */
	},

	.DRAM_RANK_SIZE = {
#if CHANNEL_NUMBER==1 // signle channel
    #if DUAL_RANK_ENABLE
		0x40000000,0x80000000,
    #else
		0x40000000,0x00000000,
    #endif
#elif CHANNEL_NUMBER==2
    #if DUAL_RANK_ENABLE
		0x80000000,0x100000000,
    #else
		0x80000000,0x00000000,
    #endif
#endif
			0,0},			/* DRAM RANK SIZE */

	.EMI_CONF_VAL = 
#if CHANNEL_NUMBER==1 // signle channel
		0x00421000,		/* EMI_CONF_VAL */ // signle channel
#elif CHANNEL_NUMBER==2	// 2 channel
		0x00421000,             /* EMI_CONF_VAL */ // dual channel
#endif
	.EMI_CONJ_VAL = 0x00000000,	/* EMI_CONJ_VAL[31:30] */
	.CHN0_EMI_CONA_VAL = 
    #if DUAL_RANK_ENABLE
	        0x04003059,            /* CHN0_EMI_CONA_VAL */  // dual rank
    #else
	        0x04003058,            /* CHN0_EMI_CONA_VAL */  // single rank
    #endif
	.CHN1_EMI_CONA_VAL = 
    #if DUAL_RANK_ENABLE
		0x04003059,            /* CHN1_EMI_CONA_VAL */  // dual rank
    #else
		0x04003058,            /* CHN1_EMI_CONA_VAL */  // single rank
    #endif
	.dram_cbt_mode_extern = CBT_R0_NORMAL_R1_BYTE,		/* dram_cbt_mode_extern */
	.reserved = {0,0,0,0,0,0},		/* reserved 6 */
	.iLPDDR3_MODE_REG_5 = 0x00000006,	/* LPDDR4_MODE_REG5 */
	.PIN_MUX_TYPE = 0x0,			/* PIN_MUX_TYPE for tablet */
};
#endif

#ifdef H9HP52ACPMMDAR
EMI_SETTINGS default_emi_setting =
{
	.sub_version = 0x1,		/* sub_version */
	.type = 0x0206,			/* TYPE */
	.id_length = 9,			/* EMMC ID/FW ID checking length */
	.fw_id_length = 0,		/* FW length */
	.ID = {0x90,0x01,0x4A,0x48,0x43,0x47,0x38,0x61,0x34,0x0,0x0,0x0,0x0,0x0,0x0,0x0},	/* NAND_EMMC_ID */
	.fw_id = {0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0},	/* FW_ID */
	.EMI_CONA_VAL =
#if CHANNEL_NUMBER==1 // signle channel
    #if DUAL_RANK_ENABLE
		0xF053F054,             /* EMI_CONA_VAL */  // single channel, dual rank
    #else
		0xF050F054,		/* EMI_CONA_VAL */  // single channel, single rank
    #endif
#elif CHANNEL_NUMBER==2 // 2 channel
    #if DUAL_RANK_ENABLE
		0xF053F154,		/* EMI_CONA_VAL */  // dual channel, dual rank
    #else
		0xF050F154,		/* EMI_CONA_VAL */  // dual channel, single rank
    #endif
#endif
	.EMI_CONH_VAL = 0x00000000,	/* EMI_CONH_VAL */
	.DRAMC_ACTIME_UNION = {
		0x05080640,		/* U 00 */
		0x2C641001,		/* U 01 */
		0x0C060201,		/* U 02 */
		0x96070207,		/* U 03 */
		0x22776561,		/* U 04 */
		0x0B090C0C,		/* U 05 */
		0x00054045,		/* U 06 */
		0x00000001,		/* U 07 */
	},

	.DRAM_RANK_SIZE = {
#if CHANNEL_NUMBER==1 // signle channel
    #if DUAL_RANK_ENABLE
		0x40000000,0x40000000,
    #else
		0x40000000,0x00000000,
    #endif
#elif CHANNEL_NUMBER==2
    #if DUAL_RANK_ENABLE
		0x80000000,0x80000000,
    #else
		0x80000000,0x00000000,
    #endif
#endif
			0,0},			/* DRAM RANK SIZE */

	.EMI_CONF_VAL = 
#if CHANNEL_NUMBER==1 // signle channel
		0x00042100,		/* EMI_CONF_VAL */ // signle channel
#elif CHANNEL_NUMBER==2	// 2 channel
		0x00421000,             /* EMI_CONF_VAL */ // dual channel
#endif
	.EMI_CONJ_VAL = 0x00000000,	/* EMI_CONJ_VAL[31:30] */
	.CHN0_EMI_CONA_VAL = 
    #if DUAL_RANK_ENABLE
	        0x0400F051,            /* CHN0_EMI_CONA_VAL */  // dual rank
    #else
	        0x0400F050,            /* CHN0_EMI_CONA_VAL */  // single rank
    #endif
	.CHN1_EMI_CONA_VAL = 
    #if DUAL_RANK_ENABLE
		0x0400F051,            /* CHN1_EMI_CONA_VAL */  // dual rank
    #else
		0x0400F050,            /* CHN1_EMI_CONA_VAL */  // single rank
    #endif
	.dram_cbt_mode_extern = CBT_R0_R1_NORMAL,		/* dram_cbt_mode_extern */
	.reserved = {0,0,0,0,0,0},		/* reserved 6 */
	.iLPDDR3_MODE_REG_5 = 0x00000006,	/* LPDDR4_MODE_REG5 */
	.PIN_MUX_TYPE = 0x0,			/* PIN_MUX_TYPE for tablet */
};
#endif

#ifdef KM3H6001CMB515
EMI_SETTINGS default_emi_setting =
{
	.sub_version = 0x1,		/* sub_version */
	.type = 0x0206,			/* TYPE */
	.id_length = 9,			/* EMMC ID/FW ID checking length */
	.fw_id_length = 0,		/* FW length */
	.ID = {0x15,0x01,0x00,0x33,0x48,0x36,0x43,0x4D,0x42,0x0,0x0,0x0,0x0,0x0,0x0,0x0},	/* NAND_EMMC_ID */
	.fw_id = {0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0},	/* FW_ID */
	.EMI_CONA_VAL =
#if CHANNEL_NUMBER==1 // signle channel
    #if DUAL_RANK_ENABLE
		0x32533054,             /* EMI_CONA_VAL */  // single channel, dual rank
    #else
		0x32503054,		/* EMI_CONA_VAL */  // single channel, single rank
    #endif
#elif CHANNEL_NUMBER==2 // 2 channel
    #if DUAL_RANK_ENABLE
		0x32533154,		/* EMI_CONA_VAL */  // dual channel, dual rank
    #else
		0x32503154,		/* EMI_CONA_VAL */  // dual channel, single rank
    #endif
#endif
	.EMI_CONH_VAL = 0x00000020,	/* EMI_CONH_VAL */
	.DRAMC_ACTIME_UNION = {
		0x05080640,		/* U 00 */
		0x2C641001,		/* U 01 */
		0x0D060201,		/* U 02 */
		0xA7070208,		/* U 03 */
		0x22776561,		/* U 04 */
		0x0C0A0C0D,		/* U 05 */
		0x00054045,		/* U 06 */
		0x00000000,		/* U 07 */
	},

	.DRAM_RANK_SIZE = {
#if CHANNEL_NUMBER==1 // signle channel
    #if DUAL_RANK_ENABLE
		0x40000000,0x80000000,
    #else
		0x40000000,0x00000000,
    #endif
#elif CHANNEL_NUMBER==2
    #if DUAL_RANK_ENABLE
		0x80000000,0x100000000,
    #else
		0x80000000,0x00000000,
    #endif
#endif
			0,0},			/* DRAM RANK SIZE */

	.EMI_CONF_VAL = 
#if CHANNEL_NUMBER==1 // signle channel
		0x00042100,		/* EMI_CONF_VAL */ // signle channel
#elif CHANNEL_NUMBER==2	// 2 channel
		0x00421000,             /* EMI_CONF_VAL */ // dual channel
#endif
	.EMI_CONJ_VAL = 0x00000000,	/* EMI_CONJ_VAL[31:30] */
	.CHN0_EMI_CONA_VAL = 
    #if DUAL_RANK_ENABLE
	        0x04003059,            /* CHN0_EMI_CONA_VAL */  // dual rank
    #else
	        0x04003058,            /* CHN0_EMI_CONA_VAL */  // single rank
    #endif
	.CHN1_EMI_CONA_VAL = 
    #if DUAL_RANK_ENABLE
		0x04003059,            /* CHN1_EMI_CONA_VAL */  // dual rank
    #else
		0x04003058,            /* CHN1_EMI_CONA_VAL */  // single rank
    #endif
	.dram_cbt_mode_extern = CBT_R0_NORMAL_R1_BYTE,		/* dram_cbt_mode_extern */
	.reserved = {0,0,0,0,0,0},		/* reserved 6 */
	.iLPDDR3_MODE_REG_5 = 0x00000001,	/* LPDDR4_MODE_REG5 */
	.PIN_MUX_TYPE = 0x0,			/* PIN_MUX_TYPE for tablet */
};
#endif

EMI_SETTINGS emi_settings[] =
{

};

#endif /* __CUSTOM_EMI__ */

