// StCheckEllitoLVDSDlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "StGenFactoryScript.h"
#include "StCheckEllitoLVDSDlg.h"
#include "afxdialogex.h"
#include "StInspectMain.h"

#include "StCheckControlRetry.h"
#include "StCommon.h"

// StCheckEllitoLVDSDlg ダイアログ

IMPLEMENT_DYNAMIC(StCheckEllitoLVDSDlg, CDialogEx)

StCheckEllitoLVDSDlg::StCheckEllitoLVDSDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(StCheckEllitoLVDSDlg::IDD, pParent)
{
	Initialize();
}


StCheckEllitoLVDSDlg::StCheckEllitoLVDSDlg( LPCTSTR szClassName, LPCTSTR szVarName, StDeviceBase *aDeviceBase, CDialogEx *aDialogEx, CWnd* pParent /*=NULL*/)
	: CDialogEx(StCheckEllitoLVDSDlg::IDD, pParent)
	, StCheckBase( szClassName, szVarName, aDeviceBase, aDialogEx )
{
	Initialize();
}

StCheckEllitoLVDSDlg::~StCheckEllitoLVDSDlg()
{
	Release();
}

void StCheckEllitoLVDSDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(StCheckEllitoLVDSDlg, CDialogEx)
	ON_LBN_SELCHANGE(IDC_LIST_LMAP, &StCheckEllitoLVDSDlg::OnLbnSelchangeListLmap)
	//タブオーダー順をEditの後はSpinにしなければいけないため、RANGE使用できない。こともないか？とりあえず作成したのでこのまま
	//ON_CONTROL_RANGE( EN_CHANGE, IDC_EDIT_CLOCK1, IDC_EDIT_CLOCK5, &StCheckEllitoLVDSDlg::OnEnChangeEditClock)
	//ON_CONTROL_RANGE( EN_CHANGE, IDC_EDIT_SYNC1, IDC_EDIT_SYNC5, &StCheckEllitoLVDSDlg::OnEnChangeEditSync)
	ON_EN_CHANGE(IDC_EDIT_CLOCK1, &StCheckEllitoLVDSDlg::OnEnChangeEditClock1)
	ON_EN_CHANGE(IDC_EDIT_CLOCK2, &StCheckEllitoLVDSDlg::OnEnChangeEditClock2)
	ON_EN_CHANGE(IDC_EDIT_CLOCK3, &StCheckEllitoLVDSDlg::OnEnChangeEditClock3)
	ON_EN_CHANGE(IDC_EDIT_CLOCK4, &StCheckEllitoLVDSDlg::OnEnChangeEditClock4)
	ON_EN_CHANGE(IDC_EDIT_CLOCK5, &StCheckEllitoLVDSDlg::OnEnChangeEditClock5)
	ON_EN_CHANGE(IDC_EDIT_SYNC1, &StCheckEllitoLVDSDlg::OnEnChangeEditSync1)
	ON_EN_CHANGE(IDC_EDIT_SYNC2, &StCheckEllitoLVDSDlg::OnEnChangeEditSync2)
	ON_EN_CHANGE(IDC_EDIT_SYNC3, &StCheckEllitoLVDSDlg::OnEnChangeEditSync3)
	ON_EN_CHANGE(IDC_EDIT_SYNC4, &StCheckEllitoLVDSDlg::OnEnChangeEditSync4)
	ON_EN_CHANGE(IDC_EDIT_SYNC5, &StCheckEllitoLVDSDlg::OnEnChangeEditSync5)
	ON_WM_VSCROLL()
	ON_COMMAND_RANGE(IDC_CHECK_DELAY0,IDC_CHECK_DELAY7, &StCheckEllitoLVDSDlg::OnCheckDelay)
	ON_BN_CLICKED(IDOK, &StCheckEllitoLVDSDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &StCheckEllitoLVDSDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, &StCheckEllitoLVDSDlg::OnBnClickedButtonSave)
	ON_BN_CLICKED(IDC_BUTTON_DEFAULT, &StCheckEllitoLVDSDlg::OnBnClickedButtonDefault)
	ON_BN_CLICKED(IDC_BUTTON_WRITE, &StCheckEllitoLVDSDlg::OnBnClickedButtonWrite)
	ON_WM_DESTROY()
END_MESSAGE_MAP()


INT nLMapTable[8][15] =  //[Clock][LVDS]
{
	7, 6, 5, 4, 3, 2, 1, 0, -1, -1, -1, -1, -1, -1, -1,
	-1, 7, 6, 5, 4, 3, 2, 1, 0, -1, -1, -1, -1, -1, -1, 
	-1, -1, 7, 6, 5, 4, 3, 2, 1, 0, -1, -1, -1, -1, -1, 
	-1, -1, -1, 7, 6, 5, 4, 3, 2, 1, 0, -1, -1, -1, -1, 
	-1, -1, -1, -1, 7, 6, 5, 4, 3, 2, 1, 0, -1, -1, -1, 
	-1, -1, -1, -1, -1, 7, 6, 5, 4, 3, 2, 1, 0, -1, -1, 
	-1, -1, -1, -1, -1, -1, 7, 6, 5, 4, 3, 2, 1, 0, -1, 
	-1, -1, -1, -1, -1, -1, -1, 7, 6, 5, 4, 3, 2, 1, 0, 
};

