// SettingTerminalDlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "StGenFactoryScript.h"
#include "SettingTerminalDlg.h"
#include "afxdialogex.h"


// CSettingTerminalDlg ダイアログ

IMPLEMENT_DYNAMIC(CSettingTerminalDlg, CDialogEx)

CSettingTerminalDlg::CSettingTerminalDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSettingTerminalDlg::IDD, pParent)
{
	m_pStructAddress = NULL;
}

CSettingTerminalDlg::~CSettingTerminalDlg()
{
}

void CSettingTerminalDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CSettingTerminalDlg, CDialogEx)
	ON_CBN_SELCHANGE(IDC_COMBO_SELECTOR, &CSettingTerminalDlg::OnCbnSelchangeComboSelector)
	ON_EN_KILLFOCUS(IDC_EDIT_TERMINALADDRESS, &CSettingTerminalDlg::OnEnKillfocusEditTerminaladdress)
	ON_BN_CLICKED(IDOK, &CSettingTerminalDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CSettingTerminalDlg メッセージ ハンドラー


BOOL CSettingTerminalDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  ここに初期化を追加してください
	m_StructAddress.Copy(m_pStructAddress);

	InitSelectorList();

	OnCbnSelchangeComboSelector();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}
void CSettingTerminalDlg::InitSelectorList(void)
{
	//if( !m_pStructPortName ) return;

	CComboBox objComboBox;

	objComboBox.Attach(GetDlgItem(IDC_COMBO_SELECTOR)->m_hWnd);
	for( int i=0; i<m_StructAddress.GetPortSelectorCount(); i++ )
	{
		CString szString;
		BOOL bReval = m_StructAddress.GetSelectorName(i, szString);
		if( bReval )
			objComboBox.AddString(szString);
	}
	objComboBox.SetCurSel(0);
	objComboBox.Detach();
}



void CSettingTerminalDlg::OnCbnSelchangeComboSelector()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。

	CString szSelectorName;
	CString szPortName;
	CComboBox objComboBox;
	objComboBox.Attach(GetDlgItem(IDC_COMBO_SELECTOR)->m_hWnd);
	objComboBox.GetWindowText(szSelectorName);
	objComboBox.Detach();

	m_StructAddress.GetPortName(szSelectorName,szPortName);

	CEdit objEdit;
	objEdit.Attach(GetDlgItem(IDC_EDIT_TERMINALADDRESS)->m_hWnd);
	objEdit.SetWindowText(szPortName);
	objEdit.Detach();

}


void CSettingTerminalDlg::OnEnKillfocusEditTerminaladdress()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	CString szSelectorName;
	CString szPortName;
	CComboBox objComboBox;
	objComboBox.Attach(GetDlgItem(IDC_COMBO_SELECTOR)->m_hWnd);
	objComboBox.GetWindowText(szSelectorName);
	objComboBox.Detach();

	CEdit objEdit;
	objEdit.Attach(GetDlgItem(IDC_EDIT_TERMINALADDRESS)->m_hWnd);
	objEdit.GetWindowText(szPortName);
	objEdit.Detach();

	m_StructAddress.SetPortName(szSelectorName,szPortName);

}


void CSettingTerminalDlg::OnBnClickedOk()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	m_pStructAddress->Copy(&m_StructAddress);

	CDialogEx::OnOK();
}
