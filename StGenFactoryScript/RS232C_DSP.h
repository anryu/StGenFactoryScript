// RS232C_OM.h: CRS232C_OM �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RS232C_OM_H__69162CA6_B564_4D55_8810_71D691F2AE64__INCLUDED_)
#define AFX_RS232C_OM_H__69162CA6_B564_4D55_8810_71D691F2AE64__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RS232C.h"

//FBC��p�R�}���h
#define CMD_FBC_CAMERA_SELECT	0x04

//��2007/10/04 v1008 [HHA]��������p�R�}���h�ǉ�
#define CMD_HHA_PORT_STATUS		0x05
//��2007/10/04 v1008 [HHA]��������p�R�}���h�ǉ�
//��2014/06/24 v2037 [HHA]����p��IO�����ݒ�֐��ǉ�
#define CMD_HHA_PORT_DIRECTION	0x06
//��2014/06/24 v2037 [HHA]����p��IO�����ݒ�֐��ǉ�

#define CMD_FIRMVERSION		0x10
#define CMD_CAMERATYPE		0x11
#define CMD_I2C				0x49

//��2009/03/04 v1015 [ILA]�����A�h���X2�o�C�gEEPROM�Ή�
#define CMD_I2C_WORD		0x4A
//��2009/03/04 v1015 [ILA]�����A�h���X2�o�C�gEEPROM�Ή�
#define CMD_OSCD			0x50
//#define CMD_ENABLE_SW		0x50
//#define CMD_MODE1			0x51
//#define CMD_MODE2			0x52
//#define CMD_COLOR_MODE		0x53

//��2010/05/18 v2026 [ILB]CDS�ݒ�p
#define CMD_CDS				0x60
//��2010/05/18 v2026 [ILB]CDS�ݒ�p
//��2009/06/29 v1018 [ILA]DSP�ƒ��ڒʐM���邽�߂̊֐���ǉ�
#define CMD_SONY_DSP		0x70
//��2009/06/29 v1018 [ILA]DSP�ƒ��ڒʐM���邽�߂̊֐���ǉ�

//��2009/07/16 v1020 [ILA]�Y�[�����ڈʒu�w��R�}���h�ǉ�
#define CMD_ZF_PN			0x80
//��2009/07/16 v1020 [ILA]�Y�[�����ڈʒu�w��R�}���h�ǉ�
//#define CMD_ZF_PD			0x81
#define CMD_ZF_PP			0x82	//�J���p
//#define CMD_ZF_ND			0x83
#define CMD_ZF_NP			0x84	//�J���p

//#define CMD_IRIS_RATIO		0x90
#define CMD_IRIS_POS		0x91	//�J���p

#define CMD_ADSINGLE		0xA0
#define CMD_ADSCAN			0xA1
//��2007/02/17 v1003 [FLC]�X�C�b�`��Ԏ擾�R�}���h�ǉ�
#define CMD_SW_STATUS	0xA2
//��2007/02/17 v1003 [FLC]�X�C�b�`��Ԏ擾�R�}���h�ǉ�
#define CMD_ADJUST			0xC0	//�J���p
#define CMD_FPGACOM			0xC1	//�J���p
//��2010/06/01 v2027 [ILB]FPGA�ݒ�p
#define CMD_FPGACOM_W			0xC2	//�J���p
//��2010/06/01 v2027 [ILB]FPGA�ݒ�p
#define CMD_STATUS			0xFE	//�J���p

#define CMD_ERROR			0xFF

//�J���������N�p

//�f�o�C�X�R�[�h(bit7-2)
#define OM_DRWP_D_CAMERA		0x00
#define OM_DRWP_D_PERIPHERAL	0x04

//�ǂݏ���(bit1)
#define OM_DRWP_RW_READ			0x00
#define OM_DRWP_RW_WRITE		0x02

//�y�[�W�ݒ�(bit0)
#define OM_DRWP_P_REGISTOR		0x00
#define OM_DRWP_P_EEPROM		0x01

class CRS232C_DSP : public CRS232C  
{
public:
	CRS232C_DSP();
	virtual ~CRS232C_DSP();

	//IIC�o�X�A�N�Z�X
	//DWORD ReadI2C(BYTE byteSlvAdd,BYTE byteStartAdd,BYTE byteEndAdd,PBYTE pbyteData);
	//DWORD WriteI2C(BYTE byteSlvAdd,BYTE byteStartAdd,BYTE byteEndAdd,PBYTE pbyteData);


	//��2009/06/29 v1018 [ILA]DSP�ƒ��ڒʐM���邽�߂̊֐���ǉ�
	//DWORD SonyDSPReadRegisterCategory(BYTE byteCategoryNo, BYTE byteStart, BYTE byteEnd, PBYTE pbyteData);
	//DWORD SonyDSPReadEEPROMCategory(BYTE byteCategoryNo, BYTE byteStart, BYTE byteEnd, PBYTE pbyteData);
	//DWORD SonyDSPWriteRegisterCategory(BYTE byteCategoryNo, BYTE byteStart, BYTE byteEnd, PBYTE pbyteData);
	//DWORD SonyDSPWriteEEPROMCategory(BYTE byteCategoryNo, BYTE byteStart, BYTE byteEnd);
	//DWORD SonyDSPWriteEEPROMAll();
	//DWORD SonyDSPClearEEPROMAll();
	//��2009/06/29 v1018 [ILA]DSP�ƒ��ڒʐM���邽�߂̊֐���ǉ�

