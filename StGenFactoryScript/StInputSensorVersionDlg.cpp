// StInputSensorVersionDlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "StGenFactoryScript.h"
#include "StInputSensorVersionDlg.h"
#include "afxdialogex.h"


// StInputSensorVersionDlg ダイアログ
INT nComboID[] = { IDC_COMBO_SENSOR1, IDC_COMBO_SENSOR2, IDC_COMBO_SENSOR3, IDC_COMBO_SENSOR4, IDC_COMBO_SENSOR5,
					IDC_COMBO_SENSOR6, IDC_COMBO_SENSOR7, IDC_COMBO_SENSOR8, IDC_COMBO_SENSOR9, IDC_COMBO_SENSOR10,
					IDC_COMBO_SENSOR11, IDC_COMBO_SENSOR12, IDC_COMBO_SENSOR13, IDC_COMBO_SENSOR14, IDC_COMBO_SENSOR15,
					IDC_COMBO_SENSOR16, IDC_COMBO_SENSOR17, IDC_COMBO_SENSOR18, IDC_COMBO_SENSOR19, IDC_COMBO_SENSOR20 };

IMPLEMENT_DYNAMIC(StInputSensorVersionDlg, CDialogEx)

StInputSensorVersionDlg::StInputSensorVersionDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(StInputSensorVersionDlg::IDD, pParent)
{

}

StInputSensorVersionDlg::StInputSensorVersionDlg( LPCTSTR szClassName, LPCTSTR szVarName, StDeviceBase *aDeviceBase, CDialogEx *aDialogEx, CWnd* pParent /*=NULL*/)
	: CDialogEx(StInputSensorVersionDlg::IDD, pParent)
	, StCheckBase( szClassName, szVarName, aDeviceBase, aDialogEx )
{

}

StInputSensorVersionDlg::~StInputSensorVersionDlg()
{
	ClearCheckVersion();
}

void StInputSensorVersionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(StInputSensorVersionDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &StInputSensorVersionDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &StInputSensorVersionDlg::OnBnClickedCancel)
	ON_CONTROL_RANGE(CBN_SELCHANGE, IDC_COMBO_SENSOR1, IDC_COMBO_SENSOR20, OnCmbSelChanged)
END_MESSAGE_MAP()


// StInputSensorVersionDlg メッセージ ハンドラー


BOOL StInputSensorVersionDlg::OnInitDialog()
{
	__super::OnInitDialog();

	// TODO:  ここに初期化を追加してください
	LPCTSTR szText[] = {_T(" "),_T("0"),_T("1"),_T("2"),_T("3"),_T("4"),_T("5"),_T("6"),_T("7"),_T("8"),_T("9")
					   ,_T("A"),_T("B"),_T("C"),_T("D"),_T("E"),_T("F"),_T("G"),_T("H"),_T("I"),_T("J"),_T("K")
					   ,_T("L"),_T("M"),_T("N"),_T("O"),_T("P"),_T("Q"),_T("R"),_T("S"),_T("T"),_T("U"),_T("V")
					   ,_T("W"),_T("X"),_T("Y"),_T("Z"),_T(".") };

	for( int i=0; i<_countof(nComboID); i++ )
	{
		CComboBox objComboBox;
		objComboBox.Attach(GetDlgItem(nComboID[i])->m_hWnd);
		for( int j=objComboBox.GetCount()-1; j>=0; j-- )
		{
			objComboBox.DeleteString(j);
		}
		INT nSel = 0;
		for( int j=0; j<_countof(szText); j++ )
		{
			objComboBox.AddString(szText[j]);
			if( m_szSensorVersion.GetLength()>i )
			{
				if( m_szSensorVersion.Mid(i,1)==szText[j] )
					nSel=j;
			}
		}
		objComboBox.SetCurSel(nSel);
		objComboBox.Detach();
	}

	OKButtonEnabled();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

//-------------------------------------------------------------------------------
//検査実行
//-------------------------------------------------------------------------------
BOOL StInputSensorVersionDlg::Execute(void)
{
	//未使用
	BOOL bReval = FALSE;
	return bReval;
}


void StInputSensorVersionDlg::OnBnClickedOk()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。

	CString szSensroVersion(_T(""));
	for( int i=0; i<_countof(nComboID); i++ )
	{
		CString szTmp;
		CComboBox objComboBox;
		objComboBox.Attach(GetDlgItem(nComboID[i])->m_hWnd);
		objComboBox.GetWindowText(szTmp);
		szSensroVersion += szTmp;
		objComboBox.Detach();
	}
	szSensroVersion.Trim();
	m_szSensorVersion = szSensroVersion;

	CDialogEx::OnOK();
}


void StInputSensorVersionDlg::OnBnClickedCancel()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	CDialogEx::OnCancel();
}

void StInputSensorVersionDlg::OnCmbSelChanged(UINT nID)
{
	OKButtonEnabled();
}

BOOL StInputSensorVersionDlg::OKButtonEnabled(void)
{
	BOOL bReval = FALSE;

	for( int i=0; i<_countof(nComboID) && bReval==FALSE; i++ )
	{
		if( ((CComboBox*)GetDlgItem(nComboID[i]))->GetCurSel()>0 )
		{
			bReval = TRUE;
		}
	}

	GetDlgItem(IDOK)->EnableWindow(bReval);


	return bReval;
}

BOOL StInputSensorVersionDlg::AddCheckVersion( LPCTSTR szVersion )
{
	CString strVersion(szVersion);
	m_vCheckVersion.push_back(strVersion);
	return TRUE;
}

BOOL StInputSensorVersionDlg::ClearCheckVersion(void)
{
	m_vCheckVersion.clear();
	return m_vCheckVersion.size()==0?TRUE:FALSE;
}

BOOL StInputSensorVersionDlg::VersionCheck(void)
{
	INT nSize = m_vCheckVersion.size();
	BOOL bReval = FALSE;
	for( int i=0; i<nSize; i++ )
	{
		if( m_vCheckVersion[i]==m_szSensorVersion )
		{
			bReval=TRUE;
			break;
		}
	}
	return bReval;
}

BOOL StInputSensorVersionDlg::function(LPCTSTR szFunc, LPCTSTR szArgument)
{
	BOOL bReval = FALSE;
	CString szFunction(szFunc);
	if( szFunction=="AddCheckVersion" )
	{
		bReval = AddCheckVersion(szArgument);
	}
	else
	if( szFunction=="ClearCheckVersion" )
	{
		bReval = ClearCheckVersion();
	}
	else
	if( szFunction=="VersionCheck" )
	{
		bReval = VersionCheck();
	}
	return bReval;
}

//▼1.0.0.1025
BOOL StInputSensorVersionDlg::GetFunction(LPCTSTR szFunc, LPCTSTR szArg, CString &szValue)
{
	BOOL bReval = FALSE;
	CString szFunction(szFunc);
	if( szFunction=="GetSensorVersion" )
	{
		szValue = m_szSensorVersion;
		bReval = TRUE;
	}
	return bReval;
}
//▲1.0.0.1025


