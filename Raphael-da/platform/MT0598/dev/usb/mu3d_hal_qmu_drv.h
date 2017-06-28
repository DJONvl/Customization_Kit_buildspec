#ifndef MTK_QMU_H
#define MTK_QMU_H

typedef struct _TGPD {
   kal_uint8	flag;				/* HWO, BPS, BDP, IOC*/
   kal_uint8	chksum;
   kal_uint16	DataBufferLen; 		/*Rx only, inteded recvLen*/
   struct _TGPD*	pNext;
   kal_uint8*			pBuf;		/* point to gpd data buf when bdp = 0, otherwise point to TBD */
   kal_uint16	bufLen;				/* intented writeLen in TX, actual recvLen in RX */
   kal_uint8	ExtLength;			/* TX only, gpd ext buf */
   kal_uint8	ZTepFlag;			/* TX only, set ZLP feature for non-isoc transfer */
}TGPD, *PGPD;


#define AT_GPD_EXT_LEN					256
#define AT_BD_EXT_LEN 					256
#define MAX_GPD_NUM						30
#define MAX_BD_NUM						500
#define STRESS_IOC_TH 					8
#define STRESS_GPD_TH 					24
#define RANDOM_STOP_DELAY 				80
#define STRESS_DATA_LENGTH 				1024*64//1024*16
#define MAX_GPD_NUM						30



#define TGPD_FLAGS_HWO              	0x01
#define TGPD_IS_FLAGS_HWO(_pd)      	(((TGPD *)_pd)->flag & TGPD_FLAGS_HWO)
#define TGPD_SET_FLAGS_HWO(_pd)     	(((TGPD *)_pd)->flag |= TGPD_FLAGS_HWO)
#define TGPD_CLR_FLAGS_HWO(_pd)     	(((TGPD *)_pd)->flag &= (~TGPD_FLAGS_HWO))
#define TGPD_FORMAT_BDP             	0x02
#define TGPD_IS_FORMAT_BDP(_pd)     	(((TGPD *)_pd)->flag & TGPD_FORMAT_BDP)
#define TGPD_SET_FORMAT_BDP(_pd)    	(((TGPD *)_pd)->flag |= TGPD_FORMAT_BDP)
#define TGPD_CLR_FORMAT_BDP(_pd)    	(((TGPD *)_pd)->flag &= (~TGPD_FORMAT_BDP))
#define TGPD_FORMAT_BPS             	0x04
#define TGPD_IS_FORMAT_BPS(_pd)     	(((TGPD *)_pd)->flag & TGPD_FORMAT_BPS)
#define TGPD_SET_FORMAT_BPS(_pd)    	(((TGPD *)_pd)->flag |= TGPD_FORMAT_BPS)
#define TGPD_CLR_FORMAT_BPS(_pd)    	(((TGPD *)_pd)->flag &= (~TGPD_FORMAT_BPS))
#define TGPD_SET_FLAG(_pd, _flag)   	((TGPD *)_pd)->flag = (((TGPD *)_pd)->flag&(~TGPD_FLAGS_HWO))|(_flag)
#define TGPD_GET_FLAG(_pd)             	(((TGPD *)_pd)->flag & TGPD_FLAGS_HWO)
#define TGPD_SET_CHKSUM(_pd, _n)    	((TGPD *)_pd)->chksum = USB_QMU_Cal_Checksum((kal_uint8 *)_pd, _n)-1
#define TGPD_SET_CHKSUM_HWO(_pd, _n)    ((TGPD *)_pd)->chksum = USB_QMU_Cal_Checksum((kal_uint8 *)_pd, _n)-1
#define TGPD_GET_CHKSUM(_pd)        	((TGPD *)_pd)->chksum
#define TGPD_SET_FORMAT(_pd, _fmt)  	((TGPD *)_pd)->flag = (((TGPD *)_pd)->flag&(~TGPD_FORMAT_BDP))|(_fmt)
#define TGPD_GET_FORMAT(_pd)        	((((TGPD *)_pd)->flag & TGPD_FORMAT_BDP)>>1)
#define TGPD_SET_DataBUF_LEN(_pd, _len) ((TGPD *)_pd)->DataBufferLen = _len
#define TGPD_ADD_DataBUF_LEN(_pd, _len) ((TGPD *)_pd)->DataBufferLen += _len
#define TGPD_GET_DataBUF_LEN(_pd)       ((TGPD *)_pd)->DataBufferLen
#define TGPD_SET_NEXT(_pd, _next)   	((TGPD *)_pd)->pNext = (TGPD *)_next;
#define TGPD_GET_NEXT(_pd)          	(TGPD *)((TGPD *)_pd)->pNext
#define TGPD_SET_TBD(_pd, _tbd)     	((TGPD *)_pd)->pBuf = (kal_uint8 *)_tbd;\
   TGPD_SET_FORMAT_BDP(_pd)
