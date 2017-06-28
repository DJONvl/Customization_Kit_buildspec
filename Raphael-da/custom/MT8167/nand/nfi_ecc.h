/*****************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of MediaTek Inc. (C) 2001
*
*****************************************************************************/

#ifndef _NFI_ECC_H_
#define _NFI_ECC_H_

#define ECC_base        		(0x11002000)


/*******************************************************************************
 * ECC register definition
 *******************************************************************************/
#define ECC_ENCCON      ((volatile uint16*)(ECC_base+0x0000))
#define ECC_ENCCON_REG16    ECC_ENCCON
#define ECC_ENCCNFG     ((volatile uint32*)(ECC_base+0x0004))
#define ECC_ENCCNFG_REG32   ECC_ENCCNFG
#define ECC_ENCDIADDR   ((volatile uint32*)(ECC_base+0x0008))
#define ECC_ENCIDLE     ((volatile uint32*)(ECC_base+0x000C))
#define ECC_ENCIDLE_REG32   ECC_ENCIDLE
#define ECC_ENCPAR0     ((volatile uint32*)(ECC_base+0x0010))
#define ECC_ENCPAR1     ((volatile uint32*)(ECC_base+0x0014))
#define ECC_ENCSTA      ((volatile uint32*)(ECC_base+0x007C))
#define ECC_ENCIRQEN    ((volatile uint16*)(ECC_base+0x0080))
#define ECC_ENCIRQSTA   ((volatile uint16*)(ECC_base+0x0084))
#define ECC_PIO_DIRDY   ((volatile uint16*)(ECC_base+0x0090))
#define ECC_PIO_DI      ((volatile uint32*)(ECC_base+0x0094))
#define ECC_DECCON      ((volatile uint16*)(ECC_base+0x0100))
#define ECC_DECCON_REG16    ECC_DECCON
#define ECC_DECCNFG     ((volatile uint32*)(ECC_base+0x0104))
#define ECC_DECCNFG_REG32   ECC_DECCNFG
#define ECC_DECDIADDR   ((volatile uint32*)(ECC_base+0x0108))
#define ECC_DECIDLE     ((volatile uint16*)(ECC_base+0x010C))
#define ECC_DECIDLE_REG16   ECC_DECIDLE
#define ECC_DECFER      ((volatile uint16*)(ECC_base+0x0110))
#define ECC_DECENUM0    ((volatile uint32*)(ECC_base+0x0114))
#define ECC_DECENUM1    ((volatile uint32*)(ECC_base+0x0118))
#define ECC_DECDONE     ((volatile uint16*)(ECC_base+0x0124))
#define ECC_DECDONE_REG16   ECC_DECDONE
#define ECC_DECEL0      ((volatile uint32*)(ECC_base+0x0128))
#define ECC_DECEL1      ((volatile uint32*)(ECC_base+0x012C))
#define ECC_FDMADDR     ((volatile uint32*)(ECC_base+0x0148))
#define ECC_FDMADDR_REG32   ECC_FDMADDR
#define ECC_DECIRQEN    ((volatile uint16*)(ECC_base+0x0200))
#define ECC_DECIRQSTA   ((volatile uint16*)(ECC_base+0x0204))
#define ECC_DECFSM ((volatile uint32*)(ECC_base+0x0208))
#define ECC_BYPASS ((volatile uint16*)(ECC_base+0x020C))

#define ECC_ENCPAR27      ((volatile uint32*)(ECC_base+0x0300))
#define ECC_ENCPAR34      ((volatile uint32*)(ECC_base+0x031C))
#define ECC_DECEL30      ((volatile uint32*)(ECC_base+0x0400))
#define ECC_DECEL39      ((volatile uint32*)(ECC_base+0x0424))

/*******************************************************************************
 * ECC register bit definition
 *******************************************************************************/
/* ECC_ENCON */
#define ENC_EN                 (0x0001)
#define ENC_DE                 (0x0000)
#define DEC_DE                 (0x0000)

/* ECC_ENCCNFG */
#define ECC_CNFG_ECC4          (0x0000)
#define ECC_CNFG_ECC6          (0x0001)
#define ECC_CNFG_ECC8          (0x0002)
#define ECC_CNFG_ECC10         (0x0003)
#define ECC_CNFG_ECC12         (0x0004)
#define ECC_CNFG_ECC14         (0x0005)
#define ECC_CNFG_ECC16         (0x0006)
#define ECC_CNFG_ECC18         (0x0007)
#define ECC_CNFG_ECC20         (0x0008)
#define ECC_CNFG_ECC22         (0x0009)
#define ECC_CNFG_ECC24         (0x000A)
#define ECC_CNFG_ECC28         (0x000B)
#define ECC_CNFG_ECC32         (0x000C)
#define ECC_CNFG_ECC36         (0x000D)
#define ECC_CNFG_ECC40         (0x000E)
#define ECC_CNFG_ECC44         (0x000F)
#define ECC_CNFG_ECC48         (0x0010)
#define ECC_CNFG_ECC52         (0x0011)
#define ECC_CNFG_ECC56         (0x0012)
#define ECC_CNFG_ECC60         (0x0013)
#define ECC_CNFG_ECC68         (0x0014)
#define ECC_CNFG_ECC72         (0x0015)
#define ECC_CNFG_ECC80         (0x0016)

#define ENC_CNFG_NFI           (0x0020)
#define ECC_CNFG_ECC_MASK      (0x0000001F)


#define ECC_CNFG_DMA           (0x0000)
#define ECC_CNFG_NFI           (0x0020)
#define ECC_CNFG_PIO           (0x0040)
#define ECC_CNFG_MODE_MASK     (0x0060)


#define ECC_CNFG_META6         (0x10300000)
#define ECC_CNFG_META8         (0x10400000)

