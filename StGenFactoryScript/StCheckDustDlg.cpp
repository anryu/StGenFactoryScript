// StCheckDustDlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "StGenFactoryScript.h"
#include "StCheckDustDlg.h"
#include "afxdialogex.h"
#include "StCommon.h"

unsigned __stdcall CheckDustThread( void *pParam )
{
INT nCount=0;


	StCheckDustDlg * aDlg = (StCheckDustDlg *)pParam;
	do
	{
		if( aDlg->GetThreadStop() )
			break;

		{
//TCHAR szText[64];
//_stprintf_s(szText,_countof(szText),_T("IN aDlg->CheckExecute(%d)\n"),nCount++);
//TRACE(szText);
		}
		BOOL bReval = aDlg->CheckExecute();
		{
//TCHAR szText[64];
//_stprintf_s(szText,_countof(szText),_T("OUT aDlg->CheckExecute(%d)\n"),nCount++);
//TRACE(szText);
		}
		Sleep(200);	//高速で動かすとOKボタンが押せなくなる。

	}while(1);

	_endthreadex(0);
	return 0;
}

// StCheckDustDlg ダイアログ

IMPLEMENT_DYNAMIC(StCheckDustDlg, CDialogEx)

StCheckDustDlg::StCheckDustDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(StCheckDustDlg::IDD, pParent)
{
	Initialize();
}

StCheckDustDlg::StCheckDustDlg( LPCTSTR szClassName, LPCTSTR szVarName, StDeviceBase *aDeviceBase, CDialogEx *aDialogEx, CWnd* pParent /*=NULL*/)
	: CDialogEx(StCheckDustDlg::IDD, pParent)
	, StCheckBase( szClassName, szVarName, aDeviceBase, aDialogEx )
{
	Initialize();
}

StCheckDustDlg::~StCheckDustDlg()
{
	Release();
}

void StCheckDustDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(StCheckDustDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &StCheckDustDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &StCheckDustDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON_START, &StCheckDustDlg::OnBnClickedButtonStart)
	ON_BN_CLICKED(IDC_BUTTON_STOP, &StCheckDustDlg::OnBnClickedButtonStop)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// StCheckDustDlg メッセージ ハンドラー


void StCheckDustDlg::OnBnClickedOk()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。

	//BMP保存
	//▼1.0.0.1026
	//if( m_WorstBuffer )
	//{
	//	if( m_szBMPFileName.GetLength()>0 )
	//	{
	//		CString szDeviceModelName;
	//		CString szSerialNumber;
	//		BOOL bReval;
	//		StString aText;
	//		bReval = GetDeviceModelName(aText);
	//		if( bReval )
	//			szDeviceModelName = aText.GetUnicode();
	//		bReval = GetSerialNumber(aText);
	//		if( bReval )
	//			szSerialNumber = aText.GetUnicode();

	//		CString szFileName;
	//		CTime gt=CTime::GetCurrentTime();
	//		CString dateFormat = gt.Format("%Y%m%d%H%M%S");
	//		szFileName = szDeviceModelName;
	//		if( szSerialNumber.GetLength()>0 )
	//		{
	//			szFileName += _T("_") + szSerialNumber;
	//		}
	//		szFileName += _T("_") + dateFormat + _T("_") + m_szBMPFileName;
	//		if(  szFileName.Find(_T("."))<0 )
	//		{
	//			szFileName += _T(".bmp");
	//		}

	//		dateFormat = gt.Format("%Y%m%d");
	//		CString szFullPath = mSaveFilePath + _T("\\") + szDeviceModelName + _T("\\") + dateFormat + _T("\\bmp\\");

	//		CStCommon::CheckDirectoryExist(szFullPath,TRUE);

	//		CString szFullFileName = szFullPath + szFileName;

	//		SaveBitmapFile(m_WorstBuffer, szFullFileName);
	//	}

	//}

	SaveBitmapMain();
	//▲1.0.0.1026

	CDialogEx::OnOK();
}


void StCheckDustDlg::OnBnClickedCancel()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	CDialogEx::OnCancel();
}


BOOL StCheckDustDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  ここに初期化を追加してください
	EnableButton( !m_bThreadStop, TRUE );

	m_Color = RGB( 0,0,0 );
	CStatic objStatic;
	objStatic.Attach(GetDlgItem(IDC_STATIC_JUDGE_DUST)->m_hWnd);

	m_pDefFont = objStatic.GetFont();
	//フォントの設定
	m_boldFont.CreateFont( 30, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
					  SHIFTJIS_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS,
					  DEFAULT_QUALITY, DEFAULT_PITCH, _T("ＭＳ Ｐゴシック") );

	objStatic.Detach();

	SetLimitText();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}


