#pragma once


// CControlDlg �_�C�A���O
#include "StDeviceEBus.h"
#include <PvGenBrowserWnd.h>

class CControlDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CControlDlg)

public:
	CControlDlg(CWnd* pParent = NULL);   // �W���R���X�g���N�^�[
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

// �_�C�A���O �f�[�^
	enum { IDD = IDD_DIALOG_CONTROLEBUS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonSend();
	afx_msg void OnBnClickedButtonClear();
	afx_msg void OnBnClickedButtonGenicam();
	afx_msg void OnBnClickedOk();
};