#define ENC_CNFG_MSG_MASK      (0x3FFF0000)
#define ENC_CNFG_MSG_SHIFT     (0x10)

/* ECC_ENCIDLE */
#define ENC_IDLE               (0x0001)

/* ECC_ENCSTA */
#define STA_FSM                      (0x001F)
#define STA_COUNT_PS           (0xFF10)
#define STA_COUNT_MS           (0x3FFF0000)

/* ECC_ENCIRQEN */
#define ENC_IRQEN              (0x0001)

/* ECC_ENCIRQSTA */
#define ENC_IRQSTA             (0x0001)

/* ECC_PIO_DIRDY */
#define PIO_DIR_DY             (0x0001)

/* ECC_DECCON */
#define DEC_EN                 (0x0001)

/* ECC_DECCNFG */
#define DEC_CNFG_ECC4          (0x0000)
#define DEC_CNFG_ECC6          (0x0001)
#define DEC_CNFG_ECC8          (0x0002)
#define DEC_CNFG_ECC10         (0x0003)
#define DEC_CNFG_ECC12         (0x0004)
#define DEC_CNFG_ECC14         (0x0005)
#define DEC_CNFG_ECC16         (0x0006)

#define DEC_CNFG_NFI           (0x0020)
#define DEC_CNFG_AHB           (0x0000)
#define DEC_CNFG_DEC_MODE_MASK (0x0060)

#define DEC_CNFG_FER           (0x01000)
#define DEC_CNFG_EL            (0x02000)
#define DEC_CNFG_CORRECT       (0x03000)
#define DEC_CNFG_TYPE_MASK     (0x03000)

#define DEC_CNFG_EMPTY_EN      (0x80000000)

#define DEC_CNFG_CODE_MASK     (0x1FFF0000)
#define DEC_CNFG_CODE_SHIFT    (0x10)

/* ECC_DECIDLE */
#define DEC_IDLE               (0x0001)

/* ECC_DECDONE for PIO */
#define DEC_PIO_DONE           (0x0001)

/* ECC_DECFER */
#define DEC_FER0               (0x0001)
#define DEC_FER1               (0x0002)
#define DEC_FER2               (0x0004)
#define DEC_FER3               (0x0008)
#define DEC_FER4               (0x0010)
#define DEC_FER5               (0x0020)
#define DEC_FER6               (0x0040)
#define DEC_FER7               (0x0080)

#define ERR_NUM0               (0x0000007F)
#define ERR_NUM1               (0x00007F00)
#define ERR_NUM2               (0x007F0000)
#define ERR_NUM3               (0x7F000000)
#define ERR_NUM4               (0x0000007F)
#define ERR_NUM5               (0x00007F00)
#define ERR_NUM6               (0x007F0000)
#define ERR_NUM7               (0x7F000000)
#define ERR_NUM_SHIFT          8

/* ECC_DECDONE */
#define DEC_DONE0               (0x0001)
#define DEC_DONE1               (0x0002)
#define DEC_DONE2               (0x0004)
#define DEC_DONE3               (0x0008)
#define DEC_DONE4               (0x0010)
#define DEC_DONE5               (0x0020)
#define DEC_DONE6               (0x0040)
#define DEC_DONE7               (0x0080)

/* ECC_DECIRQEN */
#define DEC_IRQEN               (0x0001)

/* ECC_DECIRQSTA */
#define DEC_IRQSTA              (0x0001)


/************************************************************************
 * Macro definition
 ************************************************************************/
#define WAIT_ENC_READY_WITH_TIMEOUT(timeout) \
do {\
   while( (!(*ECC_ENCIDLE & ENC_IDLE)) && (--timeout) );\
} while(0);

#define WAIT_ENC_READY \
do {\
   while(!(*ECC_ENCIDLE & ENC_IDLE));\
} while(0);

#define WAIT_ENC_PARITY_READY \
do {\
   while(*ECC_ENCSTA & STA_COUNT_MS);\
} while(0);

#define WAIT_PIO_READY \
do {\
   while(!(*ECC_PIO_DIRDY & PIO_DIR_DY));\
} while(0);
#define WAIT_DEC_READY_WITH_TIMEOUT(timeout) \
do {\
   while( (!(*ECC_DECIDLE & DEC_IDLE)) && (--timeout) );\
} while(0);

#define WAIT_DEC_DONE_WITH_TIMEOUT(blk_idx, timeout) \
do {\
   while( (!(*ECC_DECDONE & (1<<blk_idx))) && (--timeout) );\
} while(0);

#define WAIT_DEC_READY \
do {\
   while(!(*ECC_DECIDLE & DEC_IDLE));\
} while(0);

#define WAIT_DEC_PIO_DONE \
do {\
   while(!(*ECC_DECDONE & DEC_PIO_DONE));\
} while(0);

#define START_ECC_DEC \
do{\
   *ECC_DECCON = 0;\
   *ECC_DECCON = DEC_EN;\
}while(0);

#define STOP_ECC_DEC \
do{\
   *ECC_DECCON = 0;\
}while(0);

#define START_ECC_ENC \
do{\
   *ECC_ENCCON = 0;\
   *ECC_ENCCON = ENC_EN;\
}while(0);

#define STOP_ECC_ENC \
do{\
   *ECC_ENCCON = 0;\
}while(0);
#if 0
#define GET_ECC_ERRNUM(val)\
do{\
   val = *ECC_DECENUM;\
}while(0);
#endif
#define GET_ECC_ERRDONE(val) \
do{\
   val = *ECC_DECDONE;\
}while(0);

#define ECC_PARITY_NUM        (208)             //NOTE: no use now, modify here in case the define means something in later version

#define ECC_PARITY_BITS       (14)

#endif /* _NFI_ECC_H_ */