void StCheckDustDlg::OnBnClickedButtonStart()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	if( m_hThread ) return;
	//m_bStart = TRUE;
	m_bThreadStop = FALSE;

	//BMP保存用
	if( m_WorstBuffer )
	{
		delete m_WorstBuffer;
		m_WorstBuffer = NULL;
	}

	//▼1.0.0.1015
	//m_dblWorstDifference = 0.0;
	m_dblWorstDifference = -1.0;
	//▲1.0.0.1015

	//▼1.0.0.1043
	//▼1.0.0.1045
	//m_nWorstContinuousCount = 0;
	m_nWorstContinuousCount = -1;
	//▲1.0.0.1045
	//▲1.0.0.1043




	m_iJudge = 2;	//2:処理中

	//検査Thread作成-------------
	m_hThread = (HANDLE)_beginthreadex( NULL, 0, CheckDustThread, this,  0, &m_mThreadID );
	EnableButton(!m_bThreadStop);
}


void StCheckDustDlg::OnBnClickedButtonStop()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	m_bThreadStop = TRUE;

	::WaitForSingleObject( m_hThread, INFINITE);

	//▼1.0.0.1028
	//最終結果の表示をここでさせる。
	//結果表示
	JudgeText(m_iJudge==1);
	//結果値表示
	ResultText();

	if( m_dblDisplayAverage>=0.0 )
	{
		AverageText( m_dblDisplayAverage, m_iResultElement );	//赤
		m_dblDisplayAverage = -1.0;
	}
	//▲1.0.0.1028
	//▼1.0.0.1043
	CloseHandle(m_hThread);
	//▲1.0.0.1043
	m_hThread = NULL;
	//m_bStart = FALSE;
	EnableButton(!m_bThreadStop);
}


HBRUSH StCheckDustDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = __super::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  ここで DC の属性を変更してください。
	if( pWnd->GetDlgCtrlID() == IDC_STATIC_JUDGE_DUST )
	{
		pDC->SetTextColor(m_Color);
	}

	// TODO:  既定値を使用したくない場合は別のブラシを返します。
	return hbr;
}

void StCheckDustDlg::Initialize(void)
{
	//m_bStart = FALSE;
	m_bThreadStop = TRUE;
	m_hThread = NULL;

	m_iResultElement = 0;
	m_pdblResultDifferenceRatio = NULL;
	m_piResultLineRatio = NULL;
	//▼1.0.0.1024
	m_pdblResultAverage = NULL;
	//▲1.0.0.1024

	//BMP保存
	m_dblWorstDifference = 0.0;
	m_WorstBuffer = NULL;

	//mDeviceModelName = "";
	//mSerialNumber = "";
	mSaveFilePath = ".";

	//▼1.0.0.1013
	m_nStart = 0;
	m_nEnd = 4095;
	m_nWidthLines = 4096;
	m_nSeparate = 1;
	//▲1.0.0.1013
	//▼1.0.0.1041
	m_nCheckMode = 0;
	//▲1.0.0.1041

	//▼1.0.0.1026
	m_nMode = 0;
	//▲1.0.0.1026

	//▼1.0.0.1028
	m_dblDisplayAverage = -1.0;
	//▲1.0.0.1028

	//▼1.0.0.1039
	m_szBMPFileName.Empty();
	m_szCSVFileName.Empty();
	m_szPNGFileName.Empty();
	//▲1.0.0.1039

	//▼1.0.0.1043
	m_nContinuousCountMax = 0;
	m_pnResultContinuousCount = NULL;
	m_pnResultContinuousStart = NULL;
	m_pnResultContinuousEnd = NULL;

	//▼1.0.0.1045
	//m_nWorstContinuousCount = 0;
	m_nWorstContinuousCount = -1;
	//▲1.0.0.1045
	//▲1.0.0.1043

	//▼1.0.0.1058
	m_dblAverageLowLevel = 0.0;
	m_dblAverageHighLevel = 0.0;
	m_dblDifferenceMax = 0.0;
	//▲1.0.0.1058


}

void StCheckDustDlg::Release(void)
{
	if( m_pdblResultDifferenceRatio )
	{
		delete [] m_pdblResultDifferenceRatio;
		m_pdblResultDifferenceRatio = NULL;
	}
	if( m_piResultLineRatio )
	{
		delete [] m_piResultLineRatio;
		m_piResultLineRatio = NULL;
	}

	//▼1.0.0.1024
	if( m_pdblResultAverage )
	{
		delete [] m_pdblResultAverage;
		m_pdblResultAverage = NULL;
	}
	//▲1.0.0.1024
	//▼1.0.0.1043
	if( m_pnResultContinuousCount ) delete [] m_pnResultContinuousCount;
	m_pnResultContinuousCount = NULL;
	if( m_pnResultContinuousStart ) delete [] m_pnResultContinuousStart;
	m_pnResultContinuousStart = NULL;
	if( m_pnResultContinuousEnd ) delete [] m_pnResultContinuousEnd;
	m_pnResultContinuousEnd = NULL;
	//▲1.0.0.1043

	if( m_WorstBuffer )
	{
		delete m_WorstBuffer;
		m_WorstBuffer = NULL;
	}

}