#define TGPD_GET_TBD(_pd)           	(TBD *)((TGPD *)_pd)->pBuf
#define TGPD_SET_DATA(_pd, _data)   	((TGPD *)_pd)->pBuf = (kal_uint8 *)_data
#define TGPD_GET_DATA(_pd)          	(kal_uint8*)((TGPD *)_pd)->pBuf
#define TGPD_SET_BUF_LEN(_pd, _len) 	((TGPD *)_pd)->bufLen = _len
#define TGPD_ADD_BUF_LEN(_pd, _len) 	((TGPD *)_pd)->bufLen += _len
#define TGPD_GET_BUF_LEN(_pd)       	((TGPD *)_pd)->bufLen
#define TGPD_SET_EXT_LEN(_pd, _len) 	((TGPD *)_pd)->ExtLength = _len
#define TGPD_GET_EXT_LEN(_pd)        	((TGPD *)_pd)->ExtLength
#define TGPD_SET_EPaddr(_pd, _EP)  		((TGPD *)_pd)->ZTepFlag =(((TGPD *)_pd)->ZTepFlag&0xF0)|(_EP)
#define TGPD_GET_EPaddr(_pd)        	((TGPD *)_pd)->ZTepFlag & 0x0F 
#define TGPD_FORMAT_TGL             	0x10
#define TGPD_IS_FORMAT_TGL(_pd)     	(((TGPD *)_pd)->ZTepFlag & TGPD_FORMAT_TGL)
#define TGPD_SET_FORMAT_TGL(_pd)    	(((TGPD *)_pd)->ZTepFlag |=TGPD_FORMAT_TGL)
#define TGPD_CLR_FORMAT_TGL(_pd)    	(((TGPD *)_pd)->ZTepFlag &= (~TGPD_FORMAT_TGL))
#define TGPD_FORMAT_ZLP             	0x20
#define TGPD_IS_FORMAT_ZLP(_pd)     	(((TGPD *)_pd)->ZTepFlag & TGPD_FORMAT_ZLP)
#define TGPD_SET_FORMAT_ZLP(_pd)    	(((TGPD *)_pd)->ZTepFlag |=TGPD_FORMAT_ZLP)
#define TGPD_CLR_FORMAT_ZLP(_pd)    	(((TGPD *)_pd)->ZTepFlag &= (~TGPD_FORMAT_ZLP))
#define TGPD_FORMAT_IOC             	0x80
#define TGPD_IS_FORMAT_IOC(_pd)     	(((TGPD *)_pd)->flag & TGPD_FORMAT_IOC)
#define TGPD_SET_FORMAT_IOC(_pd)    	(((TGPD *)_pd)->flag |=TGPD_FORMAT_IOC) 
#define TGPD_CLR_FORMAT_IOC(_pd)    	(((TGPD *)_pd)->flag &= (~TGPD_FORMAT_IOC))
#define TGPD_SET_TGL(_pd, _TGL)  		((TGPD *)_pd)->ZTepFlag |=(( _TGL) ? 0x10: 0x00)
#define TGPD_GET_TGL(_pd)        		((TGPD *)_pd)->ZTepFlag & 0x10 ? 1:0
#define TGPD_SET_ZLP(_pd, _ZLP)  		((TGPD *)_pd)->ZTepFlag |= ((_ZLP) ? 0x20: 0x00)
#define TGPD_GET_ZLP(_pd)        		((TGPD *)_pd)->ZTepFlag & 0x20 ? 1:0
#define TGPD_GET_EXT(_pd)           	((kal_uint8 *)_pd + sizeof(TGPD))

#endif
