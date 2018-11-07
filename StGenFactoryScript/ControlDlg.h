#pragma once


// CControlDlg ダイアログ
#include "StDeviceEBus.h"
#include <PvGenBrowserWnd.h>

class CControlDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CControlDlg)

public:
	CControlDlg(CWnd* pParent = NULL);   // 標準コンストラクター
	virtual ~CControlDlg();

	StDeviceEBus *GetDeviceEBus(void){return m_pDeviceEBus;}
	void SetDeviceEBus(StDeviceEBus *aDevice){m_pDeviceEBus = aDevice;}
private:
	StDeviceEBus *m_pDeviceEBus;
	CString m_szDisplayText;
	PvGenBrowserWnd *mDeviceWnd;
	void ShowGenWindow( PvGenBrowserWnd **aWnd, PvGenParameterArray *aParams, const CString &aTitle );
	void CloseGenWindow( PvGenBrowserWnd **aWnd );

public:

// ダイアログ データ
	enum { IDD = IDD_DIALOG_CONTROLEBUS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonSend();
	afx_msg void OnBnClickedButtonClear();
	afx_msg void OnBnClickedButtonGenicam();
	afx_msg void OnBnClickedOk();
};
