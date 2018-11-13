#include "StdAfx.h"
#include "StInspectMain.h"
#include "Resource.h"
#include "StGenFactoryScriptDlg.h"
#include <locale.h>
#include "../CommonHeader/StTimed.h"
#include <PvDeviceFinderWnd.h>

#include "StCommon.h"
#include "ControlDlg.h"


#ifdef _PLEORA
//#include <PvDevice.h>


#else
//��1.0.0.73 GenApi3.0
//#include "../CommonHeader/StGenTL.h"
//#pragma comment(lib,"StGenICamLib.lib")
//
//#pragma comment(lib,"GCBase_MD_VC80_v2_4.lib")
//#pragma comment(lib,"GenApi_MD_VC80_v2_4.lib")
//��1.0.0.73 GenApi3.0
#endif

#include "StCheckSN.h"
#include "StCheckControlRetry.h"
#include "StInputSerialDlg.h"
#include "StCheckSNDlg.h"
#include "StCheckSpBehavior.h"
#include "StCheckDataLose.h"
//��1.0.0.1062
#include "StCheckDataLoseDlg.h"
//��1.0.0.1062
#include "StCheckPixelDefect.h"
#include "StCheckDustDlg.h"
#include "StCheckMessage.h"
#include "StImageMessageDlg.h"
//��1.0.0.1048
#include "StHttpControlDlg.h"
//��1.0.0.1048
#include "StCheckAdjustLinetolineDlg.h"
#include "StCheckAdjustLinetoline.h"
#include "StCheckEffectivePixel.h"
#include "StInputSensorVersionDlg.h"
#include "StInputSensorSerialDlg.h"
#include "StLightControlDlg.h"
#include "StLightControl.h"
#include "StCheckPower.h"
#include "StCheckCommon.h"
#include "StCheckSerialVersion.h"
#include "StCheckIO.h"
#include "StPTouch.h"
//��1.0.0.1049
#include "StCheckAdjustGainDlg.h"
//��1.0.0.1049
//��1.0.0.1015
#include "StWriteSerial.h"
//��1.0.0.1015
//��1.0.0.1058
#include "StWriteFileData.h"
//��1.0.0.1058
//��1.0.0.1017
#include "StTerminal.h"
//��1.0.0.1017
//��1.0.0.1020
#include "StCheckPixelCorrect.h"
//��1.0.0.1020
//��1.0.0.1021
#include "StCheckTemperature.h"
#include "StCheckTestPattern.h"
//��1.0.0.1021
//��1.0.0.1022
#include "StCheckEllitoLVDSDlg.h"
//��1.0.0.1022
//��1.0.0.1059
#include "StCheckEllitoLVDS.h"
//��1.0.0.1059
//��1.0.0.1023
#include "StLoop.h"
//��1.0.0.1023
//��1.0.0.1025
#include "StFileOperation.h"
//��1.0.0.1025
//��1.0.0.1051
#include "StInterfaceGenICam.h"
//��1.0.0.1051
//��1.0.0.1030
#include "StIF.h"
//��1.0.0.1030
//��1.0.0.1033
#include "StCheckFramerate.h"
//��1.0.0.1033
//��1.0.0.1040
#include "StCheckCSVDust.h"
//��1.0.0.1040
//��1.0.0.1044
#include "StCheckAdjustOffset.h"
//��1.0.0.1044
//��1.0.0.1046
#include "StCameraControlDlg.h"
//��1.0.0.1046
//��1.0.0.1047
#include "StCheckSwitchDlg.h"
//��1.0.0.1047
//��1.0.0.1048
#include "StCheckAdjustVolumeDlg.h"
//��1.0.0.1048


typedef struct _Dialog_Thread_Param
{
	void *pDlg;
	RECT aWindowRect;
	BOOL bReval;
}Dialog_Thread_Param, *PDialog_Thread_Param;


//=====================================================================
//Dialog�X���b�h
//=====================================================================
/*
unsigned __stdcall LightControlDialogThread( void *pParam )
{
	Dialog_Thread_Param *aDlgThreadParam = (PDialog_Thread_Param)pParam;

	RECT lWindowRect = aDlgThreadParam->aWindowRect;
	StLightControlDlg * pStLightControlDlg = (StLightControlDlg *)aDlgThreadParam->pDlg;
	INT nSizeX = lWindowRect.right - lWindowRect.left;
	INT nSizeY = lWindowRect.bottom - lWindowRect.top;
	pStLightControlDlg->SetWindowPos( &CWnd::wndTop, 0, 0, nSizeX, nSizeY, SWP_NOZORDER|SWP_SHOWWINDOW );
	pStLightControlDlg->ShowWindow(SW_SHOW);
	::ShowWindowAsync(pStLightControlDlg->m_hWnd,SW_SHOW);

	Sleep(1000);

	BOOL bReval = pStLightControlDlg->Execute();

	pStLightControlDlg->ShowWindow(SW_HIDE);

	_endthreadex(0);
	return 0;
}
*/






//=====================================================================
//�N���X�̃e�[�u��
//=====================================================================















//typedef struct _S_CLASS_TABLE
//{
//	LPCTSTR szID;
//	void (*fpFunc)();
//	//class fpFunc;
//}S_CLASS_TABLE, *PS_CLASS_TABLE;
//
//template<typename T>
//struct S_CLASS
//{
//	LPCTSTR szID;
//    typename T;
//}

//S_CLASS_TABLE gCheckBaseTable[] =
//{
//	_T("StCheckSN"), StCheckSN,
//
//}

//��1.0.0.1067a
#define _SLEEP(x) StSleepEx(x)

int StSleepEx(unsigned long x)
{
	clock_t  s = clock();
	clock_t  c;
	do{
		if ((c = clock()) == (clock_t)-1)       /* �G���[ */
			return (0);
	}while(1000UL * (c - s) / CLOCKS_PER_SEC <= x); 
	return(1);
}
//��1.0.0.1067a

//��1.0.0.1015
unsigned __stdcall sleepThread( void *pParam )
{
	INT * aTimer = (INT *)pParam;
	//��1.0.0.1067a
	//Sleep(*aTimer);
	_SLEEP(*aTimer);
	//��1.0.0.1067a
	_endthreadex(0);
	return 0;
}
/*
typedef struct _ImageMessageParam
{
	StImageMessageDlg * pImageMessageDlg;
	BOOL bOpen;
	BOOL bClose;
}ImageMessageParam, *PImageMessageParam;

unsigned __stdcall ImageMessageThread( void *pParam )
{
	ImageMessageParam * pImageMessageDlg = (ImageMessageParam *)pParam;

	pImageMessageDlg->Create( IDD_DIALOG_IMAGEMESSAGE, NULL );
	INT nSizeX = pImageMessageDlg->GetImageSizeX() + pImageMessageDlg->GetMessageSizeX() + 30;
	INT nSizeY = max( pImageMessageDlg->GetImageSizeX(), pImageMessageDlg->GetMessageSizeX() ) + 20;
	pImageMessageDlg->SetWindowPos( NULL, pImageMessageDlg->GetWindowPosX(), pImageMessageDlg->GetWindowPosY(), nSizeX, nSizeY, SWP_NOZORDER );

	do
	{




	}while(1);


				if( szSection=="open" )
				{
					if( !pImageMessageDlg->m_hWnd )
					{
						pImageMessageDlg->Create( IDD_DIALOG_IMAGEMESSAGE, NULL );
						INT nSizeX = pImageMessageDlg->GetImageSizeX() + pImageMessageDlg->GetMessageSizeX() + 30;
						INT nSizeY = max( pImageMessageDlg->GetImageSizeX(), pImageMessageDlg->GetMessageSizeX() ) + 20;

						pImageMessageDlg->SetWindowPos( NULL, pImageMessageDlg->GetWindowPosX(), pImageMessageDlg->GetWindowPosY(), nSizeX, nSizeY, SWP_NOZORDER );
					}
					bReval = pImageMessageDlg->ShowWindow(SW_SHOW);
					pImageMessageDlg->Invalidate();
				}
				else
				if( szSection=="close" )
				{
					bReval = pImageMessageDlg->ShowWindow(SW_HIDE);
				}






	Sleep(*aTimer);

	_endthreadex(0);
	return 0;
}
*/
//��1.0.0.1015

//��1.0.0.1048
void DoEvents()
{
	MSG msg;
	while(::PeekMessage(&msg,NULL,NULL,NULL,PM_NOREMOVE) )
	{
		if(!AfxGetApp()->PumpMessage() )
			break;
	}
}
//��1.0.0.1048


//��1.0.0.1062
#define _StDeviceEBus _T("StDeviceEBus")
#define _StDeviceMultiCam _T("StDeviceMultiCam")
#define _StDeviceGenICam _T("StDeviceGenICam")
#define _StSerialComm _T("StSerialComm")
#define _StTerminal _T("StTerminal")
#define _StTexioControl _T("StTexioControl")
#define _StImageMessageDlg _T("StImageMessageDlg")
#define _StHttpControlDlg _T("StHttpControlDlg")
#define _StValue _T("StValue")
#define _StFileOperation _T("StFileOperation")
#define _StInterfaceGenICam _T("StInterfaceGenICam")
//��1.0.0.1062

//��1.0.0.1025
#define _StCheckSN _T("StCheckSN")
#define _StCheckControlRetry _T("StCheckControlRetry")
#define _StInputSerialDlg _T("StInputSerialDlg")
#define _StCheckSNDlg _T("StCheckSNDlg")
#define _StCheckSpBehavior _T("StCheckSpBehavior")
#define _StCheckDataLose _T("StCheckDataLose")
//��1.0.0.1062
#define _StCheckDataLoseDlg _T("StCheckDataLoseDlg")
//��1.0.0.1062
#define _StCheckPixelDefect _T("StCheckPixelDefect")
#define _StCheckDustDlg _T("StCheckDustDlg")
#define _StCheckMessage _T("StCheckMessage")
#define _StCheckAdjustLinetolineDlg _T("StCheckAdjustLinetolineDlg")
#define _StCheckAdjustLinetoline _T("StCheckAdjustLinetoline")
#define _StCheckEffectivePixel _T("StCheckEffectivePixel")
#define _StInputSensorVersionDlg _T("StInputSensorVersionDlg")
#define _StInputSensorSerialDlg _T("StInputSensorSerialDlg")
#define _StLightControlDlg _T("StLightControlDlg")
#define _StLightControl _T("StLightControl")
#define _StCheckPower _T("StCheckPower")
#define _StCheckCommon _T("StCheckCommon")
#define _StCheckSerialVersion _T("StCheckSerialVersion")
#define _StCheckIO _T("StCheckIO")
#define _StPTouch _T("StPTouch")
#define _StWriteSerial _T("StWriteSerial")
//��1.0.0.1058
#define _StWriteFileData _T("StWriteFileData")
//��1.0.0.1058
#define _StCheckPixelCorrect _T("StCheckPixelCorrect")
#define _StCheckTemperature _T("StCheckTemperature")
#define _StCheckTestPattern _T("StCheckTestPattern")
#define _StCheckEllitoLVDSDlg _T("StCheckEllitoLVDSDlg")
//��1.0.0.1059
#define _StCheckEllitoLVDS _T("StCheckEllitoLVDS")
//��1.0.0.1059
#define _StLoop _T("StLoop")
//��1.0.0.1049
#define _StCheckAdjustGainDlg _T("StCheckAdjustGainDlg")
//��1.0.0.1049

//��1.0.0.1025
//��1.0.0.1030
#define _StIF _T("StIF")
//��1.0.0.1030
//��1.0.0.1033
#define _StCheckFramerate _T("StCheckFramerate")
//��1.0.0.1033
//��1.0.0.1040
#define _StCheckCSVDust _T("StCheckCSVDust")
//��1.0.0.1040
//��1.0.0.1044
#define _StCheckAdjustOffset _T("StCheckAdjustOffset")
//��1.0.0.1044
//��1.0.0.1046
#define _StCameraControlDlg _T("StCameraControlDlg")
//��1.0.0.1046
//��1.0.0.1047
#define _StCheckSwitchDlg _T("StCheckSwitchDlg")
//��1.0.0.1047
//��1.0.0.1048
#define _StCheckAdjustVolumeDlg _T("StCheckAdjustVolumeDlg")
//��1.0.0.1048



//=====================================================================


StInspectMain::StInspectMain(void *pParent)
	: mpParent(pParent)
{
	mHandle = INVALID_HANDLE_VALUE;
	mID = 0;

	TCHAR szPath[MAX_PATH];
	CStCommon::GetIniFilePath( szPath, _countof(szPath) );
	mSaveFilePath = szPath;
	if( mSaveFilePath.Right(1) ==_T("\\") || mSaveFilePath.Right(1) ==_T("/") )
		mSaveFilePath = mSaveFilePath.Left(mSaveFilePath.GetLength()-1);

	szErrorMessage = _T("");

	mCheckMode = 0;
	mFactoryMode = 0;

	m_nWriteScriptLogMode = 0;

	//��1.0.0.1008
	m_nRetryMode = 0;
	m_nNGContinuousMode = 0;
	//��1.0.0.1008

	m_pStructPortName = NULL;

	//��1.00.1017
	m_pStructAddress = NULL;
	//��1.00.1017

	//��1.00.1020
	m_pStructLightChannel = NULL;
	//��1.00.1020

	//Ptouch
	m_bExecutePrintPTouch = FALSE;

	//��1.0.0.1009 �r�������̈�
	m_DisplayBufferMutex = CreateMutex(NULL,FALSE,NULL);
	//��1.0.0.1009


	//��1.0.0.1013 �X�N���v�g�I�������b�Z�[�W�_�C�A���O��\�����邩�ǂ����̃t���O
	m_nNoOKMessage = 0;
	m_nNoNGMessage = 0;
	//��1.0.0.1013

	//��1.0.0.1026
	m_nNextLine = -1;
	//��1.0.0.1026

	//��1.0.0.1047
	m_nRuledLine = 200;
	//��1.0.0.1047
	//��1.0.0.1062
	m_nScriptCheckMode = 0;
	//m_pErrorListDlg = NULL;
	//��1.0.0.1062

	//��1.0.0.1072d
#ifdef GENCOMP_OLD
#else
	m_szResultGenICamControlCommandName = _T("");
	m_nResultGenICamControlValueMode = -1;
	m_szResultGenICamControlValue = _T("");
	m_dblResultGenICamControlValue = 0.0;
#endif
	//��1.0.0.1072d
}


StInspectMain::~StInspectMain(void)
{

	//��1.0.0.1009 �r�������̈�
	CloseHandle(m_DisplayBufferMutex);
	//��1.0.0.1009

	m_vScriptString.clear();

	//��1.0.0.1062
	m_vScriptCheckErrorText.clear();
	//if( m_pErrorListDlg )
	//{
	//	delete m_pErrorListDlg;
	//}
	//��1.0.0.1062

	AllDelete();

	CStGenFactoryScriptDlg *pWnd = (CStGenFactoryScriptDlg *)mpParent;
	pWnd->DeletePreview();
	pWnd->DeleteWaveform();

}

// ==========================================================================
//LOG�t�@�C���֘A
// ==========================================================================
//---------------------------------------------------------------------------
//Log�t�@�C���ۑ��p�t�H���_���̍쐬�B��x�쐬�����瓯��PATH
//---------------------------------------------------------------------------
BOOL StInspectMain::CreateLogPathName(void)
{
	BOOL bReval=FALSE;
	if( mLogFilePath.GetLength()>0 )	//���łɍ쐬�ς�
		return FALSE;
	if( mDeviceModelName.GetLength()>0 )
	{
		if( mCurrentDateFormat.GetLength()==0 )
		{
			CTime gt=CTime::GetCurrentTime();
			mCurrentDateFormat = gt.Format("%Y%m%d%H%M%S");
		}
		CString dateFormat = mCurrentDateFormat.Left(mCurrentDateFormat.GetLength()-6);
		mLogFilePath = mSaveFilePath + _T("\\") + mDeviceModelName + _T("\\") + dateFormat + _T("\\log\\");
		bReval = TRUE;
	}
	return bReval;
}

//---------------------------------------------------------------------------
//Log�t�@�C�����BRename����邱�Ƃ���
//SerialNumber�m���Rename
//---------------------------------------------------------------------------
BOOL StInspectMain::CreateLogFileName(void)
{
	BOOL bReval=FALSE;
	if( mDeviceModelName.GetLength()>0 )	//ModelName���Ȃ��ꍇ�쐬���Ȃ� PATH���Ȃ�����
	{
		if( mCurrentDateFormat.GetLength()==0 )
		{
			CTime gt=CTime::GetCurrentTime();
			mCurrentDateFormat = gt.Format("%Y%m%d%H%M%S");
		}
		//mLogFileName = mDeviceModelName + "_" + m_Serial + "_" + mCurrentDateFormat + ".log";
		CString szLogFileName = mDeviceModelName + _T("_");
		if( mSerialNumber.GetLength()>0 )
		{
			szLogFileName += mSerialNumber + _T("_");
		}
		//��1.0.0.1015 LOG�t�@�C�����ύX
		//szLogFileName += mCurrentDateFormat + _T(".log");
		szLogFileName += mCurrentDateFormat + _T("_NG.txt");
		//��1.0.0.1015 LOG�t�@�C�����ύX
		
		if( mLogFileName!=szLogFileName )
		{
			if( mLogFileName.GetLength()>0 && mLogFilePath.GetLength()>0 )
			{
				//���݂̃t�@�C����Rename������B����Ȃ�-------
				//���݂̃t�@�C�� mLogFileName -> �V�����t�@�C�� szLogFileName
				bReval = MoveFileEx( mLogFilePath+mLogFileName, mLogFilePath+szLogFileName, MOVEFILE_REPLACE_EXISTING);
			}
			mLogFileName = szLogFileName;
		}
		bReval = TRUE;
	}
	return bReval;
}

//��1.0.0.1015
//---------------------------------------------------------------------------
//Log�t�@�C��������_NG���폜
//---------------------------------------------------------------------------
BOOL StInspectMain::LogFileNameDeleteNG(void)
{

	BOOL bReval=FALSE;
	if( mLogFilePath.GetLength()>0 && mLogFileName.GetLength()>0 )
	{
		CString szNewLogFileName = mLogFileName; 

		szNewLogFileName.Replace(_T("_NG"),_T(""));

		if( mLogFileName.Compare(szNewLogFileName)!=0 )
		{
			bReval = MoveFileEx( mLogFilePath+mLogFileName, mLogFilePath+szNewLogFileName, MOVEFILE_REPLACE_EXISTING);
			if( bReval )
			{
				mLogFileName = szNewLogFileName;
			}
		}
		else
		{
			bReval=TRUE;
		}
	}
	return bReval;

}
//��1.0.0.1015

//---------------------------------------------------------------------------
//���O�t�@�C���o��
//---------------------------------------------------------------------------
BOOL StInspectMain::OutputLogFile(LPCTSTR szText)
{
	BOOL bReval = FALSE;
	if( mLogFileName.GetLength()>0 && mLogFilePath.GetLength()>0 )
	{
		//�t�H���_�쐬
		CStCommon::CheckDirectoryExist(mLogFilePath,TRUE);

		CString szLogFileFullName = mLogFilePath + mLogFileName;
		CStdioFile aLogFile;
		if( !aLogFile.Open(szLogFileFullName,CFile::modeNoTruncate|CFile::modeCreate|CFile::modeWrite|CFile::typeText ,NULL) )
			return FALSE;

		aLogFile.SeekToEnd();

		//��1.0.0.1026
		//if( _tcscmp(szText,_T("TIMESTAMP"))!=0 )
		//{
		//	aLogFile.WriteString( szText );
		//}
		//else
		//{
		//	CTime gt=CTime::GetCurrentTime();
		//	CString dateFormat = gt.Format("%Y%m%d%H%M%S");
		//	dateFormat = _T("TIME=") + dateFormat;
		//	aLogFile.WriteString( dateFormat );
		//}

		if( _tcscmp(szText,_T("TIMESTAMP"))==0 )
		{
			CTime gt=CTime::GetCurrentTime();
			CString dateFormat = gt.Format("%Y%m%d%H%M%S");
			dateFormat = _T("TIME=") + dateFormat;
			aLogFile.WriteString( dateFormat );
		}
		else
		if( _tcscmp(szText,_T("SOFTWAREVERSION"))==0 )
		{
			CString softwareFormat(_T("software="));
			softwareFormat += m_szSoftwareVersion;
			aLogFile.WriteString( softwareFormat );
		}
		//��1.0.0.1072a
		else
		if( _tcscmp(szText,_T("SCRIPTFILEINFO"))==0 )
		{
			if( m_vScriptFilename.size()>0 )
			{
				CString szText;
				aLogFile.WriteString( _T("[ScriptFileInfo]\n") );
				for( int i=0; i<m_vScriptFilename.size(); i++ )
				{
					CString szTime;
					if( i<m_vScriptFileInfos.size() )
					{
						szTime = m_vScriptFileInfos[i];
					}
					szText.Format(_T("%s=%s\n"),m_vScriptFilename[i],szTime);
					aLogFile.WriteString( szText );
				}
			}
			aLogFile.Close();
			bReval = TRUE;
			return bReval;
		}
		//��1.0.0.1072a
		else
		{
			aLogFile.WriteString( szText );
		}
		//��1.0.0.1026

		aLogFile.WriteString( _T("\n") );
		aLogFile.Close();
		//��1.0.0.1026
		bReval = TRUE;
		//��1.0.0.1026
	}
	return bReval;
}


// ==========================================================================
//�X�N���v�g�֘A
// ==========================================================================
BOOL StInspectMain::Execute(void)
{
	BOOL bReval = TRUE;
	do
	{
		mHandle = INVALID_HANDLE_VALUE;

		//bReval = RegisterEvent();
		//if(!bReval) break;

		//bReval = SetEventAdapterRegisterMain();
		//if(!bReval) break;

		mHandle = (HANDLE)_beginthreadex(
			NULL,
			0,
			Link,
			this,
			0,
			&mID);

	}while(false);
	return bReval;
}

BOOL StInspectMain::WaitForInspection(DWORD dwTimeout)
{
	BOOL bReval = FALSE;
	if( dwTimeout==0 )	dwTimeout=INFINITE;
	if(mHandle != INVALID_HANDLE_VALUE)
	{
		//mStop = true;
		DWORD lRetVal = ::WaitForSingleObject( mHandle, dwTimeout );
		//ASSERT( lRetVal != WAIT_TIMEOUT  );
		if( lRetVal != WAIT_TIMEOUT )
		{
			CloseHandle( mHandle );
			mHandle = INVALID_HANDLE_VALUE;
			mID = 0;
			bReval = TRUE;
		}
		//UnregisterEvent();
	}
	return bReval;
}

BOOL StInspectMain::GetResult(PDWORD pdwResult)
{
	*pdwResult = mReturnValue;
	return TRUE;
}

unsigned WINAPI StInspectMain::Link( void *aParam )
{
	StInspectMain *lThis = reinterpret_cast<StInspectMain *>( aParam );
	lThis->mReturnValue = lThis->Function();
	return lThis->mReturnValue;
}


//��1.0.0.1023
INT StInspectMain::GetLoopEndLine(LPCTSTR szVarName, INT nStart)
{
	INT nEnd = -1;

	for( int i=nStart + 1; i<(int)m_vScriptString.size() && nEnd<0; i++ )
	{
		CString szString = m_vScriptString[i];
		szString.Trim();
		INT iPeriodPos = szString.Find(_T("."));

		if( iPeriodPos>0 )
		{
			CString szVarName2 = szString.Left(iPeriodPos);
			if( szVarName2==szVarName )
			{
				CString szTmp = szString.Mid( iPeriodPos + 1 );
				szTmp.Trim();
				szTmp.MakeLower();
				if( szTmp==_T("end") )
				{
					nEnd = i;
				}
			}
		}
	}
	return nEnd;
}
//��1.0.0.1023

//��1.0.0.1030
INT StInspectMain::GetIFEndLine(LPCTSTR szVarName, INT nStart)
{

	INT nEnd = -1;

	for( int i=nStart + 1; i<(int)m_vScriptString.size() && nEnd<0; i++ )
	{
		CString szString = m_vScriptString[i];
		szString.Trim();
		INT iPeriodPos = szString.Find(_T("."));

		if( iPeriodPos>0 )
		{
			CString szVarName2 = szString.Left(iPeriodPos);
			if( szVarName2==szVarName )
			{
				CString szTmp = szString.Mid( iPeriodPos + 1 );
				szTmp.Trim();
				szTmp.MakeLower();
				if( szTmp==_T("end") )
				{
					nEnd = i;
				}
			}
		}
	}
	return nEnd;
}

INT StInspectMain::GetIFElseIfLine(LPCTSTR szVarName, INT nStart, INT nEnd)
{

	INT nElse = -1;

	for( int i=nStart + 1; i<(int)m_vScriptString.size() && i<nEnd && nElse<0; i++ )
	{
		CString szString = m_vScriptString[i];
		szString.Trim();
		INT iPeriodPos = szString.Find(_T("."));

		if( iPeriodPos>0 )
		{
			CString szVarName2 = szString.Left(iPeriodPos);
			if( szVarName2==szVarName )
			{
				CString szTmp = szString.Mid( iPeriodPos + 1 );
				szTmp.Trim();
				szTmp.MakeLower();
				INT nFind = szTmp.Find(_T("("));
				if( nFind>=0 )
				{
					szTmp = szTmp.Left(nFind);
				}
				if( szTmp==_T("elseif") )
				{
					nElse = i;
				}
			}
		}
	}
	return nElse;
}

INT StInspectMain::GetIFElseLine(LPCTSTR szVarName, INT nStart, INT nEnd)
{

	INT nElse = -1;

	for( int i=nStart + 1; i<(int)m_vScriptString.size() && i<nEnd && nElse<0; i++ )
	{
		CString szString = m_vScriptString[i];
		szString.Trim();
		INT iPeriodPos = szString.Find(_T("."));

		if( iPeriodPos>0 )
		{
			CString szVarName2 = szString.Left(iPeriodPos);
			if( szVarName2==szVarName )
			{
				CString szTmp = szString.Mid( iPeriodPos + 1 );
				szTmp.Trim();
				szTmp.MakeLower();
				if( szTmp==_T("else") )
				{
					nElse = i;
				}
			}
		}
	}
	return nElse;
}

//��1.0.0.1030

INT GetArgument(LPCTSTR szText)
{
	CString szString(szText);
	CString szArgument(_T(""));
	INT iPos;
	if( (iPos=szString.Find(_T("(")))>0 )
	{
		szArgument = szString.Mid(iPos+1);
		szArgument = szArgument.Left(szArgument.Find(_T(")")));
	}
	else
	if( (iPos=szString.Find(_T("=")))>0 )
	{
		szArgument = szString.Mid(iPos+1);
		szArgument.Trim();
	}
	INT nArgument=-1;
	if( szArgument.GetLength()>0 )
	{
		if( szArgument.Left(1)==_T("J") )
		{
			nArgument = 1<<8;
		}
		else
		if( szArgument.Left(1)==_T("K") )
		{
			nArgument = 1<<9;
		}
		else
		{
			nArgument = _tcstol(szArgument,NULL,0);
		}
	}
	return nArgument;
}

//��1.0.0.1061
// ==========================================================================
//��1.0.0.1062
//BOOL StInspectMain::pushScriptLine(CString &szLine, std::vector<INT> &vCommentout, bool *pbCommentout)
//��1.0.0.1072
//BOOL StInspectMain::pushScriptLine(CString &szLine, std::vector<INT> &vCommentout, bool *pbCommentout, UINT unLineIndex, UINT unFileIndex)
BOOL StInspectMain::pushScriptLine(CString &szLine, std::vector<INT> &vCommentout, bool *pbCommentout, UINT unLineIndex, UINT unFileIndex, INT nCheckLineIndex)
//��1.0.0.1072
//��1.0.0.1062
{

	szLine.Trim();
	if( !szLine.IsEmpty() && szLine.Left(1).Compare(_T("#"))!=0 )
	{
		bool bCommentword = false;
		//Commentout�`�F�b�N-----------------
		if( szLine.Left(12)==_T("CommentStart") )
		{
			INT nArgument = GetArgument(szLine);
			if( nArgument>=0 )
			{
				//���
				bool bPush = true;
				for( DWORD i=0; i<vCommentout.size() && bPush; i++ )
				{
					if( vCommentout[i] == nArgument )
					{
						bPush = false;
					}
				}
				if( bPush )
				{
					vCommentout.push_back(nArgument);
				}
			}
			bCommentword = true;
		}
		else
		if( szLine.Left(10)==_T("CommentEnd") )
		{
			INT nArgument = GetArgument(szLine);
			if( nArgument>=0 )
			{
				//�폜
				for( DWORD i=0; i<vCommentout.size(); i++ )
				{
					if( vCommentout[i] == nArgument )
					{
						vCommentout.erase(vCommentout.begin() + i);
						break;
					}
				}
			}
			bCommentword = true;
		}

		if( bCommentword )
		{
			//bCommentout����
			*pbCommentout = false;
			for( DWORD i=0; i<vCommentout.size() && *pbCommentout == false; i++ )
			{
				if( (vCommentout[i]>>mCheckMode)&1 )
				{
					*pbCommentout = true;
				}

				//FactoryMode
				if( (vCommentout[i]>>(mFactoryMode+8))&1 )
				{
					*pbCommentout = true;
				}
			}
		}

		//Comment�ȊO�e�L�X�g���
		if( !*pbCommentout && !bCommentword )
		{
			//CallScript�̏ꍇ
			//��1.0.0.1069
			//if( !CallScriptFile(szLine,vCommentout,pbCommentout) )
			bool bError = false;
			if( !CallScriptFile(szLine,vCommentout,pbCommentout,&bError) )
			//��1.0.0.1069
			{
				m_vScriptString.push_back(szLine);
				//��1.0.0.1062
				m_vScriptLineIndex.push_back((unLineIndex&0xFFFFFF)|(unFileIndex<<24));
				//��1.0.0.1062
				//��1.0.0.1072
				m_vScriptCheckLineIndex.push_back(nCheckLineIndex);
				//��1.0.0.1072
			}
			//��1.0.0.1069
			if( bError ) return FALSE;
			//��1.0.0.1069
		}
	}
	return TRUE;

}
//��1.0.0.1069
//BOOL StInspectMain::CallScriptFile(CString &szLine, std::vector<INT> &vCommentout, bool *pbCommentout)
BOOL StInspectMain::CallScriptFile(CString &szLine, std::vector<INT> &vCommentout, bool *pbCommentout, bool *pbError)
//��1.0.0.1069
{
	//��1.0.0.1069
	bool bError = false;
	//��1.0.0.1069
	CString szVariable, szArgument;
	BOOL bReval = SepareteString( szLine, szVariable, szArgument );
	if( bReval )
	{
		if( szVariable==_T("CallScriptFile") )
		{
			CStdioFile scriptFile;
			CString szSubScriptFileName = mszSubScriptPath;
			if( szSubScriptFileName.Right(1)!=_T("\\") && szArgument.Left(1)!=_T("\\") )
				szSubScriptFileName+=_T("\\");
			else
			if( szSubScriptFileName.Right(1)==_T("\\") && szArgument.Left(1)==_T("\\") )
				szSubScriptFileName+=_T(".");
			szSubScriptFileName += szArgument;
			if( scriptFile.Open(szSubScriptFileName,CFile::modeRead,NULL)!=0 )
			{
				scriptFile.SeekToBegin();
				CString tmp;
				//��1.0.0.1062
				m_vScriptFilename.push_back(szArgument);
				UINT unFileIndex = m_vScriptFilename.size()-1;
				UINT unLineIndex = 0;
				//��1.0.0.1062

				//��1.0.0.1072a
				CFileStatus rStatus;
				scriptFile.GetStatus(rStatus);
				CString szFileInfo = rStatus.m_mtime.Format("%Y/%m/%d %H:%M:%S");
				AddScriptFileInfo(szFileInfo);
				//��1.0.0.1072a

				while( scriptFile.ReadString(tmp) )
				{
					//��1.0.0.1069
					//pushScriptLine(tmp,vCommentout,pbCommentout,unLineIndex++,unFileIndex);
					bError = !pushScriptLine(tmp,vCommentout,pbCommentout,unLineIndex++,unFileIndex);
					if( bError ) break;
					//��1.0.0.1069
				}
				//��1.0.0.1069
				scriptFile.Close();
				//��1.0.0.1069
			}
			//��1.0.0.1069
			else
			{
				bError = true;
			}
			//��1.0.0.1069
		}
		else
		{
			bReval = FALSE;
		}
	}
	//��1.0.0.1069
	if( pbError ) *pbError = bError;
	//��1.0.0.1069
	return bReval;
}
//��1.0.0.1061

//��1.0.0.1069
BOOL StInspectMain::ScriptModelFileLoad(void)
{
	BOOL bReval = FALSE;
	CStdioFile scriptFile;
	m_vScriptString.clear();
	m_vScriptLineIndex.clear();
	//��1.0.0.1072
	m_vScriptCheckLineIndex.clear();
	INT nCheckLineIndex = 0;
	//��1.0.0.1072

	LPCTSTR defLocale = _tsetlocale(LC_ALL, _T("jpn"));		//�S�p�ׂ̈ɕK�v

	bool bCommentout = false;
	std::vector<INT> vCommentout;	//���l D0:�o�׌��� D1:�H��������
	for( int i=0; i<m_vScriptModelFileNames.size(); i++ )
	{
		//��1.0.0.1072
		if( ((m_vCheckMode[i]>>mCheckMode)&1)==0 )
			continue;
		//��1.0.0.1072

		bReval = FALSE;
		CString szScriptModelFileName = m_vScriptModelFileNames[i];
		CString szScriptFileName = szScriptModelFileName;
		INT nPos = szScriptModelFileName.ReverseFind('\\');
		if( nPos>=0 ) szScriptFileName = szScriptModelFileName.Mid(nPos+1);
		m_vScriptFilename.push_back(szScriptFileName);

		if( scriptFile.Open(szScriptModelFileName,CFile::modeRead,NULL)!=0 )
		{
			//��1.0.0.1072a
			CFileStatus rStatus;
			scriptFile.GetStatus(rStatus);
			//CString szFileInfo = rStatus.m_mtime.Format("%Y%M%D%h%m%s");
			CString szFileInfo = rStatus.m_mtime.Format("%Y/%m/%d %H:%M:%S");
			AddScriptFileInfo(szFileInfo);
			//��1.0.0.1072a

			//commenntout�p

			scriptFile.SeekToBegin();
			CString tmp;
			UINT unLineIndex = 0;
			
			//��1.0.0.1069
			//while( scriptFile.ReadString(tmp) )
			//{
			//	pushScriptLine(tmp,vCommentout,&bCommentout,unLineIndex++,m_vScriptFilename.size()-1);
			//}
			while( scriptFile.ReadString(tmp) )
			{
				//��1.0.0.1072
				//bReval = pushScriptLine(tmp,vCommentout,&bCommentout,unLineIndex++,m_vScriptFilename.size()-1);
				bReval = pushScriptLine( tmp, vCommentout, &bCommentout, unLineIndex++, m_vScriptFilename.size()-1, i<m_vCheckListIndex.size()?m_vCheckListIndex[i]:-1 );
				//��1.0.0.1072
				if( !bReval ) break;
			}
			//��1.0.0.1069
			scriptFile.Close();
			//��1.0.0.1069
			//if( m_vScriptString.size()>0 ) bReval = TRUE;
			//��1.0.0.1069
		}
		//��1.0.0.1069
		if( !bReval ) break; 
		//��1.0.0.1069
	}
	vCommentout.clear();
	_tsetlocale(LC_ALL, defLocale);
	return bReval;
}
//��1.0.0.1069

BOOL StInspectMain::ScriptFileLoad()
{
	BOOL bReval = FALSE;
	CStdioFile scriptFile;
	m_vScriptString.clear();
	//��1.0.0.1062
	m_vScriptLineIndex.clear();
	CString szScriptFileName = mszScriptFileName;
	INT nPos = mszScriptFileName.ReverseFind('\\');
	if( nPos>=0 ) szScriptFileName = mszScriptFileName.Mid(nPos+1);
	m_vScriptFilename.push_back(szScriptFileName);
	//��1.0.0.1062
	//��1.0.0.1072
	m_vScriptCheckLineIndex.clear();
	//��1.0.0.1072
	LPCTSTR defLocale = _tsetlocale(LC_ALL, _T("jpn"));		//�S�p�ׂ̈ɕK�v
	if( scriptFile.Open(mszScriptFileName,CFile::modeRead,NULL)!=0 )
	{
		//��1.0.0.1072a
		CFileStatus rStatus;
		scriptFile.GetStatus(rStatus);
		CString szFileInfo = rStatus.m_mtime.Format("%Y/%m/%d %H:%M:%S");
		AddScriptFileInfo(szFileInfo);
		//��1.0.0.1072a

		//commenntout�p
		bool bCommentout = false;
		std::vector<INT> vCommentout;	//���l D0:�o�׌��� D1:�H��������

		scriptFile.SeekToBegin();
		CString tmp;
		//��1.0.0.1062
		UINT unLineIndex = 0;
		//��1.0.0.1062
		while( scriptFile.ReadString(tmp) )
		{
			//��1.0.0.1061
			/*
			tmp.Trim();
			if( !tmp.IsEmpty() && tmp.Left(1).Compare(_T("#"))!=0 )
			{
				bool bCommentword = false;
				//Commentout�`�F�b�N-----------------
				if( tmp.Left(12)==_T("CommentStart") )
				{
					INT nArgument = GetArgument(tmp);
					if( nArgument>=0 )
					{
						//���
						bool bPush = true;
						for( DWORD i=0; i<vCommentout.size() && bPush; i++ )
						{
							if( vCommentout[i] == nArgument )
							{
								bPush = false;
							}
						}
						if( bPush )
						{
							vCommentout.push_back(nArgument);
						}
					}
					bCommentword = true;
				}
				else
				if( tmp.Left(10)==_T("CommentEnd") )
				{
					INT nArgument = GetArgument(tmp);
					if( nArgument>=0 )
					{
						//�폜
						for( DWORD i=0; i<vCommentout.size(); i++ )
						{
							if( vCommentout[i] == nArgument )
							{
								vCommentout.erase(vCommentout.begin() + i);
								break;
							}
						}
					}
					bCommentword = true;
				}

				if( bCommentword )
				{
					//bCommentout����
					bCommentout = false;
					for( DWORD i=0; i<vCommentout.size() && bCommentout == false; i++ )
					{
						if( (vCommentout[i]>>mCheckMode)&1 )
						{
							bCommentout = true;
						}

						//FactoryMode
						if( (vCommentout[i]>>(mFactoryMode+8))&1 )
						{
							bCommentout = true;
						}
					}
				}


				//Comment�ȊO�e�L�X�g���
				if( !bCommentout && !bCommentword )
				{
					m_vScriptString.push_back(tmp);
				}
			}
			*/
			//��1.0.0.1062
			//pushScriptLine(tmp,vCommentout,&bCommentout);
			//��1.0.0.1069
			//pushScriptLine(tmp,vCommentout,&bCommentout,unLineIndex++);
			bReval = pushScriptLine(tmp,vCommentout,&bCommentout,unLineIndex++);
			if( !bReval ) break;
			//��1.0.0.1069
			//��1.0.0.1062
			//��1.0.0.1061
		}
		scriptFile.Close();
		//��1.0.0.1069
		//if( m_vScriptString.size()>0 ) bReval = TRUE;
		//��1.0.0.1069
		vCommentout.clear();
	}
	_tsetlocale(LC_ALL, defLocale);
	return bReval;
}




