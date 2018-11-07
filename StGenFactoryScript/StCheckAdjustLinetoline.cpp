#include "StdAfx.h"
#include "StCheckAdjustLinetoline.h"
//▼1.0.0.1044
//#include "StDeviceEBus.h"
//▲1.0.0.1044
#include "StInspectMain.h"


StCheckAdjustLinetoline::StCheckAdjustLinetoline(void)
	: m_pParent(NULL)
{
	Initialize();
}

StCheckAdjustLinetoline::StCheckAdjustLinetoline(LPCTSTR szClassName, LPCTSTR szVarName, StDeviceBase *aDeviceBase, void* pParent, void *aDialog)
	:StCheckBase( szClassName, szVarName, aDeviceBase, aDialog )
	, m_pParent(pParent)
{
	Initialize();
}


StCheckAdjustLinetoline::~StCheckAdjustLinetoline(void)
{
}

void StCheckAdjustLinetoline::Initialize(void)
{
	m_nStart = 0;
	m_nEnd = 4095;

	m_nTimer = 1000;
	m_nDeviceCode = 0;
	m_nPage = 0;
	m_nAddress = 0;
	m_nInitValue = 0;
	m_dblMinAverage = 190;
	m_dblMaxAverage = 210;

	m_nAdjustCount = 10;

	m_bThreadStop = FALSE;

	//▼1.0.0.1015
	m_szCommand = "";
	//▲1.0.0.1015

}

BOOL StCheckAdjustLinetoline::function(LPCTSTR szFunc, LPCTSTR szArgument)
{
	BOOL bReval = FALSE;
	CString szFunction(szFunc);
	if( szFunction == _T("Execute") )
	{
	}
	else
	if( szFunction=="SetStartLine" )
	{
		m_nStart = _tcstol(szArgument,NULL,0);
		bReval = TRUE;
	}
	else
	if( szFunction=="SetEndLine" )
	{
		m_nEnd = _tcstol(szArgument,NULL,0);
		bReval = TRUE;
	}
	else
	if( szFunction=="SetTimer" )
	{
		m_nTimer = _tcstol(szArgument,NULL,0);
		bReval = TRUE;
	}
	else
	if( szFunction=="SetDeviceCode" )
	{
		m_nDeviceCode = _tcstol(szArgument,NULL,0);
		bReval = TRUE;
	}
	else
	if( szFunction=="SetPage" )
	{
		m_nPage = _tcstol(szArgument,NULL,0);
		bReval = TRUE;
	}
	else
	if( szFunction=="SetAddress" )
	{
		m_nAddress = _tcstol(szArgument,NULL,0);
		bReval = TRUE;
	}
	else
	if( szFunction=="SetInitValue" )
	{
		m_nInitValue = _tcstol(szArgument,NULL,0);
		bReval = TRUE;
	}
	else
	if( szFunction=="SetMinAverage" )
	{
		m_dblMinAverage = _ttof(szArgument);
		bReval = TRUE;
	}
	else
	if( szFunction=="SetMaxAverage" )
	{
		m_dblMaxAverage = _ttof(szArgument);
		bReval = TRUE;
	}
	else
	if( szFunction=="SetAdjustCount" )
	{
		m_nAdjustCount = _tcstol(szArgument,NULL,0);
		bReval = TRUE;
	}

	//▼1.0.0.1015
	else
	if( szFunction=="SetCommand" )
	{
		m_szCommand = szArgument;
		bReval = TRUE;
	}
	//▲1.0.0.1015

	return bReval;
}

