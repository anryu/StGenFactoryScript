
#include "stdafx.h"
#include "RS232C.h"
#include "stdlib.h"
#include "stdio.h"
//▼2009/12/17 v1022 [HCA]デバッグメッセージを追加
//#define DEBUG_PORT_OPEN
//▲2009/12/17 v1022 [HCA]デバッグメッセージを追加
//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
CRS232C::CRS232C(LPCTSTR szSectionName)
{
	_tcscpy_s(m_szPortName, _countof(m_szPortName) , TEXT(""));
	m_UseComPort=true;

	//COMポートファイルハンドル
	m_hComm = INVALID_HANDLE_VALUE;

	//▼2009/12/22 v1023 [HCA]SetCommState呼び出し前にGetCommStateを呼び出すように変更
	/*
	//RS232C通信設定構造体初期化
	memset(&m_sDCB, 0, sizeof(DCB));
	m_sDCB.DCBlength = sizeof(DCB);
	m_sDCB.BaudRate = 9600;
	m_sDCB.fBinary= 1;
	m_sDCB.fParity = 0;
	m_sDCB.ByteSize = 8;
	m_sDCB.StopBits = ONESTOPBIT;
	*/
	m_nBaudRate = CBR_9600;
	//▲2009/12/22 v1023 [HCA]SetCommState呼び出し前にGetCommStateを呼び出すように変更

	//タイムアウト初期値
	//▼2011/05/10 v2031 [HCA]カメラリンクAPI使用時のタイムアウト時間を増加
	m_sCommTimeouts.ReadIntervalTimeout = 500;
	m_sCommTimeouts.ReadTotalTimeoutConstant = 500;
	m_sCommTimeouts.ReadTotalTimeoutMultiplier = 0;
	m_sCommTimeouts.WriteTotalTimeoutConstant = 0;
	m_sCommTimeouts.WriteTotalTimeoutMultiplier = 0;
	//m_sCommTimeouts.ReadIntervalTimeout = 1000;
	//m_sCommTimeouts.ReadTotalTimeoutConstant = 1000;
	//m_sCommTimeouts.ReadTotalTimeoutMultiplier = 2;
	//m_sCommTimeouts.WriteTotalTimeoutConstant = 1000;
	//m_sCommTimeouts.WriteTotalTimeoutMultiplier = 2;
	//▲2011/05/10 v2031 [HCA]カメラリンクAPI使用時のタイムアウト時間を増加
	//▼1.0.0.1021
	//m_sCommTimeouts.ReadIntervalTimeout = 50;
	//m_sCommTimeouts.ReadTotalTimeoutConstant = 50;
	//m_sCommTimeouts.ReadTotalTimeoutMultiplier = 0;
	//m_sCommTimeouts.WriteTotalTimeoutConstant = 0;
	//m_sCommTimeouts.WriteTotalTimeoutMultiplier = 0;
	//▲1.0.0.1021

	//カメラリンクAPI
	//▼2011/01/07 v2028 [HCA]可能な限り関数アドレスを取得するように変更
	//m_pobjCameraLinkAPI = new CCameraLinkAPI;
//	m_pobjCameraLinkAPI = NULL;
	//▲2011/01/07 v2028 [HCA]可能な限り関数アドレスを取得するように変更
//	_tcscpy(m_CameraLinkDllFileName, TEXT(""));
//	m_CameraLinkSerialIndex = 0;

	m_dwDebugMode = 0;
	m_bOpenedFg = FALSE;

	//▼2011/11/07 v2034 [LineSensor]テキスト送信コマンド追加
	m_nDelimiter = CR;
	//▲2011/11/07 v2034 [LineSensor]テキスト送信コマンド追加


	//▼1.0.0.1049
	_tcscpy_s(m_szParity,_countof(m_szParity),_T(""));
	m_dblStopBits = -1.0;	//Default:1
	m_nByteSize = -1;		//Default:8
	//▲1.0.0.1049


}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
CRS232C::~CRS232C()
{
	//ポートを開いていたら閉じる
	Close();

	//カメラリンクAPI
	//if(m_pobjCameraLinkAPI)
	//{
	//	delete m_pobjCameraLinkAPI;
	//	m_pobjCameraLinkAPI = NULL;
	//}
}

