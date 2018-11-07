// StCheckDataLoseDlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "StGenFactoryScript.h"
#include "StCheckDataLoseDlg.h"
#include "afxdialogex.h"

#include "ResourceString.h"
#include "StCommon.h"
#include "StCheckCommon.h"

#define _ST_EXIT 1

unsigned __stdcall CheckDataLoseThread( void *pParam )
{
	StCheckDataLoseDlg * aDlg = (StCheckDataLoseDlg *)pParam;
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

// StCheckDataLoseDlg ダイアログ

IMPLEMENT_DYNAMIC(StCheckDataLoseDlg, CDialogEx)

StCheckDataLoseDlg::StCheckDataLoseDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(StCheckDataLoseDlg::IDD, pParent)
{
	Initialize();
	m_hMessageMutex = CreateMutex(NULL,FALSE,NULL); 
}
StCheckDataLoseDlg::StCheckDataLoseDlg( LPCTSTR szClassName, LPCTSTR szVarName, StDeviceBase *aDeviceBase, CDialogEx *aDialogEx, CWnd* pParent /*=NULL*/)
	: CDialogEx(StCheckDataLoseDlg::IDD, pParent)
	//▼1.0.0.1063
	//, StCheckBase( szClassName, szVarName, aDeviceBase, aDialogEx )
	, StCheckDataLose( szClassName, szVarName, aDeviceBase, aDialogEx )
	//▲1.0.0.1063
{
	Initialize();
	m_hMessageMutex = CreateMutex(NULL,FALSE,NULL); 
}

StCheckDataLoseDlg::~StCheckDataLoseDlg()
{

	//OnDestroyで開放を行うと正常取得できなくなる。
	//▼1.0.0.1063
	//if( m_pdblResultDifference ) delete [] m_pdblResultDifference;
	//if( m_piResultHighLow ) delete [] m_piResultHighLow;
	//if( m_piResultLine ) delete [] m_piResultLine;
	//▲1.0.0.1063

	if( m_pdblResultNGDifference ) delete [] m_pdblResultNGDifference;
	if( m_piResultNGHighLow ) delete [] m_piResultNGHighLow;
	if( m_piResultNGLine ) delete [] m_piResultNGLine;
	//▼1.0.0.1063
	m_pdblResultNGDifference = NULL;
	m_piResultNGHighLow = NULL;
	m_piResultNGLine = NULL;
	//▲1.0.0.1063

	CloseHandle(m_hMessageMutex);
}

void StCheckDataLoseDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(StCheckDataLoseDlg, CDialogEx)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDOK, &StCheckDataLoseDlg::OnBnClickedOk)
	ON_WM_TIMER()
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// StCheckDataLoseDlg メッセージ ハンドラー


BOOL StCheckDataLoseDlg::OnInitDialog()
{
	__super::OnInitDialog();

	// TODO:  ここに初期化を追加してください
	m_Color = RGB( 0,0,0 );
	CStatic objStatic;
	objStatic.Attach(GetDlgItem(IDC_STATIC_JUDGE_DL)->m_hWnd);

	m_pDefFont = objStatic.GetFont();
	//フォントの設定
	m_boldFont.CreateFont( 30, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
					  SHIFTJIS_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS,
					  DEFAULT_QUALITY, DEFAULT_PITCH, _T("ＭＳ Ｐゴシック") );

	objStatic.Detach();

	SetLimitText();


	//検査Thread作成-------------
	m_bResultNG = FALSE;
	m_hThread = (HANDLE)_beginthreadex( NULL, 0, CheckDataLoseThread, this,  0, &m_mThreadID );

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}


HBRUSH StCheckDataLoseDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = __super::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  ここで DC の属性を変更してください。
	if( pWnd->GetDlgCtrlID() == IDC_STATIC_JUDGE_DL )
	{
		pDC->SetTextColor(m_Color);
	}

	// TODO:  既定値を使用したくない場合は別のブラシを返します。
	return hbr;
}

void StCheckDataLoseDlg::Initialize(void)
{
	//▼1.0.0.1063
	//m_nStart = 0;
	//m_nEnd = 2047;
	//m_dblDifferenceMax = 0.0;
	//mSaveFilePath = ".";
	//▲1.0.0.1063
	m_nMode = 0;		//0:従来モード 1:NGがでたらストップモード
	m_szImageFileName = "";

	m_bResultNG = FALSE;

	m_bThreadStop = FALSE;

	//m_hThread = NULL;
	//▼1.0.0.1063
	//m_iResultElement = 0;
	//m_pdblResultDifference = NULL;
	//m_piResultHighLow = NULL;
	//m_piResultLine = NULL;
	//▲1.0.0.1063
	m_pdblResultNGDifference = NULL;
	m_piResultNGHighLow = NULL;
	m_piResultNGLine = NULL;
	
	StCheckDataLose::Initialize();

}