// StCheckEllitoLVDSDlg メッセージ ハンドラー
void StCheckEllitoLVDSDlg::Initialize(void)
{
	m_pControlRetry = NULL;
	m_szSavePath = _T(".");

	memset(m_nEditClockID,0,sizeof(m_nEditClockID));
	memset(m_nSpinClockID,0,sizeof(m_nSpinClockID));
	memset(m_nEditSyncID,0,sizeof(m_nEditSyncID));
	memset(m_nSpinSyncID,0,sizeof(m_nSpinSyncID));
	memset( m_nButtonDelayID, 0, sizeof(m_nButtonDelayID) );
	memset( m_nSliceSETable, 0, sizeof(m_nSliceSETable) );

}

void StCheckEllitoLVDSDlg::Release(void)
{



	//if( m_pdblResultDifferenceRatio )
	//{
	//	delete [] m_pdblResultDifferenceRatio;
	//	m_pdblResultDifferenceRatio = NULL;
	//}
	//if( m_piResultLineRatio )
	//{
	//	delete [] m_piResultLineRatio;
	//	m_piResultLineRatio = NULL;
	//}
	//if( m_WorstBuffer )
	//{
	//	delete m_WorstBuffer;
	//	m_WorstBuffer = NULL;
	//}

}

void StCheckEllitoLVDSDlg::InitDisplay() 
{
	//受信データを表示
	/*
	LVDS  ->  0 1 2 3 4 5 6 7 8 9 10 11 12 13 14
	Clock 
	0     ->  7 6 5 4 3 2 1 0
	1     ->    7 6 5 4 3 2 1 0
	2     ->      7 6 5 4 3 2 1 0
	3     ->        7 6 5 4 3 2 1 0
	4     ->          7 6 5 4 3 2 1  0
	5     ->            7 6 5 4 3 2  1  0
	6     ->              7 6 5 4 3  2  1  0
	7     ->                7 6 5 4  3  2  1  0
	*/
	TCHAR szText[128];
	TCHAR szTmp[128];
	CListBox *pListLMap = (CListBox *)GetDlgItem(IDC_LIST_LMAP);
	//m_ListLMap.ResetContent();
	pListLMap->ResetContent();
	for( int i=0; i<sizeof(m_wLMAP)/sizeof(m_wLMAP[0]); i++ )
	{
		//INT nLine = (i+7)/8;
		INT nClock = m_clock[(i+7)/8];
		INT nLVDS = 0;
		for( int j=0; j<15; j++ )
		{
			if( nLMapTable[nClock][j]==m_delay[i] )
			{
				nLVDS = j;
				break;
			}
		}
		_stprintf_s( szText, _countof(szText), _T("Data%02d, 0x%04X, "),i,m_wLMAP[i] );
		for( int j=0; j<15; j++ )
		{
			INT nShift = (3-((j+1)/4))*4 + 3 - ((j+1)&3);
			_stprintf_s( szTmp, _countof(szTmp), _T("%s,"), ((m_wLMAP[i]>>nShift)&1)>0?(j==nLVDS?_T("@"):_T("0")):(j==nLVDS?_T("X"):_T("x")) );
			if( j==12 )
			{
				_tcscat(szText,_T("  "));
			}
			_tcscat(szText,szTmp);
		}
		//m_ListLMap.AddString(szText);
		pListLMap->AddString(szText);

	}
	//--------------------------------------------------------------
	//m_SpinSync1.SetPos(m_sync[0]);
	//m_SpinSync2.SetPos(m_sync[1]);
	//m_SpinSync3.SetPos(m_sync[2]);
	//m_SpinSync4.SetPos(m_sync[3]);
	//m_SpinSync5.SetPos(m_sync[4]);
	for( int i=0; i<_countof(m_nSpinSyncID); i++ )
	{
		CSpinButtonCtrl *pSpin = ((CSpinButtonCtrl*)GetDlgItem(m_nSpinSyncID[i]));
		pSpin->SetPos(m_sync[i]);
	}


	for( int i=0; i<_countof(m_nSpinClockID); i++ )
	{
		CSpinButtonCtrl *pSpin = ((CSpinButtonCtrl*)GetDlgItem(m_nSpinClockID[i]));
		pSpin->SetPos(m_clock[i]);
	}

	ChangeDelay(m_nDataPos);
}

UINT GetIntFromEllitoFormat(LPCTSTR szText)
{
	UINT nData = 0xFFFFFFFF;
	if( szText )
	{
		CString szData(szText);
	//	INT nPos = szData.Find(_T("USER>"));
	//	if( nPos>0 )
	//	{
	//		TCHAR*	pszEnd;
	//		nData = ::_tcstol(szData.Left(nPos),&pszEnd,16);
	//	}
		INT nCount = 0;
		CString szValue(_T(""));
		CString szSTX =_T("0x0d,0x0a,");

		do
		{

			if( szData.Left(szSTX.GetLength())!=szSTX )
				break;
			szData = szData.Mid(szSTX.GetLength());
			if( szData.Find(szSTX)<0 )
				break;
			szData = szData.Left(szData.Find(szSTX));
			do
			{
				CString szTmp;
				INT nCammaPos = szData.Find(_T(","));
				if( nCammaPos>=0 )
				{
					szTmp = szData.Left(nCammaPos);
					szData = szData.Mid(nCammaPos+1);
				}
				else
				{
					szTmp = szData;
					szData = _T("");
				}

				//文字列
				if( szTmp.Left(1)==_T("\"") )
				{
					szTmp = szTmp.Mid(1);
					if( szTmp.Find(_T("\""))>0 )
					{
						szTmp = szTmp.Left(szTmp.Find(_T("\"")));
					}
					szValue += szTmp;
				}
				else
				//数値
				{
					if( szTmp.GetLength() )
					{
						nData = ::_tcstol(szTmp,NULL,0);
						char c[2] = {nData,0};
						szValue += c;
					}
				}
			}while(szData.GetLength()>0);
			nData = ::_tcstol(szValue,NULL,16);
		}while(0);

	}

	return nData;
}


