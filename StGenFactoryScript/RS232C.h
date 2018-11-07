
#ifdef _MSC_VER
#include "StdAfx.h"		//For Visual C++ 6.0
#else
#include "frmMain.h"
//#include <vcl.h>		//For Borland C++ Builder 5.0
#endif

//#include "CameraLinkAPI.h"

#if !defined(_RS232C_H__INCLUDED_)
#define _RS232C_H__INCLUDED_




#define RS232C_FORMAT_SOF 0x02
#define RS232C_FORMAT_EOF 0x03

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
//#define RS232C_COMPANY_NAME "SensorTechnology"


#define RS232C_ERROR_PORT_OPEN -1
#define RS232C_ERROR_DATA_SND -1
#define RS232C_ERROR_DATA_RCV -1

//-----------------------------------------------------------------------------
//Error Code
//-----------------------------------------------------------------------------

#define ERROR_CA_RCV_SOF					0xE0000001
#define ERROR_CA_RCV_EOF					0xE0000002
#define ERROR_CA_RCV_CHECKSUM				0xE0000003
#define ERROR_CA_RCV_CMD					0xE0000004
#define ERROR_CA_RCV_BUFFULL				0xE0000005
#define ERROR_CA_RCV_TIMEOUT				0xE0000006

#define ERROR_CA_RCV_OVERRUN				0xE0000008
#define ERROR_CA_RCV_FLAMING				0xE0000009
#define ERROR_CA_RCV_PARITY					0xE000000A
#define ERROR_CA_RCV_DATALEN				0xE000000B
#define ERROR_CA_I2C						0xE0000010



#define ERROR_PC_RCV_SOF					0xE0000081
#define ERROR_PC_RCV_EOF					0xE0000082
#define ERROR_PC_RCV_CHECKSUM				0xE0000083
#define ERROR_PC_RCV_CMD					0xE0000084
#define ERROR_PC_RCV_TIMEOUT				0xE0000086
#define ERROR_PC_RCV_DATALEN				0xE000008B


#define ERROR_INVALID_PORT_NUMBER			0xE0000100

#define ERROR_RCV_CAM_RCV					0xE000010B
#define ERROR_RCV_PERIPHERAL				0xE000010C
#define ERROR_RCV_CAM_BUSY					0xE000010D

#define ERROR_CLAPI_NOT_SUPPORTED			0xE0000200
#define ERROR_CLAPI_BUFFER_TOO_SMALL		0xE0000201
#define ERROR_CLAPI_MANU_DOES_NOT_EXIST		0xE0000202
#define ERROR_CLAPI_PORT_IN_USE				0xE0000203
#define ERROR_CLAPI_TIMEOUT					0xE0000204
#define ERROR_CLAPI_INVALID_INDEX				0xE0000205
#define ERROR_CLAPI_INVALID_REFERENCE			0xE0000206
#define ERROR_CLAPI_ERROR_NOT_FOUND				0xE0000207
#define ERROR_CLAPI_BAUD_RATE_NOT_SUPPORTED		0xE0000208
#define ERROR_CLAPI_OUT_OF_MEMORY				0xE0000209
#define ERROR_CLAPI_UNABLE_TO_LOAD_DLL			0xE0000262
#define ERROR_CLAPI_FUNCTION_NOT_FOUND			0xE0000263








//▼2011/11/07 v2034 [LineSensor]テキスト送信コマンド追加
typedef enum
{
	CR = 0,
	CRLF
} eDelimiter;
//▲2011/11/07 v2034 [LineSensor]テキスト送信コマンド追加

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------

class CRS232C
{
private:
	HANDLE	m_hComm;

	//TCHAR	m_szIniFileSectionName[MAX_PATH];

	//カメラリンクAPI用
//	CCameraLinkAPI	*m_pobjCameraLinkAPI;

	//シリアル設定値
	//▼2009/12/22 v1023 [HCA]SetCommState呼び出し前にGetCommStateを呼び出すように変更
	//DCB				m_sDCB;
	DWORD			m_nBaudRate;
	//▲2009/12/22 v1023 [HCA]SetCommState呼び出し前にGetCommStateを呼び出すように変更

	//▼1.0.0.1048
	TCHAR	m_szParity[64];	//Default:NONE
	double m_dblStopBits;	//Default:1
	INT m_nByteSize;		//Default:8
	//▲1.0.0.1048

	COMMTIMEOUTS	m_sCommTimeouts;

	DWORD	mUpdateDCB(void);
	DWORD	mUpdateTimeOutSetting(void);
	DWORD	mRcvDataCount(PDWORD pdwRcvDataCount);
		

	//▼2008/04/01 v1012 [HCA]デバッグモードを追加
	DWORD	m_dwDebugMode;
	BOOL	m_bOpenedFg;
	//▲2008/04/01 v1012 [HCA]デバッグモードを追加


	//▼2009/12/22 v1023 [HCA]SetCommState呼び出し前にGetCommStateを呼び出すように変更
	void	OutputDCBForDebug(DCB *pDCB);
	//▲2009/12/22 v1023 [HCA]SetCommState呼び出し前にGetCommStateを呼び出すように変更

	//void	mReadIniFile(void);
	//void	mWriteIniFile(void);
	//▼2007/07/13 v1007 [HCA]特殊COMポート名対処
	//ポート番号
	//BYTE	m_ComPortNo;

	//ポート名
	TCHAR	m_szPortName[MAX_PATH];
	//▲2007/07/13 v1007 [HCA]特殊COMポート名対処
public:
	CRS232C(LPCTSTR szSectionName);
	~CRS232C();
	
