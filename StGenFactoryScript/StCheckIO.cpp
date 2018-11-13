#include "StdAfx.h"
#include "StCheckIO.h"

#include "StDeviceEBus.h"

StCheckIO::StCheckIO(void)
{
	Initialize();
}

StCheckIO::StCheckIO(LPCTSTR szClassName, LPCTSTR szVarName, StDeviceBase *aDeviceBase, void *aDialog)
	:StCheckBase( szClassName, szVarName, aDeviceBase, aDialog )
{
	Initialize();
}

StCheckIO::~StCheckIO(void)
{
	if( m_piPinMode )
	{
		delete [] m_piPinMode;
		m_piPinMode = NULL;
	}
}

void StCheckIO::Initialize(void)
{

	m_nPinCount = 0;
	m_pSerialComm = NULL;
	m_piPinMode = NULL;

	//��1.0.0.1053
	m_pTerminal = NULL;
	m_nMode = 0;
	m_nCheckWaitTime = 500;
	//��1.0.0.1053

}

//-------------------------------------------------------------------------------
//�������s
//-------------------------------------------------------------------------------
//��1.0.0.1053
BOOL StCheckIO::Execute(void)
{
	BOOL bReval = FALSE;

	m_nCameraStatus = 0;
	m_nCameraSetValue = 0;
	m_nJiguStatus = 0;
	m_nJiguSetValue = 0;

	if( m_nMode==0 )
	{
		bReval = SerialExecute();
	}
	else
	if( m_nMode==1 )
	{
		bReval = TerminalExecute();
	}

	return bReval;
}