BOOL StCheckDataLoseDlg::JudgeText(BOOL bJudge)
{
	m_Color = bJudge?RGB(0,255,0):RGB(255,0,0);

	CString szText = bJudge?_T("OK"):_T("NG");
	CStatic objStatic;
	objStatic.Attach(GetDlgItem(IDC_STATIC_JUDGE_DL)->m_hWnd);
	objStatic.SetWindowText(szText);
	objStatic.SetFont( &m_boldFont, TRUE );
	objStatic.Detach();
	return TRUE;
}

BOOL StCheckDataLoseDlg::ResultText(void)
{
	INT nElement = GetResultElement();
	CString szText(_T("")), szTmp;
	LPCTSTR aColor[] = { _T("R:"), _T("G:"), _T("B:") };

	for( int i=0; i<nElement; i++ )
	{
		//szTmp.Format( _T("LvAve[%.2f] SNAve[%.4f] SNMax[%.4f] atpix[%d]\n"), m_dblResultAverage[i], m_dblResultSN[i], m_dblResultSNMax[i], m_nResultSDMPix[i]);
		double dblResultDifference = m_pdblResultDifference[i];
		INT iResultHighLow = m_piResultHighLow[i];
		INT iResultLine = m_piResultLine[i];
		if( m_bResultNG )
		{
			dblResultDifference = m_pdblResultNGDifference[i];
			iResultHighLow = m_piResultNGHighLow[i];
			iResultLine = m_piResultNGLine[i];
		}
		szTmp.Format( _T("Difference[%.2f] [%s] atpix[%d]\n\n"), dblResultDifference, iResultHighLow==0?_T("Low"):_T("High"), iResultLine );
		if( nElement==3 )
		{
			szTmp = aColor[GetElementColorIndex(i)] + szTmp;
		}
		szText += szTmp;
	}

	if( !GetThreadStop() )
	{
		CStatic objStatic;
		objStatic.Attach(GetDlgItem(IDC_STATIC_RESULT_DL)->m_hWnd);
		objStatic.SetWindowText(szText);
		objStatic.Detach();
	}
	return TRUE;
}

BOOL StCheckDataLoseDlg::SetLimitText(void)
{
	BOOL bReval = FALSE;
	CString szText;
	//▼1.0.0.1063
	//szText.Format( _T("Difference<[%.2f]\n"), m_dblDifferenceMax );
	szText.Format( _T("Difference<[%.2f]\n"), GetDifferenceMax() );
	//▲1.0.0.1063
	CStatic objStatic;
	objStatic.Attach(GetDlgItem(IDC_STATIC_LIMIT_DL)->m_hWnd);
	objStatic.SetWindowText(szText);
	objStatic.Detach();
	return bReval;
}
//▼1.0.0.1063
//void StCheckDataLoseDlg::SetStart(INT nData)
//{
//	m_nStart = nData;
//}
//
//void StCheckDataLoseDlg::SetEnd(INT nData)
//{
//	m_nEnd = nData;
//}
//
//void StCheckDataLoseDlg::SetMode(INT nData)
//{
//	m_nMode = nData;
//}
//
//void StCheckDataLoseDlg::SetDifferenceMax(double dblData)
//{
//	m_dblDifferenceMax = dblData;
//}
//▲1.0.0.1063