//▼1.0.0.1044
//StCheckBaseに移動させるかどうか？？？
//BOOL StCheckAdjustLinetoline::WriteDataEBus(BYTE byteDeviceCode, BYTE bytePage, BYTE byteAddress, BYTE byteData)
//{
//	StDeviceEBus *aDevice = (StDeviceEBus *)m_DeviceBase;
//	if( !aDevice->IsConnected() ) return FALSE;
//
//	BYTE byteSendData[6];
//	byteSendData[0] = 0x02;
//	byteSendData[1] = (byteDeviceCode<<2) | 0x02 | (bytePage&1);
//	byteSendData[2] = byteAddress;
//	byteSendData[3] = 1;
//	byteSendData[4] = byteData;
//	byteSendData[5] = 0x03;
//	BYTE byteRcvData[4] = {2,0,1,3};	//Write時の正常リード値
//	BYTE GetRcvData[4+2];
//	size_t getRcvSize = sizeof(GetRcvData);
//
//	BOOL bRevCtrl = aDevice->SerialControl( _T("UART0"), byteSendData, sizeof(byteSendData), GetRcvData, &getRcvSize );
//	if( bRevCtrl )
//	{
//		if( getRcvSize!=sizeof(byteRcvData) )
//		{
//			bRevCtrl = FALSE;
//		}
//	}
//	if( bRevCtrl )
//	{
//		if( memcmp(GetRcvData,byteRcvData,getRcvSize)!=0 )
//			bRevCtrl = FALSE;
//	}
//
//	return bRevCtrl;
//}
//
////▼1.0.0.1015
//BOOL StCheckAdjustLinetoline::WriteDataMultiCamAscii(StString &szCommand, BYTE byteData)
//{
//	StDeviceMultiCam *aDevice = (StDeviceMultiCam *)m_DeviceBase;
//	if( !aDevice->IsOpened() ) return FALSE;
//	StSerialComm *aSerialComm = aDevice->GetSerialComm();
//	if( !aSerialComm ) return FALSE;
//
//	TCHAR szSendText[256];
//	TCHAR szRcvText[256];
//
//	CString szCommandText(szCommand.GetAscii());
//
//	_stprintf_s( szSendText, _countof(szSendText), _T("%s=%u"), szCommandText, byteData );
//	BOOL bReval = aSerialComm->SendText( szSendText, szRcvText, _countof(szRcvText) );
//
//	TCHAR *szRcvCmpText = {_T("OK")};
//	if( bReval )
//	{
//		if( _tcscmp(szRcvText,szRcvCmpText)!=0 )
//			bReval = FALSE;
//	}
//	return bReval;
//}
//
//BOOL StCheckAdjustLinetoline::WriteDataMultiCamBinary(BYTE byteDeviceCode, BYTE bytePage, BYTE byteAddress, BYTE byteData)
//{
//	StDeviceMultiCam *aDevice = (StDeviceMultiCam *)m_DeviceBase;
//	if( !aDevice->IsOpened() ) return FALSE;
//	StSerialComm *aSerialComm = aDevice->GetSerialComm();
//	if( !aSerialComm ) return FALSE;
//
//	BYTE byteSendData[6];
//	byteSendData[0] = 0x02;
//	byteSendData[1] = (byteDeviceCode<<2) | 0x02 | (bytePage&1);
//	byteSendData[2] = byteAddress;
//	byteSendData[3] = 1;
//	byteSendData[4] = byteData;
//	byteSendData[5] = 0x03;
//	BYTE byteRcvData[4] = {2,0,1,3};	//Write時の正常リード値
//	BYTE GetRcvData[4+2];
//	size_t getRcvSize = sizeof(GetRcvData);
//
//	BOOL bRevCtrl = aSerialComm->SendBin(byteSendData,sizeof(byteSendData),GetRcvData,&getRcvSize);
//
//	if( bRevCtrl )
//	{
//		if( getRcvSize!=sizeof(byteRcvData) )
//		{
//			bRevCtrl = FALSE;
//		}
//	}
//	if( bRevCtrl )
//	{
//		if( memcmp(GetRcvData,byteRcvData,getRcvSize)!=0 )
//			bRevCtrl = FALSE;
//	}
//
//	return bRevCtrl;
//}
//▲1.0.0.1015
//▲1.0.0.1044


BOOL StCheckAdjustLinetoline::Execute(void)
{
	BOOL bReval = FALSE;

	//▼1.0.0.1007
	m_SetValue = m_nInitValue;
	m_ResultCount = 0;
	//bReval = WriteData( m_nDeviceCode, m_nPage, m_nAddress, m_SetValue );
	//if( !bReval )
	//{
	//	m_iJudge = 0;	//0:NG
	//	return FALSE;
	//}
	//▲1.0.0.1007

	//▼1.0.0.1020
	m_nErrorCode = StResult::Code::OK;
	//▲1.0.0.1020


	do
	{
		if( GetThreadStop() )
			break;

		bReval = AdjustExecute();

		if( !bReval )
			break;	//NGではなくエラー

//TRACE(TEXT("@@@@@@@@@ GetResultCount()=%d GetStandardCount()=%d\n"), GetResultCount(), GetStandardCount() );
		if( GetResultCount()>=GetStandardCount() )
			break;	//カウント終了
		if( GetJudge()==1 )	//正常終了
			break;

		Sleep(200);	//高速で動かすとOKボタンが押せなくなる？？？

	}while(1);

	return bReval;
}