//-----------------------------------------------------------------------------
//Port Open
//-----------------------------------------------------------------------------
DWORD CRS232C::Open(void)
{
	DWORD dwErrorCode = NO_ERROR;

	if(m_UseComPort)
	{
		//COMポートを開く
		m_hComm = CreateFile(
				m_szPortName,
				GENERIC_READ | GENERIC_WRITE,
				0,
				NULL,
				OPEN_EXISTING,
				FILE_ATTRIBUTE_NORMAL,
				NULL);
//▼2009/12/17 v1022 [HCA]デバッグメッセージを追加
#ifdef DEBUG_PORT_OPEN
		TCHAR szMsg[1024];
		_stprintf(szMsg, TEXT("0x%p = CreateFile(\"%s\",...)\r\n"), m_hComm, m_szPortName);
		OutputDebugString(szMsg);
#endif //DEBUG_PORT_OPEN
//▲2009/12/17 v1022 [HCA]デバッグメッセージを追加
		if(m_hComm == INVALID_HANDLE_VALUE)
		{
			dwErrorCode = GetLastError();
		}
		else
		{
			do{
				//ボーレート・パリティ・ビット数・ストップビット設定
				dwErrorCode = mUpdateDCB();
				if(dwErrorCode != NO_ERROR)
					break;

				//タイムアウト設定
				dwErrorCode = mUpdateTimeOutSetting();
				if(dwErrorCode != NO_ERROR)
					break;

				//COMポートのエラーをクリア
				DWORD dwErrors = NO_ERROR;
				ClearCommError(m_hComm,&dwErrors,NULL);
			}while(0);

			if(dwErrorCode != NO_ERROR)
			{
				//設定中にエラーが発生したらポートを閉じる
				Close();
			}
		}
	}
	/*
	else
	{
		//ポートオープン
		//▼2011/01/07 v2028 [HCA]可能な限り関数アドレスを取得するように変更
		//dwErrorCode = m_pobjCameraLinkAPI->gdwOpen(m_CameraLinkDllFileName, m_CameraLinkSerialIndex);
		if(m_pobjCameraLinkAPI) delete m_pobjCameraLinkAPI;
		m_pobjCameraLinkAPI = new CCameraLinkAPI(m_CameraLinkDllFileName);
		dwErrorCode = m_pobjCameraLinkAPI->gdwOpen(m_CameraLinkSerialIndex);
		//▲2011/01/07 v2028 [HCA]可能な限り関数アドレスを取得するように変更
		if(dwErrorCode == NO_ERROR)
		{
			do{
				//ボーレートの設定
				//▼2009/12/22 v1023 [HCA]SetCommState呼び出し前にGetCommStateを呼び出すように変更
				//dwErrorCode = m_pobjCameraLinkAPI->gdwSetBaudRate(m_sDCB.BaudRate);
				dwErrorCode = m_pobjCameraLinkAPI->gdwSetBaudRate(m_nBaudRate);
				//▲2009/12/22 v1023 [HCA]SetCommState呼び出し前にGetCommStateを呼び出すように変更
				if(dwErrorCode != NO_ERROR)
					break;
			}while(0);
			if(dwErrorCode != NO_ERROR)
			{
				//設定中にエラーが発生したらポートを閉じる
				Close();
			}

		}
	}
	*/

	if(NO_ERROR == dwErrorCode)	m_bOpenedFg = TRUE;

	return(dwErrorCode);

}

//-----------------------------------------------------------------------------
//Port Close
//-----------------------------------------------------------------------------
void CRS232C::Close(void)
{
	if(m_UseComPort)
	{
		if(m_hComm != INVALID_HANDLE_VALUE)
		{
			CloseHandle(m_hComm);
			m_hComm = INVALID_HANDLE_VALUE;
		}
	}
	/*
	else
	{
		//▼2011/01/07 v2028 [HCA]可能な限り関数アドレスを取得するように変更
		//m_pobjCameraLinkAPI->gdwClose();
		if(m_pobjCameraLinkAPI)
		{
			m_pobjCameraLinkAPI->gdwClose();
			delete m_pobjCameraLinkAPI;
			m_pobjCameraLinkAPI = NULL;
		}
		//▲2011/01/07 v2028 [HCA]可能な限り関数アドレスを取得するように変更
	}
	*/
	//▼2008/04/01 v1012 [HCA]デバッグモードを追加
	if(m_bOpenedFg)
	{
		if(m_dwDebugMode)
		{
			OutputDebugString(TEXT("Close"));
		}
		m_bOpenedFg = FALSE;
	}
	//▲2008/04/01 v1012 [HCA]デバッグモードを追加
}

