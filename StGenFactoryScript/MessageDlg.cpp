// MessageDlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "StGenFactoryScript.h"
#include "MessageDlg.h"
#include "afxdialogex.h"


// CMessageDlg ダイアログ

IMPLEMENT_DYNAMIC(CMessageDlg, CDialogEx)

CMessageDlg::CMessageDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMessageDlg::IDD, pParent)
{
	m_bOKShow = TRUE;
	m_bNGShow = TRUE;
	m_bTextShow = TRUE;
	m_szMessage = _T("");
	m_szTextButton = _T("");

	m_nJudge = -1;
}

CMessageDlg::~CMessageDlg()
{
}

void CMessageDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMessageDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CMessageDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON_NG, &CMessageDlg::OnBnClickedButtonNg)
	ON_BN_CLICKED(IDC_BUTTON_TEXT, &CMessageDlg::OnBnClickedButtonText)
END_MESSAGE_MAP()


// CMessageDlg メッセージ ハンドラー


BOOL CMessageDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  ここに初期化を追加してください
	CButton aButton;
	aButton.Attach(GetDlgItem(IDOK)->m_hWnd);
	aButton.ShowWindow(m_bOKShow);
	aButton.Detach();
	aButton.Attach(GetDlgItem(IDC_BUTTON_NG)->m_hWnd);
	aButton.ShowWindow(m_bNGShow);
	aButton.Detach();
	aButton.Attach(GetDlgItem(IDC_BUTTON_TEXT)->m_hWnd);
	aButton.SetWindowText(m_szTextButton);
	aButton.ShowWindow(m_bTextShow);
	aButton.Detach();

	CStatic aStatic;
	aStatic.Attach(GetDlgItem(IDC_STATIC_MESSAGE)->m_hWnd);
	CString szTmp = m_szMessage;
	CString szMessage(_T(""));
	//改行に認識されないため
	INT iPos;
	do
	{
		//CString szdebug;
		//for( int i=0; i<szTmp.GetLength(); i++ )
		//{
		//	szdebug = szTmp.Mid(i,1);
		//}
		iPos = szTmp.Find(TEXT("\\n"));
		if( iPos<0 )
		{
			szMessage += szTmp;
			break;
		}
		else
		{
			szMessage += szTmp.Left(iPos);
			szMessage += _T("\n");
			szTmp = szTmp.Mid(iPos+2);
		}
	}while(1);
	aStatic.SetWindowText(szMessage);
	aStatic.Detach();

	//CEdit aStatic;
	//aStatic.Attach(GetDlgItem(IDC_EDIT_MESSAGE)->m_hWnd);
	//aStatic.SetWindowText(m_szMessage);
	//aStatic.Detach();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}


void CMessageDlg::OnBnClickedOk()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	m_nJudge = 0;
	CDialogEx::OnOK();
}


void CMessageDlg::OnBnClickedButtonNg()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	m_nJudge = 1;
	CDialogEx::OnOK();
}


void CMessageDlg::OnBnClickedButtonText()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	m_nJudge = 2;
	CDialogEx::OnOK();
}
