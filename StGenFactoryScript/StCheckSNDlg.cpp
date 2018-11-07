// StCheckSNDlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "StGenFactoryScript.h"
#include "StCheckSNDlg.h"
#include "afxdialogex.h"
#include "ResourceString.h"
#include "StCommon.h"
//▼1.0.0.1056
#include "StCheckCommon.h"

#define _ST_EXIT 1
//▲1.0.0.1056

unsigned __stdcall CheckSNThread( void *pParam )
{
	StCheckSNDlg * aDlg = (StCheckSNDlg *)pParam;
	do
	{
		if( aDlg->GetThreadStop() )
			break;

		BOOL bReval = aDlg->CheckExecute();

		Sleep(200);	//高速で動かすとOKボタンが押せなくなる。

	}while(1);

	_endthreadex(0);
	return 0;
}


// StCheckSNDlg ダイアログ

IMPLEMENT_DYNAMIC(StCheckSNDlg, CDialogEx)

StCheckSNDlg::StCheckSNDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(StCheckSNDlg::IDD, pParent)
{
	Initialize();
	m_hMessageMutex = CreateMutex(NULL,FALSE,NULL); 
}

StCheckSNDlg::StCheckSNDlg( LPCTSTR szClassName, LPCTSTR szVarName, StDeviceBase *aDeviceBase, CDialogEx *aDialogEx, CWnd* pParent /*=NULL*/)
	: CDialogEx(StCheckSNDlg::IDD, pParent)
	, StCheckBase( szClassName, szVarName, aDeviceBase, aDialogEx )
{
	Initialize();
	m_hMessageMutex = CreateMutex(NULL,FALSE,NULL); 
}

StCheckSNDlg::~StCheckSNDlg()
{
	CloseHandle(m_hMessageMutex);
}

void StCheckSNDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(StCheckSNDlg, CDialogEx)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDOK, &StCheckSNDlg::OnBnClickedOk)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// StCheckSNDlg メッセージ ハンドラー


BOOL StCheckSNDlg::OnInitDialog()
{
	__super::OnInitDialog();

	// TODO:  ここに初期化を追加してください
	//CResourceString::SetResourceText(this, IDS_INPUTSERIAL);

	m_Color = RGB( 0,0,0 );
	CStatic objStatic;
	objStatic.Attach(GetDlgItem(IDC_STATIC_JUDGE_SN)->m_hWnd);

	m_pDefFont = objStatic.GetFont();
	//フォントの設定
	m_boldFont.CreateFont( 30, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
					  SHIFTJIS_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS,
					  DEFAULT_QUALITY, DEFAULT_PITCH, _T("ＭＳ Ｐゴシック") );

	objStatic.Detach();

	SetLimitText();


	//検査Thread作成-------------
	//▼1.0.0.1056
	m_bResultNG = FALSE;
	//▲1.0.0.1056
	m_hThread = (HANDLE)_beginthreadex( NULL, 0, CheckSNThread, this,  0, &m_mThreadID );



	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}


HBRUSH StCheckSNDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = __super::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  ここで DC の属性を変更してください。
	if( pWnd->GetDlgCtrlID() == IDC_STATIC_JUDGE_SN )
	{
		pDC->SetTextColor(m_Color);
	}

	// TODO:  既定値を使用したくない場合は別のブラシを返します。
	return hbr;
}

void StCheckSNDlg::Initialize(void)
{
	m_dblPermAverageMin = 180.0;
	m_dblPermAverageMax = 220.0;

	m_dblPermSNMax = 3.0;
	m_dblPermSN = 3.0;

	m_nStart = 0;
	m_nEnd = 4095;
	//▼1.0.0.1056
	m_nMode = 0;		//0:従来モード 1:NGがでたらストップモード

	m_dblPermLineAverageMin = -1.0;
	m_dblPermLineAverageMax = -1.0;

	m_bResultNG = FALSE;
	//▲1.0.0.1056

	m_bThreadStop = FALSE;
}


BOOL StCheckSNDlg::JudgeText(BOOL bJudge)
{
	m_Color = bJudge?RGB(0,255,0):RGB(255,0,0);

	CString szText = bJudge?_T("OK"):_T("NG");
	CStatic objStatic;
	objStatic.Attach(GetDlgItem(IDC_STATIC_JUDGE_SN)->m_hWnd);
	objStatic.SetWindowText(szText);
	objStatic.SetFont( &m_boldFont, TRUE );
	objStatic.Detach();
	return TRUE;
}


