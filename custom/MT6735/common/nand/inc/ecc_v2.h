/*******************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of MediaTek Inc. (C) 2013
*
*  History:
*  YYYYMMDD MTK12345 Description
*  --------------------
*  20131116 MTK81353 Delete unused code.
*
*
*******************************************************************************/


#ifndef _ECC_V2_H_
#define _ECC_V2_H_

/* Register definition */
#define ECC_base        (0x1100E000)//Sheet PerisysAPB, line 16


//#define MT6516_ECC

/*******************************************************************************
 * ECC register definition
 *******************************************************************************/
#define ECC_ENCCON      ((volatile uint16*)(ECC_base+0x0000))
#define ECC_ENCCNFG     ((volatile uint32*)(ECC_base+0x0004))
#define ECC_ENCDIADDR   ((volatile uint32*)(ECC_base+0x0008))
#define ECC_ENCIDLE     ((volatile uint32*)(ECC_base+0x000C))
#define ECC_ENCPAR0     ((volatile uint32*)(ECC_base+0x0010))
#define ECC_ENCPAR1     ((volatile uint32*)(ECC_base+0x0014))

#define ECC_ENCSTA      ((volatile uint32*)(ECC_base+0x002C))
#define ECC_ENCIRQEN    ((volatile uint16*)(ECC_base+0x0030))
#define ECC_ENCIRQSTA   ((volatile uint16*)(ECC_base+0x0034))

#define ECC_PIO_DIRDY   ((volatile uint16*)(ECC_base+0x0080))
#define ECC_PIO_DI      ((volatile uint32*)(ECC_base+0x0084))

#define ECC_DECCON      ((volatile uint16*)(ECC_base+0x0100))
#define ECC_DECCNFG     ((volatile uint32*)(ECC_base+0x0104))
#define ECC_DECDIADDR   ((volatile uint32*)(ECC_base+0x0108))
#define ECC_DECIDLE     ((volatile uint16*)(ECC_base+0x010C))
#define ECC_DECFER      ((volatile uint16*)(ECC_base+0x0110))
#define ECC_DECENUM0    ((volatile uint32*)(ECC_base+0x0114))
#define ECC_DECENUM1    ((volatile uint32*)(ECC_base+0x0118))
#define ECC_DECDONE     ((volatile uint16*)(ECC_base+0x011C))
#define ECC_DECEL0      ((volatile uint32*)(ECC_base+0x0120))
#define ECC_DECEL1      ((volatile uint32*)(ECC_base+0x0124))

#define ECC_DECIRQEN    ((volatile uint16*)(ECC_base+0x0140))
#define ECC_DECIRQSTA   ((volatile uint16*)(ECC_base+0x0144))
#define ECC_FDMADDR     ((volatile uint32*)(ECC_base+0x0148))



/*******************************************************************************
 * ECC register bit definition
 *******************************************************************************/

/* ECC_ENCON */
#define ENC_EN                 (0x0001)

/* ECC_ENCCNFG */
#define ECC_CNFG_ECC4          (0x0000)
#define ECC_CNFG_ECC6          (0x0001)
#define ECC_CNFG_ECC8          (0x0002)
#define ECC_CNFG_ECC10         (0x0003)
#define ECC_CNFG_ECC12         (0x0004)
#define ECC_CNFG_ECC14         (0x0005)
#define ECC_CNFG_ECC16         (0x0006)
#define ECC_CNFG_ECC_MASK      (0x00000007)

#define ECC_CNFG_DMA           (0x0000)
#define ECC_CNFG_NFI           (0x0010)
#define ECC_CNFG_PIO           (0x0020)
#define ECC_CNFG_MODE_MASK     (0x0030)


#define ECC_CNFG_META6         (0x10300000)
#define ECC_CNFG_META8         (0x10400000)

#define ENC_CNFG_MSG_MASK      (0x1FFF0000)
#define ENC_CNFG_MSG_SHIFT     (0x10)


/* ECC_ENCIDLE */
#define ENC_IDLE               (0x0001)

/* ECC_ENCSTA */
#define STA_FSM                 (0x001F)
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
//#define DEC_CNFG_NFI           (0x0010)
//#define DEC_CNFG_META6         (0x10300000)
//#define DEC_CNFG_META8         (0x10400000)

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


#define ERR_NUM0               (0x0000001F)
#define ERR_NUM1               (0x000001F0)
#define ERR_NUM2               (0x00001F00)
#define ERR_NUM3               (0x0001F000)
#define ERR_NUM4               (0x0000001F)
#define ERR_NUM5               (0x000001F0)
#define ERR_NUM6               (0x00001F00)
#define ERR_NUM7               (0x0001F000)
#define ERR_NUM_SHIFT          5


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

#define GET_ECC_ERRDONE(val) \
do{\
   val = *ECC_DECDONE;\
}while(0);


#define ECC_PARITY_NUM        (208)             //NOTE: no use now, modify here in case the define means something in later version
#define ECC_PARITY_BITS       (13)


#endif /* _ECC_V2_H_ */