BOOL StCheckDustDlg::JudgeText(BOOL bJudge)
{
	m_Color = bJudge?RGB(0,255,0):RGB(255,0,0);

	CString szText = bJudge?_T("OK"):_T("NG");
	CStatic objStatic;
	objStatic.Attach(GetDlgItem(IDC_STATIC_JUDGE_DUST)->m_hWnd);
	objStatic.SetWindowText(szText);
	objStatic.SetFont( &m_boldFont, TRUE );
	objStatic.Detach();
	return TRUE;
}

BOOL StCheckDustDlg::ResultText(void)
{
	CString szText(_T("")), szTmp;
	LPCTSTR aColor[] = { _T("R:"), _T("G:"), _T("B:") };


	for( int i=0; i<m_iResultElement; i++ )
	{
		//▼1.0.0.1058
		//szTmp.Format( _T("最大平均差比[%.2f%%] Line[%d]\n"), m_pdblResultDifferenceRatio[i], m_piResultLineRatio[i] );
		if( m_nCheckMode==0 )
		{
			szTmp.Format( _T("最大平均差[%.2f] Line[%d]\n"), m_pdblResultDifferenceRatio[i], m_piResultLineRatio[i] );
		}
		else
		{
			szTmp.Format( _T("最大平均差比[%.2f%%] Line[%d]\n"), m_pdblResultDifferenceRatio[i], m_piResultLineRatio[i] );
		}
		//▲1.0.0.1058
		if( m_iResultElement==3 )
		{
			szTmp = aColor[GetElementColorIndex(i)] + szTmp;
		}
		szText += szTmp;
		//▼1.0.0.1043
		if( m_nCheckMode==2 )
		{
			szTmp.Format( _T(" 連続画素数[%i]"), m_pnResultContinuousCount[i] );
			szText += szTmp;
			if( m_pnResultContinuousStart[i]>=0 )
			{
				szTmp.Format( _T(" (%i-%i)\n"), m_pnResultContinuousStart[i], m_pnResultContinuousEnd[i] );
				szText += szTmp;
			}
			szText+=_T("\n");
		}
		//▲1.0.0.1043
	}

	//for( int i=0; i<nElement; i++ )
	//{
	//	szTmp.Format( _T("LvAve[%.2f] SNAve[%.4f] SNMax[%.4f] atpix[%d]\n"), m_dblResultAverage[i], m_dblResultSN[i], m_dblResultSNMax[i], m_nResultSDMPix[i]);
	//	if( nElement==3 )
	//	{
	//		szTmp = aColor[GetElementColorIndex(i)] + szTmp;
	//	}
	//	szText += szTmp;
	//}

	CStatic objStatic;
	objStatic.Attach(GetDlgItem(IDC_STATIC_RESULT_DUST)->m_hWnd);
	objStatic.SetWindowText(szText);
	objStatic.Detach();

	return TRUE;
}

BOOL StCheckDustDlg::SetLimitText(void)
{
	BOOL bReval = TRUE;
	CString szText;
	szText.Format( _T("最大平均差比[%.2f]\n[%.2f]<Average<[%.2f]"), m_dblDifferenceMax, m_dblAverageLowLevel, m_dblAverageHighLevel );
	CStatic objStatic;
	objStatic.Attach(GetDlgItem(IDC_STATIC_LIMIT_DUST)->m_hWnd);
	objStatic.SetWindowText(szText);
	objStatic.Detach();
	return bReval;
}

BOOL StCheckDustDlg::AverageText( double dblAverage, INT nElement )
{
	BOOL bReval = TRUE;
	CString szText(_T("Average "));
	szText += nElement == 1?_T("BW"):_T("Red");
	CString tmp;
	tmp.Format( _T("=%.2f\n"), dblAverage );
	szText += tmp;
	INT nJudge = 0;
	if( dblAverage<m_dblAverageLowLevel )
		nJudge = -1;
	else
	if( dblAverage>m_dblAverageHighLevel )
		nJudge = 1;
	szText += nJudge<0?_T("照明を明るくしてください。"):(nJudge>0?_T("照明を暗くしてください。"):_T(""));

	CStatic objStatic;
	objStatic.Attach(GetDlgItem(IDC_STATIC_AVERAGE_DUST)->m_hWnd);
	objStatic.SetWindowText(szText);
	objStatic.Detach();
	return bReval;
}


void StCheckDustDlg::SetWidthLines(INT nData)
{
	m_nWidthLines = nData;
}

void StCheckDustDlg::SetStart(INT nData)
{
	m_nStart = nData;
}

void StCheckDustDlg::SetEnd(INT nData)
{
	m_nEnd = nData;
}
void StCheckDustDlg::SetSeparate(INT nData)
{
	m_nSeparate = nData;
}

void StCheckDustDlg::SetDifferenceMax(double dblData)
{
	m_dblDifferenceMax = dblData;
}

void StCheckDustDlg::SetAverageLowLevel(double dblData)
{
	m_dblAverageLowLevel = dblData;
}

