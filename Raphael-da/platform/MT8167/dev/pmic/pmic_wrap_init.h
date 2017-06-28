#ifndef __PMIC_WRAP_REGS_H__
#define __PMIC_WRAP_REGS_H__

#include "sys/types.h"
#include <debug.h>
#include <inc/register_act_util.h>
#include <inc/register_base.h>

#define PMIC_WRAP_DEBUG

#define PWRAPTAG                "[PWRAP] "
#ifdef PMIC_WRAP_DEBUG
#define DBG_MSG LOGD
#define PWRAPDEB(fmt, arg...)   DBG_MSG(PWRAPTAG fmt,##arg)
#define PWRAPFUC(fmt, arg...)   DBG_MSG(PWRAPTAG "%s\n", __FUNCTION__)
#define PWRAPLOG(fmt, arg...)   DBG_MSG(PWRAPTAG fmt,##arg)
#define PWRAPERR(fmt, arg...)   DBG_MSG(PWRAPTAG "ERROR,line=%d " fmt, __LINE__, ##arg)
#define PWRAPREG(fmt, arg...)   DBG_MSG(PWRAPTAG fmt,##arg)
#else
#define PWRAPDEB(fmt, arg...)
#define PWRAPFUC(fmt, arg...)
#define PWRAPLOG(fmt, arg...)
#define PWRAPERR(fmt, arg...)
#define PWRAPREG(fmt, arg...)
#endif

//---start ---external API--------------------------------------------------
int32 pwrap_read( uint32  adr, uint32 *rdata );
int32 pwrap_write( uint32  adr, uint32  wdata );
int32 pwrap_wacs2( uint32  write, uint32  adr, uint32  wdata, uint32 *rdata );
int32 pwrap_init ( void );
int32 pwrap_init_DA ( void );


#define read32(addr)		__raw_readl((void *)addr)
#define write32(addr,val)	mt_reg_sync_writel((val), ((void *)addr))

#define MUDI

#define MODULE_SW_CG_3_SET	(TOPCKGEN_BASE+0x02c)
#define MODULE_SW_CG_1_SET	(TOPCKGEN_BASE+0x054)
#define MODULE_SW_CG_1_CLR	(TOPCKGEN_BASE+0x084)

#define PWRAP_CG_AP		(1 << 20)
#define PWRAP_CG_MD		(1 << 27)
#define PWRAP_CG_CONN	(1 << 28)
#define PWRAP_CG_26M	(1 << 29)

#define PMIC_B_DCM_EN	(1 << 1)
#define PMIC_SPI_DCM_EN	(1 << 2)

#define CLK_CFG_5_CLR		(CKSYS_BASE+0x098)
#define CLK_SPI_CK_26M		0x1

#define PMIC_WRAP_BASE		(PWRAP_BASE)//0x1000F000
static struct MT8167_pwrap_regs * const mt8167_pwrap = (void *)PWRAP_BASE;

enum {
   WACS2 = 1 << 3
};

/* timeout setting */
enum {
   TIMEOUT_READ_US        = 255,
   TIMEOUT_WAIT_IDLE_US   = 255
};

/* PMIC_WRAP registers */
struct MT8167_pwrap_regs {
   uint32 mux_sel;
   uint32 wrap_en;
   uint32 dio_en;
   uint32 sidly;
   uint32 rddmy;
   uint32 si_ck_con;
   uint32 cshext_write;
   uint32 cshext_read;
   uint32 cslext_start;
   uint32 cslext_end;
   uint32 staupd_prd;
   uint32 staupd_grpen;
   uint32 reserved[4];
   uint32 staupd_man_trig;
   uint32 staupd_sta;
   uint32 wrap_sta;
   uint32 harb_init;
   uint32 harb_hprio;
   uint32 hiprio_arb_en;
   uint32 harb_sta0;
   uint32 harb_sta1;
   uint32 man_en;
   uint32 man_cmd;
   uint32 man_rdata;
   uint32 man_vldclr;
   uint32 wacs0_en;
   uint32 init_done0;
   uint32 wacs0_cmd;
   uint32 wacs0_rdata;
   uint32 wacs0_vldclr;
   uint32 wacs1_en;
   uint32 init_done1;
   uint32 wacs1_cmd;
   uint32 wacs1_rdata;
   uint32 wacs1_vldclr;
   uint32 wacs2_en;
   uint32 init_done2;
   uint32 wacs2_cmd;
   uint32 wacs2_rdata;
   uint32 wacs2_vldclr;
   uint32 int_en;
   uint32 int_flg_raw;
   uint32 int_flg;
   uint32 int_clr;
   uint32 sig_adr;
   uint32 sig_mode;
   uint32 sig_value;
   uint32 sig_errval;
   uint32 crc_en;
   uint32 timer_en;
   uint32 timer_sta;
   uint32 wdt_unit;
   uint32 wdt_src_en;
   uint32 wdt_flg;
   uint32 debug_int_sel;
   uint32 dvfs_adr0;
   uint32 dvfs_wdata0;
   uint32 dvfs_adr1;
   uint32 dvfs_wdata1;
   uint32 dvfs_adr2;
   uint32 dvfs_wdata2;
   uint32 dvfs_adr3;
   uint32 dvfs_wdata3;
   uint32 dvfs_adr4;
   uint32 dvfs_wdata4;
   uint32 dvfs_adr5;
   uint32 dvfs_wdata5;
   uint32 dvfs_adr6;
   uint32 dvfs_wdata6;
   uint32 dvfs_adr7;
   uint32 dvfs_wdata7;
   uint32 spminf_sta;
   uint32 cipher_key_sel;
   uint32 cipher_iv_sel;
   uint32 cipher_en;
   uint32 cipher_rdy;
   uint32 cipher_mode;
   uint32 cipher_swrst;
   uint32 dcm_en;
   uint32 dcm_dbc_prd;
   uint32 ext_ck;
   uint32 adc_cmd_addr;
   uint32 adc_cmd;
   uint32 adc_rdy_addr;
   uint32 adc_rdata_addr1;
   uint32 adc_rdata_addr2;
   uint32 gps_sta;
   uint32 swrst;
   uint32 op_type;
   uint32 msb_first;
};