BOOL StCheckEllitoLVDSDlg::OnInitDialog()
{
	__super::OnInitDialog();

	// TODO:  ここに初期化を追加してください
	StCheckControlRetry *aControlRetry = NULL;
	//INT nDefRetry = -1;
	//CString szReturn(_T(""));

	m_nDefRetry = -1;
	m_szReturn = _T("");
	for( int i=0; i<_countof(m_dwDefTimeout); i++ )
	{
		m_dwDefTimeout[i] = 0xffffffff;
	}

	do
	{
		if( !m_DeviceBase ) break;
		//▼1.0.0.1046
		if( m_DeviceBase->m_ClassName=="StDeviceMultiCam" )	//CL用
		{
		//▲1.0.0.1046

		StDeviceMultiCam *aDevice = (StDeviceMultiCam *)m_DeviceBase;
		if( !aDevice ) break;
		if( aDevice->IsOpened() )
		{
			aControlRetry = (StCheckControlRetry *)aDevice->GetControlRetry();
		}
		if( !aControlRetry ) break;
		m_pControlRetry = aControlRetry;


		m_nDefRetry = aControlRetry->GetRetry();
		aControlRetry->SetRetry(0);

		aControlRetry->GetReturn(m_szReturn);
		aControlRetry->SetRecieveData(_T(""));
		//aControlRetry->SetSendData(m_szCommand);

		aControlRetry->GetCommTimeout(&m_dwDefTimeout[0],&m_dwDefTimeout[1],&m_dwDefTimeout[2],&m_dwDefTimeout[3],&m_dwDefTimeout[4]);
		aControlRetry->SetCommTimeout(50,50,0,0,0);


		TCHAR cmdText[64];
		CString szRcvData;
		BOOL bReval = TRUE;
		for( int i=0; i<5; i++ )
		{
			//_stprintf_s( cmdText, _countof(cmdText), _T("RED %d"), i * 3 );	
			_stprintf_s( cmdText, _countof(cmdText), _T("0x02,\"RED %d\",0x0d"), i * 3 );	
			aControlRetry->SetSendData(cmdText);
			bReval = aControlRetry->Execute();
			if( !bReval ) break;
			aControlRetry->GetLastRecieveData(szRcvData);
			UINT unValue = GetIntFromEllitoFormat(szRcvData);
			if( i==0 )
			{
				m_delay[0] = (unValue >> 9 ) & 0x07;
			}
			else
			{
				for( int j=0; j<4; j++ )
				{
					m_delay[ (i-1)*8 + 3-j + 1] = ( unValue >> (j*3) ) & 0x07;
				}
			}
			if( i>0 )
			{
				//_stprintf_s( cmdText, _countof(cmdText), _T("RED %d"), i * 3 + 1 );	
				_stprintf_s( cmdText, _countof(cmdText), _T("0x02,\"RED %d\",0x0d"), i * 3 + 1 );	
				aControlRetry->SetSendData(cmdText);
				bReval = aControlRetry->Execute();
				if( !bReval ) break;
				aControlRetry->GetLastRecieveData(szRcvData);	//戻り値(17文字)  CRLF 00000000 CRLF USER>
				unValue = GetIntFromEllitoFormat(szRcvData);
				for( int j=0; j<4; j++ )
				{
					m_delay[ (i-1)*8 + 3-j + 5 ] = ( unValue >> (j*3) ) & 0x07;
				}
			}
			//_stprintf_s( cmdText, _countof(cmdText), _T("RED %d"), i * 3 + 2 );	
			_stprintf_s( cmdText, _countof(cmdText), _T("0x02,\"RED %d\",0x0d"), i * 3 + 2 );	
			aControlRetry->SetSendData(cmdText);
			bReval = aControlRetry->Execute();
			if( !bReval ) break;
			aControlRetry->GetLastRecieveData(szRcvData);	//戻り値(17文字)  CRLF 00000000 CRLF USER>
			unValue = GetIntFromEllitoFormat(szRcvData);

			m_sync[i] = ( unValue >> 3 ) & 0x07;
			m_clock[i] = ( unValue >> 0 ) & 0x07;
		}
		if( !bReval ) break;


		//LMAP受信
		for( int i=0; i<sizeof(m_wLMAP)/sizeof(m_wLMAP[0]); i++ )
		{
			//_stprintf_s( cmdText, _countof(cmdText), _T("LMAP %d"), i );	
			_stprintf_s( cmdText, _countof(cmdText), _T("0x02,\"LMAP %d\",0x0d"), i );	
			aControlRetry->SetSendData(cmdText);
			bReval = aControlRetry->Execute();
			if( !bReval ) break;
			aControlRetry->GetLastRecieveData(szRcvData);	//戻り値(19文字)  CRLF 0x00000000 CRLF USER>
			m_wLMAP[i] = (WORD)GetIntFromEllitoFormat(szRcvData);
		}

		//▼1.0.0.1046
		}
		else
		if( m_DeviceBase->m_ClassName=="StDeviceGenICam" )	//GenICam用
		{

			StDeviceGenICam *aDevice = (StDeviceGenICam *)m_DeviceBase;
			if( !aDevice ) break;
			if( !aDevice->IsOpened() ) break;

			TCHAR cmdText[64];
			CString szRcvData;

			//0xAFE0に0x000Aをセット
			UINT uiData = 0x000A; 
			BOOL bReval = aDevice->WriteMain(0xAFE0,(char *)&uiData,sizeof(uiData));
			if( !bReval ) break;

			UINT unValue;
			for( int i=0; i<5; i++ )
			{
				//_stprintf_s( cmdText, _countof(cmdText), _T("0x02,\"RED %d\",0x0d"), i * 3 );	
				//aControlRetry->SetSendData(cmdText);
				//bReval = aControlRetry->Execute();

				DWORD dwAddress = 0xB000 + (i * 3) * 4;
				bReval = aDevice->ReadMain(dwAddress,(char *)&unValue,sizeof(unValue));
				if( !bReval ) break;

				//aControlRetry->GetLastRecieveData(szRcvData);
				//UINT unValue = GetIntFromEllitoFormat(szRcvData);
				if( i==0 )
				{
					m_delay[0] = (unValue >> 9 ) & 0x07;
				}
				else
				{
					for( int j=0; j<4; j++ )
					{
						m_delay[ (i-1)*8 + 3-j + 1] = ( unValue >> (j*3) ) & 0x07;
					}
				}
				if( i>0 )
				{
					//_stprintf_s( cmdText, _countof(cmdText), _T("0x02,\"RED %d\",0x0d"), i * 3 + 1 );	
					//aControlRetry->SetSendData(cmdText);
					//bReval = aControlRetry->Execute();
					//if( !bReval ) break;
					//aControlRetry->GetLastRecieveData(szRcvData);	//戻り値(17文字)  CRLF 00000000 CRLF USER>
					//unValue = GetIntFromEllitoFormat(szRcvData);
					dwAddress = 0xB000 + (i * 3 + 1 ) * 4;
					bReval = aDevice->ReadMain(dwAddress,(char *)&unValue,sizeof(unValue));
					if( !bReval ) break;
					for( int j=0; j<4; j++ )
					{
						m_delay[ (i-1)*8 + 3-j + 5 ] = ( unValue >> (j*3) ) & 0x07;
					}
				}
				//_stprintf_s( cmdText, _countof(cmdText), _T("0x02,\"RED %d\",0x0d"), i * 3 + 2 );	
				//aControlRetry->SetSendData(cmdText);
				//bReval = aControlRetry->Execute();
				//if( !bReval ) break;
				//aControlRetry->GetLastRecieveData(szRcvData);	//戻り値(17文字)  CRLF 00000000 CRLF USER>
				//unValue = GetIntFromEllitoFormat(szRcvData);

				dwAddress = 0xB000 + (i * 3 + 2) * 4;
				bReval = aDevice->ReadMain(dwAddress,(char *)&unValue,sizeof(unValue));
				if( !bReval ) break;

				m_sync[i] = ( unValue >> 3 ) & 0x07;
				m_clock[i] = ( unValue >> 0 ) & 0x07;
			}
			if( !bReval ) break;


			//LMAP受信
			uiData = 0x0009;
			bReval = aDevice->WriteMain(0xAFE0,(char *)&uiData,sizeof(uiData));
			if( !bReval ) break;

			for( int i=0; i<_countof(m_wLMAP); i++ )
			{
				//_stprintf_s( cmdText, _countof(cmdText), _T("0x02,\"LMAP %d\",0x0d"), i );	
				//aControlRetry->SetSendData(cmdText);
				//bReval = aControlRetry->Execute();
				//if( !bReval ) break;
				//aControlRetry->GetLastRecieveData(szRcvData);	//戻り値(19文字)  CRLF 0x00000000 CRLF USER>
				//m_wLMAP[i] = (WORD)GetIntFromEllitoFormat(szRcvData);

				DWORD dwAddress = 0xB000 + i * 4;
				bReval = aDevice->ReadMain(dwAddress,(char *)&unValue,sizeof(unValue));
				if( bReval )
					m_wLMAP[i] = unValue;
			}

		}
		//▲1.0.0.1046

	}while(0);

	//Data copy
	memcpy( m_defDelay, m_delay, sizeof(m_defDelay) );
	memcpy( m_defClock, m_clock, sizeof(m_defClock) );
	memcpy( m_defSync, m_sync, sizeof(m_defSync) );

	//Slice Start End Table
	//INT nSETable[5][2] = { 0,0,  1,8,  9,16,  17,24,  25,32  };
	m_nSliceSETable[0][0] = 0;
	m_nSliceSETable[0][1] = 0;
	m_nSliceSETable[1][0] = 1;
	m_nSliceSETable[1][1] = 8;
	m_nSliceSETable[2][0] = 9;
	m_nSliceSETable[2][1] = 16;
	m_nSliceSETable[3][0] = 17;
	m_nSliceSETable[3][1] = 24;
	m_nSliceSETable[4][0] = 25;
	m_nSliceSETable[4][1] = 32;

	//Clock - Sync - 
	m_nSpinSyncID[0] = IDC_SPIN_SYNC1;
	m_nSpinSyncID[1] = IDC_SPIN_SYNC2;
	m_nSpinSyncID[2] = IDC_SPIN_SYNC3;
	m_nSpinSyncID[3] = IDC_SPIN_SYNC4;
	m_nSpinSyncID[4] = IDC_SPIN_SYNC5;
	for( int i=0; i<_countof(m_nSpinSyncID); i++ )
	{
		CSpinButtonCtrl *pSpin = ((CSpinButtonCtrl*)GetDlgItem(m_nSpinSyncID[i]));
		pSpin->SetRange(0, 7);
	}

	m_nSpinClockID[0] = IDC_SPIN_CLOCK1;
	m_nSpinClockID[1] = IDC_SPIN_CLOCK2;
	m_nSpinClockID[2] = IDC_SPIN_CLOCK3;
	m_nSpinClockID[3] = IDC_SPIN_CLOCK4;
	m_nSpinClockID[4] = IDC_SPIN_CLOCK5;
	for( int i=0; i<_countof(m_nSpinClockID); i++ )
	{
		CSpinButtonCtrl *pSpin = ((CSpinButtonCtrl*)GetDlgItem(m_nSpinClockID[i]));
		pSpin->SetRange(0, 7);
	}

	//Clock
	m_nEditClockID[0] = IDC_EDIT_CLOCK1;
	m_nEditClockID[1] = IDC_EDIT_CLOCK2;
	m_nEditClockID[2] = IDC_EDIT_CLOCK3;
	m_nEditClockID[3] = IDC_EDIT_CLOCK4;
	m_nEditClockID[4] = IDC_EDIT_CLOCK5;

	//Sync
	m_nEditSyncID[0] = IDC_EDIT_SYNC1;
	m_nEditSyncID[1] = IDC_EDIT_SYNC2;
	m_nEditSyncID[2] = IDC_EDIT_SYNC3;
	m_nEditSyncID[3] = IDC_EDIT_SYNC4;
	m_nEditSyncID[4] = IDC_EDIT_SYNC5;

	//Delay
	m_nDataPos = 0;
	m_nButtonDelayID[0] = IDC_CHECK_DELAY0;
	m_nButtonDelayID[1] = IDC_CHECK_DELAY1;
	m_nButtonDelayID[2] = IDC_CHECK_DELAY2;
	m_nButtonDelayID[3] = IDC_CHECK_DELAY3;
	m_nButtonDelayID[4] = IDC_CHECK_DELAY4;
	m_nButtonDelayID[5] = IDC_CHECK_DELAY5;
	m_nButtonDelayID[6] = IDC_CHECK_DELAY6;
	m_nButtonDelayID[7] = IDC_CHECK_DELAY7;

	InitDisplay();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}


