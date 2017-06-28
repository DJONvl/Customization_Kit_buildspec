#ifndef _SPM_MTCMOS_
#define _SPM_MTCMOS_


#define STA_POWER_DOWN  0
#define STA_POWER_ON    1

/*
 * 1. for CPU MTCMOS: CPU0, CPU1, CPU2, CPU3, DBG0, CPU4, CPU5, CPU6, CPU7, DBG1, CPUSYS1
 * 2. call spm_mtcmos_cpu_lock/unlock() before/after any operations
 */



/*
 * 1. for non-CPU MTCMOS: VDEC, VENC, ISP, DISP, MFG, INFRA, DDRPHY, MDSYS1, MDSYS2
 * 2. call spm_mtcmos_noncpu_lock/unlock() before/after any operations
 */
extern int spm_mtcmos_ctrl_vde0(int state);
extern int spm_mtcmos_ctrl_ven(int state);
extern int spm_mtcmos_ctrl_isp(int state);
extern int spm_mtcmos_ctrl_cam(int state);
extern int spm_mtcmos_ctrl_dis0t(int state);
extern int spm_mtcmos_ctrl_aud(int state);
//extern int spm_mtcmos_ctrl_ifr(int state);
extern int spm_mtcmos_ctrl_mjc(int state);
//extern int spm_mtcmos_ctrl_dpya(int state);
//extern int spm_mtcmos_ctrl_dpyb(int state);
extern int spm_mtcmos_ctrl_md1(int state);
extern int spm_mtcmos_ctrl_c2k(int state);
extern int spm_mtcmos_ctrl_conn(int state);
extern int spm_mtcmos_ctrl_mfg_async(int state);
extern int spm_mtcmos_ctrl_mfg_core0(int state);
extern int spm_mtcmos_ctrl_mfg_core1(int state);
extern int spm_mtcmos_ctrl_mfg_core2(int state);
extern int spm_mtcmos_ctrl_mfg_core3(int state);
extern int spm_mtcmos_ctrl_mfg_top(int state);

#endif