	//IICバスアクセス
	//virtual DWORD ReadI2C(BYTE byteSlvAdd,BYTE byteStartAdd,BYTE byteEndAdd,PBYTE pbyteData) = 0;
	//virtual DWORD WriteI2C(BYTE byteSlvAdd,BYTE byteStartAdd,BYTE byteEndAdd,PBYTE pbyteData) = 0;

	
	//▼2009/03/04 v1015 [ILA]内部アドレス2バイトEEPROM対応
	//virtual DWORD ReadI2CWord(BYTE byteSlvAdd, WORD wStartAdd, WORD wEndAdd, PBYTE pbyteData){return(ERROR_INVALID_HANDLE);};
	//virtual DWORD WriteI2CWord(BYTE byteSlvAdd, WORD wStartAdd, WORD wEndAdd, PBYTE pbyteData){return(ERROR_INVALID_HANDLE);};
	//▲2009/03/04 v1015 [ILA]内部アドレス2バイトEEPROM対応

	static DWORD GetComPortNameList(LPTSTR szPortNameList, PDWORD pdwPortNameListCharCount, PDWORD pdwPortCount);

	void SetPortName(LPCTSTR szPortName);

	//ポート開閉
	DWORD	Open(void);
	void	Close(void);

	//受送信
	DWORD	Send(DWORD dwBytesToSend,PBYTE pbyteSendData,PDWORD pdwBytesSended);
	DWORD	Rcv(DWORD dwBytesToRcv,PBYTE pbyteRcvData,PDWORD pdwBytesRcved);

	//▼2011/11/07 v2034 [LineSensor]テキスト送信コマンド追加
	DWORD	SendText(LPCSTR szText, PDWORD pdwBytesSended);
	DWORD	RcvText(LPSTR szText, DWORD dwLen, PDWORD pdwBytesRcved);
	DWORD	SetTextDelimiter(eDelimiter del) {m_nDelimiter = del; return(NO_ERROR);};
	DWORD	GetTextDelimiter(eDelimiter *del) { *del = m_nDelimiter; return(NO_ERROR);};
	//▲2011/11/07 v2034 [LineSensor]テキスト送信コマンド追加

	//バッファクリア
	DWORD	ClearRcvBuff(void);

	//ボーレート設定
	DWORD	SetBaudRate(DWORD dwBaudRate);
	DWORD	GetBaudRate(PDWORD pdwBaudRate);
	//▼1.0.0.1048
	//パリティ設定
	DWORD	SetParity(LPCTSTR szParity);
	DWORD	GetParity(LPTSTR szParity, size_t size);

	//ストップビット設定
	DWORD	SetStopBits(double dblStopBits);
	DWORD	GetStopBits(double *pdblStopBits);

	//バイトサイズ設定
	DWORD	SetByteSize(INT nByteSize);
	DWORD	GetByteSize(PINT pnByteSize);
	//▲1.0.0.1048

	//タイムアウト設定
	DWORD	SetTimeout(
		DWORD dwReadIntervalTimeout,
		DWORD dwReadTotalTimeoutConstant,
		DWORD dwReadTotalTimeoutMultiplier,
		DWORD dwWriteTotalTimeoutConstant,
		DWORD dwWriteTotalTimeoutMultiplier);
	DWORD	GetTimeout(
		PDWORD pdwReadIntervalTimeout,
		PDWORD pdwReadTotalTimeoutConstant,
		PDWORD pdwReadTotalTimeoutMultiplier,
		PDWORD pdwWriteTotalTimeoutConstant,
		PDWORD pdwWriteTotalTimeoutMultiplier);

	//▼2007/07/13 v1006 [HCA]カメラリンクAPI使用時のタイムアウト時間が0になっていた不具合を修正
	//▼2011/05/10 v2031 [HCA]カメラリンクAPI使用時のタイムアウト時間を増加
	//DWORD mGetTimeoutMSForCLAPI(DWORD dwNumBytes);
	DWORD mGetTimeoutMSForCLAPI(BOOL IsWritting, DWORD dwNumBytes);
	//▲2011/05/10 v2031 [HCA]カメラリンクAPI使用時のタイムアウト時間を増加
	//▲2007/07/13 v1006 [HCA]カメラリンクAPI使用時のタイムアウト時間が0になっていた不具合を修正

	//▼2008/04/01 v1012 [HCA]デバッグモードを追加
	DWORD	SetDebugMode(DWORD dwDebugMode)
	{
		m_dwDebugMode = dwDebugMode; 
		if(m_dwDebugMode)
		{
			TCHAR szTmp[1024];
			if(m_UseComPort)
			{
				_stprintf_s(szTmp, _countof(szTmp), TEXT("Debug:%s"), m_szPortName);
			}
			/*
			else
			{
				_stprintf(szTmp, TEXT("Debug:%s(%u)"), m_CameraLinkDllFileName, m_CameraLinkSerialIndex);
			}
			*/
			OutputDebugString(szTmp);
		}
		return(NO_ERROR);
	
	};
	DWORD	GetDebugMode(PDWORD pdwDebugMode){*pdwDebugMode = m_dwDebugMode; return(NO_ERROR);};
	//▲2008/04/01 v1012 [HCA]デバッグモードを追加




	//カメラリンクAPI用
//	TCHAR	m_CameraLinkDllFileName[MAX_PATH];
//	DWORD	m_CameraLinkSerialIndex;

	//通常シリアルポートorカメラリンクAPI
	BOOL	m_UseComPort;

	//▼2011/11/07 v2034 [LineSensor]テキスト送信コマンド追加
	eDelimiter	m_nDelimiter;
	//▲2011/11/07 v2034 [LineSensor]テキスト送信コマンド追加

};


#endif