void StCheckEllitoLVDSDlg::OnLbnSelchangeListLmap()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	//IDC_LIST_LMAP
	CListBox *aListBox = (CListBox *)GetDlgItem(IDC_LIST_LMAP);
	INT nCount = aListBox->GetCount();
	for( int i=0; i<nCount; i++ )
	{
		if( aListBox->GetSel(i) )
		{
			m_nDataPos = i;
			ChangeDelay(m_nDataPos);
			break;
		}
	}
}

void StCheckEllitoLVDSDlg::ChangeClock(INT nClock)
{
	if( m_nSpinClockID[nClock]==0 )	return;
	if( m_nEditClockID[nClock]==0 )	return;

	TCHAR szText[128];

	GetDlgItem(m_nEditClockID[nClock])->GetWindowText(szText,_countof(szText));
	INT nClockData = _ttoi(szText);

	m_clock[nClock] = (char)nClockData;
	for( int i=m_nSliceSETable[nClock][0]; i<=m_nSliceSETable[nClock][1]; i++ )
	{
		ChangeListBox(i);
	}
}

void StCheckEllitoLVDSDlg::OnEnChangeEditClock1()
{
	// TODO:  これが RICHEDIT コントロールの場合、このコントロールが
	// この通知を送信するには、__super::OnInitDialog() 関数をオーバーライドし、
	// CRichEditCtrl().SetEventMask() を
	// OR 状態の ENM_CHANGE フラグをマスクに入れて呼び出す必要があります。

	// TODO:  ここにコントロール通知ハンドラー コードを追加してください。
	ChangeClock(0);
}


