// StInputSensorSerialDlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "StGenFactoryScript.h"
#include "StInputSensorSerialDlg.h"
#include "afxdialogex.h"


// StInputSensorSerialDlg ダイアログ
INT nSensorSerialComboID[] = { IDC_COMBO_SENSORSERIAL1, IDC_COMBO_SENSORSERIAL2, IDC_COMBO_SENSORSERIAL3, IDC_COMBO_SENSORSERIAL4, IDC_COMBO_SENSORSERIAL5,
					   //▼1.0.0.1026
								//IDC_COMBO_SENSORSERIAL6, IDC_COMBO_SENSORSERIAL7, IDC_COMBO_SENSORSERIAL8, IDC_COMBO_SENSORSERIAL9, IDC_COMBO_SENSORSERIAL10, IDC_COMBO_SENSORSERIAL11, IDC_COMBO_SENSORSERIAL12 };
								IDC_COMBO_SENSORSERIAL6, IDC_COMBO_SENSORSERIAL7, IDC_COMBO_SENSORSERIAL8, IDC_COMBO_SENSORSERIAL9, IDC_COMBO_SENSORSERIAL10, IDC_COMBO_SENSORSERIAL11, IDC_COMBO_SENSORSERIAL12
								, IDC_COMBO_SENSORSERIAL13, IDC_COMBO_SENSORSERIAL14, IDC_COMBO_SENSORSERIAL15, IDC_COMBO_SENSORSERIAL16, IDC_COMBO_SENSORSERIAL17, IDC_COMBO_SENSORSERIAL18
								, IDC_COMBO_SENSORSERIAL19, IDC_COMBO_SENSORSERIAL20, IDC_COMBO_SENSORSERIAL21, IDC_COMBO_SENSORSERIAL22, IDC_COMBO_SENSORSERIAL23, IDC_COMBO_SENSORSERIAL24
								};
					   //▲1.0.0.1026


IMPLEMENT_DYNAMIC(StInputSensorSerialDlg, CDialogEx)

StInputSensorSerialDlg::StInputSensorSerialDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(StInputSensorSerialDlg::IDD, pParent)
{

}

StInputSensorSerialDlg::StInputSensorSerialDlg( LPCTSTR szClassName, LPCTSTR szVarName, StDeviceBase *aDeviceBase, CDialogEx *aDialogEx, CWnd* pParent /*=NULL*/)
	: CDialogEx(StInputSensorSerialDlg::IDD, pParent)
	, StCheckBase( szClassName, szVarName, aDeviceBase, aDialogEx )
{

}
StInputSensorSerialDlg::~StInputSensorSerialDlg()
{
}

void StInputSensorSerialDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_SENSORSERIAL, m_StaticSensorSerial);
}


BEGIN_MESSAGE_MAP(StInputSensorSerialDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &StInputSensorSerialDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &StInputSensorSerialDlg::OnBnClickedCancel)
   //▼1.0.0.1026
	//ON_CONTROL_RANGE(CBN_SELCHANGE, IDC_COMBO_SENSORSERIAL1, IDC_COMBO_SENSORSERIAL12, OnCmbSelChanged)
	ON_CONTROL_RANGE(CBN_SELCHANGE, IDC_COMBO_SENSORSERIAL1, IDC_COMBO_SENSORSERIAL24, OnCmbSelChanged)
	//▲1.0.0.1026
END_MESSAGE_MAP()


// StInputSensorSerialDlg メッセージ ハンドラー


void StInputSensorSerialDlg::OnBnClickedOk()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。

	CString szSensorSerial(_T(""));
	for( int i=0; i<_countof(nSensorSerialComboID); i++ )
	{
		CString szTmp;
		CComboBox objComboBox;
		objComboBox.Attach(GetDlgItem(nSensorSerialComboID[i])->m_hWnd);
		objComboBox.GetWindowText(szTmp);
		szSensorSerial += szTmp;
		objComboBox.Detach();
	}
	//▼1.0.0.1027
	szSensorSerial.Trim();
	//▲1.0.0.1027
	m_szSensorSerial = szSensorSerial;

	CDialogEx::OnOK();
}


void StInputSensorSerialDlg::OnBnClickedCancel()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	CDialogEx::OnCancel();
}