//▼2009/12/22 v1023 [HCA]SetCommState呼び出し前にGetCommStateを呼び出すように変更
//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void	CRS232C::OutputDCBForDebug(DCB *pDCB)
{
	TCHAR szMsg[1024];

	_stprintf_s(szMsg, _countof(szMsg),
		TEXT("\tBaudRate=%u, ByteSize=%u, DCBlength=%u\r\n"),
		pDCB->BaudRate, pDCB->ByteSize, pDCB->DCBlength);
	OutputDebugString(szMsg);

	_stprintf_s(szMsg, _countof(szMsg),
		TEXT("\tEofChar=0x%02X, ErrorChar=0x%02X, EvtChar=0x%02X\r\n"),
		pDCB->EofChar, pDCB->ErrorChar, pDCB->EvtChar);
	OutputDebugString(szMsg);


	_stprintf_s(szMsg, _countof(szMsg),
		TEXT("\tfAbortOnError=%u, fBinary=%u, fDsrSensitivity=%u, fDtrControl=%u\r\n"),
		pDCB->fAbortOnError, pDCB->fBinary, pDCB->fDsrSensitivity, pDCB->fDtrControl);
	OutputDebugString(szMsg);


	_stprintf_s(szMsg, _countof(szMsg),
		TEXT("\tfDummy2=%u, fErrorChar=%u, fInX=%u\r\n"),
		pDCB->fDummy2, pDCB->fErrorChar, pDCB->fInX);
	OutputDebugString(szMsg);
	
	_stprintf_s(szMsg, _countof(szMsg),
		TEXT("\tfNull=%u, fOutX=%u, fOutxCtsFlow=%u, fOutxDsrFlow=%u\r\n"),
		pDCB->fNull, pDCB->fOutX, pDCB->fOutxCtsFlow, pDCB->fOutxDsrFlow);
	OutputDebugString(szMsg);

	_stprintf_s(szMsg, _countof(szMsg),
		TEXT("\tfParity=%u, fRtsControl=%u, fTXContinueOnXoff=%u\r\n"),
		pDCB->fParity, pDCB->fRtsControl, pDCB->fTXContinueOnXoff);
	OutputDebugString(szMsg);

	_stprintf_s(szMsg, _countof(szMsg),
		TEXT("\tParity=%u, StopBits=%u, wReserved=%u, wReserved1=%u\r\n"),
		pDCB->Parity, pDCB->StopBits, pDCB->wReserved, pDCB->wReserved1);
	OutputDebugString(szMsg);

	_stprintf_s(szMsg, _countof(szMsg),
		TEXT("\tXoffChar=0x%02X, XoffLim=%u, XonChar=0x%02X, XonLim=%u\r\n"),
		pDCB->XoffChar, pDCB->XoffLim, pDCB->XonChar, pDCB->XonLim);
	OutputDebugString(szMsg);
}
//▲2009/12/22 v1023 [HCA]SetCommState呼び出し前にGetCommStateを呼び出すように変更
//-----------------------------------------------------------------------------
//UpdateDCB
//-----------------------------------------------------------------------------
DWORD CRS232C::mUpdateDCB(void)
{
	DWORD	dwErrorCode = NO_ERROR;

	if(m_UseComPort)
	{
		if(m_hComm != INVALID_HANDLE_VALUE)
		{

			//▼2009/12/22 v1023 [HCA]SetCommState呼び出し前にGetCommStateを呼び出すように変更
			do{
				DCB	sDCB;
				BOOL bReval = TRUE;

				sDCB.DCBlength = sizeof(DCB);
				bReval = GetCommState(m_hComm, &sDCB);
//▼2009/12/17 v1022 [HCA]デバッグメッセージを追加
#ifdef DEBUG_PORT_OPEN
				TCHAR szMsg[1024];
				_stprintf(szMsg, TEXT("GetCommState()\r\n"));
				OutputDebugString(szMsg);
				OutputDCBForDebug(&sDCB);
#endif //DEBUG_PORT_OPEN
//▲2009/12/17 v1022 [HCA]デバッグメッセージを追加
				if(!bReval)
				{
					dwErrorCode = GetLastError();
//▼2009/12/17 v1022 [HCA]デバッグメッセージを追加
#ifdef DEBUG_PORT_OPEN
					_stprintf(szMsg, TEXT("ErrorCode = 0x%08X\r\n"), dwErrorCode);
					OutputDebugString(szMsg);
#endif //DEBUG_PORT_OPEN
//▲2009/12/17 v1022 [HCA]デバッグメッセージを追加
					break;
				}

				sDCB.BaudRate = m_nBaudRate;
				sDCB.fBinary = 1;
				sDCB.fParity = 0;
				sDCB.fOutxCtsFlow = 0;
				sDCB.fOutxDsrFlow = 0;
				sDCB.fDtrControl = DTR_CONTROL_DISABLE;
				sDCB.fDsrSensitivity = 0;
				sDCB.fTXContinueOnXoff = 0;
				sDCB.fOutX = 0;
				sDCB.fInX = 0;
				sDCB.fErrorChar = 0;
				sDCB.fNull = 0;
				sDCB.fRtsControl = 0;
				sDCB.fAbortOnError = 0;
				sDCB.ByteSize = 8;
				//▼2009/12/22 v1024 [HCA]パリティ明示的に0に設定するように変更
				sDCB.Parity = NOPARITY;
				//▲2009/12/22 v1024 [HCA]パリティ明示的に0に設定するように変更
				sDCB.StopBits = ONESTOPBIT;
				//▼1.0.0.1048
				if( _tcscmp(m_szParity,_T("EVEN") )==0) 
				{
					sDCB.Parity = EVENPARITY;
				}
				else
				if( _tcscmp(m_szParity,_T("ODD") )==0)
				{
					sDCB.Parity = ODDPARITY;
				}
			
				if( m_dblStopBits==2.0 )
				{
					sDCB.StopBits = TWOSTOPBITS;
				}
				else
				if( m_dblStopBits==1.5 )
				{
					sDCB.StopBits = ONE5STOPBITS;
				}
				if( m_nByteSize>0 )
					sDCB.ByteSize = m_nByteSize;
				//▲1.0.0.1048


				bReval = SetCommState(m_hComm, &sDCB);
//▼2009/12/17 v1022 [HCA]デバッグメッセージを追加
#ifdef DEBUG_PORT_OPEN
				_stprintf(szMsg, TEXT("SetCommState()\r\n"));
				OutputDebugString(szMsg);
				OutputDCBForDebug(&sDCB);
#endif //DEBUG_PORT_OPEN
//▲2009/12/17 v1022 [HCA]デバッグメッセージを追加
				if(!bReval)
				{
					dwErrorCode = GetLastError();
//▼2009/12/17 v1022 [HCA]デバッグメッセージを追加
#ifdef DEBUG_PORT_OPEN
					_stprintf(szMsg, TEXT("ErrorCode = 0x%08X\r\n"), dwErrorCode);
					OutputDebugString(szMsg);
#endif //DEBUG_PORT_OPEN
//▲2009/12/17 v1022 [HCA]デバッグメッセージを追加
					break;
				}
			}while(0);

			/*
			if(!SetCommState(m_hComm, &m_sDCB))
				dwErrorCode = GetLastError();
//▼2009/12/17 v1022 [HCA]デバッグメッセージを追加
#ifdef DEBUG_PORT_OPEN
			TCHAR szMsg[1024];
			_stprintf(szMsg, TEXT("SetCommState(BaudRate=%d, fBinary=%d, fParity=%d, ByteSize=%d, StopBits=%d)\r\n"), 
				m_sDCB.BaudRate, 
				m_sDCB.fBinary,
				m_sDCB.fParity,
				m_sDCB.ByteSize,
				m_sDCB.StopBits);
			OutputDebugString(szMsg);

			if(NO_ERROR != dwErrorCode)
			{
				_stprintf(szMsg, TEXT("ErrorCode = 0x%08X\r\n"), dwErrorCode);
				OutputDebugString(szMsg);
			}
#endif //DEBUG_PORT_OPEN
//▲2009/12/17 v1022 [HCA]デバッグメッセージを追加
			*/
			//▲2009/12/22 v1023 [HCA]SetCommState呼び出し前にGetCommStateを呼び出すように変更

		}
	}
	/*
	else
	{
		//ボーレートの設定
		//▼2009/12/22 v1023 [HCA]SetCommState呼び出し前にGetCommStateを呼び出すように変更
		//dwErrorCode = m_pobjCameraLinkAPI->gdwSetBaudRate(m_sDCB.BaudRate);
		dwErrorCode = m_pobjCameraLinkAPI->gdwSetBaudRate(m_nBaudRate);
		//▲2009/12/22 v1023 [HCA]SetCommState呼び出し前にGetCommStateを呼び出すように変更
	}
	*/
	return(dwErrorCode);
}
//-----------------------------------------------------------------------------
//ボーレート設定
//-----------------------------------------------------------------------------
DWORD	CRS232C::SetBaudRate(DWORD dwBaudRate)
{
	//▼2008/04/01 v1012 [HCA]デバッグモードを追加
	if(m_dwDebugMode)
	{
		TCHAR szTmp[1024];
		_stprintf_s(szTmp, _countof(szTmp), TEXT("Set %u bps"), dwBaudRate);
		OutputDebugString(szTmp);
	}
	//▲2008/04/01 v1012 [HCA]デバッグモードを追加
	//▼2009/12/22 v1023 [HCA]SetCommState呼び出し前にGetCommStateを呼び出すように変更
	//m_sDCB.BaudRate = dwBaudRate;
	m_nBaudRate = dwBaudRate;
	//▲2009/12/22 v1023 [HCA]SetCommState呼び出し前にGetCommStateを呼び出すように変更
	return(mUpdateDCB());
}
//-----------------------------------------------------------------------------
//ボーレート設定
//-----------------------------------------------------------------------------
DWORD	CRS232C::GetBaudRate(PDWORD pdwBaudRate)
{
	//▼2009/12/22 v1023 [HCA]SetCommState呼び出し前にGetCommStateを呼び出すように変更
	//*pdwBaudRate = m_sDCB.BaudRate;
	*pdwBaudRate = m_nBaudRate;
	//▲2009/12/22 v1023 [HCA]SetCommState呼び出し前にGetCommStateを呼び出すように変更
	return(NO_ERROR);
}
//-----------------------------------------------------------------------------
//タイムアウト設定更新
//-----------------------------------------------------------------------------
//▼2009/12/17 v1022 [HCA]デバッグメッセージを追加
DWORD CRS232C::mUpdateTimeOutSetting(void)
{
#ifdef DEBUG_PORT_OPEN
	TCHAR szMsg[1024];
#endif //DEBUG_PORT_OPEN
	if(!m_UseComPort)
	{
		return(NO_ERROR);
	}
	DWORD dwErrorCode = NO_ERROR;
	do{
#ifdef DEBUG_PORT_OPEN
		_stprintf(szMsg, TEXT("SetCommTimeouts(ReadIntervalTimeout=%d, ReadTotalTimeoutConstant=%d, ReadTotalTimeoutMultiplier=%d, WriteTotalTimeoutConstant=%d, WriteTotalTimeoutMultiplier=%d)\r\n"), 
			m_sCommTimeouts.ReadIntervalTimeout,
			m_sCommTimeouts.ReadTotalTimeoutConstant,
			m_sCommTimeouts.ReadTotalTimeoutMultiplier,
			m_sCommTimeouts.WriteTotalTimeoutConstant,
			m_sCommTimeouts.WriteTotalTimeoutMultiplier);
		OutputDebugString(szMsg);
#endif //DEBUG_PORT_OPEN
		if(!SetCommTimeouts(m_hComm,&m_sCommTimeouts))
		{
			dwErrorCode = GetLastError();
			break;
		}
		
#ifdef DEBUG_PORT_OPEN
		_stprintf(szMsg, TEXT("SetCommMask(0)\r\n"));
		OutputDebugString(szMsg);
#endif //DEBUG_PORT_OPEN
		if(!SetCommMask(m_hComm,0))
		{
			dwErrorCode = GetLastError();
			break;
		}
#ifdef DEBUG_PORT_OPEN
		_stprintf(szMsg, TEXT("SetupComm(1024, 1024)\r\n"));
		OutputDebugString(szMsg);
#endif //DEBUG_PORT_OPEN
		if(!SetupComm(
			  m_hComm,     // handle to communications device
			  1024,  // size of input buffer
			  1024  // size of output buffer
			))
		{
			dwErrorCode = GetLastError();
			break;
		}
	}while(0);

#ifdef DEBUG_PORT_OPEN
	if(NO_ERROR != dwErrorCode)
	{
		_stprintf(szMsg, TEXT("ErrorCode = 0x%08X\r\n"), dwErrorCode);
		OutputDebugString(szMsg);
	}
#endif //DEBUG_PORT_OPEN
	return(dwErrorCode);
}
/*
DWORD CRS232C::mUpdateTimeOutSetting(void)
{
	//▼2009/03/17 v1016 [HCA]タイムアウト設定はCOMポート使用時のみ
	if(!m_UseComPort)
	{
		return(NO_ERROR);
	}
	//▲2009/03/17 v1016 [HCA]タイムアウト設定はCOMポート使用時のみ
	DWORD dwErrorCode = NO_ERROR;
	do{
		if(!SetCommTimeouts(m_hComm,&m_sCommTimeouts))
		{
			dwErrorCode = GetLastError();
			break;
		}
		if(!SetCommMask(m_hComm,0))
		{
			dwErrorCode = GetLastError();
			break;
		}
		if(!SetupComm(
			  m_hComm,     // handle to communications device
			  1024,  // size of input buffer
			  1024  // size of output buffer
			))
		{
			dwErrorCode = GetLastError();
			break;
		}
	}while(0);

	return(dwErrorCode);
}
*/
//▲2009/12/17 v1022 [HCA]デバッグメッセージを追加
//-----------------------------------------------------------------------------
//タイムアウト設定
//-----------------------------------------------------------------------------
DWORD CRS232C::SetTimeout(
		DWORD dwReadIntervalTimeout,
		DWORD dwReadTotalTimeoutConstant,
		DWORD dwReadTotalTimeoutMultiplier,
		DWORD dwWriteTotalTimeoutConstant,
		DWORD dwWriteTotalTimeoutMultiplier)
{
	m_sCommTimeouts.ReadIntervalTimeout = dwReadIntervalTimeout;
	m_sCommTimeouts.ReadTotalTimeoutConstant = dwReadTotalTimeoutConstant;
	m_sCommTimeouts.ReadTotalTimeoutMultiplier = dwReadTotalTimeoutMultiplier;
	m_sCommTimeouts.WriteTotalTimeoutConstant = dwWriteTotalTimeoutConstant;
	m_sCommTimeouts.WriteTotalTimeoutMultiplier = dwWriteTotalTimeoutMultiplier;

	return(mUpdateTimeOutSetting());
}
//-----------------------------------------------------------------------------
//タイムアウト設定
//-----------------------------------------------------------------------------
DWORD CRS232C::GetTimeout(
		PDWORD pdwReadIntervalTimeout,
		PDWORD pdwReadTotalTimeoutConstant,
		PDWORD pdwReadTotalTimeoutMultiplier,
		PDWORD pdwWriteTotalTimeoutConstant,
		PDWORD pdwWriteTotalTimeoutMultiplier)
{
	*pdwReadIntervalTimeout = m_sCommTimeouts.ReadIntervalTimeout;
	*pdwReadTotalTimeoutConstant = m_sCommTimeouts.ReadTotalTimeoutConstant;
	*pdwReadTotalTimeoutMultiplier = m_sCommTimeouts.ReadTotalTimeoutMultiplier;
	*pdwWriteTotalTimeoutConstant = m_sCommTimeouts.WriteTotalTimeoutConstant;
	*pdwWriteTotalTimeoutMultiplier = m_sCommTimeouts.WriteTotalTimeoutMultiplier;

	return(NO_ERROR);
}
//▼2007/07/13 v1006 [HCA]カメラリンクAPI使用時のタイムアウト時間が0になっていた不具合を修正
//▼2011/05/10 v2031 [HCA]カメラリンクAPI使用時のタイムアウト時間を増加
//-----------------------------------------------------------------------------
//タイムアウト設定
//-----------------------------------------------------------------------------
DWORD CRS232C::mGetTimeoutMSForCLAPI(BOOL IsWritting, DWORD dwNumBytes)
{
	DWORD dwTimeout = 0;

	if(IsWritting)
	{
		dwTimeout = m_sCommTimeouts.WriteTotalTimeoutConstant
				+ m_sCommTimeouts.WriteTotalTimeoutMultiplier * dwNumBytes;
	}
	else
	{
		dwTimeout = m_sCommTimeouts.ReadTotalTimeoutConstant
				+ m_sCommTimeouts.ReadIntervalTimeout
				+ m_sCommTimeouts.ReadTotalTimeoutMultiplier * dwNumBytes;
	}

	if(m_dwDebugMode)
	{
		TCHAR szText[256];
		_stprintf_s(szText, _countof(szText), TEXT("Timeout setting:%u\r\n"), dwTimeout);
		OutputDebugString(szText);
	}

	return(dwTimeout);
}
/*

DWORD CRS232C::mGetTimeoutMSForCLAPI(DWORD dwNumBytes)
{
	DWORD dwTimeout = 0;
	//▼2009/12/22 v1023 [HCA]SetCommState呼び出し前にGetCommStateを呼び出すように変更
	//DWORD dwBaudRaito = m_sDCB.BaudRate;
	DWORD dwBaudRaito = m_nBaudRate;
	//▲2009/12/22 v1023 [HCA]SetCommState呼び出し前にGetCommStateを呼び出すように変更

	if(0 != dwBaudRaito)
		dwTimeout = (12 * dwNumBytes + dwBaudRaito - 1) / dwBaudRaito;

	dwTimeout += 500;
	return(dwTimeout);
}
*/
//▲2011/05/10 v2031 [HCA]カメラリンクAPI使用時のタイムアウト時間を増加
//▲2007/07/13 v1006 [HCA]カメラリンクAPI使用時のタイムアウト時間が0になっていた不具合を修正
//-----------------------------------------------------------------------------
//データ送信
//-----------------------------------------------------------------------------
DWORD CRS232C::Send(DWORD dwBytesToSend, PBYTE pbyteSendData, PDWORD pdwBytesSended)
{
	DWORD			dwErrorCode = NO_ERROR;

	*pdwBytesSended = 0;

	//▼2011/05/10 v2031 [HCA]カメラリンクAPI使用時のタイムアウト時間を増加
	DWORD dwTimeout = 0;
	
	if(m_UseComPort)
	{
	}
	else
	{
		dwTimeout = mGetTimeoutMSForCLAPI(TRUE, dwBytesToSend);
	}
	//▲2011/05/10 v2031 [HCA]カメラリンクAPI使用時のタイムアウト時間を増加


	//▼2008/04/01 v1012 [HCA]デバッグモードを追加
	if(m_dwDebugMode)
	{
		if(0 < dwBytesToSend)
		{
			size_t bufferSize = dwBytesToSend * 4 + 256;
			TCHAR	*szTmp = new TCHAR[bufferSize];
			_tcscpy_s(szTmp, bufferSize, TEXT("SND:"));
			for(DWORD dwPos = 0; dwPos < dwBytesToSend; dwPos++)
			{
				TCHAR szTmp2[16];
				if(0 < dwPos)	_stprintf_s(szTmp2, _countof(szTmp2), TEXT(",%02X"), pbyteSendData[dwPos]);
				else			_stprintf_s(szTmp2, _countof(szTmp2), TEXT("%02X"), pbyteSendData[dwPos]);
				_tcscat_s(szTmp, bufferSize, szTmp2);
			}
			OutputDebugString(szTmp);
			delete[] szTmp;
		}
	}
	//▲2008/04/01 v1012 [HCA]デバッグモードを追加
	if(m_UseComPort)
	{
		//指定されたデータを送信
		if(!WriteFile(m_hComm,pbyteSendData,dwBytesToSend,pdwBytesSended,NULL))
		{
			dwErrorCode = GetLastError();
		}
	}
	/*
	else
	{
		*pdwBytesSended = dwBytesToSend;
		//▼2007/07/13 v1006 [HCA]カメラリンクAPI使用時のタイムアウト時間が0になっていた不具合を修正
		//dwErrorCode = m_pobjCameraLinkAPI->gdwWrite(pbyteSendData,pdwBytesSended,m_sCommTimeouts.WriteTotalTimeoutConstant);
		//▼2011/05/10 v2031 [HCA]カメラリンクAPI使用時のタイムアウト時間を増加
		//dwErrorCode = m_pobjCameraLinkAPI->gdwWrite(pbyteSendData, pdwBytesSended, mGetTimeoutMSForCLAPI(dwBytesToSend));
		dwErrorCode = m_pobjCameraLinkAPI->gdwWrite(pbyteSendData, pdwBytesSended, dwTimeout);
		//▲2011/05/10 v2031 [HCA]カメラリンクAPI使用時のタイムアウト時間を増加
		//▲2007/07/13 v1006 [HCA]カメラリンクAPI使用時のタイムアウト時間が0になっていた不具合を修正

	}
	*/
	if(dwErrorCode == NO_ERROR)
	{
		if(dwBytesToSend != *pdwBytesSended)
		{
			dwErrorCode = ERROR_WRITE_FAULT;
		}
	}

	return(dwErrorCode);
}
//-----------------------------------------------------------------------------
//受信バッファのデータ数を取得
//-----------------------------------------------------------------------------
DWORD CRS232C::mRcvDataCount(PDWORD pdwRcvDataCount)
{
	DWORD dwErrors;
	COMSTAT ComStat;
	DWORD	dwErrorCode = NO_ERROR;

	if(!ClearCommError(m_hComm,&dwErrors,&ComStat))
	{
		dwErrorCode = GetLastError();
	}
	else
	{
		*pdwRcvDataCount = ComStat.cbInQue;
	}

	return(dwErrorCode);

}
//-----------------------------------------------------------------------------
//データ受信
//-----------------------------------------------------------------------------
DWORD CRS232C::Rcv(DWORD dwBytesToRcv,PBYTE pbyteRcvData,PDWORD pdwBytesRcved)
{
	DWORD	dwErrorCode = NO_ERROR;

	*pdwBytesRcved = 0;

	if(m_UseComPort)
	{
		//データを受信
		if(!ReadFile(m_hComm,pbyteRcvData,dwBytesToRcv,pdwBytesRcved,NULL))
		{
			dwErrorCode = GetLastError();
		}
		else if(*pdwBytesRcved == 0)
		{
			//関数の呼び出しは成功したが受信できなかった場合はタイムアウト
			//dwErrorCode = ERROR_TIMEOUT;
			dwErrorCode = ERROR_PC_RCV_TIMEOUT;
		}
	}
	/*
	else
	{
		*pdwBytesRcved = dwBytesToRcv;
		//▼2007/07/13 v1006 [HCA]カメラリンクAPI使用時のタイムアウト時間が0になっていた不具合を修正
		//dwErrorCode = m_pobjCameraLinkAPI->gdwRead(pbyteRcvData,pdwBytesRcved,m_sCommTimeouts.ReadTotalTimeoutConstant);
		//▼2011/05/10 v2031 [HCA]カメラリンクAPI使用時のタイムアウト時間を増加
		//dwErrorCode = m_pobjCameraLinkAPI->gdwRead(pbyteRcvData,pdwBytesRcved, mGetTimeoutMSForCLAPI(dwBytesToRcv));
		DWORD dwTimeout = mGetTimeoutMSForCLAPI(FALSE, dwBytesToRcv);
		dwErrorCode = m_pobjCameraLinkAPI->gdwRead(pbyteRcvData,pdwBytesRcved, dwTimeout);
		//▲2011/05/10 v2031 [HCA]カメラリンクAPI使用時のタイムアウト時間を増加
		//▲2007/07/13 v1006 [HCA]カメラリンクAPI使用時のタイムアウト時間が0になっていた不具合を修正
	}
	*/
	
	//▼2008/04/01 v1012 [HCA]デバッグモードを追加
	if(m_dwDebugMode)
	{
		if(0 < *pdwBytesRcved)
		{
			size_t bufferSize = *pdwBytesRcved * 4 + 256;
			TCHAR	*szTmp = new TCHAR[bufferSize];
			_tcscpy_s(szTmp, bufferSize, TEXT("RCV:"));
			for(DWORD dwPos = 0; dwPos < *pdwBytesRcved; dwPos++)
			{
				TCHAR szTmp2[16];
				if(0 < dwPos)	_stprintf_s(szTmp2, _countof(szTmp2), TEXT(",%02X"), pbyteRcvData[dwPos]);
				else			_stprintf_s(szTmp2, _countof(szTmp2), TEXT("%02X"), pbyteRcvData[dwPos]);
				_tcscat_s(szTmp, bufferSize, szTmp2);
			}
			OutputDebugString(szTmp);

#ifdef _DEBUG
			_stprintf_s(szTmp, bufferSize, TEXT("Expect:%d, Result:%d"), dwBytesToRcv, *pdwBytesRcved);
			OutputDebugString(szTmp);
#endif //_DEBUG

			delete[] szTmp;
		}
	}
	//▲2008/04/01 v1012 [HCA]デバッグモードを追加
	return(dwErrorCode);
}