BOOL StCheckSNDlg::ResultText(void)
{
	INT nElement = GetElement();
	CString szText(_T("")), szTmp;
	LPCTSTR aColor[] = { _T("R:"), _T("G:"), _T("B:") };

	for( int i=0; i<nElement; i++ )
	{
		//▼1.0.0.1056
		//szTmp.Format( _T("LvAve[%.2f] SNAve[%.4f] SNMax[%.4f] atpix[%d]\n"), m_dblResultAverage[i], m_dblResultSN[i], m_dblResultSNMax[i], m_nResultSDMPix[i]);
		double dblResultAverage = m_dblResultAverage[i];
		double dblResultSN = m_dblResultSN[i];
		double dblResultSNMax = m_dblResultSNMax[i];
		INT nResultSDMPix = m_nResultSDMPix[i];
		double dblReslutLineAverageMin = m_dblReslutLineAverageMin[i];
		double dblReslutLineAverageMax = m_dblReslutLineAverageMax[i];
		INT nResultLineAverageMinPos = m_nResultLineAverageMinPos[i];
		INT nResultLineAverageMaxPos = m_nResultLineAverageMaxPos[i];
		if( m_bResultNG )
		{
			dblResultAverage = m_dblResultNGAverage[i];
			dblResultSN = m_dblResultNGSN[i];
			dblResultSNMax = m_dblResultNGSNMax[i];
			nResultSDMPix = m_nResultNGSDMPix[i];
			dblReslutLineAverageMin = m_dblReslutNGLineAverageMin[i];
			dblReslutLineAverageMax = m_dblReslutNGLineAverageMax[i];
			nResultLineAverageMinPos = m_nResultNGLineAverageMinPos[i];
			nResultLineAverageMaxPos = m_nResultNGLineAverageMaxPos[i];
		}
		szTmp.Format( _T("LvAve[%.2f] SNAve[%.4f] SNMax[%.4f] atpix[%d]\n   LineAve[%.4f-%.4f] pix[%d-%d]\n"), dblResultAverage, dblResultSN, dblResultSNMax, nResultSDMPix, dblReslutLineAverageMin, dblReslutLineAverageMax, nResultLineAverageMinPos, nResultLineAverageMaxPos);
		//▲1.0.0.1056
		if( nElement==3 )
		{
			szTmp = aColor[GetElementColorIndex(i)] + szTmp;
		}
		szText += szTmp;
	}
	//▼1.0.0.1056
	//CStatic objStatic;
	//objStatic.Attach(GetDlgItem(IDC_STATIC_RESULT_SN)->m_hWnd);
	//objStatic.SetWindowText(szText);
	//objStatic.Detach();

	if( !GetThreadStop() )
	{
		CStatic objStatic;
		objStatic.Attach(GetDlgItem(IDC_STATIC_RESULT_SN)->m_hWnd);
		objStatic.SetWindowText(szText);
		objStatic.Detach();
	}
	//▲1.0.0.1056
	return TRUE;
}

BOOL StCheckSNDlg::SetLimitText(void)
{
	BOOL bReval = FALSE;
	CString szText;
	//▼1.0.0.1056
	//szText.Format( _T("Lv[%.2f]～[%.2f]\nSN<[%.2f]\nMAX<[%.2f]"), m_dblPermAverageMin, m_dblPermAverageMax, m_dblPermSN, m_dblPermSNMax );
	szText.Format( _T("Lv[%.2f]～[%.2f]\nSN<[%.2f]\nMAX<[%.2f]\nLineAve[%.2f]～[%.2f]"), m_dblPermAverageMin, m_dblPermAverageMax, m_dblPermSN, m_dblPermSNMax, m_dblPermLineAverageMin, m_dblPermLineAverageMax );
	//▲1.0.0.1056
	CStatic objStatic;
	objStatic.Attach(GetDlgItem(IDC_STATIC_LIMIT_SN)->m_hWnd);
	objStatic.SetWindowText(szText);
	objStatic.Detach();
	return bReval;
}

void StCheckSNDlg::SetStart(INT nData)
{
	m_nStart = nData;
}

void StCheckSNDlg::SetEnd(INT nData)
{
	m_nEnd = nData;
}

void StCheckSNDlg::SetAverageMin(double dblData)
{
	m_dblPermAverageMin = dblData;
}