//BOOL StCheckIO::Execute(void)
BOOL StCheckIO::SerialExecute(void)
//��1.0.0.1053
{
	if( !m_pSerialComm ) return FALSE;
	BOOL bReval = FALSE;
	m_iJudge = 2;	//2:������

	do
	{
		if( m_DeviceBase->m_ClassName=="StDeviceEBus" )	//GigE�p
		{

			StDeviceEBus *aDevice = (StDeviceEBus *)m_DeviceBase;


			INT nInCount = 0;
			INT nOutCount = 0;
			WORD wCameraInMask = 0;
			WORD wCameraOutMask = 0;

			for( int i=0; i<m_nPinCount; i++ )
			{

				//�J�����ւ̏����ݒ�
				//�������ɂ�UserOutput�ύX�݂̂őΉ�������
				CString szSelector;
				szSelector.Format(_T("Line%d"),i);
				aDevice->SetGenICamControl( _T("LineSelector"), szSelector);
				TCHAR szText[128];
				bReval = aDevice->GetGenICamControl( _T("LineMode"), szText, _countof(szText));
				if( !bReval )
					break;
				if( (m_piPinMode[i]==0 && _tcscmp(szText,_T("Input"))==0 ) || (m_piPinMode[i]==1 && _tcscmp(szText,_T("Output"))==0 ) )
				{
				}
				else
				{
					bReval = FALSE;
					break;
				}

				CString szString;
				//Output����UserSet�ݒ�----�K�v�H�H�����O�̃X�N���v�g�Őݒ肳���邩�H
				//�Ƃ肠���������ōs��
				if( m_piPinMode[i]==1 )
				{
					szString.Format(_T("UserOutput%d"), nOutCount);
					bReval = aDevice->SetGenICamControl( _T("LineSource"), szString);
					szString.Format(_T("UserOutput%d"), nOutCount);
					if( bReval )
						bReval = aDevice->SetGenICamControl( _T("UserOutputSelector"), szString);
					if( bReval )
						bReval = aDevice->SetGenICamControl( _T("UserOutputValue"), false);
				}

				//�{���`�F�b�N
				if( m_piPinMode[i]==0 )
				{
					//wCameraInMask |= (1<<i);
					nInCount++;
				}
				else
				if( m_piPinMode[i]==1 )
				{
					//wCameraOutMask |= (1<<i);
					nOutCount++;
				}
			}
			if( !bReval )
				break;


			if( nInCount>3 )	//GigE��������ł�In3�{�܂�
			{
				bReval = FALSE;
			}
			if( nOutCount>5 ) //GigE��������ł�Out5�{�܂�
			{
				bReval = FALSE;
			}

			wCameraInMask = (1<<nInCount)-1;
			wCameraOutMask = (1<<nOutCount)-1;

			//����ւ̏����ݒ� ----- (GigE��������𗬗p)
			//���ɃJ�����̎d�l�𑗐M(0:Standard or OptionType 1:) 
			//�J�������猩���Ƃ��̓��o��
			//OptionType
			//Sp0:IN  Sp1:IN  Sp2:IN    Sp3:OUT  Sp4:OUT  Sp5:OUT  Sp6:OUT  Sp7:OUT  
			//StandardType
			//Sp0:IN  Sp1:--- Sp2:---   Sp3:OUT  Sp4:OUT  Sp5:---  Sp6:---  Sp7:---  
			CString szString;
			//szString.Format(_T("0xF002"));	// OptionType�ŃZ�b�g
			//bReval = m_pSerialComm->SendData(szString);
			BYTE byteSendInit[2] = { 0xF0, 0x02 };
			BYTE byteRcvInit[2] = { 0, 0 };
			size_t rcvSizeInit = sizeof(byteRcvInit);
			bReval = m_pSerialComm->SendBin( byteSendInit, sizeof(byteSendInit), byteRcvInit, &rcvSizeInit );
			if( !bReval )
				break;
			if( byteRcvInit[0]!=0xF0 || byteRcvInit[1]!=0x01 )
				break;

			//�ŏ��ɂO��ݒ肵�Ă���
			//szString.Format(_T("0xF100"));	//
			//bReval = m_pSerialComm->SendData(szString);
			//if( !bReval )
			//	break;
//			{
//
//				BYTE byteSend[2] = { 0xF1, 0x00 };
//				BYTE byteRcv[2] = { 0, 0 };
//				size_t rcvSize = sizeof(byteRcv);
//				bReval = m_pSerialComm->SendBin( byteSend, sizeof(byteSend), byteRcv, &rcvSize );
//TRACE(TEXT("@@@ byteRcv(%x:%x)\n"),byteRcv[0],byteRcv[1]);
//				if( !bReval )
//					break;
//			}

			//-------------------------------------------------------
			WORD wCameraStatusMask = (1<<m_nPinCount)-1;

			WORD	wSetCameraStatus;
			WORD wLoop = m_nPinCount * 2;
			for(WORD wStatus=0;wStatus<wLoop;wStatus++)
			{

				if( wStatus<m_nPinCount ) wSetCameraStatus = (1<<wStatus);
				else					  wSetCameraStatus = ~(1<<(wLoop-wStatus));
				wSetCameraStatus &= wCameraStatusMask;

				//�J�����ւ̐ݒ�------
				INT nOut=0;
				for( int i=0; i<m_nPinCount; i++ )
				{
					if( m_piPinMode[i]==1 )	//Out�̏ꍇ
					{
						szString.Format(_T("UserOutput%d"), nOut++);
						bReval = aDevice->SetGenICamControl( _T("UserOutputSelector"), szString);
						if( bReval )
						{
							bool bValue = ((wSetCameraStatus>>i)&1);
							bReval = aDevice->SetGenICamControl( _T("UserOutputValue"), bValue);
						}
					}
				}

				//��1.0.0.1058
				//Sleep(1);
				Sleep(m_nCheckWaitTime);
				//��1.0.0.1058

				//���[�q�ւ̓��o��------
				BYTE byteJiguSetValue = 0;
				INT nIn=0;
				for( int i=0; i<m_nPinCount; i++ )
				{
					if( m_piPinMode[i]==0 )	//In�̏ꍇ
					{
						byteJiguSetValue |= ((wSetCameraStatus>>i)&1)<<nIn++;
					}
				}
				BYTE byteSend[2] = { 0xF1, byteJiguSetValue };
				BYTE byteRcv[2] = { 0, 0 };
				size_t rcvSize = sizeof(byteRcv);
				bReval = m_pSerialComm->SendBin( byteSend, sizeof(byteSend), byteRcv, &rcvSize );
				if( byteRcv[0]!=0xF1 ) bReval = FALSE;
				if( rcvSize!=2 ) bReval = FALSE;
				if( !bReval )
					break;


				WORD wGetCameraStatus = 0;
				//---�J����Output���`�F�b�N-------------------------------------------------------------------------------
				//byteRcv[1]��Out��񂪓����Ă���
				//nOut=0;
				for( int i=0; i<m_nPinCount; i++ )
				{
					if( m_piPinMode[i]==0 )	//In�̏ꍇ
					{
						CString szSelector;
						szSelector.Format(_T("Line%d"),i);
						aDevice->SetGenICamControl( _T("LineSelector"), szSelector);
						bool bValue = false;
						bReval = aDevice->GetGenICamControl( _T("LineStatus"), bValue );
						if( bReval )
						{
							wGetCameraStatus |= (bValue?0:1)<<i;	//�ɐ����]
						}
					}
					else
					{
						bool bValue = ((byteRcv[1]>>i)&1);
						wGetCameraStatus |= (bValue?0:1)<<i;	//�ɐ����]
					}
				}




				//BYTE byteJiguInValue = byteRcv[1]&(wCameraOutMask<<3);
				////byteJiguInValue��XOR������(�ɐ����t) ---------------------- �K�v���H�H�H�H
				//byteJiguInValue ^= (wCameraOutMask<<3);

				//�J�����̏o�͂����������ǂ����m�F
				if(wSetCameraStatus != wGetCameraStatus)
				{
					m_iJudge = 0;
					//��1.0.0.1053
					m_nJiguSetValue = -1;
					m_nJiguStatus = -1;
					m_nCameraSetValue = wSetCameraStatus;
					m_nCameraStatus = wGetCameraStatus;
					//��1.0.0.1053
					break;
				}
			}
			//-------------------------------------------------------


			//�S��������ꍇ
			/*
			DWORD dwLoop = 1<<m_nPinCount;
			for( int i=0; i<dwLoop; i++ )
			{
				//�J�����ւ̐ݒ�
				DWORD dwOutputData = dwLoop>>nInCount;
				for( int j=0; j<nOutCount && bReval; j++ )
				{
					szString.Format(_T("UserOutput%d"), nOutCount);
					bReval = aDevice->SetGenICamControl( _T("UserOutputSelector"), szString);
					if( bReval )
					{
						bool bValue = ((dwOutputData>>j)&1);
						bReval = aDevice->SetGenICamControl( _T("UserOutputValue"), (bool)((dwOutputData>>j)&1));
					}
				}
				if( !bReval ) break;

				Sleep(1);

				//���[�q�ւ̓��o��
				BYTE byteJiguSetValue=(BYTE)(i&wCameraInMask);
				BYTE byteSend[2] = { 0xF1, byteJiguSetValue };
				BYTE byteRcv[2] = { 0, 0 };
				size_t rcvSize = sizeof(byteRcv);
				bReval = m_pSerialComm->SendBin( byteSend, sizeof(byteSend), byteRcv, &rcvSize );
				if( byteRcv[0]!=0xF1 ) bReval = FALSE;
				if( rcvSize!=2 ) bReval = FALSE;
				if( !bReval ) break;


				//---�J����Output���`�F�b�N-------------------------------------------------------------------------------
				BYTE byteJiguInValue = byteRcv[1]&(wCameraOutMask<<3);
				//byteJiguInValue��XOR������(�ɐ����t) ---------------------- �K�v���H�H�H�H
				byteJiguInValue ^= (wCameraOutMask<<3);

				//�J�����̏o�͂����������ǂ����m�F
				if(dwOutputData != byteJiguInValue)
				{
					m_iJudge = 0;
					break;
				}

				//----�J����Input���`�F�b�N------------------------------------------------------------------------------
				//�J�����[�q���擾
				BYTE byteCurCameraStatus = 0x00;
				INT iInputCount=0;
				for( int j=0; j<m_nPinCount && bReval; j++ )
				{
					if( m_piPinMode[j]==0 )	//IN
					{
						szString.Format(_T("Line%d"), j);
						bReval = aDevice->SetGenICamControl( _T("LineSelector"), szString);
						if( !bReval ) break;
						bool bValue;
						bReval = aDevice->GetGenICamControl( _T("LineStatus"), bValue);
						if( !bReval ) break;
						if( bValue )
						{
							byteCurCameraStatus |= 1<<iInputCount;
						}
						iInputCount++;
					}
				}
				if( !bReval ) break;

				BYTE byteCameraInValue = byteCurCameraStatus & wCameraInMask;
				//byteCameraInValue��XOR������(�ɐ����t) ---------------------- �K�v���H�H�H�H
				//byteCameraInValue ^= wCameraInMask;
				if(byteCameraInValue != byteJiguSetValue)
				{
					m_iJudge = 0;
					break;
				}
			}
			*/
		}
		else
		{

		}
	}while(0);


	if( m_iJudge==2 ) m_iJudge = 1;	//1:OK
	if( !bReval )
	{
		m_iJudge = 0;
	}

	return bReval;
}

