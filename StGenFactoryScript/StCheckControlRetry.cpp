#include "StdAfx.h"
#include "StCheckControlRetry.h"
#include "MessageDlg.h"
#include "StDeviceEBus.h"
#include "StInspectMain.h"

//class StInspectMain;

//StSerialComm.cpp
BOOL IsNumerical( LPCTSTR szData );


StCheckControlRetry::StCheckControlRetry(void)
{
	Initialize();
}

StCheckControlRetry::StCheckControlRetry(LPCTSTR szClassName, LPCTSTR szVarName, StDeviceBase *aDeviceBase, void *aDialog)
	:StCheckBase( szClassName, szVarName, aDeviceBase, aDialog )
{
	Initialize();
}


StCheckControlRetry::~StCheckControlRetry(void)
{
}

void StCheckControlRetry::Initialize(void)
{
	m_nTimer = 100;
	m_nRetry = 10;

	m_szSend = "";
	m_szReturn = "";
	m_szResult = "";
	m_szBusyResult = "";

	m_szSendSTX = "";
	m_szSendETX = "";	//Binary��"" Ascii��"0d"
	m_szRecieveSTX = "";
	m_szRecieveETX = "";
	//��1.0.0.1015
	m_szRcvReplaceSpace = "";
	//��1.0.0.1015
	//��1.0.0.1020
	m_szLastRecieveData = "";
	//��1.0.0.1020
	//��1.0.0.1022
	m_nLastRecieveValue = -1;
	//��1.0.0.1022

	//��1.0.0.1047
	m_bMainControl = TRUE;
	//m_bRetryControl = TRUE;
	//��1.0.0.1047

}

//��1.0.0.1021
BOOL StCheckControlRetry::SetCommTimeout(DWORD dwReadIntervalTimeout,DWORD dwReadTotalTimeoutConstant,DWORD dwReadTotalTimeoutMultiplier,DWORD dwWriteTotalTimeoutConstant,DWORD dwWriteTotalTimeoutMultiplier)
{
	BOOL bReval=FALSE;
	if( !m_DeviceBase ) return FALSE;
	if( m_DeviceBase->m_ClassName=="StDeviceEBus" )	//GigE�p
	{
	}
	else
	if( m_DeviceBase->m_ClassName=="StDeviceMultiCam" )	//CL�p
	{
		StDeviceMultiCam *aDevice = (StDeviceMultiCam *)m_DeviceBase;
		if( aDevice && aDevice->IsOpened() )
		{
			StSerialComm *aSerialComm = aDevice->GetSerialComm();
			if( aSerialComm )
			{
				DWORD dwReval = aSerialComm->SetTimeout(dwReadIntervalTimeout,dwReadTotalTimeoutConstant, dwReadTotalTimeoutMultiplier,dwWriteTotalTimeoutConstant,dwWriteTotalTimeoutMultiplier);
				if( dwReval==NO_ERROR ) bReval = TRUE;
			}
		}
	}
	return bReval;
}
BOOL StCheckControlRetry::GetCommTimeout(PDWORD dwReadIntervalTimeout,PDWORD dwReadTotalTimeoutConstant,PDWORD dwReadTotalTimeoutMultiplier,PDWORD dwWriteTotalTimeoutConstant,PDWORD dwWriteTotalTimeoutMultiplier)
{
	BOOL bReval=FALSE;
	if( !m_DeviceBase ) return FALSE;
	if( m_DeviceBase->m_ClassName=="StDeviceEBus" )	//GigE�p
	{
	}
	else
	if( m_DeviceBase->m_ClassName=="StDeviceMultiCam" )	//CL�p
	{
		StDeviceMultiCam *aDevice = (StDeviceMultiCam *)m_DeviceBase;
		if( aDevice && aDevice->IsOpened() )
		{
			StSerialComm *aSerialComm = aDevice->GetSerialComm();
			if( aSerialComm )
			{
				DWORD dwReval = aSerialComm->GetTimeout(dwReadIntervalTimeout,dwReadTotalTimeoutConstant, dwReadTotalTimeoutMultiplier,dwWriteTotalTimeoutConstant,dwWriteTotalTimeoutMultiplier);
				if( dwReval==NO_ERROR ) bReval = TRUE;
			}
		}
	}
	return bReval;
}

//��1.0.0.1021



