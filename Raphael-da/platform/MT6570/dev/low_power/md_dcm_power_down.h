
#ifndef	MD_DCM_POWER_DOWN_H
#define	MD_DCM_POWER_DOWN_H

// come from emi.c
#define EMI_APB_BASE    0x10203000
#define EMI_CONM	    (EMI_APB_BASE + 0x060)
#define readl(reg)		(*(volatile unsigned int * const)(reg))




void enable_MSCI_DCM( void );
int BM_SetEmiDcm(const unsigned int setting);

#endif  //MD_DCM_POWER_DOWN_H