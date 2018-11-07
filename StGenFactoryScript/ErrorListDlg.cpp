// ErrorListDlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "StGenFactoryScript.h"
#include "ErrorListDlg.h"
#include "afxdialogex.h"


// CErrorListDlg ダイアログ

IMPLEMENT_DYNAMIC(CErrorListDlg, CDialogEx)

CErrorListDlg::CErrorListDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CErrorListDlg::IDD, pParent)
{

}

CErrorListDlg::~CErrorListDlg()
{
}

void CErrorListDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CErrorListDlg, CDialogEx)
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()


// CErrorListDlg メッセージ ハンドラー
void CErrorListDlg::SetStringList(LPCTSTR szString)
{
	m_vTextList.push_back(szString);
}
void CErrorListDlg::ClearStringList(void)
{
	m_vTextList.clear();
}


BOOL CErrorListDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  ここに初期化を追加してください
	//CString szMessage;
	//for( int i=0; i<m_vTextList.size(); i++ )
	//{
	//	szMessage+=m_vTextList[i];
	//	szMessage+=_T("\n");
	//}

	//CEdit objEdit;
	//objEdit.Attach(GetDlgItem(IDC_EDIT_ERRORLIST)->m_hWnd);
	//objEdit.SetWindowText(szMessage);
	////objEdit.LineScroll(objEdit.GetLineCount());
	//objEdit.Detach();


	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}


void CErrorListDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	// TODO: ここにメッセージ ハンドラー コードを追加します。
	CString szMessage;
	for( int i=0; i<m_vTextList.size(); i++ )
	{
		szMessage+=m_vTextList[i];
		szMessage+=_T("\n");
	}
	CEdit objEdit;
	objEdit.Attach(GetDlgItem(IDC_EDIT_ERRORLIST)->m_hWnd);
	objEdit.SetWindowText(szMessage);
	//objEdit.LineScroll(objEdit.GetLineCount());
	objEdit.Detach();



}