BOOL StCheckControlRetry::Execute(void)
{
	BOOL bReval=FALSE;

	//CMessageDlg *aMessage=NULL;
	//BYTE *aCmpRcvData=NULL;
	WORD *aCmpRcvData=NULL;
	BYTE *aGetRcvData=NULL;
	WORD *aCmpBusyRcvData=NULL;
	BYTE *aGetBusyRcvData=NULL;
	

	//��1.0.0.1047
	m_bMainControl = TRUE;
	//m_bRetryControl = TRUE;
	//��1.0.0.1047

	m_iJudge = 2;
	do
	{
		StInspectMain *pInspectionMain = (StInspectMain *)m_pInspectionMain;
		if( m_DeviceBase->m_ClassName=="StDeviceEBus" )	//GigE�p
		{
			StDeviceEBus *aDevice = (StDeviceEBus *)m_DeviceBase;
			if( !aDevice->IsConnected() ) break;

			//��M��������f�[�^�ɕϊ�
			INT aRcvData = 0;
			CString szRcvData(m_szReturn);
			//�X�y�[�X��,�����O
			szRcvData.Remove(' ');
			szRcvData.Remove('�@');
			szRcvData.Remove(',');
			if( (szRcvData.GetLength()&1)!=0 ) break;

			size_t rcvSize = szRcvData.GetLength()/2;
			//aCmpRcvData = new BYTE [rcvSize];
			aCmpRcvData = new WORD [rcvSize];
			if( !aCmpRcvData ) break;
			memset( aCmpRcvData, 0, sizeof(BYTE) * rcvSize );
			for( int i=0; i<(int)rcvSize; i++ )
			{
				//aCmpRcvData[i] = (BYTE)_tcstol( szRcvData.Mid(i*2, 2), nullptr, 16 );
				if( szRcvData.Mid(i*2, 2).Compare(_T("XX")) == 0 )
				{
					aCmpRcvData[i] = 0xffff;
				}
				else
				{
					aCmpRcvData[i] = (WORD)_tcstol( szRcvData.Mid(i*2, 2), nullptr, 16 )&0xff;
				}
			}
			aGetRcvData = new BYTE [rcvSize+2]; //2BYTE�قǗ]���Ɏ���Ă���
			if( !aGetRcvData ) break;
			memset( aGetRcvData, 0, sizeof(BYTE) * (rcvSize+2) );


			/*
			aMessage = new CMessageDlg;
			if( !aMessage ) return FALSE;
			aMessage->SetOKButtonShow(FALSE);
			aMessage->SetNGButtonShow(FALSE);
			aMessage->SetTextButtonShow(FALSE);
			CString szMessage;
			szMessage.Format(_T("�J�ԉ� %d �Ԋu %dmSec"), m_nRetry, m_nTimer );
			aMessage->SetMessage(szMessage);
			aMessage->Create(IDD_DIALOG_MESSAGE);

			WINDOWINFO aInfo;
			aMessage->GetWindowInfo(&aInfo);
			INT nSizeX = aInfo.rcWindow.right - aInfo.rcWindow.left;
			INT nSizeY = aInfo.rcWindow.bottom - aInfo.rcWindow.top;
			BOOL bReval2 = aMessage->SetWindowPos( NULL, 0, 0, nSizeX, nSizeY, SWP_NOZORDER|SWP_SHOWWINDOW );

			//aMessage->ShowWindow(SW_SHOW);
			::ShowWindow(aMessage->m_hWnd,SW_SHOW);
			::ShowWindowAsync(aMessage->m_hWnd,SW_SHOW);
			*/

			CString szSend(m_szSend);

			if( szSend.GetLength() > 0 )
			{
				//�X�y�[�X��,�����O
				szSend.Remove(' ');
				szSend.Remove('�@');
				szSend.Remove(',');
				if( szSend.Left(2)==_T("0x") || szSend.Left(2)==_T("0X") )
				{
					szSend = szSend.Mid(2);
				}

				m_szResult = _T("");
				m_szBusyResult = _T("");
//{
//TCHAR szText[128];
//_stprintf_s(szText,_countof(szText),_T(" Init m_szResult=%s\n"), m_szResult);
//OutputDebugString(szText);
//}

				//for( int i=0; i<m_nRetry && bReval==FALSE; i++ )
				//�R�}���h���M��M
				for( int i=0; i<1; i++ )
				{
					//Sleep(m_nTimer);
					//��1.0.0.1060
					//size_t getRcvSize = rcvSize + 2;
					size_t getRcvSize = rcvSize;
					//��1.0.0.1060
					BOOL bRevCtrl = aDevice->SerialControl( _T("UART0"), szSend, aGetRcvData, &getRcvSize );
					if( bRevCtrl )
					{
						CString tmp;
						//�\��-----------------------------
						CString szRecieve(_T(""));
						for( int j=0; j<(int)getRcvSize; j++ )
						{
							if( j>0 )	szRecieve+=_T(",");
							tmp.Format(_T("%02x"), aGetRcvData[j] );
							szRecieve+= tmp;
						}

						m_szResult = szRecieve;
//{
//TCHAR szText[128];
//_stprintf_s(szText,_countof(szText),_T("  m_szResult=%s\n"), m_szResult);
//OutputDebugString(szText);
//}

						if( getRcvSize>0 )
						{
							pInspectionMain->DisplayListBox(szRecieve);
							//��1.0.0.1020
							m_szLastRecieveData = szRecieve;
							//��1.0.0.1020
						}
						if( rcvSize==getRcvSize )
						{
							//��r
							for( int j=0; j<(int)rcvSize; j++ )
							{
								if( (aCmpRcvData[j]>>8)>0 )	//XX�̈ʒu�͔�r���Ȃ�
									continue;
								if( aCmpRcvData[j]!=(WORD)aGetRcvData[j] )
								{
									m_iJudge = 0;
									bReval = TRUE;
									break;
								}
							}
							if( m_iJudge!=0 )
							{
								m_iJudge = 1;
								bReval = TRUE;
							}

							//if( memcmp(aCmpRcvData,aGetRcvData,rcvSize )==0 )
							//{
							//	m_iJudge = 1;
							//	bReval = TRUE;
							//}
						}
					}
				}
			}
			else
			{
				m_iJudge = 1;
				bReval = TRUE;
			}

			if( m_iJudge==0 )
			{
				//�Ⴂ������ꍇ�I��
				break;
			}

			if( m_nRetry>0 )
			{
				bReval = FALSE;
				m_iJudge = 2;

				//��M��������f�[�^�ɕϊ�
				INT aRcvData = 0;
				CString szBusyStatus(m_szBusyStatus);
				//�X�y�[�X��,�����O
				szBusyStatus.Remove(' ');
				szBusyStatus.Remove('�@');
				szBusyStatus.Remove(',');
				if( (szBusyStatus.GetLength()&1)!=0 ) break;

				size_t rcvSize = szBusyStatus.GetLength()/2;
				aCmpBusyRcvData = new WORD [rcvSize];
				if( !aCmpBusyRcvData ) break;
				memset( aCmpBusyRcvData, 0, sizeof(BYTE) * rcvSize );
				for( int i=0; i<(int)rcvSize; i++ )
				{
					if( szBusyStatus.Mid(i*2, 2).Compare(_T("XX")) == 0 )
					{
						aCmpBusyRcvData[i] = 0xffff;
					}
					else
					{
						aCmpBusyRcvData[i] = (WORD)_tcstol( szBusyStatus.Mid(i*2, 2), nullptr, 16 )&0xff;
					}
				}
				aGetBusyRcvData = new BYTE [rcvSize+2]; //2BYTE�قǗ]���Ɏ���Ă���
				if( !aGetBusyRcvData ) break;
				memset( aGetBusyRcvData, 0, sizeof(BYTE) * (rcvSize+2) );


				CString szBusySend(m_szBusyCommand);
				//�X�y�[�X��,�����O
				szBusySend.Remove(' ');
				szBusySend.Remove('�@');
				szBusySend.Remove(',');
				if( szBusySend.Left(2)==_T("0x") || szBusySend.Left(2)==_T("0X") )
				{
					szBusySend = szBusySend.Mid(2);
				}
				//Busy�R�}���h���M��M
				for( int i=0; i<m_nRetry && bReval==FALSE; i++ )
				{

					Sleep(m_nTimer);
					//��1.0.0.1060
					//size_t getRcvSize = rcvSize + 2;
					size_t getRcvSize = rcvSize;
					//��1.0.0.1060
					BOOL bRevCtrl = aDevice->SerialControl( _T("UART0"), szBusySend, aGetBusyRcvData, &getRcvSize );
					if( bRevCtrl )
					{
						CString tmp;
						//�\��-----------------------------
						CString szRecieve(_T(""));
						for( int j=0; j<(int)getRcvSize; j++ )
						{
							if( j>0 )	szRecieve+=_T(",");
							tmp.Format(_T("%02x"), aGetBusyRcvData[j] );
							szRecieve+= tmp;
						}

						m_szBusyResult = szRecieve;	//���O�p
//{
//TCHAR szText[128];
//_stprintf_s(szText,_countof(szText),_T("  m_szBusyResult=%s\n"), m_szBusyResult);
//OutputDebugString(szText);
//}

						if( getRcvSize>0 )
						{
							tmp.Format(_T(" (Busy Retry=%d)"),i+1);
							pInspectionMain->DisplayListBox(szRecieve + tmp);
						}
						BOOL bCheckNG = FALSE;
						if( rcvSize==getRcvSize )
						{
							//��r
							for( int j=0; j<(int)rcvSize; j++ )
							{
								if( (aCmpBusyRcvData[j]>>8)>0 )	//XX�̈ʒu�͔�r���Ȃ�
									continue;
								if( aCmpBusyRcvData[j]!=(WORD)aGetBusyRcvData[j] )
								{
									//m_iJudge = 0;
									//bReval = TRUE;
									bCheckNG = TRUE;
									break;
								}
							}
							if( bCheckNG == FALSE )
							{
								m_iJudge = 1;
								bReval = TRUE;
							}
						}
					}
				}
				if( m_iJudge==2 )
				{
					m_iJudge = 0;
					bReval = TRUE;
				}
			}


			//aMessage->ShowWindow(SW_HIDE);

		}
		else
		if( m_DeviceBase->m_ClassName=="StDeviceMultiCam" )	//CL�p
		{
			StDeviceMultiCam *aDevice = (StDeviceMultiCam *)m_DeviceBase;
			if( !aDevice->IsOpened() ) break;
			//CString szPortName;
			//aDevice->GetPortName(szPortName);
			//CString szVarName = m_VarName;
			//szVarName += _T("_ControlRetry");
			//StSerialComm aSerialComm(_T("StSerialComm"),szVarName,szPortName);

			StSerialComm *aSerialComm = aDevice->GetSerialComm();
//TRACE(TEXT("@@@@@@@@@ aSerialComm=0x%08x\n"), aSerialComm );
//aSerialComm->function( _T("SetRecieveData"), _T("test") );


			aSerialComm->function( _T("SetRecieveData"), m_szReturn );
			aSerialComm->function( _T("SetSendSTX"), m_szSendSTX );
			aSerialComm->function( _T("SetSendETX"), m_szSendETX );
			aSerialComm->function( _T("SetRecieveSTX"), m_szRecieveSTX );
			aSerialComm->function( _T("SetRecieveETX"), m_szRecieveETX );
			//��1.0.0.1015
			aSerialComm->function( _T("SetRcvReplaceSpace"), m_szRcvReplaceSpace );
			//��1.0.0.1015


//if( m_szSend==_T("0x02,\"USS\",0x0d") )
//if( m_szSend==_T("ctrl=1") )
//{
//	Sleep(1);
//}
			StString szRecieve;
			if( m_szSend.GetLength() > 0 )
			{
//if( m_szSend==_T("opck=8") )
//{
//	Sleep(1);
//}
				bReval = aSerialComm->function( _T("Send"), m_szSend );

				//��1.0.0.1047
				m_bMainControl = bReval;
				if( !bReval )
				{
					m_bMainControl = aSerialComm->GetControl();
				}
				if( m_bMainControl )
				//��1.0.0.1047
				//��1.0.0.1024
//				if( bReval )
				//��1.0.0.1024
				{
					aSerialComm->GetRecieveResultData(szRecieve);

//TRACE(TEXT("@@@@@@@@@ szRecieve=%s\n"), szRecieve.GetUnicode() );

					StInspectMain *pInspectionMain = (StInspectMain *)m_pInspectionMain;
					pInspectionMain->DisplayListBox(szRecieve);

					//��1.0.0.1020
					m_szLastRecieveData = szRecieve.GetAscii();
					//��1.0.0.1020

					//��1.0.0.1022
					m_szResult = m_szLastRecieveData;
					aSerialComm->GetRecieveResultValue(m_nLastRecieveValue);
					//��1.0.0.1022
				}
			}
			else
			{
				bReval = TRUE;
			}
			m_iJudge = bReval?1:0;

			if( m_iJudge==0 )
			{
				//�Ⴂ������ꍇ�I��
				break;
			}


			if( m_nRetry>0 )
			{
				bReval = FALSE;
				m_iJudge = 2;

				aSerialComm->function( _T("SetRecieveData"), m_szBusyStatus );

				//Busy�R�}���h���M��M
				CString tmp;
				for( int i=0; i<m_nRetry && bReval==FALSE; i++ )
				{
					Sleep(m_nTimer);

					bReval = aSerialComm->function( _T("Send"), m_szBusyCommand );
					aSerialComm->GetRecieveResultData(szRecieve);
					//m_szBusyResult = szRecieve.GetAscii();
					//tmp.Format(_T(" (Busy Retry=%d)CMD=%s"),i+1,m_szBusyCommand);
					tmp.Format(_T(" (Busy Retry=%d)"),i+1);
					pInspectionMain->DisplayListBox(szRecieve + tmp);
					if( bReval )
						break;
				}
				if( bReval )
				{
					m_iJudge = 1;
				}
				else
				{
					m_iJudge = 0;
				}

			}

		}

	}while(0);

	//if( aMessage ) delete aMessage;
	if( aCmpRcvData ) delete [] aCmpRcvData;
	if( aGetRcvData ) delete [] aGetRcvData;
	if( aCmpBusyRcvData ) delete [] aCmpBusyRcvData;
	if( aGetBusyRcvData ) delete [] aGetBusyRcvData;

	if( !bReval )
	{
		m_iJudge = 0;
	}

	return bReval;
}