//-----------------------------------------------------------------------------
//受信バッファクリア
//-----------------------------------------------------------------------------
DWORD CRS232C::ClearRcvBuff(void)
{
	DWORD	dwErrorCode = NO_ERROR;
	
	do{
		DWORD	dwRcvCount = 1;
		DWORD	dwRcvedLen = 0;


		if(m_UseComPort)
		{
			//受信バッファ内のデータ数を取得
			dwErrorCode = mRcvDataCount(&dwRcvCount);
		}
		/*
		else
		{
			//受信バッファ内のデータ数を取得
			dwErrorCode = m_pobjCameraLinkAPI->gdwGetNumBytesAvail(&dwRcvCount);
		}
		*/
		//受信バッファが空だったらループを抜ける
		if(dwRcvCount == 0) break;

		//全データを受信する
		BYTE	*pbyteDummy = new BYTE[dwRcvCount];
		dwErrorCode = Rcv(dwRcvCount, pbyteDummy, &dwRcvedLen);
		delete[] pbyteDummy;
		if(dwErrorCode != NO_ERROR) break;

		//受信できなかったらループを抜ける
		if(dwRcvedLen == 0) break;

		//▼2009/06/09 v1017 [ILA]カメラが9600bpsのとき38400bpsで通信するとゴミが残る不具合対処
		Sleep(10);
#ifdef _DEBUG
		OutputDebugString(TEXT("ClearSleep(10)\r\n"));
#endif //_DEBUG
		//▲2009/06/09 v1017 [ILA]カメラが9600bpsのとき38400bpsで通信するとゴミが残る不具合対処
	}while(1);
	return(NO_ERROR);
}
//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
DWORD CRS232C::GetComPortNameList(LPTSTR szPortNameList, PDWORD pdwPortNameListCharCount, PDWORD pdwPortCount)
{
	HKEY	hKey;
	
	DWORD	dwNeedSize = 1;
	DWORD	dwValueIndex = 0;
	DWORD dwErrorCode = NO_ERROR;
	do
	{
		//レジストリキーをオープン
		dwErrorCode = RegOpenKeyEx(
						HKEY_LOCAL_MACHINE, 
						TEXT("HARDWARE\\DEVICEMAP\\SERIALCOMM"), 
						0, 
						KEY_QUERY_VALUE, &hKey);
		if(NO_ERROR != dwErrorCode)	break;;

		_tcscpy(szPortNameList, TEXT(""));
		*pdwPortCount = 0;

		do{
			DWORD dwType;
			TCHAR	szKeyName[MAX_PATH];
			DWORD	dwKeyNameSize = sizeof(szKeyName)/sizeof(TCHAR);
			BYTE	pbyteData[256];
			DWORD	dwDataSize = sizeof(pbyteData);
			DWORD	dwRegEnumValueReval = 
				RegEnumValue(
					hKey, dwValueIndex,
					szKeyName, &dwKeyNameSize,
					NULL,
					&dwType, 
					pbyteData,&dwDataSize);
			
			if(dwRegEnumValueReval) break;

			//文字列の場合のみ取得
			if(REG_SZ == dwType)
			{
				//重複していないかどうかを確認
				if(NULL == _tcsstr(szPortNameList, (LPCTSTR)pbyteData))
				{
					dwNeedSize += _tcslen((LPCTSTR)pbyteData) + 1;
					if(dwNeedSize <= *pdwPortNameListCharCount)
					{
						if(_tcslen(szPortNameList))
							_tcscat(szPortNameList, TEXT("\\"));
						_tcscat(szPortNameList, (LPCTSTR)pbyteData);
					}
					(*pdwPortCount)++;
				}
			}
			dwValueIndex++;
		}while(1);

		RegCloseKey(hKey);
		*pdwPortNameListCharCount = dwNeedSize;

	}while(FALSE);
	return(dwErrorCode);
}
//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void CRS232C::SetPortName(LPCTSTR szPortName)
{
	OSVERSIONINFO ver;
	ver.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	GetVersionEx(&ver);
	if(ver.dwPlatformId == 1)
	{
		//win98用
		_stprintf_s(m_szPortName, _countof(m_szPortName), _T("%s"), szPortName);
	}
	else
	{
		//winNT用
		_stprintf_s(m_szPortName, _countof(m_szPortName), _T("\\\\.\\%s"), szPortName);
	}
}


	//DWORD	Send(DWORD dwBytesToSend,PBYTE pbyteSendData,PDWORD pdwBytesSended);
	//DWORD	Rcv(DWORD dwBytesToRcv,PBYTE pbyteRcvData,PDWORD pdwBytesRcved);