void StCheckSNDlg::SetAverageMax(double dblData)
{
	m_dblPermAverageMax = dblData;
}

void StCheckSNDlg::SetSNMax(double dblData)
{
	m_dblPermSNMax = dblData;
}

void StCheckSNDlg::SetSN(double dblData)
{
	m_dblPermSN = dblData;
}

BOOL StCheckSNDlg::function(LPCTSTR szFunc, LPCTSTR szArgument)
{
	BOOL bReval = FALSE;
	CString szFunction(szFunc);
	if( szFunction == _T("Execute") )
	{
	}
	else
	if( szFunction=="SetAverageMin" )
	{
		double dblData = _ttof(szArgument);
		SetAverageMin(dblData);
		bReval = TRUE;
	}
	else
	if( szFunction=="SetAverageMax" )
	{
		double dblData = _ttof(szArgument);
		SetAverageMax(dblData);
		bReval = TRUE;
	}
	else
	if( szFunction=="SetSNMax" )
	{
		double dblData = _ttof(szArgument);
		SetSNMax(dblData);
		bReval = TRUE;
	}
	else
	if( szFunction=="SetSN" )
	{
		double dblData = _ttof(szArgument);
		SetSN(dblData);
		bReval = TRUE;
	}
	else
	if( szFunction=="SetStartLine" )
	{
		INT nData = _tcstol(szArgument,NULL,0);
		SetStart(nData);
		bReval = TRUE;
	}
	else
	if( szFunction=="SetEndLine" )
	{
		INT nData = _tcstol(szArgument,NULL,0);
		SetEnd(nData);
		bReval = TRUE;
	}
	//▼1.0.0.1056
	else
	if( szFunction=="SetLineAverageMin" )
	{
		m_dblPermLineAverageMin = _ttof(szArgument);
		//カラーにも同じ値を入れる。
		for( int i=0; i<_countof(m_dblPermLineAverageColorMin); i++ )
			m_dblPermLineAverageColorMin[i] = m_dblPermLineAverageMin;

		bReval = TRUE;
	}
	else
	if( szFunction=="SetLineAverageMax" )
	{
		m_dblPermLineAverageMax = _ttof(szArgument);
		//カラーにも同じ値を入れる。
		for( int i=0; i<_countof(m_dblPermLineAverageColorMax); i++ )
			m_dblPermLineAverageColorMax[i] = m_dblPermLineAverageMax;
		bReval = TRUE;
	}
	else
	if( szFunction=="SetAverageRedMin" )
	{
		m_dblPermAverageColorMin[0] = _ttof(szArgument);
		bReval = TRUE;
	}
	else
	if( szFunction=="SetAverageGreenMin" )
	{
		m_dblPermAverageColorMin[1] = _ttof(szArgument);
		bReval = TRUE;
	}
	else
	if( szFunction=="SetAverageBlueMin" )
	{
		m_dblPermAverageColorMin[2] = _ttof(szArgument);
		bReval = TRUE;
	}
	else
	if( szFunction=="SetAverageRedMax" )
	{
		m_dblPermAverageColorMax[0] = _ttof(szArgument);
		bReval = TRUE;
	}
	else
	if( szFunction=="SetAverageGreenMax" )
	{
		m_dblPermAverageColorMax[1] = _ttof(szArgument);
		bReval = TRUE;
	}
	else
	if( szFunction=="SetAverageBlueMax" )
	{
		m_dblPermAverageColorMax[2] = _ttof(szArgument);
		bReval = TRUE;
	}

	else
	if( szFunction=="SetLineAverageRedMin" )
	{
		m_dblPermLineAverageColorMin[0] = _ttof(szArgument);
		bReval = TRUE;
	}
	else
	if( szFunction=="SetLineAverageGreenMin" )
	{
		m_dblPermLineAverageColorMin[1] = _ttof(szArgument);
		bReval = TRUE;
	}
	else
	if( szFunction=="SetLineAverageBlueMin" )
	{
		m_dblPermLineAverageColorMin[2] = _ttof(szArgument);
		bReval = TRUE;
	}
	else
	if( szFunction=="SetLineAverageRedMax" )
	{
		m_dblPermLineAverageColorMax[0] = _ttof(szArgument);
		bReval = TRUE;
	}
	else
	if( szFunction=="SetLineAverageGreenMax" )
	{
		m_dblPermLineAverageColorMax[1] = _ttof(szArgument);
		bReval = TRUE;
	}
	else
	if( szFunction=="SetLineAverageBlueMax" )
	{
		m_dblPermLineAverageColorMax[2] = _ttof(szArgument);
		bReval = TRUE;
	}
	else
	if( szFunction=="Mode" || szFunction=="SetMode" )
	{
		m_nMode = _ttoi(szArgument);
		bReval = TRUE;
	}
	else
	if( szFunction=="ImageFileName" || szFunction=="SetImageFileName" )
	{
		m_szImageFileName = szArgument;
		bReval = TRUE;
	}
	//▲1.0.0.1056


	return bReval;
}

