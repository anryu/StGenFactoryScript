// RS232C_OM.h: CRS232C_OM クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RS232C_OM_H__69162CA6_B564_4D55_8810_71D691F2AE64__INCLUDED_)
#define AFX_RS232C_OM_H__69162CA6_B564_4D55_8810_71D691F2AE64__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RS232C.h"

//FBC専用コマンド
#define CMD_FBC_CAMERA_SELECT	0x04

//▼2007/10/04 v1008 [HHA]検査時具用コマンド追加
#define CMD_HHA_PORT_STATUS		0x05
//▲2007/10/04 v1008 [HHA]検査時具用コマンド追加
//▼2014/06/24 v2037 [HHA]治具用にIO方向設定関数追加
#define CMD_HHA_PORT_DIRECTION	0x06
//▲2014/06/24 v2037 [HHA]治具用にIO方向設定関数追加

#define CMD_FIRMVERSION		0x10
#define CMD_CAMERATYPE		0x11
#define CMD_I2C				0x49

//▼2009/03/04 v1015 [ILA]内部アドレス2バイトEEPROM対応
#define CMD_I2C_WORD		0x4A
//▲2009/03/04 v1015 [ILA]内部アドレス2バイトEEPROM対応
#define CMD_OSCD			0x50
//#define CMD_ENABLE_SW		0x50
//#define CMD_MODE1			0x51
//#define CMD_MODE2			0x52
//#define CMD_COLOR_MODE		0x53

//▼2010/05/18 v2026 [ILB]CDS設定用
#define CMD_CDS				0x60
//▲2010/05/18 v2026 [ILB]CDS設定用
//▼2009/06/29 v1018 [ILA]DSPと直接通信するための関数を追加
#define CMD_SONY_DSP		0x70
//▲2009/06/29 v1018 [ILA]DSPと直接通信するための関数を追加

//▼2009/07/16 v1020 [ILA]ズーム直接位置指定コマンド追加
#define CMD_ZF_PN			0x80
//▲2009/07/16 v1020 [ILA]ズーム直接位置指定コマンド追加
//#define CMD_ZF_PD			0x81
#define CMD_ZF_PP			0x82	//開発用
//#define CMD_ZF_ND			0x83
#define CMD_ZF_NP			0x84	//開発用

//#define CMD_IRIS_RATIO		0x90
#define CMD_IRIS_POS		0x91	//開発用

#define CMD_ADSINGLE		0xA0
#define CMD_ADSCAN			0xA1
//▼2007/02/17 v1003 [FLC]スイッチ状態取得コマンド追加
#define CMD_SW_STATUS	0xA2
//▲2007/02/17 v1003 [FLC]スイッチ状態取得コマンド追加
#define CMD_ADJUST			0xC0	//開発用
#define CMD_FPGACOM			0xC1	//開発用
//▼2010/06/01 v2027 [ILB]FPGA設定用
#define CMD_FPGACOM_W			0xC2	//開発用
//▲2010/06/01 v2027 [ILB]FPGA設定用
#define CMD_STATUS			0xFE	//開発用

#define CMD_ERROR			0xFF

//カメラリンク用

//デバイスコード(bit7-2)
#define OM_DRWP_D_CAMERA		0x00
#define OM_DRWP_D_PERIPHERAL	0x04

//読み書き(bit1)
#define OM_DRWP_RW_READ			0x00
#define OM_DRWP_RW_WRITE		0x02

//ページ設定(bit0)
#define OM_DRWP_P_REGISTOR		0x00
#define OM_DRWP_P_EEPROM		0x01

class CRS232C_DSP : public CRS232C  
{
public:
	CRS232C_DSP();
	virtual ~CRS232C_DSP();

	//IICバスアクセス
	//DWORD ReadI2C(BYTE byteSlvAdd,BYTE byteStartAdd,BYTE byteEndAdd,PBYTE pbyteData);
	//DWORD WriteI2C(BYTE byteSlvAdd,BYTE byteStartAdd,BYTE byteEndAdd,PBYTE pbyteData);