//▼2011/11/07 v2034 [LineSensor]テキスト送信コマンド追加
//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
DWORD	CRS232C::SendText(LPCSTR szText, PDWORD pdwBytesSended)
{
	DWORD dwError = NO_ERROR;

	dwError = Send(strlen(szText), (PBYTE)szText, pdwBytesSended);
	if(dwError == NO_ERROR)
	{
		DWORD dwBytesSended = 0;
		if(m_nDelimiter == CR)
		{
			BYTE pbyteData[] = {0x0D};
			dwError = Send(1, pbyteData, &dwBytesSended);
		}
		else
		{
			BYTE pbyteData[] = {0x0D, 0x0A};
			dwError = Send(1, pbyteData, &dwBytesSended);
		}
	}
	return(dwError);
}
//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
DWORD	CRS232C::RcvText(LPSTR szText, DWORD dwLen, PDWORD pdwBytesRcved)
{
	DWORD dwError = NO_ERROR;
	DWORD dwPos = 0;
	PBYTE pbyteDest = (PBYTE)szText;

	for(dwPos = 0; dwPos < dwLen - 1; dwPos++)
	{
		DWORD dwBytesRcved = 0;
		BYTE byteCur = 0;
		dwError = Rcv(1, &byteCur, &dwBytesRcved);
		if(dwError != NO_ERROR) break;
		
		if(byteCur == 0x0D)
		{
			if(m_nDelimiter == CR) break;
		}
		else if(byteCur == 0x0A)
		{
			if(m_nDelimiter == CRLF)
			{
				dwPos--;
			}
			break;
		}
		else
		{
			*pbyteDest++ = byteCur;
		}
	}

	if(dwPos < dwLen)
	{
		*pdwBytesRcved = dwPos;
	}
	else
	{
		*pdwBytesRcved = dwLen - 1;
	}
	*pbyteDest++ = 0;

	return(dwError);
}
//▲2011/11/07 v2034 [LineSensor]テキスト送信コマンド追加