BOOL StCheckIO::CreatePinBuffer(void)
{
	BOOL bReval = FALSE;
	if( m_nPinCount>0 )
	{
		if( m_piPinMode )
		{
			delete [] m_piPinMode;
			m_piPinMode = NULL;
		}
		m_piPinMode = new INT [m_nPinCount];
		if( m_piPinMode )
		{
			memset(m_piPinMode,0, sizeof(INT)*m_nPinCount);
			bReval = TRUE;
		}
	}
	return bReval;
}

BOOL StCheckIO::function(LPCTSTR szFunc, LPCTSTR szArgument)
{
	BOOL bReval = FALSE;
	CString szFunction(szFunc);

	

	if( szFunction=="SetLineCount" )
	{
		m_nPinCount = _tcstol(szArgument,NULL,0);
		CreatePinBuffer();
		bReval = TRUE;
	}
	//��1.0.0.1053
	else
	if( szFunction=="SetMode" || szFunction=="Mode" )
	{
		m_nMode = _tcstol(szArgument,NULL,0);
		bReval = TRUE;
	}
	else
	if( szFunction=="SetCheckWaitTime" || szFunction=="CheckWaitTime" )
	{
		m_nCheckWaitTime = _tcstol(szArgument,NULL,0);
		bReval = TRUE;
	}
	//��1.0.0.1053
	else
	{
		CString szSetLineMode(_T("SetLineMode"));
		if(szFunction.Left(szSetLineMode.GetLength())==szSetLineMode)
		{
			INT nIndex = _ttoi(szFunction.Mid(szSetLineMode.GetLength()));
			if( nIndex<m_nPinCount )
			{
				if( _tcscmp(szArgument,_T("IN"))==0 )
				{
					m_piPinMode[nIndex] = 0;
				}
				else
				if( _tcscmp(szArgument,_T("OUT"))==0 )
				{
					m_piPinMode[nIndex] = 1;
				}
				else
				{
					m_piPinMode[nIndex] = _ttoi(szArgument);
				}
				bReval = TRUE;
			}
		}
	}

	return bReval;

}