BOOL StInputSensorSerialDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  ここに初期化を追加してください
	LPCTSTR szText[] = {_T(" "),_T("0"),_T("1"),_T("2"),_T("3"),_T("4"),_T("5"),_T("6"),_T("7"),_T("8"),_T("9")
					   ,_T("A"),_T("B"),_T("C"),_T("D"),_T("E"),_T("F"),_T("G"),_T("H"),_T("I"),_T("J"),_T("K")
					   ,_T("L"),_T("M"),_T("N"),_T("O"),_T("P"),_T("Q"),_T("R"),_T("S"),_T("T"),_T("U"),_T("V")
					   //▼1.0.0.1026
					   //,_T("W"),_T("X"),_T("Y"),_T("Z"),_T("."),_T("-"),_T("_") };
					   ,_T("W"),_T("X"),_T("Y"),_T("Z")
					   ,_T("a"),_T("b"),_T("c"),_T("d"),_T("e"),_T("f"),_T("g"),_T("h"),_T("i"),_T("j"),_T("k")
					   ,_T("l"),_T("m"),_T("n"),_T("o"),_T("p"),_T("q"),_T("r"),_T("s"),_T("t"),_T("u"),_T("v")
					   ,_T("w"),_T("x"),_T("y"),_T("z")
					   ,_T("."),_T("-"),_T("_") };
					   //▼1.0.0.1026

	for( int i=0; i<_countof(nSensorSerialComboID); i++ )
	{
		CComboBox objComboBox;
		objComboBox.Attach(GetDlgItem(nSensorSerialComboID[i])->m_hWnd);
		for( int j=objComboBox.GetCount()-1; j>=0; j-- )
		{
			objComboBox.DeleteString(j);
		}
		INT nSel = 0;
		for( int j=0; j<_countof(szText); j++ )
		{
			objComboBox.AddString(szText[j]);
			if( m_szSensorSerial.GetLength()>i )
			{
				if( m_szSensorSerial.Mid(i,1)==szText[j] )
					nSel=j;
			}
		}
		objComboBox.SetCurSel(nSel);
		objComboBox.Detach();
	}

	OKButtonEnabled();

	//▼1.0.0.1043
	DisplaySensorSerial();
	//▲1.0.0.1043

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}
void StInputSensorSerialDlg::OnCmbSelChanged(UINT nID)
{
	OKButtonEnabled();
	DisplaySensorSerial();
}

BOOL StInputSensorSerialDlg::OKButtonEnabled(void)
{
	BOOL bReval = FALSE;

	for( int i=0; i<_countof(nSensorSerialComboID) && bReval==FALSE; i++ )
	{
		if( ((CComboBox*)GetDlgItem(nSensorSerialComboID[i]))->GetCurSel()>0 )
		{
			bReval = TRUE;
		}
	}

	GetDlgItem(IDOK)->EnableWindow(bReval);

	return bReval;
}

//▼1.0.0.1043
void StInputSensorSerialDlg::DisplaySensorSerial(void)
{

	CString szSensorSerial(_T(""));
	for( int i=0; i<_countof(nSensorSerialComboID); i++ )
	{
		CString szTmp;
		CComboBox objComboBox;
		objComboBox.Attach(GetDlgItem(nSensorSerialComboID[i])->m_hWnd);
		objComboBox.GetWindowText(szTmp);
		szSensorSerial += szTmp;
		objComboBox.Detach();
	}
	szSensorSerial.Trim();
	m_StaticSensorSerial.SetWindowText(szSensorSerial); 
}
//▲1.0.0.1043

//-------------------------------------------------------------------------------
//検査実行
//-------------------------------------------------------------------------------
BOOL StInputSensorSerialDlg::Execute(void)
{
	//未使用
	BOOL bReval = FALSE;
	return bReval;
}

//▼1.0.0.1025
BOOL StInputSensorSerialDlg::GetFunction(LPCTSTR szFunc, LPCTSTR szArg, CString &szValue)
{
	BOOL bReval = FALSE;
	CString szFunction(szFunc);
	if( szFunction=="GetSensorSerial" )
	{
		szValue = m_szSensorSerial;
		bReval = TRUE;
	}
	return bReval;
}
//▲1.0.0.1025
