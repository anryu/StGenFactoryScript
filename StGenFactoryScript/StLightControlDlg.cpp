// StLightControlDlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "StGenFactoryScript.h"
#include "StLightControlDlg.h"
#include "afxdialogex.h"
#include <vector>

#define ST_DISPLAYTEXT 1
//#define ST_LIGHTCONTROLEXECUTE 2
//#define ST_LightControlThread 3
void UpdateLogListBoxScrollBar(CListBox *pListBox);


unsigned __stdcall LightControlDlgThread( void *pParam )
{
	StLightControlDlg * aDlg = (StLightControlDlg *)pParam;

	//初期値セット
	BOOL bReval = aDlg->SendInitValue();
	if( bReval )
	{
		aDlg->LightAdjustExecute();
		/*
		do
		{
			//if( aDlg->GetThreadStop() )
			//	break;

			BOOL bReval = aDlg->LightAdjustExecute();

			if( !bReval )
				break;	//NGではなくエラー

			//if( aDlg->GetResultCount()>=aDlg->GetStandardCount() )
			//	break;	//カウント終了
			if( aDlg->GetJudge()==1 )	//正常終了
				break;

			//Sleep(200);	//高速で動かすとOKボタンが押せなくなる。

		}while(0);
		*/
	}
	_endthreadex(0);
	return 0;
}

// StLightControlDlg ダイアログ

IMPLEMENT_DYNAMIC(StLightControlDlg, CDialogEx)

StLightControlDlg::StLightControlDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(StLightControlDlg::IDD, pParent)
{
	Initialize();
}

StLightControlDlg::StLightControlDlg( LPCTSTR szClassName, LPCTSTR szVarName, StDeviceBase *aDeviceBase, CDialogEx *aDialogEx, CWnd* pParent /*=NULL*/)
	: CDialogEx(StLightControlDlg::IDD, pParent)
	, StCheckBase( szClassName, szVarName, aDeviceBase, aDialogEx )
{
	Initialize();
}

StLightControlDlg::~StLightControlDlg()
{
	KillTimer(ST_DISPLAYTEXT);

	::WaitForSingleObject(hDisplayMutex,INFINITE);
	::ReleaseMutex(hDisplayMutex);
	CloseHandle(hDisplayMutex);
}

void StLightControlDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(StLightControlDlg, CDialogEx)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// StLightControlDlg メッセージ ハンドラー