//��1.0.0.1025
//StCheckBase�N���X����l�擾
//nMode 0:������̂� 1:dblValue����
BOOL StInspectMain::GetCheckFunction(LPCTSTR szString, CString &szValue, double &dblValue, INT &nMode)
{
	BOOL bReval = FALSE;
	//szString �� szClass,szVarName,szAddress�ɂ킯��
	CString szVarName("");
	CString szFunc("");
	CString szArgument("");
	//double dblValue;

	if( !SepareteString(szString, szVarName, szFunc, szArgument ) )
		return FALSE;

	nMode = 0;

	//��1.0.0.1046
	for( int i=0; i<(int)m_vecDeviceBase.size() && bReval==FALSE; i++ )
	{
		StDeviceBase *aDeviceBase = m_vecDeviceBase[i];
		if( szVarName.Compare(aDeviceBase->m_VarName.GetUnicode())!=0 )
			continue;
		if( aDeviceBase->m_ClassName==_StDeviceGenICam )
		{
			StDeviceGenICam *aDevice = (StDeviceGenICam *)aDeviceBase;
			bReval = aDevice->GetFunction(szFunc,szArgument,dblValue);
			if( bReval )
			{
				//��1.0.0.1072c
				//szValue.Format(_T("%f"),dblValue);
				if( dblValue-(double)(INT)dblValue>0.0 )
					szValue.Format(_T("%f"),dblValue);
				else
					szValue.Format(_T("%i"),(INT)dblValue);
				//��1.0.0.1072c
				nMode = 1;
			}
		}

		//��1.0.0.1072c
		else
		if( aDeviceBase->m_ClassName==_StDeviceEBus )
		{
			StDeviceEBus *aDevice = (StDeviceEBus *)aDeviceBase;
			bReval = aDevice->GetFunction(szFunc,szArgument,dblValue);
			if( bReval )
			{
				if( dblValue-(double)(INT)dblValue>0.0 )
					szValue.Format(_T("%f"),dblValue);
				else
					szValue.Format(_T("%i"),(INT)dblValue);
				nMode = 1;
			}
		}
		//��1.0.0.1072c
	}
	//��1.0.0.1046


	for( int i=0; i<(int)m_vecCheckBase.size() && bReval==FALSE; i++ )
	{

		StCheckBase *aCheckBase = m_vecCheckBase[i];
		if( szVarName.Compare(aCheckBase->m_VarName.GetUnicode())!=0 )
			continue;

		if( aCheckBase->m_ClassName==_StCheckSN )
		{
			StCheckSN *pCheck = (StCheckSN *)aCheckBase;
			bReval = pCheck->GetFunction(szFunc,szArgument,dblValue);
			if( bReval )
			{
				szValue.Format(_T("%f"),dblValue);
				nMode = 1;
			}
		}
		else
		if( aCheckBase->m_ClassName==_StCheckControlRetry )
		{
			StCheckControlRetry *pCheck = (StCheckControlRetry *)aCheckBase;
			bReval = pCheck->GetFunction(szFunc,szArgument,dblValue);
			if( bReval )
			{
				szValue.Format(_T("%f"),dblValue);
				nMode = 1;
			}
		}
		else
		if( aCheckBase->m_ClassName==_StInputSerialDlg )
		{
			StInputSerialDlg *pCheck = (StInputSerialDlg *)aCheckBase->GetMainHandle();
			bReval = pCheck->GetFunction(szFunc,szArgument,szValue);
		}
		else
		if( aCheckBase->m_ClassName==_StCheckSNDlg )
		{
			StCheckSNDlg *pCheck = (StCheckSNDlg *)aCheckBase->GetMainHandle();
			bReval = pCheck->GetFunction(szFunc,szArgument,dblValue);
			if( bReval )
			{
				szValue.Format(_T("%f"),dblValue);
				nMode = 1;
			}
		}
		else
		if( aCheckBase->m_ClassName==_StCheckSpBehavior )
		{
			StCheckSpBehavior *pCheck = (StCheckSpBehavior *)aCheckBase;
			bReval = pCheck->GetFunction(szFunc,szArgument,dblValue);
			if( bReval )
			{
				szValue.Format(_T("%f"),dblValue);
				nMode = 1;
			}
		}
		else
		if( aCheckBase->m_ClassName==_StCheckDataLose )
		{
			StCheckDataLose *pCheck = (StCheckDataLose *)aCheckBase;
			bReval = pCheck->GetFunction(szFunc,szArgument,dblValue);
			if( bReval )
			{
				szValue.Format(_T("%f"),dblValue);
				nMode = 1;
			}
		}
		//��1.0.0.1062
		else
		if( aCheckBase->m_ClassName==_StCheckDataLoseDlg )
		{
			StCheckDataLoseDlg *pCheck = (StCheckDataLoseDlg *)aCheckBase->GetMainHandle();
			bReval = pCheck->GetFunction(szFunc,szArgument,dblValue);
			if( bReval )
			{
				szValue.Format(_T("%f"),dblValue);
				nMode = 1;
			}
		}
		//��1.0.0.1062
		else
		if( aCheckBase->m_ClassName==_StCheckPixelDefect )
		{
			StCheckPixelDefect *pCheck = (StCheckPixelDefect *)aCheckBase;
			bReval = pCheck->GetFunction(szFunc,szArgument,dblValue);
			if( bReval )
			{
				szValue.Format(_T("%f"),dblValue);
				nMode = 1;
			}
		}
		else
		if( aCheckBase->m_ClassName==_StCheckDustDlg )
		{
			StCheckDustDlg *pCheck = (StCheckDustDlg *)aCheckBase->GetMainHandle();
			bReval = pCheck->GetFunction(szFunc,szArgument,dblValue);
			if( bReval )
			{
				szValue.Format(_T("%f"),dblValue);
				nMode = 1;
			}
		}
		else
		if( aCheckBase->m_ClassName==_StCheckMessage )
		{
			StCheckMessage *pCheck = (StCheckMessage *)aCheckBase;
			bReval = pCheck->GetFunction(szFunc,szArgument,dblValue);
			if( bReval )
			{
				szValue.Format(_T("%f"),dblValue);
				nMode = 1;
			}
		}
		else
		if( aCheckBase->m_ClassName==_StCheckAdjustLinetolineDlg )
		{
			StCheckAdjustLinetolineDlg *pCheck = (StCheckAdjustLinetolineDlg *)aCheckBase->GetMainHandle();
			bReval = pCheck->GetFunction(szFunc,szArgument,dblValue);
			if( bReval )
			{
				szValue.Format(_T("%f"),dblValue);
				nMode = 1;
			}
		}
		else
		if( aCheckBase->m_ClassName==_StCheckAdjustLinetoline )
		{
			StCheckAdjustLinetoline *pCheck = (StCheckAdjustLinetoline *)aCheckBase;
			bReval = pCheck->GetFunction(szFunc,szArgument,dblValue);
			if( bReval )
			{
				szValue.Format(_T("%f"),dblValue);
				nMode = 1;
			}
		}
		else
		if( aCheckBase->m_ClassName==_StCheckEffectivePixel )
		{
			StCheckEffectivePixel *pCheck = (StCheckEffectivePixel *)aCheckBase;
			bReval = pCheck->GetFunction(szFunc,szArgument,dblValue);
			if( bReval )
			{
				szValue.Format(_T("%f"),dblValue);
				nMode = 1;
			}
		}
		else
		if( aCheckBase->m_ClassName==_StInputSensorVersionDlg )
		{
			StInputSensorVersionDlg *pCheck = (StInputSensorVersionDlg *)aCheckBase->GetMainHandle();
			bReval = pCheck->GetFunction(szFunc,szArgument,szValue);
		}
		else
		if( aCheckBase->m_ClassName==_StInputSensorSerialDlg )
		{
			StInputSensorSerialDlg *pCheck = (StInputSensorSerialDlg *)aCheckBase->GetMainHandle();
			bReval = pCheck->GetFunction(szFunc,szArgument,szValue);
		}
		else
		if( aCheckBase->m_ClassName==_StLightControlDlg )
		{
			StLightControlDlg *pCheck = (StLightControlDlg *)aCheckBase->GetMainHandle();
			bReval = pCheck->GetFunction(szFunc,szArgument,dblValue);
			if( bReval )
			{
				szValue.Format(_T("%f"),dblValue);
				nMode = 1;
			}
		}
		else
		if( aCheckBase->m_ClassName==_StLightControl )
		{
			StLightControl *pCheck = (StLightControl *)aCheckBase;
			bReval = pCheck->GetFunction(szFunc,szArgument,dblValue);
			if( bReval )
			{
				szValue.Format(_T("%f"),dblValue);
				nMode = 1;
			}
		}
		else
		if( aCheckBase->m_ClassName==_StCheckPower )
		{
			StCheckPower *pCheck = (StCheckPower *)aCheckBase;
			bReval = pCheck->GetFunction(szFunc,szArgument,dblValue);
			if( bReval )
			{
				szValue.Format(_T("%f"),dblValue);
				nMode = 1;
			}
		}
		else
		if( aCheckBase->m_ClassName==_StCheckCommon )
		{
			//StCheckCommon *pCheck = (StCheckCommon *)aCheckBase;
			//bReval = pCheck->GetFunction(szFunc,szArgument,dblValue);
		}
		else
		if( aCheckBase->m_ClassName==_StCheckSerialVersion )
		{
			StCheckSerialVersion *pCheck = (StCheckSerialVersion *)aCheckBase;
			bReval = pCheck->GetFunction(szFunc,szArgument,szValue);
		}
		else
		if( aCheckBase->m_ClassName==_StCheckIO )
		{
			//StCheckIO *pCheck = (StCheckIO *)aCheckBase;
			//bReval = pCheck->GetFunction(szFunc,szArgument,dblValue);
		}
		else
		if( aCheckBase->m_ClassName==_StPTouch )
		{
			//StPTouch *pCheck = (StPTouch *)aCheckBase;
			//bReval = pCheck->GetFunction(szFunc,szArgument,dblValue);
		}
		else
		if( aCheckBase->m_ClassName==_StWriteSerial )
		{
			//StWriteSerial *pCheck = (StWriteSerial *)aCheckBase;
			//bReval = pCheck->GetFunction(szFunc,szArgument,dblValue);
		}
		//��1.0.0.1058
		else
		if( aCheckBase->m_ClassName==_StWriteFileData )
		{
		}
		//��1.0.0.1058
		else
		if( aCheckBase->m_ClassName==_StCheckPixelCorrect )
		{
			//StCheckPixelCorrect *pCheck = (StCheckPixelCorrect *)aCheckBase->GetMainHandle();
			//bReval = pCheck->GetFunction(szFunc,szArgument,dblValue);
		}
		else
		if( aCheckBase->m_ClassName==_StCheckTemperature )
		{
			StCheckTemperature *pCheck = (StCheckTemperature *)aCheckBase;
			bReval = pCheck->GetFunction(szFunc,szArgument,dblValue);
			if( bReval )
			{
				szValue.Format(_T("%f"),dblValue);
				nMode = 1;
			}
		}
		else
		if( aCheckBase->m_ClassName==_StCheckTestPattern )
		{
			StCheckTestPattern *pCheck = (StCheckTestPattern *)aCheckBase;
			bReval = pCheck->GetFunction(szFunc,szArgument,dblValue);
			if( bReval )
			{
				szValue.Format(_T("%f"),dblValue);
				nMode = 1;
			}
		}
		else
		if( aCheckBase->m_ClassName==_StCheckEllitoLVDSDlg )
		{
			//StCheckEllitoLVDSDlg *pCheck = (StCheckEllitoLVDSDlg *)aCheckBase->GetMainHandle();
			//bReval = pCheck->GetFunction(szFunc,szArgument,dblValue);
		}
		//��1.0.0.1059
		else
		if( aCheckBase->m_ClassName==_StCheckEllitoLVDS )
		{
		}
		//��1.0.0.1059
		else
		if( aCheckBase->m_ClassName==_StLoop )
		{
			//StLoop *pCheck = (StLoop *)aCheckBase;
			//bReval = pCheck->GetFunction(szFunc,szArgument,dblValue);
		}
		//��1.0.0.1033
		else
		if( aCheckBase->m_ClassName==_StCheckFramerate )
		{
			StCheckFramerate *pCheck = (StCheckFramerate *)aCheckBase;
			bReval = pCheck->GetFunction(szFunc,szArgument,dblValue);
			if( bReval )
			{
				szValue.Format(_T("%f"),dblValue);
				nMode = 1;
			}
		}
		//��1.0.0.1033
		//��1.0.0.1040
		else
		if( aCheckBase->m_ClassName==_StCheckCSVDust )
		{
			StCheckCSVDust *pCheck = (StCheckCSVDust *)aCheckBase;
			bReval = pCheck->GetFunction(szFunc,szArgument,dblValue);
			if( bReval )
			{
				szValue.Format(_T("%f"),dblValue);
				nMode = 1;
			}
		}
		//��1.0.0.1040
		//��1.0.0.1044
		else
		if( aCheckBase->m_ClassName==_StCheckAdjustOffset )
		{
			StCheckAdjustOffset *pCheck = (StCheckAdjustOffset *)aCheckBase;
			bReval = pCheck->GetFunction(szFunc,szArgument,dblValue);
			if( bReval )
			{
				szValue.Format(_T("%f"),dblValue);
				nMode = 1;
			}
		}
		//��1.0.0.1044
		//��1.0.0.1049
		else
		if( aCheckBase->m_ClassName==_StCheckAdjustGainDlg )
		{
			StCheckAdjustGainDlg *pCheck = (StCheckAdjustGainDlg *)aCheckBase->GetMainHandle();
			bReval = pCheck->GetFunction(szFunc,szArgument,dblValue);
			if( bReval )
			{
				szValue.Format(_T("%f"),dblValue);
				nMode = 1;
			}
		}
		//��1.0.0.1049

	}

	//��1.0.0.70



	//��1.0.0.70




	return bReval;
}

//�����_�ł�GigE��GenICam�̂�
//-------------------------------------
//LPCTSTR szString : �X�N���v�g�L�q�̕�����
//CString &szValue : �߂�l������
//double &dblValue : �߂�ldouble�^
//INT &nMode       : ���l�������񂩂ǂ���
//-------------------------------------
//��1.0.0.1072d
#ifdef GENCOMP_OLD
BOOL StInspectMain::GetDeviceFunction(LPCTSTR szString, CString &szValue, double &dblValue, INT &nMode)
#else
BOOL StInspectMain::GetDeviceFunction(LPCTSTR szString, CString &szValue, double &dblValue, INT &nMode, PBOOL pbCheck )
#endif
//��1.0.0.1072d
{
	BOOL bReval = FALSE;

	CString szVarName("");
	CString szFunc("");
	CString szArgument("");

	if( !SepareteString(szString, szVarName, szFunc, szArgument ) )
		return FALSE;

	//��1.0.0.1072d
#ifdef GENCOMP_OLD
#else
	INT nCheckGenICamControlMode = -1;
	double dblCheckGenICamControlValue = 0;
	CString szCheckGenICamControlValue = _T("");
	INT nCheckGenICamControlValueMode = -1;	//0: ���l 1:������

	StDeviceBase *aDeviceBaseResult = NULL;
#endif
	//��1.0.0.1072d

	nMode = 0;
	for( int i=0; i<(int)m_vecDeviceBase.size() && bReval==FALSE; i++ )
	{
		StDeviceBase *aDeviceBase = m_vecDeviceBase[i];
		if( szVarName.Compare(aDeviceBase->m_VarName.GetUnicode())!=0 )
			continue;

		//��1.0.0.1072d
#ifdef GENCOMP_OLD
#else
		aDeviceBaseResult = aDeviceBase;

		nCheckGenICamControlMode = aDeviceBase->GetCheckGenICamControlMode();
		if( nCheckGenICamControlMode!=0 )
		{
			nCheckGenICamControlValueMode = aDeviceBase->GetCheckGenICamControlValueMode();
			if( nCheckGenICamControlValueMode==0 )
			{
				dblCheckGenICamControlValue = aDeviceBase->GetCheckGenICamControlValue();
			}
			else
			{
				LPCTSTR szString = NULL;
				aDeviceBase->GetCheckGenICamControlValue(szString);
				if( szString ) szCheckGenICamControlValue = szString;
			}
		}
#endif
		//��1.0.0.1072d

		if( aDeviceBase->m_ClassName==_StDeviceEBus )
		{
			StDeviceEBus *pDeviceEBus = (StDeviceEBus *)aDeviceBase;



			TCHAR szData[256];
			bReval = pDeviceEBus->GetGenICamControl( szFunc, szData, _countof(szData) );	//������ String  or Enum
//if( szFunc==_T("PixelFormat") )
//Sleep(1);
			if( bReval ) 
			{
				szValue = szData;
				break;
			}
			INT nData=0;
			bReval = pDeviceEBus->GetGenICamControl( szFunc, nData );	// Int Boolean (Enum)
			if( bReval ) 
			{
				nMode=1;
				szValue.Format(_T("%i"),nData);
				dblValue = nData;
				break;
			}

			double dblData=0;
			bReval = pDeviceEBus->GetGenICamControl( szFunc, dblData );	// double
			if( bReval ) 
			{
				nMode=1;
				szValue.Format(_T("%f"),dblData);
				dblValue = dblData;
				break;
			}


			//��1.0.0.1030
			//GetStream----------
			bReval = pDeviceEBus->GetFunction( szFunc, szArgument, szData, _countof(szData) );	//������ String  or Enum
			if( bReval ) 
			{
				szValue = szData;
				break;
			}
			bReval = pDeviceEBus->GetFunction( szFunc, szArgument, nData );	// Int Boolean (Enum)
			if( bReval ) 
			{
				nMode=1;
				szValue.Format(_T("%i"),nData);
				dblValue = nData;
				break;
			}
			bReval = pDeviceEBus->GetFunction( szFunc, szArgument, dblData );	// double
			if( bReval ) 
			{
				nMode=1;
				szValue.Format(_T("%f"),dblData);
				dblValue = dblData;
				break;
			}
			//��1.0.0.1030


		}
		else
		if( aDeviceBase->m_ClassName==_StDeviceMultiCam )
		{
			//StDeviceMultiCam *pDeviceMulticam = (StDeviceMultiCam *)aDeviceBase;
			//bReval = TRUE;
		}
		//��1.0.0.1044
		else
		if( aDeviceBase->m_ClassName==_StDeviceGenICam )
		{
			//��1.0.0.1062
			if( m_nScriptCheckMode==0 )
			{
			//��1.0.0.1062

			StDeviceGenICam *pDeviceGenICam = (StDeviceGenICam *)aDeviceBase;


			TCHAR szData[256];
			bReval = pDeviceGenICam->GetGenICamControl( szFunc, szData, _countof(szData) );	//������ String  or Enum
//if( szFunc==_T("PixelFormat") )
//Sleep(1);
			if( bReval ) 
			{
				szValue = szData;
				break;
			}
			INT nData=0;
			bReval = pDeviceGenICam->GetGenICamControl( szFunc, nData );	// Int (Enum)
			if( bReval ) 
			{
				nMode=1;
				szValue.Format(_T("%i"),nData);
				dblValue = nData;
				break;
			}

			double dblData=0;
			bReval = pDeviceGenICam->GetGenICamControl( szFunc, dblData );	// double
			if( bReval ) 
			{
				nMode=1;
				szValue.Format(_T("%f"),dblData);
				dblValue = dblData;
				break;
			}

			bool bData=false;
			bReval = pDeviceGenICam->GetGenICamControl( szFunc, bData );	// Boolean
			if( bReval ) 
			{
				nMode=1;
				//szValue.Format(bData?_T("true"):_T("false"),bData);
				dblValue = (double)bData;
				break;
			}
			//��1.0.0.1062
			}
			//��1.0.0.1062

			bReval = TRUE;
		}
		//��1.0.0.1044
	}

	//��1.0.0.1072d
#ifdef GENCOMP_OLD
#else
	//if( aDeviceBaseResult )
	//{
	//	aDeviceBaseResult->SetResultGenICamControlCommandName(szFunc);
	//	if( bReval )
	//	{
	//		aDeviceBaseResult->SetResultGenICamControlValueMode(nCheckGenICamControlValueMode);
	//		if( nCheckGenICamControlValueMode==0 )	//���l
	//		{
	//			aDeviceBaseResult->SetResultGenICamControlValue(dblValue);
	//		}
	//		else
	//		if( nCheckGenICamControlValueMode==1 )	//����
	//		{
	//			aDeviceBaseResult->SetResultGenICamControlValue(szValue);
	//		}
	//	}
	//	else
	//	{
	//		aDeviceBaseResult->SetResultGenICamControlValueMode(-1);
	//		aDeviceBaseResult->SetResultGenICamControlValue(_T(""));
	//		aDeviceBaseResult->SetResultGenICamControlValue(0.0);
	//	}
	//}


	if( bReval )
	{
		//��1.0.0.1073 beta1
		//����(LPCTSTR)�������ɑ������ƂȂ����t���[�Y�p������B�B
		//bReval�̒��Ɉړ������B�B
		m_szResultGenICamControlCommandName = (LPCTSTR)szFunc;
		//��1.0.0.1073 beta1

		m_nResultGenICamControlValueMode = nCheckGenICamControlValueMode;
		if( nMode==1 )	//���l
		{
			m_nResultGenICamControlValueMode = 0;
			m_dblResultGenICamControlValue = dblValue;
		}
		else
		if( nMode==0 )	//����
		{
			m_nResultGenICamControlValueMode = 1;
			m_szResultGenICamControlValue = (LPCTSTR)szValue;
		}
	}
	else
	{
		m_nResultGenICamControlValueMode = -1;
		m_szResultGenICamControlValue = _T("");
		m_dblResultGenICamControlValue = 0.0;
	}

	if( bReval )
	{
		if( pbCheck)
		{
			if( m_nScriptCheckMode )
			{
				*pbCheck = TRUE;
			}
			else
			{
				if( nCheckGenICamControlMode==1 )
				{
					if( nCheckGenICamControlValueMode==0 && nMode==1 )	//���l
					{
						if( dblValue==dblCheckGenICamControlValue )
						{
							*pbCheck = TRUE;
						}
						else *pbCheck = FALSE;
					}
					else
					if( nCheckGenICamControlValueMode==1 && nMode==0 )	//����
					{

						if( szCheckGenICamControlValue.Compare(szValue)==0 )
						{
							*pbCheck = TRUE;
						}
						else *pbCheck = FALSE;
					}
					else *pbCheck = FALSE;
				}
			}
		}
	}
#endif
	//��1.0.0.1072d

	return bReval;
}

//��1.0.0.1031
BOOL StInspectMain::GetControlFunction(LPCTSTR szString, CString &szValue, double &dblValue, INT &nMode)
{
	BOOL bReval = FALSE;

	CString szVarName("");
	CString szFunc("");
	CString szArgument("");

	if( !SepareteString(szString, szVarName, szFunc, szArgument ) )
		return FALSE;

	nMode = 0;
	for( int i=0; i<(int)m_vecCheckBase.size() && bReval==FALSE; i++ )
	{
		StCheckBase *aControl = m_vecCheckBase[i];
		if( szVarName.Compare(aControl->m_VarName.GetUnicode())!=0 )
			continue;
		StCheckControlRetry *aControlRetry = (StCheckControlRetry *)aControl;
		INT szData=0;
		aControlRetry->GetLastRecieveValue(szData);

		dblValue = szData;
		nMode = 0;	//0:���l 1:������

		//��1.0.0.70
		bReval = TRUE;
		//��1.0.0.70

		/*
		if( aDeviceBase->m_ClassName=="StDeviceEBus" )
		{
			StDeviceEBus *pDeviceEBus = (StDeviceEBus *)aDeviceBase;

			TCHAR szData[256];
			bReval = pDeviceEBus->GetGenICamControl( szFunc, szData, _countof(szData) );	//������ String  or Enum
//if( szFunc==_T("PixelFormat") )
//Sleep(1);
			if( bReval ) 
			{
				szValue = szData;
				break;
			}
			INT nData=0;
			bReval = pDeviceEBus->GetGenICamControl( szFunc, nData );	// Int Boolean (Enum)
			if( bReval ) 
			{
				nMode=1;
				szValue.Format(_T("%i"),nData);
				dblValue = nData;
				break;
			}

			double dblData=0;
			bReval = pDeviceEBus->GetGenICamControl( szFunc, dblData );	// double
			if( bReval ) 
			{
				nMode=1;
				szValue.Format(_T("%f"),dblData);
				dblValue = dblData;
				break;
			}


			//GetStream----------
			bReval = pDeviceEBus->GetFunction( szFunc, szArgument, szData, _countof(szData) );	//������ String  or Enum
			if( bReval ) 
			{
				szValue = szData;
				break;
			}
			bReval = pDeviceEBus->GetFunction( szFunc, szArgument, nData );	// Int Boolean (Enum)
			if( bReval ) 
			{
				nMode=1;
				szValue.Format(_T("%i"),nData);
				dblValue = nData;
				break;
			}
			bReval = pDeviceEBus->GetFunction( szFunc, szArgument, dblData );	// double
			if( bReval ) 
			{
				nMode=1;
				szValue.Format(_T("%f"),dblData);
				dblValue = dblData;
				break;
			}


		}
		else
		if( aDeviceBase->m_ClassName=="StDeviceMultiCam" )
		{
			//StDeviceMultiCam *pDeviceMulticam = (StDeviceMultiCam *)aDeviceBase;
			//bReval = TRUE;
		}
		*/
	}



	//��1.0.0.70
	for( int i=0; i<(int)m_vecControl.size() && bReval==FALSE; i++ )
	{
		StControl *aControl = m_vecControl[i];
		if( szVarName.Compare(aControl->m_VarName.GetUnicode())!=0 )
			continue;

		if( aControl->m_ClassName==_StTerminal )
		{
			StTerminal * aTerminal = (StTerminal *)aControl;
			if( aTerminal )
			{
				INT nData;
				bReval = aTerminal->GetFunction( szFunc, szArgument, nData );
				if( bReval )
				{
					nMode=1;	//0:���l 1:������
					szValue.Format(_T("%i"),nData);
					dblValue = nData;
				}

				//��1.0.0.1073 beta2
				//�ۗ�----
				//else
				//{
				//	StString tmp = aTerminal->GetErrorMessage();
				//	if( tmp.GetLength()>0 )
				//	{

				//	}
				//}
				//��1.0.0.1073 beta2
			}
		}
		break;
	}	
	//��1.0.0.70

	return bReval;
}
//��1.0.0.1031


//��1.0.0.1072d
#ifdef GENCOMP_OLD
BOOL StInspectMain::GetFunction(CString szString, CString &szValue)
#else
BOOL StInspectMain::GetFunction(CString szString, CString &szValue, PBOOL pbCheck)
#endif
//��1.0.0.1072d
{
	BOOL bReval = FALSE;
//if( szString==_T("getPixelFormat=aDevice.PixelFormat") )
//	Sleep(1);
//if( szString==_T("aDevice.Gain=aValue") )
//	Sleep(1);


	//���(=)�̂�
	INT nPos = szString.Find(_T("="));
	if( nPos<0 ) return FALSE;
	CString szTmp = szString.Mid(nPos+1);
	szTmp.Trim();

	double dblValue;
	INT nMode=-1;

//if( szString=="aCount = aControlRetry.GetRecieveValue" )
//	Sleep(1);
	bReval = GetCheckFunction(szTmp, szValue,dblValue,nMode);
	if( !bReval )
	{
		//��1.0.0.1072d
#ifdef GENCOMP_OLD
		bReval = GetDeviceFunction( szTmp, szValue,dblValue,nMode);
#else
		bReval = GetDeviceFunction( szTmp, szValue,dblValue,nMode,pbCheck);
#endif
		//��1.0.0.1072d
	}
	//��1.0.0.1031
	if( !bReval )
	{
		bReval = GetControlFunction( szTmp, szValue,dblValue,nMode);
	}
	//��1.0.0.1031

	if( bReval )
	{
		//��1.0.0.70
		//�ԈႢ�H�H
		if( nMode==0 )
		//if( nMode!=0 )	//�߂��� 1.0.0.71
		//��1.0.0.70
		{
			//������
			szValue = szString.Left(nPos+1) + CString(_T("\"")) + szValue + CString(_T("\""));
		}
		else
		{
			//���l
			szValue = szString.Left(nPos+1) + szValue;
		}
	}
	return bReval;
}

//��1.0.0.1025

//��1.0.0.1023 �J�Ԃ��p�ϐ��ǉ�
//��1.0.0.1072d
#ifdef GENCOMP_OLD
BOOL StInspectMain::StValueFunction(LPCTSTR szString)
#else
BOOL StInspectMain::StValueFunction(LPCTSTR szString, PBOOL pbCheck)
#endif
//��1.0.0.1072d
{
	BOOL bReval = FALSE;
	//��1.0.0.1025
	CString szValue(szString);

	//if( szValue=="ifValue = aDevice.GevSCPD" )
	//	Sleep(1);

	//��1.0.0.1072d
#ifdef GENCOMP_OLD
	GetFunction( szString, szValue );	//�e�����N���X��getfunction����l�擾
#else
	BOOL bCheck = TRUE;
	GetFunction( szString, szValue, &bCheck );	//�e�����N���X��getfunction����l�擾
	if( pbCheck ) *pbCheck = bCheck;
#endif
	//��1.0.0.1072d
	for( int i=0; i<(int)m_vecValue.size() && bReval==FALSE; i++ )
	{
		//��1.0.0.1024 �ϐ�
		m_vecValue[i]->SetValue(m_vecValue);
		//��1.0.0.1024 �ϐ�
		//��1.0.0.1025
		//bReval = m_vecValue[i]->function(szString);
		bReval = m_vecValue[i]->function(szValue);
		//��1.0.0.1025
	}
	return bReval;
}
//��1.0.0.1023 �J�Ԃ��p�ϐ��ǉ�

//��1.0.0.1062
BOOL StInspectMain::CheckVariableName(CString szVarName)
{
	BOOL bReval = TRUE;

	LPCTSTR pszClassName[] = 
	{
		_StDeviceEBus
		,_StDeviceMultiCam
		,_StDeviceGenICam
		,_StSerialComm
		,_StTerminal
		,_StTexioControl
		,_StImageMessageDlg
		,_StHttpControlDlg
		,_StValue
		,_StFileOperation
		,_StInterfaceGenICam
		,_StCameraControlDlg
		,_StCheckSwitchDlg
		,_StCheckAdjustVolumeDlg
		,_StCheckSN
		,_StCheckControlRetry
		,_StInputSerialDlg
		,_StCheckSNDlg
		,_StCheckSpBehavior
		,_StCheckDataLose
		,_StCheckDataLoseDlg
		,_StCheckPixelDefect
		,_StCheckPixelCorrect
		,_StCheckTemperature
		,_StCheckTestPattern
		,_StCheckEllitoLVDSDlg
		,_StCheckEllitoLVDS
		,_StCheckFramerate
		,_StCheckCSVDust
		,_StCheckAdjustOffset
		,_StCheckDustDlg
		,_StCheckMessage
		,_StCheckAdjustLinetolineDlg
		,_StCheckAdjustLinetoline
		,_StCheckEffectivePixel
		,_StInputSensorVersionDlg
		,_StInputSensorSerialDlg
		,_StLightControlDlg
		,_StLightControl
		,_StCheckPower
		,_StCheckCommon
		,_StCheckSerialVersion
		,_StCheckIO
		,_StPTouch
		,_StWriteSerial
		,_StWriteFileData
		,_StLoop
		,_StIF
		,_StCheckAdjustGainDlg
	};
	for( int i=0; i<_countof(pszClassName) && bReval; i++ )
	{
		if( szVarName.Compare(pszClassName[i])==0 )
		{
			bReval = FALSE;
		}
	}
	for( int i=0; i<m_vecCheckBase.size() && bReval; i++ )
	{
		if( szVarName.Compare(m_vecCheckBase[i]->m_VarName.GetUnicode())==0 )
		{
			bReval = FALSE;
		}
	}
	for( int i=0; i<m_vecControl.size() && bReval; i++ )
	{
		if( szVarName.Compare(m_vecControl[i]->m_VarName.GetUnicode())==0 )
		{
			bReval = FALSE;
		}
	}
	for( int i=0; i<m_vecTexioControl.size() && bReval; i++ )
	{
		if( szVarName.Compare(m_vecTexioControl[i]->m_VarName.GetUnicode())==0 )
		{
			bReval = FALSE;
		}
	}
	for( int i=0; i<m_vecCustomDlg.size() && bReval; i++ )
	{
		if( szVarName.Compare(m_vecCustomDlg[i]->m_VarName.GetUnicode())==0 )
		{
			bReval = FALSE;
		}
	}
	for( int i=0; i<m_vecCommonBase.size() && bReval; i++ )
	{
		if( szVarName.Compare(m_vecCommonBase[i]->GetVarName()->GetUnicode())==0 )
		{
			bReval = FALSE;
		}
	}
	for( int i=0; i<m_vecValue.size() && bReval; i++ )
	{
		if( szVarName.Compare(m_vecValue[i]->GetVarName()->GetUnicode())==0 )
		{
			bReval = FALSE;
		}
	}
	return bReval;
}
//��1.0.0.1062