//��1.0.0.1053
BOOL StCheckIO::TerminalExecute(void)
{
	if( !m_pTerminal ) return FALSE;
	BOOL bReval = FALSE;
	m_iJudge = 2;	//2:������

	do
	{
		if( m_DeviceBase->m_ClassName=="StDeviceEBus" )	//GigE�p
		{

			TCHAR szText[128];
			StDeviceEBus *aDevice = (StDeviceEBus *)m_DeviceBase;

			INT nInCount = 0;
			INT nOutCount = 0;
			WORD wCameraInMask = 0;
			WORD wCameraOutMask = 0;

			for( int i=0; i<m_nPinCount; i++ )
			{

				//�J�����ւ̏����ݒ�
//TRACE( _T("@@@ m_nPinCount=%i\n"), i );

				CString szSelector;
				szSelector.Format(_T("Line%d"),i);
				aDevice->SetGenICamControl( _T("LineSelector"), szSelector);

			
				//�������ɂ�UserOutput�ύX�݂̂őΉ�������
				bReval = aDevice->GetGenICamControl( _T("LineMode"), szText, _countof(szText));
				if( !bReval )
					break;
				TCHAR szLineMode[128];
				if( m_piPinMode[i]==0 )
					_tcscpy_s( szLineMode, _countof(szLineMode), _T("Input") );
				else
					_tcscpy_s( szLineMode, _countof(szLineMode), _T("Output") );

				if( _tcscmp(szText,szLineMode)!=0 )
				{
					bReval = aDevice->SetGenICamControl( _T("LineMode"), szLineMode);
					if( !bReval ) break;
				}

				CString szString;
				if( m_piPinMode[i]==1 )	//Output
				{
					szString.Format(_T("UserOutput%d"), nOutCount++);
					bReval = aDevice->SetGenICamControl( _T("LineSource"), szString);
					//szString.Format(_T("UserOutput%d"), nOutCount);
					if( bReval )
						bReval = aDevice->SetGenICamControl( _T("UserOutputSelector"), szString);
					if( bReval )
						bReval = aDevice->SetGenICamControl( _T("UserOutputValue"), false);
				}

				////�{���`�F�b�N
				//if( m_piPinMode[i]==0 )
				//{
				//	nInCount++;
				//}
				//else
				//if( m_piPinMode[i]==1 )
				//{
				//	nOutCount++;
				//}
			}
			if( !bReval )
				break;

			//if( nInCount>3 )	//GigE��������ł�In3�{�܂�
			//{
			//	bReval = FALSE;
			//}
			//if( nOutCount>5 ) //GigE��������ł�Out5�{�܂�
			//{
			//	bReval = FALSE;
			//}

			//wCameraInMask = (1<<nInCount)-1;
			//wCameraOutMask = (1<<nOutCount)-1;

			//����ւ̏����ݒ� ----- (GigE��������𗬗p)
			//���ɃJ�����̎d�l�𑗐M(0:Standard or OptionType 1:) 
			//�J�������猩���Ƃ��̓��o��
			//OptionType
			//Sp0:IN  Sp1:IN  Sp2:IN    Sp3:OUT  Sp4:OUT  Sp5:OUT  Sp6:OUT  Sp7:OUT  
			//StandardType
			//Sp0:IN  Sp1:--- Sp2:---   Sp3:OUT  Sp4:OUT  Sp5:---  Sp6:---  Sp7:---  
			CString szString;
			//szString.Format(_T("0xF002"));	// OptionType�ŃZ�b�g
			//bReval = m_pSerialComm->SendData(szString);
			//BYTE byteSendInit[2] = { 0xF0, 0x02 };
			//BYTE byteRcvInit[2] = { 0, 0 };
			//size_t rcvSizeInit = sizeof(byteRcvInit);
			//bReval = m_pSerialComm->SendBin( byteSendInit, sizeof(byteSendInit), byteRcvInit, &rcvSizeInit );
			//if( !bReval )
			//	break;
			//if( byteRcvInit[0]!=0xF0 || byteRcvInit[1]!=0x01 )
			//	break;


			//���[�q�ւ̓��o�͏����ݒ�------
			_stprintf_s(szText,_countof(szText),_T("%i"),m_nPinCount);
			m_pTerminal->function(_T("IOPortCount"),szText);

			//��1.0.0.1073 beta2
			//m_pTerminal->function(_T("IOPortCount"),szText);
			bReval = m_pTerminal->function(_T("IOPortCount"),szText);
			if( !bReval )
			{
				m_szLastErrorMessage = m_pTerminal->GetErrorMessage();
				break;
			}
			//��1.0.0.1073 beta2


			//��1.0.0.1073 beta2
			//INT nAllPort = 0;
			//for( int i=0; i<m_nPinCount; i++ )
			//{
			//	if( m_piPinMode[i]==0 )	//�J��������݂�IN�@JIGU���猩��Out
			//	{
			//		nAllPort |= (1<<i);
			//	}
			//}
			//_stprintf_s(szText,_countof(szText),_T("%i"),nAllPort);
			//m_pTerminal->function(_T("SetAllport"),szText);


			//����Ȃ�--------------

			//bReval = m_pTerminal->function(_T("SetAllOutportValue"),szText);
			//if( !bReval )
			//{
			//	m_szLastErrorMessage = m_pTerminal->GetErrorMessage();
			//	break;
			//}
			//��1.0.0.1073 beta2
			//
			WORD	wSetCameraStatus;
			//��1.0.0.1073 beta2
			//for( INT nReverse=0; nReverse<2 && m_iJudge==2; nReverse++ )	//���]
			for( INT nReverse=0; nReverse<2 && m_iJudge==2 && bReval; nReverse++ )	//���]
			//��1.0.0.1073 beta2
			{
				for( INT nPin=0; nPin<m_nPinCount && m_iJudge==2; nPin++ )		//PIN
				{
					wSetCameraStatus = (1<<nPin);
					if( nReverse>0 ) wSetCameraStatus = ~wSetCameraStatus;

					//�J�����̏o�͐ݒ�------
					INT nCameraSetValue = 0;
					INT nOutIndex = 0;
					for( int i=0; i<m_nPinCount; i++ )
					{
						if( m_piPinMode[i]==1 )	//Out�̏ꍇ
						{
							nCameraSetValue |= (wSetCameraStatus&(1<<i));
							szString.Format(_T("UserOutput%d"), nOutIndex++);
							bReval = aDevice->SetGenICamControl( _T("UserOutputSelector"), szString);
							if( bReval )
							{
								bool bValue = ((wSetCameraStatus>>i)&1);
								bReval = aDevice->SetGenICamControl( _T("UserOutputValue"), bValue);
							}
						}
					}
					//Sleep(1);
					//���[�q�̏o�͐ݒ�------
					INT nJiguSetValue = 0;
					for( int i=0; i<m_nPinCount; i++ )
					{
						if( m_piPinMode[i]==0 )	//In�̏ꍇ�@JIGU����݂���OUT
						{
							//byteJiguSetValue |= ((wSetCameraStatus>>i)&1)<<i;
							nJiguSetValue |= wSetCameraStatus&(1<<i);
						}
					}
					_stprintf_s(szText,_countof(szText),_T("%i"),nJiguSetValue);
					//��1.0.0.1073 beta2
					//m_pTerminal->ExecutePublic(_T("SetAllOutportValue"),szText);
					bReval = m_pTerminal->ExecutePublic(_T("SetAllOutportValue"),szText);
					if( !bReval )
					{
						m_szLastErrorMessage = m_pTerminal->GetErrorMessage();
						break;
					}
					//��1.0.0.1073 beta2

					//��1.0.0.1058
					//Sleep(500);
					Sleep(m_nCheckWaitTime);
					//��1.0.0.1058
					//���̓��͎擾
					INT nJiguStatusValue = 0;
					//��1.0.0.1072 beta2
					//JiguStatus�����������ꍇ�����邽�߂Q����
					bReval = m_pTerminal->GetFunction(_T("GetAllport"), NULL, nJiguStatusValue);
					//��1.0.0.1072 beta2
					bReval = m_pTerminal->GetFunction(_T("GetAllport"), NULL, nJiguStatusValue);
					//��1.0.0.1073 beta2
					if( !bReval )
					{
						m_szLastErrorMessage = m_pTerminal->GetErrorMessage();
						break;
					}
					//��1.0.0.1073 beta2

					//Mask����
					for( int i=0; i<m_nPinCount; i++ )
					{
						if( m_piPinMode[i]==0 )	//IN�̏ꍇ�@JIGU����݂���OUT
						{
							nJiguStatusValue &= ~(1<<i);
						}
					}

					//�J�����̓��͎擾
					INT nCameraStatusValue = 0;
					bReval = aDevice->GetGenICamControl( _T("LineStatusAll"), nCameraStatusValue);
					//Mask����
					for( int i=0; i<m_nPinCount; i++ )
					{
						if( m_piPinMode[i]==1 )	//OUT�̏ꍇ�@JIGU����݂���IN
						{
							nCameraStatusValue &= ~(1<<i);
						}
					}


					//�J�����̏o�͂����������ǂ����m�F
					if(nJiguStatusValue != nCameraSetValue)
					{
						m_iJudge = 0;
					}
					//�J�����̓��͂����������ǂ����m�F
					if(nJiguSetValue != nCameraStatusValue)
					{
						m_iJudge = 0;
					}

					if( m_iJudge==0 )
					{
						m_nCameraStatus = nCameraStatusValue;
						m_nCameraSetValue = nCameraSetValue;
						m_nJiguStatus = nJiguStatusValue;
						m_nJiguSetValue = nJiguSetValue;
						break;
					}
				}
			}


			/*
			//-------------------------------------------------------
			WORD wCameraStatusMask = (1<<m_nPinCount)-1;

			WORD wLoop = m_nPinCount * 2;
			for(WORD wStatus=0;wStatus<wLoop;wStatus++)
			{

				if( wStatus<m_nPinCount ) wSetCameraStatus = (1<<wStatus);
				else					  wSetCameraStatus = ~(1<<(wLoop-wStatus));
				wSetCameraStatus &= wCameraStatusMask;

				//�J�����ւ̐ݒ�------
				INT nOut=0;
				for( int i=0; i<m_nPinCount; i++ )
				{
					if( m_piPinMode[i]==1 )	//Out�̏ꍇ
					{
						szString.Format(_T("UserOutput%d"), nOut++);
						bReval = aDevice->SetGenICamControl( _T("UserOutputSelector"), szString);
						if( bReval )
						{
							bool bValue = ((wSetCameraStatus>>i)&1);
							bReval = aDevice->SetGenICamControl( _T("UserOutputValue"), bValue);
						}
					}

				}

				Sleep(1);

				//���[�q�ւ̓��o��------
				BYTE byteJiguSetValue = 0;
				INT nIn=0;
				for( int i=0; i<m_nPinCount; i++ )
				{
					if( m_piPinMode[i]==0 )	//In�̏ꍇ
					{
						byteJiguSetValue |= ((wSetCameraStatus>>i)&1)<<nIn++;
					}
				}
				BYTE byteSend[2] = { 0xF1, byteJiguSetValue };
				BYTE byteRcv[2] = { 0, 0 };
				size_t rcvSize = sizeof(byteRcv);
				bReval = m_pSerialComm->SendBin( byteSend, sizeof(byteSend), byteRcv, &rcvSize );
				if( byteRcv[0]!=0xF1 ) bReval = FALSE;
				if( rcvSize!=2 ) bReval = FALSE;
				if( !bReval )
					break;


				WORD wGetCameraStatus = 0;
				//---�J����Output���`�F�b�N-------------------------------------------------------------------------------
				//byteRcv[1]��Out��񂪓����Ă���
				//nOut=0;
				for( int i=0; i<m_nPinCount; i++ )
				{
					if( m_piPinMode[i]==0 )	//In�̏ꍇ
					{
						CString szSelector;
						szSelector.Format(_T("Line%d"),i);
						aDevice->SetGenICamControl( _T("LineSelector"), szSelector);
						bool bValue = false;
						bReval = aDevice->GetGenICamControl( _T("LineStatus"), bValue );
						if( bReval )
						{
							wGetCameraStatus |= (bValue?0:1)<<i;	//�ɐ����]
						}
					}
					else
					{
						bool bValue = ((byteRcv[1]>>i)&1);
						wGetCameraStatus |= (bValue?0:1)<<i;	//�ɐ����]
					}
				}




				//BYTE byteJiguInValue = byteRcv[1]&(wCameraOutMask<<3);
				////byteJiguInValue��XOR������(�ɐ����t) ---------------------- �K�v���H�H�H�H
				//byteJiguInValue ^= (wCameraOutMask<<3);

				//�J�����̏o�͂����������ǂ����m�F
				if(wSetCameraStatus != wGetCameraStatus)
				{
					m_iJudge = 0;
					break;
				}
			}
			*/
		}
		else
		{

		}
	}while(0);


	if( m_iJudge==2 ) m_iJudge = 1;	//1:OK
	if( !bReval )
	{
		m_iJudge = 0;
	}

	return bReval;
}

BOOL StCheckIO::GetFunction(LPCTSTR szFunc, LPCTSTR szArg, double &dblValue)
{
	BOOL bReval = FALSE;
	CString szFunction(szFunc);
	if( szFunction=="GetCameraStatus" || szFunction=="CameraStatus")
	{
		dblValue = m_nCameraStatus;
		bReval = TRUE;
	}
	else
	if( szFunction=="GetCameraSetValue" || szFunction=="CameraSetValue" )
	{
		dblValue = m_nCameraSetValue;
		bReval = TRUE;
	}
	else
	if( szFunction=="GetJiguStatus" || szFunction=="JiguStatus" )
	{
		dblValue = m_nJiguStatus;
		bReval = TRUE;
	}
	else
	if( szFunction=="GetJiguSetValue" || szFunction=="JiguSetValue" )
	{
		dblValue = m_nJiguSetValue;
		bReval = TRUE;
	}
	return bReval;
}

//��1.0.0.1053