BOOL StCheckDataLoseDlg::function(LPCTSTR szFunc, LPCTSTR szArgument)
{
	BOOL bReval = FALSE;

	//▼1.0.0.1063
	bReval = StCheckDataLose::function(szFunc,szArgument);
	if( bReval ) return bReval;
	//▲1.0.0.1063

	CString szFunction(szFunc);
	//▼1.0.0.1063
	//if( szFunction == _T("Execute") )
	//{
	//}
	//else
	//if( szFunction=="SetStartLine" )
	//{
	//	INT nData = _tcstol(szArgument,NULL,0);
	//	SetStart(nData);
	//	bReval = TRUE;
	//}
	//else
	//if( szFunction=="SetEndLine" )
	//{
	//	INT nData = _tcstol(szArgument,NULL,0);
	//	SetEnd(nData);
	//	bReval = TRUE;
	//}
	//else
	//if( szFunction=="SetDifferenceMax" )
	//{
	//	double dblData = _ttof(szArgument);
	//	SetDifferenceMax(dblData);
	//	bReval = TRUE;
	//}
	//else
	//▲1.0.0.1063
	if( szFunction=="Mode" || szFunction=="SetMode" )
	{
		m_nMode = _tcstol(szArgument,NULL,0);
		bReval = TRUE;
	}
	else
	if( szFunction=="ImageFileName" || szFunction=="SetImageFileName" )
	{
		m_szImageFileName = szArgument;
		bReval = TRUE;
	}
	return bReval;
}

double StCheckDataLoseDlg::GetResultDifference(int nIndex)
{
	if( nIndex>=m_iResultElement ) return -1.0;
	double dblData = (m_bResultNG==FALSE)?m_pdblResultDifference[nIndex]:m_pdblResultNGDifference[nIndex];
	return dblData;
}

INT StCheckDataLoseDlg::GetResultHighLow(int nIndex)
{
	if( nIndex>=m_iResultElement ) return -1;
	INT nData = (m_bResultNG==FALSE)?m_piResultHighLow[nIndex]:m_piResultNGHighLow[nIndex];
	return nData;
}

INT StCheckDataLoseDlg::GetResultLine(int nIndex)
{
	if( nIndex>=m_iResultElement ) return -1;
	INT nData = (m_bResultNG==FALSE)?m_piResultLine[nIndex]:m_piResultNGLine[nIndex];
	return nData;
}