//-------------------------------------------------
//�錾 �N���X�Ȃ�
//Example
//  StDeviceEBus aDevice
//  StCheckSN aSN
//-------------------------------------------------
//��1.0.0.1072d
#ifdef GENCOMP_OLD
BOOL StInspectMain::ExecuteDeclaration(CString &szString)
#else
BOOL StInspectMain::ExecuteDeclaration(CString &szString, PBOOL pbCheck)
#endif
//��1.0.0.1072d
{
	BOOL bReval = FALSE;
	do
	{
		szString.Trim();

		//��1.0.0.1023 �J�Ԃ��p�ϐ��ǉ�
		//��1.0.0.1072d
#ifdef GENCOMP_OLD
		if( StValueFunction(szString) )
#else
		if( StValueFunction(szString,pbCheck) )
#endif
		//��1.0.0.1072d
		{
			bReval = TRUE;
			break;
		}
		//��1.0.0.1023 �J�Ԃ��p�ϐ��ǉ�

		INT iSpacePos = szString.Find(_T(" "));
		if( iSpacePos<0 ) break;
		CString szClass = szString.Left(iSpacePos);
		szClass.Trim();

		CString szTmp = szString.Mid( iSpacePos + 1 );
		szTmp.Trim();
		CString szVarName = szTmp;
		CString szArgument("");

		if( !SepareteString(szTmp, szVarName, szArgument ) )
			break;

		//��1.0.0.1072c
		//������ł͂Ȃ��ꍇ�A���l�ɂ���������邩�m�F���A�u������
		//ReplaceNumForData(szArgument);
		//��1.0.0.1072c

		//INT iPos=0;
		//if( (iPos=szTmp.Find(_T("(")))>0 )
		//{
		//	szVarName = szTmp.Left(iPos);
		//	szVarName.Trim();
		//	szArgument = szTmp.Mid(iPos+1);
		//	szArgument = szArgument.Left(szArgument.Find(_T(")")));
		//	szArgument.Trim();
		//}
		//else
		//if( (iPos=szTmp.Find(_T("=")))>0 )
		//{
		//	szVarName = szTmp.Left(iPos);
		//	szVarName.Trim();
		//	szArgument = szTmp.Mid(iPos+1);
		//	szArgument.Trim();
		//}

		//�f�o�C�X-----------------------------------------
		if( szClass==_StDeviceEBus )
		{
			//��1.0.0.1062
			if( CheckVariableName(szVarName)==FALSE ) break;
			//��1.0.0.1062
			StDeviceEBus *pDeviceEBus = new StDeviceEBus(szClass,szVarName);
			//��1.0.0.1062
			pDeviceEBus->SetScriptCheckMode(m_nScriptCheckMode);
			//��1.0.0.1062
			m_vecDeviceBase.push_back(pDeviceEBus);
			bReval = TRUE;
		}
		else
		if( szClass==_StDeviceMultiCam )
		{
			//��1.0.0.1062
			if( CheckVariableName(szVarName)==FALSE ) break;
			//��1.0.0.1062
			StDeviceMultiCam *pDeviceMulticam = new StDeviceMultiCam(szClass,szVarName);
			//��1.0.0.1062
			pDeviceMulticam->SetScriptCheckMode(m_nScriptCheckMode);
			//��1.0.0.1062
			m_vecDeviceBase.push_back(pDeviceMulticam);
			bReval = TRUE;
		}
		//��1.0.0.1044
		else
		if( szClass==_StDeviceGenICam )
		{
			//��1.0.0.1062
			if( CheckVariableName(szVarName)==FALSE ) break;
			//��1.0.0.1062
			StDeviceGenICam *pDeviceGenICam = new StDeviceGenICam(szClass,szVarName);
			//��1.0.0.1062
			pDeviceGenICam->SetScriptCheckMode(m_nScriptCheckMode);
			//��1.0.0.1062
			m_vecDeviceBase.push_back(pDeviceGenICam);
			bReval = TRUE;
		}
		//��1.0.0.1044
		//�V���A��COMM-----------------------------------------------------
		else
		if( szClass==_StSerialComm )
		{
			//��1.0.0.1062
			if( CheckVariableName(szVarName)==FALSE ) break;
			//��1.0.0.1062

			//��1.0.0.1015
			bReval = FALSE;
			//��1.0.0.1015
			CString szPortName = szArgument;
			if( m_pStructPortName )
			//��1.0.0.1015
				//m_pStructPortName->GetPortName(szArgument,szPortName);
				bReval = m_pStructPortName->GetPortName(szArgument,szPortName);
			if( bReval==FALSE )
			{
				for( int i=0; i<(int)m_vecDeviceBase.size(); i++ )
				{
					if( szArgument==m_vecDeviceBase[i]->m_VarName.GetAscii() )
					{
						if( m_vecDeviceBase[i]->m_ClassName==_StDeviceMultiCam )
						{
							StDeviceMultiCam *aMultiCam = (StDeviceMultiCam *)m_vecDeviceBase[i];
							CString szTmpPortName;
							aMultiCam->GetPortName(szTmpPortName);
							if( szTmpPortName.GetLength()>0 )
							{
								szPortName = szTmpPortName;
								bReval = TRUE;
							}
						}
						break;
					}
				}
			}
			//��1.0.0.1015
			StSerialComm *pSerialComm = new StSerialComm(szClass,szVarName,szPortName);
			//��1.0.0.1062
			pSerialComm->SetScriptCheckMode(m_nScriptCheckMode);
			//��1.0.0.1062

////TRACE(TEXT("@@@@@@@@@ pSerialComm=0x%x\n") , pSerialComm );
////StControl *aControl = pSerialComm;
////TRACE(TEXT("@@@@@@@@@ aControl=0x%x\n") , aControl );
////CRS232C_DSP *pCRS232C_DSP = pSerialComm;
////TRACE(TEXT("@@@@@@@@@ aControl=0x%x\n") , pCRS232C_DSP );

			//��1.0.0.1020
			//m_vecSerialComm.push_back(pSerialComm);
			pSerialComm->SetMainHandle((HANDLE)pSerialComm);
			m_vecControl.push_back(pSerialComm);
			//��1.0.0.1020

			bReval = TRUE;
		}

		//��1.0.0.1017
		else
		if( szClass==_StTerminal )
		{
			//��1.0.0.1062
			if( CheckVariableName(szVarName)==FALSE ) break;
			//��1.0.0.1062
			CString szAddress = szArgument;
			if( m_pStructAddress )
				m_pStructAddress->GetPortName(szArgument,szAddress);
			StTerminal *pTerminal = new StTerminal(szClass,szVarName,szAddress);
			//��1.0.0.1021
			if( pTerminal )
			{
				pTerminal->SetStructLightChannel(m_pStructLightChannel);
			}
			//��1.0.0.1021
			//��1.0.0.1062
			pTerminal->SetScriptCheckMode(m_nScriptCheckMode);
			//��1.0.0.1062
			//��1.0.0.1020
			//m_vecSerialComm.push_back(pTerminal);
			pTerminal->SetMainHandle((HANDLE)pTerminal);
			m_vecControl.push_back(pTerminal);
			//��1.0.0.1020
			bReval = TRUE;
		}
		//��1.0.0.1017

		//TexioControl-----------------------------------------------------
		else
		if( szClass==_StTexioControl )
		{
			//��1.0.0.1062
			if( CheckVariableName(szVarName)==FALSE ) break;
			//��1.0.0.1062
			//��1.0.0.1062
			//StTexioControl *pTexioControl = new StTexioControl(szClass,szVarName);
			StTexioControl *pTexioControl = new StTexioControl(szClass,szVarName,m_nScriptCheckMode);
			//��1.0.0.1062
			m_vecTexioControl.push_back(pTexioControl);
			bReval = TRUE;
		}

		//ImageMessageDlg-----------------------------------------------------
		else
		if( szClass==_StImageMessageDlg )
		{
			//��1.0.0.1062
			if( CheckVariableName(szVarName)==FALSE ) break;
			//��1.0.0.1062
			StImageMessageDlg *pImageMessageDlg = new StImageMessageDlg;
			pImageMessageDlg->m_VarName = szVarName;
			//��1.0.0.1048
			//m_vecImageMessageDlg.push_back(pImageMessageDlg);
			m_vecCustomDlg.push_back(pImageMessageDlg);
			//��1.0.0.1048
			bReval = TRUE;
		}
		//��1.0.0.1048
		//StHttpControlDlg-----------------------------------------------------
		else
		if( szClass==_StHttpControlDlg )
		{
			//��1.0.0.1062
			if( CheckVariableName(szVarName)==FALSE ) break;
			//��1.0.0.1062
			StHttpControlDlg *pStHttpControlDlg = new StHttpControlDlg;
			pStHttpControlDlg->m_VarName = szVarName;
			m_vecCustomDlg.push_back(pStHttpControlDlg);
			bReval = TRUE;
		}
		//��1.0.0.1048

		//��1.0.0.1023 �J�Ԃ��p�ϐ��ǉ�
		else
		if( szClass==_StValue )
		{
			//��1.0.0.1062
			if( CheckVariableName(szVarName)==FALSE ) break;
			//��1.0.0.1062
			StValue *pValue = new StValue(szVarName,szArgument);
			m_vecValue.push_back(pValue);
			bReval = TRUE;
		}
		//��1.0.0.1023 �J�Ԃ��p�ϐ��ǉ�

		//��1.0.0.1025 �t�@�C������ǉ�
		else
		if( szClass==_StFileOperation )
		{
			//��1.0.0.1062
			if( CheckVariableName(szVarName)==FALSE ) break;
			//��1.0.0.1062
			StFileOperation *pBase = new StFileOperation(szClass,szVarName,szArgument);
			//��1.0.0.1062
			pBase->SetScriptCheckMode(m_nScriptCheckMode);
			//��1.0.0.1062
			m_vecCommonBase.push_back(pBase);
			bReval = TRUE;
		}
		//��1.0.0.1025 �t�@�C������ǉ�

		//��1.0.0.1051
		else
		if( szClass==_StInterfaceGenICam )
		{
			//��1.0.0.1062
			if( CheckVariableName(szVarName)==FALSE ) break;
			//��1.0.0.1062

			//��1.0.0.1054
			//StInterfaceGenICam *pBase = new StInterfaceGenICam(szClass,szVarName,szArgument);
			StInterfaceGenICam *pBase = NULL;
			StDeviceBase *aDeviceBase = GetDeviceBase(szArgument);
			if( aDeviceBase )
			{
				pBase = new StInterfaceGenICam(szClass,szVarName,aDeviceBase);
			}
			else
			{
				pBase = new StInterfaceGenICam(szClass,szVarName,szArgument);
			}
			//��1.0.0.1054
			//��1.0.0.1062
			pBase->SetScriptCheckMode(m_nScriptCheckMode);
			//��1.0.0.1062
			m_vecCommonBase.push_back(pBase);
			bReval = TRUE;
		}
		//��1.0.0.1051
		//��1.0.0.1046 �J��������ǉ�
		else
		if( szClass==_StCameraControlDlg )
		{
			//��1.0.0.1062
			if( CheckVariableName(szVarName)==FALSE ) break;
			//��1.0.0.1062
			StCameraControlDlg *pCheck = new StCameraControlDlg(szClass,szVarName,GetDeviceBase(szArgument));
			//��1.0.0.1062
			pCheck->SetScriptCheckMode(m_nScriptCheckMode);
			//��1.0.0.1062
			pCheck->SetMainHandle((void *)pCheck);
			m_vecCheckBase.push_back(pCheck);
			//��1.0.0.1062
			//bReval = TRUE;
			bReval = GetDeviceBase(szArgument)==NULL?FALSE:TRUE;
			//��1.0.0.1062
		}
		//��1.0.0.1046 �J��������ǉ�

		//��1.0.0.1047
		else
		if( szClass==_StCheckSwitchDlg )
		{
			//��1.0.0.1062
			if( CheckVariableName(szVarName)==FALSE ) break;
			//��1.0.0.1062
			StCheckSwitchDlg *pCheck = new StCheckSwitchDlg(szClass,szVarName,GetDeviceBase(szArgument));
			//��1.0.0.1062
			pCheck->SetScriptCheckMode(m_nScriptCheckMode);
			//��1.0.0.1062
			pCheck->SetMainHandle((void *)pCheck);
			m_vecCheckBase.push_back(pCheck);
			//��1.0.0.1062
			//bReval = TRUE;
			bReval = GetDeviceBase(szArgument)==NULL?FALSE:TRUE;
			//��1.0.0.1062
		}
		//��1.0.0.1047

		//��1.0.0.1048
		else
		if( szClass==_StCheckAdjustVolumeDlg )
		{
			//��1.0.0.1062
			if( CheckVariableName(szVarName)==FALSE ) break;
			//��1.0.0.1062
			StCheckAdjustVolumeDlg *pCheck = new StCheckAdjustVolumeDlg(szClass,szVarName,GetDeviceBase(szArgument));
			//��1.0.0.1062
			pCheck->SetScriptCheckMode(m_nScriptCheckMode);
			//��1.0.0.1062
			pCheck->SetMainHandle((void *)pCheck);
			m_vecCheckBase.push_back(pCheck);
			//��1.0.0.1062
			//bReval = TRUE;
			bReval = GetDeviceBase(szArgument)==NULL?FALSE:TRUE;
			//��1.0.0.1062
		}
		//��1.0.0.1048

		//����-----------------------------------------------------
		else
		if( szClass==_StCheckSN )
		{
			//��1.0.0.1062
			if( CheckVariableName(szVarName)==FALSE ) break;
			//��1.0.0.1062
			StCheckSN *pCheck = new StCheckSN(szClass,szVarName,GetDeviceBase(szArgument));
			//��1.0.0.1062
			pCheck->SetScriptCheckMode(m_nScriptCheckMode);
			//��1.0.0.1062
			m_vecCheckBase.push_back(pCheck);
			//��1.0.0.1062
			//bReval = TRUE;
			bReval = GetDeviceBase(szArgument)==NULL?FALSE:TRUE;
			//��1.0.0.1062
		}
		else
		if( szClass==_StCheckControlRetry )
		{
			//��1.0.0.1062
			if( CheckVariableName(szVarName)==FALSE ) break;
			//��1.0.0.1062
			StCheckControlRetry *pCheck = new StCheckControlRetry(szClass,szVarName,GetDeviceBase(szArgument));
			//��1.0.0.1062
			pCheck->SetScriptCheckMode(m_nScriptCheckMode);
			//��1.0.0.1062
			pCheck->SetInspectionMain((void*)this);	//�\���p�̈�
			m_vecCheckBase.push_back(pCheck);
			//��1.0.0.1062
			//bReval = TRUE;
			bReval = GetDeviceBase(szArgument)==NULL?FALSE:TRUE;
			//��1.0.0.1062
		}
		else
		if( szClass==_StInputSerialDlg )
		{
			//��1.0.0.1062
			if( CheckVariableName(szVarName)==FALSE ) break;
			//��1.0.0.1062
			StInputSerialDlg *pCheck = new StInputSerialDlg(szClass,szVarName,GetDeviceBase(szArgument));
			//��1.0.0.1062
			pCheck->SetScriptCheckMode(m_nScriptCheckMode);
			//��1.0.0.1062
			pCheck->SetMainHandle((void *)pCheck);
			m_vecCheckBase.push_back(pCheck);	//��������̂�StCheckBase�̃n���h��
			bReval = TRUE;
		}

		else
		if( szClass==_StCheckSNDlg )
		{
			//��1.0.0.1062
			if( CheckVariableName(szVarName)==FALSE ) break;
			//��1.0.0.1062
			StCheckSNDlg *pCheck = new StCheckSNDlg(szClass,szVarName,GetDeviceBase(szArgument));
			//��1.0.0.1062
			pCheck->SetScriptCheckMode(m_nScriptCheckMode);
			//��1.0.0.1062
			pCheck->SetMainHandle((void *)pCheck);
			m_vecCheckBase.push_back(pCheck);	//��������̂�StCheckBase�̃n���h��
			//��1.0.0.1062
			//bReval = TRUE;
			bReval = GetDeviceBase(szArgument)==NULL?FALSE:TRUE;
			//��1.0.0.1062
		}

		else
		if( szClass==_StCheckSpBehavior )
		{
			//��1.0.0.1062
			if( CheckVariableName(szVarName)==FALSE ) break;
			//��1.0.0.1062
			StCheckSpBehavior *pCheck = new StCheckSpBehavior(szClass,szVarName,GetDeviceBase(szArgument));
			//��1.0.0.1062
			pCheck->SetScriptCheckMode(m_nScriptCheckMode);
			//��1.0.0.1062
			m_vecCheckBase.push_back(pCheck);	//��������̂�StCheckBase�̃n���h��
			//��1.0.0.1062
			//bReval = TRUE;
			bReval = GetDeviceBase(szArgument)==NULL?FALSE:TRUE;
			//��1.0.0.1062
		}

		else
		if( szClass==_StCheckDataLose )
		{
			//��1.0.0.1062
			if( CheckVariableName(szVarName)==FALSE ) break;
			//��1.0.0.1062
			StCheckDataLose *pCheck = new StCheckDataLose(szClass,szVarName,GetDeviceBase(szArgument));
			//��1.0.0.1062
			pCheck->SetScriptCheckMode(m_nScriptCheckMode);
			//��1.0.0.1062
			m_vecCheckBase.push_back(pCheck);	//��������̂�StCheckBase�̃n���h��
			//��1.0.0.1062
			//bReval = TRUE;
			bReval = GetDeviceBase(szArgument)==NULL?FALSE:TRUE;
			//��1.0.0.1062
		}
		//��1.0.0.1062
		else
		if( szClass==_StCheckDataLoseDlg )
		{
			if( CheckVariableName(szVarName)==FALSE ) break;
			StCheckDataLoseDlg *pCheck = new StCheckDataLoseDlg(szClass,szVarName,GetDeviceBase(szArgument));
			pCheck->SetScriptCheckMode(m_nScriptCheckMode);
			pCheck->SetMainHandle((void *)pCheck);
			m_vecCheckBase.push_back(pCheck);	//��������̂�StCheckBase�̃n���h��
			bReval = GetDeviceBase(szArgument)==NULL?FALSE:TRUE;
		}
		//��1.0.0.1062
		else
		if( szClass==_StCheckPixelDefect )
		{
			//��1.0.0.1062
			if( CheckVariableName(szVarName)==FALSE ) break;
			//��1.0.0.1062
			StCheckPixelDefect *pCheck = new StCheckPixelDefect(szClass,szVarName,GetDeviceBase(szArgument));
			//��1.0.0.1062
			pCheck->SetScriptCheckMode(m_nScriptCheckMode);
			//��1.0.0.1062
			m_vecCheckBase.push_back(pCheck);	//��������̂�StCheckBase�̃n���h��
			//��1.0.0.1062
			//bReval = TRUE;
			bReval = GetDeviceBase(szArgument)==NULL?FALSE:TRUE;
			//��1.0.0.1062
		}

		//��1.0.0.1020
		else
		if( szClass==_StCheckPixelCorrect )
		{
			//��1.0.0.1062
			if( CheckVariableName(szVarName)==FALSE ) break;
			//��1.0.0.1062
			StCheckPixelCorrect *pCheck = new StCheckPixelCorrect(szClass,szVarName,GetDeviceBase(szArgument));
			//��1.0.0.1062
			pCheck->SetScriptCheckMode(m_nScriptCheckMode);
			//��1.0.0.1062
			pCheck->SetInspectionMain((void*)this);	//�\���p�̈�
			pCheck->SetMainHandle((void *)pCheck);
			m_vecCheckBase.push_back(pCheck);	//��������̂�StCheckBase�̃n���h��
			//��1.0.0.1062
			//bReval = TRUE;
			bReval = GetDeviceBase(szArgument)==NULL?FALSE:TRUE;
			//��1.0.0.1062
		}
		//��1.0.0.1020
		//��1.0.0.1021
		else
		if( szClass==_StCheckTemperature )
		{
			//��1.0.0.1062
			if( CheckVariableName(szVarName)==FALSE ) break;
			//��1.0.0.1062
			StCheckTemperature *pCheck = new StCheckTemperature(szClass,szVarName,GetDeviceBase(szArgument));
			//��1.0.0.1062
			pCheck->SetScriptCheckMode(m_nScriptCheckMode);
			//��1.0.0.1062
			m_vecCheckBase.push_back(pCheck);	//��������̂�StCheckBase�̃n���h��
			//��1.0.0.1062
			//bReval = TRUE;
			bReval = GetDeviceBase(szArgument)==NULL?FALSE:TRUE;
			//��1.0.0.1062
		}
		else
		if( szClass==_StCheckTestPattern )
		{
			//��1.0.0.1062
			if( CheckVariableName(szVarName)==FALSE ) break;
			//��1.0.0.1062
			StCheckTestPattern *pCheck = new StCheckTestPattern(szClass,szVarName,GetDeviceBase(szArgument));
			//��1.0.0.1062
			pCheck->SetScriptCheckMode(m_nScriptCheckMode);
			//��1.0.0.1062
			m_vecCheckBase.push_back(pCheck);	//��������̂�StCheckBase�̃n���h��
			//��1.0.0.1062
			//bReval = TRUE;
			bReval = GetDeviceBase(szArgument)==NULL?FALSE:TRUE;
			//��1.0.0.1062
		}
		//��1.0.0.1021
		//��1.0.0.1022
		else
		if( szClass==_StCheckEllitoLVDSDlg )
		{
			//��1.0.0.1062
			if( CheckVariableName(szVarName)==FALSE ) break;
			//��1.0.0.1062
			StCheckEllitoLVDSDlg *pCheck = new StCheckEllitoLVDSDlg(szClass,szVarName,GetDeviceBase(szArgument));
			//��1.0.0.1062
			pCheck->SetScriptCheckMode(m_nScriptCheckMode);
			//��1.0.0.1062
			pCheck->SetMainHandle((void *)pCheck);
			m_vecCheckBase.push_back(pCheck);	//��������̂�StCheckBase�̃n���h��
			//��1.0.0.1062
			//bReval = TRUE;
			bReval = GetDeviceBase(szArgument)==NULL?FALSE:TRUE;
			//��1.0.0.1062
		}
		//��1.0.0.1022
		//��1.0.0.1059
		else
		if( szClass==_StCheckEllitoLVDS )
		{
			//��1.0.0.1062
			if( CheckVariableName(szVarName)==FALSE ) break;
			//��1.0.0.1062
			StCheckEllitoLVDS *pCheck = new StCheckEllitoLVDS(szClass,szVarName,GetDeviceBase(szArgument));
			//��1.0.0.1062
			pCheck->SetScriptCheckMode(m_nScriptCheckMode);
			//��1.0.0.1062
			m_vecCheckBase.push_back(pCheck);	//��������̂�StCheckBase�̃n���h��
			//��1.0.0.1062
			//bReval = TRUE;
			bReval = GetDeviceBase(szArgument)==NULL?FALSE:TRUE;
			//��1.0.0.1062
		}
		//��1.0.0.1059

		//��1.0.0.1033
		else
		if( szClass==_StCheckFramerate )
		{
			//��1.0.0.1062
			if( CheckVariableName(szVarName)==FALSE ) break;
			//��1.0.0.1062
			StCheckFramerate *pCheck = new StCheckFramerate(szClass,szVarName,GetDeviceBase(szArgument));
			//��1.0.0.1062
			pCheck->SetScriptCheckMode(m_nScriptCheckMode);
			//��1.0.0.1062
			m_vecCheckBase.push_back(pCheck);	//��������̂�StCheckBase�̃n���h��
			//��1.0.0.1062
			//bReval = TRUE;
			bReval = GetDeviceBase(szArgument)==NULL?FALSE:TRUE;
			//��1.0.0.1062
		}
		//��1.0.0.1033
		//��1.0.0.1040
		else
		if( szClass==_StCheckCSVDust )
		{
			//��1.0.0.1062
			if( CheckVariableName(szVarName)==FALSE ) break;
			//��1.0.0.1062
			StCheckCSVDust *pCheck = new StCheckCSVDust(szClass,szVarName,GetDeviceBase(szArgument));
			//��1.0.0.1062
			pCheck->SetScriptCheckMode(m_nScriptCheckMode);
			//��1.0.0.1062
			m_vecCheckBase.push_back(pCheck);	//��������̂�StCheckBase�̃n���h��
			//��1.0.0.1062
			//bReval = TRUE;
			bReval = GetDeviceBase(szArgument)==NULL?FALSE:TRUE;
			//��1.0.0.1062
		}
		//��1.0.0.1040
		//��1.0.0.1044
		else
		if( szClass==_StCheckAdjustOffset )
		{
			//��1.0.0.1062
			if( CheckVariableName(szVarName)==FALSE ) break;
			//��1.0.0.1062
			StCheckAdjustOffset *pCheck = new StCheckAdjustOffset(szClass,szVarName,GetDeviceBase(szArgument), this );
			//��1.0.0.1062
			pCheck->SetScriptCheckMode(m_nScriptCheckMode);
			//��1.0.0.1062
			m_vecCheckBase.push_back(pCheck);	//��������̂�StCheckBase�̃n���h��
			//��1.0.0.1062
			//bReval = TRUE;
			bReval = GetDeviceBase(szArgument)==NULL?FALSE:TRUE;
			//��1.0.0.1062
		}
		//��1.0.0.1044

		else
		if( szClass==_StCheckDustDlg )
		{
			//��1.0.0.1062
			if( CheckVariableName(szVarName)==FALSE ) break;
			//��1.0.0.1062
			StCheckDustDlg *pCheck = new StCheckDustDlg(szClass,szVarName,GetDeviceBase(szArgument));
			//��1.0.0.1062
			pCheck->SetScriptCheckMode(m_nScriptCheckMode);
			//��1.0.0.1062
			pCheck->SetMainHandle((void *)pCheck);
			m_vecCheckBase.push_back(pCheck);	//��������̂�StCheckBase�̃n���h��
			//��1.0.0.1062
			//bReval = TRUE;
			bReval = GetDeviceBase(szArgument)==NULL?FALSE:TRUE;
			//��1.0.0.1062
		}
		else
		if( szClass==_StCheckMessage )
		{
			//��1.0.0.1062
			if( CheckVariableName(szVarName)==FALSE ) break;
			//��1.0.0.1062
			StCheckMessage *pCheck = new StCheckMessage(szClass,szVarName,GetDeviceBase(szArgument));
			//��1.0.0.1062
			pCheck->SetScriptCheckMode(m_nScriptCheckMode);
			//��1.0.0.1062
			m_vecCheckBase.push_back(pCheck);	//��������̂�StCheckBase�̃n���h��
			bReval = TRUE;
		}

		else
		if( szClass==_StCheckAdjustLinetolineDlg )
		{
			//��1.0.0.1062
			if( CheckVariableName(szVarName)==FALSE ) break;
			//��1.0.0.1062
			StCheckAdjustLinetolineDlg *pCheck = new StCheckAdjustLinetolineDlg(szClass,szVarName,GetDeviceBase(szArgument));
			//��1.0.0.1062
			pCheck->SetScriptCheckMode(m_nScriptCheckMode);
			//��1.0.0.1062
			pCheck->SetMainHandle((void *)pCheck);
			m_vecCheckBase.push_back(pCheck);	//��������̂�StCheckBase�̃n���h��
			//��1.0.0.1062
			//bReval = TRUE;
			bReval = GetDeviceBase(szArgument)==NULL?FALSE:TRUE;
			//��1.0.0.1062
		}
		else
		if( szClass==_StCheckAdjustLinetoline )
		{
			//��1.0.0.1062
			if( CheckVariableName(szVarName)==FALSE ) break;
			//��1.0.0.1062
			StCheckAdjustLinetoline *pCheck = new StCheckAdjustLinetoline(szClass,szVarName,GetDeviceBase(szArgument),this);
			//��1.0.0.1062
			pCheck->SetScriptCheckMode(m_nScriptCheckMode);
			//��1.0.0.1062
			m_vecCheckBase.push_back(pCheck);	//��������̂�StCheckBase�̃n���h��
			//��1.0.0.1062
			//bReval = TRUE;
			bReval = GetDeviceBase(szArgument)==NULL?FALSE:TRUE;
			//��1.0.0.1062
		}

		else
		if( szClass==_StCheckEffectivePixel )
		{
			//��1.0.0.1062
			if( CheckVariableName(szVarName)==FALSE ) break;
			//��1.0.0.1062
			StCheckEffectivePixel *pCheck = new StCheckEffectivePixel(szClass,szVarName,GetDeviceBase(szArgument));
			//��1.0.0.1062
			pCheck->SetScriptCheckMode(m_nScriptCheckMode);
			//��1.0.0.1062
			m_vecCheckBase.push_back(pCheck);	//��������̂�StCheckBase�̃n���h��
			//��1.0.0.1062
			//bReval = TRUE;
			bReval = GetDeviceBase(szArgument)==NULL?FALSE:TRUE;
			//��1.0.0.1062
		}

		else
		if( szClass==_StInputSensorVersionDlg )
		{
			//��1.0.0.1062
			if( CheckVariableName(szVarName)==FALSE ) break;
			//��1.0.0.1062
			StInputSensorVersionDlg *pCheck = new StInputSensorVersionDlg(szClass,szVarName,GetDeviceBase(szArgument));
			//��1.0.0.1062
			pCheck->SetScriptCheckMode(m_nScriptCheckMode);
			//��1.0.0.1062
			pCheck->SetMainHandle((void *)pCheck);
			m_vecCheckBase.push_back(pCheck);	//��������̂�StCheckBase�̃n���h��
			bReval = TRUE;
		}
		else
		if( szClass==_StInputSensorSerialDlg )
		{
			//��1.0.0.1062
			if( CheckVariableName(szVarName)==FALSE ) break;
			//��1.0.0.1062
			StInputSensorSerialDlg *pCheck = new StInputSensorSerialDlg(szClass,szVarName,GetDeviceBase(szArgument));
			//��1.0.0.1062
			pCheck->SetScriptCheckMode(m_nScriptCheckMode);
			//��1.0.0.1062
			pCheck->SetMainHandle((void *)pCheck);
			m_vecCheckBase.push_back(pCheck);	//��������̂�StCheckBase�̃n���h��
			bReval = TRUE;
		}
		else
		if( szClass==_StLightControlDlg )
		{
			//��1.0.0.1062
			if( CheckVariableName(szVarName)==FALSE ) break;
			//��1.0.0.1062
			StLightControlDlg *pCheck = new StLightControlDlg(szClass,szVarName,GetDeviceBase(szArgument));
			//��1.0.0.1062
			pCheck->SetScriptCheckMode(m_nScriptCheckMode);
			//��1.0.0.1062
			pCheck->SetMainHandle((void *)pCheck);
			m_vecCheckBase.push_back(pCheck);	//��������̂�StCheckBase�̃n���h��
			//��1.0.0.1062
			//bReval = TRUE;
			bReval = GetDeviceBase(szArgument)==NULL?FALSE:TRUE;
			//��1.0.0.1062
		}
		else
		if( szClass==_StLightControl )
		{
			//��1.0.0.1062
			if( CheckVariableName(szVarName)==FALSE ) break;
			//��1.0.0.1062
			StLightControl *pCheck = new StLightControl(szClass,szVarName,GetDeviceBase(szArgument),this);
			//��1.0.0.1062
			pCheck->SetScriptCheckMode(m_nScriptCheckMode);
			//��1.0.0.1062
			m_vecCheckBase.push_back(pCheck);
			//��1.0.0.1062
			//bReval = TRUE;
			bReval = GetDeviceBase(szArgument)==NULL?FALSE:TRUE;
			//��1.0.0.1062
		}
		else
		if( szClass==_StCheckPower )
		{
			//��1.0.0.1062
			if( CheckVariableName(szVarName)==FALSE ) break;
			//��1.0.0.1062
			StCheckPower *pCheck = new StCheckPower(szClass,szVarName,GetDeviceBase(szArgument));
			//��1.0.0.1062
			pCheck->SetScriptCheckMode(m_nScriptCheckMode);
			//��1.0.0.1062
			m_vecCheckBase.push_back(pCheck);
			bReval = TRUE;
		}
		else
		if( szClass==_StCheckCommon )
		{
			//��1.0.0.1062
			if( CheckVariableName(szVarName)==FALSE ) break;
			//��1.0.0.1062
			StCheckCommon *pCheck = new StCheckCommon(szClass,szVarName,GetDeviceBase(szArgument));
			//��1.0.0.1062
			pCheck->SetScriptCheckMode(m_nScriptCheckMode);
			//��1.0.0.1062
			m_vecCheckBase.push_back(pCheck);
			//��1.0.0.1062
			//bReval = TRUE;
			bReval = GetDeviceBase(szArgument)==NULL?FALSE:TRUE;
			//��1.0.0.1062
		}
		else
		if( szClass==_StCheckSerialVersion )
		{
			//��1.0.0.1062
			if( CheckVariableName(szVarName)==FALSE ) break;
			//��1.0.0.1062
			StCheckSerialVersion *pCheck = new StCheckSerialVersion(szClass,szVarName,GetDeviceBase(szArgument));
			//��1.0.0.1062
			pCheck->SetScriptCheckMode(m_nScriptCheckMode);
			//��1.0.0.1062
			m_vecCheckBase.push_back(pCheck);
			pCheck->function(_T("SetDeviceSerialNumber"), mSerialNumber);	//�V���A���ԍ�������Ă���
			//��1.0.0.15
			pCheck->function(_T("SetSensorSerialNumber"), mSensorSerialNumber);	//�Z���T�[�V���A���ԍ�������Ă���
			//��1.0.0.15
			//��1.0.0.1062
			//bReval = TRUE;
			bReval = GetDeviceBase(szArgument)==NULL?FALSE:TRUE;
			//��1.0.0.1062
		}
		else
		if( szClass==_StCheckIO )
		{
			//��1.0.0.1062
			if( CheckVariableName(szVarName)==FALSE ) break;
			//��1.0.0.1062
			StCheckIO *pCheck = new StCheckIO(szClass,szVarName,GetDeviceBase(szArgument));
			//��1.0.0.1062
			pCheck->SetScriptCheckMode(m_nScriptCheckMode);
			//��1.0.0.1062
			m_vecCheckBase.push_back(pCheck);
			//��1.0.0.1062
			//bReval = TRUE;
			bReval = GetDeviceBase(szArgument)==NULL?FALSE:TRUE;
			//��1.0.0.1062
		}
		else
		if( szClass==_StPTouch )
		{
			//��1.0.0.1062
			if( CheckVariableName(szVarName)==FALSE ) break;
			//��1.0.0.1062
			StPTouch *pCheck = new StPTouch(szClass,szVarName,GetDeviceBase(szArgument));
			//��1.0.0.1062
			pCheck->SetScriptCheckMode(m_nScriptCheckMode);
			//��1.0.0.1062
			m_vecCheckBase.push_back(pCheck);
			bReval = TRUE;
		}

		//��1.0.0.1015
		else
		if( szClass==_StWriteSerial )
		{
			//��1.0.0.1062
			if( CheckVariableName(szVarName)==FALSE ) break;
			//��1.0.0.1062
			StWriteSerial *pCheck = new StWriteSerial(szClass,szVarName,GetDeviceBase(szArgument));
			//��1.0.0.1062
			pCheck->SetScriptCheckMode(m_nScriptCheckMode);
			//��1.0.0.1062
			m_vecCheckBase.push_back(pCheck);
			//��1.0.0.1062
			//bReval = TRUE;
			bReval = GetDeviceBase(szArgument)==NULL?FALSE:TRUE;
			//��1.0.0.1062
		}
		//��1.0.0.1015
		//��1.0.0.1058
		else
		if( szClass==_StWriteFileData )
		{
			//��1.0.0.1062
			if( CheckVariableName(szVarName)==FALSE ) break;
			//��1.0.0.1062
			StWriteFileData *pCheck = new StWriteFileData(szClass,szVarName,GetDeviceBase(szArgument));
			//��1.0.0.1062
			pCheck->SetScriptCheckMode(m_nScriptCheckMode);
			//��1.0.0.1062
			m_vecCheckBase.push_back(pCheck);
			//��1.0.0.1062
			//bReval = TRUE;
			bReval = GetDeviceBase(szArgument)==NULL?FALSE:TRUE;
			//��1.0.0.1062
		}
		//��1.0.0.1058

		//��1.0.0.1023
		else
		if( szClass==_StLoop )
		{
			//��1.0.0.1062
			if( CheckVariableName(szVarName)==FALSE ) break;
			//��1.0.0.1062
			StLoop *pCheck = new StLoop(szClass,szVarName,szArgument);
			//��1.0.0.1062
			pCheck->SetScriptCheckMode(m_nScriptCheckMode);
			//��1.0.0.1062
			m_vecCheckBase.push_back(pCheck);
			bReval = TRUE;
		}
		//��1.0.0.1023

		//��1.0.0.1030
		else
		if( szClass==_StIF )
		{
			//��1.0.0.1062
			if( CheckVariableName(szVarName)==FALSE ) break;
			//��1.0.0.1062
			StIF *pCheck = new StIF(szClass,szVarName,szArgument);
			//��1.0.0.1062
			pCheck->SetScriptCheckMode(m_nScriptCheckMode);
			//��1.0.0.1062
			m_vecCheckBase.push_back(pCheck);
			bReval = TRUE;
		}
		//��1.0.0.1030
		//��1.0.0.1049
		else
		if( szClass==_StCheckAdjustGainDlg )
		{
			//��1.0.0.1062
			if( CheckVariableName(szVarName)==FALSE ) break;
			//��1.0.0.1062
			StCheckAdjustGainDlg *pCheck = new StCheckAdjustGainDlg(szClass,szVarName,GetDeviceBase(szArgument));
			//��1.0.0.1062
			pCheck->SetScriptCheckMode(m_nScriptCheckMode);
			//��1.0.0.1062
			pCheck->SetMainHandle((void *)pCheck);
			m_vecCheckBase.push_back(pCheck);	//��������̂�StCheckBase�̃n���h��
			//��1.0.0.1062
			//bReval = TRUE;
			bReval = GetDeviceBase(szArgument)==NULL?FALSE:TRUE;
			//��1.0.0.1062
		}
		//��1.0.0.1049

		//�N���X�̍폜
		else
		if( szClass=="delete" )
		{
			//Device
			for( DWORD i=0; i<m_vecDeviceBase.size() && bReval==FALSE; i++ )
			{
				if( szVarName==m_vecDeviceBase[i]->m_VarName.GetAscii() )
				{
					bReval = DeleteDeviceBase(i);
				}
			}

			//SerialComm
			//��1.0.0.1020
			//for( DWORD i=0; i<m_vecSerialComm.size() && bReval==FALSE; i++ )
			for( DWORD i=0; i<m_vecControl.size() && bReval==FALSE; i++ )
			//��1.0.0.1020
			{
				//��1.0.0.1020
				//if( szVarName==m_vecSerialComm[i]->m_VarName.GetAscii() )
				if( szVarName==m_vecControl[i]->m_VarName.GetAscii() )
				//��1.0.0.1020
				{
					bReval = DeleteSerialComm(i);
				}
			}

			//TexioControl
			for( DWORD i=0; i<m_vecTexioControl.size() && bReval==FALSE; i++ )
			{
				if( szVarName==m_vecTexioControl[i]->m_VarName.GetAscii() )
				{
					bReval = DeleteTexioControl(i);
				}
			}

			//Inspection
			for( DWORD i=0; i<m_vecCheckBase.size() && bReval==FALSE; i++ )
			{
				if( szVarName==m_vecCheckBase[i]->m_VarName.GetAscii() )
				{
					bReval = DeleteCheckBase(i);
				}
			}

			//ImageMessageDlg
			//��1.0.0.1048
			//for( DWORD i=0; i<m_vecImageMessageDlg.size() && bReval==FALSE; i++ )
			//{
			//	if( szVarName==m_vecImageMessageDlg[i]->m_VarName.GetAscii() )
			//	{
			//		bReval = DeleteImageMessageDlg(i);
			//	}
			//}
			for( DWORD i=0; i<m_vecCustomDlg.size() && bReval==FALSE; i++ )
			{
				if( szVarName==m_vecCustomDlg[i]->m_VarName.GetAscii() )
				{
					bReval = DeleteCustomDlg(i);
				}
			}
			//��1.0.0.1048

			//��1.0.0.1023 �J�Ԃ��p�ϐ��ǉ�
			for( DWORD i=0; i<m_vecValue.size() && bReval==FALSE; i++ )
			{
				if( szVarName==m_vecValue[i]->GetVarName()->GetAscii() )
				{
					bReval = DeleteValue(i);
				}
			}
			//��1.0.0.1023 �J�Ԃ��p�ϐ��ǉ�

			//��1.0.0.1025 �t�@�C������ǉ�
			for( DWORD i=0; i<m_vecCommonBase.size() && bReval==FALSE; i++ )
			{
				if( szVarName==m_vecCommonBase[i]->GetVarName()->GetAscii() )
				{
					bReval = DeleteCommonBase(i);
				}
			}
			//��1.0.0.1025 �t�@�C������ǉ�
		}
	}while(0);
	return bReval;
}

//��1.0.0.1023 �J�Ԃ��p
//-------------------------------------------------------------------------------------
//���Z�̋L�q���t�|�[�����h�L�q�ɂ���i�J���}��؂�j
//���Z�q +-*/�̂S���
//�J�b�R ()
//szDst�F�J���}��؂�B
//-------------------------------------------------------------------------------------
void ChangeReversePoland( CString szSrc, CString &szDst )
{
	PTCHAR szExpresssion[] = {_T("+"),_T("-"),_T("*"),_T("/"),_T("("),_T(")")};
	std::vector <CString> vszStac; 

	//�󔒍폜
	szSrc.Remove(' ');

	CString szValue("");
	INT nParenthesis = 0;
	for( int i=0; i<szSrc.GetLength(); i++ )
	{
		INT nExpPos = -1;
		for( int j=0; j<_countof(szExpresssion); j++ )
		{
			if( szSrc.Mid(i,1)==szExpresssion[j] )
			{
				nExpPos = j;
				break;
			}
		}
		if( nExpPos<0 )
		{
			szValue += szSrc.Mid(i,1);
		}
		else
		{
			if( szValue.GetLength() )
			{
				//Output
				if( szDst.GetLength()>0 ) szDst+=_T(",");
				szDst += szValue;
				szValue.Empty();
			}

			if( _tcscmp(szExpresssion[nExpPos],_T("("))==0 )
			{
				nParenthesis++;
				vszStac.push_back(szExpresssion[nExpPos]);
			}
			else
			if( _tcscmp(szExpresssion[nExpPos],_T(")"))==0 )
			{
				nParenthesis--;
				while(vszStac.size()>0 )
				{
					CString szTmp = vszStac[vszStac.size()-1];
					vszStac.pop_back();
					if( szTmp==_T("(") )
						break;
					else
					{
						//Output
						if( szDst.GetLength()>0 ) szDst+=_T(",");
						szDst += szTmp;
					}
				};
			}
			else
			if( _tcscmp(szExpresssion[nExpPos],_T("*"))==0 || _tcscmp(szExpresssion[nExpPos],_T("/"))==0 )
			{

				vszStac.push_back(szExpresssion[nExpPos]);

				//Output
				//if( szDst.GetLength()>0 ) szDst+=_T(",");
				//szDst += szExpresssion[nExpPos];

			}
			else
			if( _tcscmp(szExpresssion[nExpPos],_T("+"))==0 || _tcscmp(szExpresssion[nExpPos],_T("-"))==0 )
			{
				BOOL bSet = FALSE;
				if( vszStac.size()>0 )
				{
					if( _tcscmp(vszStac[vszStac.size()-1],_T("*"))==0 || _tcscmp(vszStac[vszStac.size()-1],_T("/"))==0 )
					{
						//Output
						if( szDst.GetLength()>0 ) szDst+=_T(",");
						szDst += vszStac[vszStac.size()-1];
						vszStac[vszStac.size()-1] = szExpresssion[nExpPos];
						bSet = TRUE;
					}
				}
				if( !bSet )
				{
					vszStac.push_back(szExpresssion[nExpPos]);
				}


				//if( nParenthesis>0 )
				//{
				//	vszStac.push_back(szExpresssion[nExpPos]);
				//}
				//else
				//{
				//	//Output
				//	if( szDst.GetLength()>0 ) szDst+=_T(",");
				//	szDst += szExpresssion[nExpPos];
				//}
			}

		}
	}
	if( szValue.GetLength() )
	{
		//Output
		if( szDst.GetLength()>0 ) szDst+=_T(",");
		szDst += szValue;

	}

	while(vszStac.size()>0 )
	{
		CString szTmp = vszStac[vszStac.size()-1];
		vszStac.pop_back();
		if( szTmp==_T("(") )
			break;
		else
		{
			//Output
			if( szDst.GetLength()>0 ) szDst+=_T(",");
			szDst += szTmp;
		}
	};


}