void StCheckEllitoLVDSDlg::OnEnChangeEditClock2()
{
	// TODO:  これが RICHEDIT コントロールの場合、このコントロールが
	// この通知を送信するには、__super::OnInitDialog() 関数をオーバーライドし、
	// CRichEditCtrl().SetEventMask() を
	// OR 状態の ENM_CHANGE フラグをマスクに入れて呼び出す必要があります。

	// TODO:  ここにコントロール通知ハンドラー コードを追加してください。
	ChangeClock(1);
}


void StCheckEllitoLVDSDlg::OnEnChangeEditClock3()
{
	// TODO:  これが RICHEDIT コントロールの場合、このコントロールが
	// この通知を送信するには、__super::OnInitDialog() 関数をオーバーライドし、
	// CRichEditCtrl().SetEventMask() を
	// OR 状態の ENM_CHANGE フラグをマスクに入れて呼び出す必要があります。

	// TODO:  ここにコントロール通知ハンドラー コードを追加してください。
	ChangeClock(2);
}


void StCheckEllitoLVDSDlg::OnEnChangeEditClock4()
{
	// TODO:  これが RICHEDIT コントロールの場合、このコントロールが
	// この通知を送信するには、__super::OnInitDialog() 関数をオーバーライドし、
	// CRichEditCtrl().SetEventMask() を
	// OR 状態の ENM_CHANGE フラグをマスクに入れて呼び出す必要があります。

	// TODO:  ここにコントロール通知ハンドラー コードを追加してください。
	ChangeClock(3);
}


void StCheckEllitoLVDSDlg::OnEnChangeEditClock5()
{
	// TODO:  これが RICHEDIT コントロールの場合、このコントロールが
	// この通知を送信するには、__super::OnInitDialog() 関数をオーバーライドし、
	// CRichEditCtrl().SetEventMask() を
	// OR 状態の ENM_CHANGE フラグをマスクに入れて呼び出す必要があります。

	// TODO:  ここにコントロール通知ハンドラー コードを追加してください。
	ChangeClock(4);
}


