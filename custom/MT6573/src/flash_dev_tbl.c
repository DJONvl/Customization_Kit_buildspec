/*******************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of MediaTek Inc. (C) 2004
*
*******************************************************************************/
#include "flash_dev_tbl.h"
#include "flash_drv_DUMMY.h"
#include "flash_drv_AMD.h"
#include "flash_drv_AM29PDL128G.h"
#include "flash_drv_INTEL.h"
#include "flash_drv_RENESAS.h"
#include "flash_drv_SHARP.h"
#include "flash_drv_SST.h"
#include "flash_drv_TOSHIBA.h"
#include "flash_drv_SAMSUNG.h"

//#include "flash_drv_SF_SST.h"
//#include "flash_drv_SF_MXIC.h"

#pragma arm section rodata = "NOR_FLASH_TABLE"

#if defined(FEATURE_NOR_SUPPORT)	
// flash table 
const NOR_Device_S g_FlashDevTbl_Internal[] = {

	//------------------------------------------------------------------------------
	// Serial flash                                                             
	//------------------------------------------------------------------------------
#if defined(BB_MT6251)

	// SST
	{ SF_SST26VF016,			{ { { &SST_TB_16     }, { &DUMMY_MAP  } }, NULL, 1, 0x00BF, 0x0026, 0x0001,      0 }, &SST_CMD_CB_BUF_PGM, NULL },
	{ SF_SST26VF032,			{ { { &SST_TB_32     }, { &DUMMY_MAP  } }, NULL, 1, 0x00BF, 0x0026, 0x0002,      0 }, &SST_CMD_CB_BUF_PGM, NULL },
	{ SF_SST26VF064,			{ { { &SST_TB_64     }, { &DUMMY_MAP  } }, NULL, 1, 0x00BF, 0x0026, 0x0003,      0 }, &SST_CMD_CB_BUF_PGM, NULL },
	{ SF_SST26WF080,			{ { { &SST_TB_8      }, { &DUMMY_MAP  } }, NULL, 1, 0x00BF, 0x0026, 0x0028,      0 }, &SST_CMD_CB_BUF_PGM, NULL },
	{ SF_SST26WF016,			{ { { &SST_TB_16     }, { &DUMMY_MAP  } }, NULL, 1, 0x00BF, 0x0026, 0x0021,      0 }, &SST_CMD_CB_BUF_PGM, NULL },
	{ SF_SST26WF032,			{ { { &SST_TB_32     }, { &DUMMY_MAP  } }, NULL, 1, 0x00BF, 0x0026, 0x0022,      0 }, &SST_CMD_CB_BUF_PGM, NULL },
	{ SF_SST26WF064,			{ { { &SST_TB_64     }, { &DUMMY_MAP  } }, NULL, 1, 0x00BF, 0x0026, 0x0023,      0 }, &SST_CMD_CB_BUF_PGM, NULL },

	// MXIC
	{ SF_MX25U8035E,			{ { { &MXIC_8        }, { &DUMMY_MAP  } }, NULL, 1, 0x00C2, 0x0025, 0x0034,      0 }, &MXIC_CMD_CB_BUF_PGM, NULL },
	{ SF_MX25U1635E,			{ { { &MXIC_16       }, { &DUMMY_MAP  } }, NULL, 1, 0x00C2, 0x0025, 0x0035,      0 }, &MXIC_CMD_CB_BUF_PGM, NULL },
	{ SF_MX25U3235E,			{ { { &MXIC_32       }, { &DUMMY_MAP  } }, NULL, 1, 0x00C2, 0x0025, 0x0036,      0 }, &MXIC_CMD_CB_BUF_PGM, NULL },

#elif defined(FEATURE_NOR_SUPPORT)

	//------------------------------------------------------------------------------
	// AMD family flash                                                             
	//------------------------------------------------------------------------------
	
	// AMD & SPANSION 
	{ NOR_AM29DL323D,			{ { { &AMD_T_32     }, { &DUMMY_MAP  } }, NULL, 1, 0x0001, 0x2250,      0,      0 }, &AMD_CMD_CB_UNLOCK_BYPASS_PGM, NULL },
	{ NOR_AM29DL322GT,			{ { { &AMD_T_32     }, { &DUMMY_MAP  } }, NULL, 1, 0x0001, 0x2255,      0,      0 }, &AMD_CMD_CB_UNLOCK_BYPASS_PGM, NULL },
	{ NOR_AM49DL3208GB,			{ { { &AMD_B_32     }, { &DUMMY_MAP  } }, NULL, 1, 0x0001, 0x227E, 0x220A, 0x2200 }, &AMD_CMD_CB_UNLOCK_BYPASS_PGM, NULL },
	{ NOR_AM49DL3208GT,			{ { { &AMD_T_32     }, { &DUMMY_MAP  } }, NULL, 1, 0x0001, 0x227E, 0x220A, 0x2201 }, &AMD_CMD_CB_UNLOCK_BYPASS_PGM, NULL },
	{ NOR_S29PL032J,			{ { { &AMD_TB_32    }, { &DUMMY_MAP  } }, NULL, 1, 0x0001, 0x227E, 0x220A, 0x2201 }, &AMD_CMD_CB_UNLOCK_BYPASS_PGM, NULL },
	{ NOR_AM29DL640D,			{ { { &AMD_TB_64    }, { &DUMMY_MAP  } }, NULL, 1, 0x0001, 0x227E, 0x2202, 0x2201 }, &AMD_CMD_CB_UNLOCK_BYPASS_PGM, NULL },
	{ NOR_AM50DL128CG,			{ { { &AMD_TB_64    }, { &AMD_TB_64  } }, NULL, 2, 0x0001, 0x227E, 0x2202, 0x2201 }, &AMD_CMD_CB_UNLOCK_BYPASS_PGM, NULL },
	{ NOR_AM29PDL128G,			{ { { &AMD_TB_128   }, { &DUMMY_MAP  } }, NULL, 1, 0x0001, 0x227E, 0x220D, 0x2200 }, &AMD_AM29PDL128G_CMD_CB_UNLOCK_BYPASS_PGM, NULL },
//	{ NOR_AM49PDL127BH,			{ { { &AMD_TB_128   }, { &DUMMY_MAP  } }, NULL, 1, 0x0001, 0x227E, 0x2220, 0x2200 }, &AMD_CMD_CB_UNLOCK_BYPASS_PGM, NULL },	--> expired to NOR_S71PL127J!!
	{ NOR_S71PL254J,			{ { { &AMD_TB_128   }, { &AMD_TB_128 } }, NULL, 2, 0x0001, 0x227E, 0x2220, 0x2200 }, &AMD_CMD_CB_UNLOCK_BYPASS_PGM, NULL },
	{ NOR_AM49PDL129BH,			{ { { &AMD_B_64     }, { &AMD_T_64   } }, NULL, 2, 0x0001, 0x227E, 0x2221, 0x2200 }, &AMD_CMD_CB_UNLOCK_BYPASS_PGM, NULL },
	{ NOR_S71AL016D_T,			{ { { &AMD_T_16     }, { &DUMMY_MAP  } }, NULL, 1, 0x0001, 0x22C4,      0,      0 }, &AMD_CMD_CB_UNLOCK_BYPASS_PGM, NULL },
	{ NOR_S71AL016D_B,			{ { { &AMD_B_16     }, { &DUMMY_MAP  } }, NULL, 1, 0x0001, 0x2249,      0,      0 }, &AMD_CMD_CB_UNLOCK_BYPASS_PGM, NULL },
	{ NOR_S71GL032R3_T,			{ { { &AMD_T_32     }, { &DUMMY_MAP  } }, NULL, 1, 0x0001, 0x227E, 0x221A, 0x2201 }, &AMD_CMD_CB_UNLOCK_BYPASS_PGM, NULL },
	{ NOR_S71GL032R4_B,			{ { { &AMD_B_32     }, { &DUMMY_MAP  } }, NULL, 1, 0x0001, 0x227E, 0x221A, 0x2200 }, &AMD_CMD_CB_UNLOCK_BYPASS_PGM, NULL },
	{ NOR_S71GL032R1R2,			{ { { &AMD_32       }, { &DUMMY_MAP  } }, NULL, 1, 0x0001, 0x227E, 0x221D, 0x2200 }, &AMD_CMD_CB_UNLOCK_BYPASS_PGM, NULL },
	// SPANSION (MirrorBit Family) 
	//- PL-N
	{ NOR_S71PL127N,			{ { { &AMD_MB_TB_128 }, { &DUMMY_MAP     } }, &SPANSION_OTP_128W_SECSI, 1, 0x0001, 0x227E, 0x2220, 0x2200 }, &AMD_CMD_CB_MIRRORBIT_BUF_PGM, &AMD_OTP_CMD_CB },
	{ NOR_S71PL127J,			{ { { &AMD_TB_128    }, { &DUMMY_MAP     } }, &SPANSION_OTP_128W_SECSI, 1, 0x0001, 0x227E, 0x2220, 0x2200 }, &AMD_CMD_CB_UNLOCK_BYPASS_PGM, &AMD_OTP_CMD_CB },
	{ NOR_S71PL129N,			{ { { &AMD_MB_B_64   }, { &AMD_MB_T2_64  } }, &SPANSION_OTP_128W_SECSI, 2, 0x0001, 0x227E, 0x2221, 0x2200 }, &AMD_CMD_CB_MIRRORBIT_BUF_PGM, &AMD_OTP_CMD_CB },
	{ NOR_S71PL256N,			{ { { &AMD_MB_TB_256 }, { &DUMMY_MAP     } }, &SPANSION_OTP_128W_SECSI, 1, 0x0001, 0x227E, 0x223C, 0x2200 }, &AMD_CMD_CB_MIRRORBIT_BUF_PGM, &AMD_OTP_CMD_CB },
	//- NS-N
	{ NOR_S29NS064N,			{ { { &AMD_MB_T_64	 }, { &DUMMY_MAP     } }, &SPANSION_OTP_128W_SECSI, 1, 0x0001, 0x2B7E, 0x2B33, 0x2B00 }, &AMD_CMD_CB_S29N_MIRRORBIT_BUF_PGM, &AMD_OTP_CMD_CB },
	{ NOR_S29NS128N,			{ { { &AMD_MB_T2_128 }, { &DUMMY_MAP     } }, &SPANSION_OTP_128W_SECSI, 1, 0x0001, 0x2C7E, 0x2C35, 0x2C00 }, &AMD_CMD_CB_S29N_MIRRORBIT_BUF_PGM, &AMD_OTP_CMD_CB },
	{ NOR_S29NS256N,			{ { { &AMD_MB_T2_256 }, { &DUMMY_MAP     } }, &SPANSION_OTP_128W_SECSI, 1, 0x0001, 0x2D7E, 0x2D2F, 0x2D00 }, &AMD_CMD_CB_S29N_MIRRORBIT_BUF_PGM, &AMD_OTP_CMD_CB },
	//- NS-J
	{ NOR_S29NS016J,			{ { { &AMD_MB_T_16	 }, { &DUMMY_MAP     } }, 					  NULL, 1, 0x0001, 0x297E, 0x2915, 0x2900 }, &AMD_CMD_CB_UNLOCK_BYPASS_PGM, NULL },
	{ NOR_S29NS032J,			{ { { &AMD_MB_T_32	 }, { &DUMMY_MAP     } }, 					  NULL, 1, 0x0001, 0x2A7E, 0x2A24, 0x2A00 }, &AMD_CMD_CB_UNLOCK_BYPASS_PGM, NULL },
	{ NOR_S29NS064J,			{ { { &AMD_MB_T_64	 }, { &DUMMY_MAP     } }, 					  NULL, 1, 0x0001, 0x277E, 0x2702, 0x2700 }, &AMD_CMD_CB_UNLOCK_BYPASS_PGM, NULL },
	{ NOR_S29NS128J,			{ { { &AMD_MB_T_128	 }, { &DUMMY_MAP     } }, 					  NULL, 1, 0x0001, 0x007E, 0x0016, 0x0000 }, &AMD_CMD_CB_UNLOCK_BYPASS_PGM, NULL },
	//- GL-N
	{ NOR_S29GL128N_T,			{ { { &AMD_128 	 	 }, { &DUMMY_MAP     } }, 					  NULL, 1, 0x0001, 0x227E, 0x2221, 0x2201 }, &AMD_CMD_CB_S29GL_MIRRORBIT_BUF_PGM, NULL },
	{ NOR_S29GL256N_T,			{ { { &AMD_256 	 	 }, { &DUMMY_MAP     } }, 					  NULL, 1, 0x0001, 0x227E, 0x2222, 0x2201 }, &AMD_CMD_CB_S29GL_MIRRORBIT_BUF_PGM, NULL },
	{ NOR_S29GL512N_T,			{ { { &AMD_512 	 	 }, { &DUMMY_MAP     } }, 					  NULL, 1, 0x0001, 0x227E, 0x2223, 0x2201 }, &AMD_CMD_CB_S29GL_MIRRORBIT_BUF_PGM, NULL },
	{ NOR_S29GL128N_B,			{ { { &AMD_128 	 	 }, { &DUMMY_MAP     } }, 					  NULL, 1, 0x0001, 0x227E, 0x0021, 0x0001 }, &AMD_CMD_CB_S29GL_MIRRORBIT_BUF_PGM, NULL },
	{ NOR_S29GL256N_B,			{ { { &AMD_256 	 	 }, { &DUMMY_MAP     } }, 					  NULL, 1, 0x0001, 0x227E, 0x0022, 0x0001 }, &AMD_CMD_CB_S29GL_MIRRORBIT_BUF_PGM, NULL },
	{ NOR_S29GL512N_B,			{ { { &AMD_512 	 	 }, { &DUMMY_MAP     } }, 					  NULL, 1, 0x0001, 0x227E, 0x0023, 0x0001 }, &AMD_CMD_CB_S29GL_MIRRORBIT_BUF_PGM, NULL },
	//- GL-A
	{ NOR_S29GL064A_T,			{ { { &AMD_T_64	 	 }, { &DUMMY_MAP     } }, 					  NULL, 1, 0x0001, 0x227E, 0x2210, 0x2201 }, &AMD_CMD_CB_UNLOCK_BYPASS_PGM, NULL },
	{ NOR_S29GL064A_B,			{ { { &AMD_B_64	 	 }, { &DUMMY_MAP     } }, 					  NULL, 1, 0x0001, 0x227E, 0x2210, 0x2200 }, &AMD_CMD_CB_UNLOCK_BYPASS_PGM, NULL },
	//- WS-N
	{ NOR_S29WS128N,			{ { { &AMD_MB_TB2_128}, { &DUMMY_MAP  	 } }, &SPANSION_OTP_128W_SECSI, 1, 0x0001, 0x227E, 0x2231, 0x2200 }, &AMD_CMD_CB_S29WS_MIRRORBIT_BUF_PGM, &AMD_OTP_CMD_CB },
	{ NOR_S29WS256N,			{ { { &AMD_MB_TB2_256}, { &DUMMY_MAP  	 } }, &SPANSION_OTP_128W_SECSI, 1, 0x0001, 0x227E, 0x2230, 0x2200 }, &AMD_CMD_CB_S29WS_MIRRORBIT_BUF_PGM, &AMD_OTP_CMD_CB },
	//- WS-P
	{ NOR_S29WS128P,			{ { { &AMD_MB_TB2_128}, { &DUMMY_MAP     } }, &SPANSION_OTP_128W_SECSI, 1, 0x0001, 0x227E, 0x2244, 0x2200 }, &AMD_CMD_CB_MIRRORBIT_BUF_PGM, &AMD_OTP_CMD_CB },
	{ NOR_S29WS256P,			{ { { &AMD_MB_TB2_256}, { &DUMMY_MAP     } }, &SPANSION_OTP_128W_SECSI, 1, 0x0001, 0x227E, 0x2242, 0x2200 }, &AMD_CMD_CB_MIRRORBIT_BUF_PGM, &AMD_OTP_CMD_CB },
	{ NOR_S29WS512P,			{ { { &AMD_MB_TB_512 }, { &DUMMY_MAP     } }, &SPANSION_OTP_128W_SECSI, 1, 0x0001, 0x227E, 0x223D, 0x2200 }, &AMD_CMD_CB_MIRRORBIT_BUF_PGM, &AMD_OTP_CMD_CB },

	//- WS-R
	{ NOR_S29WS128R_T,			{ { { &AMD_MB_T2_128 }, { &DUMMY_MAP     } }, NULL, 1, 0x0001, 0x007E, 0x0027, 0x0003 }, &AMD_CMD_CB_MIRRORBIT_V2_BUF_PGM, NULL},
	{ NOR_S29WS128R_B,			{ { { &AMD_MB_B_128  }, { &DUMMY_MAP     } }, NULL, 1, 0x0001, 0x007E, 0x0027, 0x0003 }, &AMD_CMD_CB_MIRRORBIT_V2_BUF_PGM, NULL},
	{ NOR_S29WS128R,			{ { { &AMD_128       }, { &DUMMY_MAP     } }, NULL, 1, 0x0001, 0x007E, 0x0027, 0x0003 }, &AMD_CMD_CB_MIRRORBIT_V2_BUF_PGM, NULL},

	{ NOR_S29WS256R_T,			{ { { &AMD_MB_T2_256 }, { &DUMMY_MAP     } }, NULL, 1, 0x0001, 0x007E, 0x0026, 0x0003 }, &AMD_CMD_CB_MIRRORBIT_V2_BUF_PGM, NULL},
	{ NOR_S29WS256R_B,			{ { { &AMD_MB_B_256  }, { &DUMMY_MAP     } }, NULL, 1, 0x0001, 0x007E, 0x0026, 0x0003 }, &AMD_CMD_CB_MIRRORBIT_V2_BUF_PGM, NULL},
	{ NOR_S29WS256R,			{ { { &AMD_256       }, { &DUMMY_MAP     } }, NULL, 1, 0x0001, 0x007E, 0x0026, 0x0003 }, &AMD_CMD_CB_MIRRORBIT_V2_BUF_PGM, NULL},

	{ NOR_S29WS512R_T,			{ { { &AMD_MB_T_512  }, { &DUMMY_MAP     } }, NULL, 1, 0x0001, 0x007E, 0x0025, 0x0003 }, &AMD_CMD_CB_MIRRORBIT_V2_BUF_PGM, NULL},
	{ NOR_S29WS512R_B,			{ { { &AMD_MB_B_512  }, { &DUMMY_MAP     } }, NULL, 1, 0x0001, 0x007E, 0x0025, 0x0003 }, &AMD_CMD_CB_MIRRORBIT_V2_BUF_PGM, NULL},
	{ NOR_S29WS512R,			{ { { &AMD_512       }, { &DUMMY_MAP     } }, NULL, 1, 0x0001, 0x007E, 0x0025, 0x0003 }, &AMD_CMD_CB_MIRRORBIT_V2_BUF_PGM, NULL},

	{ NOR_S29WS01GR_T,			{ { { &AMD_MB_T_1024 }, { &DUMMY_MAP     } }, NULL, 1, 0x0001, 0x007E, 0x0024, 0x0003 }, &AMD_CMD_CB_MIRRORBIT_V2_BUF_PGM, NULL},
	{ NOR_S29WS01GR_B,			{ { { &AMD_MB_B_1024 }, { &DUMMY_MAP     } }, NULL, 1, 0x0001, 0x007E, 0x0024, 0x0003 }, &AMD_CMD_CB_MIRRORBIT_V2_BUF_PGM, NULL},
	{ NOR_S29WS01GR,			{ { { &AMD_1024      }, { &DUMMY_MAP     } }, NULL, 1, 0x0001, 0x007E, 0x0024, 0x0003 }, &AMD_CMD_CB_MIRRORBIT_V2_BUF_PGM, NULL},

	//- VS-R
	{ NOR_S29VS128R_T,			{ { { &AMD_MB_T2_128 }, { &DUMMY_MAP     } }, NULL, 1, 0x0001, 0x007E, 0x0063, 0x0001 }, &AMD_CMD_CB_MIRRORBIT_V2_BUF_PGM, NULL},
	{ NOR_S29VS128R_B,			{ { { &AMD_MB_B_128  }, { &DUMMY_MAP     } }, NULL, 1, 0x0001, 0x007E, 0x0065, 0x0001 }, &AMD_CMD_CB_MIRRORBIT_V2_BUF_PGM, NULL},

	{ NOR_S29VS256R_T,			{ { { &AMD_MB_T2_256 }, { &DUMMY_MAP     } }, NULL, 1, 0x0001, 0x007E, 0x0064, 0x0001 }, &AMD_CMD_CB_MIRRORBIT_V2_BUF_PGM, NULL},
	{ NOR_S29VS256R_B,			{ { { &AMD_MB_B_256  }, { &DUMMY_MAP     } }, NULL, 1, 0x0001, 0x007E, 0x0066, 0x0001 }, &AMD_CMD_CB_MIRRORBIT_V2_BUF_PGM, NULL},

	// Fujitsu 
	{ NOR_MB84VD23280FA,		{ { { &AMD_TB_64    }, { &DUMMY_MAP  } }, NULL, 1, 0x0004, 0x227E, 0x2202, 0x2201 }, &AMD_CMD_CB_UNLOCK_BYPASS_PGM, NULL },
	{ NOR_MB84VD22280FE,		{ { { &AMD_T_32     }, { &DUMMY_MAP  } }, NULL, 1, 0x0004, 0x227E, 0x220A, 0x2201 }, &AMD_CMD_CB_UNLOCK_BYPASS_PGM, NULL },
	{ NOR_MB84VP24491HK,		{ { { &AMD_B_64     }, { &AMD_T_64   } }, NULL, 2, 0x0004, 0x227E, 0x2221, 0x2200 }, &AMD_CMD_CB_UNLOCK_BYPASS_PGM, NULL },
	{ NOR_MB84VP24581HK,		{ { { &AMD_TB_128   }, { &DUMMY_MAP  } }, NULL, 1, 0x0004, 0x227E, 0x2220, 0x2200 }, &AMD_CMD_CB_UNLOCK_BYPASS_PGM, NULL },
	// ISSI 
	{ NOR_IS75V16F128GS32,		{ { { &AMD_TB_64    }, { &AMD_TB_64  } }, NULL, 2, 0x0004, 0x227E, 0x2202, 0x2201 }, &AMD_CMD_CB_UNLOCK_BYPASS_PGM, NULL },
	// ST 
	{ NOR_ST_M29DW640D,			{ { { &AMD_TB_64    }, { &DUMMY_MAP  } }, NULL, 1, 0x0020, 0x227E, 0x2202, 0x2201 }, &AMD_CMD_CB_UNLOCK_BYPASS_PGM, NULL },
	{ NOR_ST_M74DW66500B,		{ { { &AMD_TB_64    }, { &AMD_TB_64  } }, NULL, 2, 0x0020, 0x227E, 0x2202, 0x2201 }, &AMD_CMD_CB_UNLOCK_BYPASS_PGM, NULL },
	// SAMSUNG 
	{ NOR_K5A3280YT,			{ { { &AMD_T_32     }, { &DUMMY_MAP  } }, NULL, 1, 0x00EC, 0x22A0,      0,      0 }, &AMD_CMD_CB_UNLOCK_BYPASS_PGM, NULL },
	{ NOR_K5J6316CTM,			{ { { &AMD_T_64     }, { &DUMMY_MAP  } }, NULL, 1, 0x00EC, 0x22E0,      0,      0 }, &AMD_CMD_CB_UNLOCK_BYPASS_PGM, NULL },
	{ NOR_KAD060300B,			{ { { &AMD_T_64     }, { &AMD_T_64   } }, NULL, 2, 0x00EC, 0x22E0,      0,      0 }, &AMD_CMD_CB_UNLOCK_BYPASS_PGM, NULL },
	{ NOR_K5L3316CAM,           { { { &AMD_TB_32    }, { &DUMMY_MAP  } }, NULL, 1, 0x00EC, 0x257E, 0x2503, 0x2501 }, &AMD_CMD_CB_UNLOCK_BYPASS_PGM, NULL },
	{ NOR_K5L6331CAA,           { { { &AMD_TB_64    }, { &DUMMY_MAP  } }, NULL, 1, 0x00EC, 0x257E, 0x2506, 0x2501 }, &AMD_CMD_CB_UNLOCK_BYPASS_PGM, NULL },
	{ NOR_K5L2931CAM,           { { { &AMD_B_64     }, { &AMD_T_64   } }, NULL, 2, 0x00EC, 0x257E, 0x2508, 0x2501 }, &AMD_CMD_CB_UNLOCK_BYPASS_PGM, NULL },
	{ NOR_K5L2833ATA,           { { { &AMD_T_128    }, { &DUMMY_MAP  } }, NULL, 1, 0x00EC, 0x2402,      0,      0 }, &AMD_SAMSUNG_CB_UNLOCK_BYPASS_PGM, NULL },
	{ NOR_K5L2731CAM,           { { { &AMD_TB_128   }, { &DUMMY_MAP  } }, NULL, 1, 0x00EC, 0x257E, 0x2508, 0x2501 }, &AMD_CMD_CB_UNLOCK_BYPASS_PGM, NULL },
	{ NOR_KAL5563CAM,  			{ { { &AMD_MB_TB_256}, { &DUMMY_MAP  } }, NULL, 1, 0x00EC, 0x227E, 0x2263, 0x2260 }, &AMD_CMD_CB_MIRRORBIT_BUF_PGM, NULL },
	// Winbond 
	{ NOR_W19B322TM,			{ { { &AMD_T_32     }, { &DUMMY_MAP  } }, NULL, 1, 0x00DA, 0x2210,      0,      0 }, &AMD_CMD_CB_UNLOCK_BYPASS_PGM, NULL },
	{ NOR_W19B323TM,			{ { { &AMD_T_32     }, { &DUMMY_MAP  } }, NULL, 1, 0x00DA, 0x2213,      0,      0 }, &AMD_CMD_CB_UNLOCK_BYPASS_PGM, NULL },
	{ NOR_W19B320ATB,			{ { { &AMD_B_32     }, { &DUMMY_MAP  } }, NULL, 1, 0x00DA, 0x227E, 0x220A, 0x2200 }, &AMD_CMD_CB_UNLOCK_BYPASS_PGM, NULL },
	{ NOR_W19B320ATT,			{ { { &AMD_T_32     }, { &DUMMY_MAP  } }, NULL, 1, 0x00DA, 0x227E, 0x220A, 0x2201 }, &AMD_CMD_CB_UNLOCK_BYPASS_PGM, NULL },
	// SILICON7 
	{ NOR_S7_SV7E160XT,			{ { { &AMD_T_16     }, { &DUMMY_MAP  } }, NULL, 1, 0x007F, 0x22C4,      0,      0 }, &AMD_CMD_CB_UNLOCK_BYPASS_PGM, NULL },
	{ NOR_S7_SV7E160XB,			{ { { &AMD_B_16     }, { &DUMMY_MAP  } }, NULL, 1, 0x007F, 0x2249,      0,      0 }, &AMD_CMD_CB_UNLOCK_BYPASS_PGM, NULL },
	{ NOR_S7_SV7E320XT,			{ { { &AMD_T_32     }, { &DUMMY_MAP  } }, NULL, 1, 0x007F, 0x22F6,      0,      0 }, &AMD_CMD_CB_UNLOCK_BYPASS_PGM, NULL },
	{ NOR_S7_SV7E320XB,			{ { { &AMD_B_32     }, { &DUMMY_MAP  } }, NULL, 1, 0x007F, 0x22F9,      0,      0 }, &AMD_CMD_CB_UNLOCK_BYPASS_PGM, NULL },
	// TOSHIBA 
	{ NOR_TH50VPF6782AASB,		{ { { &AMD_64       }, { &AMD_T_64   } }, 						NULL, 2, 0x0098, 0x0074,      0,      0 }, &AMD_CMD_CB_UNLOCK_BYPASS_PGM, NULL },
	{ NOR_TH50VPF6783AASB,		{ { { &AMD_B_64     }, { &AMD_64     } }, 						NULL, 2, 0x0098, 0x0084,      0,      0 }, &AMD_CMD_CB_UNLOCK_BYPASS_PGM, NULL },
	{ NOR_TH50VPF5682CDSB,		{ { { &AMD_T_64     }, { &DUMMY_MAP  } }, 						NULL, 1, 0x0098, 0x0057,      0,      0 }, &AMD_CMD_CB_UNLOCK_BYPASS_PGM, NULL },
	{ NOR_TH50VPF5683CDSB,		{ { { &AMD_B_64     }, { &DUMMY_MAP  } }, 						NULL, 1, 0x0098, 0x0058,      0,      0 }, &AMD_CMD_CB_UNLOCK_BYPASS_PGM, NULL },
	{ NOR_TV00560002DDGB,		{ { { &AMD_T_64     }, { &DUMMY_MAP  } }, 						NULL, 1, 0x0098, 0x0049,      0,      0 }, &AMD_CMD_CB_UNLOCK_BYPASS_PGM, NULL },
	{ NOR_TV00560002EDGB,		{ { { &AMD_T_64   	}, { &DUMMY_MAP  } },  &TOSHIBA_HIDDEN_ROM_T_32K, 1, 0x0098, 0x0096,      0,      0 }, &AMD_CMD_CB_UNLOCK_BYPASS_PGM, &TOSHIBA_HIDDEN_ROM_CMD_CB },
	{ NOR_TV00560003EDGB,		{ { { &AMD_B_64   	}, { &DUMMY_MAP  } }, 						NULL, 1, 0x0098, 0x0097,      0,      0 }, &AMD_CMD_CB_UNLOCK_BYPASS_PGM, NULL },
	{ NOR_TV00569002AABD,		{ { { &AMD_T_64     }, { &DUMMY_MAP  } }, 						NULL, 1, 0x0098, 0x002D,      0,      0 }, &AMD_CMD_CB_UNLOCK_BYPASS_PGM, NULL },
	{ NOR_TV00569003AABD,		{ { { &AMD_B_64     }, { &DUMMY_MAP  } }, 						NULL, 1, 0x0098, 0x002E,      0,      0 }, &AMD_CMD_CB_UNLOCK_BYPASS_PGM, NULL },
	{ NOR_TV00569002BABD,		{ { { &AMD_T_64     }, { &DUMMY_MAP  } }, 						NULL, 1, 0x0098, 0x0057,      0,      0 }, &AMD_CMD_CB_UNLOCK_BYPASS_PGM, NULL },
	{ NOR_TV00569003BABD,		{ { { &AMD_B_64     }, { &DUMMY_MAP  } }, 						NULL, 1, 0x0098, 0x0058,      0,      0 }, &AMD_CMD_CB_UNLOCK_BYPASS_PGM, NULL },
	{ NOR_TV00578002AABD,		{ { { &AMD_T_128    }, { &DUMMY_MAP  } }, 						NULL, 1, 0x0098, 0x007C,      0,      0 }, &AMD_CMD_CB_UNLOCK_BYPASS_PGM, NULL },
	{ NOR_TV00578003AABD,		{ { { &AMD_B_128    }, { &DUMMY_MAP  } }, 						NULL, 1, 0x0098, 0x0082,      0,      0 }, &AMD_CMD_CB_UNLOCK_BYPASS_PGM, NULL },
	{ NOR_TV00578002DABD,		{ { { &AMD_T_128    }, { &DUMMY_MAP  } },  &TOSHIBA_HIDDEN_ROM_T_64K, 1, 0x0098, 0x0049,      0,      0 }, &AMD_CMD_CB_UNLOCK_BYPASS_PGM, &TOSHIBA_HIDDEN_ROM_CMD_CB },
	{ NOR_TV00578003DABD,		{ { { &AMD_B_128    }, { &DUMMY_MAP  } }, 						NULL, 1, 0x0098, 0x004A,      0,      0 }, &AMD_CMD_CB_UNLOCK_BYPASS_PGM, NULL },
	{ NOR_TV0057A002AABD,		{ { { &AMD_T_128    }, { &DUMMY_MAP  } }, 						NULL, 1, 0x0098, 0x001B,      0,      0 }, &AMD_CMD_CB_UNLOCK_BYPASS_PGM, NULL },
	{ NOR_TV0057A003AABD,		{ { { &AMD_B_128    }, { &DUMMY_MAP  } }, 						NULL, 1, 0x0098, 0x001D,      0,      0 }, &AMD_CMD_CB_UNLOCK_BYPASS_PGM, NULL },
	{ NOR_TY00670002APGN,		{ { { &AMD_T2_128   }, { &DUMMY_MAP  } }, 						NULL, 1, 0x0098, 0x00E7,      0,      0 }, &AMD_CMD_CB_UNLOCK_BYPASS_PGM, NULL },
	{ NOR_TC58FVM7TDD,			{ { { &AMD_T2_128   }, { &DUMMY_MAP  } },						NULL, 1, 0x0098, 0x0003, 	  0,	  0 }, &TOSHIBA_CMD_CB_PAGE_8WORDS_PGM, NULL},
	{ NOR_TC58FVM7BDD,    		{ { { &AMD_B2_128   }, { &DUMMY_MAP  } }, 						NULL, 1, 0x0098, 0x0014,      0,      0 }, &AMD_CMD_CB_WORD_PGM, NULL },
	{ NOR_TY0068B012APGG,		{ { { &AMD_T_256    }, { &DUMMY_MAP  } }, 						NULL, 1, 0x0098, 0x006B,      0,      0 }, &TOSHIBA_CMD_CB_BUF_PGM,	 	NULL },
	{ NOR_TY0068B013APGG,		{ { { &AMD_B_256    }, { &DUMMY_MAP  } }, 						NULL, 1, 0x0098, 0x006C,      0,      0 }, &AMD_CMD_CB_UNLOCK_BYPASS_PGM, NULL },
	{ NOR_TY00689002APGN,		{ { { &AMD_T2_256   }, { &DUMMY_MAP  } }, 						NULL, 1, 0x0098, 0x0075,      0,      0 }, &AMD_CMD_CB_UNLOCK_BYPASS_PGM, NULL },
	{ NOR_TC58FYM8T7D,			{ { { &AMD_T2_256   }, { &DUMMY_MAP  } }, 						NULL, 1, 0x0098, 0x006F,      0,      0 }, &AMD_CMD_CB_WORD_PGM, NULL },
    { NOR_TC58FYM8B7D,  	 	{ { { &AMD_B2_256   }, { &DUMMY_MAP  } },                       NULL, 1, 0x0098, 0x00EF,      0,      0 }, &AMD_CMD_CB_WORD_PGM, NULL },
    { NOR_TC58FYM8T8D,		    { { { &AMD_T2_256   }, { &DUMMY_MAP  } }, 						NULL, 1, 0x0098, 0x005F,      0,      0 }, &AMD_CMD_CB_WORD_PGM, NULL },
    { NOR_TC58FYM8B8D,		    { { { &AMD_B2_256   }, { &DUMMY_MAP  } }, 						NULL, 1, 0x0098, 0x00CF,      0,      0 }, &AMD_CMD_CB_WORD_PGM, NULL },
	// SST
	{ NOR_SST34HF16x1,			{ { { &SST_16       }, { &DUMMY_MAP  } }, NULL, 1, 0x00BF, 0x734B,      0,      0 }, &SST_CMD_CB_WORD_PGM, NULL },
	{ NOR_SST34HF32x4,			{ { { &SST_32       }, { &DUMMY_MAP  } }, NULL, 1, 0x00BF, 0x7353,      0,      0 }, &SST_CMD_CB_WORD_PGM, NULL },
	{ NOR_SST32HF3241C,			{ { { &SST_32       }, { &DUMMY_MAP  } }, NULL, 1, 0x00BF, 0x235B,      0,      0 }, &SST_CMD_CB_WORD_PGM, NULL },
	// AMIC
	//- A82DL32x8T(U)
	{ NOR_A82DL3228T,       	{ { { &AMD_T_32  	}, { &DUMMY_MAP  } }, NULL, 1,  0x0037, 0x2255,     0,      0 }, &AMD_CMD_CB_UNLOCK_BYPASS_PGM, NULL},
	{ NOR_A82DL3228U,       	{ { { &AMD_B_32  	}, { &DUMMY_MAP  } }, NULL, 1,  0x0037, 0x2256,     0,      0 }, &AMD_CMD_CB_UNLOCK_BYPASS_PGM, NULL},
	{ NOR_A82DL3238T,       	{ { { &AMD_T_32  	}, { &DUMMY_MAP  } }, NULL, 1,  0x0037, 0x2250,     0,      0 }, &AMD_CMD_CB_UNLOCK_BYPASS_PGM, NULL},
	{ NOR_A82DL3238U,       	{ { { &AMD_B_32  	}, { &DUMMY_MAP  } }, NULL, 1,  0x0037, 0x2253,     0,      0 }, &AMD_CMD_CB_UNLOCK_BYPASS_PGM, NULL},
	{ NOR_A82DL3248T,       	{ { { &AMD_T_32  	}, { &DUMMY_MAP  } }, NULL, 1,  0x0037, 0x225C,     0,      0 }, &AMD_CMD_CB_UNLOCK_BYPASS_PGM, NULL},
	{ NOR_A82DL3248U,       	{ { { &AMD_B_32  	}, { &DUMMY_MAP  } }, NULL, 1,  0x0037, 0x225F,     0,      0 }, &AMD_CMD_CB_UNLOCK_BYPASS_PGM, NULL},	
	// EON
	{ NOR_EON_EN29PL032,			{ { { &AMD_TB_32    }, { &DUMMY_MAP  } }, NULL, 1, 0x007F, 0x227E, 0x220A, 0x2201 }, &AMD_CMD_CB_UNLOCK_BYPASS_PGM, NULL },
	{ NOR_EON_EN29PL064,			{ { { &AMD_TB_64    }, { &DUMMY_MAP  } }, NULL, 1, 0x007F, 0x227E, 0x2202, 0x2201 }, &AMD_CMD_CB_UNLOCK_BYPASS_PGM, NULL },	
	// ESMT (Elite Semiconductor Memory Technology Inc)
	{ NOR_FM91L03208UA,         { { { &AMD_T_32     }, { &DUMMY_MAP  } }, NULL, 1, 0x008C, 0x22F6,      0,      0 }, &AMD_CMD_CB_UNLOCK_BYPASS_PGM, NULL },
	
	//------------------------------------------------------------------------------
	// INTEL family flash                                                           
	//------------------------------------------------------------------------------

	// INTEL 
	{ NOR_INTEL_28F640L18_T,	{ { { &INTEL_L_T_64   }, { &DUMMY_MAP     } }, &INTEL_OTP_17PR , 1, 0x0089, 0x880B,      0,      0 }, &INTEL_CMD_CB_32WORD_BUF_PGM, &INTEL_OTP_CMD_CB },
	{ NOR_INTEL_28F128L18_T,	{ { { &INTEL_L_T_128  }, { &DUMMY_MAP     } }, &INTEL_OTP_17PR , 1, 0x0089, 0x880C,      0,      0 }, &INTEL_CMD_CB_32WORD_BUF_PGM, &INTEL_OTP_CMD_CB },
	{ NOR_INTEL_28F256L18_T,	{ { { &INTEL_L_T_256  }, { &DUMMY_MAP     } }, &INTEL_OTP_17PR , 1, 0x0089, 0x880D,      0,      0 }, &INTEL_CMD_CB_32WORD_BUF_PGM, &INTEL_OTP_CMD_CB },
	{ NOR_INTEL_28F640L18_B,	{ { { &INTEL_L_B_64   }, { &DUMMY_MAP     } }, &INTEL_OTP_17PR , 1, 0x0089, 0x880E,      0,      0 }, &INTEL_CMD_CB_32WORD_BUF_PGM, &INTEL_OTP_CMD_CB },
	{ NOR_INTEL_28F128L18_B,	{ { { &INTEL_L_B_128  }, { &DUMMY_MAP     } }, &INTEL_OTP_17PR , 1, 0x0089, 0x880F,      0,      0 }, &INTEL_CMD_CB_32WORD_BUF_PGM, &INTEL_OTP_CMD_CB },
	{ NOR_INTEL_28F256L18_B,	{ { { &INTEL_L_B_256  }, { &DUMMY_MAP     } }, &INTEL_OTP_17PR , 1, 0x0089, 0x8810,      0,      0 }, &INTEL_CMD_CB_32WORD_BUF_PGM, &INTEL_OTP_CMD_CB },
	{ NOR_INTEL_28F640L30_T,	{ { { &INTEL_L_T_64   }, { &DUMMY_MAP     } }, &INTEL_OTP_17PR , 1, 0x0089, 0x8811,      0,      0 }, &INTEL_CMD_CB_32WORD_BUF_PGM, &INTEL_OTP_CMD_CB },
	{ NOR_INTEL_28F128L30_T,	{ { { &INTEL_L_T_128  }, { &DUMMY_MAP     } }, &INTEL_OTP_17PR , 1, 0x0089, 0x8812,      0,      0 }, &INTEL_CMD_CB_32WORD_BUF_PGM, &INTEL_OTP_CMD_CB },
	{ NOR_INTEL_28F256L30_T,	{ { { &INTEL_L_T_256  }, { &DUMMY_MAP     } }, &INTEL_OTP_17PR , 1, 0x0089, 0x8813,      0,      0 }, &INTEL_CMD_CB_32WORD_BUF_PGM, &INTEL_OTP_CMD_CB },
	{ NOR_INTEL_28F640L30_B,	{ { { &INTEL_L_B_64   }, { &DUMMY_MAP     } }, &INTEL_OTP_17PR , 1, 0x0089, 0x8814,      0,      0 }, &INTEL_CMD_CB_32WORD_BUF_PGM, &INTEL_OTP_CMD_CB },
	{ NOR_INTEL_28F128L30_B,	{ { { &INTEL_L_B_128  }, { &DUMMY_MAP     } }, &INTEL_OTP_17PR , 1, 0x0089, 0x8815,      0,      0 }, &INTEL_CMD_CB_32WORD_BUF_PGM, &INTEL_OTP_CMD_CB },
	{ NOR_INTEL_28F256L30_B,	{ { { &INTEL_L_B_256  }, { &DUMMY_MAP     } }, &INTEL_OTP_17PR , 1, 0x0089, 0x8816,      0,      0 }, &INTEL_CMD_CB_32WORD_BUF_PGM, &INTEL_OTP_CMD_CB },
	{ NOR_INTEL_28F320W30_T,	{ { { &INTEL_W_T_32   }, { &DUMMY_MAP     } }, &INTEL_OTP_1PR  , 1, 0x0089, 0x8852,      0,      0 }, &INTEL_CMD_CB_WORD_PGM, &INTEL_OTP_CMD_CB },
	{ NOR_INTEL_28F320W30_B,	{ { { &INTEL_W_B_32   }, { &DUMMY_MAP     } }, &INTEL_OTP_1PR  , 1, 0x0089, 0x8853,      0,      0 }, &INTEL_CMD_CB_WORD_PGM, &INTEL_OTP_CMD_CB },
	{ NOR_INTEL_28F640W30_T,	{ { { &INTEL_W_T_64   }, { &DUMMY_MAP     } }, &INTEL_OTP_1PR  , 1, 0x0089, 0x8854,      0,      0 }, &INTEL_CMD_CB_WORD_PGM, &INTEL_OTP_CMD_CB },
	{ NOR_INTEL_28F640W30_B,	{ { { &INTEL_W_B_64   }, { &DUMMY_MAP     } }, &INTEL_OTP_1PR  , 1, 0x0089, 0x8855,      0,      0 }, &INTEL_CMD_CB_WORD_PGM, &INTEL_OTP_CMD_CB },
	{ NOR_INTEL_28F128W30_T,	{ { { &INTEL_W_T_128  }, { &DUMMY_MAP     } }, &INTEL_OTP_1PR  , 1, 0x0089, 0x8856,      0,      0 }, &INTEL_CMD_CB_WORD_PGM, &INTEL_OTP_CMD_CB },
	{ NOR_INTEL_28F128W30_B,	{ { { &INTEL_W_B_128  }, { &DUMMY_MAP     } }, &INTEL_OTP_1PR  , 1, 0x0089, 0x8857,      0,      0 }, &INTEL_CMD_CB_WORD_PGM, &INTEL_OTP_CMD_CB },
	{ NOR_INTEL_28F320W18_T,	{ { { &INTEL_W_T_32   }, { &DUMMY_MAP     } }, &INTEL_OTP_1PR  , 1, 0x0089, 0x8862,      0,      0 }, &INTEL_CMD_CB_WORD_PGM, &INTEL_OTP_CMD_CB },
	{ NOR_INTEL_28F320W18_B,	{ { { &INTEL_W_B_32   }, { &DUMMY_MAP     } }, &INTEL_OTP_1PR  , 1, 0x0089, 0x8863,      0,      0 }, &INTEL_CMD_CB_WORD_PGM, &INTEL_OTP_CMD_CB },
	{ NOR_INTEL_28F640W18_T,	{ { { &INTEL_W_T_64   }, { &DUMMY_MAP     } }, &INTEL_OTP_1PR  , 1, 0x0089, 0x8864,      0,      0 }, &INTEL_CMD_CB_WORD_PGM, &INTEL_OTP_CMD_CB },
	{ NOR_INTEL_28F640W18_B,	{ { { &INTEL_W_B_64   }, { &DUMMY_MAP     } }, &INTEL_OTP_1PR  , 1, 0x0089, 0x8865,      0,      0 }, &INTEL_CMD_CB_WORD_PGM, &INTEL_OTP_CMD_CB },
	{ NOR_INTEL_28F128W18_T,	{ { { &INTEL_W_T_128  }, { &DUMMY_MAP     } }, &INTEL_OTP_1PR  , 1, 0x0089, 0x8866,      0,      0 }, &INTEL_CMD_CB_WORD_PGM, &INTEL_OTP_CMD_CB },
	{ NOR_INTEL_28F128W18_B,	{ { { &INTEL_W_B_128  }, { &DUMMY_MAP     } }, &INTEL_OTP_1PR  , 1, 0x0089, 0x8867,      0,      0 }, &INTEL_CMD_CB_WORD_PGM, &INTEL_OTP_CMD_CB },
	{ NOR_INTEL_38F1010C0ZBL0,	{ { { &INTEL_W_B_32   }, { &DUMMY_MAP     } }, &INTEL_OTP_1PR  , 1, 0x0089, 0x88C5,      0,      0 }, &INTEL_CMD_CB_WORD_PGM, &INTEL_OTP_CMD_CB },
	{ NOR_INTEL_28F1602C3BD70,	{ { { &INTEL_W_B_16   }, { &DUMMY_MAP     } }, &INTEL_OTP_1PR  , 1, 0x0089, 0x88C3,      0,      0 }, &INTEL_CMD_CB_WORD_PGM, &INTEL_OTP_CMD_CB },
	{ NOR_INTEL_28F3204C3TD70,	{ { { &INTEL_W_T_32   }, { &DUMMY_MAP     } }, &INTEL_OTP_1PR  , 1, 0x0089, 0x88C4,      0,      0 }, &INTEL_CMD_CB_WORD_PGM, &INTEL_OTP_CMD_CB },
	// INTEL M18 Family (Sibley/Woodmere) 
	{ NOR_INTEL_PF38F30xxM0x0xx,	{ { { &INTEL_M18_128  }, { &DUMMY_MAP } }, &INTEL_OTP_17PR , 1, 0x0089, 0x8900,	   0,	   0 }, &INTEL_CMD_CB_SIBLEY_BUF_PGM, &INTEL_OTP_CMD_CB },	//Non-Mux
	{ NOR_INTEL_PF38F30xxM0x1xx,	{ { { &INTEL_M18_128  }, { &DUMMY_MAP } }, &INTEL_OTP_17PR , 1, 0x0089, 0x8903,	   0,	   0 }, &INTEL_CMD_CB_SIBLEY_BUF_PGM, &INTEL_OTP_CMD_CB },	//Ad-Mux	
	{ NOR_INTEL_PF48F40xxM0x0xx,	{ { { &INTEL_M18_256  }, { &DUMMY_MAP } }, &INTEL_OTP_17PR , 1, 0x0089, 0x8901,	   0,	   0 }, &INTEL_CMD_CB_SIBLEY_BUF_PGM, &INTEL_OTP_CMD_CB },	//Non-Mux
	{ NOR_INTEL_PF48F40xxM0x1xx,	{ { { &INTEL_M18_256  }, { &DUMMY_MAP } }, &INTEL_OTP_17PR , 1, 0x0089, 0x8904,	   0,	   0 }, &INTEL_CMD_CB_SIBLEY_BUF_PGM, &INTEL_OTP_CMD_CB },	//AD-Mux
	{ NOR_INTEL_PF48F50xxM0x0xx,	{ { { &INTEL_M18_512  }, { &DUMMY_MAP } }, &INTEL_OTP_17PR , 1, 0x0089, 0x887E,	   0,	   0 }, &INTEL_CMD_CB_SIBLEY_BUF_PGM, &INTEL_OTP_CMD_CB },	//Non-Mux
	{ NOR_INTEL_PF48F50xxM0x1xx,	{ { { &INTEL_M18_512  }, { &DUMMY_MAP } }, &INTEL_OTP_17PR , 1, 0x0089, 0x8881,	   0,	   0 }, &INTEL_CMD_CB_SIBLEY_BUF_PGM, &INTEL_OTP_CMD_CB },	//AD-Mux
	{ NOR_INTEL_PF38F60xxM0x0xx,	{ { { &INTEL_M18_1024 }, { &DUMMY_MAP } }, &INTEL_OTP_17PR , 1, 0x0089, 0x88B0,	   0,	   0 }, &INTEL_CMD_CB_SIBLEY_BUF_PGM, &INTEL_OTP_CMD_CB },	//Non-Mux
	{ NOR_INTEL_PF38F60xxM0x1xx,	{ { { &INTEL_M18_1024 }, { &DUMMY_MAP } }, &INTEL_OTP_17PR , 1, 0x0089, 0x88B1,	   0,	   0 }, &INTEL_CMD_CB_SIBLEY_BUF_PGM, &INTEL_OTP_CMD_CB },	//AD-Mux
	// RENESAS 
	{ NOR_M6MGD13BW66CDG,		{ { { &INTEL_W_B_64   }, { &INTEL_W_T_64  } }, NULL            , 2, 0x001C, 0x00B9, 0x001C, 0x00B8 }, &RENESAS_CMD_CB_128WORD_PAGE_PGM_64Mb, NULL },
	{ NOR_M6MGB64BM34CDG,		{ { { &INTEL_W_B_64   }, { &DUMMY_MAP     } }, NULL            , 1, 0x001C, 0x00B9,      0,      0 }, &RENESAS_CMD_CB_128WORD_PAGE_PGM_64Mb, NULL },
	{ NOR_M6MGT64BM34CDG,		{ { { &INTEL_W_T_64   }, { &DUMMY_MAP     } }, NULL            , 1, 0x001C, 0x00B8,      0,      0 }, &RENESAS_CMD_CB_128WORD_PAGE_PGM_64Mb, NULL },
	{ NOR_M6MGD15BM34CDG,		{ { { &INTEL_W_TB_128 }, { &DUMMY_MAP     } }, NULL            , 1, 0x001C, 0x00C8,      0,      0 }, &RENESAS_CMD_CB_128WORD_PAGE_PGM, NULL },
	// SILICON7 
	{ NOR_S7_SV6D2832UTA,		{ { { &INTEL_W_T_128  }, { &DUMMY_MAP     } }, NULL            , 1, 0x001C, 0x00C5,      0,      0 }, &RENESAS_CMD_CB_128WORD_PAGE_PGM, NULL },
	{ NOR_S7_SV6D2832UBA,		{ { { &INTEL_W_B_128  }, { &DUMMY_MAP     } }, NULL            , 1, 0x001C, 0x00C6,      0,      0 }, &RENESAS_CMD_CB_128WORD_PAGE_PGM, NULL },
	{ NOR_S7_SV6C2832UTA,		{ { { &INTEL_W_T_128  }, { &DUMMY_MAP     } }, NULL            , 1, 0x001C, 0x00C9,      0,      0 }, &RENESAS_CMD_CB_128WORD_PAGE_PGM, NULL },
	{ NOR_S7_SV6C2832UBA,		{ { { &INTEL_W_B_128  }, { &DUMMY_MAP     } }, NULL            , 1, 0x001C, 0x00CA,      0,      0 }, &RENESAS_CMD_CB_128WORD_PAGE_PGM, NULL },
	// ST 
	{ NOR_ST_M30L0T7000T0,		{ { { &INTEL_L_T_128  }, { &DUMMY_MAP     } }, &INTEL_OTP_17PR , 1, 0x0020, 0x88C4,      0,      0 }, &INTEL_CMD_CB_32WORD_BUF_PGM, &INTEL_OTP_CMD_CB },
	{ NOR_ST_M30L0T7000B0,		{ { { &INTEL_L_B_128  }, { &DUMMY_MAP     } }, &INTEL_OTP_17PR , 1, 0x0020, 0x88C5,      0,      0 }, &INTEL_CMD_CB_32WORD_BUF_PGM, &INTEL_OTP_CMD_CB },
	{ NOR_ST_M58PPR256J,		{ { { &INTEL_M18_256  }, { &DUMMY_MAP  	  } }, NULL            , 1, 0x0020, 0x8818,      0,      0 }, &INTEL_CMD_CB_SIBLEY_BUF_PGM, NULL },
	{ NOR_ST_M58PPR512J,		{ { { &INTEL_M18_512  }, { &DUMMY_MAP  	  } }, NULL            , 1, 0x0020, 0x8819,      0,      0 }, &INTEL_CMD_CB_SIBLEY_BUF_PGM, NULL },
	{ NOR_ST_M58WR016QT,		{ { { &INTEL_W_T_16   }, { &DUMMY_MAP  	  } }, NULL            , 1, 0x0020, 0x8812,      0,      0 }, &INTEL_CMD_CB_WORD_PGM, NULL },
	{ NOR_ST_M58WR016QB,		{ { { &INTEL_W_B_16   }, { &DUMMY_MAP  	  } }, NULL            , 1, 0x0020, 0x8813,      0,      0 }, &INTEL_CMD_CB_WORD_PGM, NULL },
	{ NOR_ST_M58WR032QT,		{ { { &INTEL_W_T_32   }, { &DUMMY_MAP  	  } }, NULL            , 1, 0x0020, 0x8814,      0,      0 }, &INTEL_CMD_CB_WORD_PGM, NULL },
	{ NOR_ST_M58WR032QB,		{ { { &INTEL_W_B_32   }, { &DUMMY_MAP  	  } }, NULL            , 1, 0x0020, 0x8815,      0,      0 }, &INTEL_CMD_CB_WORD_PGM, NULL },
	{ NOR_ST_M58WR064KT,        { { { &INTEL_W_T_64   }, { &DUMMY_MAP  	  } }, NULL			   , 1, 0x0020, 0x8810,      0,      0 }, &INTEL_CMD_CB_WORD_PGM, NULL },
	{ NOR_ST_M58WR064HU,		{ { { &INTEL_W_T_64   }, { &DUMMY_MAP  	  } }, NULL            , 1, 0x0020, 0x88C0,      0,      0 }, &INTEL_CMD_CB_WORD_PGM, NULL },
	{ NOR_ST_M58WR064HL,		{ { { &INTEL_W_B_64   }, { &DUMMY_MAP  	  } }, NULL            , 1, 0x0020, 0x88C1,      0,      0 }, &INTEL_CMD_CB_WORD_PGM, NULL },
	// NUMONYX (Intel+ST)
	{ NOR_M36C0W6050T0,         { { { &INTEL_W_T_64   }, { &DUMMY_MAP  	  } }, NULL			   , 1, 0x0020, 0x8848,      0,      0 }, &INTEL_CMD_CB_WORD_PGM, NULL },
	{ NOR_M36C0W5030T0,         { { { &INTEL_W_T_32   }, { &DUMMY_MAP  	  } }, NULL			   , 1, 0x0020, 0x88BA,      0,      0 }, &INTEL_CMD_CB_WORD_PGM, NULL },
	{ NOR_M36L0T8060T3ZAQ,		{ { { &INTEL_L_T_256  }, { &DUMMY_MAP     } }, &INTEL_OTP_17PR , 1, 0x0020, 0x880D,      0,      0 }, &INTEL_CMD_CB_32WORD_BUF_PGM, &INTEL_OTP_CMD_CB },
	// SHARP 
	{ NOR_SHARP_LH28F16,		{ { { &INTEL_W_B_16   }, { &DUMMY_MAP     } }, NULL            , 1, 0x00B0, 0x00E9,      0,      0 }, &SHARP_CMD_CB_WORD_PGM_NO_UNLOCK, NULL },
	{ NOR_SHARP_LRS18R,			{ { { &SHARP_T_32	  }, { &DUMMY_MAP     } }, NULL            , 1, 0x00B0, 0x00DC,      0,      0 }, &SHARP_CMD_CB_16WORD_BUF_PGM, &INTEL_OTP_CMD_CB },
	{ NOR_SHARP_LRS1806A,		{ { { &INTEL_W_B_64   }, { &DUMMY_MAP     } }, NULL            , 1, 0x00B0, 0x00B1,      0,      0 }, &SHARP_CMD_CB_16WORD_BUF_PGM, NULL },
	{ NOR_SHARP_LRS1828C,		{ { { &INTEL_W_B_64   }, { &INTEL_W_T_64  } }, NULL            , 2, 0x00B0, 0x00B1, 0x00B0, 0x00B0 }, &SHARP_CMD_CB_16WORD_BUF_PGM, NULL },
	{ NOR_SHARP_LRS18CK,		{ { { &INTEL_L_B_64   }, { &DUMMY_MAP     } }, NULL            , 1, 0x00B0, 0x008D,      0,      0 }, &SHARP_CMD_CB_16WORD_BUF_PGM, NULL },
	{ NOR_SHARP_LRS1862,		{ { { &INTEL_W_T_128  }, { &DUMMY_MAP     } }, NULL            , 1, 0x00B0, 0x0010,      0,      0 }, &SHARP_CMD_CB_16WORD_BUF_PGM, NULL },
	{ NOR_SHARP_LRS1863,		{ { { &INTEL_W_B_128  }, { &INTEL_W_T_128 } }, NULL            , 2, 0x00B0, 0x0011, 0x00B0, 0x0010 }, &SHARP_CMD_CB_16WORD_BUF_PGM, NULL },
	{ NOR_SHARP_LRS18B0,		{ { { &INTEL_L_B_128  }, { &INTEL_L_128   } }, &INTEL_OTP_17PR , 2, 0x00B0, 0x000F,      0,      0 }, &SHARP_CMD_CB_16WORD_BUF_PGM, &INTEL_OTP_CMD_CB },
	{ NOR_SHARP_LRS18BK,		{ { { &INTEL_L_T_128  }, { &DUMMY_MAP     } }, &SHARP_OTP_128_T, 1, 0x00B0, 0x001C,      0,      0 }, &SHARP_CMD_CB_16WORD_BUF_PGM, &INTEL_OTP_CMD_CB },
	{ NOR_SHARP_LRS18C8A,		{ { { &INTEL_L_B_128  }, { &DUMMY_MAP     } }, &INTEL_OTP_17PR , 1, 0x00B0, 0x001D,      0,      0 }, &SHARP_CMD_CB_16WORD_BUF_PGM, &INTEL_OTP_CMD_CB },
	{ NOR_SHARP_LRS18D5_D7,	{ { { &INTEL_L_T_128  }, { &DUMMY_MAP     } }, NULL            , 1, 0x00B0, 0x001A,      0,      0 }, &SHARP_CMD_CB_16WORD_BUF_PGM, NULL },
	{ NOR_SHARP_LRS18A7A,	  { { { &INTEL_L_T_256  }, { &DUMMY_MAP     } }, NULL            , 1, 0x00B0, 0x000A,      0,      0 }, &SHARP_CMD_CB_16WORD_BUF_PGM, NULL },

#endif

	// <----- NOTICE!!! You can add your own flash device into whatever row before UNKNOWN_DEVICE_ENTRY. 
	
	UNKNOWN_DEVICE_ENTRY
};
#endif

#pragma arm section rodata