//-------------------------------------------------------------------------------------
//�t�|�[�����h�����Z
//���Z�q +-*/�̂S���
//bVariable:�ϐ����͂����Ă��邩�ǂ���
//-------------------------------------------------------------------------------------
BOOL OperationMain( std::vector <StValue *> &vecValue, CString szString, double &dblValue, BOOL &bVariable )
{
	BOOL bReval = TRUE;

//if( szString==_T("VER4.004.11") )
//	Sleep(9);
	//�����������Ă���ꍇ���̂܂ܔ�����B
	PTCHAR szJudgement[] = {_T("=="),_T("!="),_T("<="),_T(">="),_T("<"),_T(">")};
	for( int i=0; i<_countof(szJudgement); i++ )
	{
		if( szString.Find(szJudgement[i])>=0 )
		{
			return FALSE;
		}
	}

	CString szPoland("");	//�J���}��؂�
	ChangeReversePoland( szString, szPoland );

	PTCHAR szExpresssion[] = {_T("+"),_T("-"),_T("*"),_T("/")};
	std::vector <double> vdblStac; 
	bVariable = FALSE;
	do
	{
		INT nCammaPos = szPoland.Find(_T(","));
		CString szValue = szPoland;
		if( nCammaPos>=0 )
		{
			szValue = szValue.Left(nCammaPos);
			szPoland = szPoland.Mid(nCammaPos+1);
		}
		else
		{
			szPoland.Empty();
		}
		INT nExpPos = -1;
		for( int i=0; i<_countof(szExpresssion) && nExpPos<0; i++ )
		{
			if( szValue==szExpresssion[i] )
			{
				nExpPos = i;
			}
		}
		if( nExpPos>=0 )
		{
			//���Z
			if( vdblStac.size()<2 )
			{
				bReval = FALSE;
				break;
			}
			double dblA = vdblStac[vdblStac.size()-1];
			vdblStac.pop_back();
			double dblB = vdblStac[vdblStac.size()-1];
			vdblStac.pop_back();
			if( nExpPos==0 )
			{
				dblA = dblB + dblA;
			}
			else
			if( nExpPos==1 )
			{
				dblA = dblB - dblA;
			}
			else
			if( nExpPos==2 )
			{
				dblA = dblB * dblA;
			}
			else
			if( nExpPos==3 )
			{
				dblA = dblB / dblA;
			}
			vdblStac.push_back(dblA);
		}
		else
		{
			//Stac
			double dblVal;
			BOOL bGet = FALSE;
			for( int i=0; i<(int)vecValue.size(); i++ )
			{
				if( szValue==vecValue[i]->GetVarName()->GetUnicode() )
				{
					if( vecValue[i]->GetMode()!=0 )
					{
						bReval = FALSE;
						break;
					}
					bGet = TRUE;
					dblVal = vecValue[i]->GetValue();
					bVariable = TRUE;
				}
			}
			if( bReval )
			{
				if( !bGet )
				{
					dblVal = _ttof(szValue);
					//��1.0.0.1024	���������Ƃ悭�Ȃ��@���l����Ȃ����̂܂Ő��l�Ɣ��f�����B
					//bVariable = TRUE;
					//��1.0.0.1024
				}
			}
			vdblStac.push_back(dblVal);
		}
	}while(szPoland.GetLength()>0);

	if( bReval )
	{
		if( vdblStac.size()!=1 )
		{
			bReval = FALSE;
		}
		if( bReval )
		{
			dblValue = vdblStac[0];
		}
	}
	return bReval;

}


//��1.0.0.1072d
//������Ή�  StIF.cpp����Ă΂��
//-------------------------------------------------------------------------------
//BOOL &bVariable  :  �ϐ����͂����Ă��邩�ǂ���
//BOOL &bRetString    : false:���l true:������
//-------------------------------------------------------------------------------
#ifdef IF_OLD
#else
BOOL OperationMain2( std::vector <StValue *> &vecValue, CString szString, double &dblValue, CString &szValueString, BOOL &bVariable, BOOL &bRetString )
{
	BOOL bReval = TRUE;

	//�����������Ă���ꍇ���̂܂ܔ�����B
	PTCHAR szJudgement[] = {_T("=="),_T("!="),_T("<="),_T(">="),_T("<"),_T(">")};
	for( int i=0; i<_countof(szJudgement); i++ )
	{
		if( szString.Find(szJudgement[i])>=0 )
		{
			return FALSE;
		}
	}
	szString.Trim();
	//�����񂩂ǂ���
	BOOL bString = FALSE;
	if( szString.Left(1)==_T("\"") )
	{
		bString = TRUE;
	}
	if( !bString )
	{
		CString szFirstString = szString;
		INT nPos = szString.Find(_T("+"));
		//�ϐ����ǂ������f
		if( nPos>0 )
		{
			szFirstString = szString.Left(nPos);
		}
		szFirstString.Trim();
		for( int i=0; i<(int)vecValue.size(); i++ )
		{
			if( szFirstString==vecValue[i]->GetVarName()->GetUnicode() )
			{
				if( vecValue[i]->GetMode()!=0 )	//������
				{
					bString = TRUE;
					break;
				}
			}
		}
	}
	if( bString )
	{
		CString szResultString(_T(""));
		do
		{
			INT nPos = szString.Find(_T("+"));
			CString szTmpString = szString;
			if( nPos>=0 )
			{
				szTmpString = szString.Left(nPos);
				szString = szString.Mid(nPos+1);
			}
			else
				szString.Empty();
			szTmpString.Trim();
			if( szTmpString.Left(1)==_T("\"") )
			{
				if( szTmpString.Right(1)!=_T("\"") )
				{
					bReval = FALSE;
					break;
				}
				szTmpString = szTmpString.Mid(1);
				szTmpString = szTmpString.Left(szTmpString.GetLength()-1);
				szResultString += szTmpString;
			}
			else
			{

				for( int i=0; i<(int)vecValue.size(); i++ )
				{
					if( szTmpString==vecValue[i]->GetVarName()->GetUnicode() )
					{
						if( vecValue[i]->GetMode()!=0 )
						{
							//������
							szTmpString = vecValue[i]->GetValueString();
							if( szTmpString.Left(1)==_T("\"") )
							{
								szTmpString = szTmpString.Mid(1);
								if( szTmpString.Right(1)==_T("\"") )
									szTmpString = szTmpString.Left(szTmpString.GetLength()-1);
							}
							szResultString += szTmpString;
						}
						else
						{	
							//���l
							bReval = FALSE;
							break;
						}
					}
				}
			}
		}while(szString.GetLength()>0 && bReval);

		if( bReval )
		{
			szValueString = CString(_T("")) + szResultString;
			szValueString += CString(_T(""));
			bRetString = TRUE;
		}
		return bReval;
	}
	bRetString = FALSE;
	return OperationMain( vecValue, szString, dblValue, bVariable );
}
#endif
//��1.0.0.1072d



BOOL OperationString( std::vector <StValue *> &vecValue, CString szString, CString &szValue, BOOL &bVariable )
{
	BOOL bReval = FALSE;
	bVariable = FALSE;
	for( int i=0; i<(int)vecValue.size(); i++ )
	{
		//��1.0.0.1031
		//if( szValue==vecValue[i]->GetVarName()->GetUnicode() )
		if( szString==vecValue[i]->GetVarName()->GetUnicode() )
		//��1.0.0.1031
		{
			if( vecValue[i]->GetMode()!=0 )
			{
				szValue = vecValue[i]->GetValueString();
				bVariable = TRUE;
				bReval = TRUE;
				break;
			}
		}
	}
	if( bVariable==FALSE )
	{
		szValue = szString;
		bReval = TRUE;
	}
	return bReval;
}

//��1.0.0.1031
BOOL OperationStValue( std::vector <StValue *> &vecValue, CString szString, CString &szValue )
{

	CString szPoland = szString;
	CString szChange(_T(""));
	do
	{
		INT nCammaPos = szPoland.Find(_T(","));
		CString szTmp = szPoland;
		if( nCammaPos>=0 )
		{
			szTmp = szPoland.Left(nCammaPos);
			szPoland = szPoland.Mid(nCammaPos+1);
		}
		else
		{
			szPoland.Empty();
		}
		szTmp.Trim();

		for( int i=0; i<(int)vecValue.size(); i++ )
		{
			if( szTmp==vecValue[i]->GetVarName()->GetUnicode() )
			{
				if( vecValue[i]->GetMode()!=0 )
				{
					//��1.0.0.1072a	������̃_�u���N�H�[�e�[�V������t���Ȃ�
					//szTmp = "\"";
					//szTmp += vecValue[i]->GetValueString();
					//szTmp += "\"";
					szTmp = vecValue[i]->GetValueString();
					//��1.0.0.1072a
				}
				else
				{
					//��1.0.0.1072c
					//szTmp.Format(_T("%i"), (INT)vecValue[i]->GetValue());
					//INT�^��Double�^�̋��
					double dblValue = vecValue[i]->GetValue();
					if( dblValue - (double)(INT)dblValue > 0.0 )
					{
						szTmp.Format(_T("%f"), dblValue);
					}
					else
					{
						szTmp.Format(_T("%i"), (INT)dblValue);
					}
					//��1.0.0.1072c
				}
				break;
			}
		}

		szChange += szTmp;
		if( nCammaPos>=0 )
		{
			szChange+= _T(",");
		}
	}while(!szPoland.IsEmpty());
	szValue = szChange;
	return TRUE;
}
//��1.0.0.1031



//-------------------------------------------------------------------------------------
//�t�|�[�����h�����Z
//���Z�q +-*/�̂S���
//bVariable:�ϐ����͂����Ă��邩�ǂ���
//-------------------------------------------------------------------------------------
BOOL StInspectMain::Operation( CString szString, double &dblValue, BOOL &bVariable )
{

	return OperationMain( m_vecValue, szString, dblValue, bVariable );

	/*
	BOOL bReval = TRUE;
	CString szPoland("");	//�J���}��؂�
	ChangeReversePoland( szString, szPoland );

	PTCHAR szExpresssion[] = {_T("+"),_T("-"),_T("*"),_T("/")};
	std::vector <double> vdblStac; 
	bVariable = FALSE;
	do
	{
		INT nCammaPos = szPoland.Find(_T(","));
		CString szValue = szPoland;
		if( nCammaPos>=0 )
		{
			szValue = szValue.Left(nCammaPos);
			szPoland = szPoland.Mid(nCammaPos+1);
		}
		else
		{
			szPoland.Empty();
		}
		INT nExpPos = -1;
		for( int i=0; i<_countof(szExpresssion) && nExpPos<0; i++ )
		{
			if( szValue==szExpresssion[i] )
			{
				nExpPos = i;
			}
		}
		if( nExpPos>=0 )
		{
			//���Z
			if( vdblStac.size()<2 )
			{
				bReval = FALSE;
				break;
			}
			double dblA = vdblStac[vdblStac.size()-1];
			vdblStac.pop_back();
			double dblB = vdblStac[vdblStac.size()-1];
			vdblStac.pop_back();
			if( nExpPos==0 )
			{
				dblA = dblB + dblA;
			}
			else
			if( nExpPos==1 )
			{
				dblA = dblB - dblA;
			}
			else
			if( nExpPos==2 )
			{
				dblA = dblB * dblA;
			}
			else
			if( nExpPos==3 )
			{
				dblA = dblB / dblA;
			}
			vdblStac.push_back(dblA);
		}
		else
		{
			//Stac
			double dblVal;
			BOOL bGet = FALSE;
			for( int i=0; i<m_vecValue.size(); i++ )
			{
				if( szValue==m_vecValue[i]->GetVarName()->GetUnicode() )
				{
					if( m_vecValue[i]->GetMode()!=0 )
					{
						bReval = FALSE;
						break;
					}
					bGet = TRUE;
					dblVal = m_vecValue[i]->GetValue();
					bVariable = TRUE;
				}
			}
			if( bReval )
			{
				if( !bGet )
				{
					dblVal = _ttof(szValue);
				}
			}
			vdblStac.push_back(dblVal);
		}
	}while(szPoland.GetLength()>0);

	if( bReval )
	{
		if( vdblStac.size()!=1 )
		{
			bReval = FALSE;
		}
		if( bReval )
		{
			dblValue = vdblStac[0];
		}
	}
	return bReval;
	*/
}

void StInspectMain::ChangeArgument( CString &szString )
{
	double dblValue;
	BOOL bVariable = FALSE;

	//��1.0.0.1031
	OperationStValue( m_vecValue, szString, szString );
	//��1.0.0.1031
	Operation( szString, dblValue, bVariable );
	if( bVariable )
	{
		szString.Format(_T("%f"),dblValue);
	}
	//��1.0.0.1025
	else
	{
		CString szValue;
		OperationString( m_vecValue, szString, szValue, bVariable );
		szString = szValue;
	}
	//��1.0.0.1025

}
//��1.0.0.1023 �J�Ԃ��p
//��1.0.0.1025
BOOL StInspectMain::ChangeStringToOutputString(CString szSrc, CString &szDst)
{
	BOOL bReval = TRUE;
	std::vector <CString> vecSeparate;

	//szSrc��vecSeparate�ɑ��
	while(szSrc.GetLength()>0)
	{
		INT nMode = 0;
		if( szSrc.Left(1)==_T("\"") )
		{
			nMode = 1;	//������
		}
		INT nPos = szSrc.Mid(nMode).Find(_T("\""));
		if( nPos<0 )
		{
			vecSeparate.push_back(szSrc);
			break;
		}
		else
		{
			vecSeparate.push_back(szSrc.Left(nPos + nMode + nMode ));
			szSrc = szSrc.Mid(nPos + nMode + nMode );
		}
	};

	//vecSeparate��szDst�ɂȂ�ׂ�
	szDst.Empty();
	for( int i=0; i<(int)vecSeparate.size(); i++ )
	{
		CString szTmp;
		if( vecSeparate[i].Left(1)==_T("\"") ) //������
		{
			szTmp = vecSeparate[i].Mid(1);
			if( szTmp.Right(1)==_T("\"") )
			{
				szTmp = szTmp.Left(szTmp.GetLength()-1);
			}
		}
		else
		{
			//���l�A�ϐ�StValue
			double dblValue;
			BOOL bVariable;
			bReval = Operation( vecSeparate[i], dblValue, bVariable );
			szTmp = vecSeparate[i];
			if( bReval )
			{
				if( bVariable )
					szTmp.Format(_T("%f"),dblValue);
			}
			else
			{
				OperationString( m_vecValue, vecSeparate[i], szTmp, bVariable );
			}
		}
		szDst += szTmp;
	}
	return bReval;
}
//��1.0.0.1025