BOOL StCheckSNDlg::CheckExecute(void)
{

	BOOL bReval = FALSE;
	m_iJudge = 2;	//2:処理中
	if( m_Buffer ) delete m_Buffer;
	m_Buffer = new StBuffer;
	bReval = m_DeviceBase->GetStBuffer(m_Buffer);	//変換して取得もありかも。現時点ではコピー

	if( bReval )
		//▼1.0.0.1056
		//bReval = GetSNAverage( m_Buffer, m_nStart, m_nEnd, m_dblResultAverage, m_dblResultSN, m_dblResultSNMax, m_nResultSDMPix );
		bReval = GetSNAverage( m_Buffer, m_nStart, m_nEnd, m_dblResultAverage, m_dblResultSN, m_dblResultSNMax, m_nResultSDMPix, m_dblReslutLineAverageMin, m_dblReslutLineAverageMax, m_nResultLineAverageMinPos, m_nResultLineAverageMaxPos );
		//▲1.0.0.1056
	if( bReval )
	{
		m_nElement = GetElementFromBuffer(m_Buffer);
		SetElementColorIndexFromBuffer(m_Buffer);
		for( int i=0; i<m_nElement && m_iJudge==2; i++ )
		{
			//m_nColorIndex[i] = GetElementColorIndexFromBuffer(m_Buffer, i);

			if( m_dblResultAverage[i]<m_dblPermAverageMin || m_dblPermAverageMax<m_dblResultAverage[i] )
			{
				m_iJudge=0;
			}
			if( m_dblPermSNMax<m_dblResultSNMax[i] )
			{
				m_iJudge=0;
			}
			if( m_dblPermSN<m_dblResultSN[i] )
			{
				m_iJudge=0;
			}
			//▼1.0.0.1056
			if( m_dblPermLineAverageMin>=0.0 )
			{
				if( m_dblPermLineAverageMin>m_dblReslutLineAverageMin[i] )
				{
					m_iJudge=0;
				}
			}
			if( m_dblPermLineAverageMax>=0.0 )
			{
				if( m_dblPermLineAverageMax<m_dblReslutLineAverageMax[i] )
				{
					m_iJudge=0;
				}
			}
			//▲1.0.0.1056
		}
		if( m_iJudge==2 ) m_iJudge = 1;	//1:OK

		//▼1.0.0.1056

		if( m_iJudge==0 && m_nMode==1 && !m_bResultNG )
		{
			for( int i=0; i<m_nElement; i++ )
			{
				m_dblResultNGAverage[i] = m_dblResultAverage[i];
				m_dblResultNGSN[i] = m_dblResultSN[i];
				m_dblResultNGSNMax[i] = m_dblResultSNMax[i];
				m_nResultNGSDMPix[i] = m_nResultSDMPix[i];

				m_dblReslutNGLineAverageMin[i] = m_dblReslutLineAverageMin[i];
				m_dblReslutNGLineAverageMax[i] = m_dblReslutLineAverageMax[i];
				m_nResultNGLineAverageMinPos[i] = m_nResultLineAverageMinPos[i];
				m_nResultNGLineAverageMaxPos[i] = m_nResultLineAverageMaxPos[i];
			}
			m_bResultNG = TRUE;

			//この時にBufferに画像保存----------
			if( m_szImageFileName.GetLength()>0 )
			{
				mNGImageBuffer.CreateCopy(*m_Buffer);
			}
		}
		if( m_bResultNG ) m_iJudge = 0;
		//▲1.0.0.1056

		//結果数値表示
		ResultText();
	}
	else
	{
		m_iJudge = 0;	//0:NG
	}

	//結果表示
	JudgeText(m_iJudge==1);

	return bReval;

}


