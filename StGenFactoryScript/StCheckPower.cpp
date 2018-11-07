#include "StdAfx.h"
#include "StCheckPower.h"
//▼1.0.0.1054
#include "StDeviceGenICam.h"
//▲1.0.0.1054


StCheckPower::StCheckPower(void)
{
	Initialize();
}

StCheckPower::StCheckPower(LPCTSTR szClassName, LPCTSTR szVarName, StDeviceBase *aDeviceBase, void *aDialog)
	:StCheckBase( szClassName, szVarName, aDeviceBase, aDialog )
{
	Initialize();
}

StCheckPower::~StCheckPower(void)
{
}

void StCheckPower::Initialize(void)
{
	//▼1.0.0.1052
	//m_nCurrentMax = 50;
	//m_nCurrentMin = 0;
	m_dblCurrentMax = 50.0;
	m_dblCurrentMin = 0.0;
	//▲1.0.0.1052


	//▼1.0.0.1048
	//m_nResultCurrent = 0;
	m_dblResultCurrent = 0.0;
	//▲1.0.0.1048
	m_pSerialComm = NULL;
	m_pTexioControl = NULL;
	m_nMode = 0;

	//▼1.0.0.1054
	m_InterfaceGenICam = NULL;
	//▲1.0.0.1054

}
//-------------------------------------------------------------------------------
//検査実行
//-------------------------------------------------------------------------------
BOOL StCheckPower::Execute(void)
{
	BOOL bReval = FALSE;
	m_iJudge = 2;	//2:処理中

	//治具から電流取得
	//▼1.0.0.1048
	//bReval = GetCurrentFromComm( &m_nResultCurrent );
	bReval = GetCurrentFromComm( &m_dblResultCurrent );
	//▲1.0.0.1048
	if( bReval )
	{
		//▼1.0.0.1048
		//if( m_nCurrentMax<m_nResultCurrent || m_nCurrentMin>m_nResultCurrent )
		//▼1.0.0.1052
		//if( m_nCurrentMax<m_dblResultCurrent || m_nCurrentMin>m_dblResultCurrent )
		if( m_dblCurrentMax<m_dblResultCurrent || m_dblCurrentMin>m_dblResultCurrent )
		//▲1.0.0.1052
		//▲1.0.0.1048
		{
			m_iJudge=0;
		}
		if( m_iJudge==2 ) m_iJudge = 1;	//1:OK
	}
	else
	{
		m_iJudge = 0;	//0:NG
	}
	return bReval;
}

BOOL StCheckPower::function(LPCTSTR szFunc, LPCTSTR szArgument)
{
	BOOL bReval = FALSE;
	CString szFunction(szFunc);
	if( szFunction=="SetCurrentMax" )
	{
		//▼1.0.0.1052
		//m_nCurrentMax = _ttoi(szArgument);
		m_dblCurrentMax = _ttof(szArgument);
		//▲1.0.0.1052
		bReval = TRUE;
	}
	else
	if( szFunction=="SetCurrentMin" )
	{
		//▼1.0.0.1052
		//m_nCurrentMin = _ttoi(szArgument);
		m_dblCurrentMin = _ttof(szArgument);
		//▲1.0.0.1052
		bReval = TRUE;
	}
	else
	if( szFunction=="SetMode" )
	{
		m_nMode = _ttoi(szArgument);
		bReval = TRUE;
	}
	//▼1.0.0.1052
	else
	if( szFunction=="SetCommand" || szFunction=="Command" )
	{
		szCommand = szArgument;
		bReval = TRUE;
	}
	//▲1.0.0.1052
	return bReval;

}

//消費電流取得
//▼1.0.0.1048
//BOOL StCheckPower::GetCurrentFromComm(INT *piCurrent)
BOOL StCheckPower::GetCurrentFromComm(double *pdblCurrent)
//▲1.0.0.1048
{
	BOOL bReval = FALSE;
	
	//▼1.0.0.1048
	//if( m_nMode==0 )
	//{
	//	bReval = GetCurrentFromLineComm(piCurrent);
	//}
	//else
	//if( m_nMode==1 )
	//{
	//	bReval = GetCurrentFromPoEComm(piCurrent);
	//}
	//else
	//if( m_nMode==2 )
	//{
	//	bReval = GetCurrentFromTexio(piCurrent);
	//}
	INT nCurrent=0;
	if( m_nMode==0 )
	{
		bReval = GetCurrentFromLineComm(&nCurrent);
		if( bReval ) *pdblCurrent = (double)nCurrent;
	}
	else
	if( m_nMode==1 )
	{
		bReval = GetCurrentFromPoEComm(&nCurrent);
		if( bReval ) *pdblCurrent = (double)nCurrent;
	}
	else
	if( m_nMode==2 )
	{
		bReval = GetCurrentFromTexio(&nCurrent);
		if( bReval ) *pdblCurrent = (double)nCurrent;
	}
	else
	if( m_nMode==3 )
	{
		bReval = GetCurrentFromPoEComm2(pdblCurrent);
	}
	//▲1.0.0.1048

	//▼1.0.0.1052
	else
	if( m_nMode==4 )
	{
		bReval = GetCurrentFromInterface(pdblCurrent);
	}
	//▲1.0.0.1052
	return bReval;
}