//-------------------------------------------------
//�N���X�̎��s Execute,Set�Ȃ�
//Example
//  aDevice.Connect
//  aSN.Execute
//-------------------------------------------------
//��1.0.0.1023
//BOOL StInspectMain::ExecuteClassFunction(CString &szString)
BOOL StInspectMain::ExecuteClassFunction(CString &szString, INT nScriptLine)
//��1.0.0.1023
{
	BOOL bReval = FALSE;

	CStGenFactoryScriptDlg *pWnd = (CStGenFactoryScriptDlg *)mpParent;

	do
	{

		szString.Trim();
		INT iPeriodPos = szString.Find(_T("."));
		if( iPeriodPos<=0 ) break;
		CString szVarName = szString.Left(iPeriodPos);
		CString szTmp = szString.Mid( iPeriodPos + 1 );
		szTmp.Trim();
		CString szSection = szTmp;
		CString szArgument("");
//if( szString==_T("ksData = aDevice.ExposureTime") )
//{
//	Sleep(1);
//}

		if( !SepareteString(szTmp, szSection, szArgument ) )
			break;


		//��1.0.0.1023 �J�Ԃ��p
		ChangeArgument(szArgument);
		//��1.0.0.1023 �J�Ԃ��p


		//INT iPos=0;
		//if( (iPos=szTmp.Find(_T("=")))>0 )	//"("���"="�D��...DeviceVersion�ŃJ�b�R�����邽��
		//{
		//	szSection = szTmp.Left(iPos);
		//	szSection.Trim();
		//	szArgument = szTmp.Mid(iPos+1);
		//	szArgument.Trim();
		//}
		//else
		//if( (iPos=szTmp.Find(_T("(")))>0 )
		//{
		//	szSection = szTmp.Left(iPos);
		//	szSection.Trim();
		//	szArgument = szTmp.Mid(iPos+1);
		//	//��납��T��...�J�b�R���ɃJ�b�R�������Ă������悤��
		//	//TCHAR *szFindCh = _T(")");
		//	//szArgument = szArgument.Left(szArgument.ReverseFind(szFindCh[0]));
		//	szArgument = szArgument.Left(szArgument.ReverseFind(')'));
		//	//szArgument = szArgument.Left(szArgument.Find(_T(")")));
		//	szArgument.Trim();
		//}

		//�N���X���擾
		CString szClassName("");
		void *pBase=NULL;
		//Device
		for( DWORD i=0; i<m_vecDeviceBase.size() && pBase==NULL; i++ )
		{
			if( szVarName == m_vecDeviceBase[i]->m_VarName.GetAscii() )
			{
				szClassName = m_vecDeviceBase[i]->m_ClassName.GetAscii();
				pBase = m_vecDeviceBase[i];
			}
		}

		//SerialComm
		//��1.0.0.1020
		//for( DWORD i=0; i<m_vecSerialComm.size() && pBase==NULL; i++ )
		//{
		//	if( szVarName == m_vecSerialComm[i]->m_VarName.GetAscii() )
		//	{
		//		szClassName = m_vecSerialComm[i]->m_ClassName.GetAscii();
		//		pBase = m_vecSerialComm[i];
		//	}
		//}
		for( DWORD i=0; i<m_vecControl.size() && pBase==NULL; i++ )
		{
			if( szVarName == m_vecControl[i]->m_VarName.GetAscii() )
			{
				szClassName = m_vecControl[i]->m_ClassName.GetAscii();
				pBase = m_vecControl[i]->GetMainHandle();
			}
		}
		//��1.0.0.1020

		//TexioControl
		for( DWORD i=0; i<m_vecTexioControl.size() && pBase==NULL; i++ )
		{
			if( szVarName == m_vecTexioControl[i]->m_VarName.GetAscii() )
			{
				szClassName = m_vecTexioControl[i]->m_ClassName.GetAscii();
				pBase = m_vecTexioControl[i];
			}
		}
		//��1.0.0.1048
		////m_vecImageMessageDlg
		//for( DWORD i=0; i<m_vecImageMessageDlg.size() && pBase==NULL; i++ )
		//{
		//	if( szVarName == m_vecImageMessageDlg[i]->m_VarName.GetAscii() )
		//	{
		//		szClassName = m_vecImageMessageDlg[i]->m_ClassName.GetAscii();
		//		pBase = m_vecImageMessageDlg[i];
		//	}
		//}
		//m_vecCustomDlg
		for( DWORD i=0; i<m_vecCustomDlg.size() && pBase==NULL; i++ )
		{
			if( szVarName == m_vecCustomDlg[i]->m_VarName.GetAscii() )
			{
				szClassName = m_vecCustomDlg[i]->m_ClassName.GetAscii();
				pBase = m_vecCustomDlg[i];
			}
		}
		//��1.0.0.1048

		//��1.0.0.1023 �J�Ԃ��p�ϐ��ǉ�
		for( DWORD i=0; i<m_vecValue.size() && pBase==NULL; i++ )
		{
			if( szVarName == m_vecValue[i]->GetVarName()->GetAscii() )
			{
				szClassName = m_vecValue[i]->GetClassName()->GetAscii();
				pBase = m_vecValue[i];
			}
		}
		//��1.0.0.1023 �J�Ԃ��p�ϐ��ǉ�


		//��1.0.0.1025 �t�@�C������ǉ�
		for( DWORD i=0; i<m_vecCommonBase.size() && bReval==FALSE; i++ )
		{
			if( szVarName==m_vecCommonBase[i]->GetVarName()->GetAscii() )
			{
				szClassName = m_vecCommonBase[i]->GetClassName()->GetAscii();
				pBase = m_vecCommonBase[i];
			}
		}
		//��1.0.0.1025 �t�@�C������ǉ�

		//Inspection
		for( DWORD i=0; i<m_vecCheckBase.size() && pBase==NULL; i++ )
		{
			if( szVarName == m_vecCheckBase[i]->m_VarName.GetAscii() )
			{
				szClassName = m_vecCheckBase[i]->m_ClassName.GetAscii();
				pBase = m_vecCheckBase[i];
			}
		}

		//Device
		if( szClassName==_StDeviceEBus )
		{
			StDeviceEBus *pStDeviceEBus = (StDeviceEBus *)pBase;
			if( szSection=="Connect" )
			{
				bReval = ConnectEBus(pStDeviceEBus, szArgument);
				//��1.0.0.1009
				if( !bReval )
				{
					szErrorMessage = _T("eBUS�ڑ��G���[");
				}
				//��1.0.0.1009
				//DeviceModelName�m��Ń��OPATH�A�t�@�C�����쐬
				CreateLogPathName();
				CreateLogFileName();
			}
			else
			if( szSection=="Disconnect" )
			{
				bReval = DisonnectEBus(pStDeviceEBus);
				//��1.0.0.1009
				if( !bReval )
				{
					szErrorMessage = _T("eBUS�ؒf�G���[");
				}
				//��1.0.0.1009
			}
			else
			if( szSection=="Start" )
			{
				bReval = StartEBus(pStDeviceEBus);
				//��1.0.0.1009
				if( !bReval )
				{
					szErrorMessage = _T("eBUS�X�^�[�g�G���[");
				}
				//��1.0.0.1009
			}
			else
			if( szSection=="Stop" )
			{
				bReval = StopEBus(pStDeviceEBus);
				//��1.0.0.1009
				if( !bReval )
				{
					szErrorMessage = _T("eBUS�X�g�b�v�G���[");
				}
				//��1.0.0.1009
			}

			else
			if( szSection=="ControlDialog" )
			{
				bReval = ControlDialogEBus(pStDeviceEBus, szArgument);
				//��1.0.0.1009
				if( !bReval )
				{
					szErrorMessage = _T("eBUSControlDialog�G���[");
				}
				//��1.0.0.1009
			}
			else
			if( szSection=="UART0" || szSection=="UART1" || szSection=="BULK0" )
			{
				bReval = SerialControlEBus( pStDeviceEBus, szSection, szArgument );
				//��1.0.0.1009
				if( !bReval )
				{
					szErrorMessage = _T("eBUSUART�ʐM�G���[");
				}
				//��1.0.0.1009
			}
			else
			{
				//��1.0.0.1060
				//GenICam
				//bReval = SetGenICamControlEBus( pStDeviceEBus, szSection, szArgument );
				////��1.0.0.1009
				//if( !bReval )
				//{
				//	szErrorMessage = _T("eBUSGenICam�ʐM�G���[");
				//}
				////��1.0.0.1009
				bReval = pStDeviceEBus->function( szSection, szArgument );
				if( !bReval )
				{
					bReval = SetGenICamControlEBus( pStDeviceEBus, szSection, szArgument );
					if( !bReval )
					{
						szErrorMessage = _T("eBUSGenICam�ʐM�G���[");
					}
				}

				//��1.0.0.1060
			}
		}
		else
		if( szClassName==_StDeviceMultiCam )
		{
			StDeviceMultiCam *pStDeviceMultiCam = (StDeviceMultiCam *)pBase;
			if( szSection=="Open" )
			{
				bReval = OpenMultiCam(pStDeviceMultiCam);
				if( !bReval )
				{
					pStDeviceMultiCam->GetLastError(szErrorMessage);
				}
				//DeviceModelName�m��Ń��OPATH�A�t�@�C�����쐬
				CreateLogPathName();
				CreateLogFileName();
			}
			else
			if( szSection=="Close" )
			{
				bReval = CloseMultiCam(pStDeviceMultiCam);
				if( !bReval )
				{
					pStDeviceMultiCam->GetLastError(szErrorMessage);
				}
			}
			else
			if( szSection=="Start" )
			{
				bReval = StartMultiCam(pStDeviceMultiCam);
				if( !bReval )
				{
					pStDeviceMultiCam->GetLastError(szErrorMessage);
				}
			}
			else
			if( szSection=="Stop" )
			{
				bReval = StopMultiCam(pStDeviceMultiCam);
				if( !bReval )
				{
					pStDeviceMultiCam->GetLastError(szErrorMessage);
				}
			}

			//��1.0.0.1015
			else
			if( !bReval && szSection=="SetSerialComm" )
			{
				//��1.0.0.1062

				////��1.0.0.1017
				////pStDeviceMultiCam->SetSerialComm(GetSerialComm(szArgument));
				////��1.0.0.1020 �N���X�p���̌��̃n���h����ۑ�
				////pStDeviceMultiCam->SetSerialComm((StSerialComm*)GetSerialComm(szArgument));
				//StSerialComm * aSerialComm = (StSerialComm *)GetSerialComm(szArgument)->GetMainHandle();
				//pStDeviceMultiCam->SetSerialComm(aSerialComm);
				////��1.0.0.1020
				////��1.0.0.1017
				//bReval = TRUE;

				bReval = FALSE;
				StControl *aControl = GetSerialComm(szArgument);
				if( aControl )
				{
					StSerialComm * aSerialComm = (StSerialComm *)aControl->GetMainHandle();
					if( aSerialComm ) bReval = TRUE;
					pStDeviceMultiCam->SetSerialComm(aSerialComm);
				}
				//��1.0.0.1062
			}
			//��1.0.0.1015

			//��1.0.0.1020
			else
			if( !bReval && szSection=="SetCheckControlRetry" )
			{
				StCheckControlRetry * aControlRetry = (StCheckControlRetry *)GetCheckBase(szArgument);
				pStDeviceMultiCam->SetControlRetry(aControlRetry);
				bReval = TRUE;
				//��1.0.0.1062
				if( !aControlRetry ) bReval = FALSE;
				//��1.0.0.1062
			}
			//��1.0.0.1020
			else
			{
				bReval = pStDeviceMultiCam->function( szSection, szArgument );
				if( !bReval )
				{
					szErrorMessage = _T("�X�N���v�g�G���[");
				}
			}

		}

		//��1.0.0.1044
		else
		if( szClassName==_StDeviceGenICam )
		{
			StDeviceGenICam *pStDeviceGenICam = (StDeviceGenICam *)pBase;
			if( szSection=="Open" )
			{
				bReval = OpenGenICam(pStDeviceGenICam,szArgument);
				if( !bReval )
				{
					pStDeviceGenICam->GetLastError(szErrorMessage);
				}
				//DeviceModelName�m��Ń��OPATH�A�t�@�C�����쐬
				CreateLogPathName();
				CreateLogFileName();
			}
			else
			if( szSection=="Close" )
			{
				bReval = CloseGenICam(pStDeviceGenICam);
				if( !bReval )
				{
					pStDeviceGenICam->GetLastError(szErrorMessage);
				}
			}
			else
			if( szSection=="Start" )
			{
				bReval = StartGenICam(pStDeviceGenICam);
				if( !bReval )
				{
					pStDeviceGenICam->GetLastError(szErrorMessage);
				}
			}
			else
			if( szSection=="Stop" )
			{
				bReval = StopGenICam(pStDeviceGenICam);
				if( !bReval )
				{
					pStDeviceGenICam->GetLastError(szErrorMessage);
				}
			}
			else
			if( szSection=="ReadExecute" )
			{
				//��1.0.0.1062
				if( m_nScriptCheckMode==0 )
				{
				//��1.0.0.1062

				BOOL bJudge=FALSE;
				bReval = pStDeviceGenICam->ReadCheck(szArgument,&bJudge);
				if( bReval )
				{
					CString szResult;
					//���O�ɕۑ�
					szResult.Format(_T("RcvData=%i"),pStDeviceGenICam->GetLastReadCheckData());
					OutputLogFile(szResult);
					if( pStDeviceGenICam->IsReadCheckData() )
					{
						CString szText = _T("Check=");
						//BOOL bJudge = pStDeviceGenICam->GetReadCheckJudge();
						bReval = bJudge;
						OutputLogFile(szText + (bJudge?_T("OK"):_T("NG")));
					}				
					pStDeviceGenICam->GetLastError(szErrorMessage);
				}
				else
				{
					OutputLogFile(_T("Check=Error"));
					pStDeviceGenICam->GetLastError(szErrorMessage);
				}

				//��1.0.0.1062
				}
				else
				{
					bReval=TRUE;
				}
				//��1.0.0.1062

			}
			else
			{
				//��1.0.0.1062
				if( m_nScriptCheckMode==0 )
				{
				//��1.0.0.1062

				bReval = pStDeviceGenICam->function( szSection, szArgument );
				if( !bReval )
				{
					pStDeviceGenICam->GetLastError(szErrorMessage);
					if( szErrorMessage.GetLength()==0 )
					{
						szErrorMessage = _T("�X�N���v�g�G���[");
					}
					//��1.0.0.1066
					else
					{
						szString.Format(_T("Error=%s"),szErrorMessage);
						OutputLogFile(szString);
					}
					//��1.0.0.1066
				}

				//��1.0.0.1062
				}
				else
				{
					bReval=TRUE;
				}
				//��1.0.0.1062

			}
		}
		//��1.0.0.1044

		//SerialComm
		else
		if( szClassName==_StSerialComm )
		{
			StSerialComm *pSerialComm = (StSerialComm *)pBase;
//TRACE(TEXT("@@@@@@@@@ pSerialComm=0x%08x\n"), pSerialComm );

			bReval = pSerialComm->function( szSection, szArgument );

			//if( szSection=="Send" )
			{
				//bReval = pSerialComm->SendData(szArgument);
				//��1.0.0.1009
				if( !bReval )
				{
					szErrorMessage = _T("����ʐM�G���[");
				}
				//��1.0.0.1009
			}
		}


		//��1.0.0.1017
		else
		if( szClassName==_StTerminal )
		{
			StTerminal *pTerminal = (StTerminal *)pBase;

			bReval = pTerminal->function( szSection, szArgument );

			//if( szSection=="Send" )
			{
				//bReval = pSerialComm->SendData(szArgument);
				//��1.0.0.1009
				if( !bReval )
				{
					szErrorMessage = _T("Terminal�ʐM�G���[");
				}
				//��1.0.0.1009
			}
		}


		//��1.0.0.1017


		//��1.0.0.1046 �J��������ǉ�
		else
		if( szClassName==_StCameraControlDlg )
		{
			StCheckBase * pCheckBase = (StCheckBase *)pBase;
			StCameraControlDlg *pStCameraControlDlg = (StCameraControlDlg *)pCheckBase->GetMainHandle();

			bReval = pStCameraControlDlg->function( szSection, szArgument);
			if( !bReval )
			{
				if( szSection=="Execute" || szSection=="DoModal"  )
				{
					//bReval = pStInputSerialDlg->Execute();
					//��1.0.0.1062
					if( m_nScriptCheckMode==0 )
					{
					//��1.0.0.1062
					if( pStCameraControlDlg->DoModal() == IDOK )
					{
					}
					//��1.0.0.1062
					}
					//��1.0.0.1062
					bReval = TRUE;
				}
				else
				if( szSection=="Open" || szSection=="open" )
				{
					//��1.0.0.1062
					if( m_nScriptCheckMode==0 )
					{
					//��1.0.0.1062
					if( !pStCameraControlDlg->m_hWnd )
					{
						pStCameraControlDlg->Create( IDD_DIALOG_CAMERACONTROL, NULL );
						RECT rc1;
						pStCameraControlDlg->GetWindowRect(&rc1);
						//pStCameraControlDlg->GetClientRect(&rc2);
						INT nPosX = pStCameraControlDlg->GetWindowPosX();
						if( nPosX<0 )	nPosX = rc1.left;
						INT nPosY = pStCameraControlDlg->GetWindowPosY();
						if( nPosY<0 )	nPosY = rc1.top;
						pStCameraControlDlg->SetWindowPos( NULL, nPosX, nPosY, rc1.right-rc1.left, rc1.bottom-rc1.top, SWP_NOZORDER );
					}
					bReval = pStCameraControlDlg->ShowWindow(SW_SHOW);
					//DoEvents�K���K�v�B�Ȃ��ƃ��b�Z�[�W���\������Ȃ��B�B
					DoEvents();
					//��1.0.0.1062
					}
					//��1.0.0.1062

					bReval = TRUE;
				}
				else
				if( szSection=="Close" || szSection=="close" )
				{
					//��1.0.0.1062
					if( m_nScriptCheckMode==0 )
					{
					//��1.0.0.1062
					if( pStCameraControlDlg->m_hWnd )
					{
						bReval = pStCameraControlDlg->ShowWindow(SW_HIDE);
						//bReval = TRUE;
					}
					//��1.0.0.1062
					}else bReval = TRUE;
					//��1.0.0.1062
				}
				else
				{
					szErrorMessage = _T("�X�N���v�g�G���[");
				}
			}
		}
		//��1.0.0.1046 �J��������ǉ�

		//��1.0.0.1047
		else
		if( szClassName==_StCheckSwitchDlg )
		{
			StCheckBase * pCheckBase = (StCheckBase *)pBase;
			StCheckSwitchDlg *pStCheckSwitchDlg = (StCheckSwitchDlg *)pCheckBase->GetMainHandle();

			bReval = pStCheckSwitchDlg->function( szSection, szArgument );
			if( !bReval )
			{
				if( szSection=="Execute" || szSection=="DoModal"  )
				{
					if( pStCheckSwitchDlg->DoModal() == IDOK )
					{
						bReval = TRUE;
					}

					//���O�ɕۑ�
					CString szText = _T("Check=");
					OutputLogFile(szText + (bReval?_T("OK"):_T("NG")));

					if( !bReval )
					{
						szErrorMessage = _T("Switch����NG");
						bReval = FALSE;
					}

				}
				else
				{
					szErrorMessage = _T("�X�N���v�g�G���[");
				}
			}
		}
		//��1.0.0.1047
		//��1.0.0.1048
		else
		if( szClassName==_StCheckAdjustVolumeDlg )
		{
			StCheckBase * pCheckBase = (StCheckBase *)pBase;
			StCheckAdjustVolumeDlg *pStCheckAdjustVolumeDlg = (StCheckAdjustVolumeDlg *)pCheckBase->GetMainHandle();

			bReval = pStCheckAdjustVolumeDlg->function( szSection, szArgument );
			if( !bReval )
			{
				if( szSection=="Execute" || szSection=="DoModal"  )
				{
					if( pStCheckAdjustVolumeDlg->DoModal() == IDOK )
					{
						INT nJudge = pStCheckAdjustVolumeDlg->GetJudge();
						bReval = nJudge==0?FALSE:TRUE;

					}

					//���O�ɕۑ�
					CString szText = _T("Check=");
					OutputLogFile(szText + (bReval?_T("OK"):_T("NG")));












					if( !bReval )
					{
						szErrorMessage = _T("�{�����[������NG");
						bReval = FALSE;
					}
				}
				else
				{
					szErrorMessage = _T("�X�N���v�g�G���[");
				}
			}
		}
		//��1.0.0.1048

		//TexioControl-----------------------------------------------------
		else
		if( szClassName==_StTexioControl )
		{
			StTexioControl *pTexioControl = (StTexioControl *)pBase;
			bReval = pTexioControl->function( szSection, szArgument );
			//��1.0.0.1009
			if( !bReval )
			{
				szErrorMessage = _T("Texio�ʐM�G���[");
			}
			//��1.0.0.1009
		}

		//StImageMessageDlg
		else
		if( szClassName==_StImageMessageDlg )
		{
			StImageMessageDlg *pImageMessageDlg = (StImageMessageDlg *)pBase;
			bReval = pImageMessageDlg->function( szSection, szArgument );
			if( !bReval )
			{
				//��1.0.0.1062
				if(m_nScriptCheckMode==0)
				{
				//��1.0.0.1062
				if( szSection=="open" )
				{
					if( !pImageMessageDlg->m_hWnd )
					{
						pImageMessageDlg->Create( IDD_DIALOG_IMAGEMESSAGE, NULL );
						INT nSizeX = pImageMessageDlg->GetImageSizeX() + pImageMessageDlg->GetMessageSizeX() + 30;
						RECT rc1, rc2;
						pImageMessageDlg->GetWindowRect(&rc1);
						pImageMessageDlg->GetClientRect(&rc2);
						INT nMenuY = (rc1.bottom-rc1.top) - (rc2.bottom-rc2.top);
						
						INT nSizeY = max( pImageMessageDlg->GetImageSizeY(), pImageMessageDlg->GetMessageSizeY() ) + 30 + nMenuY;

						pImageMessageDlg->SetWindowPos( NULL, pImageMessageDlg->GetWindowPosX(), pImageMessageDlg->GetWindowPosY(), nSizeX, nSizeY, SWP_NOZORDER );
					}
					bReval = pImageMessageDlg->ShowWindow(SW_SHOW);
					//DoEvents�K���K�v�B�Ȃ��ƃ��b�Z�[�W���\������Ȃ��B�B
					DoEvents();
				}
				else
				if( szSection=="close" )
				{
					bReval = pImageMessageDlg->ShowWindow(SW_HIDE);
				}
				//��1.0.0.1062
				}
				//��1.0.0.1062
				bReval = TRUE;
			}
		}

		//��1.0.0.1048
		//StHttpControlDlg-----------------------------------------------------
		else
		if( szClassName==_StHttpControlDlg )
		{
			StHttpControlDlg *pHttpControlDlg = (StHttpControlDlg *)pBase;
			bReval = pHttpControlDlg->function( szSection, szArgument );
			if( !bReval )
			{
				if( szSection=="Execute" || szSection=="DoModal"  )
				{
					StString szAddress;
					pHttpControlDlg->GetAddress(szAddress);
					CString CAddress = szAddress.GetUnicode();
					if( m_pStructAddress )
						m_pStructAddress->GetPortName(szAddress.GetUnicode(),CAddress);
					pHttpControlDlg->function( _T("SetAddress"), CAddress );
					if( pHttpControlDlg->DoModal() == IDOK )
					{
					}
					bReval = TRUE;
				}
			}
		}
		//��1.0.0.1048

		//Inspection
		else
		if( szClassName==_StCheckSN )
		{

			StCheckSN *pStCheckSN = (StCheckSN *)pBase;
			bReval = pStCheckSN->function( szSection, szArgument );

			if( !bReval && szSection=="Execute" )
			{

				//��1.0.0.1062
				if( m_nScriptCheckMode==0 )
				{
				//��1.0.0.1062

				bReval = pStCheckSN->Execute();
				//�������I����Ă�����bReval=TRUE�ŏI��---------------
				//�����GetJudge���擾
				if( bReval )
				{
					//���O�ɕۑ�
					CString szText = _T("Check=");
					INT aJudge = pStCheckSN->GetJudge();
					OutputLogFile(szText + (aJudge==1?_T("OK"):_T("NG")));
					INT aElement = pStCheckSN->GetElement();
					//LPCTSTR aColor[] = { _T("R:"), _T("G:"), _T("B:") };
					LPCTSTR szColor[] = {_T("Red"),_T("Green"),_T("Blue")};
					for( int i=0; i<aElement; i++ )
					{
//if( aElement==3 )
//Sleep(1);

						INT nColorIndex = pStCheckSN->GetElementColorIndex(i);
						CString aColor(_T(""));
						if( aElement==3 )
						{
							aColor = _T("_");
							aColor += szColor[nColorIndex];
						}
						szText.Format(_T("Average%s=%f"), aColor, pStCheckSN->GetAverage(i));
						//if( aElement==3 )	szText = aColor[nColorIndex] + szText;
						OutputLogFile( szText );
						pWnd->DisplayListBox(szText);
						szText.Format(_T("SN%s=%f"), aColor, pStCheckSN->GetSN(i));
						//��1.0.0.1043
						//if( aElement==3 )	szText = aColor[nColorIndex] + szText;
						//��1.0.0.1043
						OutputLogFile( szText );
						pWnd->DisplayListBox(szText);
						szText.Format(_T("SNMax%s=%f"), aColor, pStCheckSN->GetSNMax(i));
						//��1.0.0.1043
						//if( aElement==3 )	szText = aColor[nColorIndex] + szText;
						//��1.0.0.1043
						OutputLogFile( szText );
						pWnd->DisplayListBox(szText);
						szText.Format(_T("SDMPix%s=%i"), aColor, pStCheckSN->GetSDMPix(i));
						//��1.0.0.1043
						//if( aElement==3 )	szText = aColor[nColorIndex] + szText;
						//��1.0.0.1043
						OutputLogFile( szText );
						pWnd->DisplayListBox(szText);
						//��1.0.0.1028
						szText.Format(_T("LineAverageMin%s=%f"), aColor, pStCheckSN->GetLineAverageMin(i));
						//��1.0.0.1043
						//if( aElement==3 )	szText = aColor[nColorIndex] + szText;
						//��1.0.0.1043
						OutputLogFile( szText );
						pWnd->DisplayListBox(szText);
						szText.Format(_T("LineAverageMax%s=%f"), aColor, pStCheckSN->GetLineAverageMax(i));
						//��1.0.0.1043
						//if( aElement==3 )	szText = aColor[nColorIndex] + szText;
						//��1.0.0.1043
						OutputLogFile( szText );
						pWnd->DisplayListBox(szText);
						//��1.0.0.1028
					}
					if( aJudge!=1 )
					{
						szErrorMessage = _T("SN����NG");
						bReval = FALSE;
					}
				}
				else
				{
					szErrorMessage = _T("SN�����G���[");
				}

				//��1.0.0.1062
				}else bReval = TRUE;
				//��1.0.0.1062

			}
			else
			if( !bReval )
			{
				szErrorMessage = _T("�X�N���v�g�G���[");
			}
		}

		else
		if( szClassName==_StCheckControlRetry )
		{
			StCheckControlRetry *pStCheckControlRetry = (StCheckControlRetry *)pBase;
			bReval = pStCheckControlRetry->function(szSection,szArgument);
			if( !bReval && szSection=="Execute" )
			{

				//��1.0.0.1062
				if( m_nScriptCheckMode==0 )
				{
				//��1.0.0.1062


				bReval = pStCheckControlRetry->Execute();

				//���O��Send Return�ۑ�
				CString szSendData;
				pStCheckControlRetry->GetSendData(szSendData);
				szSendData = CString(_T("SendData=")) + szSendData;
				OutputLogFile(szSendData);
				CString szResult;
				pStCheckControlRetry->GetResult(szResult);
				szResult = CString(_T("RcvData=")) + szResult;
				OutputLogFile(szResult);
//{
//TCHAR szText[128];
//_stprintf_s(szText,_countof(szText),_T(" Execute=%d SendData=%s RcvData=%s \n"), bReval,szSendData,szResult);
//OutputDebugString(szText);
//}

				if( pStCheckControlRetry->GetRetry()>0 )
				{
					pStCheckControlRetry->GetBusyResult(szResult);
					szResult = CString(_T("BusyRcvData=")) + szResult;
					OutputLogFile(szResult);
				}

				if( bReval )
				{
					//���O�ɕۑ�
					CString szText = _T("Check=");
					INT aJudge = pStCheckControlRetry->GetJudge();
//{
//TCHAR szText[128];
//_stprintf_s(szText,_countof(szText),_T(" aJudge=%d \n"), aJudge);
//OutputDebugString(szText);
//}
					if( aJudge!=1)
					{
						OutputLogFile(szText + (aJudge==1?_T("OK"):_T("NG")));
					}
					if( aJudge!=1 )
					{
						szErrorMessage = _T("�J�����ʐM�J�Ԍ���NG");
						bReval = FALSE;
					}
				}
				else
				{
					OutputLogFile(_T("Check=ControlError"));
					szErrorMessage = _T("�J�����ʐM�J�Ԍ����G���[");
				}

				//��1.0.0.1062
				}else bReval = TRUE;
				//��1.0.0.1062

			}
			else
			if( !bReval )
			{
				szErrorMessage = _T("�X�N���v�g�G���[");
			}
		}

		else
		if( szClassName==_StInputSerialDlg )
		{
			StCheckBase * pCheckBase = (StCheckBase *)pBase;
			StInputSerialDlg *pStInputSerialDlg = (StInputSerialDlg *)pCheckBase->GetMainHandle();

			//��1.0.0.1031
			//if( szSection=="Execute" )
			bReval = pStInputSerialDlg->function( szSection, szArgument);
			if( !bReval && szSection=="Execute" )
			//��1.0.0.1031
			{

				//��1.0.0.1062
				if( m_nScriptCheckMode==0 )
				{
				//��1.0.0.1062

				//bReval = pStInputSerialDlg->Execute();
				if( pStInputSerialDlg->DoModal() == IDOK )
				{
					mSerialNumber = pStInputSerialDlg->GetSerialNumber();
					//��1.0.0.1015
					mHinmokuCode = pStInputSerialDlg->GetHinmokuCode();
					//��1.0.0.1015
					bReval = TRUE;
					//DeviceModelName�m��Ńt�@�C�����쐬
					CreateLogFileName();
					//���O�ɕۑ�
					CString szText = _T("SerialNumber=") + mSerialNumber;
					OutputLogFile(szText);
					szText.Format(_T("WorkerName=%s"),pStInputSerialDlg->GetWorkerName());
					OutputLogFile(szText);

					//��1.0.0.1031
					bReval = pStInputSerialDlg->GetCheck();
					if( !bReval )
					{
						szErrorMessage = _T("��V���A���i���o�[�`�F�b�NNG");
					}
					//��1.0.0.1031
				}
				//��1.0.0.1009
				else
				{
					szErrorMessage = _T("�V���A�����̓G���[");
				}
				//��1.0.0.1009

				//��1.0.0.1062
				}else bReval = TRUE;
				//��1.0.0.1062

			}
			else
			if( !bReval )
			{
				szErrorMessage = _T("�X�N���v�g�G���[");
			}
		}
		else
		if( szClassName==_StCheckSNDlg )
		{
			StCheckBase * pCheckBase = (StCheckBase *)pBase;
			StCheckSNDlg *pStCheckSNDlg = (StCheckSNDlg *)pCheckBase->GetMainHandle();
			bReval = pStCheckSNDlg->function( szSection, szArgument);

			if( !bReval && szSection=="Execute" )
			{

				//��1.0.0.1062
				if( m_nScriptCheckMode==0 )
				{
				//��1.0.0.1062


				//��1.0.0.1056
				pStCheckSNDlg->SetSaveFilePath(mSaveFilePath);
				//��1.0.0.1056
				if( pStCheckSNDlg->DoModal() == IDOK )
				{
					bReval = TRUE;
					//���O�ɕۑ�
					CString szText = _T("Check=");
					INT aJudge = pStCheckSNDlg->GetJudge();
					OutputLogFile(szText + (aJudge==1?_T("OK"):_T("NG")));
					INT aElement = pStCheckSNDlg->GetElement();
					LPCTSTR aColor[] = { _T("R:"), _T("G:"), _T("B:") };
					for( int i=0; i<aElement; i++ )
					{
						INT nColorIndex = pStCheckSNDlg->GetElementColorIndex(i);
						szText.Format(_T("Average=%f"), pStCheckSNDlg->GetAverage(i));
						if( aElement==3 )	szText = aColor[nColorIndex] + szText;
						OutputLogFile( szText );
						pWnd->DisplayListBox(szText);
						szText.Format(_T("SN=%f"), pStCheckSNDlg->GetSN(i));
						if( aElement==3 )	szText = aColor[nColorIndex] + szText;
						OutputLogFile( szText );
						pWnd->DisplayListBox(szText);
						szText.Format(_T("SNMax=%f"), pStCheckSNDlg->GetSNMax(i));
						if( aElement==3 )	szText = aColor[nColorIndex] + szText;
						OutputLogFile( szText );
						pWnd->DisplayListBox(szText);
						szText.Format(_T("SDMPix=%i"), pStCheckSNDlg->GetSDMPix(i));
						if( aElement==3 )	szText = aColor[nColorIndex] + szText;
						OutputLogFile( szText );
						pWnd->DisplayListBox(szText);
					}
					if( aJudge!=1 )
					{
						szErrorMessage = _T("SN����NG");
						bReval = FALSE;
					}
				}
				//��1.0.0.1009
				else
				{
					szErrorMessage = _T("SN����Dlg�G���[");
				}
				//��1.0.0.1009


				//��1.0.0.1062
				}else bReval = TRUE;
				//��1.0.0.1062

			}
			else
			if( !bReval )
			{
				szErrorMessage = _T("�X�N���v�g�G���[");
			}
		}
		else
		if( szClassName==_StCheckSpBehavior )
		{
			StCheckSpBehavior *pStCheckSpBehavior = (StCheckSpBehavior *)pBase;
			bReval = pStCheckSpBehavior->function(szSection,szArgument);
			if( !bReval && szSection=="Execute" )
			{
				//��1.0.0.1062
				if( m_nScriptCheckMode==0 )
				{
				//��1.0.0.1062

				bReval = pStCheckSpBehavior->Execute();
				//�������I����Ă�����bReval=TRUE�ŏI��---------------
				//�����GetJudge���擾
				if( bReval )
				{
					//���O�ɕۑ�
					CString szText = _T("Check=");
					INT aJudge = pStCheckSpBehavior->GetJudge();
					OutputLogFile(szText + (aJudge==1?_T("OK"):_T("NG")));

					szText.Format(_T("ContinuousCount=%i"), pStCheckSpBehavior->GetContinuous());
					OutputLogFile( szText );
					if( pStCheckSpBehavior->GetContinuous()>0 )
					{
						szText.Format(_T("Position=%i"), pStCheckSpBehavior->GetLine());
						OutputLogFile( szText );
						pWnd->DisplayListBox(szText);
						//��1.0.0.1044
						szText.Format(_T("LineAverage=%f"), pStCheckSpBehavior->GetLineAverage());
						OutputLogFile( szText );
						pWnd->DisplayListBox(szText);
						//��1.0.0.1044
					}
					LPCTSTR szColor[] = {_T("Red"),_T("Green"),_T("Blue")};
					INT iResultColor = pStCheckSpBehavior->GetColor();
					if( iResultColor>=0 && iResultColor<_countof(szColor) )
					{
						szText.Format(_T("Color=%s"), szColor[iResultColor]);
						OutputLogFile( szText );
						pWnd->DisplayListBox(szText);
					}
					if( aJudge!=1 )
					{
						szErrorMessage = _T("���苓������NG");
						bReval = FALSE;
					}
				}
				else
				{
					szErrorMessage = _T("���苓�������G���[");
				}

				//��1.0.0.1062
				}else bReval = TRUE;
				//��1.0.0.1062

			}
			else
			if( !bReval )
			{
				szErrorMessage = _T("�X�N���v�g�G���[");
			}
		}
		else
		if( szClassName==_StCheckDataLose )
		{
			StCheckDataLose *pStCheckDataLose = (StCheckDataLose *)pBase;

			bReval = pStCheckDataLose->function(szSection,szArgument);
			if( !bReval && szSection=="Execute" )
			{
				//��1.0.0.1062
				if( m_nScriptCheckMode==0 )
				{
				//��1.0.0.1062


				pStCheckDataLose->SetSaveFilePath(mSaveFilePath);

				bReval = pStCheckDataLose->Execute();
				//�������I����Ă�����bReval=TRUE�ŏI��---------------
				//�����GetJudge���擾
				if( bReval )
				{
					//���O�ɕۑ�
					CString szText = _T("Check=");
					INT aJudge = pStCheckDataLose->GetJudge();
					OutputLogFile(szText + (aJudge==1?_T("OK"):_T("NG")));

					INT nElement = pStCheckDataLose->GetResultElement();
					if( nElement==1 )
					{
						INT nHighLow = pStCheckDataLose->GetResultHighLow(0);
						szText.Format(_T("Difference=%f"), pStCheckDataLose->GetResultDifference(0) );
						OutputLogFile( szText );
						pWnd->DisplayListBox(szText);
						szText.Format(_T("Direction=%s"), nHighLow!=0?_T("High"):_T("Low"));
						OutputLogFile( szText );
						pWnd->DisplayListBox(szText);
						szText.Format(_T("Line=%d"), pStCheckDataLose->GetResultLine(0));
						OutputLogFile( szText );
						pWnd->DisplayListBox(szText);
					}
					else
					if( nElement==3 )
					{
						LPCTSTR szColor[] = {_T("Red"),_T("Green"),_T("Blue")};
						for( int i=0; i<nElement; i++ )
						{
							INT nColorIndex = pStCheckDataLose->GetElementColorIndex(i);
							INT nHighLow = pStCheckDataLose->GetResultHighLow(i);
							szText.Format(_T("Difference_%s=%f"), szColor[nColorIndex], pStCheckDataLose->GetResultDifference(i));
							OutputLogFile( szText );
							pWnd->DisplayListBox(szText);
							szText.Format(_T("Direction_%s=%s"), szColor[nColorIndex], nHighLow!=0?_T("High"):_T("Low"));
							OutputLogFile( szText );
							pWnd->DisplayListBox(szText);
							szText.Format(_T("Line_%s=%d"), szColor[nColorIndex], pStCheckDataLose->GetResultLine(i));
							OutputLogFile( szText );
							pWnd->DisplayListBox(szText);
						}
					}


					//szText.Format(_T("ContinuousCount=%i"), pStCheckSpBehavior->GetContinuous());
					//OutputLogFile( szText );
					//if( pStCheckSpBehavior->GetContinuous()>0 )
					//{
					//	szText.Format(_T("Position=%i"), pStCheckSpBehavior->GetLine());
					//	OutputLogFile( szText );
					//}
					//LPCTSTR szColor[] = {_T("Red"),_T("Green"),_T("Blue")};
					//INT iColor = pStCheckSpBehavior->GetColor();
					//if( iColor>=0 && iColor<_countof(szColor) )
					//{
					//	szText.Format(_T("Color=%i"), szColor[iColor]);
					//	OutputLogFile( szText );
					//}
					if( aJudge!=1 )
					{
						szErrorMessage = _T("�f�W�^���f�[�^��肱�ڂ�����NG");
						bReval = FALSE;
					}
				}
				else
				{
					szErrorMessage = _T("�f�W�^���f�[�^��肱�ڂ������G���[");
				}

				//��1.0.0.1062
				}else bReval = TRUE;
				//��1.0.0.1062

			}
			else
			if( !bReval )
			{
				szErrorMessage = _T("�X�N���v�g�G���[");
			}
		}


		//��1.0.0.1062
		else
		if( szClassName==_StCheckDataLoseDlg )
		{
			StCheckBase * pCheckBase = (StCheckBase *)pBase;
			StCheckDataLoseDlg *pStCheckDataLoseDlg = (StCheckDataLoseDlg *)pCheckBase->GetMainHandle();
			bReval = pStCheckDataLoseDlg->function( szSection, szArgument);

			if( !bReval && szSection=="Execute" )
			{
				if( m_nScriptCheckMode==0 )
				{
					pStCheckDataLoseDlg->SetSaveFilePath(mSaveFilePath);
					if( pStCheckDataLoseDlg->DoModal() == IDOK )
					{
						bReval = TRUE;
						//���O�ɕۑ�
						CString szText = _T("Check=");
						INT aJudge = pStCheckDataLoseDlg->GetJudge();
						OutputLogFile(szText + (aJudge==1?_T("OK"):_T("NG")));

						INT nElement = pStCheckDataLoseDlg->GetResultElement();
						if( nElement==1 )
						{
							INT nHighLow = pStCheckDataLoseDlg->GetResultHighLow(0);
							szText.Format(_T("Difference=%f"), pStCheckDataLoseDlg->GetResultDifference(0) );
							OutputLogFile( szText );
							pWnd->DisplayListBox(szText);
							szText.Format(_T("Direction=%s"), nHighLow!=0?_T("High"):_T("Low"));
							OutputLogFile( szText );
							pWnd->DisplayListBox(szText);
							szText.Format(_T("Line=%d"), pStCheckDataLoseDlg->GetResultLine(0));
							OutputLogFile( szText );
							pWnd->DisplayListBox(szText);
						}
						else
						if( nElement==3 )
						{
							LPCTSTR szColor[] = {_T("Red"),_T("Green"),_T("Blue")};
							for( int i=0; i<nElement; i++ )
							{
								INT nColorIndex = pStCheckDataLoseDlg->GetElementColorIndex(i);
								INT nHighLow = pStCheckDataLoseDlg->GetResultHighLow(i);
								szText.Format(_T("Difference_%s=%f"), szColor[nColorIndex], pStCheckDataLoseDlg->GetResultDifference(i));
								OutputLogFile( szText );
								pWnd->DisplayListBox(szText);
								szText.Format(_T("Direction_%s=%s"), szColor[nColorIndex], nHighLow!=0?_T("High"):_T("Low"));
								OutputLogFile( szText );
								pWnd->DisplayListBox(szText);
								szText.Format(_T("Line_%s=%d"), szColor[nColorIndex], pStCheckDataLoseDlg->GetResultLine(i));
								OutputLogFile( szText );
								pWnd->DisplayListBox(szText);
							}
						}

						if( aJudge!=1 )
						{
							szErrorMessage = _T("�f�W�^���f�[�^��肱�ڂ�Dlg����NG");
							bReval = FALSE;
						}
					}
					else
					{
						szErrorMessage = _T("�f�W�^���f�[�^��肱�ڂ�Dlg�����G���[");
					}
				}else bReval = TRUE;
			}
			else
			if( !bReval )
			{
				szErrorMessage = _T("�X�N���v�g�G���[");
			}
		}
		//��1.0.0.1062

		else
		if( szClassName==_StCheckPixelDefect )
		{

			StCheckPixelDefect *pStCheckPixelDefect = (StCheckPixelDefect *)pBase;
			bReval = pStCheckPixelDefect->function(szSection,szArgument);
			if( !bReval && szSection=="Execute" )
			{
				//��1.0.0.1062
				if( m_nScriptCheckMode==0 )
				{
				//��1.0.0.1062

				bReval = pStCheckPixelDefect->Execute();
				//�������I����Ă�����bReval=TRUE�ŏI��---------------
				//�����GetJudge���擾
				if( bReval )
				{
					//���O�ɕۑ�
					CString szText = _T("Check=");
					INT aJudge = pStCheckPixelDefect->GetJudge();
					OutputLogFile(szText + (aJudge==1?_T("OK"):_T("NG")));

					INT nElement = pStCheckPixelDefect->GetResultElement();
					if( nElement==1 )
					{
						szText.Format(_T("Difference=%f"), pStCheckPixelDefect->GetResultDifference(0) );
						OutputLogFile( szText );
						pWnd->DisplayListBox(szText);
						szText.Format(_T("Line=%d"), pStCheckPixelDefect->GetResultLine(0));
						OutputLogFile( szText );
						pWnd->DisplayListBox(szText);
					}
					else
					if( nElement==3 )
					{
						LPCTSTR szColor[] = {_T("Red"),_T("Green"),_T("Blue")};
						for( int i=0; i<nElement; i++ )
						{
							INT nColorIndex = pStCheckPixelDefect->GetElementColorIndex(i);
							szText.Format(_T("Difference_%s=%f"), szColor[nColorIndex], pStCheckPixelDefect->GetResultDifference(i));
							OutputLogFile( szText );
							pWnd->DisplayListBox(szText);
							szText.Format(_T("Line_s=%d"), szColor[nColorIndex], pStCheckPixelDefect->GetResultLine(i));
							OutputLogFile( szText );
							pWnd->DisplayListBox(szText);
						}
					}


					if( aJudge!=1 )
					{
						szErrorMessage = _T("��f���׌���NG");
						bReval = FALSE;
					}
				}
				else
				{
					szErrorMessage = _T("��f���׌����G���[");
				}

				//��1.0.0.1062
				}else bReval = TRUE;
				//��1.0.0.1062

			}
			else
			if( !bReval )
			{
				szErrorMessage = _T("�X�N���v�g�G���[");
			}
		}


		//��1.0.0.1020
		else
		if( szClassName==_StCheckPixelCorrect )
		{
			StCheckPixelCorrect *pStCheckPixelCorrect = (StCheckPixelCorrect *)pBase;
			bReval = pStCheckPixelCorrect->function(szSection,szArgument);
			if( !bReval && szSection=="Execute" )
			{

				//��1.0.0.1062
				if( m_nScriptCheckMode==0 )
				{
				//��1.0.0.1062

				//SerialNumber�̓o�^
				bReval = pStCheckPixelCorrect->Execute(mSensorSerialNumber);
				//�������I����Ă�����bReval=TRUE�ŏI��---------------
				//�����GetJudge���擾
				if( bReval )
				{
					//���O�ɕۑ�
					CString szText = _T("Check=");
					INT aJudge = pStCheckPixelCorrect->GetJudge();
					OutputLogFile(szText + (aJudge==1?_T("OK"):_T("NG")));
					if( aJudge!=1 )
					{
						szErrorMessage = _T("��f���דo�^NG");
						bReval = FALSE;
					}
				}
				else
				{
					szErrorMessage = _T("��f���דo�^�G���[");
				}

				//��1.0.0.1062
				}else bReval = TRUE;
				//��1.0.0.1062

			}
			else
			if( !bReval )
			{
				szErrorMessage = _T("�X�N���v�g�G���[");
			}
		}
		//��1.0.0.1020

		//��1.0.0.1021
		else
		if( szClassName==_StCheckTemperature )
		{
			StCheckTemperature *pStCheckTemperature = (StCheckTemperature *)pBase;
			bReval = pStCheckTemperature->function(szSection,szArgument);
			if( !bReval && szSection=="Execute" )
			{
				//��1.0.0.1062
				if( m_nScriptCheckMode==0 )
				{
				//��1.0.0.1062

				bReval = pStCheckTemperature->Execute();
				//�������I����Ă�����bReval=TRUE�ŏI��---------------
				//�����GetJudge���擾
				if( bReval )
				{
					//���O�ɕۑ�
					CString szText = _T("Check=");
					INT aJudge = pStCheckTemperature->GetJudge();
					OutputLogFile(szText + (aJudge==1?_T("OK"):_T("NG")));

					szText.Format(_T("Temperature=%i"), pStCheckTemperature->GetResultTemperature() );
					OutputLogFile( szText );
					pWnd->DisplayListBox(szText);

					if( aJudge!=1 )
					{
						szErrorMessage = _T("���x����NG");
						bReval = FALSE;
					}
				}
				else
				{
					szErrorMessage = _T("���x�����G���[");
				}

				//��1.0.0.1062
				}else bReval = TRUE;
				//��1.0.0.1062

			}
			else
			if( !bReval )
			{
				szErrorMessage = _T("�X�N���v�g�G���[");
			}
		}
		else
		if( szClassName==_StCheckTestPattern )
		{
			StCheckTestPattern *pStCheckTestPattern = (StCheckTestPattern *)pBase;
			bReval = pStCheckTestPattern->function(szSection,szArgument);
			if( !bReval && szSection=="Execute" )
			{
				//��1.0.0.1062
				if( m_nScriptCheckMode==0 )
				{
				//��1.0.0.1062

				bReval = pStCheckTestPattern->Execute();
				//�������I����Ă�����bReval=TRUE�ŏI��---------------
				//�����GetJudge���擾
				if( bReval )
				{
					//���O�ɕۑ�
					CString szText = _T("Check=");
					INT aJudge = pStCheckTestPattern->GetJudge();
					OutputLogFile(szText + (aJudge==1?_T("OK"):_T("NG")));
					if( aJudge!=1 )
					{

						szText.Format(_T("PosX=%i"), pStCheckTestPattern->GetX());
						OutputLogFile(szText);
						pWnd->DisplayListBox(szText);
						szText.Format(_T("PosY=%i"), pStCheckTestPattern->GetY());
						OutputLogFile(szText);
						pWnd->DisplayListBox(szText);

						szErrorMessage = _T("�e�X�g�p�^�[������NG");
						bReval = FALSE;
					}
				}
				else
				{
					szErrorMessage = _T("�e�X�g�p�^�[�������G���[");
				}

				//��1.0.0.1062
				}else bReval = TRUE;
				//��1.0.0.1062

			}
			else
			if( !bReval )
			{
				szErrorMessage = _T("�X�N���v�g�G���[");
			}
		}
		//��1.0.0.1021
		//��1.0.0.1022
		else
		if( szClassName==_StCheckEllitoLVDSDlg )
		{
			StCheckBase * pCheckBase = (StCheckBase *)pBase;
			StCheckEllitoLVDSDlg *pStCheckEllitoLVDSDlg = (StCheckEllitoLVDSDlg *)pCheckBase->GetMainHandle();
			bReval = pStCheckEllitoLVDSDlg->function( szSection, szArgument);

			if( !bReval && szSection=="Execute" )
			{

				//��1.0.0.1062
				if( m_nScriptCheckMode==0 )
				{
				//��1.0.0.1062

				pStCheckEllitoLVDSDlg->SetSaveFilePath(mSaveFilePath);

				if( pStCheckEllitoLVDSDlg->DoModal() == IDOK )
				{
					bReval = TRUE;
				}

				//��1.0.0.1062
				}else bReval = TRUE;
				//��1.0.0.1062

			}
			else
			if( !bReval )
			{
				szErrorMessage = _T("�X�N���v�g�G���[");
			}
		}
		//��1.0.0.1022
		
		//��1.0.0.1059
		else
		if( szClassName==_StCheckEllitoLVDS )
		{
			StCheckEllitoLVDS *pStCheckEllitoLVDS = (StCheckEllitoLVDS *)pBase;
			bReval = pStCheckEllitoLVDS->function(szSection,szArgument);
			if( !bReval && szSection=="Execute" )
			{
				//��1.0.0.1062
				if( m_nScriptCheckMode==0 )
				{
				//��1.0.0.1062


				bReval = pStCheckEllitoLVDS->Execute();
				//�������I����Ă�����bReval=TRUE�ŏI��---------------
				//�����GetJudge���擾
				if( bReval )
				{
					//���O�ɕۑ�
					CString szText = _T("Check=");
					INT aJudge = pStCheckEllitoLVDS->GetJudge();
					OutputLogFile(szText + (aJudge==1?_T("OK"):_T("NG")));

					//szText.Format(_T("Framerate=%f"), pStCheckFramerate->GetFramerate() );
					INT nIndex=0;
					BOOL bResult;
					do
					{
						szText.Format(_T("Data%02i="),nIndex);
						TCHAR szResult[256];
						bResult = pStCheckEllitoLVDS->GetResultText(nIndex++,szResult,_countof(szResult) );
						if( bResult )
						{
							OutputLogFile( szText + szResult );
							pWnd->DisplayListBox( szText + szResult);
						}
					}while(bResult);
					if( aJudge!=1 )
					{
						szErrorMessage = _T("LVDS����NG");
						bReval = FALSE;
					}
				}
				else
				{
					szErrorMessage = _T("LVDS�����G���[");
				}

				//��1.0.0.1062
				}else bReval = TRUE;
				//��1.0.0.1062

			}
			else
			if( !bReval )
			{
				szErrorMessage = _T("�X�N���v�g�G���[");
			}
		}
		//��1.0.0.1059
		
		
		//��1.0.0.1033
		else
		if( szClassName==_StCheckFramerate )
		{
			StCheckFramerate *pStCheckFramerate = (StCheckFramerate *)pBase;
			bReval = pStCheckFramerate->function(szSection,szArgument);
			if( !bReval && szSection=="Execute" )
			{
				//��1.0.0.1062
				if( m_nScriptCheckMode==0 )
				{
				//��1.0.0.1062

				bReval = pStCheckFramerate->Execute();
				//�������I����Ă�����bReval=TRUE�ŏI��---------------
				//�����GetJudge���擾
				if( bReval )
				{
					//���O�ɕۑ�
					CString szText = _T("Check=");
					INT aJudge = pStCheckFramerate->GetJudge();
					OutputLogFile(szText + (aJudge==1?_T("OK"):_T("NG")));

					szText.Format(_T("Framerate=%f"), pStCheckFramerate->GetFramerate() );
					OutputLogFile( szText );
					pWnd->DisplayListBox(szText);

					if( aJudge!=1 )
					{
						szErrorMessage = _T("�t���[�����[�g����NG");
						bReval = FALSE;
					}
				}
				else
				{
					szErrorMessage = _T("�t���[�����[�g�����G���[");
				}

				//��1.0.0.1062
				}else bReval = TRUE;
				//��1.0.0.1062

			}
			else
			if( !bReval )
			{
				szErrorMessage = _T("�X�N���v�g�G���[");
			}
		}
		//��1.0.0.1033

		//��1.0.0.1040
		else
		if( szClassName==_StCheckCSVDust )
		{
			StCheckCSVDust *pStCheckCSVDust = (StCheckCSVDust *)pBase;
			bReval = pStCheckCSVDust->function(szSection,szArgument);
			if( !bReval && szSection=="Execute" )
			{
				//��1.0.0.1062
				if( m_nScriptCheckMode==0 )
				{
				//��1.0.0.1062

				bReval = pStCheckCSVDust->Execute();
				//�������I����Ă�����bReval=TRUE�ŏI��---------------
				//�����GetJudge���擾
				if( bReval )
				{
					//���O�ɕۑ�
					CString szText = _T("Check=");
					INT aJudge = pStCheckCSVDust->GetJudge();
					OutputLogFile(szText + (aJudge==1?_T("OK"):_T("NG")));

					szText.Format(_T("Average=%f"), pStCheckCSVDust->GetResultAverage() );
					OutputLogFile( szText );
					pWnd->DisplayListBox(szText);

					szText.Format(_T("DifferenceRatio=%f"), pStCheckCSVDust->GetResultDifferenceRatio() );
					OutputLogFile( szText );
					pWnd->DisplayListBox(szText);

					szText.Format(_T("Line=%i"), pStCheckCSVDust->GetResultLineRatio() );
					OutputLogFile( szText );
					pWnd->DisplayListBox(szText);

					//��1.0.0.1043
					//���̏ꍇ�̓��[�h���Ⴄ
					INT nContinuousCount = pStCheckCSVDust->GetResultContinuousCount();
					if( nContinuousCount>=0 )
					{
						szText.Format(_T("ContinuousCount=%i"), nContinuousCount );
						OutputLogFile( szText );
						pWnd->DisplayListBox(szText);
						if( nContinuousCount>0 )
						{
							INT nContinuousStart = pStCheckCSVDust->GetResultContinuousStart();
							szText.Format(_T("ContinuousStart=%i"), nContinuousStart );
							OutputLogFile( szText );
							pWnd->DisplayListBox(szText);
							INT nContinuousEnd = pStCheckCSVDust->GetResultContinuousEnd();
							szText.Format(_T("ContinuousEnd=%i"), nContinuousEnd );
							OutputLogFile( szText );
							pWnd->DisplayListBox(szText);
						}
					}
					//��1.0.0.1043

					if( aJudge!=1 )
					{
						szErrorMessage = _T("CSV�S�~����NG");
						bReval = FALSE;
					}
				}
				else
				{
					szErrorMessage = _T("CSV�S�~�����G���[");
				}

				//��1.0.0.1062
				}else bReval = TRUE;
				//��1.0.0.1062

			}
			else
			if( !bReval )
			{
				szErrorMessage = _T("�X�N���v�g�G���[");
			}
		}
		//��1.0.0.1040
		//��1.0.0.1044
		else
		if( szClassName==_StCheckAdjustOffset )
		{
			StCheckAdjustOffset *pStCheckAdjustOffset = (StCheckAdjustOffset *)pBase;
			bReval = pStCheckAdjustOffset->function(szSection,szArgument);
			if( !bReval && szSection=="Execute" )
			{
				//��1.0.0.1062
				if( m_nScriptCheckMode==0 )
				{
				//��1.0.0.1062

				bReval = pStCheckAdjustOffset->Execute();
				//�������I����Ă�����bReval=TRUE�ŏI��---------------
				//�����GetJudge���擾
				if( bReval )
				{
					////���O�ɕۑ�
					CString szText = _T("Check=");
					INT aJudge = pStCheckAdjustOffset->GetJudge();
					OutputLogFile(szText + (aJudge==1?_T("OK"):_T("NG")));
					double dblChannelCount=0.0;
					pStCheckAdjustOffset->GetFunction( _T("GetChannelCount"), _T(""),  dblChannelCount);
					for( int i=0; i<(int)dblChannelCount; i++ )
					{
						TCHAR szText[64];
						_stprintf_s( szText, _countof(szText), _T("%i"), i);
						double dblOffset=0.0;
						pStCheckAdjustOffset->GetFunction( _T("GetResultOffset"), szText,  dblOffset);
						_stprintf_s( szText, _countof(szText), _T("FinalOffset_%i=%i"), i+1, (INT)dblOffset);
						OutputLogFile(szText);
					}

					//szText.Format(_T("Average=%f"), pStCheckCSVDust->GetResultAverage() );
					//OutputLogFile( szText );
					//pWnd->DisplayListBox(szText);

					//szText.Format(_T("DifferenceRatio=%f"), pStCheckCSVDust->GetResultDifferenceRatio() );
					//OutputLogFile( szText );
					//pWnd->DisplayListBox(szText);

					//szText.Format(_T("Line=%i"), pStCheckCSVDust->GetResultLineRatio() );
					//OutputLogFile( szText );
					//pWnd->DisplayListBox(szText);

					////��1.0.0.1043
					////���̏ꍇ�̓��[�h���Ⴄ
					//INT nContinuousCount = pStCheckCSVDust->GetResultContinuousCount();
					//if( nContinuousCount>=0 )
					//{
					//	szText.Format(_T("ContinuousCount=%i"), nContinuousCount );
					//	OutputLogFile( szText );
					//	pWnd->DisplayListBox(szText);
					//	if( nContinuousCount>0 )
					//	{
					//		INT nContinuousStart = pStCheckCSVDust->GetResultContinuousStart();
					//		szText.Format(_T("ContinuousStart=%i"), nContinuousStart );
					//		OutputLogFile( szText );
					//		pWnd->DisplayListBox(szText);
					//		INT nContinuousEnd = pStCheckCSVDust->GetResultContinuousEnd();
					//		szText.Format(_T("ContinuousEnd=%i"), nContinuousEnd );
					//		OutputLogFile( szText );
					//		pWnd->DisplayListBox(szText);
					//	}
					//}
					//��1.0.0.1043

					if( aJudge!=1 )
					{
						szErrorMessage = _T("�I�t�Z�b�g����NG");
						bReval = FALSE;
					}
				}
				else
				{
					szErrorMessage = _T("�I�t�Z�b�g�����G���[");
				}

				//��1.0.0.1062
				}else bReval = TRUE;
				//��1.0.0.1062

			}
			else
			if( !bReval )
			{
				szErrorMessage = _T("�X�N���v�g�G���[");
			}
		}
		//��1.0.0.1044

		else
		if( szClassName==_StCheckDustDlg )
		{
			StCheckBase * pCheckBase = (StCheckBase *)pBase;
			StCheckDustDlg *pStCheckDustDlg = (StCheckDustDlg *)pCheckBase->GetMainHandle();
			bReval = pStCheckDustDlg->function( szSection, szArgument);

			if( !bReval && szSection=="Execute" )
			{
				//��1.0.0.1062
				if( m_nScriptCheckMode==0 )
				{
				//��1.0.0.1062

				//pStCheckDustDlg->SetDeviceModelName(mDeviceModelName);
				//pStCheckDustDlg->SetSerialNumber(mSerialNumber);
				pStCheckDustDlg->SetSaveFilePath(mSaveFilePath);

				//��1.0.0.1026
				INT nMode = pStCheckDustDlg->GetMode();
				if( nMode==0 )
				{
					bReval = pStCheckDustDlg->DoModal() == IDOK;
				}
				else
				{
					bReval = pStCheckDustDlg->Execute();
				}

				//if( pStCheckDustDlg->DoModal() == IDOK )
				if( bReval )
				{
					//bReval = TRUE;
				//��1.0.0.1026
					//���O�ɕۑ�
					CString szText = _T("Check=");
					INT aJudge = pStCheckDustDlg->GetJudge();
					OutputLogFile(szText + (aJudge==1?_T("OK"):_T("NG")));
					INT aElement = pStCheckDustDlg->GetResultElement();
					LPCTSTR szColor[] = {_T("_Red"),_T("_Green"),_T("_Blue"),_T("")};
					for( int i=0; i<aElement; i++ )
					{
						INT nColorIndex = pStCheckDustDlg->GetElementColorIndex(i);
						if( nColorIndex<0 || nColorIndex>=3 ) nColorIndex=3;
						szText.Format(_T("Difference%s=%f"), szColor[nColorIndex], pStCheckDustDlg->GetResultDifference(i));
						OutputLogFile( szText );
						pWnd->DisplayListBox(szText);
						szText.Format(_T("Line%s=%d"), szColor[nColorIndex], pStCheckDustDlg->GetResultLine(i));
						OutputLogFile( szText );
						pWnd->DisplayListBox(szText);
						//��1.0.0.1024
						szText.Format(_T("Average%s=%f"), szColor[nColorIndex], pStCheckDustDlg->GetResultAverage(i));
						OutputLogFile( szText );
						pWnd->DisplayListBox(szText);
						//��1.0.0.1024

						//��1.0.0.1043
						//���̏ꍇ�̓��[�h���Ⴄ
						INT nContinuousCount = pStCheckDustDlg->GetResultContinuousCount(i);
						if( nContinuousCount>=0 )
						{
							szText.Format(_T("ContinuousCount%s=%i"), szColor[nColorIndex], nContinuousCount );
							OutputLogFile( szText );
							pWnd->DisplayListBox(szText);
							if( nContinuousCount>0 )
							{
								INT nContinuousStart = pStCheckDustDlg->GetResultContinuousStart(i);
								szText.Format(_T("ContinuousStart=%i"), nContinuousStart );
								OutputLogFile( szText );
								pWnd->DisplayListBox(szText);
								INT nContinuousEnd = pStCheckDustDlg->GetResultContinuousEnd(i);
								szText.Format(_T("ContinuousEnd=%i"), nContinuousEnd );
								OutputLogFile( szText );
								pWnd->DisplayListBox(szText);
							}
						}
						//��1.0.0.1043
					}
					if( aJudge!=1 )
					{
						szErrorMessage = _T("�S�~����NG");
						bReval = FALSE;
					}
				}
				//��1.0.0.1026
				else
				{
					szErrorMessage = _T("�S�~�����G���[");
				}
				//��1.0.0.1026

				//��1.0.0.1062
				}else bReval = TRUE;
				//��1.0.0.1062

			}
			else
			if( !bReval )
			{
				szErrorMessage = _T("�X�N���v�g�G���[");
			}
		}
		else
		if( szClassName==_StCheckMessage )
		{

			StCheckMessage *pStCheckMessage = (StCheckMessage *)pBase;
			bReval = pStCheckMessage->function(szSection,szArgument);
			if( !bReval && szSection=="Execute" )
			{
				//��1.0.0.1062
				if( m_nScriptCheckMode==0 )
				{
				//��1.0.0.1062

				bReval = pStCheckMessage->Execute();
				//�����GetJudge���擾
				//if( bReval )
				{
					CString szText = _T("Check=");
					INT aMessageJudge = pStCheckMessage->GetMessageJudge();
					INT aJudge = pStCheckMessage->GetJudge();
					szText += aJudge==1?_T("OK"):_T("NG");
					OutputLogFile(szText);
					pWnd->DisplayListBox(szText);
					if( aMessageJudge==2 )
					{
						szText = pStCheckMessage->GetTextButton();
						OutputLogFile(szText);
						pWnd->DisplayListBox(szText);
					}

					if( aJudge!=1 )
					{
						szErrorMessage = _T("���b�Z�[�WNG");
						bReval = FALSE;
					}
				}

				//��1.0.0.1062
				}else bReval = TRUE;
				//��1.0.0.1062

			}
			else
			if( !bReval && szSection=="DoModal" )
			{
				//��1.0.0.1062
				if( m_nScriptCheckMode==0 )
				{
				//��1.0.0.1062
				bReval = pStCheckMessage->Execute();
				//��1.0.0.1062
				}else bReval = TRUE;
				//��1.0.0.1062
			}
			else
			if( !bReval )
			{
				szErrorMessage = _T("�X�N���v�g�G���[");
			}
		}
		else
		if( szClassName==_StCheckAdjustLinetolineDlg )
		{
			StCheckBase * pCheckBase = (StCheckBase *)pBase;
			StCheckAdjustLinetolineDlg *pStCheckDlg = (StCheckAdjustLinetolineDlg *)pCheckBase->GetMainHandle();
			bReval = pStCheckDlg->function( szSection, szArgument);

			if( !bReval && szSection=="Execute" )
			{
				//��1.0.0.1062
				if( m_nScriptCheckMode==0 )
				{
				//��1.0.0.1062

				//if( pStCheckDlg->DoModal() == IDOK )
				//{
				//	bReval = TRUE;
				//}

				//pStCheckDlg->Create( IDD_DIALOG_LINETOLINE, NULL );
				pStCheckDlg->Create( StCheckAdjustLinetolineDlg::IDD, NULL );

				//INT nSizeX = pImageMessageDlg->GetImageSizeX() + pImageMessageDlg->GetMessageSizeX() + 30;
				//INT nSizeY = max( pImageMessageDlg->GetImageSizeX(), pImageMessageDlg->GetMessageSizeX() ) + 20;
				WINDOWINFO aInfo;
				pStCheckDlg->GetWindowInfo(&aInfo);
				INT nSizeX = aInfo.rcWindow.right - aInfo.rcWindow.left;
				INT nSizeY = aInfo.rcWindow.bottom - aInfo.rcWindow.top;
				pStCheckDlg->SetWindowPos( &CWnd::wndTop, 0, 0, nSizeX, nSizeY, SWP_NOZORDER|SWP_SHOWWINDOW );
				//::SetWindowPos( pStCheckDlg->m_hWnd, HWND_TOP, 0, 0, 0, 0, SWP_NOZORDER|SWP_SHOWWINDOW );
				pStCheckDlg->ShowWindow(SW_SHOW);
				::ShowWindowAsync(pStCheckDlg->m_hWnd,SW_SHOW);

//AfxMessageBox( _T("abc"), MB_OK );

				//SetWindowPos(NULL, 0, 0, 0, 0, SWP_HIDEWINDOW|
				//	SWP_NOSIZE|SWP_NOMOVE|SWP_NOACTIVATE|SWP_NOZORDER);
				//Sleep(1000);
				bReval = pStCheckDlg->Execute();

				pStCheckDlg->ShowWindow(SW_HIDE);

				if( bReval )
				{
					//���O�ɕۑ�
					CString szText = _T("Check=");
					INT aJudge = pStCheckDlg->GetJudge();
					OutputLogFile(szText + (aJudge==1?_T("OK"):_T("NG")));

					if( aJudge!=1 )
					{
						szErrorMessage = _T("Linetoline����NG");
						bReval = FALSE;
					}
				}
				else
				{
					szErrorMessage = _T("Linetoline�����G���[");
				}

				//��1.0.0.1062
				}else bReval = TRUE;
				//��1.0.0.1062

			}
			else
			if( !bReval )
			{
				szErrorMessage = _T("�X�N���v�g�G���[");
			}
		}
		else
		if( szClassName==_StCheckAdjustLinetoline )
		{
			StCheckAdjustLinetoline * pCheckLinetoline = (StCheckAdjustLinetoline *)pBase;
			bReval = pCheckLinetoline->function( szSection, szArgument);
			if( !bReval && szSection=="Execute" )
			{
				//��1.0.0.1062
				if( m_nScriptCheckMode==0 )
				{
				//��1.0.0.1062

				bReval = pCheckLinetoline->Execute();
				//�����GetJudge���擾
				if( bReval )
				{
					//���O�ɕۑ�
					CString szText = _T("Check=");
					INT aJudge = pCheckLinetoline->GetJudge();
					//��1.0.0.1015
					szText += aJudge==1?_T("OK"):_T("NG");
					OutputLogFile(szText);
					//OutputLogFile(szText + (aJudge==1?_T("OK"):_T("NG")));
					//��1.0.0.1015
					pWnd->DisplayListBox(szText);
					//��1.0.0.1020 beta1
					szText.Format(_T("MinAverage=%f"), pCheckLinetoline->GetResultMinAverage());
					OutputLogFile(szText);
					pWnd->DisplayListBox(szText);
					szText.Format(_T("Line=%i"), pCheckLinetoline->GetResultLine());
					OutputLogFile(szText);
					pWnd->DisplayListBox(szText);
					szText.Format(_T("Value=%i"), pCheckLinetoline->GetResultValue());
					OutputLogFile(szText);
					pWnd->DisplayListBox(szText);
					//��1.0.0.1020 beta1
					if( aJudge!=1 )
					{
						szErrorMessage = _T("Linetoline����NG");
						bReval = FALSE;
					}
				}
				else
				{
					//��1.0.0.1020 beta1
					StResult aResult = pCheckLinetoline->GetErrorCode();
					CString szText;
					szText.Format(_T("Error=[%s]"),aResult.GetCodeString().GetUnicode() );
					OutputLogFile(szText);
					pWnd->DisplayListBox(szText);
					//��1.0.0.1020 beta1
					szErrorMessage = _T("Linetoline�����G���[");
				}

				//��1.0.0.1062
				}else bReval = TRUE;
				//��1.0.0.1062

			}
			else
			if( !bReval )
			{
				szErrorMessage = _T("�X�N���v�g�G���[");
			}
		}

		else
		if( szClassName==_StCheckEffectivePixel )
		{

			StCheckEffectivePixel *pStCheckEffectivePixel = (StCheckEffectivePixel *)pBase;
			bReval = pStCheckEffectivePixel->function(szSection,szArgument);
			if( !bReval && szSection=="Execute" )
			{

				//��1.0.0.1062
				if( m_nScriptCheckMode==0 )
				{
				//��1.0.0.1062

				bReval = pStCheckEffectivePixel->Execute();
				//�����GetJudge���擾
				if( bReval )
				{
					//���O�ɕۑ�
					CString szText = _T("Check=");
					INT aJudge = pStCheckEffectivePixel->GetJudge();
					OutputLogFile(szText + (aJudge==1?_T("OK"):_T("NG")));
					pWnd->DisplayListBox(szText);
					INT aElement = pStCheckEffectivePixel->GetElement();
					LPCTSTR szColor[] = {_T("Red"),_T("Green"),_T("Blue")};
					for( int i=0; i<aElement; i++ )
					{
						INT nColorIndex = pStCheckEffectivePixel->GetElementColorIndex(i);
						szText.Format(_T("Start=%i"), pStCheckEffectivePixel->GetResultEffectiveStart(i));
						if( aElement==3 )	szText = szColor[nColorIndex] + szText;
						OutputLogFile( szText );
						pWnd->DisplayListBox(szText);
						szText.Format(_T("End=%i"), pStCheckEffectivePixel->GetResultEffectiveEnd(i));
						if( aElement==3 )	szText = szColor[nColorIndex] + szText;
						OutputLogFile( szText );
						pWnd->DisplayListBox(szText);
					}

					if( aJudge!=1 )
					{
						szErrorMessage = _T("�L����f����NG");
						bReval = FALSE;
					}
				}
				else
				{
					szErrorMessage = _T("�L����f�����G���[");
				}


				//��1.0.0.1062
				}else bReval = TRUE;
				//��1.0.0.1062

			}
			else
			if( !bReval )
			{
				szErrorMessage = _T("�X�N���v�g�G���[");
			}
		}
		else
		if( szClassName==_StInputSensorVersionDlg )
		{
			StCheckBase * pCheckBase = (StCheckBase *)pBase;
			StInputSensorVersionDlg *pStInputSensorVersionDlg = (StInputSensorVersionDlg *)pCheckBase->GetMainHandle();

			bReval = pStInputSensorVersionDlg->function(szSection,szArgument);

			if( !bReval )
			{
				if( szSection=="Execute" )
				{

					//��1.0.0.1062
					if( m_nScriptCheckMode==0 )
					{
					//��1.0.0.1062

					//bReval = pStInputSerialDlg->Execute();
					TCHAR szIniFileName[MAX_PATH];
					CStCommon::GetIniFileName(szIniFileName, _countof(szIniFileName));
					LPCTSTR	szSection = TEXT("SensorInformation");
					LPCTSTR	szKeyName = TEXT("SensorVersion");
					TCHAR	szTmp[MAX_PATH];
					GetPrivateProfileString(szSection, szKeyName, _T(""), szTmp, _countof(szTmp), szIniFileName);
					pStInputSensorVersionDlg->SetSensorVersion(szTmp);
					if( pStInputSensorVersionDlg->DoModal() == IDOK )
					{
						CString szSensorVersion= pStInputSensorVersionDlg->GetSensorVersion();
						bReval = TRUE;
						//���O�ɕۑ�
						CString szText = _T("SensorVersion=") + szSensorVersion;
						OutputLogFile(szText);
						pWnd->DisplayListBox(szText);
						//�ۑ�
						WritePrivateProfileString(szSection, szKeyName, szSensorVersion, szIniFileName);
						bReval = pStInputSensorVersionDlg->function(_T("VersionCheck"),NULL);
						if( !bReval )
						{
							szErrorMessage = _T("�Z���T�[�o�[�W�����`�F�b�NNG");
						}
					}
					else
					{
						szErrorMessage = _T("�Z���T�[�o�[�W�����I��");
					}

					//��1.0.0.1062
					}else bReval = TRUE;
					//��1.0.0.1062

				}
				else
				{
					szErrorMessage = _T("�X�N���v�g�G���[");
				}
			}

		}
		else
		if( szClassName==_StInputSensorSerialDlg )
		{
			StCheckBase * pCheckBase = (StCheckBase *)pBase;
			StInputSensorSerialDlg *pStInputSensorSerialDlg = (StInputSensorSerialDlg *)pCheckBase->GetMainHandle();
			if( szSection=="Execute" )
			{

				//��1.0.0.1062
				if( m_nScriptCheckMode==0 )
				{
				//��1.0.0.1062

				//bReval = pStInputSerialDlg->Execute();
				TCHAR szIniFileName[MAX_PATH];
				CStCommon::GetIniFileName(szIniFileName, _countof(szIniFileName));
				LPCTSTR	szSection = TEXT("SensorInformation");
				LPCTSTR	szKeyName = TEXT("SensorSerial");
				TCHAR	szTmp[MAX_PATH];
				GetPrivateProfileString(szSection, szKeyName, _T(""), szTmp, _countof(szTmp), szIniFileName);
				pStInputSensorSerialDlg->SetSensorSerial(szTmp);
				if( pStInputSensorSerialDlg->DoModal() == IDOK )
				{
					CString szSensorSerial= pStInputSensorSerialDlg->GetSensorSerial();
					//��1.0.0.1015
					mSensorSerialNumber = szSensorSerial;
					//��1.0.0.1015
					bReval = TRUE;
					//���O�ɕۑ�
					CString szText = _T("SensorSerial=") + szSensorSerial;
					OutputLogFile(szText);
					pWnd->DisplayListBox(szText);
					//�ۑ�
					WritePrivateProfileString(szSection, szKeyName, szSensorSerial, szIniFileName);
				}

				//��1.0.0.1062
				}else bReval = TRUE;
				//��1.0.0.1062

			}
			else
			{
				szErrorMessage = _T("�X�N���v�g�G���[");
			}
		}
		else
		if( szClassName==_StLightControlDlg )
		{
			StCheckBase * pCheckBase = (StCheckBase *)pBase;
			StLightControlDlg *pStLightControlDlg = (StLightControlDlg *)pCheckBase->GetMainHandle();

			bReval = pStLightControlDlg->function(szSection,szArgument);
			if( !bReval && szSection=="SetSerialComm" )
			{
				//��1.0.0.1062

				////��1.0.0.1017
				////pStLightControlDlg->SetSerialComm(GetSerialComm(szArgument));

				////��1.0.0.1020 �N���X�p���̌��̃n���h����ۑ�
				////pStLightControlDlg->SetSerialComm((StSerialComm*)GetSerialComm(szArgument));
				//StSerialComm * aSerialComm = (StSerialComm *)GetSerialComm(szArgument)->GetMainHandle();
				//pStLightControlDlg->SetSerialComm(aSerialComm);
				////��1.0.0.1020
				////��1.0.0.1017
				//bReval = TRUE;

				bReval = FALSE;
				StControl *aControl = GetSerialComm(szArgument);
				if( aControl )
				{
					StSerialComm * aSerialComm = (StSerialComm *)aControl->GetMainHandle();
					if( aSerialComm ) bReval = TRUE;
					pStLightControlDlg->SetSerialComm(aSerialComm);
				}
				//��1.0.0.1062

			}
			else
			if( !bReval && szSection=="Execute" )
			{

				//��1.0.0.1062
				if( m_nScriptCheckMode==0 )
				{
				//��1.0.0.1062


				pStLightControlDlg->Create( StLightControlDlg::IDD, NULL );

//�\����ʃX���b�h�ɂ��Ȃ��Ɩ������ۂ�
				/*
				unsigned int aThreadID;

				WINDOWINFO aInfo;
				pStLightControlDlg->GetWindowInfo(&aInfo);
				//INT nSizeX = aInfo.rcWindow.right - aInfo.rcWindow.left;
				//INT nSizeY = aInfo.rcWindow.bottom - aInfo.rcWindow.top;

				Dialog_Thread_Param aParam;
				aParam.pDlg = (void*)pStLightControlDlg;
				aParam.aWindowRect = aInfo.rcWindow;
				aParam.bReval = FALSE;
				HANDLE aThread = (HANDLE)_beginthreadex( NULL, 0, LightControlDialogThread, &aParam,  0, &aThreadID );

				WaitForSingleObject( aThread, INFINITE );
				bReval = aParam.bReval;
				*/

				WINDOWINFO aInfo;
				pStLightControlDlg->GetWindowInfo(&aInfo);
				INT nSizeX = aInfo.rcWindow.right - aInfo.rcWindow.left;
				INT nSizeY = aInfo.rcWindow.bottom - aInfo.rcWindow.top;
				pStLightControlDlg->SetWindowPos( &CWnd::wndTop, 0, 0, nSizeX, nSizeY, SWP_NOZORDER|SWP_SHOWWINDOW );
				pStLightControlDlg->ShowWindow(SW_SHOW);
				::ShowWindowAsync(pStLightControlDlg->m_hWnd,SW_SHOW);

				Sleep(1000);

				bReval = pStLightControlDlg->Execute();

				pStLightControlDlg->ShowWindow(SW_HIDE);
				
				if( bReval )
				{
					//���O�ɕۑ�
					//CString szText = _T("Check=");
					INT aJudge = pStLightControlDlg->GetJudge();
					//OutputLogFile(szText + (aJudge==1?_T("OK"):_T("NG")));
					if( aJudge!=1 )
					{
						szErrorMessage = _T("LightControl NG");
						bReval = FALSE;
					}
				}
				else
				{
					szErrorMessage = _T("LightControl �G���[");
					//��1.0.0.1073 beta2
					CString szError = pStLightControlDlg->GetErrorMessage();
					CString szText = _T("Error=");
					szText += szError;
					OutputLogFile(szText);
					if( szError.GetLength()>0 )
					{
						szErrorMessage += _T("(");
						szErrorMessage += szError;
						szErrorMessage += _T(")");
					}
					//��1.0.0.1073 beta2
				}

				//��1.0.0.1062
				}else bReval = TRUE;
				//��1.0.0.1062

			}
			else
			if( !bReval )
			{
				szErrorMessage = _T("�X�N���v�g�G���[");
			}
		}

		else
		if( szClassName==_StLightControl )
		{
			StLightControl * pStLightControl = (StLightControl *)pBase;

			bReval = pStLightControl->function(szSection,szArgument);
			if( !bReval && szSection=="SetSerialComm" )
			{
				//��1.0.0.1062

				////��1.0.0.1017
				////pStLightControl->SetSerialComm(GetSerialComm(szArgument));
				////��1.0.0.1020 �N���X�p���̌��̃n���h����ۑ�
				////pStLightControl->SetSerialComm((StSerialComm*)GetSerialComm(szArgument));
				//StSerialComm * aSerialComm = (StSerialComm *)GetSerialComm(szArgument)->GetMainHandle();
				//pStLightControl->SetSerialComm(aSerialComm);
				////��1.0.0.1020
				////��1.0.0.1017
				//bReval = TRUE;

				bReval = FALSE;
				StControl *aControl = GetSerialComm(szArgument);
				if( aControl )
				{
					StSerialComm * aSerialComm = (StSerialComm *)aControl->GetMainHandle();
					if( aSerialComm ) bReval = TRUE;
					pStLightControl->SetSerialComm(aSerialComm);
				}
				//��1.0.0.1062

			}
			//��1.0.0.1017
			else
			if( !bReval && szSection=="SetTerminal" )
			{
				//��1.0.0.1062

				////��1.0.0.1020 �N���X�p���̌��̃n���h����ۑ�
				////pStLightControl->SetTerminal((StTerminal *)GetSerialComm(szArgument));
				//StTerminal * aTerminal = (StTerminal *)GetSerialComm(szArgument)->GetMainHandle();
				//pStLightControl->SetTerminal(aTerminal);
				////��1.0.0.1020
				//bReval = TRUE;

				bReval = FALSE;
				StControl *aControl = GetSerialComm(szArgument);
				if( aControl )
				{
					StTerminal * aTerminal = (StTerminal *)aControl->GetMainHandle();
					if( aTerminal ) bReval = TRUE;
					pStLightControl->SetTerminal(aTerminal);
				}
				//��1.0.0.1062
			}
			//��1.0.0.1017
			else
			if( !bReval && szSection=="Execute" )
			{


				//��1.0.0.1021
				//��1.0.0.1020
				//pStLightControl->ClearLightChannel();
				//if( m_pStructLightChannel )
				//{
				//	INT nCount = m_pStructLightChannel->GetPortSelectorCount();
				//	for( int i=0; i<nCount; i++ )
				//	{
				//		INT nIndex;
				//		m_pStructLightChannel->GetIndex(i,nIndex);
				//		CString szChannel;
				//		m_pStructLightChannel->GetSelectorName(i,szChannel);
				//		pStLightControl->AddLightChannel(szChannel,nIndex);
				//	}
				//}
				//��1.0.0.1020
				//��1.0.0.1021


				//��1.0.0.1062
				if( m_nScriptCheckMode==0 )
				{
				//��1.0.0.1062

				bReval = pStLightControl->Execute();
				//�������I����Ă�����bReval=TRUE�ŏI��---------------
				//�����GetJudge���擾
				if( bReval )
				{
					//���O�ɕۑ�
					INT aJudge = pStLightControl->GetJudge();
					CString szText = _T("Check=");
					OutputLogFile(szText + (aJudge==1?_T("OK"):_T("NG")));

					INT iColorIndex=0;
					CString szCheckColor("");
					do
					{
						CString szColor;
						BOOL bColor = pStLightControl->GetColorIndex(iColorIndex,szColor);
						if( !bColor ) break;
						if( szCheckColor.Find(szColor)<0 )
						{
							INT nValue = pStLightControl->GetResultValue(szColor);
							double dblAverage = pStLightControl->GetResultAverage(szColor);
							if( nValue>=0 && dblAverage>=0.0 )
							{
								szText.Format(_T("%s=%i"),szColor,nValue);
								OutputLogFile(szText);
								szText.Format(_T("%s_Ave=%f"),szColor,dblAverage);
								OutputLogFile(szText);
							}
							szCheckColor += szColor;
						}
						iColorIndex++;
					}while(1);

					if( aJudge!=1 )
					{
						szErrorMessage = _T("LightControl NG");
						bReval = FALSE;
					}
				}
				else
				{
					szErrorMessage = _T("LightControl �G���[");
					//��1.0.0.1073 beta2
					CString szError = pStLightControl->GetErrorMessage();
					CString szText = _T("Error=");
					szText += szError;
					OutputLogFile(szText);
					if( szError.GetLength()>0 )
					{
						szErrorMessage += _T("(");
						szErrorMessage += szError;
						szErrorMessage += _T(")");
					}
					//��1.0.0.1073 beta2
				}

				//��1.0.0.1062
				}else bReval = TRUE;
				//��1.0.0.1062

			}
			else
			if( !bReval )
			{
				szErrorMessage = _T("�X�N���v�g�G���[");
			}

			//��1.0.0.1062
			if( m_nScriptCheckMode==0 )
			{
			//��1.0.0.1062

			//��1.0.0.1024
			INT nCount= pStLightControl->GetLogMode();
			if( nCount>0 )
			{
				pStLightControl->SetLogMode(0);	//Reset
				for( int i=0; i<nCount; i++ )
				{
					CString szText;
					pStLightControl->GetLightResultString( i, szText );
					OutputLogFile(szText);
				}
			}
			//��1.0.0.1024

			//��1.0.0.1062
			}else bReval = TRUE;
			//��1.0.0.1062
		}

		else
		if( szClassName==_StCheckPower )
		{
			StCheckPower *pCheckPower = (StCheckPower *)pBase;
			bReval = pCheckPower->function( szSection, szArgument );

			if( !bReval && szSection=="SetSerialComm" )
			{
				//��1.0.0.1062

				////��1.0.0.1017
				////pCheckPower->SetSerialComm(GetSerialComm(szArgument));
				////��1.0.0.1020 �N���X�p���̌��̃n���h����ۑ�
				////pCheckPower->SetSerialComm((StSerialComm*)GetSerialComm(szArgument));
				//StSerialComm * aSerialComm = (StSerialComm *)GetSerialComm(szArgument)->GetMainHandle();
				//pCheckPower->SetSerialComm(aSerialComm);
				////��1.0.0.1020
				////��1.0.0.1017
				//bReval = TRUE;

				bReval = FALSE;
				StControl *aControl = GetSerialComm(szArgument);
				if( aControl )
				{
					StSerialComm * aSerialComm = (StSerialComm *)aControl->GetMainHandle();
					if( aSerialComm ) bReval = TRUE;
					pCheckPower->SetSerialComm(aSerialComm);
				}
				//��1.0.0.1062


			}
			else
			if( !bReval && szSection=="SetTexioControl" )
			{
				//��1.0.0.1062
				//pCheckPower->SetTexioControl(GetTexioControl(szArgument));
				//bReval = TRUE;
				StTexioControl *aControl = GetTexioControl(szArgument);
				if( aControl )
				{
					pCheckPower->SetTexioControl(GetTexioControl(szArgument));
				}
				bReval = aControl==NULL?FALSE:TRUE;
				//��1.0.0.1062
			}
			//��1.0.0.1052
			else
			if( !bReval && szSection=="SetInterfaceGenICam" )
			{
				//��1.0.0.1062
				//pCheckPower->SetInterfaceGenICam((StInterfaceGenICam*)GetCommonBase(szArgument));
				//bReval = TRUE;
				StCommonBase *aCommonBase = GetCommonBase(szArgument);
				pCheckPower->SetInterfaceGenICam((StInterfaceGenICam*)aCommonBase);
				bReval = aCommonBase==NULL?FALSE:TRUE;
				//��1.0.0.1062
			}
			//��1.0.0.1052
			else
			if( !bReval && szSection=="Execute" )
			{
				//��1.0.0.1062
				if( m_nScriptCheckMode==0 )
				{
				//��1.0.0.1062

				bReval = pCheckPower->Execute();
				//�������I����Ă�����bReval=TRUE�ŏI��---------------
				//�����GetJudge���擾
				if( bReval )
				{
					//���O�ɕۑ�
					CString szText = _T("Check=");
					INT aJudge = pCheckPower->GetJudge();
					OutputLogFile(szText + (aJudge==1?_T("OK"):_T("NG")));
					//��1.0.0.1048
					//INT nCurrent = pCheckPower->GetCurrent();
					//szText.Format(_T("Current=%i"), nCurrent);
					double dblCurrent = pCheckPower->GetCurrent();
					//��1.0.0.1052
					//szText.Format(_T("Current=%.01f"), dblCurrent);
					szText.Format(_T("Current=%.03f"), dblCurrent);
					//��1.0.0.1052
					//��1.0.0.1048
					OutputLogFile( szText );
					pWnd->DisplayListBox(szText);

					if( aJudge!=1 )
					{
						szErrorMessage = _T("����d������NG");
						bReval = FALSE;
					}
				}
				else
				{
					szErrorMessage = _T("����d�������G���[");
				}

				//��1.0.0.1062
				}else bReval = TRUE;
				//��1.0.0.1062


			}
			else
			if( !bReval )
			{
				szErrorMessage = _T("�X�N���v�g�G���[");
			}
		}

		else
		if( szClassName==_StCheckCommon )
		{
			StCheckCommon *pCheckCommon = (StCheckCommon *)pBase;
			pCheckCommon->SetSaveFilePath(mSaveFilePath);
			bReval = pCheckCommon->function( szSection, szArgument );
			//��1.0.0.1062
			//if( !bReval )
			if( !bReval && m_nScriptCheckMode==0 )
			//��1.0.0.1062
			{
				szErrorMessage = _T("�X�N���v�g�G���[");
				//��1.0.0.1035
				CString szGetErrorMessage;
				pCheckCommon->GetLastErrorMessage(szGetErrorMessage);
				if( szGetErrorMessage.GetLength()>0 )
				{
					szErrorMessage = szGetErrorMessage;
					pCheckCommon->ClearLastErrorMessage();
				}
				//��1.0.0.1035
			}
		}

		else
		if( szClassName==_StCheckSerialVersion )
		{
			StCheckSerialVersion *pCheckSerialVersion = (StCheckSerialVersion *)pBase;
			bReval = pCheckSerialVersion->function( szSection, szArgument );

			if( !bReval && szSection=="Execute" )
			{
				//��1.0.0.1043
				TCHAR szText[MAX_PATH];
				pCheckSerialVersion->GetEllitoSerialNumberFilepath(szText,_countof(szText));
				if( _tcslen(szText)==0 )
				{
					CStCommon::GetExeFilePath(szText, _countof(szText));
					_tcscat_s(szText, _countof(szText), _T("\\CONFIG\\Standard"));
					pCheckSerialVersion->function(_T("SetEllitoSerialNumberFilepath"),szText);
				}
				//pCheckSerialVersion->GetEllitoSerialNumberFilename(szText,_countof(szText));
				//if( _tcslen(szText)==0 )
				//{
				//	pCheckSerialVersion->function(_T("SetEllitoSerialNumberFilename"),_T("EllitoSerialNumber.txt"));
				//}
				//��1.0.0.1043

				//��1.0.0.1062
				if( m_nScriptCheckMode==0 )
				{
				//��1.0.0.1062

				bReval = pCheckSerialVersion->Execute();
				if( bReval )
				{
					//���O�ɕۑ�
					CString szText = _T("Check=");
					INT aJudge = pCheckSerialVersion->GetJudge();
					OutputLogFile(szText + (aJudge==1?_T("OK"):_T("NG")));
					TCHAR szTmp[256];
					if( pCheckSerialVersion->IsCheckDeviceVersion() )
					{
						pCheckSerialVersion->GetResultDeviceVersion(szTmp, _countof(szTmp));
						szText.Format(_T("DeviceVersion="));
						szText += szTmp;
						OutputLogFile( szText );
						pWnd->DisplayListBox(szText);
					}
					if( pCheckSerialVersion->IsCheckDeviceFirmwareVersion() )
					{
						pCheckSerialVersion->GetResultDeviceFirmwareVersion(szTmp, _countof(szTmp));
						szText.Format(_T("DeviceFirmwareVersion="));
						szText += szTmp;
						OutputLogFile( szText );
						pWnd->DisplayListBox(szText);
					}
					if( pCheckSerialVersion->IsCheckDeviceSerialNumber() )
					{
						pCheckSerialVersion->GetResultDeviceSerialNumber(szTmp, _countof(szTmp));
						szText.Format(_T("DeviceSerialNumber="));
						szText += szTmp;
						OutputLogFile( szText );
						pWnd->DisplayListBox(szText);
					}
					//��1.0.0.1043
					if( pCheckSerialVersion->IsCheckSensorSerialNumber() )
					{
						pCheckSerialVersion->GetResultSensorSerialNumber(szTmp, _countof(szTmp));
						szText.Format(_T("SensorSerialNumber="));
						szText += szTmp;
						OutputLogFile( szText );
						pWnd->DisplayListBox(szText);
					}
					//��1.0.0.1043

					//��1.0.0.1071
					if( pCheckSerialVersion->IsCheckDeviceModelName() )
					{
						pCheckSerialVersion->GetResultDeviceModelName(szTmp, _countof(szTmp));
						szText.Format(_T("DeviceModelName="));
						szText += szTmp;
						OutputLogFile( szText );
						pWnd->DisplayListBox(szText);
					}
					//��1.0.0.1071

					if( aJudge!=1 )
					{
						szErrorMessage = _T("�V���A���o�[�W��������NG");
						bReval = FALSE;
					}
				}
				else
				{
					szErrorMessage = _T("�V���A���o�[�W���������G���[");
				}

				//��1.0.0.1062
				}else bReval = TRUE;
				//��1.0.0.1062


			}
			else
			if( !bReval )
			{
				szErrorMessage = _T("�X�N���v�g�G���[");
			}
		}
		else
		if( szClassName==_StCheckIO )
		{
			StCheckIO *pCheckIO = (StCheckIO *)pBase;
			bReval = pCheckIO->function( szSection, szArgument );

			if( !bReval && szSection=="SetSerialComm" )
			{
				//��1.0.0.1062

				////��1.0.0.1017
				////pCheckIO->SetSerialComm(GetSerialComm(szArgument));
				////��1.0.0.1020 �N���X�p���̌��̃n���h����ۑ�
				////pCheckIO->SetSerialComm((StSerialComm*)GetSerialComm(szArgument));
				//StSerialComm * aSerialComm = (StSerialComm *)GetSerialComm(szArgument)->GetMainHandle();
				//pCheckIO->SetSerialComm(aSerialComm);
				////��1.0.0.1020
				////��1.0.0.1017
				//bReval = TRUE;

				bReval = FALSE;
				StControl *aControl = GetSerialComm(szArgument);
				if( aControl )
				{
					StSerialComm * aSerialComm = (StSerialComm *)aControl->GetMainHandle();
					if( aSerialComm ) bReval = TRUE;
					pCheckIO->SetSerialComm(aSerialComm);
				}
				//��1.0.0.1062

			}
			//��1.0.0.1053
			else
			if( !bReval && szSection=="SetTerminal" )
			{
				//��1.0.0.1062
				//StTerminal * aTerminal = (StTerminal *)GetSerialComm(szArgument)->GetMainHandle();
				//pCheckIO->SetTerminal(aTerminal);
				//bReval = TRUE;

				bReval = FALSE;
				StControl *aControl = GetSerialComm(szArgument);
				if( aControl )
				{
					StTerminal * aTerminal = (StTerminal *)aControl->GetMainHandle();
					if( aTerminal ) bReval = TRUE;
					pCheckIO->SetTerminal(aTerminal);
				}
				//��1.0.0.1062

			}
			//��1.0.0.1053
			else
			if( !bReval && szSection=="Execute" )
			{

				//��1.0.0.1062
				if( m_nScriptCheckMode==0 )
				{
				//��1.0.0.1062

				bReval = pCheckIO->Execute();
				//�������I����Ă�����bReval=TRUE�ŏI��---------------
				//�����GetJudge���擾
				if( bReval )
				{
					//���O�ɕۑ�
					CString szText = _T("Check=");
					INT aJudge = pCheckIO->GetJudge();
					OutputLogFile(szText + (aJudge==1?_T("OK"):_T("NG")));
					//��1.0.0.1053
					if( aJudge!=1 ) 
					{
						double dblValue;
						PTCHAR IO_CHECK[] = { _T("CameraStatus"),_T("CameraSetValue"),_T("JiguStatus"),_T("JiguSetValue") };
						for( int i=0; i<_countof(IO_CHECK); i++ )
						{
							if( pCheckIO->GetFunction(IO_CHECK[i], NULL, dblValue) )
							{
								INT nValue = (INT)dblValue;
								if( nValue>=0 )
								{
									szText.Format(_T("%s=0x%x"),IO_CHECK[i],nValue);
									OutputLogFile(szText);
								}
							}
						}
					}
					//��1.0.0.1053

					if( aJudge!=1 )
					{
						szErrorMessage = _T("IO����NG");
						bReval = FALSE;
					}
				}
				else
				{
					//��1.0.0.1073 beta2
					//szErrorMessage = _T("IO�����G���[");
					CString szError = pCheckIO->GetErrorMessage();
					CString szText = _T("Error=");
					szText += szError;
					OutputLogFile(szText);
					szErrorMessage = _T("IO�����G���[");
					if( szError.GetLength()>0 )
					{
						szErrorMessage += _T("(");
						szErrorMessage += szError;
						szErrorMessage += _T(")");
					}
					//��1.0.0.1073 beta2
				}

				//��1.0.0.1062
				}else bReval = TRUE;
				//��1.0.0.1062


			}
			else
			if( !bReval )
			{
				szErrorMessage = _T("�X�N���v�g�G���[");
			}
		}
		//��1.0.0.1049
		else
		if( szClassName==_StCheckAdjustGainDlg )
		{
			StCheckBase * pCheckBase = (StCheckBase *)pBase;
			StCheckAdjustGainDlg *pStCheckDlg = (StCheckAdjustGainDlg *)pCheckBase->GetMainHandle();
			bReval = pStCheckDlg->function( szSection, szArgument);

			if( !bReval && szSection=="Execute" )
			{

				//if( pStCheckDlg->DoModal() == IDOK )
				//{
				//	bReval = TRUE;
				//}

				//��1.0.0.1062
				if( m_nScriptCheckMode==0 )
				{
				//��1.0.0.1062

				//pStCheckDlg->Create( IDD_DIALOG_LINETOLINE, NULL );
				pStCheckDlg->Create( StCheckAdjustGainDlg::IDD, NULL );

				//INT nSizeX = pImageMessageDlg->GetImageSizeX() + pImageMessageDlg->GetMessageSizeX() + 30;
				//INT nSizeY = max( pImageMessageDlg->GetImageSizeX(), pImageMessageDlg->GetMessageSizeX() ) + 20;
				WINDOWINFO aInfo;
				pStCheckDlg->GetWindowInfo(&aInfo);
				INT nSizeX = aInfo.rcWindow.right - aInfo.rcWindow.left;
				INT nSizeY = aInfo.rcWindow.bottom - aInfo.rcWindow.top;
				pStCheckDlg->SetWindowPos( &CWnd::wndTop, 0, 0, nSizeX, nSizeY, SWP_NOZORDER|SWP_SHOWWINDOW );
				//::SetWindowPos( pStCheckDlg->m_hWnd, HWND_TOP, 0, 0, 0, 0, SWP_NOZORDER|SWP_SHOWWINDOW );
				pStCheckDlg->ShowWindow(SW_SHOW);
				::ShowWindowAsync(pStCheckDlg->m_hWnd,SW_SHOW);


				//SetWindowPos(NULL, 0, 0, 0, 0, SWP_HIDEWINDOW|
				//	SWP_NOSIZE|SWP_NOMOVE|SWP_NOACTIVATE|SWP_NOZORDER);
				//Sleep(1000);
				bReval = pStCheckDlg->Execute();

				pStCheckDlg->ShowWindow(SW_HIDE);

				if( bReval )
				{
					//���O�ɕۑ�
					CString szText = _T("Check=");
					INT aJudge = pStCheckDlg->GetJudge();
					OutputLogFile(szText + (aJudge==1?_T("OK"):_T("NG")));

					if( aJudge!=1 )
					{
						szErrorMessage = _T("Gain����NG");
						bReval = FALSE;
					}
				}
				else
				{
					szErrorMessage = _T("Gain�����G���[");
				}
				
				//��1.0.0.1062
				}else bReval = TRUE;
				//��1.0.0.1062

			}
			else
			if( !bReval )
			{
				szErrorMessage = _T("�X�N���v�g�G���[");
			}
		}
		//��1.0.0.1049
		else
		if( szClassName==_StPTouch )
		{
			StPTouch *pPTouch = (StPTouch *)pBase;
			//��1.0.0.1047
			if( szSection==_T("SetHinmokuCode") )
			{
				CString szHinmokuCode = mHinmokuCode;
				if( szArgument.GetLength()>0 )
					szHinmokuCode = szArgument;
				pPTouch->SetHinmokuCode(szHinmokuCode);
				bReval = TRUE;
			}
			else
			if( szSection==_T("SetSerialNumber") )
			{
				CString szSerialNumber = mSerialNumber;
				if( szArgument.GetLength()>0 )
					szSerialNumber = szArgument;
				pPTouch->SetSerialNumber(szSerialNumber);
				bReval = TRUE;
			}
			else
			if( szSection==_T("SetDeviceModelName") )
			{
				CString szDeviceModelName = mDeviceModelName;
				if( szArgument.GetLength()>0 )
					szDeviceModelName = szArgument;
				pPTouch->SetDeviceModelName(szDeviceModelName);
				bReval = TRUE;
			}
			if( !bReval )
			//��1.0.0.1047
			bReval = pPTouch->function( szSection, szArgument );

			if( !bReval && szSection=="Execute" )
			{

				//��1.0.0.1062
				if( m_nScriptCheckMode==0 )
				{
				//��1.0.0.1062

				m_bExecutePrintPTouch = TRUE;	//���C���X���b�h�œ��삳����
				do
				{
					Sleep(500);
				}while(m_bExecutePrintPTouch);
				if( m_nPTouchErrorCode==0 )
				{
					bReval = TRUE;
				}
				else
				{
					szErrorMessage = _T("P-Touch �G���[");
				}

				//��1.0.0.1062
				}else bReval = TRUE;
				//��1.0.0.1062

			}
			else
			if( !bReval )
			{
				//��1.0.0.1009
				szErrorMessage = _T("�X�N���v�g�G���[");
				//szErrorMessage = _T("P-Touch �G���[");
				//��1.0.0.1009
			}
		}
		//��1.0.0.1015
		else
		if( szClassName==_StWriteSerial )
		{
			StWriteSerial *pWriteSerial = (StWriteSerial *)pBase;
			bReval = pWriteSerial->function( szSection, szArgument );
			if( !bReval && szSection=="Execute" )
			{

				//��1.0.0.1062
				if( m_nScriptCheckMode==0 )
				{
				//��1.0.0.1062

				pWriteSerial->SetSerialNumber(mSerialNumber);
				pWriteSerial->SetSensorSerialNumber(mSensorSerialNumber);
				bReval = pWriteSerial->Execute();
				//�������I����Ă�����bReval=TRUE�ŏI��---------------
				//�����GetJudge���擾
				if( bReval )
				{
					//���O�ɕۑ�
					CString szText = _T("Check=");
					INT aJudge = pWriteSerial->GetJudge();
					OutputLogFile(szText + (aJudge==1?_T("OK"):_T("NG")));

					if( aJudge!=1 )
					{
						szErrorMessage = _T("�V���A��������NG");
						bReval = FALSE;
					}
				}
				else
				{
					szErrorMessage = _T("�V���A�������݃G���[");
				}

				//��1.0.0.1062
				}else bReval = TRUE;
				//��1.0.0.1062

			}
			else
			if( !bReval )
			{
				szErrorMessage = _T("�X�N���v�g�G���[");
			}
		}
		//��1.0.0.1015

		//��1.0.0.1058
		else
		if( szClassName==_StWriteFileData )
		{
			StWriteFileData *pWriteFileData = (StWriteFileData *)pBase;
			bReval = pWriteFileData->function( szSection, szArgument );
			if( !bReval && szSection=="Execute" )
			{

				//��1.0.0.1062
				if( m_nScriptCheckMode==0 )
				{
				//��1.0.0.1062

				bReval = pWriteFileData->Execute();
				//�������I����Ă�����bReval=TRUE�ŏI��---------------
				//�����GetJudge���擾
				if( bReval )
				{
					//���O�ɕۑ�
					CString szText = _T("Check=");
					INT aJudge = pWriteFileData->GetJudge();
					OutputLogFile(szText + (aJudge==1?_T("OK"):_T("NG")));

					if( aJudge!=1 )
					{
						szErrorMessage = _T("WriteFileData NG");
						bReval = FALSE;
					}
				}
				else
				{
					szErrorMessage = _T("WriteFileData�G���[");
				}

				//��1.0.0.1062
				}else bReval = TRUE;
				//��1.0.0.1062

			}
			else
			if( !bReval )
			{
				szErrorMessage = _T("�X�N���v�g�G���[");
			}
		}
		//��1.0.0.1058


		//��1.0.0.1023
		else
		if( szClassName==_StLoop )
		{
			StLoop *pLoop = (StLoop *)pBase;
			if( szSection=="Start" )
			{
				pLoop->SetStartLine(nScriptLine);
				StString szVarName = pLoop->m_VarName;
				INT nEnd = GetLoopEndLine(szVarName.GetUnicode(),nScriptLine);
				pLoop->SetEndLine(nEnd);
				pLoop->SetValue(m_vecValue);

			}
			bReval = pLoop->function( szSection, szArgument );
			if( bReval )
			{
				//��1.0.0.1062
				if( m_nScriptCheckMode==0 )
				//��1.0.0.1062
				m_nNextLine = pLoop->GetNextLine();
			}
			else
			{
				szErrorMessage = _T("�X�N���v�g�G���[");
			}
		}

		else
		if( szClassName==_StValue )
		{

			StValue *pValue = (StValue *)pBase;
			bReval = pValue->function( szSection );
			//bReval = pValue->function( szSection, szArgument );
			if( bReval )
			{
			}
			else
			{
				szErrorMessage = _T("�X�N���v�g�G���[");
			}

		}
		//��1.0.0.1023

		//��1.0.0.1030
		else
		if( szClassName==_StIF )
		{
			StIF *pIF = (StIF *)pBase;
			if( szSection=="Start" )
			{
				pIF->SetStartLine(nScriptLine);
				StString szVarName = pIF->m_VarName;
				//INT nEnd = GetLoopEndLine(szVarName.GetUnicode(),nScriptLine);
				INT nEnd = GetIFEndLine(szVarName.GetUnicode(),nScriptLine);
				if( nEnd>0 )
				{
					pIF->SetEndLine(nEnd);
					INT nElse;
					INT nStart = nScriptLine;
					do
					{
						nElse = GetIFElseIfLine(szVarName.GetUnicode(),nStart, nEnd);
						if( nElse>0 )	pIF->SetElseIfLine(nElse);
						else break;
						nStart = nElse + 1;
					}while(nElse>0);
					nElse = GetIFElseLine(szVarName.GetUnicode(),nScriptLine, nEnd);
					if( nElse>0 )	pIF->SetElseLine(nElse);
				}
				pIF->SetValue(m_vecValue);
			}

			pIF->SetNowLine(nScriptLine);
			bReval = pIF->function( szSection, szArgument );

			if( bReval )
			{
				//��1.0.0.1062
				if( m_nScriptCheckMode==0 )
				//��1.0.0.1062
				m_nNextLine = pIF->GetNextLine();
			}
			else
			{
				szErrorMessage = _T("�X�N���v�g�G���[");
			}
		}
		//��1.0.0.1030

		//��1.0.0.1025 �t�@�C������ǉ�
		else
		if( szClassName==_StFileOperation )
		{

			StFileOperation *pOperation = (StFileOperation *)pBase;
			CString szDst = szArgument;
			//szArgument�̕ϊ�
			if( szSection==_T("Write") )
			{
				ChangeStringToOutputString(szArgument,szDst);
			}
			bReval = pOperation->function( szSection, szDst );
			if( bReval )
			{
			}
			else
			{
				szErrorMessage = _T("�X�N���v�g�G���[");
			}

		}
		//��1.0.0.1025 �t�@�C������ǉ�
		//��1.0.0.1051
		else
		if( szClassName==_StInterfaceGenICam )
		{
			//��1.0.0.1062
			if( m_nScriptCheckMode==0 )
			{
			//��1.0.0.1062


			StInterfaceGenICam *pOperation = (StInterfaceGenICam *)pBase;
			bReval = pOperation->function( szSection, szArgument );
			if( bReval )
			{
			}
			else
			{
				szErrorMessage = _T("�X�N���v�g�G���[");
			}

			//��1.0.0.1062
			}else bReval = TRUE;
			//��1.0.0.1062

		}
		//��1.0.0.1051
	
	}while(0);

	return bReval;
}