void StCheckSNDlg::OnBnClickedOk()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	m_bThreadStop = TRUE;



	//▼1.0.0.1056
	//SaveBitmapMain();
	//▲1.0.0.1056

	//▼1.0.0.1056
	//::WaitForSingleObject( m_hThread, INFINITE);
	//タイムラグをいれないとフリーズする。。
	SetTimer(_ST_EXIT, 200, NULL);
	//CDialogEx::OnOK();
	//▲1.0.0.1056

}

//▼1.0.0.1025
BOOL StCheckSNDlg::GetFunction(LPCTSTR szFunc, LPCTSTR szArg, double &dblValue)
{
	BOOL bReval = FALSE;
	CString szFunction(szFunc);
	if( szFunction=="GetAverage" )
	{
		INT nIndex = _ttoi(szArg);
		if( nIndex>=0 && nIndex<_countof(m_dblResultAverage) )
		{
			dblValue = m_dblResultAverage[nIndex];
			bReval = TRUE;
		}
	}
	else
	if( szFunction=="GetSN" )
	{
		INT nIndex = _ttoi(szArg);
		if( nIndex>=0 && nIndex<_countof(m_dblResultSN) )
		{
			dblValue = m_dblResultSN[nIndex];
			bReval = TRUE;
		}
	}
	else
	if( szFunction=="GetSNMax" )
	{
		INT nIndex = _ttoi(szArg);
		if( nIndex>=0 && nIndex<_countof(m_dblResultSNMax) )
		{
			dblValue = m_dblResultSNMax[nIndex];
			bReval = TRUE;
		}
	}
	else
	if( szFunction=="GetSDMPix" )
	{
		INT nIndex = _ttoi(szArg);
		if( nIndex>=0 && nIndex<_countof(m_nResultSDMPix) )
		{
			dblValue = m_nResultSDMPix[nIndex];
			bReval = TRUE;
		}
	}

	//▼1.0.0.1056
	else
	if( szFunction=="GetLineAverageMin" )
	{
		INT nIndex = _ttoi(szArg);
		if( nIndex>=0 && nIndex<_countof(m_dblReslutLineAverageMin) )
		{
			dblValue = m_dblReslutLineAverageMin[nIndex];
			bReval = TRUE;
		}
	}
	else
	if( szFunction=="GetLineAverageMax" )
	{
		INT nIndex = _ttoi(szArg);
		if( nIndex>=0 && nIndex<_countof(m_dblReslutLineAverageMax) )
		{
			dblValue = m_dblReslutLineAverageMax[nIndex];
			bReval = TRUE;
		}
	}
	else
	if( szFunction=="GetLineAverageMinPos" )
	{
		INT nIndex = _ttoi(szArg);
		if( nIndex>=0 && nIndex<_countof(m_nResultLineAverageMinPos) )
		{
			dblValue = m_nResultLineAverageMinPos[nIndex];
			bReval = TRUE;
		}
	}
	else
	if( szFunction=="GetLineAverageMaxPos" )
	{
		INT nIndex = _ttoi(szArg);
		if( nIndex>=0 && nIndex<_countof(m_nResultLineAverageMaxPos) )
		{
			dblValue = m_nResultLineAverageMaxPos[nIndex];
			bReval = TRUE;
		}
	}
	//▲1.0.0.1056


	return bReval;
}
//▲1.0.0.1025

//▼1.0.0.1056
void StCheckSNDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: ここにメッセージ ハンドラー コードを追加するか、既定の処理を呼び出します。

	if( nIDEvent==_ST_EXIT )
	{
		KillTimer(_ST_EXIT);
		::WaitForSingleObject( m_hThread, INFINITE);

		//画像保存
		if( mNGImageBuffer.GetSize()>0)
		{
			StCheckCommon aCommon( _T("StCheckCommon"), _T("aCheckCommonDammy"), m_DeviceBase);
			aCommon.SetSaveFilePath(mSaveFilePath);
			aCommon.function(_T("SetFileName"),m_szImageFileName);
			//▼1.0.0.1057
			//aCommon.SaveBitmap( &mNGImageBuffer, NULL );
			CTime gt=CTime::GetCurrentTime();
			CString dateFormat = gt.Format("%Y%m%d%H%M%S");
			aCommon.SaveBitmap( &mNGImageBuffer, dateFormat );
			aCommon.SaveCSV( &mNGImageBuffer, dateFormat );
			//▲1.0.0.1057
		}
		CDialogEx::OnOK();
		return;
	}
	__super::OnTimer(nIDEvent);
}
//▲1.0.0.1056