//▼1.0.0.1048
//-----------------------------------------------------------------------------
//パリティ設定
//-----------------------------------------------------------------------------
DWORD	CRS232C::SetParity(LPCTSTR szParity)
{
	_tcscpy_s(m_szParity,_countof(m_szParity),szParity);
	return(mUpdateDCB());
}
//-----------------------------------------------------------------------------
//パリティ取得
//-----------------------------------------------------------------------------
DWORD	CRS232C::GetParity(LPTSTR szParity, size_t size)
{
	_tcscpy_s(szParity,size,m_szParity);
	return(NO_ERROR);
}

//-----------------------------------------------------------------------------
//ストップビット設定
//-----------------------------------------------------------------------------
DWORD	CRS232C::SetStopBits(double dblStopBits)
{
	m_dblStopBits = dblStopBits;
	return(mUpdateDCB());
}
//-----------------------------------------------------------------------------
//ストップビット取得
//-----------------------------------------------------------------------------
DWORD	CRS232C::GetStopBits(double *pdblStopBits)
{
	*pdblStopBits = m_dblStopBits;
	return(NO_ERROR);
}

//-----------------------------------------------------------------------------
//バイトサイズ設定
//-----------------------------------------------------------------------------
DWORD	CRS232C::SetByteSize(INT nByteSize)
{
	m_nByteSize = nByteSize;
	return(mUpdateDCB());
}
//-----------------------------------------------------------------------------
//バイトサイズ取得
//-----------------------------------------------------------------------------
DWORD	CRS232C::GetByteSize(PINT pnByteSize)
{
	*pnByteSize = m_nByteSize;
	return(NO_ERROR);
}
//▲1.0.0.1048

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------