BOOL StLightControlDlg::OnInitDialog()
{
	__super::OnInitDialog();

	// TODO:  ここに初期化を追加してください



	//表示用SetTimer
	hDisplayMutex = CreateMutex(NULL,FALSE,NULL);
	SetTimer( ST_DISPLAYTEXT, 200, NULL );		//unsigned int iID;

	//検査Thread作成-------------
	//SetTimer( ST_LightControlThread, 1, NULL );		//unsigned int iID;
	m_hThread = (HANDLE)_beginthreadex( NULL, 0, LightControlDlgThread, this,  0, &m_mThreadID );


	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

void StLightControlDlg::Initialize(void)
{
	m_hThread = NULL;
	m_pSerialComm = NULL;

	m_nStart = 0;
	m_nEnd = 4095;
	m_nTimer = 1000;
	m_szColor = _T("");

	for( int i=0; i<_countof(m_nMaxValue); i++  )
		m_nMaxValue[i] = 1023;
	for( int i=0; i<_countof(m_nInitValue); i++  )
		m_nInitValue[i] = 0;
	for( int i=0; i<_countof(m_nTarget); i++  )
		m_nTarget[i] = 210;
	for( int i=0; i<_countof(m_nTargetRange); i++  )
		m_nTargetRange[i] = 10;

}

BOOL StLightControlDlg::function(LPCTSTR szFunc, LPCTSTR szArgument)
{
	BOOL bReval = FALSE;
	CString szFunction(szFunc);
	if( szFunction == _T("Execute") )
	{
	}
	else
	if( szFunction=="SetSerialComm" )
	{
		//m_nStart = _tcstol(szArgument,NULL,0);
		//bReval = TRUE;
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
	if( szFunction=="SetColor" )
	{
		m_szColor = szArgument;
		bReval = TRUE;
	}
	else
	if( szFunction=="SetMaxValueR" )
	{
		m_nMaxValue[0] = _tcstol(szArgument,NULL,0);
		bReval = TRUE;
	}
	else
	if( szFunction=="SetMaxValueG" )
	{
		m_nMaxValue[1] = _tcstol(szArgument,NULL,0);
		bReval = TRUE;
	}
	else
	if( szFunction=="SetMaxValueB" )
	{
		m_nMaxValue[2] = _tcstol(szArgument,NULL,0);
		bReval = TRUE;
	}
	else
	if( szFunction=="SetInitValueR" )
	{
		m_nInitValue[0] = _tcstol(szArgument,NULL,0);
		bReval = TRUE;
	}
	else
	if( szFunction=="SetInitValueG" )
	{
		m_nInitValue[1] = _tcstol(szArgument,NULL,0);
		bReval = TRUE;
	}
	else
	if( szFunction=="SetInitValueB" )
	{
		m_nInitValue[2] = _tcstol(szArgument,NULL,0);
		bReval = TRUE;
	}
	else
	if( szFunction=="SetTargetR" )
	{
		m_nTarget[0] = _tcstol(szArgument,NULL,0);
		bReval = TRUE;
	}
	else
	if( szFunction=="SetTargetG" )
	{
		m_nTarget[1] = _tcstol(szArgument,NULL,0);
		bReval = TRUE;
	}
	else
	if( szFunction=="SetTargetB" )
	{
		m_nTarget[2] = _tcstol(szArgument,NULL,0);
		bReval = TRUE;
	}
	else
	if( szFunction=="SetTargetRangeR" )
	{
		m_nTargetRange[0] = _tcstol(szArgument,NULL,0);
		bReval = TRUE;
	}
	else
	if( szFunction=="SetTargetRangeG" )
	{
		m_nTargetRange[1] = _tcstol(szArgument,NULL,0);
		bReval = TRUE;
	}
	else
	if( szFunction=="SetTargetRangeB" )
	{
		m_nTargetRange[2] = _tcstol(szArgument,NULL,0);
		bReval = TRUE;
	}

	return bReval;
}


typedef struct _LightControlDlgResultList
{
	double dblAverage;
	double dblDifference;
	INT nSetting;
	//INT iResult;
}LightControlDlgResultList, *PLightControlDlgResultList;

BOOL StLightControlDlg::LightAdjustExecute(void)
{

	BOOL bReval = TRUE;
	m_iJudge = 2;	//2:処理中

	INT nShade = 0;
	INT nColorLength = m_szColor.GetLength();
	if( nColorLength==0 ) return FALSE;

	INT nGo = 0;

	for( int i=0; i<nColorLength && bReval; i++ )	//
	{
		if( m_szColor.Mid(i,1).Compare(_T("R"))==0 ) nGo = 0;
		else if( m_szColor.Mid(i,1).Compare(_T("G"))==0 ) nGo = 1;
		else if( m_szColor.Mid(i,1).Compare(_T("B"))==0 ) nGo = 2;
		SetShadeGo(nGo);

		//LightAdjustColor(nGo);
		INT nSetValue = m_nMaxValue[nGo];
		double dblCheckAverage = 0.0;
		INT nBeforeValue = 0;

		std::vector<LightControlDlgResultList> vResultList;

		do
		{
			//設定
			SetShadeValue(nSetValue);

			//待ち時間
			Sleep(m_nTimer);

			//画像取得
			if( m_Buffer ) delete m_Buffer;
			m_Buffer = new StBuffer;
			bReval = m_DeviceBase->GetStBuffer(m_Buffer);	//変換して取得もありかも。現時点ではコピー

			//平均値取得
			double dblResultAverage[4];
			if( bReval )
				bReval = GetSNAverage( m_Buffer, m_nStart, m_nEnd, dblResultAverage, NULL, NULL, NULL );

			if( bReval )
			{
				dblCheckAverage = dblResultAverage[nGo];
				INT nElement = GetElementFromBuffer(m_Buffer);
				if( nElement==1 )	dblCheckAverage = dblResultAverage[0];	//Mono

				LightControlDlgResultList aResult;
				aResult.dblAverage = dblCheckAverage;
				aResult.nSetting = nSetValue;
				aResult.dblDifference = fabs((double)m_nTarget[nGo]-dblCheckAverage);
				vResultList.push_back(aResult);
//TRACE
				//Check
				INT nNextValue = nSetValue;
				if( (double)m_nTarget[nGo]<dblCheckAverage )
				{
					nNextValue -= abs(nSetValue - nBeforeValue)/2;
				}
				else
				if( (double)m_nTarget[nGo]>dblCheckAverage )
				{
					nNextValue += abs(nSetValue - nBeforeValue)/2;
				}

				nBeforeValue = nSetValue;
				nSetValue = nNextValue;

				if( nBeforeValue==nSetValue )
					break;	//終了
			}

		}while(bReval);

		if( bReval )
		{
			double dblBestDifference = 9999.0;
			INT nBestIndex = -1;
			//vResultListリストから一番いい数値を求める
			for( int j=0; j<(int)vResultList.size(); j++ )	//
			{
				if( vResultList[j].dblDifference<dblBestDifference )
				{
					dblBestDifference = vResultList[j].dblDifference;
					nBestIndex = j;
				}
			}

			bReval = SetShadeValue(vResultList[nBestIndex].nSetting);
			if( dblBestDifference>(double)m_nTargetRange[nGo] )
			{
				m_iJudge = 0;
			}

		}
		vResultList.clear();
		if( m_iJudge==0 ) break;
	}

	if( bReval )
	{
		if( m_iJudge == 2 ) m_iJudge = 1;
	}
	else	 m_iJudge = 0;

	return bReval;
}

BOOL StLightControlDlg::Execute(void)
{

	//SetTimer( ST_LIGHTCONTROLEXECUTE, 200, NULL );


	BOOL bReval = FALSE;
	Sleep(200);
	if( m_hThread )
	{
		::WaitForSingleObject( m_hThread, INFINITE );
		if( m_iJudge==1 || m_iJudge==0 ) //NG時もTRUEで返す
			bReval=TRUE;
	}
	return bReval;
}

BOOL StLightControlDlg::SetShadeGo(INT nColor)
{
	BOOL bReval = FALSE;
	INT nShadeGo = 3 - nColor;
	CString szSend;
	szSend.Format(_T("shd_go=%d"),nShadeGo);	//照明を可変する色設定
	bReval = m_pSerialComm->SendData(szSend);

DisplayListBox(szSend);

	return bReval;
}

BOOL StLightControlDlg::SetShadeValue(INT nValue)
{
	BOOL bReval = FALSE;
	CString szSend;
	szSend.Format(_T("shd_ad0=%d"),nValue&0xff);	//値設定
	bReval = m_pSerialComm->SendData(szSend);
DisplayListBox(szSend);
	if( bReval )
	{
		szSend.Format(_T("shd_ad1=%d"),(nValue>>8)&3);	//値設定
		bReval = m_pSerialComm->SendData(szSend);
DisplayListBox(szSend);
	}
	if( bReval )
	{
		Sleep(100);
		szSend.Format(_T("shd_set"));	//値設定
		bReval = m_pSerialComm->SendData(szSend);	//確定
DisplayListBox(szSend);
	}
	return bReval;
}


BOOL StLightControlDlg::SendInitValue(void)
{
	BOOL bReval = FALSE;
	if( !m_pSerialComm ) return FALSE;
	INT nShade = 0;
	INT nColorLength = m_szColor.GetLength();
	if( nColorLength==0 ) return FALSE;
	for( int i=0; i<nColorLength; i++ )
	{
		if( m_szColor.Mid(i,1).Compare(_T("R"))==0 ) nShade |= 1;
		else if( m_szColor.Mid(i,1).Compare(_T("G"))==0 ) nShade |= 2;
		else if( m_szColor.Mid(i,1).Compare(_T("B"))==0 ) nShade |= 4;
	}
	if( nShade==0 ) return FALSE;
	CString szSend;
	szSend.Format(_T("shade=%d"),nShade);	//照明をつける色設定
	bReval = m_pSerialComm->SendData(szSend);
DisplayListBox(szSend);
	if( !bReval ) return FALSE;

	//初期値設定
	for( int i=0; i<3; i++ )
	{
		if( ((nShade>>i)&1)==0 )
		{
			continue;
		}
		bReval = SetShadeGo(i);
		if( !bReval ) return FALSE;
		bReval = SetShadeValue(m_nInitValue[i]);
		if( !bReval ) return FALSE;
	}

	return bReval;
}


void StLightControlDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: ここにメッセージ ハンドラー コードを追加するか、既定の処理を呼び出します。
	if( nIDEvent==ST_DISPLAYTEXT )
	{
		DisplayBufferListBox();
	}
	//else
	//if( nIDEvent==ST_LIGHTCONTROLEXECUTE )
	//{


	//}

	//else
	//if( nIDEvent==ST_LightControlThread )
	//{
	//	KillTimer(ST_LightControlThread);
	//	m_hThread = (HANDLE)_beginthreadex( NULL, 0, LightControlDlgThread, this,  0, &m_mThreadID );

	//}

	__super::OnTimer(nIDEvent);
}

void StLightControlDlg::DisplayBufferListBox(void)
{
	::WaitForSingleObject(hDisplayMutex,INFINITE);

	HWND hListBox = ::GetDlgItem(m_hWnd, IDC_LIST_LINETOLINE);
	CListBox *pListBox = (CListBox *)CWnd::FromHandle( hListBox );

	UpdateData(TRUE);
	for( DWORD i=0; i<m_vDisplayString.size(); i++ )
	{
		int nIndex = pListBox->AddString(m_vDisplayString[i]);
		UpdateLogListBoxScrollBar(pListBox);
		pListBox->UpdateWindow();
	}
	UpdateData(FALSE);

	m_vDisplayString.clear();

	::ReleaseMutex(hDisplayMutex);
}

void StLightControlDlg::DisplayListBox(CString szMessge)
{

//TRACE(TEXT("@@@@@@@@@ DisplayListBox %s\n"), szMessge );

	::WaitForSingleObject(hDisplayMutex,INFINITE);

	m_vDisplayString.push_back(szMessge);

	::ReleaseMutex(hDisplayMutex);

}

//▼1.0.0.1025
BOOL StLightControlDlg::GetFunction(LPCTSTR szFunc, LPCTSTR szArg, double &dblValue)
{
	BOOL bReval = FALSE;
	return bReval;
}
//▲1.0.0.1025

