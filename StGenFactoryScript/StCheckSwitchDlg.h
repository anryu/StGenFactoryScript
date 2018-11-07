#pragma once

#include "stcheckbase.h"

// StCheckSwitchDlg �_�C�A���O

class StCheckSwitchDlg : public CDialogEx, public StCheckBase
{
	DECLARE_DYNAMIC(StCheckSwitchDlg)

public:
	StCheckSwitchDlg(CWnd* pParent = NULL);   // �W���R���X�g���N�^�[
	StCheckSwitchDlg( LPCTSTR szClassName, LPCTSTR szVarName, StDeviceBase *aDeviceBase, CDialogEx *aDialogEx = NULL, CWnd* pParent = NULL);   // �R���X�g���N�^�[
	virtual ~StCheckSwitchDlg();

	BOOL Execute(void){return TRUE;}


	BOOL function(LPCTSTR szFunc, LPCTSTR szArgument);

private:
	void Initialize(void);
	BOOL GetCameraSwitch(INT &nSwitch);

	INT m_nLastSwitch;
	INT m_nDurationSwitch;
	INT m_nFreqSwitch;

	INT m_nIndex;
	INT m_nOldIndex;


public:
// �_�C�A���O �f�[�^
	enum { IDD = IDD_DIALOG_CHECKSWITCH };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