//ラインセンサー用治具から消費電流取得
BOOL StCheckPower::GetCurrentFromLineComm(INT *piCurrent)
{
	BOOL bReval = FALSE;
	if( !m_pSerialComm ) return FALSE;
	
	TCHAR szSendText[256];
	TCHAR szRcvText[256];

	do
	{
		INT nCurrent = 0;
		_tcscpy_s( szSendText, _countof(szSendText), _T("shd_ch=1") );
		bReval = m_pSerialComm->SendText( szSendText, szRcvText, _countof(szRcvText) );
		if( !bReval ) break;

		_tcscpy_s( szSendText, _countof(szSendText), _T("dipsw") );
		memset( szRcvText, 0, sizeof(szRcvText) );
		bReval = m_pSerialComm->SendText( szSendText, szRcvText, _countof(szRcvText) );
		if( !bReval ) break;
		INT nTmp = _ttoi(szRcvText);
		nCurrent = (nTmp&0xf) + ((nTmp>>4)&0xf) * 10;

		_tcscpy_s( szSendText, _countof(szSendText), _T("shd_ch=2") );
		bReval = m_pSerialComm->SendText( szSendText, szRcvText, _countof(szRcvText) );
		if( !bReval ) break;

		_tcscpy_s( szSendText, _countof(szSendText), _T("dipsw") );
		memset( szRcvText, 0, sizeof(szRcvText) );
		bReval = m_pSerialComm->SendText( szSendText, szRcvText, _countof(szRcvText) );
		if( !bReval ) break;
		nTmp = _ttoi(szRcvText);
		nCurrent += (nTmp&0xf) * 100 + ((nTmp>>4)&0xf) * 1000;

		*piCurrent = nCurrent;

	}while(0);

	return bReval;
}

//GigE用治具からPoE消費電流取得
BOOL StCheckPower::GetCurrentFromPoEComm(INT *piCurrent)
{
	BOOL bReval = FALSE;
	if( !m_pSerialComm ) return FALSE;
	
	BYTE byteSend[2] = { 0xF2, 0x01};

	BYTE byteRecieve[3];
	size_t rcvSize = sizeof(byteRecieve);
	bReval = m_pSerialComm->SendBin( byteSend, sizeof(byteSend), byteRecieve, &rcvSize );
	if( bReval )
	{
		if( byteRecieve[0]!=0xF2 ) bReval = FALSE;
		else
		{
			*piCurrent = (INT)*(PWORD)&byteRecieve[1];
		}
	}

	return bReval;
}

//Texioから消費電流取得
BOOL StCheckPower::GetCurrentFromTexio(INT *piCurrent)
{
	if( !m_pTexioControl ) return FALSE;
	double dblCurrnt=0.0;
	BOOL bReval = m_pTexioControl->GetCurrent(&dblCurrnt);
	if( bReval )
	{
		//AからmAに変換
		*piCurrent = (INT)(dblCurrnt*1000);
	}
	return bReval;
}

//▼1.0.0.1025
BOOL StCheckPower::GetFunction(LPCTSTR szFunc, LPCTSTR szArg, double &dblValue)
{
	BOOL bReval = FALSE;
	CString szFunction(szFunc);
	if( szFunction=="GetCurrent" )
	{
		//▼1.0.0.1047
		//dblValue = m_nResultCurrent;
		dblValue = m_dblResultCurrent;
		//▲1.0.0.1047
		bReval = TRUE;
	}
	return bReval;
}
//▲1.0.0.1025

//▼1.0.0.1048
//GigE用治具からPoE消費電流取得
BOOL StCheckPower::GetCurrentFromPoEComm2(double *pdblCurrent)
{
	BOOL bReval = FALSE;
	if( !m_pSerialComm ) return FALSE;
	
	//BYTE byteSend[2] = { 0xF2, 0x01};


	//"DSP",CR(0x0d),LF(0x0a)
	BYTE byteSend[5] = { 0x44, 0x53, 0x50, 0x0d, 0x0a};

	//RecieveFormat 0x02,0x02,0x02,0x02,"42.5",CR(0x0d),LF(0x0a)

	BYTE byteRecieve[10];
	size_t rcvSize = sizeof(byteRecieve);
	bReval = m_pSerialComm->SendBin( byteSend, sizeof(byteSend), byteRecieve, &rcvSize );
	if( bReval )
	{
		BYTE byteETX[2] = { 0x0d, 0x0a };
		INT nETXPos = -1;
		for( int i=0; i<rcvSize-_countof(byteETX) + 1; i++  ) 
		{
			if( memcmp(&byteRecieve[i],byteETX,sizeof(byteETX))==0 )
			{
				nETXPos = i;
				break;
			}
		}
		if( nETXPos>=0 )
		{
			byteRecieve[nETXPos] = 0;
			CString szCurrent(byteRecieve);
			szCurrent.Trim();
			*pdblCurrent = _ttof(szCurrent);
		}
		else
		{
			bReval = FALSE;
		}
	}
	return bReval;
}
//▲1.0.0.1048

//▼1.0.0.1052
BOOL StCheckPower::GetCurrentFromInterface(double *pdblCurrent)
{
	BOOL bReval = FALSE;
	////▼1.0.0.1054
	//StInterfaceGenICam *aInterface = NULL;
	//if( m_DeviceBase )
	//{
	//	if( m_DeviceBase->m_ClassName=="StDeviceGenICam" )
	//	{
	//		StDeviceGenICam *aDeviceBase = (StDeviceGenICam *)m_DeviceBase;
	//		if( aDeviceBase->GetDeviceInfo() )
	//		{
	//			aInterface = aDeviceBase->GetDeviceInfo()->GetInterface();
	//		}
	//	}
	//}
	////▲1.0.0.1054

	if( !m_InterfaceGenICam ) return FALSE;
	if( !szCommand.GetLength() ) return FALSE;

	double dblData;
	bReval = m_InterfaceGenICam->GetGenICamControl(szCommand.GetUnicode(),dblData);
	if( bReval )
	{
		*pdblCurrent = dblData;
	}
	return bReval;
}
//▲1.0.0.1052
