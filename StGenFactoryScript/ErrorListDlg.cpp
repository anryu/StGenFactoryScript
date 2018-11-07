// ErrorListDlg.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "StGenFactoryScript.h"
#include "ErrorListDlg.h"
#include "afxdialogex.h"


// CErrorListDlg �_�C�A���O

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


// CErrorListDlg ���b�Z�[�W �n���h���[
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

	// TODO:  �����ɏ�������ǉ����Ă�������
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
	// ��O : OCX �v���p�e�B �y�[�W�͕K�� FALSE ��Ԃ��܂��B
}


void CErrorListDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	// TODO: �����Ƀ��b�Z�[�W �n���h���[ �R�[�h��ǉ����܂��B
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
