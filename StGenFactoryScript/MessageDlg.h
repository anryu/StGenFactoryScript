#pragma once

#include "Resource.h"
// CMessageDlg �_�C�A���O

class CMessageDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CMessageDlg)

public:
	CMessageDlg(CWnd* pParent = NULL);   // �W���R���X�g���N�^�[
	virtual ~CMessageDlg();

	void SetOKButtonShow(BOOL bData){m_bOKShow = bData;}
	void SetNGButtonShow(BOOL bData){m_bNGShow = bData;}
	void SetTextButtonShow(BOOL bData){m_bTextShow = bData;}
	void SetTextButton(LPCTSTR szMessage){m_szTextButton = szMessage;}
	void SetMessage(LPCTSTR szMessage){m_szMessage = szMessage;}
	INT GetJudge(void){return m_nJudge;}

private:
	BOOL m_bOKShow;
	BOOL m_bNGShow;
	BOOL m_bTextShow;
	CString m_szMessage;
	CString m_szTextButton;

	INT m_nJudge;

public:

// �_�C�A���O �f�[�^
	enum { IDD = IDD_DIALOG_MESSAGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButtonNg();
	afx_msg void OnBnClickedButtonText();
};
