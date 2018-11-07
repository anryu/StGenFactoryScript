#pragma once

#include "CustomDialogEx.h"

// StHttpControlDlg ダイアログ

class StHttpControlDlg : public CCustomDialogEx
{
	DECLARE_DYNAMIC(StHttpControlDlg)

public:
	StHttpControlDlg(CWnd* pParent = NULL);   // 標準コンストラクター
	virtual ~StHttpControlDlg();

	BOOL function(LPCTSTR szFunc, LPCTSTR szArgument);
	void GetAddress(StString &szAddress){szAddress = m_Address;}

private:
	StString m_Address;

public:
// ダイアログ データ
	enum { IDD = IDD_DIALOG_HTTPCONTROL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonHttpput();
	afx_msg void OnBnClickedButtonHttpget();
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
};