enum {
   RDATA_WACS_RDATA_SHIFT = 0,
   RDATA_WACS_FSM_SHIFT   = 16,
   RDATA_WACS_REQ_SHIFT   = 19,
   RDATA_SYNC_IDLE_SHIFT,
   RDATA_INIT_DONE_SHIFT,
   RDATA_SYS_IDLE_SHIFT,
};

enum {
   RDATA_WACS_RDATA_MASK = 0xffff,
   RDATA_WACS_FSM_MASK   = 0x7,
   RDATA_WACS_REQ_MASK   = 0x1,
   RDATA_SYNC_IDLE_MASK  = 0x1,
   RDATA_INIT_DONE_MASK  = 0x1,
   RDATA_SYS_IDLE_MASK   = 0x1,
};

/* WACS_FSM */
enum {
   WACS_FSM_IDLE     = 0x00,
   WACS_FSM_REQ      = 0x02,
   WACS_FSM_WFDLE    = 0x04, /* wait for dle, wait for read data done */
   WACS_FSM_WFVLDCLR = 0x06, /* finish read data, wait for valid flag
                             * clearing */
                             WACS_INIT_DONE    = 0x01,
                             WACS_SYNC_IDLE    = 0x01,
                             WACS_SYNC_BUSY    = 0x00
};

/* dewrapper defaule value */
enum {
   DEFAULT_VALUE_READ_TEST  = 0x5aa5,
   WRITE_TEST_VALUE         = 0xa55a
};

enum pmic_regck {
   REG_CLOCK_18MHZ,
   REG_CLOCK_26MHZ,
   REG_CLOCK_SAFE_MODE
};

/* manual commnd */
enum {
   OP_WR    = 0x1,
   OP_CSH   = 0x0,
   OP_CSL   = 0x1,
   OP_OUTS  = 0x8,
   OP_OUTD  = 0x9,
   OP_INS   = 0xC,
   OP_IND   = 0xE
};

/* error information flag */
enum {
   E_PWR_INVALID_ARG             = 1,
   E_PWR_INVALID_RW              = 2,
   E_PWR_INVALID_ADDR            = 3,
   E_PWR_INVALID_WDAT            = 4,
   E_PWR_INVALID_OP_MANUAL       = 5,
   E_PWR_NOT_IDLE_STATE          = 6,
   E_PWR_NOT_INIT_DONE           = 7,
   E_PWR_NOT_INIT_DONE_READ      = 8,
   E_PWR_WAIT_IDLE_TIMEOUT       = 9,
   E_PWR_WAIT_IDLE_TIMEOUT_READ  = 10,
   E_PWR_INIT_SIDLY_FAIL         = 11,
   E_PWR_RESET_TIMEOUT           = 12,
   E_PWR_TIMEOUT                 = 13,
   E_PWR_INIT_RESET_SPI          = 20,
   E_PWR_INIT_SIDLY              = 21,
   E_PWR_INIT_REG_CLOCK          = 22,
   E_PWR_INIT_ENABLE_PMIC        = 23,
   E_PWR_INIT_DIO                = 24,
   E_PWR_INIT_CIPHER             = 25,
   E_PWR_INIT_WRITE_TEST         = 26,
   E_PWR_INIT_ENABLE_CRC         = 27,
   E_PWR_INIT_ENABLE_DEWRAP      = 28,
   E_PWR_INIT_ENABLE_EVENT       = 29,
   E_PWR_READ_TEST_FAIL          = 30,
   E_PWR_WRITE_TEST_FAIL         = 31,
   E_PWR_SWITCH_DIO              = 32
};
#endif //__PMIC_WRAP_REGS_H__