void StCheckDustDlg::SetAverageHighLevel(double dblData)
{
	m_dblAverageHighLevel = dblData;
}

void StCheckDustDlg::SetBMPFileName(LPCTSTR szData)
{
	m_szBMPFileName = szData;
}
//▼1.0.0.1039
void StCheckDustDlg::SetCSVFileName(LPCTSTR szData)
{
	m_szCSVFileName = szData;
}
void StCheckDustDlg::SetPNGFileName(LPCTSTR szData)
{
	m_szPNGFileName = szData;
}
//▲1.0.0.1039
//▼1.0.0.1041
void StCheckDustDlg::SetCheckMode(INT nData)
{
	m_nCheckMode = nData;
}
//▲1.0.0.1041
BOOL StCheckDustDlg::function(LPCTSTR szFunc, LPCTSTR szArgument)
{
	BOOL bReval = FALSE;
	CString szFunction(szFunc);
	if( szFunction == _T("Execute") )
	{
	}
	else
	if( szFunction=="SetWidthLines" )
	{
		INT nData = _tcstol(szArgument,NULL,0);
		SetWidthLines(nData);
		bReval = TRUE;
	}
	else
	if( szFunction=="SetDifferenceMax" )
	{
		double dblData = _ttof(szArgument);
		SetDifferenceMax(dblData);
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
	else
	if( szFunction=="SetSeparate" )
	{
		INT nData = _tcstol(szArgument,NULL,0);
		SetSeparate(nData);
		bReval = TRUE;
	}
	else
	if( szFunction=="SetAverageLowLevel" )
	{
		double dblData = _ttof(szArgument);
		SetAverageLowLevel(dblData);
		bReval = TRUE;
	}
	else
	if( szFunction=="SetAverageHighLevel" )
	{
		double dblData = _ttof(szArgument);
		SetAverageHighLevel(dblData);
		bReval = TRUE;
	}
	else
	if( szFunction=="SetBMPFileName" )
	{
		SetBMPFileName(szArgument);
		bReval = TRUE;
	}
	//▼1.0.0.1026
	else
	if( szFunction=="SetMode" || szFunction=="Mode" )
	{
		m_nMode = _ttoi(szArgument);
		bReval = TRUE;
	}
	//▲1.0.0.1026
	//▼1.0.0.1039
	else
	if( szFunction=="SetCSVFileName" )
	{
		SetCSVFileName(szArgument);
		bReval = TRUE;
	}
	else
	if( szFunction=="SetPNGFileName" )
	{
		SetPNGFileName(szArgument);
		bReval = TRUE;
	}
	//▲1.0.0.1039
	//▼1.0.0.1041
	else
	if( szFunction=="SetCheckMode" || szFunction=="CheckMode" )
	{
		INT nData = _tcstol(szArgument,NULL,0);
		SetCheckMode(nData);
		bReval = TRUE;
	}
	//▲1.0.0.1041
	//▼1.0.0.1043
	else
	if( szFunction=="SetContinuousCountMax" || szFunction=="ContinuousCountMax" )
	{
		INT nData = _tcstol(szArgument,NULL,0);
		SetContinuousCountMax(nData);
		bReval = TRUE;
	}
	//▲1.0.0.1043

	return bReval;
}

void StCheckDustDlg::EnableButton(BOOL bStart, BOOL bInit)
{
	CButton *pButton = (CButton *)GetDlgItem(IDC_BUTTON_START); 
	pButton->EnableWindow(!bStart);
	pButton = (CButton *)GetDlgItem(IDC_BUTTON_STOP); 
	pButton->EnableWindow(bStart);
	pButton = (CButton *)GetDlgItem(IDOK); 
	pButton->EnableWindow( !bInit && !bStart );
}

//▼1.0.0.1026
//BOOL StCheckDustDlg::CheckExecute(void)
BOOL StCheckDustDlg::CheckExecute(INT nDisplayMode)
//▲1.0.0.1026
{

	BOOL bReval = FALSE;
	//m_iJudge = 2;	//2:処理中
	if( m_Buffer ) delete m_Buffer;
	m_Buffer = new StBuffer;
	m_DeviceBase->GetStBuffer(m_Buffer);	//変換して取得もありかも。現時点ではコピー

	size_t nElement = GetElementFromBuffer(m_Buffer);
	double * pdblDifference = NULL;
	PINT piLine = NULL;
	double * pdblDifferenceRatio = NULL;
	PINT piLineRatio = NULL;

	double *pdblAverage = NULL;
	//▼1.0.0.1043
	PINT pnContinuousCount = NULL;
	PINT pnContinuousStart = NULL;
	PINT pnContinuousEnd = NULL;
	//▲1.0.0.1043
	do
	{
		BOOL bWorstJudge = FALSE;

		pdblDifference = new double [nElement];
		if( !pdblDifference ) break;
		piLine = new INT [nElement];
		if( !piLine ) break;
		pdblDifferenceRatio = new double [nElement];
		if( !pdblDifferenceRatio ) break;
		piLineRatio = new INT [nElement];
		if( !piLineRatio ) break;

		pdblAverage = new double [nElement];
		if( !pdblAverage ) break;
		for( int i=0; i<(int)nElement; i++ )
			pdblAverage[i] = -1.0;

		//▼1.0.0.1043
		////▼1.0.0.1041
		////bReval = GetWidthLineAverageDifference( m_Buffer, m_nStart, m_nEnd, m_nWidthLines, m_nSeparate, &nElement, pdblDifference, piLine, pdblDifferenceRatio, piLineRatio, pdblAverage );
		//bReval = GetWidthLineAverageDifference( m_Buffer, m_nStart, m_nEnd, m_nWidthLines, m_nSeparate, m_nCheckMode, &nElement, pdblDifference, piLine, pdblDifferenceRatio, piLineRatio, pdblAverage );
		////▲1.0.0.1041

		pnContinuousCount = new INT [nElement];
		if( !pnContinuousCount ) break;
		pnContinuousStart = new INT [nElement];
		if( !pnContinuousStart ) break;
		pnContinuousEnd = new INT [nElement];
		if( !pnContinuousEnd ) break;
		for( int i=0; i<(int)nElement; i++ )
		{
			//▼1.0.0.1058
			//pnContinuousCount[i] = 0;
			pnContinuousCount[i] = -1;
			//▲1.0.0.1058
			pnContinuousStart[i] = -1;
			pnContinuousEnd[i] = -1;
		}
		bReval = GetWidthLineAverageDifference( m_Buffer, m_nStart, m_nEnd, m_nWidthLines, m_nSeparate, m_nCheckMode, &nElement, pdblDifference, piLine, pdblDifferenceRatio, piLineRatio, pdblAverage, m_dblDifferenceMax, pnContinuousCount, pnContinuousStart, pnContinuousEnd  );
		//▲1.0.0.1043
		if( bReval )
		{
			if(m_iResultElement != nElement)
			{
				m_iResultElement = nElement;
				SetElementColorIndexFromBuffer(m_Buffer);

				if( m_pdblResultDifferenceRatio ) delete [] m_pdblResultDifferenceRatio;
				m_pdblResultDifferenceRatio = new double [m_iResultElement];
				if( m_piResultLineRatio ) delete [] m_piResultLineRatio;
				m_piResultLineRatio = new INT [m_iResultElement];
				//▼1.0.0.1024
				if( m_pdblResultAverage ) delete [] m_pdblResultAverage;
				m_pdblResultAverage = new double [m_iResultElement];
				//▲1.0.0.1024

				//▼1.0.0.1045
				memset(m_pdblResultDifferenceRatio,0,sizeof(double)*m_iResultElement);
				memset(m_piResultLineRatio,0,sizeof(INT)*m_iResultElement);
				memset(m_pdblResultAverage,0,sizeof(double)*m_iResultElement);
				//▲1.0.0.1045


				//▼1.0.0.1043
				if( m_pnResultContinuousCount ) delete [] m_pnResultContinuousCount;
				m_pnResultContinuousCount = new INT [m_iResultElement];
				if( m_pnResultContinuousStart ) delete [] m_pnResultContinuousStart;
				m_pnResultContinuousStart = new INT [m_iResultElement];
				if( m_pnResultContinuousEnd ) delete [] m_pnResultContinuousEnd;
				m_pnResultContinuousEnd = new INT [m_iResultElement];
				for( int i=0; i<m_iResultElement; i++ )
				{
					m_pnResultContinuousCount[i]=0;
					m_pnResultContinuousStart[i]=-1;
					m_pnResultContinuousEnd[i]=-1;
				}
				//▲1.0.0.1043

				//if( m_pdblResultDifferenceRatio )
				//	memcpy( m_pdblResultDifferenceRatio, pdblDifferenceRatio, sizeof(double) * m_iResultElement);
				//if( m_piResultLineRatio )
				//	memcpy( m_piResultLineRatio, piLineRatio, sizeof(INT) * m_iResultElement);
			}
			//判定
			for( int i=0; i<(int)nElement; i++ )
			{
				//▼1.0.0.1050
				//if( pdblDifferenceRatio[i]>m_dblDifferenceMax )
				//{
				//	m_iJudge = 0;
				//}
				//▼1.0.0.1058
				//for( int i=0; i<nElement; i++ )
				//▲1.0.0.1058
				{
					//▼1.0.0.1058
					//if( pnContinuousCount[i]<0 )	//Mode = 0 or 1
					//{
					//	if( pdblDifferenceRatio[i]>m_dblDifferenceMax )
					//	{
					//		m_iJudge = 0;
					//	}
					//}
					//else
					//{
					//	if( pnContinuousCount[i]>m_nContinuousCountMax )
					//	{
					//		m_iJudge = 0;
					//	}
					//}

					if( m_nCheckMode==0 )
					{
						if( pdblDifference[i]>m_dblDifferenceMax )
						{
							m_iJudge = 0;
						}
					}
					else
					if( m_nCheckMode==1 )
					{
						if( pdblDifferenceRatio[i]>m_dblDifferenceMax )
						{
							m_iJudge = 0;
						}
					}
					else
					{
						if( pnContinuousCount[i]>m_nContinuousCountMax )
						{
							m_iJudge = 0;
						}

					}
					//▲1.0.0.1058

				}
				//▲1.0.0.1050

				//▼1.0.0.1043
				//最悪値を代入
				//if( pdblDifferenceRatio[i]>m_dblWorstDifference )
				//{
				//	m_dblWorstDifference = pdblDifferenceRatio[i];
				//	if( m_pdblResultDifferenceRatio )
				//		memcpy( m_pdblResultDifferenceRatio, pdblDifferenceRatio, sizeof(double) * m_iResultElement);
				//	if( m_piResultLineRatio )
				//		memcpy( m_piResultLineRatio, piLineRatio, sizeof(INT) * m_iResultElement);
				//	//▼1.0.0.1024
				//	if( m_pdblResultAverage )
				//		memcpy( m_pdblResultAverage, pdblAverage, sizeof(double) * m_iResultElement);
				//	//NGじゃなくても最悪値を保存
				//	//if( pdblDifferenceRatio[i]>m_dblDifferenceMax )
				//	//▲1.0.0.1024
				//		bWorstJudge = TRUE;
				//}
				//▼1.0.0.1058
				//if( m_nCheckMode<2 )
				//{
				//	if( pdblDifferenceRatio[i]>m_dblWorstDifference )
				//	{
				//		m_dblWorstDifference = pdblDifferenceRatio[i];
				//		if( m_pdblResultDifferenceRatio )
				//			memcpy( m_pdblResultDifferenceRatio, pdblDifferenceRatio, sizeof(double) * m_iResultElement);
				//		if( m_piResultLineRatio )
				//			memcpy( m_piResultLineRatio, piLineRatio, sizeof(INT) * m_iResultElement);
				//		if( m_pdblResultAverage )
				//			memcpy( m_pdblResultAverage, pdblAverage, sizeof(double) * m_iResultElement);
				//		bWorstJudge = TRUE;
				//	}
				//}
				if( m_nCheckMode==0 )
				{
					if( pdblDifferenceRatio[i]>m_dblWorstDifference )
					{
						m_dblWorstDifference = pdblDifference[i];
						if( m_pdblResultDifferenceRatio )
							memcpy( m_pdblResultDifferenceRatio, pdblDifference, sizeof(double) * m_iResultElement);
						if( m_piResultLineRatio )
							memcpy( m_piResultLineRatio, piLine, sizeof(INT) * m_iResultElement);
						if( m_pdblResultAverage )
							memcpy( m_pdblResultAverage, pdblAverage, sizeof(double) * m_iResultElement);
						bWorstJudge = TRUE;
					}
				}
				if( m_nCheckMode==1 )
				{
					if( pdblDifferenceRatio[i]>m_dblWorstDifference )
					{
						m_dblWorstDifference = pdblDifferenceRatio[i];
						if( m_pdblResultDifferenceRatio )
							memcpy( m_pdblResultDifferenceRatio, pdblDifferenceRatio, sizeof(double) * m_iResultElement);
						if( m_piResultLineRatio )
							memcpy( m_piResultLineRatio, piLineRatio, sizeof(INT) * m_iResultElement);
						if( m_pdblResultAverage )
							memcpy( m_pdblResultAverage, pdblAverage, sizeof(double) * m_iResultElement);
						bWorstJudge = TRUE;
					}
				}
				//▲1.0.0.1058
				else
				if( m_nCheckMode==2 )
				{
					if( pnContinuousCount[i]>m_nWorstContinuousCount )
					{
						m_nWorstContinuousCount = pnContinuousCount[i];
						if( m_pdblResultDifferenceRatio )
							memcpy( m_pdblResultDifferenceRatio, pdblDifferenceRatio, sizeof(double) * m_iResultElement);
						if( m_piResultLineRatio )
							memcpy( m_piResultLineRatio, piLineRatio, sizeof(INT) * m_iResultElement);
						if( m_pdblResultAverage )
							memcpy( m_pdblResultAverage, pdblAverage, sizeof(double) * m_iResultElement);

						if( m_pnResultContinuousCount )
							memcpy( m_pnResultContinuousCount, pnContinuousCount, sizeof(double) * m_iResultElement);
						if( m_pnResultContinuousStart )
							memcpy( m_pnResultContinuousStart, pnContinuousStart, sizeof(double) * m_iResultElement);
						if( m_pnResultContinuousEnd )
							memcpy( m_pnResultContinuousEnd, pnContinuousEnd, sizeof(double) * m_iResultElement);

						bWorstJudge = TRUE;
					}
				}
				//▲1.0.0.1043
			}

			//Average判定-----------------------------
			double dblAverage = pdblAverage[0];
			if( m_iResultElement==3 )
			{
				dblAverage = pdblAverage[GetElementColorIndex(0)];
			}
			if( dblAverage<m_dblAverageLowLevel || dblAverage>m_dblAverageHighLevel  )
				m_iJudge = 0;

			//-----------------------------
			if( m_iJudge==2 )
				m_iJudge=1;

			//BMP保存用にバッファ保存
			if( bWorstJudge )
			{
				//m_WorstBuffer
				if( !m_WorstBuffer ) m_WorstBuffer = new StBuffer;
				if( m_WorstBuffer )
				{
					m_WorstBuffer->CreateCopy(*m_Buffer);
				}
			}
		}
		else
		{
			m_iJudge = 0;	//0:NG
		}

	}while(0);

	if(pdblDifference) delete [] pdblDifference;
	if(piLine) delete [] piLine;
	if(pdblDifferenceRatio) delete [] pdblDifferenceRatio;
	if(piLineRatio) delete [] piLineRatio;

	//▼1.0.0.1043
	if(pnContinuousCount) delete [] pnContinuousCount;
	if(pnContinuousStart) delete [] pnContinuousStart;
	if(pnContinuousEnd) delete [] pnContinuousEnd;
	//▲1.0.0.1043

	if( m_iJudge == 2 )
		m_iJudge = 0;

	//▼1.0.0.1026
	//▼1.0.0.1028
	//if( nDisplayMode==1 )
	if( nDisplayMode==1 && m_bThreadStop==FALSE )
	//▲1.0.0.1028
	{
	//▲1.0.0.1026


	//結果表示
	JudgeText(m_iJudge==1);
	//結果値表示
	ResultText();

	//▼1.0.0.1026
	}
	//▲1.0.0.1026

	if( pdblAverage )
	{
		//▼1.0.0.1026
		if( nDisplayMode==1 )
		{
		//▲1.0.0.1026
		//平均値値表示
		double dblAverage = pdblAverage[0];
		if( m_iResultElement==3 )
		{
//TCHAR szText[64];
//_stprintf_s(szText,_countof(szText),_T("dblAverage[%d]=%f \n"),GetElementColorIndex(0),pdblAverage[GetElementColorIndex(0)]);
//TRACE(szText);
			dblAverage = pdblAverage[GetElementColorIndex(0)];
		}
		//▼1.0.0.1028
		//AverageText( dblAverage, m_iResultElement );	//赤
		if( m_bThreadStop==FALSE )
		{
			AverageText( dblAverage, m_iResultElement );	//赤
		}
		else
		{
			m_dblDisplayAverage = dblAverage;
		}
		//▲1.0.0.1028

		//▼1.0.0.1026
		}
		//▲1.0.0.1026

		delete [] pdblAverage;
	}
	return bReval;

}

INT StCheckDustDlg::GetResultElement(void)
{
	return m_iResultElement;
}

double StCheckDustDlg::GetAverage(void)
{
	return m_dblResultAverage;
}

double StCheckDustDlg::GetResultDifference(int nIndex)
{
	if( nIndex>=m_iResultElement ) return -1;
	return m_pdblResultDifferenceRatio[nIndex];
}

INT StCheckDustDlg::GetResultLine(int nIndex)
{
	if( nIndex>=m_iResultElement ) return -1;
	return m_piResultLineRatio[nIndex];
}

//▼1.0.0.1024
double StCheckDustDlg::GetResultAverage(int nIndex)
{
	if( nIndex>=m_iResultElement ) return -1;
	return m_pdblResultAverage[nIndex];
}
//▲1.0.0.1024

//▼1.0.0.1043
INT StCheckDustDlg::GetResultContinuousCount(int nIndex)
{
	if( nIndex>=m_iResultElement ) return -1;
	INT iValue=-1;
	if( m_pnResultContinuousCount )
	{
		iValue = m_pnResultContinuousCount[nIndex];
	}
	return iValue;
}
INT StCheckDustDlg::GetResultContinuousStart(int nIndex)
{
	if( nIndex>=m_iResultElement ) return -1;
	INT iValue=-1;
	if( m_pnResultContinuousStart )
	{
		iValue = m_pnResultContinuousStart[nIndex];
	}
	return iValue;
}

INT StCheckDustDlg::GetResultContinuousEnd(int nIndex)
{
	if( nIndex>=m_iResultElement ) return -1;
	INT iValue=-1;
	if( m_pnResultContinuousEnd )
	{
		iValue = m_pnResultContinuousEnd[nIndex];
	}
	return iValue;
}

//▲1.0.0.1043

//▼1.0.0.1025
BOOL StCheckDustDlg::GetFunction(LPCTSTR szFunc, LPCTSTR szArg, double &dblValue)
{
	BOOL bReval = FALSE;
	CString szFunction(szFunc);
	if( szFunction=="GetElement" )
	{
		dblValue = GetResultElement();
		bReval = TRUE;
	}
	else
	if( szFunction=="GetAverage" )
	{
		INT nIndex = _ttoi(szArg);
		dblValue = GetResultAverage(nIndex);
		bReval = TRUE;
	}
	else
	if( szFunction=="GetLine" )
	{
		INT nIndex = _ttoi(szArg);
		dblValue = GetResultLine(nIndex);
		bReval = TRUE;
	}

	//▼1.0.0.1043
	else
	if( szFunction=="GetContinuousCountMax" )
	{
		dblValue = m_nContinuousCountMax;
		bReval = TRUE;
	}
	//▲1.0.0.1043

	return bReval;
}
//▲1.0.0.1025

//▼1.0.0.1026
BOOL StCheckDustDlg::Execute(void)
{

	m_iJudge = 2;

	BOOL bReval = CheckExecute(0);

	SaveBitmapMain();

	return bReval;
}


BOOL StCheckDustDlg::SaveBitmapMain(void)
{
	BOOL bReval = FALSE;

	if( m_WorstBuffer )
	{
		//▼1.0.0.1039
		CString szDeviceModelName;
		CString szSerialNumber;
		BOOL bReval;
		StString aText;
		bReval = GetDeviceModelName(aText);
		if( bReval )
			szDeviceModelName = aText.GetUnicode();
		bReval = GetSerialNumber(aText);
		if( bReval )
			szSerialNumber = aText.GetUnicode();

		CString szMainFileName;
		CTime gt=CTime::GetCurrentTime();
		//▼1.0.0.1059
		//CString dateFormat = gt.Format("%Y%m%d%H%M%S");
		CString timeFormat = gt.Format("%Y%m%d%H%M%S");
		CString dateFormat = gt.Format("%Y%m%d");
		//▲1.0.0.1059

		szMainFileName = szDeviceModelName;
		if( szSerialNumber.GetLength()>0 )
		{
			szMainFileName += _T("_") + szSerialNumber;
		}
		szMainFileName += _T("_") + timeFormat + _T("_");
		CString szFileName;
		//▲1.0.0.1039

		if( m_szBMPFileName.GetLength()>0 )
		{
			//▼1.0.0.1039
			//CString szDeviceModelName;
			//CString szSerialNumber;
			//BOOL bReval;
			//StString aText;
			//bReval = GetDeviceModelName(aText);
			//if( bReval )
			//	szDeviceModelName = aText.GetUnicode();
			//bReval = GetSerialNumber(aText);
			//if( bReval )
			//	szSerialNumber = aText.GetUnicode();

			//CString szFileName;
			//CTime gt=CTime::GetCurrentTime();
			//CString dateFormat = gt.Format("%Y%m%d%H%M%S");
			//szFileName = szDeviceModelName;
			//if( szSerialNumber.GetLength()>0 )
			//{
			//	szFileName += _T("_") + szSerialNumber;
			//}
			//szFileName += _T("_") + dateFormat + _T("_") + m_szBMPFileName;
			szFileName = szMainFileName + m_szBMPFileName;
			//▲1.0.0.1039
			if(  szFileName.Find(_T("."))<0 )
			{
				szFileName += _T(".bmp");
			}
			//▼1.0.0.1059
			//dateFormat = gt.Format("%Y%m%d");
			//▲1.0.0.1059
			CString szFullPath = mSaveFilePath + _T("\\") + szDeviceModelName + _T("\\") + dateFormat + _T("\\bmp\\");

			CStCommon::CheckDirectoryExist(szFullPath,TRUE);

			CString szFullFileName = szFullPath + szFileName;

			bReval = SaveBitmapFile(m_WorstBuffer, szFullFileName);
		}

		//▼1.0.0.1039
		if( m_szCSVFileName.GetLength()>0 )
		{
			szFileName = szMainFileName + m_szCSVFileName;
			if(  szFileName.Find(_T("."))<0 )
			{
				szFileName += _T(".csv");
			}

			CString szFullPath = mSaveFilePath + _T("\\") + szDeviceModelName + _T("\\") + dateFormat + _T("\\csv\\");

			CStCommon::CheckDirectoryExist(szFullPath,TRUE);

			//CString szFullFileName = szFullPath + szFileName;

			bReval = SaveCSVFile(m_WorstBuffer, szFullPath, szFileName, m_nStart, m_nEnd );

		}

		if( m_szPNGFileName.GetLength()>0 )
		{
			szFileName = szMainFileName + m_szPNGFileName;
			if(  szFileName.Find(_T("."))<0 )
			{
				szFileName += _T(".png");
			}

			CString szFullPath = mSaveFilePath + _T("\\") + szDeviceModelName + _T("\\") + dateFormat + _T("\\Image\\");

			CStCommon::CheckDirectoryExist(szFullPath,TRUE);

			CString szFullFileName = szFullPath + szFileName;

			bReval = SaveImageFile(m_WorstBuffer, szFullFileName );
		}

		//▲1.0.0.1039

	}
	return bReval;
}

//▲1.0.0.1026