BOOL StCheckControlRetry::function(LPCTSTR szFunc, LPCTSTR szArgument)
{
	BOOL bReval = FALSE;
	CString szFunction(szFunc);
	if( szFunction == _T("Execute") )
	{
	}
	else
	if( szFunction=="SetRetry" )
	{
		INT nData = _tcstol(szArgument,NULL,0);
		SetRetry(nData);
		bReval = TRUE;
	}
	else
	if( szFunction=="SetTimer" )
	{
		INT nData = _tcstol(szArgument,NULL,0);
		SetTimer(nData);
		bReval = TRUE;
	}
	else
	if( szFunction=="SendData" )
	{
		SetSendData(szArgument);
		bReval = TRUE;
	}
	else
	if( szFunction=="ReturnData" )
	{
		SetRecieveData(szArgument);
		bReval = TRUE;
	}
	else
	if( szFunction=="SetBusyCommand" )
	{
		SetBusyCommand(szArgument);
		bReval = TRUE;
	}
	else
	if( szFunction=="SetBusyStatus" )
	{
		SetBusyStatus(szArgument);
		bReval = TRUE;
	}

	//�^�J�m�����Ή�
	else
	if( szFunction=="SetSendSTX" )
	{
		SetSendSTX(szArgument);
		bReval = TRUE;
	}
	else
	if( szFunction=="SetSendETX" )
	{
		SetSendETX(szArgument);
		bReval = TRUE;
	}
	else
	if( szFunction=="SetRecieveSTX" )
	{
		SetRecieveSTX(szArgument);
		bReval = TRUE;
	}
	else
	if( szFunction=="SetRecieveETX" )
	{
		SetRecieveETX(szArgument);
		bReval = TRUE;
	}
	//��1.0.0.1015
	else
	if( szFunction=="SetRcvReplaceSpace" )
	{
		SetRcvReplaceSpace(szArgument);
		bReval = TRUE;
	}
	//��1.0.0.1015

	return bReval;
}

BOOL StCheckControlRetry::GetFunction(LPCTSTR szFunc, LPCTSTR szArg,double &dblValue)
{
	BOOL bReval = FALSE;
	CString szFunction(szFunc);
	if( szFunction=="GetRecieveValue" )
	{
		dblValue = m_nLastRecieveValue;
		bReval = TRUE;
	}
	return bReval;
}