	//��2009/03/04 v1015 [ILA]�����A�h���X2�o�C�gEEPROM�Ή�
	//DWORD ReadI2CWord(BYTE byteSlvAdd, WORD wStartAdd, WORD wEndAdd, PBYTE pbyteData);
	//DWORD WriteI2CWord(BYTE byteSlvAdd, WORD wStartAdd, WORD wEndAdd, PBYTE pbyteData);
	//��2009/03/04 v1015 [ILA]�����A�h���X2�o�C�gEEPROM�Ή�

	//1�o�C�g�ݒ�
	DWORD GetBytes(BYTE byteCmdNok, WORD wDataCount, PBYTE pbyteData);
	DWORD SetBytes(BYTE byteCmdNo,WORD wDataCount,PBYTE pbyteData);

	//2�o�C�g�ݒ�
	DWORD SetWord(BYTE byteCmdNo,WORD wData);
	DWORD GetWord(BYTE byteCmdNo,PWORD pwData);

	//4�o�C�g�ݒ�
	DWORD Get2Word(BYTE byteCmdNo,PWORD pwData1,PWORD pwData2);
	DWORD Set2Word(BYTE byteCmdNo,WORD wData1,WORD wData2);

	//1�o�C�g���M->1+���o�C�g��M
	DWORD SetByteGetBytes(BYTE byteCmdNo, BYTE byteSetData, WORD wDataCount, PBYTE pbyteGetData);

	//��2010/06/01 v2027 [ILB]FPGA�ݒ�p
	//2�o�C�g���M->2+���o�C�g��M
	DWORD SetWORDGetBytes(BYTE byteCmdNo, WORD wSetData, WORD wDataCount, PBYTE pbyteGetData);
	//��2010/06/01 v2027 [ILB]FPGA�ݒ�p
private:
	
	//��2009/06/29 v1018 [ILA]DSP�ƒ��ڒʐM���邽�߂̊֐���ǉ�
	//DWORD mSonyDSPReadCategory(BYTE byteSwitch, BYTE byteCategoryNo, BYTE byteStart, BYTE byteEnd, PBYTE pbyteData);
	//DWORD mSonyDSPWriteCategory(BYTE byteSwitch, BYTE byteCategoryNo, BYTE byteStart, BYTE byteEnd, PBYTE pbyteData, WORD nDataSize);
	//DWORD mSonyDSPEEPROMAll(BYTE byteSwitch);
	//��2009/06/29 v1018 [ILA]DSP�ƒ��ڒʐM���邽�߂̊֐���ǉ�

	//DWORD mWriteI2CShortSize(BYTE byteSlvAdd,BYTE byteStartAdd,BYTE byteEndAdd,PBYTE pbyteData);

	//��2009/03/04 v1015 [ILA]�����A�h���X2�o�C�gEEPROM�Ή�
	//DWORD mWriteI2CWordShortSize(BYTE byteSlvAdd, WORD wStartAdd, WORD wEndAdd, PBYTE pbyteData);
	//��2009/03/04 v1015 [ILA]�����A�h���X2�o�C�gEEPROM�Ή�
	//���`���ꂽ�f�[�^�𑗎�M����
	DWORD mFormatSend(
				BOOL	blnDirWriteFg,
				BYTE	byteCmdNo,
				WORD	wDataLen,
				PBYTE	pbyteData);
	DWORD mFormatRcv(
				PBYTE	pbyteCmdNo,
				PWORD	pwDataLen,
				PBYTE	pbyteData,
				WORD	wMaxRcvBytes);

	DWORD mSendAndRcv(
				BOOL	blnDirWriteFg,
				BYTE	byteSndCmdNo,
				WORD	wSndDataLen,
				PBYTE	pbyteSndData,
				PBYTE	pbyteRcvCmdNo,
				PWORD	pwRcvDataLen,
				PBYTE	pbyteRcvData,
				WORD	wMaxRcvBytes);

	//enum�n�b�N
	enum{ m_scdwTmpRcvBufferSize = 1024 };
};

/*
class CRS232C_HCA : public CRS232C  
{
public:
	CRS232C_HCA();
	virtual ~CRS232C_HCA();

	//IIC�o�X�A�N�Z�X
	DWORD ReadI2C(BYTE byteSlvAdd,BYTE byteStartAdd,BYTE byteEndAdd,PBYTE pbyteData);
	DWORD WriteI2C(BYTE byteSlvAdd,BYTE byteStartAdd,BYTE byteEndAdd,PBYTE pbyteData);

private:
	//���`���ꂽ�f�[�^�𑗎�M����
	DWORD mSendAndRcv(
				BYTE	byteDRWPCode,
				BYTE	byteCmdNo,
				BYTE	byteDataLen,
				PBYTE	pbyteData);
	DWORD mFormatSend(
				BYTE	byteDRWPCode,
				BYTE	byteCmdNo,
				BYTE	byteDataLen,
				PBYTE	pbyteData);
	DWORD mFormatRcv(
				BYTE	byteDRWPCode,
				BYTE	byteCmdNo,
				BYTE	byteDataLen,
				PBYTE	pbyteData);
	//enum�n�b�N
	enum{ m_scdwTmpRcvBufferSize = 1024 };
};
*/
#endif // !defined(AFX_RS232C_OM_H__69162CA6_B564_4D55_8810_71D691F2AE64__INCLUDED_)
