#ifndef __C_SLA_H__
#define __C_SLA_H__

#include "brom.h"		//CALLBACK_SLA_CHALLENGE
#include "mtk_msp.h"
#include "Mdebug.h"		//MTRACE

//FIXME: MT6276 SV5
#if defined(M1_MAX_KEY_LEN)
#undef M1_MAX_KEY_LEN
#endif
#define M1_MAX_KEY_LEN 2048

//==============================================================================
// BOOTROM security relevant command	(From MT6276 @ 2009H2)
//==============================================================================
#define BROM_CMD_GET_TARGET_CONFIG  0xD8

#define BROM_SCMD_SEND_CERT         0xE0
#define BROM_SCMD_GET_ME_ID         0xE1
#define BROM_SCMD_SEND_AUTH         0xE2
#define BROM_SCMD_SLA               0xE3

#define SEC_CFG_SBC_EN              0x00000001
#define SEC_CFG_SLA_EN              0x00000002
#define SEC_CFG_DAA_EN              0x00000004

//==============================================================================
// BOOTROM security relevant command	(622x, 623x, 6253, 6268, 6516)
//==============================================================================
#define BROM_SCMD_OLD_SLA_SEND_AUTH			0xC1
#define BROM_SCMD_OLD_SLA_GET_RN			0xC2
#define BROM_SCMD_OLD_SLA_VERIFY_RN			0xC3


class BRom_Base;

//------------------------------------------------------------
// SLA base class
//------------------------------------------------------------
class CSLA
{
public:
	CSLA(){};
	~CSLA(){};

public:

	virtual int SLA_Flow(
		HANDLE hCOM, 
		const unsigned char *pAUTH, unsigned int AUTH_Len, 
		CALLBACK_SLA_CHALLENGE  cb, void *cb_arg, 
		CALLBACK_SLA_CHALLENGE_END  cb_end, void *cb_end_arg, 
		unsigned char &err_code)=0;


protected:
	BRom_Base	* m_brom_base;

	hMTRACE	m_hBROM_DEBUG;

};

//------------------------------------------------------------
//------------------------------------------------------------
class CSLA_SV1 : public CSLA
{
public:
	CSLA_SV1(BRom_Base * brom_base, hMTRACE hBROM_DEBUG);
	~CSLA_SV1(){};

private:
	virtual int BromCmd_SendAUTH(HANDLE hCOM, const unsigned char *data, unsigned int len_in_byte, unsigned char  &err_code);
	virtual int BromCmd_GetRN(HANDLE hCOM, unsigned char  *p_rn_buf, unsigned int rn_buf_len, unsigned int &rn_len);
	virtual int BromCmd_VerifyRN(HANDLE hCOM, const unsigned char  *p_rn2, unsigned int rn2_len, unsigned char  &err_code);

public:
	virtual int SLA_Flow(
		HANDLE hCOM, 
		const unsigned char *pAUTH, unsigned int AUTH_Len, 
		CALLBACK_SLA_CHALLENGE  cb, void *cb_arg, 
		CALLBACK_SLA_CHALLENGE_END  cb_end, void *cb_end_arg, 
		unsigned char &err_code);
};

class CSLA_SV5 : public CSLA
{
public:
	CSLA_SV5(BRom_Base * brom_base, hMTRACE hBROM_DEBUG);
	~CSLA_SV5(){};


public:
	virtual int BromCmd_SendAUTH(HANDLE hCOM, const unsigned char *data, unsigned int len_in_byte, unsigned char  &err_code);
	virtual int BromCmd_SendCERT(HANDLE hCOM, const unsigned char *data, unsigned int len_in_byte, unsigned char  &err_code);
	virtual int BromCmd_GetME(HANDLE hCOM, unsigned char  *p_me_buf, unsigned int me_buf_len, unsigned int &me_len);
	virtual int BromCmd_GetSecConf(HANDLE hCOM, unsigned int &sec_config);

	virtual int SLA_Flow(
		HANDLE hCOM, 
		const unsigned char *pAUTH, unsigned int AUTH_Len, 
		CALLBACK_SLA_CHALLENGE  cb, void *cb_arg, 
		CALLBACK_SLA_CHALLENGE_END  cb_end, void *cb_end_arg, 
		unsigned char &err_code);
};

#endif//__C_SLA_H__