//-------------------------------------------------
//�����N���X�ȊO�̎��s Execute,Set�Ȃ�
//"."�����_T(" ")��������͖̂���
//_T("(")�����_T("=")���������
//-------------------------------------------------
BOOL StInspectMain::ExecuteFunction(CString &szString)
{
	BOOL bReval = FALSE;
	do
	{
		szString.Trim();	//�ŏ���Trim����B
		CString szSection,szArgument;

		if( !SepareteString(szString, szSection, szArgument ) )
			break;

		//��1.0.0.1023 �J�Ԃ��p
		ChangeArgument(szArgument);
		//��1.0.0.1023 �J�Ԃ��p

		//INT iPos;
		//INT iPeriodPos = szString.Find(_T("."));
		//INT iSpacePos = szString.Find(_T(" "));
		//if( (iPos=szString.Find(_T("(")))>0 )
		//{
		//	//if( (iPeriodPos>0 && iPos>iPeriodPos) || (iSpacePos>0 && iPos>iSpacePos) ) break;
		//	if( iPeriodPos>0 && iPos>iPeriodPos) break;
		//	//�֐� ���b�Z�[�W�A�ݒ�Ȃ�
		//	szSection = szString.Left(iPos);
		//	szSection.Trim();
		//	//szSection.MakeLower();
		//	szArgument = szString.Mid(iPos+1);
		//	szArgument = szArgument.Left(szArgument.Find(_T(")")));
		//	szArgument.Trim();

		//}
		//else
		//if( (iPos=szString.Find(_T("=")))>0 )
		//{
		//	//if( (iPeriodPos>0 && iPos>iPeriodPos) || (iSpacePos>0 && iPos>iSpacePos) ) break;
		//	if( iPeriodPos>0 && iPos>iPeriodPos) break;
		//	//�֐� ����Ȃ�
		//	szSection = szString.Left(iPos);
		//	szSection.Trim();
		//	//szSection.MakeLower();
		//	szArgument = szString.Mid(iPos+1);
		//	szArgument.Trim();
		//}
		//else
		//{
		//	if( iPeriodPos>0 || iSpacePos>0 ) break;
		//	szSection = szString;
		//}
		szSection.MakeLower();

		//���s------------------------------------------------
		if( szSection.GetLength()>0 )
		{
			if( szSection==_T("devicemodelname") )
			{
				mDeviceModelName = szArgument;
				//DeviceModelName�m��Ń��OPATH�A�t�@�C�����쐬
				CreateLogPathName();
				CreateLogFileName();
				bReval = TRUE;
			}
			else
			if( szSection==_T("savefilepath") )
			{
				if( szArgument ==_T("CSIDL_COMMON_APPDATA") || szArgument ==_T("SameIniFilePath") )
				{
					TCHAR szPath[MAX_PATH];
					CStCommon::GetIniFilePath( szPath, _countof(szPath) );
					mSaveFilePath = szPath;
				}
				else
				{
					mSaveFilePath = szArgument;
				}
				if( mSaveFilePath.Right(1) ==_T("\\") || mSaveFilePath.Right(1) ==_T("/") )
					mSaveFilePath = mSaveFilePath.Left(mSaveFilePath.GetLength()-1);

				bReval = TRUE;
			}
			else
			if( szSection==_T("messagebox") )
			{
				//��1.0.0.1062
				if( m_nScriptCheckMode==0 )
				{
				//��1.0.0.1062
				HWND hWnd = NULL;
				if( mpParent )
				{
					//�ʒu�̎w�肪�ł��Ȃ�
					//CWnd *pWnd = (CWnd *)mpParent;
					CStGenFactoryScriptDlg *pWnd = (CStGenFactoryScriptDlg *)mpParent;
					pWnd->MessageScript(szArgument, MB_OK);
					//hWnd = pWnd->m_hWnd;
				}
				else
				{
					AfxMessageBox( szArgument, MB_OK );
				}
				//��1.0.0.1062
				}
				//��1.0.0.1062
				bReval = TRUE;
			}
			//��1.0.0.1062
			//else
			//if( szSection==_T("connect") )
			//{
			//	bReval = ConnectEBus( &mCustomDevice, szArgument);
			//	if( !bReval )
			//	{
			//		szErrorMessage = _T("�J�����ڑ��G���[");
			//	}

			//}
			//else
			//if( szSection==_T("disconnect") )
			//{

			//	bReval = DisonnectEBus( &mCustomDevice );
			//	if( !bReval )
			//	{
			//		szErrorMessage = _T("�J�����ؒf�G���[");
			//	}
			//}
			//else
			//if( szSection==_T("start") )
			//{
			//	bReval = StartEBus( &mCustomDevice );
			//	if( !bReval )
			//	{
			//		szErrorMessage = _T("�J�����X�^�[�g�G���[");
			//	}
			//}
			//else
			//if( szSection==_T("stop") )
			//{
			//	bReval = StopEBus( &mCustomDevice );
			//	if( !bReval )
			//	{
			//		szErrorMessage = _T("�J�����X�g�b�v�G���[");
			//	}
			//}
			//��1.0.0.1062
			else
			if( szSection==_T("sleep") )
			{
				//��1.0.0.1062
				if( m_nScriptCheckMode==0 )
				{
				//��1.0.0.1062
				//��1.0.0.1015
				//Sleep(_tcstol(szArgument,NULL,0));
				//�����ŃX���[�v������Ɖf����ImageMessage������ɕ\������Ȃ����߁A
				//�X���b�h�쐬�����̒��ŃX���[�v������B
				INT nTimer = _tcstol(szArgument,NULL,0);
				unsigned int aID = NULL; 
				HANDLE hThread = NULL;
				//Sleep�X���b�h
				hThread = (HANDLE)_beginthreadex( NULL, 0, sleepThread, &nTimer,  0, &aID );
				DWORD lRetVal = ::WaitForSingleObject(hThread,INFINITE);
				::CloseHandle( hThread );
				//��1.0.0.1015
				//��1.0.0.1062
				}
				//��1.0.0.1062
				bReval = TRUE;
			}
			else
			if( szSection==_T("print") )
			{
				//��1.0.0.1062
				if( m_nScriptCheckMode==0 )
				{
				//��1.0.0.1062
				//��1.0.0.1033
				//OutputLogFile(szArgument);
				CString szArgumentTmp = szArgument;
				if( szArgumentTmp.Left(1)==_T("\"") && szArgumentTmp.Left(1)==_T("\"") )
				{
					szArgumentTmp = szArgumentTmp.Mid(1);
					szArgumentTmp = szArgumentTmp.Left(szArgumentTmp.GetLength()-1);
				}
				OutputLogFile(szArgumentTmp);
				//��1.0.0.1033
				//��1.0.0.1062
				}
				//��1.0.0.1062
				bReval = TRUE;
			}
			else
			if( szSection==_T("logfilename") )
			{

				//���g�p
				//Sleep(_ttoi(szArgument));
				//return TRUE;
			}

			else
			if( szSection==_T("beep") )
			{
				INT nCommaPos = szArgument.Find(_T(","));
				if( nCommaPos>0 )
				{
					INT nFleq = _ttoi(szArgument.Left(nCommaPos));
					INT nDuration = _ttoi(szArgument.Mid(nCommaPos+1));
					if( nFleq>0 && nDuration>0 )
					{
						//��1.0.0.1062
						if( m_nScriptCheckMode==0 )
						{
						//��1.0.0.1062
						Beep(nFleq, nDuration);
						//��1.0.0.1062
						}
						//��1.0.0.1062
						bReval = TRUE;
					}
				}
			}

			else
			if( szSection==_T("savebitmap") )
			{
				//StCheckCommon aCheckBase;


				//INT nCommaPos = szArgument.Find(_T(","));
				//if( nCommaPos>0 )
				//{
				//	INT nFleq = _ttoi(szArgument.Left(nCommaPos));
				//	INT nDuration = _ttoi(szArgument.Mid(nCommaPos+1));
				//	if( nFleq>0 && nDuration>0 )
				//	{
				//		Beep(nFleq, nDuration);
				//		bReval = TRUE;
				//	}
				//}
			}

		
			else
			if( szSection==_T("writescriptlogmode") )
			{
				INT nCurrentWriteScriptLogMode = m_nWriteScriptLogMode;
				m_nWriteScriptLogMode = _tcstol(szArgument,NULL,0);
				if( m_nWriteScriptLogMode==1 && nCurrentWriteScriptLogMode==0 )
				{
					//Off -> On�̎�����Log�L�q�������ōs��
					//��1.0.0.1062
					if( m_nScriptCheckMode==0 )
					//��1.0.0.1062
					OutputLogFile(szString);
				}
				bReval = TRUE;
			}

			//��1.0.0.1008
			else
			if( szSection==_T("retrymode") )
			{
				m_nRetryMode = _tcstol(szArgument,NULL,0);
				bReval = TRUE;
			}
			else
			if( szSection==_T("ngcontinuousmode") )
			{
				m_nNGContinuousMode = _tcstol(szArgument,NULL,0);
				bReval = TRUE;
			}
			//��1.0.0.1008

			//��1.0.0.1012
			else
			if( szSection==_T("ngendprocessregister") )
			{
				m_vNGEndProcessString.push_back(szArgument);
				bReval = TRUE;
			}
			//��1.0.0.1012


			//��1.0.0.1013
			else
			if( szSection==_T("nookmessage") )
			{
				m_nNoOKMessage = _tcstol(szArgument,NULL,0);
				bReval = TRUE;
			}
			else
			if( szSection==_T("nongmessage") )
			{
				m_nNoNGMessage = _tcstol(szArgument,NULL,0);
				bReval = TRUE;
			}
			//��1.0.0.1013

			//��1.0.0.1047
			else
			if( szSection==_T("ruledline") || szSection==_T("setruledline") )
			{
				m_nRuledLine = _tcstol(szArgument,NULL,0);

				//��1.0.0.1062
				if( m_nScriptCheckMode==0 )
				{
				//��1.0.0.1062
				CStGenFactoryScriptDlg *pWnd = (CStGenFactoryScriptDlg *)mpParent;
				if( pWnd )
				{
					CWaveformDlg *aWaveform = (CWaveformDlg *)pWnd->GetWaveform();
					if( aWaveform )
					{
						aWaveform->SetRuledLine(m_nRuledLine);
					}
				}
				//��1.0.0.1062
				}
				//��1.0.0.1062
				bReval = TRUE;
			}
			//��1.0.0.1047
			//��1.0.0.1048
			else
			if( szSection==_T("displaypositionx") )
			{
				//��1.0.0.1062
				if( m_nScriptCheckMode==0 )
				{
				//��1.0.0.1062
				CStGenFactoryScriptDlg *pWnd = (CStGenFactoryScriptDlg *)mpParent;
				if( pWnd )
				{
					CPreview *aDisplayWnd = (CPreview *)pWnd->GetPreview();
					if( aDisplayWnd )
					{
						INT nMax=0, nMin=0;
						aDisplayWnd->GetScrollRange(SB_HORZ, &nMin, &nMax);
						INT nValue = _tcstol(szArgument,NULL,0);
						nValue = nValue<nMin?nMin:(nValue>nMax?nMax:nValue);
						aDisplayWnd->SetScrollPos(SB_HORZ,nValue);
						bReval = TRUE;
					}
				}
				//��1.0.0.1062
				}else bReval = TRUE;
				//��1.0.0.1062
			}
			else
			if( szSection==_T("displaypositiony") )
			{
				//��1.0.0.1062
				if( m_nScriptCheckMode==0 )
				{
				//��1.0.0.1062
				CStGenFactoryScriptDlg *pWnd = (CStGenFactoryScriptDlg *)mpParent;
				if( pWnd )
				{
					CPreview *aDisplayWnd = (CPreview *)pWnd->GetPreview();
					if( aDisplayWnd )
					{
						INT nMax=0, nMin=0;
						aDisplayWnd->GetScrollRange(SB_VERT, &nMin, &nMax);
						INT nValue = _tcstol(szArgument,NULL,0);
						nValue = nValue<nMin?nMin:(nValue>nMax?nMax:nValue);
						aDisplayWnd->SetScrollPos(SB_VERT,nValue);
						bReval = TRUE;
					}
				}
				//��1.0.0.1062
				}else bReval = TRUE;
				//��1.0.0.1062
			}
			else
			if( szSection==_T("displayratio") )
			{
				//��1.0.0.1062
				if( m_nScriptCheckMode==0 )
				{
				//��1.0.0.1062
				CStGenFactoryScriptDlg *pWnd = (CStGenFactoryScriptDlg *)mpParent;
				if( pWnd )
				{
					CPreview *aDisplayWnd = (CPreview *)pWnd->GetPreview();
					if( aDisplayWnd )
					{
						INT nMagnify = _tcstol(szArgument,NULL,0);
						aDisplayWnd->SetMagnify(nMagnify-3);
						bReval = TRUE;
					}
				}
				//��1.0.0.1062
				}else bReval = TRUE;
				//��1.0.0.1062
			}
			//��1.0.0.1048

			//else
			//{
			//	szErrorMessage = _T("�X�N���v�g�G���[");
			//}
		}
		//else
		//{
		//	szErrorMessage = _T("�X�N���v�g�G���[");
		//}

	}while(0);
	return bReval;
}