	//▼2009/06/29 v1018 [ILA]DSPと直接通信するための関数を追加
	//DWORD SonyDSPReadRegisterCategory(BYTE byteCategoryNo, BYTE byteStart, BYTE byteEnd, PBYTE pbyteData);
	//DWORD SonyDSPReadEEPROMCategory(BYTE byteCategoryNo, BYTE byteStart, BYTE byteEnd, PBYTE pbyteData);
	//DWORD SonyDSPWriteRegisterCategory(BYTE byteCategoryNo, BYTE byteStart, BYTE byteEnd, PBYTE pbyteData);
	//DWORD SonyDSPWriteEEPROMCategory(BYTE byteCategoryNo, BYTE byteStart, BYTE byteEnd);
	//DWORD SonyDSPWriteEEPROMAll();
	//DWORD SonyDSPClearEEPROMAll();
	//▲2009/06/29 v1018 [ILA]DSPと直接通信するための関数を追加

	//▼2009/03/04 v1015 [ILA]内部アドレス2バイトEEPROM対応
	//DWORD ReadI2CWord(BYTE byteSlvAdd, WORD wStartAdd, WORD wEndAdd, PBYTE pbyteData);
	//DWORD WriteI2CWord(BYTE byteSlvAdd, WORD wStartAdd, WORD wEndAdd, PBYTE pbyteData);
	//▲2009/03/04 v1015 [ILA]内部アドレス2バイトEEPROM対応

	//1バイト設定
	DWORD GetBytes(BYTE byteCmdNok, WORD wDataCount, PBYTE pbyteData);
	DWORD SetBytes(BYTE byteCmdNo,WORD wDataCount,PBYTE pbyteData);

	//2バイト設定
	DWORD SetWord(BYTE byteCmdNo,WORD wData);
	DWORD GetWord(BYTE byteCmdNo,PWORD pwData);

	//4バイト設定
	DWORD Get2Word(BYTE byteCmdNo,PWORD pwData1,PWORD pwData2);
	DWORD Set2Word(BYTE byteCmdNo,WORD wData1,WORD wData2);

	//1バイト送信->1+数バイト受信
	DWORD SetByteGetBytes(BYTE byteCmdNo, BYTE byteSetData, WORD wDataCount, PBYTE pbyteGetData);

	//▼2010/06/01 v2027 [ILB]FPGA設定用
	//2バイト送信->2+数バイト受信
	DWORD SetWORDGetBytes(BYTE byteCmdNo, WORD wSetData, WORD wDataCount, PBYTE pbyteGetData);
	//▲2010/06/01 v2027 [ILB]FPGA設定用
private:
	
	//▼2009/06/29 v1018 [ILA]DSPと直接通信するための関数を追加
	//DWORD mSonyDSPReadCategory(BYTE byteSwitch, BYTE byteCategoryNo, BYTE byteStart, BYTE byteEnd, PBYTE pbyteData);
	//DWORD mSonyDSPWriteCategory(BYTE byteSwitch, BYTE byteCategoryNo, BYTE byteStart, BYTE byteEnd, PBYTE pbyteData, WORD nDataSize);
	//DWORD mSonyDSPEEPROMAll(BYTE byteSwitch);
	//▲2009/06/29 v1018 [ILA]DSPと直接通信するための関数を追加

	//DWORD mWriteI2CShortSize(BYTE byteSlvAdd,BYTE byteStartAdd,BYTE byteEndAdd,PBYTE pbyteData);

	//▼2009/03/04 v1015 [ILA]内部アドレス2バイトEEPROM対応
	//DWORD mWriteI2CWordShortSize(BYTE byteSlvAdd, WORD wStartAdd, WORD wEndAdd, PBYTE pbyteData);
	//▲2009/03/04 v1015 [ILA]内部アドレス2バイトEEPROM対応
	//整形されたデータを送受信する
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

	//enumハック
	enum{ m_scdwTmpRcvBufferSize = 1024 };
};

/*
class CRS232C_HCA : public CRS232C  
{
public:
	CRS232C_HCA();
	virtual ~CRS232C_HCA();

	//IICバスアクセス
	DWORD ReadI2C(BYTE byteSlvAdd,BYTE byteStartAdd,BYTE byteEndAdd,PBYTE pbyteData);
	DWORD WriteI2C(BYTE byteSlvAdd,BYTE byteStartAdd,BYTE byteEndAdd,PBYTE pbyteData);

private:
	//整形されたデータを送受信する
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
	//enumハック
	enum{ m_scdwTmpRcvBufferSize = 1024 };
};
*/
#endif // !defined(AFX_RS232C_OM_H__69162CA6_B564_4D55_8810_71D691F2AE64__INCLUDED_)