//---------------------------------------------------
BOOL StCheckDataLoseDlg::GetDataLose(void)
{

	double *pdblDifference = NULL;
	INT *piHighLow = NULL;
	INT *piLine = NULL;

	double **dblLineAverage = NULL;
	INT **piLineMin = NULL;
	INT **piLineMax = NULL;
	//▼1.0.0.1063
	//INT nLines = m_nEnd - m_nStart + 1;
	INT nEnd = GetEnd();
	INT nStart = GetStart();
	INT nLines = nEnd - nStart + 1;
	//▲1.0.0.1063
	size_t nElement = GetElementFromBuffer(m_Buffer);

	BOOL bReval = FALSE;
	do
	{
		pdblDifference = new double [nElement];
		if( !pdblDifference ) break;
		piHighLow = new INT [nElement];
		if( !piHighLow ) break;
		piLine = new INT [nElement];
		if( !piLine ) break;
		//----------------------
		dblLineAverage = new double * [nElement];
		if( !dblLineAverage ) break;
		memset( dblLineAverage, NULL, sizeof(double *) * nElement );
		double *pDblBuffer = NULL;
		for( int i=0; i<(int)nElement; i++ )
		{
			dblLineAverage[i] = pDblBuffer = new double [nLines];
			if( !pDblBuffer ) break;
		}
		if( !pDblBuffer ) break;
		//----------------------
		piLineMin = new INT * [nElement];
		if( !piLineMin ) break;
		memset( piLineMin, NULL, sizeof(INT *) * nElement );
		INT *piBuffer = NULL;
		for( int i=0; i<(int)nElement; i++ )
		{
			piLineMin[i] = piBuffer = new INT [nLines];
			if( !piBuffer ) break;
		}
		if( !piBuffer ) break;
		//----------------------
		piLineMax = new INT * [nElement];
		if( !piLineMax ) break;
		memset( piLineMax, NULL, sizeof(INT *) * nElement );
		for( int i=0; i<(int)nElement; i++ )
		{
			piLineMax[i] = piBuffer = new INT [nLines];
			if( !piBuffer ) break;
		}
		if( !piBuffer ) break;

		//▼1.0.0.1063
		//bReval = GetLineAverageDifference( m_Buffer, m_nStart, m_nEnd, pdblDifference, piHighLow, piLine, &nElement
		bReval = GetLineAverageDifference( m_Buffer, nStart, nEnd, pdblDifference, piHighLow, piLine, &nElement
		//▲1.0.0.1063
										   , dblLineAverage, piLineMin, piLineMax, nLines );

		if( bReval )
		{
			if( m_iResultElement != nElement )
			{
				m_iResultElement = nElement;
				if( m_pdblResultDifference ) delete [] m_pdblResultDifference;
				m_pdblResultDifference = new double [m_iResultElement];
				memset(m_pdblResultDifference,0,sizeof(double)*m_iResultElement);
				if( m_piResultHighLow ) delete [] m_piResultHighLow;
				m_piResultHighLow = new INT [m_iResultElement];
				memset(m_piResultHighLow,0,sizeof(INT)*m_iResultElement);
				if( m_piResultLine ) delete [] m_piResultLine;
				m_piResultLine = new INT [m_iResultElement];
				memset(m_piResultLine,0,sizeof(INT)*m_iResultElement);

				//if( m_nMode==1 )	//途中でModeを変えられた場合にそなえBufferの確保をしておく。
				{
					if( m_pdblResultNGDifference ) delete [] m_pdblResultNGDifference;
					m_pdblResultNGDifference = new double [m_iResultElement];
					memset(m_pdblResultNGDifference,0,sizeof(double)*m_iResultElement);
					if( m_piResultNGHighLow ) delete [] m_piResultNGHighLow;
					m_piResultNGHighLow = new INT [m_iResultElement];
					memset(m_piResultNGHighLow,0,sizeof(INT)*m_iResultElement);
					if( m_piResultNGLine ) delete [] m_piResultNGLine;
					m_piResultNGLine = new INT [m_iResultElement];
					memset(m_piResultNGLine,0,sizeof(INT)*m_iResultElement);
				}
			}

			SetElementColorIndexFromBuffer(m_Buffer);

			if( m_pdblResultDifference )
				memcpy( m_pdblResultDifference, pdblDifference, sizeof(double) * m_iResultElement);
			if( m_piResultHighLow )
				memcpy( m_piResultHighLow, piHighLow, sizeof(INT) * m_iResultElement);
			if( m_piResultLine )
				memcpy( m_piResultLine, piLine, sizeof(INT) * m_iResultElement);

			//判定
			for( int i=0; i<(int)nElement; i++ )
			{
				//▼1.0.0.1063
				//if( pdblDifference[i]>m_dblDifferenceMax )
				if( pdblDifference[i]>GetDifferenceMax() )
				//▲1.0.0.1063
					m_iJudge=0;
			}
			if( m_iJudge==2 )
				m_iJudge=1;

			//CSV保存
			//if( m_szCSVFileName.GetLength()>0 )
			//{
			//	SaveResultCSVFile( nElement, dblLineAverage, piLineMin, piLineMax, nLines );
			//	//SaveResultCSVFile( nElement, (double (*)[])dblLineAverage, (INT (*)[])piLineMin, (INT (*)[])piLineMax, nLines );
			//}

			if( m_iJudge==0 && m_nMode==1 && !m_bResultNG )
			{
				//if( m_pdblResultNGDifference ) delete [] m_pdblResultNGDifference;
				//m_pdblResultNGDifference = new double [m_iResultElement];
				//if( m_piResultNGHighLow ) delete [] m_piResultNGHighLow;
				//m_piResultNGHighLow = new INT [m_iResultElement];
				//if( m_piResultLine ) delete [] m_piResultLine;
				//m_piResultLine = new INT [m_iResultElement];

				for( int i=0; i<nElement && i<m_iResultElement; i++ )
				{
					m_pdblResultNGDifference[i] = m_pdblResultDifference[i];
					m_piResultNGHighLow[i] = m_piResultHighLow[i];
					m_piResultNGLine[i] = m_piResultLine[i];
				}
				m_bResultNG = TRUE;

				//この時にBufferに画像保存----------
				if( m_szImageFileName.GetLength()>0 )
				{
					mNGImageBuffer.CreateCopy(*m_Buffer);
				}
			}
			if( m_bResultNG ) m_iJudge = 0;

			//結果数値表示
			ResultText();

		}
		else
		{
			m_iJudge = 0;	//0:NG
		}

	}while(0);

	if(pdblDifference) delete [] pdblDifference;
	if(piHighLow) delete [] piHighLow;
	if(piLine) delete [] piLine;

	//----------------------
	if( dblLineAverage )
	{
		for( int i=0; i<(int)nElement; i++ )
		{
			double * pdblAverage =  dblLineAverage[i];
			if( pdblAverage ) delete [] pdblAverage;
		}
		delete [] dblLineAverage;
	}
	//----------------------
	if( piLineMin )
	{
		for( int i=0; i<(int)nElement; i++ )
		{
			INT * pLine =  piLineMin[i];
			if( pLine ) delete [] pLine;
		}
		delete [] piLineMin;
	}
	//----------------------
	if( piLineMax )
	{
		for( int i=0; i<(int)nElement; i++ )
		{
			INT * pLine =  piLineMax[i];
			if( pLine ) delete [] pLine;
		}
		delete [] piLineMax;
	}
	if( m_iJudge == 2 )
		m_iJudge = 0;


//TRACE(TEXT(" m_piResultNGHighLow(%d:%d)\n"),m_piResultHighLow[0],m_piResultNGHighLow[0]);

	return bReval;
}