BOOL StCheckAdjustLinetoline::AdjustExecute(void)
{

	BOOL bReval = FALSE;
	m_iJudge = 2;	//2:処理中

	do
	{
		BYTE setValue = m_SetValue;
		if( m_ResultCount>0 )	//２回目以降
		{
			if( m_ResultMinAverage>m_dblMaxAverage )
			{
				if( m_ResultCount>1 )
				{
					setValue-=1;
				}
				else
				{
					setValue -= (BYTE)(m_ResultMinAverage - m_dblMaxAverage + 1.0);
				}
			}
			else
			if( m_ResultMinAverage<m_dblMinAverage )
			{
				if( m_ResultCount>1 )
				{
					setValue+=1;
				}
				else
				{
					//▼1.0.0.1007
					setValue += max( (BYTE)( m_dblMinAverage - m_ResultMinAverage + 1.0), 1 );
					//setValue += (BYTE)( m_dblMinAverage - m_ResultMinAverage + 1.0);
					//▲1.0.0.1007
				}
			}
		}
		//値を設定し、SetTimer後に画像取得評価
		if( m_DeviceBase->m_ClassName=="StDeviceEBus" )	//GigE用
		{
			bReval = WriteDataEBus( m_nDeviceCode, m_nPage, m_nAddress, setValue );
			if( !bReval )
				break;
		}


		//▼1.0.0.1015
		else
		if( m_DeviceBase->m_ClassName=="StDeviceMultiCam" )	//CL用
		{
			if( m_szCommand.GetLength()>0 )
			{
				bReval = WriteDataMultiCamAscii( m_szCommand, setValue );
			}
			else
			{
				bReval = WriteDataMultiCamBinary( m_nDeviceCode, m_nPage, m_nAddress, setValue );
			}
			if( !bReval )
			//▼1.0.0.1020
			//	break;
			{
				m_nErrorCode = StResult::Code::NOT_SUPPORTED;
				break;
			}
			//▲1.0.0.1020

		}
		//▲1.0.0.1015


		else
		{
			//▼1.0.0.1020
			m_nErrorCode = StResult::Code::NOT_SUPPORTED;
			//▲1.0.0.1020
			break;
		}
		Sleep(m_nTimer);


//TRACE(TEXT("@@@@@@@@@ StCheckAdjustLinetoline m_DeviceBase=0x%x\n"), m_DeviceBase );

		if( m_Buffer ) delete m_Buffer;
		m_Buffer = new StBuffer;
		bReval = m_DeviceBase->GetStBuffer(m_Buffer);	//変換して取得もありかも。現時点ではコピー

		size_t nElementSize=1;
		double dblMinAverage=0.0;
		INT  nLine = -1;
		INT  nElement = -1;
		if( bReval )
			bReval = GetMinimumLineAverage( m_Buffer, m_nStart, m_nEnd, &nElementSize, &dblMinAverage, &nLine, &nElement );

		if( bReval )
		{

//TRACE(TEXT("@@@@  setValue=%d dblMinAverage(%f)\n"), setValue, dblMinAverage);

			//別スレッドなのでここで表示ができない。。。。

			CString szText;
			szText.Format(_T("mindata=%.02f value=%d"), dblMinAverage, setValue );
//TRACE(szText);
//TRACE(_T("\n"));

			DisplayListBox(szText);
			//CListBox aList;
			//aList.Attach(GetDlgItem(IDC_LIST_LINETOLINE)->m_hWnd);
			//aList.AddString(szText);
			//UpdateLogListBoxScrollBar(&aList);
			//aList.Detach();

			m_SetValue = setValue;
			m_ResultMinAverage = dblMinAverage;
			m_ResultLine = nLine;
			m_ResultElement = nElement;
			m_ResultCount++;

			if( dblMinAverage<m_dblMinAverage || m_dblMinAverage>m_dblMaxAverage )
				m_iJudge=0;
			if( m_iJudge==2 ) m_iJudge = 1;	//1:OK

			//結果数値表示
			//ResultText();
		}
		else
		{
			m_iJudge = 0;	//0:NG
		}
	}while(0);

	if( m_iJudge==2 ) m_iJudge = 0;	//0:NG

	//結果表示
	//JudgeText(m_iJudge==1);

	return bReval;
}

void StCheckAdjustLinetoline::DisplayListBox(CString szMessge)
{
	if( !m_pParent ) return;
	StInspectMain *aParent = (StInspectMain *)m_pParent;
	aParent->DisplayListBox(szMessge);

}
//▼1.0.0.1025
BOOL StCheckAdjustLinetoline::GetFunction(LPCTSTR szFunc, LPCTSTR szArg, double &dblValue)
{
	BOOL bReval = FALSE;
	CString szFunction(szFunc);
	if( szFunction=="GetCount" )
	{
		dblValue = GetResultCount();
		bReval = TRUE;
	}
	else
	if( szFunction=="GetElement" )
	{
		dblValue = m_ResultElement;
		bReval = TRUE;
	}
	else
	if( szFunction=="GetMinAverage" )
	{
		dblValue = m_ResultMinAverage;
		bReval = TRUE;
	}
	else
	if( szFunction=="GetLine" )
	{
		dblValue = GetResultLine();
		bReval = TRUE;
	}
	return bReval;
}
//▲1.0.0.1025