//void StCheckEllitoLVDSDlg::OnEnChangeEditSync(UINT nID)
//{
//	// TODO:  これが RICHEDIT コントロールの場合、このコントロールが
//	// この通知を送信するには、__super::OnInitDialog() 関数をオーバーライドし、
//	// CRichEditCtrl().SetEventMask() を
//	// OR 状態の ENM_CHANGE フラグをマスクに入れて呼び出す必要があります。
//
//	// TODO:  ここにコントロール通知ハンドラー コードを追加してください。
//}
void StCheckEllitoLVDSDlg::ChangeSync(INT nSync) 
{
	if( m_nEditSyncID[nSync]==0 )	return;

	TCHAR szText[128];
	GetDlgItem(m_nEditSyncID[nSync])->GetWindowText(szText,_countof(szText));
	m_sync[nSync] = (char)_ttoi(szText);
}


void StCheckEllitoLVDSDlg::OnEnChangeEditSync1()
{
	// TODO:  これが RICHEDIT コントロールの場合、このコントロールが
	// この通知を送信するには、__super::OnInitDialog() 関数をオーバーライドし、
	// CRichEditCtrl().SetEventMask() を
	// OR 状態の ENM_CHANGE フラグをマスクに入れて呼び出す必要があります。

	// TODO:  ここにコントロール通知ハンドラー コードを追加してください。
	ChangeSync(0);
}


void StCheckEllitoLVDSDlg::OnEnChangeEditSync2()
{
	// TODO:  これが RICHEDIT コントロールの場合、このコントロールが
	// この通知を送信するには、__super::OnInitDialog() 関数をオーバーライドし、
	// CRichEditCtrl().SetEventMask() を
	// OR 状態の ENM_CHANGE フラグをマスクに入れて呼び出す必要があります。

	// TODO:  ここにコントロール通知ハンドラー コードを追加してください。
	ChangeSync(1);
}


void StCheckEllitoLVDSDlg::OnEnChangeEditSync3()
{
	// TODO:  これが RICHEDIT コントロールの場合、このコントロールが
	// この通知を送信するには、__super::OnInitDialog() 関数をオーバーライドし、
	// CRichEditCtrl().SetEventMask() を
	// OR 状態の ENM_CHANGE フラグをマスクに入れて呼び出す必要があります。

	// TODO:  ここにコントロール通知ハンドラー コードを追加してください。
	ChangeSync(2);
}


void StCheckEllitoLVDSDlg::OnEnChangeEditSync4()
{
	// TODO:  これが RICHEDIT コントロールの場合、このコントロールが
	// この通知を送信するには、__super::OnInitDialog() 関数をオーバーライドし、
	// CRichEditCtrl().SetEventMask() を
	// OR 状態の ENM_CHANGE フラグをマスクに入れて呼び出す必要があります。

	// TODO:  ここにコントロール通知ハンドラー コードを追加してください。
	ChangeSync(3);
}


void StCheckEllitoLVDSDlg::OnEnChangeEditSync5()
{
	// TODO:  これが RICHEDIT コントロールの場合、このコントロールが
	// この通知を送信するには、__super::OnInitDialog() 関数をオーバーライドし、
	// CRichEditCtrl().SetEventMask() を
	// OR 状態の ENM_CHANGE フラグをマスクに入れて呼び出す必要があります。

	// TODO:  ここにコントロール通知ハンドラー コードを追加してください。
	ChangeSync(4);
}


void StCheckEllitoLVDSDlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: ここにメッセージ ハンドラー コードを追加するか、既定の処理を呼び出します。

	__super::OnVScroll(nSBCode, nPos, pScrollBar);
}

void StCheckEllitoLVDSDlg::OnCheckDelay(UINT nID) 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	m_delay[m_nDataPos] = nID - IDC_CHECK_DELAY0;
	ChangeDelay(m_nDataPos);
	ChangeListBox(m_nDataPos);
}

void StCheckEllitoLVDSDlg::ChangeDelay(INT nData) 
{
	if( m_nButtonDelayID[0]==0 ) return;
	CString szText;
	szText.Format(_T("%02d"),nData);
	GetDlgItem(IDC_EDIT_DELAY)->SetWindowText(szText);

	for( int i=0; i<sizeof(m_nButtonDelayID)/sizeof(m_nButtonDelayID[0]); i++ )
	{
		CButton *aCheckBox = (CButton *)GetDlgItem(m_nButtonDelayID[i]);
		int nCheck = BST_UNCHECKED;
		if( i==m_delay[nData] )
		{
			//押した状態-------
			nCheck = BST_CHECKED;
		}
		aCheckBox->SetCheck(nCheck);
	}
}

void StCheckEllitoLVDSDlg::ChangeListBox(INT nData) 
{
	INT nClockData = 0;
	for( int i=0; i<sizeof(m_nSliceSETable)/sizeof(m_nSliceSETable[0]); i++ )
	{
		if( m_nSliceSETable[i][0]<=nData && m_nSliceSETable[i][1]>=nData )
		{
			nClockData = m_clock[i];
			break;
		}
	}
	INT nLVDS = 0;
	for( int j=0; j<15; j++ )
	{
		if( nLMapTable[nClockData][j]==m_delay[nData] )
		{
			nLVDS = j;
			break;
		}
	}
	TCHAR szText[128];
	TCHAR szTmp[128];
	_stprintf_s( szText, _countof(szText), _T("Data%02d, 0x%04X, "),nData,m_wLMAP[nData] );
	for( int j=0; j<15; j++ )
	{
		INT nShift = (3-((j+1)/4))*4 + 3 - ((j+1)&3);
		_stprintf_s( szTmp, _countof(szTmp), _T("%s,"), ((m_wLMAP[nData]>>nShift)&1)>0?(j==nLVDS?_T("@"):_T("0")):(j==nLVDS?_T("X"):_T("x")) );
		if( j==12 )
		{
			_tcscat(szText,_T("  "));
		}
		_tcscat(szText,szTmp);
	}

	CListBox *pListLMap = (CListBox *)GetDlgItem(IDC_LIST_LMAP);
	pListLMap->DeleteString(nData);
	pListLMap->InsertString(nData,szText);

}