//��1.0.0.1023 �J�Ԃ��p�ϐ��ǉ�
//BOOL StInspectMain::ExecuteValueFunction(CString &szString)
//{
//	BOOL bReval = FALSE;
//	do
//	{
//		szString.Trim();	//�ŏ���Trim����B
//
////		StValue
//
//
//
//	}while(0);
//	return bReval;
//}
//��1.0.0.1023 �J�Ԃ��p�ϐ��ǉ�




BOOL StInspectMain::SepareteString(CString szInputString, CString &szClass, CString &szVariable, CString &szArgument )
{
	BOOL bReval = FALSE;
	do
	{
		INT iPos = szInputString.Find(_T("."));
		if( iPos<0 ) break;
		szClass = szInputString.Left(iPos);
		szClass.Trim();

		CString szTmp = szInputString.Mid( iPos + 1 );
		szTmp.Trim();
		szVariable = szTmp;
		szArgument = _T("");
		bReval = SepareteString(szTmp, szVariable, szArgument );
	}while(0);
	return bReval;
}
//AAA(BBB) AAA=BBB�̕������AAA��BBB�ɕ���������
//BBB�ɂ�()=�̕����񂪓����Ă���ꍇ������BAAA�ɂ͂Ȃ�
BOOL StInspectMain::SepareteString(CString &szInputString, CString &szVariable, CString &szArgument )
{
	BOOL bReval = FALSE;

	szInputString.Trim();
	int iEqualPos=szInputString.Find(_T("="));
	int iParentPos=szInputString.Find(_T("("));
	if( iEqualPos<0 && iParentPos<0 )
	{
		szVariable = szInputString;
		szArgument = _T("");
		bReval = TRUE;
	}
	else
	if( iParentPos<0 || (iEqualPos>0 && iEqualPos<iParentPos ) )
	{
		//("=")�̏ꍇ
		szVariable = szInputString.Left(iEqualPos);
		szVariable.Trim();
		szArgument = szInputString.Mid(iEqualPos+1);
		szArgument.Trim();
		bReval = TRUE;

	}
	else
	if( iEqualPos<0 || (iParentPos>0 && iParentPos<iEqualPos) )
	{
		//("(")�̏ꍇ
		szVariable = szInputString.Left(iParentPos);
		szVariable.Trim();
		szArgument = szInputString.Mid(iParentPos+1);
		INT iReversePos = szArgument.ReverseFind(')');
		if( iReversePos>0 )
		{
			szArgument = szArgument.Left(iReversePos);
			szArgument.Trim();
			bReval = TRUE;
		}

		//��1.0.0.1015
		else
		if( iReversePos==0 )
		{
			szArgument = "";
			bReval = TRUE;
		}
		//��1.0.0.1015
	}

	return bReval;
}


DWORD StInspectMain::Function()
{
	//��1.0.0.1062
	m_vScriptCheckErrorText.clear();
	//��1.0.0.1062


	//��1.0.0.1072a
	m_vScriptFilename.clear();
	m_vScriptLineIndex.clear();
	m_vScriptFileInfos.clear();
	//��1.0.0.1072a


//OutputDebugString(_T("ScriptFileLoad Start\n"));
	//��1.0.0.1069
	//BOOL bReval = ScriptFileLoad();
	BOOL bReval;
	if( mszScriptFileName.GetLength()>0 )
	{
		bReval = ScriptFileLoad();
	}
	else
	{
		bReval = ScriptModelFileLoad();
	}
	//��1.0.0.1069
	if( !bReval )
	{
		//��1.0.0.1069
		AfxMessageBox( _T("File not found."), MB_OK );
		return ERROR_FILE_NOT_FOUND;
		//��1.0.0.1069
	}
	//CString szObject1;
	//CString szObject2;
//OutputDebugString(_T("ScriptFileLoad End\n"));

	CStGenFactoryScriptDlg *pWnd = (CStGenFactoryScriptDlg *)mpParent;

//OutputDebugString(_T("ScriptFileLoad pWnd\n"));

	//��1.0.0.1026
	m_nNextLine = -1;
	//��1.0.0.1026
	//��1.0.0.1072
	INT nCheckListBoxIndex = -1;
	//��1.0.0.1072

	//��1.0.0.1008
	BOOL bAllReval = TRUE;
	//for( int i=0; i<(int)m_vScriptString.size() && bReval; i++ )
	for( int i=0; i<(int)m_vScriptString.size(); i++ )
	//��1.0.0.1008
	{
//OutputDebugString(_T("GetNextLine Start\n"));
		//��1.0.0.1023 �J�Ԃ��p�ǉ�
		INT nNextLine = GetNextLine();
//{
//CString szString;
//szString.Format(_T("GetNextLine End NextLine(%i) \n"),nNextLine);
//OutputDebugString(szString);
//}
		if( nNextLine>=0 ) i=nNextLine;
		//��1.0.0.1023 �J�Ԃ��p�ǉ�
//{
//OutputDebugString(m_vScriptString[i]);
//OutputDebugString(_T("\n"));
//}

		pWnd->DisplayListBox(m_vScriptString[i]);

		//��1.0.0.1072
		if( m_vScriptCheckLineIndex[i]>=0 && m_vScriptCheckLineIndex[i]!=nCheckListBoxIndex )
		{
			pWnd->SetListBoxCurSel(m_vScriptCheckLineIndex[i]);
			nCheckListBoxIndex = m_vScriptCheckLineIndex[i];
		}
		//��1.0.0.1072

		//��1.0.0.1068
		//������DoEvents�͏d�v
		DoEvents();
		//��1.0.0.1068

//TRACE(m_vScriptString[i]);
//TRACE(_T("\n"));

		//��1.0.0.1062
		//if( m_nWriteScriptLogMode )
		if( m_nWriteScriptLogMode && m_nScriptCheckMode==0 )
		//��1.0.0.1026
		{
			//print�ȊO�A������LOG���L�q����

			//��1.0.0.1027
			//if( m_vScriptString[i].Left(5)!=_T("print") )
			CString szScript = m_vScriptString[i].Left(5);
			szScript.MakeLower();
			if( szScript!=_T("print") )
			//��1.0.0.1027
			{
				OutputLogFile(m_vScriptString[i]);
			}
		}

//TRACE(TEXT("@@@@@@@@@ IN  m_vScriptString[%d]=%s\n"), i, m_vScriptString[i] );
////if( m_vScriptString[i]==_T("aValue = aTerminal.GetLightValue(1)") )
//if( m_vScriptString[i]==_T("aInputSensorVersionDlg.AddCheckVersion=aCheckVersion0") )
//Sleep(1);

//if( m_vScriptString[i]==_T("ksData = aDevice.ExposureTime") )
//{
//	Sleep(1);
//}

		//LOG�o��-----------------------------
		//�N���X�錾 or delete ���ǂ���
		//��1.0.0.1072d
#ifdef GENCOMP_OLD
		bReval = ExecuteDeclaration(m_vScriptString[i]);
#else
		BOOL bGenICamControlCheck=TRUE;
		bReval = ExecuteDeclaration(m_vScriptString[i],&bGenICamControlCheck);
#endif
		//��1.0.0.1072d
		if( !bReval )
		{
			//�N���X�̎��s���ǂ���
			//��1.0.0.1023
			//bReval = ExecuteClassFunction(m_vScriptString[i]);
			bReval = ExecuteClassFunction(m_vScriptString[i],i);
			//��1.0.0.1023
		}
		if( !bReval )
		{
			bReval = ExecuteFunction(m_vScriptString[i]);
		}
		//��1.0.0.1023 �J�Ԃ��p�ϐ��ǉ�
		//if( !bReval )
		//{
		//	bReval = ExecuteValueFunction(m_vScriptString[i]);
		//}
		//��1.0.0.1023 �J�Ԃ��p�ϐ��ǉ�
		//��1.0.0.1072d
#ifdef GENCOMP_OLD
#else
		if( bReval && !bGenICamControlCheck )
		{
			if( szErrorMessage.GetLength()==0 )
			{
				szErrorMessage = _T("GenICam�ʐM��r�G���[");
			}

			//���O�ɕۑ�
			CString szText = _T("Check=");
			OutputLogFile(szText + (bGenICamControlCheck?_T("OK"):_T("NG")));
			if( m_nResultGenICamControlValueMode==0 )
			{
				if( m_dblResultGenICamControlValue-(double)(INT)m_dblResultGenICamControlValue>0.0 )
					szText.Format(_T("%s=%f"),m_szResultGenICamControlCommandName.GetUnicode(), m_dblResultGenICamControlValue );
				else
					szText.Format(_T("%s=%i"),m_szResultGenICamControlCommandName.GetUnicode(), (INT)m_dblResultGenICamControlValue );
			}
			else
				szText.Format(_T("%s=%s"),m_szResultGenICamControlCommandName.GetUnicode(),m_szResultGenICamControlValue.GetUnicode() );
			OutputLogFile(szText);

			bReval = FALSE;
		}
#endif
		//��1.0.0.1072d
		if( !bReval )
		{

			//��1.0.0.1062
			if( m_nScriptCheckMode==0 )
			{
			//��1.0.0.1026

			if( szErrorMessage.GetLength()==0 )
			{
				szErrorMessage = _T("�X�N���v�g�G���[");
			}

			//��1.0.0.1008
			bAllReval = FALSE;
			if( m_nNGContinuousMode==0 )
			{
				break;
			}
			//��1.0.0.1008

			//��1.0.0.1062
			}
			else
			{

				//Print
				UINT nIndex = m_vScriptLineIndex[i];
				UINT nFileIndex =  nIndex>>24;
				nIndex &= 0xFFFFFF;
				CString szFileName = _T("");
				if(nFileIndex<m_vScriptFilename.size() )
					szFileName = m_vScriptFilename[nFileIndex];
				CString szText;
				szText.Format(_T("File[%s] Line(%i) Text[%s]"),szFileName,nIndex+1,m_vScriptString[i]);
				m_vScriptCheckErrorText.push_back(szText);
//TRACE(TEXT("Error File[%s] Line(%i) Text[%s]\n"),szFileName,nIndex,m_vScriptString[i]);
			}
			//��1.0.0.1062
		}
	}

	//��1.0.0.1062
	//if( m_vScriptCheckErrorText.size()>0 )
	//{
	//	if( !m_pErrorListDlg )
	//	{
	//		m_pErrorListDlg = new CErrorListDlg;
	//	}
	//
	//	if( m_pErrorListDlg && !m_pErrorListDlg->m_hWnd )
	//	{
	//		m_pErrorListDlg->Create( CErrorListDlg::IDD, NULL );
	//		RECT rc1;
	//		m_pErrorListDlg->GetWindowRect(&rc1);
	//		//INT nPosX = m_pErrorListDlg->GetWindowPosX();
	//		INT nPosX = 50;
	//		if( nPosX<0 )	nPosX = rc1.left;
	//		//INT nPosY = m_pErrorListDlg->GetWindowPosY();
	//		INT nPosY = 50;
	//		if( nPosY<0 )	nPosY = rc1.top;
	//		m_pErrorListDlg->SetWindowPos( NULL, nPosX, nPosY, rc1.right-rc1.left, rc1.bottom-rc1.top, SWP_NOZORDER );
	//	}
	//	if( m_pErrorListDlg )
	//	{
	//		m_pErrorListDlg->ClearStringList();
	//		for( int i=0; i<m_vScriptCheckErrorText.size(); i++ )
	//		{
	//			m_pErrorListDlg->SetStringList(m_vScriptCheckErrorText[i]);
	//		}
	//		m_pErrorListDlg->ShowWindow(SW_SHOW);
	//		DoEvents();
	//	}
	//}
	//��1.0.0.1062


	//��1.0.0.1012 NG�I�����Ɏ��s����R�}���h
	//��1.0.0.1062
	//if( !bAllReval )
	if( !bAllReval &&  m_nScriptCheckMode==0 )
	//��1.0.0.1062
	{
		for( int i=0; i<(int)m_vNGEndProcessString.size(); i++ )
		{
			pWnd->DisplayListBox(m_vNGEndProcessString[i]);
			//NG�ł�����
			bReval = ExecuteDeclaration(m_vNGEndProcessString[i]);
			if( !bReval )
			{
				bReval = ExecuteClassFunction(m_vNGEndProcessString[i]);
			}
			if( !bReval )
			{
				bReval = ExecuteFunction(m_vNGEndProcessString[i]);
			}
			//��1.0.0.1023 �J�Ԃ��p�ϐ��ǉ�
			//if( !bReval )
			//{
			//	bReval = ExecuteValueFunction(m_vScriptString[i]);
			//}
			//��1.0.0.1023 �J�Ԃ��p�ϐ��ǉ�

		}
		m_vNGEndProcessString.clear();
	}
	//��1.0.0.1012

	//Function���ɂ��ׂĂ��폜���Ă���
	//�f�X�g���N�^�ł����Ȃ��ƃG���[��������B�����炭�X���b�h������������B
	m_vScriptString.clear();
	AllDelete();



	//��1.0.0.1062
	if( m_nScriptCheckMode==0 )
	{
	//��1.0.0.1062

	//��1.0.0.1015
	if( bAllReval )
	{
		LogFileNameDeleteNG();
	}
	//��1.0.0.1015

	if( szErrorMessage.GetLength()>0 )
	{
		//��1.0.0.1013
		if( m_nNoNGMessage==0 )
		//��1.0.0.1013
		//��1.0.0.1031
			//AfxMessageBox(szErrorMessage);
			AfxMessageBox(szErrorMessage, MB_ICONERROR);
		//��1.0.0.1031
	}
	else
	{
		//��1.0.0.1013
		if( m_nNoOKMessage==0 )
		//��1.0.0.1013
		//��1.0.0.1031
			//AfxMessageBox(_T("����I��"));
			AfxMessageBox(_T("����I��"), MB_ICONINFORMATION );
		//��1.0.0.1031
	}

	//��1.0.0.1062
	}
	else
	{
		if( m_vScriptCheckErrorText.size()==0 )
		{
			if( m_nNoOKMessage==0 )
				AfxMessageBox(_T("����I��"), MB_ICONINFORMATION );
		}
	}
	//��1.0.0.1062

	//��1.0.0.1008
	//return bReval;
	return bAllReval;
	//��1.0.0.1008

}

void StInspectMain::DisplayListBox(LPCTSTR szText)
{
	CStGenFactoryScriptDlg *pWnd = (CStGenFactoryScriptDlg *)mpParent;
	pWnd->DisplayListBox(szText);
	//��1.0.0.1068
	DoEvents();
	//��1.0.0.1068
}

//#ifdef _PLEORA
PvDeviceInfo * StInspectMain::DiscoveryDeviceInfo( PvSystem &aSystem, LPCTSTR aCameraTypeName, INT nTimeout, StDeviceEBus *aDeviceEBus)
{
	PvDeviceInfo *pDeviceInfo = NULL;
	INT iEachTimeout=2000;
	//2Sec�Ԋu��(nTimeout/2000)��
	INT iSearchCount = nTimeout/iEachTimeout;

	do
	{
		aSystem.SetDetectionTimeout(iEachTimeout);
		aSystem.Find();
		PvUInt32 aInterfaceCount = aSystem.GetInterfaceCount();
		for( PvUInt32 i=0; i<aInterfaceCount && pDeviceInfo==NULL; i++ )
		{
			PvInterface *aInterface = aSystem.GetInterface(i);
			PvUInt32 aDeviceCount = aInterface->GetDeviceCount();
			for( PvUInt32 j=0; j<aDeviceCount && pDeviceInfo==NULL; j++ )
			{
				PvDeviceInfo *aDeviceInfo = aInterface->GetDeviceInfo(j);
				if( aDeviceEBus )
				{
					PvString szMACAddress;
					aDeviceEBus->GetMACAddressStr(szMACAddress);
					if( aDeviceInfo->GetMACAddress()==szMACAddress && aDeviceInfo->GetAccessStatus()!=PvAccessType::PvAccessOpen )
					{
						PvString szIPAddress;
						aDeviceEBus->GetIPAddressStr(szIPAddress);
						PvString szSubnetMask;
						aDeviceEBus->GetSubnetMaskStr(szSubnetMask);
						PvDevice lDevice;
						lDevice.SetIPConfiguration(szMACAddress,szIPAddress ,szSubnetMask ,_T("0.0.0.0") );
						Sleep(1000);
					}
				}
				if( aDeviceInfo->GetModel()==aCameraTypeName && aDeviceInfo->GetAccessStatus()==PvAccessType::PvAccessOpen )
				{
					pDeviceInfo = aDeviceInfo;
				}
			}
		}
		iSearchCount--;
	}while(iSearchCount>0 && pDeviceInfo==NULL );
	//FindDevice�͂Ȃ������o�ł��Ȃ��B�B
	//PvResult aResult = aSystem.FindDevice( aCameraTypeName, &pDeviceInfo);
	//if( aResult.IsFailure() ) return NULL;
	return pDeviceInfo;
}

//#else
//StGenDeviceInfo * StInspectMain::DiscoveryDeviceInfo( StGenDeviceManager &aDeviceManager, CCameraType * aCameraType, INT nTimeout, StGenInterface **aInterface)
//{
//
//	StString aCameraTypeName = "";
//	if( aCameraType )
//		aCameraTypeName = aCameraType->GetCameraTypeName();
//
//	StGenDeviceInfo *pTargetGenDeviceInfo=NULL;
//	int iCameraNum=0;
//	//StGenDeviceManager aDeviceManager;
//	StTimed aTime;
//	do
//	{
//		aDeviceManager.UpdateList();
//		int iGenTLSize = aDeviceManager.GetGenTLSize();
//		for( int i=0; i<iGenTLSize && pTargetGenDeviceInfo==NULL; i++ )
//		{
//			StGenTL *pGenTL = (StGenTL *)aDeviceManager.GetGenTL(i);
//			StString aName = pGenTL->GetName();
//			aName = pGenTL->GetDisplayName();
//
//			StInt32 iNumInterface = pGenTL->GetNumInterfaces();
//			for( int j=0; j<iNumInterface && pTargetGenDeviceInfo==NULL; j++ )
//			{
//				StGenInterface *pGenIF = pGenTL->GetGenInterface(j);
//
//				StString bName = pGenIF->GetDisplayName();
//			
//				StInt32 iNumDeviceInfo = pGenIF->GetNumDeviceInfo();
//				for( int k=0; k<iNumDeviceInfo && pTargetGenDeviceInfo==NULL; k++ )
//				{
//					StGenDeviceInfo *pGenDeviceInfo = pGenIF->GetGenDeviceInfo(k);
//					//StString cName = pGenDeviceInfo->GetDisplayName();
//					//StString dName = pGenDeviceInfo->GetVendor();
//					StString modelName = pGenDeviceInfo->GetModel();
//
//					//Euresys CoaX�̃{�[�h���}�����Ă���ƃJ�������ڑ�����Ă��Ȃ��Ă�
//					//Device�ƔF�������B���̏ꍇmodelName��""�ƂȂ��Ă���B
//					if( modelName.GetLength()>0 )
//					{
//						if( aCameraTypeName.GetLength()==0 || modelName==aCameraTypeName )
//						{
//							pTargetGenDeviceInfo=pGenDeviceInfo;
//							if( aInterface ) *aInterface = pGenIF;
//							iCameraNum++;
//						}
//					}
//				}
//			}
//		}
//		if( aTime.GetTime()>nTimeout || pTargetGenDeviceInfo ) break;
//		Sleep(1000);
//	}while(pTargetGenDeviceInfo==NULL);
//	return pTargetGenDeviceInfo;
//}
//
//#endif