BOOL StCheckDataLoseDlg::CheckExecute(void)
{

	BOOL bReval = FALSE;
	m_iJudge = 2;	//2:処理中
	if( m_Buffer ) delete m_Buffer;
	m_Buffer = new StBuffer;
	bReval = m_DeviceBase->GetStBuffer(m_Buffer);	//変換して取得もありかも。現時点ではコピー

	if( bReval )
	{

		bReval = GetDataLose();

//TRACE(TEXT(" 11  m_piResultNGHighLow(%d:%d)\n"),m_piResultHighLow[0],m_piResultNGHighLow[0]);

		//bReval = GetLineAverageDifference( m_Buffer, m_nStart, m_nEnd, pdblDifference, piHighLow, piLine, &nElement
		//								   , dblLineAverage, piLineMin, piLineMax, nLines );

	}

	/*
	bReval = GetSNAverage( m_Buffer, m_nStart, m_nEnd, m_dblResultAverage, m_dblResultSN, m_dblResultSNMax, m_nResultSDMPix, m_dblReslutLineAverageMin, m_dblReslutLineAverageMax, m_nResultLineAverageMinPos, m_nResultLineAverageMaxPos );
	if( bReval )
	{
		m_iResultElement = GetElementFromBuffer(m_Buffer);
		SetElementColorIndexFromBuffer(m_Buffer);
		for( int i=0; i<m_iResultElement && m_iJudge==2; i++ )
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
	*/

	//結果表示
	JudgeText(m_iJudge==1);

	return bReval;

}
//▼1.0.0.1063
//BOOL StCheckDataLoseDlg::GetFunction(LPCTSTR szFunc, LPCTSTR szArg, double &dblValue)
//{
//	BOOL bReval = FALSE;
//	CString szFunction(szFunc);
//	if( szFunction=="GetDifference" )
//	{
//		INT nIndex = _ttoi(szArg);
//		if( nIndex>=0 && nIndex<m_iResultElement )
//		{
//			dblValue = m_pdblResultDifference[nIndex];
//			bReval = TRUE;
//		}
//	}
//	else
//	if( szFunction=="GetHighLow" )
//	{
//		INT nIndex = _ttoi(szArg);
//		if( nIndex>=0 && nIndex<m_iResultElement )
//		{
//			dblValue = (double)m_piResultHighLow[nIndex];
//			bReval = TRUE;
//		}
//	}
//	else
//	if( szFunction=="GetLine" )
//	{
//		INT nIndex = _ttoi(szArg);
//		if( nIndex>=0 && nIndex<m_iResultElement )
//		{
//			dblValue = (double)m_piResultLine[nIndex];
//			bReval = TRUE;
//		}
//	}
//
//	return bReval;
//}
//▲1.0.0.1063


void StCheckDataLoseDlg::OnBnClickedOk()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	m_bThreadStop = TRUE;
	//タイムラグをいれないとフリーズする。。
	SetTimer(_ST_EXIT, 200, NULL);

}


void StCheckDataLoseDlg::OnTimer(UINT_PTR nIDEvent)
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
			CTime gt=CTime::GetCurrentTime();
			CString dateFormat = gt.Format("%Y%m%d%H%M%S");
			aCommon.SaveBitmap( &mNGImageBuffer, dateFormat );
			//▼1.0.0.1063
			//aCommon.SaveCSV( &mNGImageBuffer, dateFormat );
			SaveCheckExecute( &mNGImageBuffer, FALSE, dateFormat, m_szImageFileName );
			//▲1.0.0.1063
		}
		CDialogEx::OnOK();
		return;
	}

	__super::OnTimer(nIDEvent);
}


void StCheckDataLoseDlg::OnDestroy()
{
	__super::OnDestroy();

	// TODO: ここにメッセージ ハンドラー コードを追加します。

}