void StCheckEllitoLVDSDlg::OnBnClickedOk()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	CDialogEx::OnOK();
}


void StCheckEllitoLVDSDlg::OnBnClickedCancel()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	CDialogEx::OnCancel();

}


void StCheckEllitoLVDSDlg::OnBnClickedButtonSave()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	//Defの値をファイル保存
	//カレントディレクトリが変わってしまう為---------
	TCHAR chCurrentDir[MAX_PATH];
	GetCurrentDirectory( sizeof( chCurrentDir ), chCurrentDir ); 
	//------------------------------------------

	CString szDeviceModelName;
	CString szSerialNumber;
	StString aText;
	BOOL bReval = GetDeviceModelName(aText);

	if( bReval )
		szDeviceModelName = aText.GetUnicode();
	bReval = GetSerialNumber(aText);
	if( bReval )
		szSerialNumber = aText.GetUnicode();

	CTime gt=CTime::GetCurrentTime();
	CString dateFormat = gt.Format("%Y%m%d%H%M%S");

	CString strFileName = szDeviceModelName + "_" + szSerialNumber + "_" + dateFormat + "_LMAP.txt";

	dateFormat = gt.Format("%Y%m%d");
	CString szFullPath = m_szSavePath + "/" + szDeviceModelName + "/" + dateFormat + "/log";
	//gCreateDirectory( outPath );

	CStCommon::CheckDirectoryExist(szFullPath,TRUE);

	CFileDialog dlgLoad(
		FALSE, 0, strFileName,
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		TEXT("Text File(*.txt)|*.txt|AllFiles(*.*)|*.*||"));

	dlgLoad.m_ofn.lpstrTitle = TEXT("Select File");
	dlgLoad.m_ofn.lpstrDefExt = TEXT("txt");
//	dlgLoad.m_ofn.lpstrFileName = TEXT("tes.txt");
	dlgLoad.m_ofn.lpstrInitialDir = szFullPath;

	//ファイル名
	//LMap_SerialNumber_YYYYMMDDhhmmss.txt

	int res = dlgLoad.DoModal();

	//カレントディレクトリが変わってしまう為
	SetCurrentDirectory( chCurrentDir );

	if(res == IDOK)
	{
		CString szFullFileName = _T("");
		szFullFileName = dlgLoad.m_ofn.lpstrFile;
		if( szFullFileName.GetLength() )
		{
			//Save
			CStdioFile stdFile;
			if(stdFile.Open( szFullFileName, CFile::modeCreate|CFile::modeWrite,NULL)!=0)
			{
				CString szText;
				TCHAR szTmp[32];
				for( int i=0; i<sizeof(m_wLMAP)/sizeof(m_wLMAP[0]); i++ )
				{
					//INT nLine = (i+7)/8;
					INT nClock = m_defClock[(i+7)/8];
					INT nLVDS = 0;
					for( int j=0; j<15; j++ )
					{
						if( nLMapTable[nClock][j]==m_defDelay[i] )
						{
							nLVDS = j;
							break;
						}
					}
					szText.Format(_T("Data%02d, 0x%04X, "),i,m_wLMAP[i]);
					for( int j=0; j<15; j++ )
					{
						INT nShift = (3-((j+1)/4))*4 + 3 - ((j+1)&3);
						_stprintf_s( szTmp, _countof(szTmp), _T("%s,"), ((m_wLMAP[i]>>nShift)&1)>0?(j==nLVDS?_T("@"):_T("0")):(j==nLVDS?_T("X"):_T("x")) );
						if( j==12 )
						{
							szText += _T("  ");
						}
						szText += szTmp;
					}
					stdFile.WriteString(szText+"\n");
				}
				stdFile.Close();
				AfxMessageBox(_T("Saved successfully."),MB_OK|MB_ICONEXCLAMATION);
			}
		}
	}
}


void StCheckEllitoLVDSDlg::OnBnClickedButtonDefault()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	//Data copy
	memcpy( m_delay, m_defDelay, sizeof(m_delay) );
	memcpy( m_clock, m_defClock, sizeof(m_clock) );
	memcpy( m_sync, m_defSync, sizeof(m_sync) );

	InitDisplay();
}


