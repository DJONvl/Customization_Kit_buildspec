/* ------------
*   Type definition.
*/

#ifndef _REGISTER_ACT_UTIL_H
#define _REGISTER_ACT_UTIL_H


#define READ_REGISTER_UINT32(reg) \
   (*(volatile unsigned int * const)(reg))

#define WRITE_REGISTER_UINT32(reg, val) \
   (*(volatile unsigned int * const)(reg)) = (val)

#define READ_REGISTER_UINT16(reg) \
   (*(volatile unsigned short * const)(reg))

#define WRITE_REGISTER_UINT16(reg, val) \
   (*(volatile unsigned short * const)(reg)) = (val)

#define READ_REGISTER_UINT8(reg) \
   (*(volatile unsigned char * const)(reg))

#define WRITE_REGISTER_UINT8(reg, val) \
   (*(volatile unsigned char * const)(reg)) = (val)

#define INREG8(x)           READ_REGISTER_UINT8((unsigned char *)(x))
#define OUTREG8(x, y)       WRITE_REGISTER_UINT8((unsigned char *)(x), (unsigned char)(y))
#define SETREG8(x, y)       OUTREG8(x, INREG8(x)|(y))
#define CLRREG8(x, y)       OUTREG8(x, INREG8(x)&~(y))
#define MASKREG8(x, y, z)   OUTREG8(x, (INREG8(x)&~(y))|(z))

#define INREG16(x)          READ_REGISTER_UINT16((unsigned short *)(x))
#define OUTREG16(x, y)      WRITE_REGISTER_UINT16((unsigned short *)(x),(unsigned short)(y))
#define SETREG16(x, y)      OUTREG16(x, INREG16(x)|(y))
#define CLRREG16(x, y)      OUTREG16(x, INREG16(x)&~(y))
#define MASKREG16(x, y, z)  OUTREG16(x, (INREG16(x)&~(y))|(z))

#define INREG32(x)          READ_REGISTER_UINT32((unsigned int *)(x))
#define OUTREG32(x, y)      WRITE_REGISTER_UINT32((unsigned int *)(x), (unsigned int )(y))
#define SETREG32(x, y)      OUTREG32(x, INREG32(x)|(y))
#define CLRREG32(x, y)      OUTREG32(x, INREG32(x)&~(y))
#define MASKREG32(x, y, z)  OUTREG32(x, (INREG32(x)&~(y))|(z))


#define DRV_Reg8(addr)              INREG8(addr)
#define DRV_WriteReg8(addr, data)   OUTREG8(addr, data)
#define DRV_SetReg8(addr, data)     SETREG8(addr, data)
#define DRV_ClrReg8(addr, data)     CLRREG8(addr, data)

#define DRV_Reg16(addr)             INREG16(addr)
#define DRV_WriteReg16(addr, data)  OUTREG16(addr, data)
#define DRV_SetReg16(addr, data)    SETREG16(addr, data)
#define DRV_ClrReg16(addr, data)    CLRREG16(addr, data)

#define DRV_Reg32(addr)             INREG32(addr)
#define DRV_WriteReg32(addr, data)  OUTREG32(addr, data)
#define DRV_SetReg32(addr, data)    SETREG32(addr, data)
#define DRV_ClrReg32(addr, data)    CLRREG32(addr, data)

#define dsb() __asm__ __volatile__ ("dsb" : : : "memory")



#endif