typedef struct _DISPLAY_INFO
{
	CPreview *aDisplayWnd;
	CWaveformDlg *aWaveform;
	StBuffer *aBuffer;
	//��1.0.0.1009 �r�������̈�
	HANDLE aDisplayBufferMutex;
	//��1.0.0.1009
}DISPLAY_INFO, *PDISPLAY_INFO;

unsigned __stdcall displayThread( void *pParam )
{
	PDISPLAY_INFO aDisplayInfo = (PDISPLAY_INFO)pParam;
	if( aDisplayInfo )
	{
		//��1.0.0.1067a
		//::WaitForSingleObject( aDisplayInfo->aDisplayBufferMutex, INFINITE );
		//if( aDisplayInfo->aDisplayWnd )
		//	aDisplayInfo->aDisplayWnd->StGenImageDisplay(aDisplayInfo->aBuffer);
		//if( aDisplayInfo->aWaveform )
		//	aDisplayInfo->aWaveform->StWaveformDisplay(aDisplayInfo->aBuffer);
		//::ReleaseMutex(aDisplayInfo->aDisplayBufferMutex);

		if( ::WaitForSingleObject( aDisplayInfo->aDisplayBufferMutex, 0 )==WAIT_OBJECT_0 )
		{
			if( aDisplayInfo->aDisplayWnd )
				aDisplayInfo->aDisplayWnd->StGenImageDisplay(aDisplayInfo->aBuffer);
			if( aDisplayInfo->aWaveform )
				aDisplayInfo->aWaveform->StWaveformDisplay(aDisplayInfo->aBuffer);
			::ReleaseMutex(aDisplayInfo->aDisplayBufferMutex);
		}



		//��1.0.0.1067a
	}
	_endthreadex(0);
	return 0;

}

void StInspectMain::ImageCallback(StBuffer *aBuffer, void *pValue)
{
	//��������ѕ\��---------Thread�쐬���ĕ\������Bmessage�\��������ƃ{�^���������Ȃ��Ȃ邩��B�B�B
	CStGenFactoryScriptDlg *pWnd = (CStGenFactoryScriptDlg *)mpParent;
	CPreview *aDisplayWnd = (CPreview *)pWnd->GetPreview();
	CWaveformDlg *aWaveform = (CWaveformDlg *)pWnd->GetWaveform();
	//mDisplayWnd->StGenImageDisplay(aBuffer);

//StPixelType aType;
//aType = aBuffer->GetPixelType();
//PBYTE aData = (PBYTE)aBuffer->GetDataPointer();
//TRACE( _T("**ImageCallback aType=0x%x\n"), aType );
//TRACE( _T("**ImageCallback aData=%d\n"), *(PWORD)aData );

	DISPLAY_INFO aDisplayInfo;
	aDisplayInfo.aDisplayWnd = aDisplayWnd;
	aDisplayInfo.aWaveform = aWaveform;

	//��1.0.0.1009 �r�������̈�
	//��aBuffer���펞�g�p���Ȃ�...mDisplayBuffer�ɃR�s�[����B
	::WaitForSingleObject( m_DisplayBufferMutex, INFINITE );
	//��1.0.0.1011�s��C��
	m_DisplayBuffer.Free();	//��������Ȃ���CreateCopy�ł��Ȃ�...
	//��1.0.0.1011�s��C��

	StResult aResult = m_DisplayBuffer.CreateCopy(*aBuffer);

	//��1.0.0.1046
	if( m_DisplayBuffer.GetHeight()==0 )
	{
		m_DisplayBuffer.SetHeight(m_DisplayBuffer.GetDeliveredImageHeight());
	}
	//��1.0.0.1046

	//TRACE(TEXT(" CreateCopy(%s)\n"),aResult.GetCodeString().GetUnicode());
//
//PBYTE pbInBuffer = (PBYTE)aBuffer->GetDataPointer();
//PBYTE pbBuffer = (PBYTE)m_DisplayBuffer.GetDataPointer();
//TRACE(TEXT("IN Buffer(0x%02X:0x%02X:0x%02X:0x%02X:)\n"),pbInBuffer[0],pbInBuffer[10000],pbInBuffer[20000],pbInBuffer[30000]);
//TRACE(TEXT("Out Buffer(0x%02X:0x%02X:0x%02X:0x%02X:)\n"),pbBuffer[0],pbBuffer[10000],pbBuffer[20000],pbBuffer[30000]);
	::ReleaseMutex( m_DisplayBufferMutex );
	//aDisplayInfo.aBuffer = aBuffer;
	aDisplayInfo.aBuffer = &m_DisplayBuffer;
	aDisplayInfo.aDisplayBufferMutex = m_DisplayBufferMutex;
	//��1.0.0.1009

	unsigned int aID[2] = {NULL,NULL}; 
	HANDLE hThread[2] = {NULL,NULL};
	//�\���X���b�h

	hThread[0] = (HANDLE)_beginthreadex( NULL, 0, displayThread, &aDisplayInfo,  0, &aID[0] );

	//�����X���b�h
	//hThread[1] = (HANDLE)_beginthreadex( NULL, 0, inspectionThread, &aDisplayInfo,  0, &aID[1] );

	//DWORD lRetVal = ::WaitForMultipleObjects(1,hThread,TRUE,INFINITE);
	DWORD lRetVal = ::WaitForSingleObject(hThread[0],INFINITE);
	::CloseHandle( hThread[0] );

	//mDisplay.Display( *aBuffer );

}
//------------------------------------------------------------------------
//
//------------------------------------------------------------------------
BOOL StInspectMain::DeleteDeviceBase(INT nDeviceBaseIndex)
{
	if( nDeviceBaseIndex>=(INT)m_vecDeviceBase.size() ) return FALSE;

	StDeviceBase *aDeviceBase = m_vecDeviceBase[nDeviceBaseIndex];
	BOOL bReval = FALSE;
	if( aDeviceBase->m_ClassName==_StDeviceEBus )
	{
		StDeviceEBus *pDevice = (StDeviceEBus *)aDeviceBase;
		delete pDevice;
		bReval = TRUE;
	}
	else
	if( aDeviceBase->m_ClassName==_StDeviceMultiCam )
	{
		StDeviceMultiCam *pDevice = (StDeviceMultiCam *)aDeviceBase;
		delete pDevice;
		bReval = TRUE;
	}
	//��1.0.0.1044
	else
	if( aDeviceBase->m_ClassName==_StDeviceGenICam )
	{
		StDeviceGenICam *pDevice = (StDeviceGenICam *)aDeviceBase;
		delete pDevice;
		bReval = TRUE;
	}
	//��1.0.0.1044

	if( bReval )
		m_vecDeviceBase.erase(m_vecDeviceBase.begin() + nDeviceBaseIndex );

	return bReval;
}

BOOL StInspectMain::DeleteSerialComm(INT nSerialCommIndex)
{
	//��1.0.0.1020
	//if( nSerialCommIndex>=(INT)m_vecSerialComm.size() ) return FALSE;
	if( nSerialCommIndex>=(INT)m_vecControl.size() ) return FALSE;
	//��1.0.0.1020

	//��1.0.0.1017
	//StSerialComm *aSerialComm = m_vecSerialComm[nSerialCommIndex];
	//delete aSerialComm;
	//m_vecSerialComm.erase(m_vecSerialComm.begin() + nSerialCommIndex );
	//return TRUE;

	BOOL bReval = FALSE;
	//��1.0.0.1020
	//StControl *aControl = m_vecSerialComm[nSerialCommIndex];
	StControl *aControl = m_vecControl[nSerialCommIndex];
	//��1.0.0.1020
	if( aControl->m_ClassName==_StSerialComm )
	{
		//��1.0.0.1020
		//StSerialComm *pControl = (StSerialComm *)aControl;
		StSerialComm *pControl = (StSerialComm *)aControl->GetMainHandle();
		//��1.0.0.1020
		delete pControl;
		bReval = TRUE;
	}
	else
	if( aControl->m_ClassName==_StTerminal )
	{
		//��1.0.0.1020
		//StTerminal *pControl = (StTerminal *)aControl;
		StTerminal *pControl = (StTerminal *)aControl->GetMainHandle();
		//��1.0.0.1020
		delete pControl;
		bReval = TRUE;
	}
	//��1.0.0.1020
	//m_vecSerialComm.erase(m_vecSerialComm.begin() + nSerialCommIndex );
	m_vecControl.erase(m_vecControl.begin() + nSerialCommIndex );
	//��1.0.0.1020
	return bReval;
	//��1.0.0.1017
}

BOOL StInspectMain::DeleteTexioControl(INT nTexioControlIndex)
{
	if( nTexioControlIndex>=(INT)m_vecTexioControl.size() ) return FALSE;

	StTexioControl *aTexioControl = m_vecTexioControl[nTexioControlIndex];
	delete aTexioControl;
	m_vecTexioControl.erase(m_vecTexioControl.begin() + nTexioControlIndex );

	return TRUE;
}

BOOL StInspectMain::DeleteCheckBase(INT nCheckBaseIndex)
{
	if( nCheckBaseIndex>=(INT)m_vecCheckBase.size() ) return FALSE;

	StCheckBase *aCheckBase = m_vecCheckBase[nCheckBaseIndex];
	if( aCheckBase->m_ClassName==_StCheckSN )
	{
		StCheckSN *pCheck = (StCheckSN *)aCheckBase;
		delete pCheck;
	}
	else
	if( aCheckBase->m_ClassName==_StCheckControlRetry )
	{
		StCheckControlRetry *pCheck = (StCheckControlRetry *)aCheckBase;
		delete pCheck;
	}
	else
	if( aCheckBase->m_ClassName==_StInputSerialDlg )
	{
		//StInputSerialDlg *pCheck = (StInputSerialDlg *)aCheckBase;
		StInputSerialDlg *pCheck = (StInputSerialDlg *)aCheckBase->GetMainHandle();
		delete pCheck;
	}
	else
	if( aCheckBase->m_ClassName==_StCheckSNDlg )
	{
		StCheckSNDlg *pCheck = (StCheckSNDlg *)aCheckBase->GetMainHandle();
		delete pCheck;
	}
	else
	if( aCheckBase->m_ClassName==_StCheckSpBehavior )
	{
		StCheckSpBehavior *pCheck = (StCheckSpBehavior *)aCheckBase;
		delete pCheck;
	}
	else
	if( aCheckBase->m_ClassName==_StCheckDataLose )
	{
		StCheckDataLose *pCheck = (StCheckDataLose *)aCheckBase;
		delete pCheck;
	}
	//��1.0.0.1062
	else
	if( aCheckBase->m_ClassName==_StCheckDataLoseDlg )
	{
		StCheckDataLoseDlg *pCheck = (StCheckDataLoseDlg *)aCheckBase->GetMainHandle();
		delete pCheck;
	}
	//��1.0.0.1062
	else
	if( aCheckBase->m_ClassName==_StCheckPixelDefect )
	{
		StCheckPixelDefect *pCheck = (StCheckPixelDefect *)aCheckBase;
		delete pCheck;
	}
	else
	if( aCheckBase->m_ClassName==_StCheckDustDlg )
	{
		StCheckDustDlg *pCheck = (StCheckDustDlg *)aCheckBase->GetMainHandle();
		delete pCheck;
	}
	else
	if( aCheckBase->m_ClassName==_StCheckMessage )
	{
		StCheckMessage *pCheck = (StCheckMessage *)aCheckBase;
		delete pCheck;
	}
	else
	if( aCheckBase->m_ClassName==_StCheckAdjustLinetolineDlg )
	{
		StCheckAdjustLinetolineDlg *pCheck = (StCheckAdjustLinetolineDlg *)aCheckBase->GetMainHandle();
		delete pCheck;
	}
	else
	if( aCheckBase->m_ClassName==_StCheckAdjustLinetoline )
	{
		StCheckAdjustLinetoline *pCheck = (StCheckAdjustLinetoline *)aCheckBase;
		delete pCheck;
	}
	else
	if( aCheckBase->m_ClassName==_StCheckEffectivePixel )
	{
		StCheckEffectivePixel *pCheck = (StCheckEffectivePixel *)aCheckBase;
		delete pCheck;
	}
	else
	if( aCheckBase->m_ClassName==_StInputSensorVersionDlg )
	{
		StInputSensorVersionDlg *pCheck = (StInputSensorVersionDlg *)aCheckBase->GetMainHandle();
		delete pCheck;
	}
	else
	if( aCheckBase->m_ClassName==_StInputSensorSerialDlg )
	{
		StInputSensorSerialDlg *pCheck = (StInputSensorSerialDlg *)aCheckBase->GetMainHandle();
		delete pCheck;
	}
	else
	if( aCheckBase->m_ClassName==_StLightControlDlg )
	{
		StLightControlDlg *pCheck = (StLightControlDlg *)aCheckBase->GetMainHandle();
		delete pCheck;
	}
	else
	if( aCheckBase->m_ClassName==_StLightControl )
	{
		StLightControl *pCheck = (StLightControl *)aCheckBase;
		delete pCheck;
	}
	else
	if( aCheckBase->m_ClassName==_StCheckPower )
	{
		StCheckPower *pCheck = (StCheckPower *)aCheckBase;
		delete pCheck;
	}
	else
	if( aCheckBase->m_ClassName==_StCheckCommon )
	{
		StCheckCommon *pCheck = (StCheckCommon *)aCheckBase;
		delete pCheck;
	}
	else
	if( aCheckBase->m_ClassName==_StCheckSerialVersion )
	{
		StCheckSerialVersion *pCheck = (StCheckSerialVersion *)aCheckBase;
		delete pCheck;
	}
	else
	if( aCheckBase->m_ClassName==_StCheckIO )
	{
		StCheckIO *pCheck = (StCheckIO *)aCheckBase;
		delete pCheck;
	}
	else
	if( aCheckBase->m_ClassName==_StPTouch )
	{
		StPTouch *pCheck = (StPTouch *)aCheckBase;
		delete pCheck;
	}
	//��1.0.0.1015
	else
	if( aCheckBase->m_ClassName==_StWriteSerial )
	{
		StWriteSerial *pCheck = (StWriteSerial *)aCheckBase;
		delete pCheck;
	}
	//��1.0.0.1015
	//��1.0.0.1058
	else
	if( aCheckBase->m_ClassName==_StWriteFileData )
	{
		StWriteFileData *pCheck = (StWriteFileData *)aCheckBase;
		delete pCheck;
	}
	//��1.0.0.1058

	//��1.0.0.1021
	else
	if( aCheckBase->m_ClassName==_StCheckPixelCorrect )
	{
		StCheckPixelCorrect *pCheck = (StCheckPixelCorrect *)aCheckBase->GetMainHandle();
		delete pCheck;
	}
	//��1.0.0.1021
	//��1.0.0.1021
	else
	if( aCheckBase->m_ClassName==_StCheckTemperature )
	{
		StCheckTemperature *pCheck = (StCheckTemperature *)aCheckBase;
		delete pCheck;
	}
	else
	if( aCheckBase->m_ClassName==_StCheckTestPattern )
	{
		StCheckTestPattern *pCheck = (StCheckTestPattern *)aCheckBase;
		delete pCheck;
	}
	//��1.0.0.1021

	//��1.0.0.1022
	else
	if( aCheckBase->m_ClassName==_StCheckEllitoLVDSDlg )
	{
		StCheckEllitoLVDSDlg *pCheck = (StCheckEllitoLVDSDlg *)aCheckBase->GetMainHandle();
		delete pCheck;
	}
	//��1.0.0.1022

	//��1.0.0.1059
	else
	if( aCheckBase->m_ClassName==_StCheckEllitoLVDS )
	{
		StCheckEllitoLVDS *pCheck = (StCheckEllitoLVDS *)aCheckBase;
		delete pCheck;
	}
	//��1.0.0.1059

	//��1.0.0.1023
	else
	if( aCheckBase->m_ClassName==_StLoop )
	{
		StLoop *pCheck = (StLoop *)aCheckBase;
		delete pCheck;
	}
	//��1.0.0.1023
	//��1.0.0.1030
	else
	if( aCheckBase->m_ClassName==_StIF )
	{
		StIF *pCheck = (StIF *)aCheckBase;
		delete pCheck;
	}
	//��1.0.0.1030
	//��1.0.0.1033
	else
	if( aCheckBase->m_ClassName==_StCheckFramerate )
	{
		StCheckFramerate *pCheck = (StCheckFramerate *)aCheckBase;
		delete pCheck;
	}
	//��1.0.0.1033
	//��1.0.0.1040
	else
	if( aCheckBase->m_ClassName==_StCheckCSVDust )
	{
		StCheckCSVDust *pCheck = (StCheckCSVDust *)aCheckBase;
		delete pCheck;
	}
	//��1.0.0.1040
	//��1.0.0.1044
	else
	if( aCheckBase->m_ClassName==_StCheckAdjustOffset )
	{
		StCheckAdjustOffset *pCheck = (StCheckAdjustOffset *)aCheckBase;
		delete pCheck;
	}
	//��1.0.0.1044
	//��1.0.0.1046
	else
	if( aCheckBase->m_ClassName==_StCameraControlDlg )
	{
		StCameraControlDlg *pCheck = (StCameraControlDlg *)aCheckBase->GetMainHandle();
		delete pCheck;
	}
	//��1.0.0.1046
	//��1.0.0.1047
	else
	if( aCheckBase->m_ClassName==_StCheckSwitchDlg )
	{
		StCheckSwitchDlg *pCheck = (StCheckSwitchDlg *)aCheckBase->GetMainHandle();
		delete pCheck;
	}
	//��1.0.0.1047
	//��1.0.0.1048
	else
	if( aCheckBase->m_ClassName==_StCheckAdjustVolumeDlg )
	{
		StCheckAdjustVolumeDlg *pCheck = (StCheckAdjustVolumeDlg *)aCheckBase->GetMainHandle();
		delete pCheck;
	}
	//��1.0.0.1048
	//��1.0.0.1049
	else
	if( aCheckBase->m_ClassName==_StCheckAdjustGainDlg )
	{
		StCheckAdjustGainDlg *pCheck = (StCheckAdjustGainDlg *)aCheckBase->GetMainHandle();
		delete pCheck;
	}
	//��1.0.0.1049
	else
	{
		return FALSE;
	}
	//vector����폜
	m_vecCheckBase.erase(m_vecCheckBase.begin() + nCheckBaseIndex );
	return TRUE;
}

//��1.0.0.1048
//BOOL StInspectMain::DeleteImageMessageDlg(INT nImageMessageDlgIndex)
//{
//	if( nImageMessageDlgIndex>=(INT)m_vecImageMessageDlg.size() ) return FALSE;
//
//	StImageMessageDlg *aStImageMessageDlg = m_vecImageMessageDlg[nImageMessageDlgIndex];
//	delete aStImageMessageDlg;
//	m_vecImageMessageDlg.erase(m_vecImageMessageDlg.begin() + nImageMessageDlgIndex );
//
//	return TRUE;
//}
BOOL StInspectMain::DeleteCustomDlg(INT nCustomDlgIndex)
{
	if( nCustomDlgIndex>=(INT)m_vecCustomDlg.size() ) return FALSE;

	CCustomDialogEx *aCustomDialog = m_vecCustomDlg[nCustomDlgIndex];
	if( aCustomDialog->m_ClassName==_StImageMessageDlg )
	{
		StImageMessageDlg *pCheck = (StImageMessageDlg *)aCustomDialog;
		delete pCheck;
	}
	else
	if( aCustomDialog->m_ClassName==_StHttpControlDlg )
	{
		StHttpControlDlg *pCheck = (StHttpControlDlg *)aCustomDialog;
		delete pCheck;
	}
	else
	{
		delete aCustomDialog;
	}
	m_vecCustomDlg.erase(m_vecCustomDlg.begin() + nCustomDlgIndex );

	return TRUE;
}

//��1.0.0.1048


//��1.0.0.1023 �J�Ԃ��p�ϐ��ǉ�
BOOL StInspectMain::DeleteValue(INT nValueIndex)
{
	if( nValueIndex>=(INT)m_vecValue.size() ) return FALSE;

	StValue *aStValue = m_vecValue[nValueIndex];
	delete aStValue;
	m_vecValue.erase(m_vecValue.begin() + nValueIndex );

	return TRUE;
}
//��1.0.0.1023 �J�Ԃ��p�ϐ��ǉ�

//��1.0.0.1025
BOOL StInspectMain::DeleteCommonBase(INT nIndex)
{
	if( nIndex>=(INT)m_vecCommonBase.size() ) return FALSE;

	StCommonBase *aCommonBase = m_vecCommonBase[nIndex];
	if( *aCommonBase->GetClassName()==_StFileOperation )
	{
		StFileOperation *pBase = (StFileOperation *)aCommonBase;
		delete pBase;
	}
	//��1.0.0.1051
	else
	if( *aCommonBase->GetClassName()==_StInterfaceGenICam )
	{
		StInterfaceGenICam *pBase = (StInterfaceGenICam *)aCommonBase;
		delete pBase;
	}
	//��1.0.0.1051
	m_vecCommonBase.erase(m_vecCommonBase.begin() + nIndex );

	return TRUE;
}
//��1.0.0.1025

void StInspectMain::AllDelete(void)
{
	for( DWORD i=0; i<m_vecControlDlg.size(); i++ )
	{
		CControlDlg *aDlg = (CControlDlg *)m_vecControlDlg[i];
		delete aDlg;
	}
	m_vecControlDlg.clear();


	//��1.0.0.1020
	//INT nSerialCommCount = m_vecSerialComm.size();
	//for( INT i=nSerialCommCount-1; i>=0; i-- )
	//{
	//	DeleteSerialComm(i);
	//}
	//m_vecSerialComm.clear();
	INT nSerialCommCount = m_vecControl.size();
	for( INT i=nSerialCommCount-1; i>=0; i-- )
	{
		DeleteSerialComm(i);
	}
	m_vecControl.clear();
	//��1.0.0.1020

	INT nTexioControlCount = m_vecTexioControl.size();
	for( INT i=nTexioControlCount-1; i>=0; i-- )
	{
		DeleteTexioControl(i);
	}
	m_vecTexioControl.clear();

	INT nCheckCount = m_vecCheckBase.size();
	for( INT i=nCheckCount-1; i>=0; i-- )
	{
		DeleteCheckBase(i);
	}
	m_vecCheckBase.clear();

	INT nDeviceCount = m_vecDeviceBase.size();
	for( INT i=nDeviceCount-1; i>=0; i-- )
	{
		DeleteDeviceBase(i);
	}
	m_vecDeviceBase.clear();

	//��1.0.0.1048
	////��1.0.0.1043
	//INT nImageMessageCount = m_vecImageMessageDlg.size();
	//for( INT i=nImageMessageCount-1; i>=0; i-- )
	//{
	//	DeleteImageMessageDlg(i);
	//}
	//m_vecImageMessageDlg.clear();
	////��1.0.0.1043
	INT nCustomDlgCount = m_vecCustomDlg.size();
	for( INT i=nCustomDlgCount-1; i>=0; i-- )
	{
		DeleteCustomDlg(i);
	}
	m_vecCustomDlg.clear();
	//��1.0.0.1048

	//��1.0.0.1023 �J�Ԃ��p�ϐ��ǉ�
	INT nValueCount = m_vecValue.size();
	for( INT i=nValueCount-1; i>=0; i-- )
	{
		DeleteValue(i);
	}
	m_vecValue.clear();
	//��1.0.0.1023 �J�Ԃ��p�ϐ��ǉ�


	//��1.0.0.1025
	INT nCommonCount = m_vecCommonBase.size();
	for( INT i=nCommonCount-1; i>=0; i-- )
	{
		DeleteCommonBase(i);
	}
	m_vecCommonBase.clear();
	//��1.0.0.1025

}

//��1.0.0.1020
StCheckBase * StInspectMain::GetCheckBase(LPCTSTR szName)
{
	StCheckBase * aCheckBase = NULL;
	for( DWORD i=0; i<m_vecCheckBase.size() && aCheckBase==NULL; i++ )
	{
		if( m_vecCheckBase[i]->m_VarName==szName )
		{
			aCheckBase = m_vecCheckBase[i];
		}
	}
	return aCheckBase;
}
//��1.0.0.1020

StDeviceBase * StInspectMain::GetDeviceBase(LPCTSTR szName)
{
	StDeviceBase * aDeviceBase = NULL;
	for( DWORD i=0; i<m_vecDeviceBase.size() && aDeviceBase==NULL; i++ )
	{
		if( m_vecDeviceBase[i]->m_VarName==szName )
		{
			aDeviceBase = m_vecDeviceBase[i];
		}
	}
	return aDeviceBase;
}

//��1.0.0.1017
//StSerialComm * StInspectMain::GetSerialComm(LPCTSTR szName)
StControl * StInspectMain::GetSerialComm(LPCTSTR szName)
//��1.0.0.1017
{
	//��1.0.0.1017
	//StSerialComm * aSerialComm = NULL;
	StControl * aSerialComm = NULL;
	//��1.0.0.1017
	//��1.0.0.1020
	//for( DWORD i=0; i<m_vecSerialComm.size() && aSerialComm==NULL; i++ )
	//{
	//	if( m_vecSerialComm[i]->m_VarName==szName )
	//	{
	//		aSerialComm = m_vecSerialComm[i];
	//	}
	//}
	for( DWORD i=0; i<m_vecControl.size() && aSerialComm==NULL; i++ )
	{
		if( m_vecControl[i]->m_VarName==szName )
		{
			aSerialComm = m_vecControl[i];
		}
	}
	//��1.0.0.1020
	return aSerialComm;
}

StTexioControl * StInspectMain::GetTexioControl(LPCTSTR szName)
{
	StTexioControl * aTexioControl = NULL;
	for( DWORD i=0; i<m_vecTexioControl.size() && aTexioControl==NULL; i++ )
	{
		if( m_vecTexioControl[i]->m_VarName==szName )
		{
			aTexioControl = m_vecTexioControl[i];
		}
	}
	return aTexioControl;
}
//��1.0.0.1052
StCommonBase * StInspectMain::GetCommonBase(LPCTSTR szName)
{
	StCommonBase * aCommonBase = NULL;
	for( DWORD i=0; i<m_vecCommonBase.size() && aCommonBase==NULL; i++ )
	{
		if( *m_vecCommonBase[i]->GetVarName()==szName )
		{
			aCommonBase = m_vecCommonBase[i];
		}
	}
	return aCommonBase;
}
//��1.0.0.1052

//------------------------------------------------------------------------
//eBUS
//------------------------------------------------------------------------
BOOL StInspectMain::ConnectEBus(StDeviceEBus *pDevice, LPCTSTR szArgument)
{
	BOOL bReval=FALSE;
	INT nTimeout=1000;


	//��1.0.0.1062
	if( m_nScriptCheckMode!=0 )
	{
		return pDevice->VirturalConnect();
	}
	//��1.0.0.1062

	//------��x�ڑ���A�d��OFFON��ɍēxConnect����ꍇ-----------------------------------------------------------
	PvString szMACAddress;
	pDevice->GetMACAddressStr(szMACAddress);
	if( szMACAddress.GetLength()>0 )
	{
		PvString szIPAddress;
		pDevice->GetIPAddressStr(szIPAddress);
		PvString szSubnetMask;
		pDevice->GetSubnetMaskStr(szSubnetMask);
		PvDevice lDevice;
		lDevice.SetIPConfiguration(szMACAddress,szIPAddress ,szSubnetMask ,_T("0.0.0.0") );

		nTimeout=30000;
	}
	//-----------------------------------------------------------------


	if( _tcslen(szArgument)>0 )
		mDeviceModelName = szArgument;
	PvSystem lSystem;
	PvDeviceFinderWnd lDeviceFinderWnd;
	PvDeviceInfo *pDeviceInfo = DiscoveryDeviceInfo( lSystem, mDeviceModelName, nTimeout, pDevice);	//30Sec
	if( !pDeviceInfo )
	{
		PvResult lResult = lDeviceFinderWnd.ShowModal();
		if( lResult.IsOK() )
		{
			pDeviceInfo = lDeviceFinderWnd.GetSelected();
		}
	}
	bReval = pDevice->ConnectCustom( pDeviceInfo );
	if( bReval )
	{
		pDevice->RegisterCallback( this, &StInspectMain::ImageCallback, NULL );
	}
	return bReval;
}

BOOL StInspectMain::DisonnectEBus(StDeviceEBus *pDevice)
{
	BOOL bReval=TRUE;

	//��1.0.0.1062
	if( m_nScriptCheckMode!=0 )
	{
		return pDevice->VirturalDisconnect();
	}
	//��1.0.0.1062

	if( pDevice->IsConnected() )
	{
//TRACE(TEXT("@@@@@@@@@ UnregisterCallback IN\n") );
		pDevice->UnregisterCallback();

//TRACE(TEXT("@@@@@@@@@ DisconnectCustom IN\n") );
		bReval = pDevice->DisconnectCustom();
//TRACE(TEXT("@@@@@@@@@ DisconnectCustom OUT\n") );
	}
	return bReval;

}


BOOL StInspectMain::StartEBus(StDeviceEBus *pDevice)
{
	BOOL bReval=TRUE;

	//��1.0.0.1062
	if( m_nScriptCheckMode!=0 )
	{
		return pDevice->VirturalStart();
	}
	//��1.0.0.1062

	CStGenFactoryScriptDlg *pWnd = (CStGenFactoryScriptDlg *)mpParent;
	PvInt64 iWidth, iHeight;
	pDevice->GetGenParameters()->GetIntegerValue("Width", iWidth);
	pDevice->GetGenParameters()->GetIntegerValue("Height", iHeight);
	pWnd->CreatePreview((int)iWidth,(int)iHeight);
	//��1.0.0.1047
	//pWnd->CreateWaveform((int)iWidth,(int)iHeight);
	pWnd->CreateWaveform(iWidth,iHeight,m_nRuledLine);
	//��1.0.0.1047
	pDevice->StartAcquisition();
	return bReval;
}

BOOL StInspectMain::StopEBus(StDeviceEBus *pDevice)
{
	BOOL bReval=TRUE;

	//��1.0.0.1062
	if( m_nScriptCheckMode!=0 )
	{
		return pDevice->VirturalStop();
	}
	//��1.0.0.1062

	if( pDevice->IsConnected() )
		pDevice->StopAcquisition();
	CStGenFactoryScriptDlg *pWnd = (CStGenFactoryScriptDlg *)mpParent;

	//��1.0.0.1014 beta1
	Sleep(1000);
	//��1.0.0.1014 beta1

	pWnd->DeletePreview();
	pWnd->DeleteWaveform();


	return bReval;
}

BOOL StInspectMain::ControlDialogEBus(StDeviceEBus *pDevice, LPCTSTR szArgument)
{
	//��1.0.0.1062
	if( m_nScriptCheckMode!=0 )
	{
		return TRUE;
	}
	//��1.0.0.1062

	INT nExistIndex = -1;
	CControlDlg *aExistDlg = NULL;
	for( DWORD i=0; i<m_vecControlDlg.size() && nExistIndex<0; i++ )
	{
		CControlDlg *aDlg = (CControlDlg *)m_vecControlDlg[i];
		if( aDlg->GetDeviceEBus() == pDevice )
		{
			aExistDlg = aDlg;
			nExistIndex = i;
		}
	}

	BOOL bReval=FALSE;
	if( _tcscmp(szArgument,_T("delete"))==0 )
	{
		if( nExistIndex>=0 )
		{
			CControlDlg *aDlg = (CControlDlg *)m_vecControlDlg[nExistIndex];
			//HIDE-------
			aDlg->ShowWindow(SW_HIDE);
			delete aDlg;
			//vector����폜
			m_vecControlDlg.erase(m_vecControlDlg.begin() + nExistIndex);
			bReval = TRUE;
		}
	}
	else
	{
		if( !aExistDlg )
		{
			CControlDlg *aDlg = new CControlDlg;
			if( aDlg )
			{
				aDlg->SetDeviceEBus(pDevice);
				aDlg->Create(IDD_DIALOG_CONTROLEBUS);
				aDlg->SetWindowPos(
							NULL,
							0,
							0,
							0,0,SWP_NOZORDER | SWP_NOSIZE);

				m_vecControlDlg.push_back((void *)aDlg);
				aExistDlg = aDlg;
			}
		}
		if( aExistDlg )
		{
			//Show
			aExistDlg->ShowWindow(SW_SHOW);
			bReval = TRUE;
		}
	}

	return bReval;
}



BOOL StInspectMain::SerialControlEBus( StDeviceEBus *pDevice, LPCTSTR szPort, LPCTSTR szData )
{
	BOOL bReval=FALSE;
	//��1.0.0.1062
	if( m_nScriptCheckMode!=0 )
	{
		return pDevice->IsVirturalConnected();
	}
	//��1.0.0.1062

	if( pDevice->IsConnected() )
	{
		bReval = pDevice->SerialControl( szPort, szData);
	}
	return bReval;
}

BOOL StInspectMain::SetGenICamControlEBus( StDeviceEBus *pDevice, LPCTSTR szPort, LPCTSTR szData )
{
	BOOL bReval=FALSE;
	//��1.0.0.1062
	if( m_nScriptCheckMode!=0 )
	{
		return pDevice->IsVirturalConnected();
	}
	//��1.0.0.1062
	if( pDevice->IsConnected() )
	{
		bReval = pDevice->SetGenICamControl( szPort, szData);
	}
	return bReval;
}

//------------------------------------------------------------------------
//MultiCam
//------------------------------------------------------------------------
BOOL StInspectMain::OpenMultiCam(StDeviceMultiCam *pDevice)
{
	//��1.0.0.1062
	if( m_nScriptCheckMode!=0 )
	{
		return pDevice->VirturalConnect();
	}
	//��1.0.0.1062


	BOOL bReval = pDevice->function(_T("Open"),NULL);
	if( bReval )
	{
		pDevice->RegisterCallback( this, &StInspectMain::ImageCallback, NULL );

		//��1.0.0.1015
		//DeviceModelName
		pDevice->SetDeviceModelName(mDeviceModelName);
		//�V���A���ԍ�
		pDevice->SetSerialNumber(mSerialNumber);
		//�i�ڃR�[�h
		pDevice->SetHinmokuCode(mHinmokuCode);
		//��1.0.0.1015

	}
	return bReval;
}

BOOL StInspectMain::CloseMultiCam(StDeviceMultiCam *pDevice)
{
	//��1.0.0.1062
	if( m_nScriptCheckMode!=0 )
	{
		return pDevice->VirturalDisconnect();
	}
	//��1.0.0.1062



	pDevice->UnregisterCallback();

	BOOL bReval = pDevice->function(_T("Close"),NULL);
	return bReval;
}

BOOL StInspectMain::StartMultiCam(StDeviceMultiCam *pDevice)
{
	BOOL bReval=TRUE;

	//��1.0.0.1062
	if( m_nScriptCheckMode!=0 )
	{
		return pDevice->VirturalStart();
	}
	//��1.0.0.1062

	CStGenFactoryScriptDlg *pWnd = (CStGenFactoryScriptDlg *)mpParent;
	INT iWidth, iHeight;
	iWidth = pDevice->GetWidth();
	iHeight = pDevice->GetHeight();
	pWnd->CreatePreview(iWidth,iHeight);
	//��1.0.0.1047
	//pWnd->CreateWaveform(iWidth,iHeight);
	pWnd->CreateWaveform(iWidth,iHeight,m_nRuledLine);
	//��1.0.0.1047
	bReval = pDevice->function(_T("Start"),NULL);
	return bReval;
}

BOOL StInspectMain::StopMultiCam(StDeviceMultiCam *pDevice)
{
	BOOL bReval=TRUE;
	//��1.0.0.1062
	if( m_nScriptCheckMode!=0 )
	{
		return pDevice->VirturalStop();
	}
	//��1.0.0.1062
	if( pDevice->IsOpened() && pDevice->IsStarted() )
		bReval = pDevice->function(_T("Stop"),NULL);
	CStGenFactoryScriptDlg *pWnd = (CStGenFactoryScriptDlg *)mpParent;

	//��1.0.0.1015
	Sleep(1000);
	//��1.0.0.1015

	pWnd->DeletePreview();
	pWnd->DeleteWaveform();
	return bReval;
}

//��1.0.0.1046
//------------------------------------------------------------------------
//GenICam
//------------------------------------------------------------------------
BOOL StInspectMain::OpenGenICam(StDeviceGenICam *pDevice, LPCTSTR szArgument)
{
	//��1.0.0.1062
	if( m_nScriptCheckMode!=0 )
	{
		return pDevice->VirturalConnect();
	}
	//��1.0.0.1062

	BOOL bReval = pDevice->function(_T("Open"),szArgument);
	if( bReval )
	{
		pDevice->RegisterCallback( this, &StInspectMain::ImageCallback, NULL );

		//��1.0.0.1015
		//DeviceModelName
		pDevice->SetDeviceModelName(mDeviceModelName);
		//�V���A���ԍ�
		pDevice->SetSerialNumber(mSerialNumber);
		//�i�ڃR�[�h
		pDevice->SetHinmokuCode(mHinmokuCode);
		//��1.0.0.1015

	}
	return bReval;
}

BOOL StInspectMain::CloseGenICam(StDeviceGenICam *pDevice)
{
	//��1.0.0.1062
	if( m_nScriptCheckMode!=0 )
	{
		return pDevice->VirturalDisconnect();
	}
	//��1.0.0.1062

	pDevice->UnregisterCallback();

	BOOL bReval = pDevice->function(_T("Close"),NULL);
	return bReval;
}

BOOL StInspectMain::StartGenICam(StDeviceGenICam *pDevice)
{
	//��1.0.0.1062
	if( m_nScriptCheckMode!=0 )
	{
		return pDevice->VirturalStart();
	}
	//��1.0.0.1062
	BOOL bReval=TRUE;
	CStGenFactoryScriptDlg *pWnd = (CStGenFactoryScriptDlg *)mpParent;
	INT iWidth = 0;
	INT iHeight = pDevice->GetDisplayHeight();

	pDevice->GetGenICamControl(_T("Width"), iWidth);
	if( iHeight==0 )
		pDevice->GetGenICamControl(_T("Height"), iHeight);

	pWnd->CreatePreview(iWidth,iHeight);
	//��1.0.0.1047
	//pWnd->CreateWaveform(iWidth,iHeight);
	pWnd->CreateWaveform(iWidth,iHeight,m_nRuledLine);
	//��1.0.0.1047
	bReval = pDevice->function(_T("Start"),NULL);
	return bReval;
}

BOOL StInspectMain::StopGenICam(StDeviceGenICam *pDevice)
{
	//��1.0.0.1062
	if( m_nScriptCheckMode!=0 )
	{
		return pDevice->VirturalStop();
	}
	//��1.0.0.1062
	BOOL bReval=TRUE;
	//if( pDevice->IsOpened() && pDevice->IsStarted() )
	if( pDevice->IsOpened() )
		bReval = pDevice->function(_T("Stop"),NULL);
	CStGenFactoryScriptDlg *pWnd = (CStGenFactoryScriptDlg *)mpParent;

	Sleep(1000);

	pWnd->DeletePreview();
	pWnd->DeleteWaveform();
	return bReval;
}

//��1.0.0.1046