void StCheckEllitoLVDSDlg::OnBnClickedButtonWrite()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。


	BOOL bReval = TRUE;

	//▼1.0.0.1046
	//if( !m_pControlRetry ) return;
	StCheckControlRetry *aControlRetry = NULL;
	StDeviceGenICam *aDeviceGenICam = NULL;
	if( m_DeviceBase->m_ClassName=="StDeviceMultiCam" )	//CL用
	{
		if( !m_pControlRetry ) return;
		aControlRetry = (StCheckControlRetry *)m_pControlRetry;
	}
	else
	if( m_DeviceBase->m_ClassName=="StDeviceGenICam" )	//GenICam用
	{
		aDeviceGenICam = (StDeviceGenICam *)m_DeviceBase;
		UINT uiData = 0x000A;
		bReval = aDeviceGenICam->WriteMain(0xAFE0,(char *)&uiData, sizeof(uiData));
		if( !bReval ) return;
	}
	else
	{
		return;
	}
	//▲1.0.0.1046

	do
	{
		TCHAR cmdText[64];
		//TCHAR revText[64];
		//▼1.0.0.1046
		//StCheckControlRetry *aControlRetry = (StCheckControlRetry *)m_pControlRetry;
		//▲1.0.0.1046

		//Delay   ----   
		//Slice A

		if( memcmp( &m_defDelay[0], &m_delay[0], sizeof(m_delay[0]) ) )
		{
			//▼1.0.0.1046
			//_stprintf_s( cmdText, _countof(cmdText), _T("0x02,\"SED %d 0x%08x\",0x0d"), 0, (m_delay[0]&7)<<9 );
			//aControlRetry->SetSendData(cmdText);
			//bReval = aControlRetry->Execute();
			UINT uiData = (m_delay[0]&7)<<9;

			if( aControlRetry )
			{
				_stprintf_s( cmdText, _countof(cmdText), _T("0x02,\"SED %d 0x%08x\",0x0d"), 0, uiData );
				aControlRetry->SetSendData(cmdText);
				bReval = aControlRetry->Execute();
			}
			else
			if( aDeviceGenICam )
			{
				bReval = aDeviceGenICam->WriteMain(0xB000 + 0 * 4, (char *)&uiData, sizeof(uiData) );
			}
			//▲1.0.0.1046
			if( !bReval ) break;
			m_defDelay[0] = m_delay[0];
		}


		//Slice B0 - B3
		for( int i=0; i<4 && bReval; i++ )
		{
			for( int j=0; j<2 && bReval; j++ )	//0-3  4-7
			{
				INT nIndex = i*8 + j*4 + 1;
				if( memcmp( &m_defDelay[ nIndex ], &m_delay[ nIndex ], sizeof(m_delay[0]) * 4 ) )
				{
					UINT unData = 0;
					for( int loop=0; loop<4; loop++ )
					{
						unData |= (m_delay[nIndex+loop]&7)<<((3-loop)*3);
					}
					INT aIndex = (i+1) * 3 + j;
					//▼1.0.0.1046
					//_stprintf_s( cmdText, _countof(cmdText), _T("0x02,\"SED %d 0x%08x\",0x0d"), aIndex, unData );	
					//aControlRetry->SetSendData(cmdText);
					//bReval = aControlRetry->Execute();

					if( aControlRetry )
					{
						_stprintf_s( cmdText, _countof(cmdText), _T("0x02,\"SED %d 0x%08x\",0x0d"), aIndex, unData );	
						aControlRetry->SetSendData(cmdText);
						bReval = aControlRetry->Execute();
					}
					else
					if( aDeviceGenICam )
					{
						bReval = aDeviceGenICam->WriteMain(0xB000 + aIndex * 4, (char *)&unData, sizeof(unData) );
					}
					//▲1.0.0.1046

					if( !bReval ) break;
					memcpy( &m_defDelay[ nIndex ], &m_delay[ nIndex ], sizeof(m_delay[0]) * 4 );
				}
			}
		}

		//Clock & Sync ----   SED 2 0x00000000
		for( int i=0; i<sizeof(m_clock)/sizeof(m_clock[0]) && bReval; i++ )
		{
			if( m_clock[i]!=m_defClock[i] || m_sync[i]!=m_defSync[i] )
			{
				//▼1.0.0.1046
				//_stprintf_s( cmdText, _countof(cmdText), _T("0x02,\"SED %d 0x%08x\",0x0d"), i * 3 + 2, ((m_sync[i]&7)<<3) | (m_clock[i]&7) );	
				//aControlRetry->SetSendData(cmdText);
				//bReval = aControlRetry->Execute();
				UINT unData = ((m_sync[i]&7)<<3) | (m_clock[i]&7);
				INT aIndex = i * 3 + 2;
				if( aControlRetry )
				{
					_stprintf_s( cmdText, _countof(cmdText), _T("0x02,\"SED %d 0x%08x\",0x0d"), aIndex, unData );	
					aControlRetry->SetSendData(cmdText);
					bReval = aControlRetry->Execute();
				}
				else
				if( aDeviceGenICam )
				{
					bReval = aDeviceGenICam->WriteMain( 0xB000 + aIndex * 4, (char *)&unData, sizeof(unData) );
				}
				//▲1.0.0.1046


				if( !bReval ) break;
				m_defClock[i] = m_clock[i];
				m_defSync[i] = m_sync[i];
			}
		}

	}while(0);

}

BOOL StCheckEllitoLVDSDlg::function(LPCTSTR szFunc, LPCTSTR szArgument)
{
	BOOL bReval = FALSE;
	CString szFunction(szFunc);
	if( szFunction=="SetSaveFilePath" )
	{
		SetSaveFilePath(szArgument);
		bReval = TRUE;
	}
	return bReval;
}


void StCheckEllitoLVDSDlg::OnDestroy()
{
	__super::OnDestroy();

	// TODO: ここにメッセージ ハンドラー コードを追加します。
	if( m_pControlRetry )
	{
		StCheckControlRetry *aControlRetry = (StCheckControlRetry *)m_pControlRetry;
		if( m_nDefRetry>=0 )
		{
			aControlRetry->SetRetry(m_nDefRetry);
		}
		if( m_szReturn.GetLength()>0 )
		{
			aControlRetry->SetRecieveData(m_szReturn);
		}
		if( m_dwDefTimeout[0]!=0xffffffff )
		{
			aControlRetry->SetCommTimeout(m_dwDefTimeout[0],m_dwDefTimeout[1],m_dwDefTimeout[2],m_dwDefTimeout[3],m_dwDefTimeout[4]);
		}
	}
}

